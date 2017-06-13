#include "GameState.h"

GameState::GameState( LPDIRECT3DDEVICE9 Device ) : m_Device(Device )
{
}


GameState::~GameState()
{
}

void GameState::Enter()
{
}

void GameState::Exit(GameState * nextState)
{
}
