#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "D3D.h"
#include "GameTimer.h"

class GameState;

extern GameState *currentState, *previousState, *gameWorld;

class GameState
{
public:
	GameState();
	~GameState();

	virtual bool Init() = 0;
	virtual void Enter() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Exit(GameState *nextState) = 0;

	void GameLoop();

	bool isInit, changeState;
};

#endif
