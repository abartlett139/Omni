#include "MainMenu.h"
#include "Input.h"
#include "UIControls.h"
#include "UIWrappers.h"	

MainMenu::MainMenu( LPDIRECT3DDEVICE9 Device )
{
    m_Init = false;
    m_Device = Device;
    m_Sprite = NULL;
    m_Sprite = new Sprite( m_Device );
}

MainMenu::~MainMenu( )
{
    if( m_Sprite )
        delete m_Sprite;
    if( m_Background )
        delete m_Background;
    if( m_ButtonDefault )
        delete m_ButtonDefault;
    if( m_ButtonOver )
        delete m_ButtonOver;
    if( m_ButtonOver )
        delete m_ButtonOver;
    if( wc )
        delete wc;
}

bool MainMenu::Init( )
{
    if( !m_Init )
    {
        m_Background = new Texture( m_Device, "menuBackground.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        m_ButtonDefault = new Texture( m_Device, "button.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        m_ButtonOver = new Texture( m_Device, "buttonOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        m_SliderOver = new Texture( m_Device, "sliderHandleOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        m_SliderDefault = new Texture( m_Device, "sliderHandle.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        m_SlideBar = new Texture( m_Device, "slideBar.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        wc = new WindowControl( NULL, NULL );
        wc->SetSprite( m_Sprite );
        wc->SetTexture( m_Background );
        wc->SetRect( graphics.m_ScreneRect );
        ButtonControl* temp;
        temp = new ButtonControl( wc->GetThis( ), 1, D3DXVECTOR2{ 100,100 }, m_Device );
        temp->SetTextures( m_ButtonDefault, m_ButtonOver );
        temp->SetCaption( "Play Game" );
        temp->SetChangeState( graphics.m_GameWorld );
        wc->AddChildControl( temp );
        temp = NULL;

        temp = new ButtonControl( wc->GetThis( ), 2, D3DXVECTOR2{ 100,600 }, m_Device );
        temp->SetTextures( m_ButtonDefault, m_ButtonOver );
        temp->SetCaption( "Credits" );
        temp->SetChangeState( graphics.m_GameWorld );
        wc->AddChildControl( temp );

        SlideBar* tempSlide;
        tempSlide = new SlideBar( wc->GetThis( ), 3, D3DXVECTOR2{ 100,300 }, m_Device );
        tempSlide->SetTextures( m_SlideBar, m_SliderDefault, m_SliderOver );
        tempSlide->SetCaption( "Volume" );
        wc->AddChildControl( tempSlide );
        m_Init = true;
    }
    return m_Init;
}

void MainMenu::Enter( )
{
    if( !m_Init )
    {
        m_Init = Init( );
    }
}

void MainMenu::Render( )
{
    m_Device->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0 );
    m_Device->BeginScene( );

    wc->OnRender( );

    m_Device->EndScene( );
    m_Device->Present( NULL, NULL, NULL, NULL );
}

void MainMenu::Update( )
{

    return;
}

void MainMenu::ProcessMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    //wc->PostMessage(WM_PAINT, wParam, lParam, NULL);
    wc->PostMessage( msg, wParam, lParam, NULL );
}

void MainMenu::Exit( GameState * nextState )
{
    graphics.m_CurrentState = graphics.m_PreviousState;
    graphics.m_CurrentState = nextState;
    graphics.m_CurrentState->Enter( );
}

