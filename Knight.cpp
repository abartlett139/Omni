#include "Knight.h"

Knight::Knight()
{
}

Knight::~Knight()
{
}

bool Knight::Initialize(Terrain * terrainPTR)
{
	speed = 100.0f;
	strafeSpeed = 100.0f;
	turnSpeed = 2.0f;

	terrain = terrainPTR;

	characterMesh.Initialize("models/knight.x");

	//	set the scale
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);


	//	set start position
	_pos = { 900.0f, 0.0f, -900.0f };
	//	set start state
	currentState = IDLE;

	canJump = true;

	return true;
}

void Knight::Render()
{
	//	IDirect3DDevice9* Device = graphics.GetDevice();

	//	the position and rotation translation matrix is the inverse of the characters's view matrix
	getViewMatrix(&T);
	D3DXMatrixInverse(&T, NULL, &T);
	P = S*T;
	Device->SetTransform(D3DTS_WORLD, &P);

	characterMesh.Render();

	//	update the bounding box coordinates to match the mesh
	D3DXVec3TransformCoord(&box.MIN, &characterMesh.min, &P);
	D3DXVec3TransformCoord(&box.MAX, &characterMesh.max, &P);
}

void Knight::Update()
{
	//	make sure model doesn't go below the terrain
	if (_pos.y < terrain->getHeight(_pos.x, _pos.z))
		_pos.y = terrain->getHeight(_pos.x, _pos.z);

	if (!isAuto) {
		//	update third person camera position (rear view)
		thirdPersonCamera._pos = ((-_look * 20.0) + (_up * 10.0f)) + _pos;

		Device->SetTransform(D3DTS_VIEW, &getRearView());

		if (GetAsyncKeyState(VK_SPACE) && 0x8000f && canJump) {
			canJump = false;
			currentState = JUMP;
			velocity.y = 20.0f;
			prevPos = _pos;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000f) {
			yaw(timer.DeltaTime()* turnSpeed);
		}
		else
			if (GetAsyncKeyState(VK_LEFT) & 0x8000f) {
				yaw(-timer.DeltaTime() * turnSpeed);
			}
			else
				if (GetAsyncKeyState('W') & 0x8000f) {
					walk(timer.DeltaTime() * speed);
				}
				else
					if (GetAsyncKeyState('S') & 0x8000f) {
						walk(-timer.DeltaTime() * speed);
					}
					else
						if (GetAsyncKeyState('A') & 0x8000f) {
							strafe(-timer.DeltaTime() * strafeSpeed);
						}
						else
							if (GetAsyncKeyState('D') & 0x8000f) {
								strafe(timer.DeltaTime() * strafeSpeed);
							}





		if (currentState == JUMP) {
			_pos.y += velocity.y*timer.DeltaTime();
			velocity.y += -20.0f*timer.DeltaTime();
			if (velocity.y <= 0.0f)
				currentState = FALL;
			if (_pos.y <= terrain->getHeight(_pos.x, _pos.z)) {
				currentState = IDLE;
				canJump = true;
			}
		}

		if (currentState == FALL) {
			_pos.y += velocity.y * timer.DeltaTime();
			velocity.y += -20.0f*timer.DeltaTime();
		}
	}


}

void Knight::Reset()
{
	//	set start position
	_pos = { 900.0f, 0.0f, -900.0f };
	//	set start state
	currentState = IDLE;

	canJump = true;
}

void Knight::GetMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data)
{
}

D3DXMATRIX Knight::getRearView()
{
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box.MAX.y, _pos.z), &D3DXVECTOR3(0, 1, 0));
	return V;
}
