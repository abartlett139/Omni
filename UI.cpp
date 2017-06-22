#include "UI.h"
#include "Graphics.h"
UIDevice::UIDevice(IDirect3DDevice9 * Dev) : dev{ Dev }
{
	D3DXCreateFont(Dev, 50, 15, FW_BOLD, 0, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial"),
		&font);
}

UIDevice::~UIDevice()
{
	if(font)
		font->Release();
}

void UIDevice::Draw(std::string str, D3DXVECTOR2 pos, drawType dt)
{
	RECT scrR;
	scrR.top = (LONG)pos.y;
	scrR.bottom = (LONG)(pos.y + 100);
	DWORD format = 0;
	switch (dt)
	{
	case centered:
		format |= DT_CENTER;
		scrR.left = (LONG)(pos.x -500);
		scrR.right = (LONG)(pos.x + 500);
		break;
	case left:
		format |= DT_LEFT;
		scrR.left = (LONG)(pos.x);
		scrR.right = (LONG)(pos.x + 300);
		break;
	case right:
		format |= DT_RIGHT;
		scrR.left = (LONG)(pos.x - 300);
		scrR.right = (LONG)(pos.x);
		break;
	}
	font->DrawText(NULL, str.c_str(), -1, &scrR, format, D3D::RED);
}
