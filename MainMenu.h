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

class MainMenu : public GameState
{
private:
	LPDIRECT3DDEVICE9 m_Device;
	bool m_Init;
	Sprite* m_Sprite;
	Texture* m_ButtonOver;
	Texture* m_ButtonDefault;
	Texture* m_Background;
	Input* m_Input;
	Mouse* m_Mouse;
	WindowControl* wc;
public:
	MainMenu( LPDIRECT3DDEVICE9 Device);
	~MainMenu();
	bool Init();
	void Enter();
	void Render();
	void Update(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
	void Exit(GameState* nextState);
};

#endif // !MAINMENU_H
