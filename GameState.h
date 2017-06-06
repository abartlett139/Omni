#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "D3D.h"
#include "GameTimer.h"
#include "SoundEngine.h"

//	GameState class delcaration
class GameState;

//	external game states
extern GameState *currentState, *previousState, *gameWorld;

class GameState
{
public:
	GameState();
	~GameState();

	virtual bool Init() = 0;
	virtual void Enter();
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Exit(GameState *nextState);

	void GameLoop();

	bool isInit, changeState;
	int musicType;
};

#endif
