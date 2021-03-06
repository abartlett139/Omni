#ifndef UICONTROLS_H
#define UICONTROLS_H

#include "UIBase.h"
#include "GameState.h"

class WindowControl: public UIBase
{
private:
    bool m_IsParentWindow;
    bool m_IsMouseDown;
    D3DXVECTOR2 m_MouseClickPos;
public:
    WindowControl( UIBase* parent, int vecPos );
    ~WindowControl( );
    bool OnRender( );
    void OnMouseDown( int Button, int x, int y );
    void OnMouseMove( int x, int y );
    void OnMouseUp( int Button, int x, int y );
    void OnSetFocus( );
    void OnLostFocus( );
    void OnKeyDown( WPARAM Key, LPARAM Extended );
    void OnKeyUp( WPARAM Key, LPARAM Extended );
    bool LoadCanvasFromFile( char* File );
    void SetRect( RECT rect ) { m_Texture->SetRect( rect ); }
};

//--------------------------------------------------------------------Labels---------------------------
class LabelControl: public UIBase
{
private:
    char m_Caption[ 255 ];
    LPD3DXFONT m_Font;
    D3DCOLOR m_Color;
    D3DCOLOR m_ColorOver;
    D3DCOLOR m_ColorNULL;
    DWORD m_Format;
    bool m_ButtonLabel;
    RECT m_Rect;
public:
    LabelControl( UIBase* parent, int vecPos, LOGFONT Font, RECT Rect);
    ~LabelControl( );
    bool OnRender( );
    void OnMouseDown( int Button, int x, int y );
    void OnMouseMove( int x, int y );
    void OnMouseUp( int Button, int x, int y );
    void OnKeyDown( WPARAM Key, LPARAM Extended ) { return; }
    void OnKeyUp( WPARAM Key, LPARAM Extended ) { return; }
    void SetCaption( char* Caption );
    char* GetCaption( ) { return m_Caption; }
    void SetColor( D3DCOLOR Color ) { m_Color = Color; }
    void SetFormat( DWORD Format ) { m_Format = Format; }
    void OnLostFocus( ) { return; }
};
//-----------------------------------------------------------------Buttons------------------------------------------
class ButtonControl: public UIBase
{
    HFONT m_Font;
    LabelControl* m_Caption;
    Texture* m_DefaultTex;
    Texture* m_OverTex;
    bool m_Over;
    RECT m_Rect;
    GameState* m_ChangeState;
public:
    ButtonControl( UIBase* parent, int vecPos, D3DXVECTOR2 Position);
    ~ButtonControl( );
    bool OnRender( );
    void OnMouseDown( int Button, int x, int y );
    void OnMouseMove( int x, int y );
    void OnMouseUp( int Button, int x, int y );
    void OnKeyDown( WPARAM Key, LPARAM Extended ) { return; }
    void OnKeyUp( WPARAM Key, LPARAM Extended ) { return; }
    bool SetTextures( Texture* fileDefault, Texture* fileOver );
    void SetCaption( char* Caption );
    void OnLostFocus( );
    void SetChangeState( GameState* gameState ) { m_ChangeState = gameState; }
    void ChangeState( GameState* gameState ) { gameState = m_ChangeState; }
};
//----------------------------------------------------------SlideBar---------------------------------------------------

class SlideBar: public UIBase
{
private:
    
    HFONT m_Font;
    Texture* m_Bar;
    Texture* m_SlideDefault;
    Texture* m_SlideOver;
    bool m_Over;
	bool m_ButtonDown;
    LabelControl* m_Caption;
    DWORD      m_Format;
    RECT       m_CapRect;
    D3DXVECTOR2 m_BarPos, m_CapPos;
public:
    SlideBar( UIBase* parent, int vecPos, D3DXVECTOR2 Position );
    ~SlideBar( );
    bool OnRender( );
    void OnMouseDown( int Button, int x, int y );
    void OnMouseMove( int x, int y );
    void OnMouseUp( int Button, int x, int y );
    void OnKeyDown( WPARAM Key, LPARAM Extended ) { return; }
    void OnKeyUp( WPARAM Key, LPARAM Extended ) { return; }
    bool SetTextures( Texture* Bar, Texture* SlideUp, Texture* SlideDown );
    void SetCaption( char* Caption );
    void OnLostFocus( );
};

//--------------------------------------------------------------Health-------------------------------------------------
class HealthBar: public UIBase
{
    
    HFONT m_Font;
    LabelControl* m_Caption;
    Texture* m_BackTex;
    RECT m_CapRect;
    float m_FillAmount;
	D3DXVECTOR2  m_CapPos;
public:
    HealthBar( UIBase* parent, int vecPos, D3DXVECTOR2 Position);
    ~HealthBar( );
    bool OnRender( );
    void OnMouseDown( int Button, int x, int y ) { return; }
    void OnMouseMove( int x, int y ) { return; }
    void OnMouseUp( int Button, int x, int y ) { return; }
    void OnKeyDown( WPARAM Key, LPARAM Extended ) { return; }
    void OnKeyUp( WPARAM Key, LPARAM Extended ) { return; }
    bool SetTextures( Texture* Background, Texture* Fill );
    void SetCaption( char* Caption );
    void OnLostFocus( );
    void SetFill( float Percent );
};
#endif // !UICONTROLS_H

