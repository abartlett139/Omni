#include "MainMenu.h"
#include "Input.h"
#include "UIControls.h"
#include "UIWrappers.h"	
#include "FileIOHelper.h"
MainMenu::MainMenu( LPDIRECT3DDEVICE9 Device )
{
    m_Init = false;
    m_Device = Device;
    m_Sprite = NULL;
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
    if( wc )
        delete wc;
}

bool MainMenu::Init( )
{
    if( !m_Init )
    {
        wc = new WindowControl( NULL, NULL );
        m_Sprite = graphics.m_Sprite;
        m_Background = new Texture( m_Device, "menuBackground.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        m_ButtonDefault = new Texture( m_Device, "button.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        m_ButtonOver = new Texture( m_Device, "buttonOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 } );
        wc->SetSprite( m_Sprite );
        wc->SetTexture( m_Background );
        wc->SetRect( graphics.m_ScreneRect );

        ButtonControl* temp;
        temp = new ButtonControl( wc->GetThis( ), 1, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2)- (m_ButtonDefault->GetWidth()/2)),(FLOAT)100 }, m_Device );
        temp->SetTextures( m_ButtonDefault, m_ButtonOver );
        temp->SetCaption( "Play Game" );
        temp->SetChangeState( graphics.m_GameWorld );
        wc->AddChildControl( temp );
        temp = NULL;

		temp = new ButtonControl(wc->GetThis(), 2, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)300 }, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("The Story");
		temp->SetChangeState(graphics.m_Story);
		wc->AddChildControl(temp);
		temp = NULL;

        temp = new ButtonControl( wc->GetThis( ), 2, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)500 }, m_Device );
        temp->SetTextures( m_ButtonDefault, m_ButtonOver );
        temp->SetCaption( "Credits" );
        temp->SetChangeState( graphics.m_Credits );
        wc->AddChildControl( temp );
		temp = NULL;

	
		temp = new ButtonControl(wc->GetThis(), 2, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)700 }, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Options");
		temp->SetChangeState(graphics.m_Options);
		wc->AddChildControl(temp);
	}
    return m_Init;
}

void MainMenu::Enter( )
{
    //if( graphics.m_PreviousState != graphics.m_CurrentState )
    //    graphics.m_PreviousState->OnLostDevice();
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

void MainMenu::OnLostDevice( )
{
    m_Sprite = NULL;
    m_Init = false;
    //if( m_Background )
    //    delete m_Background;
    //if( m_ButtonDefault )
    //    delete m_ButtonDefault;
    //if( m_ButtonOver )
    //    delete m_ButtonOver;
    if( wc )
        delete wc;
}

void MainMenu::ProcessMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    //wc->PostMessage(WM_PAINT, wParam, lParam, NULL);
        wc->PostMessage( msg, wParam, lParam, NULL );
}

void MainMenu::Exit( GameState * nextState )
{
    graphics.m_PreviousState=graphics.m_CurrentState;
    graphics.m_CurrentState = nextState;
    graphics.m_CurrentState->Enter( );
}
//---------------------------------------------------------------------------Story---------------------------------------------------------------------
Story::Story(LPDIRECT3DDEVICE9 Device)
{
	m_Init = false;
	m_Device = Device;
	m_Sprite = NULL;
	m_Sprite = new Sprite(m_Device);
}

Story::~Story()
{
	if (m_Background)
		delete m_Background;
	if (m_ButtonDefault)
		delete m_ButtonDefault;
	if (m_ButtonOver)
		delete m_ButtonOver;
	if (m_TextBox)
		delete m_TextBox;
	if (wc)
		delete wc;
}

bool Story::Init()
{
	if (!m_Init)
	{
		m_Background = new Texture(m_Device, "menuBackground.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_ButtonDefault = new Texture(m_Device, "button.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_ButtonOver = new Texture(m_Device, "buttonOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_TextBox = new Texture(m_Device, "Text.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		//m_TextBox->SetTranslation(D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_TextBox->GetWidth() / 2)),(FLOAT)100 });
		wc = new WindowControl(NULL, NULL);
		wc->SetSprite(m_Sprite);
		wc->SetTexture(m_Background);
		wc->SetRect(graphics.m_ScreneRect);


		ButtonControl* temp = NULL;
		temp = new ButtonControl(wc->GetThis(), 1, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth * 0.85f) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)700 }, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Play Game");
		temp->SetChangeState(graphics.m_GameWorld);
		wc->AddChildControl(temp);
		temp = NULL;

		temp = new ButtonControl(wc->GetThis(), 2, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth *0.15f) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)700}, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Main Menu");
		temp->SetChangeState(graphics.m_MainMenu);
		wc->AddChildControl(temp);

		m_File = new FileIOHelper(m_Device, "UIFile.txt", m_TextBox->GetRect(), "georgia.ttf", 14, 8, FW_THIN, DT_LEFT);
		m_File->getRecord("Test");
		m_Init = true;
	}
	return m_Init;
}

