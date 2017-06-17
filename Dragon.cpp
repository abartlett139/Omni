#include "Dragon.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

Dragon::Dragon(): m_KnightPointer( nullptr )
{
}


Dragon::~Dragon()
{
}

bool Dragon::Initialize()
{
	//	set speed variables
	speed = 200.0f;
	strafeSpeed = 75.0f;
	turnSpeed = 1.0f;

	//	set start position
	_pos = { -900.0f, 0.0f, 900.0f };

	//	initialize the physical mesh object
	characterMesh.Initialize("models/dragon.x");

	//	set the scale
	D3DXMatrixScaling(&S, 1.5f, 1.5f, 1.5f);

	return true;
}

void Dragon::Render()
{
	//	the position and rotation translation matrix is the inverse of the characters's view matrix
	getViewMatrix(&T);
	D3DXMatrixInverse(&T, NULL, &T);
	P = S*T;

	IDirect3DDevice9* Device = graphics.GetDevice();
	Device->SetTransform(D3DTS_WORLD, &P);

	characterMesh.Render();

	//	update the bounding box coordinates to match the mesh
	D3DXVec3TransformCoord(&box.MIN, &characterMesh.min, &P);
	D3DXVec3TransformCoord(&box.MAX, &characterMesh.max, &P);
}

void Dragon::GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
	//	update third person camera position (rear view)
	thirdPersonCamera._pos = ((-_look * 100.0f) + (_up * 50.0f)) + _pos;


	if (!isAuto) {
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000f) {
			yaw(timer.DeltaTime()* turnSpeed);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000f) {
			yaw(-timer.DeltaTime() * turnSpeed);
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000f) {
			fly(-timer.DeltaTime()* turnSpeed);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000f) {
			fly(timer.DeltaTime() * turnSpeed);
		}
		if (GetAsyncKeyState('W') & 0x8000f) {
			walk(timer.DeltaTime() * speed);
		}
		if (GetAsyncKeyState('S') & 0x8000f) {
			walk(-timer.DeltaTime() * speed);
		}
		if (GetAsyncKeyState('A') & 0x8000f) {
			strafe(-timer.DeltaTime() * strafeSpeed);
		}
		if (GetAsyncKeyState('D') & 0x8000f) {
			strafe(timer.DeltaTime() * strafeSpeed);
		}
	}
	else {
		//	do ai for dragon here
		//	maybe walk back and forth or whatever
	}
}

void Dragon::Update( )
{
	if(isAuto)
		ChaseState();

}

void Dragon::Reset()
{
	//	set start position
	_pos = { -900.0f, 0.0f, 900.0f };
}

void Dragon::IdleState()
{
	D3DXVECTOR3 test;
}

void Dragon::ChaseState()
{
	if (m_KnightPointer)
	{
		D3DXVECTOR3 l_TempDirection{ 0,0,0 }, l_TempNormal{ 0,0,0 };
		D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
		m_KnightPointer->getPosition(&l_KnightLoc);
		D3DXVec3Subtract(&l_TempDirection, &l_KnightLoc, &_pos);
		D3DXVec3Normalize(&l_TempDirection, &l_TempDirection);
		_pos += l_TempDirection*speed;
	}
}

void Dragon::AttackState()
{
}

void Dragon::FleeState()
{
}

D3DXMATRIX Dragon::getRearView()
{
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box.MAX.y, _pos.z), &D3DXVECTOR3(0, 1, 0));

	return V;
}

D3DXMATRIX Dragon::getSideView()
{
	D3DXMATRIX V;
	thirdPersonCamera._pos = ((_right * 500.0f) + (_up * 50.0f)) + _pos;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box.MAX.y, _pos.z), &D3DXVECTOR3(1, 0, 0));

	return V;
}
