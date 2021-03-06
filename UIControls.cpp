#include "UIControls.h"
#include "Graphics.h"

WindowControl::WindowControl( UIBase * parent, int vecPos ): UIBase( parent, vecPos )
{
    if( parent == NULL )
        m_IsParentWindow = true;
    m_IsMouseDown = false;
    m_MouseClickPos = { 0,0 };
}

WindowControl::~WindowControl( )
{
    if( GetTexture( ) )
        delete GetTexture( );

}

bool WindowControl::OnRender( )
{
    if( m_IsParentWindow )
    {
        m_Texture->SetRect( graphics.m_ScreneRect );
    }
    D3DXVECTOR2 l_ControlAbsPos;
    l_ControlAbsPos.x = 0;
    l_ControlAbsPos.y = 0;
    GetAbsolutePosition( &l_ControlAbsPos );
    GetTexture( )->SetTranslation( l_ControlAbsPos );
    m_Sprite->DrawBackground( GetTexture( ) );
    GetTexture( )->SetTranslation( D3DXVECTOR2{ 0,0 } );

    for( int i = 1; i < m_vControl.size( ); i++ )
    {
        m_vControl[ i ]->OnRender( );
    }

    return true;
}

void WindowControl::OnMouseDown( int Button, int x, int y )
{
    if( !m_IsParentWindow )
    {
        m_MouseClickPos = { float( x ),float( y ) };
        D3DXVECTOR2 Abs{ 0,0 };
        GetAbsolutePosition( &Abs );
        D3DXVec2Subtract( &m_MouseClickPos, &D3DXVECTOR2{ float( x ),float( y ) }, &Abs );
        m_IsMouseDown = true;
    }
}

void WindowControl::OnMouseMove( int x, int y )
{
    if( m_IsMouseDown && !m_IsParentWindow )
    {
        D3DXVECTOR2 Abs{ 0,0 }, Rel{ 0,0 };
        GetAbsolutePosition( &Abs );
        D3DXVec2Subtract( &Rel, &D3DXVECTOR2{ float( x ),float( y ) }, &Abs );
        D3DXVec2Subtract( &m_Position, &Rel, &m_MouseClickPos );
    }
}

void WindowControl::OnMouseUp( int Button, int x, int y )
{
    if( m_IsMouseDown && !m_IsParentWindow )
    {
        m_IsMouseDown = false;
    }
}

void WindowControl::OnSetFocus( )
{
}

void WindowControl::OnLostFocus( )
{
}

void WindowControl::OnKeyDown( WPARAM Key, LPARAM Extended )
{
}

void WindowControl::OnKeyUp( WPARAM Key, LPARAM Extended )
{
}

bool WindowControl::LoadCanvasFromFile( char * File )
{
    D3DXIMAGE_INFO l_ImageInfo;
    if( SUCCEEDED( D3DXGetImageInfoFromFile( File, &l_ImageInfo ) ) )
    {
        SetWidthHeight( l_ImageInfo.Width, l_ImageInfo.Height );
        if( SUCCEEDED( GetTexture( )->LoadFromFile( File ) ) )
            return true;
    }
    return false;
}
///----------------------------------------------------------------Label--------------------------------------------------------

LabelControl::LabelControl( UIBase * parent, int vecPos, LOGFONT Font, RECT Rect): UIBase( parent, vecPos ), m_Rect( Rect )
{
    D3DXCreateFont( Device, Font.lfWidth, Font.lfHeight, Font.lfWeight, 1, Font.lfItalic,
                    Font.lfCharSet, Font.lfOutPrecision, Font.lfQuality, Font.lfPitchAndFamily, Font.lfFaceName, &m_Font );
    strcpy_s( m_Caption, " " );
    m_Color = D3DCOLOR_XRGB( 0, 0, 0 );
    m_ColorOver = D3DCOLOR_XRGB( 255, 0, 0 );
    m_ColorNULL = D3DCOLOR_XRGB( 0, 0, 0 );
    m_Format = DT_CENTER | DT_VCENTER;
    m_ButtonLabel = false;
}

LabelControl::~LabelControl( )
{
    if( m_Font )
        m_Font->Release( );
}

bool LabelControl::OnRender( )
{
    if( m_Font )
    {
        RECT l_Temp{ (LONG)m_Position.x,(LONG)m_Position.y, (LONG)m_Position.x + (LONG)GetWidth( ), (LONG)m_Position.y + LONG( GetHeight( ) - 40 ) };
        m_Font->DrawText( NULL, m_Caption, (INT)strlen( m_Caption ), &l_Temp, m_Format, m_Color );
    }
    return true;
}

void LabelControl::OnMouseDown( int Button, int x, int y )
{
}

