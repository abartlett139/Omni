#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <memory>

#include "GameState.h"
#include "Camera.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Knight.h"
#include "Dragon.h"
#include "Witch.h"
#include "Skele.h"
#include "3rd-party\xanimator\include\XAnimator_lib.h"

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

	// The gameworld is a good place to hold the master reference to the .x model & animation library,
	// since all models in the game world will probably need a reference to it
	// We use a shared_ptr to manage the lifetime of the x model & animation library, to reduce confusion
	// about who the owner of the library should be as there will be many references to it
	std::shared_ptr<IXAnimator> xAnimator;

};

#endif