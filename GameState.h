#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Graphics.h"
#include "GameTimer.h"
#include "SoundEngine.h"

<<<<<<< HEAD
//	GameState class delcaration
class GameState;

//	external game states
extern GameState *currentState, *previousState, *gameWorld;
=======
//class GameState;

//extern GameState *currentState, *previousState, *gameWorld;
>>>>>>> refs/remotes/origin/ui-merge

class GameState
{
public:
	GameState();
	~GameState();

	virtual bool Init() = 0;
	virtual void Enter();
	virtual void Render() = 0;
<<<<<<< HEAD
	virtual void Update() = 0;
	virtual void Exit(GameState *nextState);
=======
	virtual void Update(UINT msg, WPARAM wParam, LPARAM lParam, void * Data) = 0;
	virtual void Exit(GameState *nextState) = 0;
>>>>>>> refs/remotes/origin/ui-merge

	void GameLoop();

	bool isInit, changeState;
	int musicType;
};

#endif
