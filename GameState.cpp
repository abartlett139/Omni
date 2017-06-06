#include "GameState.h"



GameState::GameState()
{
}


GameState::~GameState()
{
}

void GameState::Enter()
{
	if (!currentState->isInit)
		currentState->isInit = currentState->Init();

	soundEngine->PlayMusic(currentState->musicType);
}

void GameState::Exit(GameState * nextState)
{
	soundEngine->StopMusic();

<<<<<<< HEAD
	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0);
=======
	//currentState->Render();
	//currentState->Update();
>>>>>>> refs/remotes/origin/ui-merge

	previousState = currentState;
	currentState = nextState;
	nextState->Enter();
}

void GameState::GameLoop()
{
	Render();
	Update();
}
