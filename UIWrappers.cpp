#include "UIWrappers.h"

Surface::Surface()
{
   // this->SetDevice(pDevice);
    this->SetSurface(NULL);
    //m_BackBuffer = new Surface(pDevice);
    m_SourceRect = NULL;
}

Surface::~Surface()
{
    delete m_BackBuffer;
    if (m_Surface != NULL)
        m_Surface->Release();
}

HRESULT Surface::CreateSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool)
{
    return Device->CreateOffscreenPlainSurface(Width, Height, Format, Pool, &m_Surface, NULL);
}

HRESULT Surface::LoadFromFile(LPSTR Path)
{
    HRESULT Result = E_FAIL;
    ZeroMemory(&Info, sizeof(D3DXIMAGE_INFO));

    if (SUCCEEDED(D3DXGetImageInfoFromFile(Path, &Info)))
    {
        Result = CreateSurface(Info.Width, Info.Height, Info.Format, D3DPOOL_SYSTEMMEM);
    }
    if (Result == S_OK)
    {
        Result = D3DXLoadSurfaceFromFile(m_Surface, NULL, NULL, Path, NULL, D3DX_FILTER_NONE, 0, NULL);
    }
    else
        Result = E_FAIL;
    return Result;
}

HRESULT Surface::MakeBackBuffer(void)
{
    if (Device)
    {
        SafeRelease(m_BackBuffer->m_Surface);
		D3D::Release(m_BackBuffer->m_Surface);
        return Device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer->m_Surface);
    }
    return E_NOTIMPL;
}

HRESULT Surface::UpdateSurface(Surface * Source, RECT * pSourceRect, int x, int y)
{
    m_SourceRect = pSourceRect;
    if ((Device) && (Source))
    {
        POINT Point;
        Point.x = x;
        Point.y = y;

        return Device->UpdateSurface(Source->GetSurface(), pSourceRect, m_Surface, &Point);
    }
    else 
        return E_FAIL;
}

HRESULT Surface::CopySurface(IDirect3DSurface9 * CopyTo, RECT CopyFromRect, int x, int y)
{
	if ((Device) && (CopyTo))
	{
		POINT l_Point;
		l_Point.x = x;
		l_Point.y = y;
		return Device->UpdateSurface(m_Surface, &CopyFromRect, CopyTo, &l_Point);
	}
	return E_FAIL;
}

void Surface::Render(void)
{
    m_BackBuffer->UpdateSurface(this, m_SourceRect, 0, 0);
}
///---------------------------------------------------------------------------------Texture wrapper-----------------------------------------------------------------------
Texture::Texture()
{
    D3DXVECTOR2 Vec;
    D3DXVECTOR2 VecS ;
    Vec.x = 0;
    Vec.y = 0;
    VecS.x = 1;
    VecS.y = 1;

    //	SetDevice(pDevice);
    SetTexture(NULL);
    SetRotation(0.0f);
    SetRotationCenter(Vec);
    SetScaling(VecS);
    SetTranslation(Vec);
	m_Info;
}

Texture::Texture(LPSTR Path, D3DXVECTOR2 RotationCenter, FLOAT Rotation, D3DXVECTOR2 Translation, D3DXVECTOR2 Scaling)
{
    //	SetDevice(pDevice);
	LoadFromFile(Path);
    InitTexture( RotationCenter,  Rotation,  Translation,  Scaling);
}

void Texture::InitTexture( D3DXVECTOR2 RotationCenter, FLOAT Rotation, D3DXVECTOR2 Translation, D3DXVECTOR2 Scaling)
{
    SetRotation(Rotation);
    SetRotationCenter(RotationCenter);
    SetScaling(Scaling);
    SetTranslation(Translation);
}

Texture::~Texture()
{
    if (m_Texture != NULL)
        SafeRelease(m_Texture);
}

HRESULT Texture::LoadFromFile(LPSTR Path)
{
    if (m_Texture != NULL)
        m_Texture->Release();
	if (SUCCEEDED(D3DXGetImageInfoFromFile(Path, &m_Info)))
	{
	
		m_SrcRect.top = 0;
		m_SrcRect.left = 0;
		m_SrcRect.bottom = m_Info.Height;
		m_SrcRect.right = m_Info.Width;
		return D3DXCreateTextureFromFileEx(Device, Path, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT, 0, m_Info.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_Texture);
	}

	return E_FAIL;
}

///----------------------------------------------------------------------Sprite Wrapper-------------------------------------------------------------------------------

Sprite::Sprite()
{
    //	SetDevice(pDevice);
    D3DXCreateSprite(Device, &m_Sprite);
}

Sprite::~Sprite()
{
    if (m_Sprite != NULL)
        SafeRelease(m_Sprite);
}

HRESULT Sprite::DrawTexture(Texture* Tex, D3DXVECTOR2 Pos)
{
    if ((Tex->GetTexture() != NULL) && (m_Sprite != NULL))
    {
        D3DXMATRIX Mat;
        D3DXMatrixTransformation2D(&Mat, NULL, 0, &Tex->GetScaling(), &Tex->GetRotationCenter(), Tex->GetRotation(), &Pos);

        m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
        m_Sprite->SetTransform(&Mat);
        HRESULT Result = m_Sprite->Draw(Tex->GetTexture(), &Tex->GetRect(), NULL, NULL, 0xFFFFFFFF);
        m_Sprite->End();
        return Result;
    }
    else
        return E_FAIL;
}

HRESULT Sprite::DrawBackground(Texture* Tex)
{
	if ((Tex->GetTexture() != NULL) && (m_Sprite != NULL))
	{
		D3DXMATRIX Mat;
		D3DXVECTOR2 m_scale;
		ZeroMemory(&m_scale, sizeof(D3DXVECTOR2));
		m_scale.x = (FLOAT(Tex->GetRect().right)/ FLOAT(Tex->GetWidth()));
		m_scale.y = (FLOAT(Tex->GetRect().bottom)/ FLOAT(Tex->GetHeight()));
		D3DXMatrixTransformation2D(&Mat, NULL, 0, &m_scale, &Tex->GetRotationCenter(), Tex->GetRotation(), &Tex->GetTranslation());

		m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_Sprite->SetTransform(&Mat);
		HRESULT Result = m_Sprite->Draw(Tex->GetTexture(), NULL, NULL, NULL, 0xFFFFFFFF);
		m_Sprite->End();
		return Result;
	}
	else
		return E_FAIL;
}
