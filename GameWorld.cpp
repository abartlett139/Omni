#include "GameWorld.h"



GameWorld::GameWorld()
{
}


GameWorld::~GameWorld()
{
	D3D::Delete<Terrain*>(terrain);
}

bool GameWorld::Init()
{
	//	directional light
	worldLight = D3D::InitDirectionalLight(&D3DXVECTOR3(0.0f, -1.0f, 0.0f), (D3DXCOLOR*)&D3D::WHITE);
	Device->SetLight(0, &worldLight);
	Device->LightEnable(0, TRUE);

	//	spot light
	spotLight = D3D::InitSpotLight(&D3DXVECTOR3(0.0f, 100.0f, 10.0f), &D3DXVECTOR3(0.0f, -1.0f, 0.0f), (D3DXCOLOR*)&D3D::WHITE);
	Device->SetLight(1, &spotLight);
	Device->LightEnable(1, TRUE);

	//	point light
	pointLight = D3D::InitPointLight(&D3DXVECTOR3(0.0f, 2.0f, 10.0f), (D3DXCOLOR*)&D3D::WHITE);
	Device->SetLight(2, &pointLight);
	Device->LightEnable(2, TRUE);

	//	turn on lighting
	Device->SetRenderState(D3DRS_LIGHTING, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//	intialize skybox
	skybox.generate();

	//	initialize terrain
	terrain = new Terrain("textures/heightMap2.raw", 2001, 2001, 1, 0.2f);
	terrain->genTexture(&D3DXVECTOR3(0.0, -1.0f, 0.0f));
	terrain->loadTexture("textures/terrain2.png");

	//	initialize game objects
	knight.Initialize();
	dragon.Initialize();
	witch.Initialize();

	moat.Initialize("models/moat.x");

	knight.isAuto = false;

	//	set music type
	musicType = soundEngine->GAME_BACKGROUND;

	return true;
}

void GameWorld::Enter()
{
	GameState::Enter();

	//	set projection matrix
	D3DXMATRIX P;
	D3DXMatrixPerspectiveFovLH(&P, D3DX_PI / 4, screenWidth / screenHeight, 1.0f, 3000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &P);
}

void GameWorld::Render()
{
	//	update character y positions so they don't go beneath the terrain
	knight._pos.y = terrain->getHeight(knight._pos.x, knight._pos.z);
	dragon._pos.y = terrain->getHeight(dragon._pos.x, dragon._pos.z);
	witch._pos.y = terrain->getHeight(witch._pos.x, witch._pos.z);

	//	set view matrix
	Device->SetTransform(D3DTS_VIEW, &knight.getRearView());

	if (Device) {

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, COLOR_WINDOW, 1.0f, 0);
		Device->BeginScene();

		//	render skybox
		D3DXMATRIX W;
		D3DXMatrixTranslation(&W, knight.thirdPersonCamera._pos.x, knight.thirdPersonCamera._pos.y, knight.thirdPersonCamera._pos.z);
		Device->SetTransform(D3DTS_WORLD, &W);
		skybox.render();

		//	render terrain
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		terrain->draw(&I, false);


		//	render knight
		knight.Render();

		// render dragon
		dragon.Render();

		//	render witch
		witch.Render();

		D3DXMATRIX moatScale, moatPos;
		D3DXMatrixScaling(&moatScale, 100.0f, 100.0f, 100.0f);
		D3DXMatrixTranslation(&moatPos, 0.0f, 10.0f, -300.0f);
		Device->SetTransform(D3DTS_WORLD, &(moatScale * moatPos));
		moat.Render();

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
}

void GameWorld::Update()
{
	knight.Update();
	dragon.Update();
	witch.Update();
}

void GameWorld::Exit(GameState * nextState)
{
	//	reset the projection and view matrices to the default
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_PROJECTION, &I);
	Device->SetTransform(D3DTS_VIEW, &I);
	
	//	reset game objects
	knight.Reset();
	dragon.Reset();
	witch.Reset();

	GameState::Exit(nextState);
}