void Story::Enter()
{
    //if( graphics.m_PreviousState != graphics.m_CurrentState )
    //    graphics.m_PreviousState->OnLostDevice( );
    if (!m_Init)
	{
		m_Init = Init();
	}
}

void Story::Render()
{
	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0);
	m_Device->BeginScene();

	//m_Sprite->DrawTexture(m_TextBox, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_TextBox->GetWidth() / 2)),(FLOAT)100 });
 //   m_File->Render( m_Sprite->GetSprite( ) );
    wc->OnRender( );
    m_Device->EndScene();
	m_Device->Present(NULL, NULL, NULL, NULL);
}

void Story::Update()
{
	return;
}

void Story::OnLostDevice( )
{
    //if( m_Background )
    //    delete m_Background;
    //if( m_ButtonDefault )
    //    delete m_ButtonDefault;
    //if( m_ButtonOver )
    //    delete m_ButtonOver;
    m_Init = false;
    if( wc )
        delete wc;
}

void Story::Exit(GameState * nextState)
{
    graphics.m_PreviousState = graphics.m_CurrentState;
    graphics.m_CurrentState = nextState;
    graphics.m_CurrentState->Enter( );
}

void Story::ProcessMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	wc->PostMessage(msg, wParam, lParam, NULL);
}
//------------------------------------------------------------------------------------Credits-----------------------------------------------------------
Credits::Credits(LPDIRECT3DDEVICE9 Device)
{
	m_Init = false;
	m_Device = Device;
	m_Sprite = NULL;
	m_Sprite = new Sprite(m_Device);
}

Credits::~Credits()
{
	if (m_Sprite)
		delete m_Sprite;
	if (m_Background)
		delete m_Background;
	if (m_ButtonDefault)
		delete m_ButtonDefault;
	if (m_TextBox)
		delete m_TextBox;
	if (wc)
		delete wc;
}

bool Credits::Init()
{
	if (!m_Init)
	{
		m_Background = new Texture(m_Device, "menuBackground.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_ButtonDefault = new Texture(m_Device, "button.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_ButtonOver = new Texture(m_Device, "buttonOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_TextBox = new Texture(m_Device, "Text.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		//m_TextBox->SetTranslation(D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_TextBox->GetWidth() / 2)),(FLOAT)100 });
		wc = new WindowControl(NULL, NULL);
		wc->SetSprite(m_Sprite);
		wc->SetTexture(m_Background);
		wc->SetRect(graphics.m_ScreneRect);


		ButtonControl* temp;
		temp = new ButtonControl(wc->GetThis(), 1, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth * 0.75f) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)700 }, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Play Game");
		temp->SetChangeState(graphics.m_GameWorld);
		wc->AddChildControl(temp);
		temp = NULL;

		temp = new ButtonControl(wc->GetThis(),2, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth *0.25f) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)700 }, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Main Menu");
		temp->SetChangeState(graphics.m_MainMenu);
		wc->AddChildControl(temp);

		m_File = new FileIOHelper(m_Device, "UIFile.txt", m_TextBox->GetRect(), "georgia.ttf", 14, 8, FW_THIN, DT_LEFT);
		m_File->getRecord("Test");
		m_Init = true;
	}
	return m_Init;
}

void Credits::Enter()
{
    //if( graphics.m_PreviousState != graphics.m_CurrentState )
    //    graphics.m_PreviousState->OnLostDevice( );
    if (!m_Init)
	{
		m_Init = Init();
	}
}

void Credits::Render()
{
	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0);
	m_Device->BeginScene();

	m_File->Render(m_Sprite->GetSprite());
	m_Sprite->DrawTexture(m_TextBox, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_TextBox->GetWidth() / 2)),(FLOAT)100 });
    wc->OnRender( );
    m_Device->EndScene();
	m_Device->Present(NULL, NULL, NULL, NULL);
}

void Credits::Update()
{
	return;
}

