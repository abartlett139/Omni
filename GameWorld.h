#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "GameState.h"
#include "Camera.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Knight.h"
#include "Dragon.h"
#include "Witch.h"

class GameWorld : public GameState
{
public:
	GameWorld( LPDIRECT3DDEVICE9 Device );
	~GameWorld();

	bool Init();
	void Enter();
	void Render();
	void Update(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
	void Exit(GameState *nextState);

	D3DLIGHT9 worldLight;
	D3DLIGHT9 spotLight;
	D3DLIGHT9 pointLight;

	Skybox skybox;
	Terrain *terrain = 0;

	Knight knight;
	//Dragon dragon;
	//Witch witch;

	Mesh moat;

	ID3DXFont *font;


};

#endif