#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include "GameState.h"

// Forward declerations to eliminate header file cycles
class Input;
class Mouse;
class WindowControl;
class Sprite;
class Texture;
class FileIOHelper;
class MainMenu: public GameState
{
private:
    //	
    bool m_Init;
    Sprite* m_Sprite;
    Texture* m_ButtonOver;
    Texture* m_ButtonDefault;
    Texture* m_Background;

    WindowControl* wc;
public:
    MainMenu();
    ~MainMenu( );
    bool Init( );
    void Enter( );
    void Render( );
    void Update( );
    void Exit( GameState* nextState );
    void ProcessMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
};

class Story : public GameState
{
private:
	//
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	Texture* m_TextBox;
	FileIOHelper* m_File;
	WindowControl* wc;
public:
	Story();
	~Story();
	bool Init();
	void Enter();
	void Render();
	void Update();
	void Exit(GameState* nextState);
	void ProcessMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
};

class Credits : public GameState
{
private:
	
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	Texture* m_TextBox;
	FileIOHelper* m_File;
	WindowControl* wc;
public:
	Credits();
	~Credits();
	bool Init();
	void Enter();
	void Render();
	void Update();
	void Exit(GameState* nextState);
	void ProcessMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
};

class Options : public GameState
{
private:
	
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	Texture* m_SlideBar;
	Texture* m_Slider;
	Texture* m_SliderOver;
	WindowControl* wc;
public:
	Options();
	~Options();
	bool Init();
	void Enter();
	void Render();
	void Update();
	void Exit(GameState* nextState);
	void ProcessMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
};

#endif // !MAINMENU_H
