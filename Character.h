#ifndef CHARACTER_H
#define CHARACTER_H

#include "Camera.h"
#include "Mesh.h"
#include "GameTimer.h"

class Character : public Camera {
public:
	Character();
	~Character();

	virtual bool Initialize() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Reset() = 0;

	Mesh characterMesh;
	D3D::BoundingBox box;

	D3DXMATRIX P, T, S;

	bool isAuto;
	int currentState;
	virtual enum STATES;

	float speed, strafeSpeed, turnSpeed;
};

#endif