#include "GameState.h"



GameState::GameState()
{
}


GameState::~GameState()
{
}

void GameState::GameLoop()
{

	currentState->Render();
	currentState->Update();

}
