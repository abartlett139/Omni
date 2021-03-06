#include "Input.h"
#include <string>
#define KEYDOWN(name,key) (name[key] & 0x80)



///------------------------------------------------------------------Input------------------------------------------------------------------------
Input::Input(HINSTANCE hinst, HWND hWnd)
{
    m_pInput = NULL;
    m_hWnd = hWnd;
    DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL);
}

Input::~Input()
{
    if (m_pInput != NULL)
        m_pInput->Release();
}

Keyboard * Input::CreateKeyboard()
{
	Keyboard* l_keyboard = NULL;

	if (m_pInput)
	{
		l_keyboard = new Keyboard(m_pInput, m_hWnd);
	}
    return l_keyboard;
}

Mouse * Input::CreateMouse(LPDIRECT3DDEVICE9 pDevice, bool Exclusive)
{
	Mouse* l_mouse = NULL;
	if (m_pInput)
	{
		l_mouse = new Mouse(pDevice, m_pInput, m_hWnd, Exclusive);
	}
    return l_mouse;
}
///----------------------------------------------------------------------Keyboard--------------------------------------------------------------------
Keyboard::Keyboard(LPDIRECTINPUT8 pInput, HWND hWnd)
{
    HRESULT Result = E_FAIL;
    m_pInputDevice = NULL;
    Result = pInput->CreateDevice(GUID_SysKeyboard, &m_pInputDevice, NULL);
    if (SUCCEEDED(Result))
    {
        Result = m_pInputDevice->SetDataFormat(&c_dfDIKeyboard);
        if (FAILED(Result))
        {
            SafeRelease(m_pInputDevice);
            return;
        }
        Result = m_pInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(Result))
        {
            SafeRelease(m_pInputDevice);
            return;
        }
        memset(m_KeyState, 0, 256 * sizeof(char));
    }
}

Keyboard::~Keyboard()
{
    m_pInputDevice->Unacquire();
    SafeRelease(m_pInputDevice);
}

bool Keyboard::IsKeyPressed(int Key)
{
    if (m_pInputDevice)
    {
        if (m_KeyState[Key] & 0x80)
            return true;
    }
    return false;
}

HRESULT Keyboard::Update()
{
    HRESULT Result = E_FAIL;
    if (m_pInputDevice)
    {
        Result = m_pInputDevice->Acquire();
        if (FAILED(Result))
            return Result;
        Result = m_pInputDevice->GetDeviceState(sizeof(m_KeyState), (LPVOID)&m_KeyState);
        if (FAILED(Result))
            return Result;
    }
    return S_OK;
}
///----------------------------------------------------------------Mouse-------------------------------------------------------------------------------
Mouse::Mouse( LPDIRECT3DDEVICE9 pDevice, LPDIRECTINPUT8 pInput, HWND hWnd,
              bool Exclusive/*, D3DDISPLAYMODE Mode*/ ): m_pInputDevice( NULL )
{
    Device = NULL;
    //Initial cursor position
    m_iX = 0;
    m_iY = 0;

    if( pInput&&pDevice )
    {
        HRESULT Result = E_FAIL;
        Device = pDevice;
        Result = pInput->CreateDevice( GUID_SysMouse, &m_pInputDevice, NULL );
        if( FAILED( Result ) )
            return;
        Result = m_pInputDevice->SetDataFormat( &c_dfDIMouse );
        if( FAILED( Result ) )
        {
            SafeRelease( m_pInputDevice );
            return;
        }
        if( Exclusive )
        {
            Result = m_pInputDevice->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND );
        }
        else
        {
            Result = m_pInputDevice->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
        }
        if( FAILED( Result ) )
        {
            SafeRelease( m_pInputDevice );
            return;
        }

	      D3DXIMAGE_INFO ImageInfo;
        //load image iformation
        D3DXGetImageInfoFromFile( "textures/cursor.png", &ImageInfo );
        Device->CreateOffscreenPlainSurface( ImageInfo.Height, ImageInfo.Width, ImageInfo.Format, D3DPOOL_DEFAULT, &m_Cursor, NULL );
        D3DXLoadSurfaceFromFile( m_Cursor, NULL, NULL, "textures/cursor.png", NULL, D3DX_FILTER_NONE, D3DCOLOR_XRGB( 255, 255, 255 ), &ImageInfo );
        Device->SetCursorProperties( 0, 0, m_Cursor );
        Device->SetCursorPosition( 0, 0, D3DCURSOR_IMMEDIATE_UPDATE );
        m_Changed = false;
        m_Buttons = false;
    }
	
}

Mouse::~Mouse( )
{
    if( m_pInputDevice )
    {
        m_pInputDevice->Unacquire( );
        SafeRelease( m_pInputDevice );
    }
	SafeRelease(m_Cursor);
	//delete m_Surf;
}

HRESULT Mouse::Update( )
{
	HRESULT Result = E_FAIL;
	//long OldX, OldY;
	bool Pressed = this->IsButtonPressed(0);
	m_Changed = false;

	if (m_pInputDevice)
	{
        if( SUCCEEDED( m_pInputDevice->Acquire( ) ) )
        {
            Result = m_pInputDevice->GetDeviceState( sizeof( DIMOUSESTATE ), (void*)&m_State );
            if( FAILED( Result ) )
                return Result;
        }
        else
            return Result;
		Result = m_pInputDevice->Poll();
		if (FAILED(Result))
			return Result;
		if (this->IsButtonPressed(0) != Pressed)
			m_Buttons = true;
		else
			m_Buttons = false;
		m_iX += m_State.lX;
		m_iY += m_State.lY;
	}
    return Result;
}

LONG Mouse::GetXPos( )
{
    return m_iX;
}

LONG Mouse::GetYPos( )
{
    return m_iY;
}

bool Mouse::IsButtonPressed( int Button )
{
	if (KEYDOWN(m_State.rgbButtons, Button))
		return true;
	else
		return false;
}


HRESULT Mouse::SetMouseCursor( char * FilePath, UINT x, UINT y, int Type )
{
    return E_NOTIMPL;
}

void Mouse::SetCursorImage( int Type )
{
	Device->SetCursorProperties(m_iX, m_iX, m_Cursor);
}

void Mouse::SetCursorPosition( int x, int y )
{
	Device->SetCursorPosition(x, y, 0);
}

HRESULT Mouse::SetCursorVisible( bool Show )
{
    return Device->ShowCursor(Show);
}
