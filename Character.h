#ifndef CHARACTER_H
#define CHARACTER_H

#include "Camera.h"
#include "Mesh.h"
#include "GameTimer.h"

class Character : public Camera
{
public:
	Character();
	~Character();

	bool Initialize(int characterType);
	void Render();


	//--------------------//
	//	Update()
	//--------------------//
	//	character update
	//	called in GameWorld.cpp update
	//--------------------//
	void Update();


	Camera thirdPersonCamera;
	Mesh characterMesh;
	D3D::BoundingBox box;
	int type;
	int view;
	float scale;

	D3DXMATRIX P, T, S;

	bool isAuto;	//	is the character being controlled by AI or not

	enum CharacterType{KNIGHT, DRAGON, PRINCESS};
	enum CameraView{REAR, SIDE};
};

#endif