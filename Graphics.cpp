#include "Graphics.h"
#include "GameWorld.h"
#include "UIWrappers.h"
#include "MainMenu.h"
#include "Input.h"

D3DMATERIAL9 D3D::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

D3DLIGHT9 D3D::InitDirectionalLight(D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color;
	light.Diffuse = *color;
	light.Specular = *color;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 D3D::InitSpotLight(D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = *color;
	light.Specular = *color;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.125f;
	light.Attenuation2 = 0.0f;
	light.Phi = D3DXToRadian(40.0f);
	light.Theta = D3DXToRadian(20.0f);
	light.Falloff = 1.0f;

	return light;
}

D3DLIGHT9 D3D::InitPointLight(D3DXVECTOR3 * position, D3DXCOLOR * color)
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.5f;
	light.Diffuse = *color * 0.5f;
	light.Specular = *color;
	light.Position = *position;
	light.Range = 100.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.125f;
	light.Attenuation2 = 0.0f;

	return light;
}

float D3D::Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

DWORD D3D::FtoDw(float f)
{
	return *((DWORD*)&f);
}

bool Graphics::Render()
{
    m_Mouse->Update( );
    m_Keyboard->Update( );

   //clear back buffer
    m_Device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 255, 255, 255 ), 1.0F, 0 );

    //begin the scene
    if( SUCCEEDED( m_Device->BeginScene( ) ) )
    {
		m_CurrentState->Render();
        //sprt->DrawTexture( tex );
        //end the scene
        m_Device->EndScene( );
    }
    //present the back buffer contents to the display
    m_Device->Present( NULL, NULL, NULL, NULL );
	return false;
}

Graphics::Graphics()
{
	m_MainMenu = NULL, m_GameWorld = NULL, m_CurrentState = NULL, m_PreviousState = NULL;
    m_Device = NULL;
    m_D3DInterface = NULL;
    tex = NULL;
    //sprt = NULL;
}


Graphics::~Graphics()
{
}

bool Graphics::Initialized(int height, int width, HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW ;
    wc.lpfnWndProc = (WNDPROC)D3D::MsgProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "D3DApp";

    if (!RegisterClass(&wc))
        return false;


    HWND hWnd = CreateWindow("D3DApp", "Omni Kitty", WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX , 100, 100, width, height, 0, 0, hInstance, 0);
    if (!hWnd)
        return false;

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);


    HRESULT hr = 0;



    D3DCAPS9 caps;

    m_D3DInterface = Direct3DCreate9( D3D_SDK_VERSION );

    m_D3DInterface->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

    if( !m_D3DInterface )
        return false;

    int vp = 0;
    if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


    D3DPRESENT_PARAMETERS d3dpp; //create a struct ot hold device ingormation

    ZeroMemory(&d3dpp, sizeof(d3dpp)); //clear structure for use
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;
    d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Windowed = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.Flags = 0;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    hr = m_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_Device);

    if (FAILED(hr)) {
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

        hr = m_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_Device);

        if (FAILED(hr)) {
            m_D3DInterface->Release();
            return false;
        }
    }

	m_Input = new Input(hInstance, hWnd);
	m_Keyboard = m_Input->CreateKeyboard();
	m_Mouse = m_Input->CreateMouse(m_Device, false);
	m_MainMenu = new MainMenu(m_Device);
	m_MainMenu->Init();
	m_GameWorld = new GameWorld();
	m_GameWorld->Init();
	m_CurrentState = m_MainMenu;
    return TRUE;
}

void Graphics::Shutdown()
{
    if (m_Device)
    {
        m_Device->Release();
        m_Device = 0;
    }
	if (m_Keyboard)
		delete m_Keyboard;
	if (m_Mouse)
		delete m_Mouse;
    if( tex )
        delete tex;
    if( sprt )
        delete sprt;
	if (m_MainMenu)
		delete m_MainMenu;
}

bool Graphics::Frame()
{
	return false;
}

void Graphics::RecvMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	m_CurrentState->Update(msg, wParam, lParam, Data);
}

void Graphics::BeginScene(float, float, float, float)
{
}

void Graphics::EndScene()
{
}