void LabelControl::OnMouseMove( int x, int y )
{
    if( CursorIntersect( float( x ), float( y ) ) )
    {
        m_Color = m_ColorOver;
    }
    else
        m_Color = m_ColorNULL;
}

void LabelControl::OnMouseUp( int Button, int x, int y )
{
}

void LabelControl::SetCaption( char * Caption )
{
    if( Caption && *Caption )
    {
        strcpy_s( m_Caption, Caption );
    }
    else
        strcpy_s( m_Caption, " " );
}
///--------------------------------------------------------------Buttons-----------------------------------------
ButtonControl::ButtonControl( UIBase * parent, int vecPos, D3DXVECTOR2 Position ):UIBase( parent, vecPos )
{
    m_Over = false;
    m_Caption = NULL;
    m_Position = Position;
    m_OverTex = NULL;
    m_DefaultTex = NULL;
    m_ChangeState = NULL;
}

ButtonControl::~ButtonControl( )
{
    DeleteObject( m_Font );
    if( m_Caption )
        delete m_Caption;
}

bool ButtonControl::OnRender( )
{
    m_Sprite->DrawTexture( m_Texture, m_Position );
    if( m_Caption )
    {
        m_Caption->OnRender( );
    }
    return false;
}

void ButtonControl::OnMouseDown( int Button, int x, int y )
{
    if( CursorIntersect( (FLOAT)x, (FLOAT)y ) && (Button == WM_LBUTTONDOWN) )
    {
        graphics.m_CurrentState->Exit( m_ChangeState );
    }
}

void ButtonControl::OnMouseMove( int x, int y )
{
    if( CursorIntersect( float( x ), float( y ) ) )
    {
        m_Over = true;
        SetTexture( m_OverTex );
        m_Caption->SetColor( D3DCOLOR_XRGB( 255, 0, 0 ) );
    }
    else
    {
        m_Over = false;
        SetTexture( m_DefaultTex );
        m_Caption->SetColor( D3DCOLOR_XRGB( 255, 255, 0 ) );
    }
}

void ButtonControl::OnMouseUp( int Button, int x, int y )
{
}

bool ButtonControl::SetTextures( Texture * fileDefault, Texture * fileOver )
{
    if( !m_DefaultTex )
    {
        m_DefaultTex = fileDefault;
        SetWidthHeight( fileDefault->GetWidth( ), fileDefault->GetHeight( ) );
        //m_DefaultTex->SetTranslation(m_Position);
    }
    if( !m_OverTex )
    {
        m_OverTex = fileOver;
        SetWidthHeight( fileOver->GetWidth( ), fileOver->GetHeight( ) );
        //m_OverTex->SetTranslation(m_Position);
    }
    m_Rect.left = 0;
    m_Rect.top = 0;
    m_Rect.right = GetWidth( );
    m_Rect.bottom = GetHeight( );
    m_OverTex->SetRect( m_Rect );
    //m_DefaultTex->SetRect(m_Rect);
    m_Texture = m_DefaultTex;
    return true;
}

void ButtonControl::SetCaption( char * Caption )
{
    LOGFONT lf;
    if( m_Caption )
        delete m_Caption;

    SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lf ), &lf, 0 );
    m_Caption = new LabelControl( GetThis( ), 1, lf, m_Rect );
    m_Caption->SetCaption( Caption );
    m_Caption->SetSprite( m_Sprite );
    m_Caption->SetPosition( m_Position );
    m_Caption->SetWidthHeight( GetWidth( ), GetHeight( ) );
}

void ButtonControl::OnLostFocus( )
{
}
//---------------------------------------------------------------Slide Bar---------------------------------------------------------------------
SlideBar::SlideBar( UIBase * parent, int vecPos, D3DXVECTOR2 Position):UIBase( parent, vecPos )
{
    m_Font = NULL;
    m_Bar = NULL;
    m_SlideDefault = NULL;
    m_SlideOver = NULL;
    m_Over = false;
    m_ButtonDown = false;
    m_Format = DT_VCENTER | DT_CENTER;
    m_Position = Position;
    m_CapPos = Position;
    m_BarPos = Position;
}
SlideBar::~SlideBar( )
{
    DeleteObject( m_Font );
    if( m_Caption )
        delete m_Caption;
}
bool SlideBar::OnRender( )
{
    m_Sprite->DrawTexture( m_Bar, m_BarPos );
    m_Sprite->DrawTexture( m_Texture, m_Position );
    if( m_Caption )
    {
        m_Caption->OnRender( );
    }
    return false;
}

void SlideBar::OnMouseDown( int Button, int x, int y )
{
    if( CursorIntersect( (FLOAT)x, (FLOAT)y ) && (Button == WM_LBUTTONDOWN) )
    {
        printf( "mouse x: %d \n   slide pos x: %f \n", x, m_Position.x );
        m_ButtonDown = true;
    }
}

