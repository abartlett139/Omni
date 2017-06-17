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

class GameWorld: public GameState
{
public:
    GameWorld( );
    ~GameWorld( );

    bool Init( );
    void Enter( );
    void Render( );
    void Update( );
    void Exit( GameState *nextState );
    void ProcessMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );

    D3DLIGHT9 worldLight;
    D3DLIGHT9 spotLight;
    D3DLIGHT9 pointLight;

    Skybox skybox;
    Terrain *terrain = 0;

    Knight knight;
    Dragon dragon;
    Witch witch;

    Mesh moat;
	Mesh castle;

    ID3DXFont *font;

	ID3DXSprite *sprite;
	IDirect3DTexture9* treeTexture;


};

#endif