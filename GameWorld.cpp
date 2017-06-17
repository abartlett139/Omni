#include "GameWorld.h"



GameWorld::GameWorld( )
{
}


GameWorld::~GameWorld( )
{
    D3D::Delete<Terrain*>( terrain );
}

bool GameWorld::Init( )
{
    IDirect3DDevice9* Device = graphics.GetDevice( );

    //	directional light
    worldLight = D3D::InitDirectionalLight( &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ), (D3DXCOLOR*)&D3D::WHITE );
    Device->SetLight( 0, &worldLight );
    Device->LightEnable( 0, TRUE );

    //	spot light
    spotLight = D3D::InitSpotLight( &D3DXVECTOR3( 0.0f, 100.0f, 10.0f ), &D3DXVECTOR3( 0.0f, -1.0f, 0.0f ), (D3DXCOLOR*)&D3D::WHITE );
    Device->SetLight( 1, &spotLight );
    Device->LightEnable( 1, TRUE );

    //	point light
    pointLight = D3D::InitPointLight( &D3DXVECTOR3( 0.0f, 2.0f, 10.0f ), (D3DXCOLOR*)&D3D::WHITE );
    Device->SetLight( 2, &pointLight );
    Device->LightEnable( 2, TRUE );

    //	turn on lighting
    Device->SetRenderState( D3DRS_LIGHTING, true );
    Device->SetRenderState( D3DRS_SPECULARENABLE, true );

    //	intialize skybox
    skybox.generate( );

    //	initialize terrain
    terrain = new Terrain( "textures/newHeightmap.raw", 2001, 2001, 1, 0.2f );
    terrain->genTexture( &D3DXVECTOR3( 0.0, -1.0f, 0.0f ) );
    terrain->loadTexture( "textures/newTexture.png" );

    //	initialize game objects
    knight.Initialize( );
    dragon.Initialize( );
    witch.Initialize( );
	dragon.SetEnemy(&knight);
    moat.Initialize( "models/moat.x" );
	castle.Initialize("models/castle.x");

    knight.isAuto = false;


	D3DXCreateSprite(Device, &sprite);

	D3DXCreateTextureFromFile(Device, "textures/tree.png", &treeTexture);

    //	set music type
    musicType = soundEngine->GAME_BACKGROUND;

    return true;
}

void GameWorld::Enter( )
{
    GameState::Enter( );

    soundEngine->PlayMusic( musicType );

    //	set projection matrix
    D3DXMATRIX P;
    IDirect3DDevice9* Device = graphics.GetDevice( );

    D3DXMatrixPerspectiveFovLH( &P, D3DX_PI / 4, screenWidth / screenHeight, 1.0f, 3000.0f );
    Device->SetTransform( D3DTS_PROJECTION, &P );
}

void GameWorld::Render( )
{

    //	set view matrix
    IDirect3DDevice9* Device = graphics.GetDevice( );

    if( Device )
    {

        Device->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0 );
        Device->BeginScene( );
        Device->SetTransform( D3DTS_VIEW, &knight.getRearView( ) );

        //	render skybox
        D3DXMATRIX W;
        D3DXMatrixTranslation( &W, knight.thirdPersonCamera._pos.x, knight.thirdPersonCamera._pos.y, knight.thirdPersonCamera._pos.z );
        Device->SetTransform( D3DTS_WORLD, &W );

        skybox.render( );

        //	render terrain
        D3DXMATRIX I;
        D3DXMatrixIdentity( &I );
        terrain->draw( &I, false );


        //	render knight
        knight.Render( );

        // render dragon
        dragon.Render( );

        //	render witch
        witch.Render( );

        D3DXMATRIX moatScale, moatPos;
        D3DXMatrixScaling( &moatScale, 100.0f, 100.0f, 100.0f );
        D3DXMatrixTranslation( &moatPos, 0.0f, 10.0f, -300.0f );
        Device->SetTransform( D3DTS_WORLD, &(moatScale * moatPos*I) );
        moat.Render( );

		D3DXMATRIX castlePos, castleScale;
		D3DXMatrixScaling(&castleScale, 25.0f, 25.0f, 25.0f);
		D3DXMatrixTranslation(&castlePos, 500.0f, terrain->getHeight(800.0f, 800.0f), 800.0f);
		Device->SetTransform(D3DTS_WORLD, &(castleScale*castlePos));
		castle.Render();


		D3DXMATRIX treeMatrix;
		for (int i = 0; i < 10; i++) {
			D3DXMatrixTranslation(&treeMatrix, 100 * i, terrain->getHeight(10 * i, -990), -990);
			sprite->SetWorldViewLH(&I, &knight.getRearView());
			sprite->Begin(D3DXSPRITE_BILLBOARD | D3DXSPRITE_ALPHABLEND);
			sprite->Draw(treeTexture, NULL, NULL, &D3DXVECTOR3(100*i, terrain->getHeight(10*i, -990), -990), D3D::WHITE);
			sprite->End();
		}
		

        Device->EndScene( );
        Device->Present( 0, 0, 0, 0 );
    }
}

void GameWorld::Update( )
{
    knight.Update( );
    dragon.Update( );
    witch.Update( );
    //	update character y positions so they don't go beneath the terrain
    knight._pos.y = terrain->getHeight( knight._pos.x, knight._pos.z );
    dragon._pos.y = terrain->getHeight( dragon._pos.x, dragon._pos.z );
    witch._pos.y = terrain->getHeight( witch._pos.x, witch._pos.z );
}

void GameWorld::ProcessMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    knight.GetMessages( msg, wParam, lParam, Data );
    dragon.GetMessages( msg, wParam, lParam, Data );
    witch.GetMessages( msg, wParam, lParam, Data );
}
void GameWorld::OnLostDevice(  )
{
    D3D::Delete<Terrain*>( terrain );
}

void GameWorld::Exit( GameState * nextState )
{
    //	reset the projection and view matrices to the default
    IDirect3DDevice9* Device = graphics.GetDevice( );

    D3DXMATRIX I;
    D3DXMatrixIdentity( &I );
    Device->SetTransform( D3DTS_PROJECTION, &I );
    Device->SetTransform( D3DTS_VIEW, &I );

    //	reset game objects
    knight.Reset( );
    dragon.Reset( );
    witch.Reset( );

    soundEngine->StopMusic( );

    GameState::Exit( nextState );
}