void SlideBar::OnMouseMove( int x, int y )
{
    if( CursorIntersect( float( x ), float( y ) ) || m_ButtonDown )
    {
        printf( "m_ButtonDown %d \n", m_ButtonDown );
        printf( "m_Position.x: %f \n", m_Position.x );
        m_Over = true;
        SetTexture( m_SlideOver );
        if( m_ButtonDown )
        {
                m_Position.x = (FLOAT)x;
            if( m_Position.x  < m_BarPos.x )
                m_Position.x = (FLOAT)(m_BarPos.x + 5);
            if( (m_Position.x + (FLOAT)GetWidth()) > (m_BarPos.x + (FLOAT)m_Bar->GetWidth()) )
                m_Position.x = (FLOAT)((m_BarPos.x + (FLOAT)m_Bar->GetWidth( )) -(FLOAT)(GetWidth()+ 5));
        }
    }
    else
    {
        m_ButtonDown = false;
        SetTexture( m_SlideDefault );
    }
}

void SlideBar::OnMouseUp( int Button, int x, int y )
{
    if( Button == WM_LBUTTONUP )
        m_ButtonDown = false;
}


bool SlideBar::SetTextures( Texture * Bar, Texture * SlideUp, Texture * SlideDown )
{
    if( !m_SlideDefault )
    {
        m_SlideDefault = SlideUp;
        SetWidthHeight( SlideUp->GetWidth( ), SlideUp->GetHeight( ) );
    }
    if( !m_SlideOver )
    {
        m_SlideOver = SlideDown;
    }
    if( !m_Bar )
    {
        m_Bar = Bar;
    }

    //get a RECT to draw the caption in
    m_CapRect.left = 0;
    m_CapRect.top = 0;
    m_CapRect.right = 100;
    m_CapRect.bottom = m_Bar->GetHeight( );

    //compute individual positions
    //the width of the bar should be big enough to hold the caption
    //we move it to the left of the slide bar by subtracting the width
    m_CapPos.x = (m_BarPos.x -100);
    m_Position.x = m_BarPos.x + ((m_Bar->GetWidth( ) / 2) - (m_SlideDefault->GetWidth( ) / 2));//put the slider in the middle of the bar
    //m_Bar->SetRect( m_CapRect );
    m_Texture = m_SlideDefault;
    return true;
}

void SlideBar::SetCaption( char * Caption )
{
    LOGFONT lf;
    if( m_Caption )
        delete m_Caption;

    SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lf ), &lf, 0 );
    m_Caption = new LabelControl( GetThis( ), 1, lf, m_CapRect);
    m_Caption->SetCaption( Caption );
    m_Caption->SetSprite( m_Sprite );
    m_Caption->SetPosition( m_CapPos );
    m_Caption->SetWidthHeight( 100, m_Bar->GetHeight( ) );
}

void SlideBar::OnLostFocus( )
{
    m_ButtonDown = false;
}

//---------------------------------------------------------------Health Bar---------------------------------------------------------------------
HealthBar::HealthBar( UIBase * parent, int vecPos, D3DXVECTOR2 Position):UIBase( parent, vecPos )
{
	m_Font = NULL;
	m_BackTex = NULL;
	m_Position = Position;
	m_FillAmount = 1.0f;
}

HealthBar::~HealthBar( )
{
	DeleteObject(m_Font);
	if (m_Caption)
		delete m_Caption;
}

bool HealthBar::OnRender( )
{
	m_Sprite->DrawTexture(m_BackTex, m_Position);
	m_Sprite->DrawTexture(m_Texture, m_Position);
	if (m_Caption)
	{
		m_Caption->OnRender();
	}
	return false;
}

bool HealthBar::SetTextures( Texture * Background, Texture * Fill )
{
	if (!m_Texture)
	{
		m_Texture = Fill;
		SetWidthHeight(m_Texture->GetWidth(), m_Texture->GetHeight());
	}
	if (!m_BackTex)
	{
		m_BackTex = Background;
	}
	return true;
}

void HealthBar::SetCaption( char * Caption )
{
	LOGFONT lf;
	if (m_Caption)
		delete m_Caption;

	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
	m_Caption = new LabelControl(GetThis(), 1, lf, m_CapRect);
	m_Caption->SetCaption(Caption);
	m_Caption->SetSprite(m_Sprite);
	m_Caption->SetPosition(m_CapPos);
	m_Caption->SetWidthHeight(100, m_Texture->GetHeight());
}

void HealthBar::OnLostFocus( )
{
}

void HealthBar::SetFill( float Percent )
{
	m_Texture->SetScaling(D3DXVECTOR2{ Percent, 1.0f });
}
