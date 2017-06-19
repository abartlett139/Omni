#ifndef CHARACTER_H
#define CHARACTER_H

#include "Camera.h"
#include "Mesh.h"
#include "GameTimer.h"
#include "Terrain.h"

class Character : public Camera {
public:
	Character();
	~Character();
	enum state { IDLE, CHASE, FLEE, ATTACK, JUMP, FALL };
	int m_State;
	virtual bool Initialize(Terrain *terrain) = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Reset() = 0;
	virtual void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data ) = 0;
	Mesh characterMesh;
	D3D::BoundingBox box;

	D3DXMATRIX P, T, S;

	bool isAuto;
	int currentState;

	float speed, strafeSpeed, turnSpeed;

	D3DXVECTOR3 prevPos, velocity;

	bool canJump;
};

#endif