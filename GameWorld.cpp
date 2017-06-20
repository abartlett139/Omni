#include "GameWorld.h"



GameWorld::GameWorld( )
{
	// reset() transfers the pointer to the shared_ptr class to manage
	// Can not use std::make_shared<IXAnimator> directly because IXAnimator is a virtual base class 
	// and not meant to be created except through the factory function below
	//	xAnimator.reset( CreateXAnimator( graphics.GetDevice( ) ) );
	XAnimator.reset(CreateXAnimator(Device));
}


GameWorld::~GameWorld( )
{
    D3D::Delete<Terrain*>( terrain );
}

bool GameWorld::Init( )
{
   // IDirect3DDevice9* Device = graphics.GetDevice( );

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
    terrain->loadTexture( "textures/terrain.png" );

    //	initialize game objects
	knight.Initialize(terrain );
    dragon.Initialize(terrain );
    witch.Initialize(terrain );
	skele.Initialize( XAnimator );
	dragon.SetEnemy(&knight);
    moat.Initialize( "models/moat.x" );
	castle.Initialize("models/castle.x");
	bridge.Init();

    knight.isAuto = false;


	D3DXCreateSprite(Device, &sprite);


	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);


	treeTexture = D3D::LoadTexture("textures/tree3.png");

    //	set music type
    musicType = soundEngine->GAME_BACKGROUND;

	p.Initialize("models/bridge.x", 8.0f);
	m.Initialize("models/moat.x", 100.0f);



    return true;
}

void GameWorld::Enter( )
{
    GameState::Enter( );

    soundEngine->PlayMusic( musicType );

    //	set projection matrix
    D3DXMATRIX P;
    //	IDirect3DDevice9* Device = graphics.GetDevice( );

    D3DXMatrixPerspectiveFovLH( &P, D3DX_PI / 4, screenWidth / screenHeight, 1.0f, 3000.0f );
    Device->SetTransform( D3DTS_PROJECTION, &P );
}

void GameWorld::Render( )
{

    //	set view matrix
   //	 IDirect3DDevice9* Device = graphics.GetDevice( );

    if( Device )
    {

        Device->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0 );
        Device->BeginScene( );
       // Device->SetTransform( D3DTS_VIEW, &knight.getRearView( ) );

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

		//	render witch
		skele.Render();

		//	give moat a wrapper class or something to render it as a static object
		//	so it has a bounding box with the matrix transformation on it.
        //	D3DXMATRIX moatScale, moatPos;
        //	D3DXMatrixScaling( &moatScale, 100.0f, 100.0f, 100.0f );
        //	D3DXMatrixTranslation( &moatPos, 0.0f, 10.0f, -300.0f );
        //	Device->SetTransform( D3DTS_WORLD, &(moatScale * moatPos*I) );
        //	moat.Render( );

		m.Render(&D3DXVECTOR3(0.0f, 10.0f, -300.0f));
		p.Render(&D3DXVECTOR3(-350.0f, 20.0f, -380.0f));

		D3DXMATRIX castlePos, castleScale;
		D3DXMatrixScaling(&castleScale, 25.0f, 25.0f, 25.0f);
		D3DXMatrixTranslation(&castlePos, 500.0f, terrain->getHeight(800.0f, 800.0f), 800.0f);
		Device->SetTransform(D3DTS_WORLD, &(castleScale*castlePos));
		castle.Render();
		

		bridge.Render();


		D3DXMATRIX treeMatrix, treeScale;
		D3DXMatrixScaling(&treeScale, .3f, .3f, .3f);
		D3DXMatrixTranslation(&treeMatrix, 3.0f, 3.0f, 3.0f);
		t = knight.getRearView();
		for (int i = 0; i < 20; i++) {
			sprite->SetWorldViewLH(&treeScale, &t);
			sprite->Begin(D3DXSPRITE_BILLBOARD | D3DXSPRITE_ALPHABLEND);
			sprite->Draw(treeTexture, NULL, &D3DXVECTOR3(202.5f, 0.0f, 0.0f), &D3DXVECTOR3((float)(100 * i * 3), (float)(terrain->getHeight(100 * i, -990)*3), (float)(-990 * 3)), D3D::WHITE);
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
	skele.Update( );

	//	if the knight is over the moat
	//if (m.PlatformCollision(&knight)) {
	//	//	check to see if the knight is already on a platform
	//	if (!knight.isOnPlatform) {
	//		//	is the knight over the platform
	//		if (p.PlatformCollision(&knight)) {
	//			//	is the knight on the platform
	//			if (knight._pos.y < p.box.MAX.y) {
	//				knight._pos.y = p.box.MAX.y;
	//				knight.isOnPlatform = true;
	//			}
	//		}
	//		else if(knight._pos.y < terrain->getHeight(knight._pos.x, knight._pos.z)){
	//			knight.currentState = Character::FALL;
	//		}
	//	}
		//	2103948580
		//	call and spam
		//	endlessly
	//}
	//	check to see if the knight is in the moat
	if (knight._pos.x <= moat.max.x && knight._pos.x >= moat.min.x && knight._pos.z <= moat.max.z && knight._pos.z >= moat.min.z) {
		//	check to see if player is dead (if it's y position has touched the moat)
		if (knight._pos.y <= moat.max.y) {
			MessageBox(0, 0, 0, 0);
		}
		//	check to see if the player is on a bridge fragment
		if (knight._pos.x <= bridge.box.MAX.x && knight._pos.x >= bridge.box.MIN.x && knight._pos.z <= bridge.box.MAX.z && knight._pos.z >= bridge.box.MIN.z) {
			//	make sure the player stays on top of the bridge fragment
			if (knight._pos.y <= bridge.box.MAX.y)
					knight._pos.y = bridge.box.MAX.y;
		}
		//	if the knight is not on a bridge fragment, make him fall
		else { knight.currentState = Character::FALL; }
		}





    //	update character y positions so they don't go beneath the terrain
	//	knight._pos.y = terrain->getHeight( knight._pos.x, knight._pos.z );
	//	dragon._pos.y = terrain->getHeight( dragon._pos.x, dragon._pos.z );
	//	witch._pos.y = terrain->getHeight( witch._pos.x, witch._pos.z );
	//	skele._pos.y = terrain->getHeight( skele._pos.x, skele._pos.z );
}

void GameWorld::ProcessMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
   //	 knight.GetMessages( msg, wParam, lParam, Data );
   //	 dragon.GetMessages( msg, wParam, lParam, Data );
   // witch.GetMessages( msg, wParam, lParam, Data );
	//skele.GetMessages( msg, wParam, lParam, Data );
}

void GameWorld::Exit( GameState * nextState )
{
    //	reset the projection and view matrices to the default
    //	IDirect3DDevice9* Device = graphics.GetDevice( );

    D3DXMATRIX I;
    D3DXMatrixIdentity( &I );
    Device->SetTransform( D3DTS_PROJECTION, &I );
    Device->SetTransform( D3DTS_VIEW, &I );

    //	reset game objects
    knight.Reset( );
    dragon.Reset( );
    witch.Reset( );
	skele.Reset();

    soundEngine->StopMusic( );

    GameState::Exit( nextState );
}
