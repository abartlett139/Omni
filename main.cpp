#include "GameWorld.h"
#include "GameTimer.h"
#include <time.h>
#include "Graphics.h"

//Hi Amy, how is it going?

Graphics graphics;
bool g_init = false;
GameTimer timer;
SoundEngine *soundEngine = new SoundEngine( );

FILE* console_log;
HRESULT hr = NULL;
///WinProc
///this class is declared in the D3D namspace(this function must be declared globally,
///so it must be a part of the namespace and not a member of the graphics class),
///then when you fill out the WNDCLASS class, a pointer is made to the function
///wc.lpfnWndProc = (WNDPROC)D3D::MsgProc; (like this)
///this is the fuction that WinMain dispatches messages to.

LRESULT CALLBACK D3D::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

    switch( msg )
    {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    case WM_SIZE:
        switch( wParam )
        {
        case SIZE_MAXIMIZED:///you know what this does since I basically stole this code from your final project
            if( !(graphics.d3dpp.Windowed) )
                break;
            SetWindowLong( hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
            graphics.d3dpp.Windowed = false;
            graphics.d3dpp.BackBufferWidth = GetSystemMetrics( SM_CXSCREEN );
            graphics.d3dpp.BackBufferHeight = GetSystemMetrics( SM_CYSCREEN );
            graphics.d3dpp.BackBufferCount = 1;
            graphics.d3dpp.Windowed = false;
            graphics.SetScreenRect( );
            //graphics.m_Cursor->Release( );
            //graphics.m_CurrentState->OnLostDevice();
            //if( FAILED( graphics.GetDevice( )->Reset( &graphics.d3dpp ) ) )
            //    MessageBox( NULL, "reset d3dpp failed", NULL, NULL );
            //graphics.m_CurrentState->Enter( );
            break;
        case SIZE_RESTORED:///ditto
            //	set windowed to true
            graphics.d3dpp.BackBufferWidth = screenWidth;
            graphics.d3dpp.BackBufferHeight = screenHeight;
            graphics.SetScreenRect( );
            graphics.d3dpp.Windowed = true;
            break;
        }break;
    case WM_SETCURSOR:
       // SetCursor( NULL );
        //return graphics.GetDevice( )->ShowCursor( true );
        break;
    case WM_KEYUP:
    case WM_KEYDOWN:
#ifndef NDEBUG
        printf( "Key Event: %d, %d\n", (int)wParam, (int)lParam );
#endif
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
        if( wParam == VK_ESCAPE )
            DestroyWindow( hWnd );
        printf( "mouse x: %f  \n mouse y: %f \n", (float)LOWORD( lParam ), (float)HIWORD( lParam ) );
        ///The graphics class receives the messages and sends them on to the state machine
        ///in the UIBase class, this is done in the PostMessage function.  All of the classes
        ///in the UIControl files are derived from the UIBase class.  The functions called within
        ///the PostMessage function are pure virtual functions(ex.OnMouseUp(msg, LOWORD(lParam), HIWORD(lParam));),
        ///and are implemented by the derived classes.
        if( g_init )
            graphics.RecvMessages( msg, wParam, lParam, NULL );
        return 0;
    default:
        break;

    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------//
//	WinMain
//-----------//
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevinstances, LPSTR cmdLine, int showCmd )
{

    // cast the state types from generic GameState to their true state
    // reinterpret_cast<GameWorld&>(gameWorld);
    srand( (unsigned int)time( NULL ) );

    // these next few lines create and attach a console
    // to this process.  note that each process is only allowed one console.
#ifndef NDEBUG
    AllocConsole( );
    AttachConsole( GetCurrentProcessId( ) );
    freopen_s( &console_log, "CON", "w", stdout );

    printf( "Omni Debug Console v0.0.1!\n" );
#endif

    g_init = graphics.Initialized( screenHeight, screenWidth, hInstance );///initialize the graphics class 

    //	initialize the sound engine
    soundEngine->Initialize( );

    //	message for loop
    MSG msg;
    ZeroMemory( &msg, sizeof( MSG ) );
    while( msg.message != WM_QUIT )
    {

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) && !IsDialogMessage( NULL, &msg ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );///Displatches messages to the WndProc function
        }
        else
        {
            timer.Tick( );
            graphics.Update( );///graphics update function
            graphics.Render( );///graphics class render call, this will call render for the current state
        }

    }

    fclose( console_log );

    return (int)msg.wParam;
}

///move on to the graphics class
