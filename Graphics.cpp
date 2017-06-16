#include "Graphics.h"
#include "GameWorld.h"
#include "UIWrappers.h"
#include "MainMenu.h"
#include "Input.h"

D3DMATERIAL9 D3D::InitMtrl( D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p )
{
    D3DMATERIAL9 mtrl;
    mtrl.Ambient = a;
    mtrl.Diffuse = d;
    mtrl.Specular = s;
    mtrl.Emissive = e;
    mtrl.Power = p;
    return mtrl;
}

D3DLIGHT9 D3D::InitDirectionalLight( D3DXVECTOR3 * direction, D3DXCOLOR * color )
{
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof( light ) );

    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Ambient = *color;
    light.Diffuse = *color;
    light.Specular = *color;
    light.Direction = *direction;

    return light;
}

D3DLIGHT9 D3D::InitSpotLight( D3DXVECTOR3 * position, D3DXVECTOR3 * direction, D3DXCOLOR * color )
{
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );

    light.Type = D3DLIGHT_SPOT;
    light.Diffuse = *color;
    light.Specular = *color;
    light.Position = *position;
    light.Direction = *direction;
    light.Range = 1000.0f;
    light.Attenuation0 = 0.0f;
    light.Attenuation1 = 0.125f;
    light.Attenuation2 = 0.0f;
    light.Phi = D3DXToRadian( 40.0f );
    light.Theta = D3DXToRadian( 20.0f );
    light.Falloff = 1.0f;

    return light;
}

D3DLIGHT9 D3D::InitPointLight( D3DXVECTOR3 * position, D3DXCOLOR * color )
{
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );

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

float D3D::Lerp( float a, float b, float t )
{
    return a - (a*t) + (b*t);
}

DWORD D3D::FtoDw( float f )
{
    return *((DWORD*)&f);
}

bool Graphics::Render( )
{
    m_CurrentState->Render( );

    //m_Mouse->Update( );
    //m_Keyboard->Update( );
    return false;
}

void Graphics::Update( )
{
    m_CurrentState->Update( );
}

Graphics::Graphics( )
{
    m_MainMenu = NULL, m_GameWorld = NULL, m_CurrentState = NULL, m_PreviousState = NULL;
    m_Device = NULL;
    m_D3DInterface = NULL;
    tex = NULL;
}


Graphics::~Graphics( )
{
    Shutdown( );
}

bool Graphics::Initialized( int height, int width, HINSTANCE hInstance )
{
    WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)D3D::MsgProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor( 0, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wc.lpszMenuName = 0;
    wc.lpszClassName = "D3DApp";

    if( !RegisterClass( &wc ) )
        return false;


    HWND hWnd = CreateWindow( "D3DApp", "Omni", WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX, 100, 100, width, height, 0, 0, hInstance, 0 );
    if( !hWnd )
        return false;

    ShowWindow( hWnd, SW_SHOW );
    UpdateWindow( hWnd );


    HRESULT hr = 0;



    D3DCAPS9 caps;

    m_D3DInterface = Direct3DCreate9( D3D_SDK_VERSION );

    m_D3DInterface->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

    if( !m_D3DInterface )
        return false;

    int vp = 0;
    if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
        vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else
        vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;



    ZeroMemory( &d3dpp, sizeof( d3dpp ) ); //clear structure for use
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

    hr = m_D3DInterface->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_Device );

    if( FAILED( hr ) )
    {
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

        hr = m_D3DInterface->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_Device );

        if( FAILED( hr ) )
        {
            m_D3DInterface->Release( );
            return false;
        }
    }

    m_MainMenu = new MainMenu( m_Device );
    m_GameWorld = new GameWorld( );
	m_Story = new Story(m_Device);
	m_Credits = new Credits(m_Device);
	m_Options = new Options(m_Device);
	m_Story->Init();
    m_GameWorld->Init( );
    m_MainMenu->Init( );
    m_CurrentState = m_MainMenu;
    //load image iformation
    D3DXIMAGE_INFO ImageInfo;
    D3DXGetImageInfoFromFile( "cursor2.png", &ImageInfo );
    m_Device->CreateOffscreenPlainSurface( ImageInfo.Height, ImageInfo.Width, ImageInfo.Format, D3DPOOL_DEFAULT, &m_Cursor, NULL );
    D3DXLoadSurfaceFromFile( m_Cursor, NULL, NULL, "cursor2.png", NULL, D3DX_FILTER_NONE, D3DCOLOR_XRGB( 255, 255, 255 ), &ImageInfo );
    m_Device->SetCursorProperties( 0, 0, m_Cursor );
    m_Device->SetCursorPosition( 0, 0, D3DCURSOR_IMMEDIATE_UPDATE );
    return TRUE;
}

void Graphics::Shutdown( )
{
    if( m_Device )
    {
        m_Device->Release( );
        m_Device = 0;
    }
    if( tex )
        delete tex;
    if( sprt )
        delete sprt;
    if( m_MainMenu )
        delete m_MainMenu;
    if( m_GameWorld )
        delete m_GameWorld;
}

//this is basically the update function for the graphics class, it passes on message data to the State Machine Update functions
void Graphics::RecvMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    m_CurrentState->ProcessMessages( msg, wParam, lParam, Data );
}

//changes the h/w of the screen when the screen is maximized or returned to windows mode
void Graphics::SetScreenRect( )
{
    m_ScreneRect.top = 0;
    m_ScreneRect.left = 0;
    m_ScreneRect.bottom = d3dpp.BackBufferHeight;
    m_ScreneRect.right = d3dpp.BackBufferWidth;
}


