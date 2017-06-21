#include "GameState.h"

GameState::GameState()
{
}


GameState::~GameState()
{
}

void GameState::Enter()
{
	if (!isInit)
		isInit = Init();
}

void GameState::Exit(GameState * nextState)
{
	nextState->Enter();
}
