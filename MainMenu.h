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
    LPDIRECT3DDEVICE9 m_Device;
    bool m_Init;
    Sprite* m_Sprite;
    Texture* m_ButtonOver;
    Texture* m_ButtonDefault;
    Texture* m_Background;

    WindowControl* wc;
public:
    MainMenu( LPDIRECT3DDEVICE9 Device );
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
	LPDIRECT3DDEVICE9 m_Device;
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	Texture* m_TextBox;
	FileIOHelper* m_File;
	WindowControl* wc;
public:
	Story(LPDIRECT3DDEVICE9 Device);
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
	LPDIRECT3DDEVICE9 m_Device;
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	Texture* m_TextBox;
	FileIOHelper* m_File;
	WindowControl* wc;
public:
	Credits(LPDIRECT3DDEVICE9 Device);
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
	LPDIRECT3DDEVICE9 m_Device;
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	WindowControl* wc;
public:
	Options(LPDIRECT3DDEVICE9 Device);
	~Options();
	bool Init();
	void Enter();
	void Render();
	void Update();
	void Exit(GameState* nextState);
	void ProcessMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
};

#endif // !MAINMENU_H