void Credits::OnLostDevice( )
{
    m_Sprite = NULL;
    //if( m_Background )
    //    delete m_Background;
    //if( m_ButtonDefault )
    //    delete m_ButtonDefault;
    //if( m_ButtonOver )
    //    delete m_ButtonOver;
    m_Init = false;
    if( m_TextBox )
        delete m_TextBox;
    if( wc )
        delete wc;
}

void Credits::Exit(GameState * nextState)
{
    graphics.m_PreviousState = graphics.m_CurrentState;
    graphics.m_CurrentState = nextState;
    graphics.m_PreviousState->OnLostDevice( );
    graphics.m_CurrentState->Enter( );
}

void Credits::ProcessMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	wc->PostMessage(msg, wParam, lParam, NULL);
}
//-------------------------------------------------------------------------------Options--------------------------------------------------------
Options::Options(LPDIRECT3DDEVICE9 Device)
{
	m_Init = false;
	m_Device = Device;
	m_Sprite = NULL;
	m_Sprite = new Sprite(m_Device);
}

Options::~Options()
{
	if (m_Sprite)
		delete m_Sprite;
	if (m_Background)
		delete m_Background;
	if (m_ButtonDefault)
		delete m_ButtonDefault;
	if (m_ButtonOver)
		delete m_ButtonOver;
	if (wc)
		delete wc;
}

bool Options::Init()
{
	if (!m_Init)
	{
		m_Background = new Texture(m_Device, "menuBackground.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_ButtonDefault = new Texture(m_Device, "button.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_ButtonOver = new Texture(m_Device, "buttonOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_SlideBar = new Texture(m_Device, "slideBar.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_Slider = new Texture(m_Device, "sliderHandle.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		m_SliderOver = new Texture(m_Device, "sliderHandleOver.png", D3DXVECTOR2{ 0,0 }, 0, D3DXVECTOR2{ 0,0 }, D3DXVECTOR2{ 1,1 });
		//m_TextBox->SetTranslation(D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_TextBox->GetWidth() / 2)),(FLOAT)100 });
		wc = new WindowControl(NULL, NULL);
		wc->SetSprite(m_Sprite);
		wc->SetTexture(m_Background);
		wc->SetRect(graphics.m_ScreneRect);


		ButtonControl* temp;
		temp = new ButtonControl(wc->GetThis(), 1, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth * 0.75f) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)800 }, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Play Game");
		temp->SetChangeState(graphics.m_GameWorld);
		wc->AddChildControl(temp);
		temp = NULL;

		temp = new ButtonControl(wc->GetThis(), 2, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth *0.25f) - (m_ButtonDefault->GetWidth() / 2)),(FLOAT)800 }, m_Device);
		temp->SetTextures(m_ButtonDefault, m_ButtonOver);
		temp->SetCaption("Main Menu");
		temp->SetChangeState(graphics.m_MainMenu);
		wc->AddChildControl(temp);

		SlideBar* tempS;
		tempS = new SlideBar(wc->GetThis(), 2, D3DXVECTOR2{ (FLOAT)((graphics.d3dpp.BackBufferWidth / 2) - (m_SlideBar->GetWidth() / 2)),(FLOAT)700 }, m_Device);
		tempS->SetTextures(m_SlideBar, m_Slider, m_SliderOver);
		tempS->SetCaption("Volume");
		wc->AddChildControl(tempS);
		m_Init = true;
	}
	return m_Init;
}

void Options::Enter()
{
    //if( graphics.m_PreviousState != graphics.m_CurrentState )
    //    graphics.m_PreviousState->OnLostDevice( );
    if (!m_Init)
	{
		m_Init = Init();
	}
}

void Options::Render()
{
	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0);
	m_Device->BeginScene();

	wc->OnRender();

	m_Device->EndScene();
	m_Device->Present(NULL, NULL, NULL, NULL);
}

void Options::Update()
{
	return;
}

void Options::OnLostDevice( )
{
    m_Sprite = NULL;
    //if( m_Background )
    //    delete m_Background;
    //if( m_ButtonDefault )
    //    delete m_ButtonDefault;
    //if( m_ButtonOver )
    //    delete m_ButtonOver;
    m_Init = false;
    if( wc )
        delete wc;
}

void Options::Exit(GameState * nextState)
{
    graphics.m_PreviousState = graphics.m_CurrentState;
    graphics.m_CurrentState = nextState;
    graphics.m_PreviousState->OnLostDevice( );
    graphics.m_CurrentState->Enter( );
}

void Options::ProcessMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
	wc->PostMessage(msg, wParam, lParam, NULL);
}
