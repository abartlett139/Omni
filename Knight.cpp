#include "Knight.h"



Knight::Knight()
{
}


Knight::~Knight()
{
}

bool Knight::Initialize()
{
	//	set speed variables
	speed = 100.0f;
	strafeSpeed = 50.0f;
	turnSpeed = 2.0f;

	//	set start position
	_pos = { 900.0f, 0.0f, -900.0f };

	//	initialize the physical mesh object
	characterMesh.Initialize("models/knight.x");

	//	set the scale
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);

	return true;
}

void Knight::Render()
{
	//	the position and rotation translation matrix is the inverse of the characters's view matrix
	getViewMatrix(&T);
	D3DXMatrixInverse(&T, NULL, &T);
	P = S*T;
	Device->SetTransform(D3DTS_WORLD, &P);

	characterMesh.Render();

	//	update the bounding box coordinates to match the mesh
	D3DXVec3TransformCoord(&box._min, &characterMesh.min, &P);
	D3DXVec3TransformCoord(&box._max, &characterMesh.max, &P);
}

void Knight::Update()
{
	//	update third person camera position (rear view)
	thirdPersonCamera._pos = ((-_look * 50.0f) + (_up * 25.0f)) + _pos;


	if (!isAuto) {
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000f) {
			yaw(timer.DeltaTime()* turnSpeed);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000f) {
			yaw(-timer.DeltaTime() * turnSpeed);
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000f) {
			pitch(-timer.DeltaTime()* turnSpeed);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000f) {
			pitch(timer.DeltaTime() * turnSpeed);
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
		//	do ai for knight here
		//	maybe strafe back and forth or whatever
	}
	
}

void Knight::Reset()
{
}

D3DXMATRIX Knight::getRearView()
{
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box._max.y, _pos.z), &D3DXVECTOR3(0, 1, 0));
	return V;
}

D3DXMATRIX Knight::getSideView()
{
	D3DXMATRIX V;
	thirdPersonCamera._pos = ((_right * 500.0f) + (_up * 50.0f)) + _pos;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box._max.y, _pos.z), &D3DXVECTOR3(1, 0, 0));

	return V;
}
