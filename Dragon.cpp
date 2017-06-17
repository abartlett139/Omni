#include "Dragon.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

Dragon::Dragon(): m_KnightPointer( nullptr ), m_mood( idle )
{
}


Dragon::~Dragon()
{
}

bool Dragon::Initialize()
{
	//	set speed variables
	speed = 5.0f;
	strafeSpeed = 5.0f;
	turnSpeed = 2.0f;

	//	set start position
	Reset();

	//	initialize the physical mesh object
	characterMesh.Initialize("models/dragon.x");

	//	set the scale
	D3DXMatrixScaling(&S, 1.5f, 1.5f, 1.5f);

	return true;
}

void Dragon::Render()
{
	// The dragon model is rotated 90 degrees from the 'forward' direction
	// When we render it, we patch it up here with a rotation matrix (1.5708 is 90 degrees in radians)
	D3DXMATRIX rotation_fix;
	D3DXMatrixRotationY(&rotation_fix, 1.5708 );

	//	the position and rotation translation matrix is the inverse of the characters's view matrix
	getViewMatrix(&T);
	D3DXMatrixInverse(&T, NULL, &T);
	P = rotation_fix*S*T;

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

	if (isAuto)
	{
		switch (msg)
		{
		case WM_CHAR:
			switch (wParam)
			{
			case ',':
				m_mood = Dragon::idle;
				printf("Dragon taking a smoke break, sharpening her claws\n");
				break;
			case '.':
				printf("Dragon hungry; chasing silly knight\n");
				m_mood = Dragon::chase;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

}

void Dragon::Update( )
{
	if (isAuto)
	{
		if (m_mood == Dragon::idle)
			IdleState();
		else if( m_mood == Dragon::chase)
			ChaseState();
	}
	else
	{
		printf("Dragon error: Dragon should be AI driven\n");
	}
}

void Dragon::Reset()
{
	//	set start position
	_pos = { -900.0f, 0.0f, 900.0f };
}

// Trying to get the dragon to always face the knight
void Dragon::IdleState()
{
	D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
	m_KnightPointer->getPosition(&l_KnightLoc);

	//Calculate vector pointing from dragon to knight	
	D3DXVECTOR3 l_TempDirection{ 0,0,0 };
	D3DXVec3Subtract(&l_TempDirection, &l_KnightLoc, &_pos);
	D3DXVec3Normalize(&l_TempDirection, &l_TempDirection);

	D3DXVECTOR3 l_dragonLookDir{ 0,0,0 };
	getLook(&l_dragonLookDir);
	D3DXVec3Normalize(&l_dragonLookDir, &l_dragonLookDir);

	float l_angle = D3DXVec3Dot(&l_dragonLookDir, &l_TempDirection);

	// turn dragon to face the knight, if the angle between the two is greater than a small theshold
	if (fabs(l_angle) >= 0.001f)
	{
		yaw(l_angle);
	}

}

void Dragon::ChaseState()
{
	if (m_KnightPointer)
	{
		D3DXVECTOR3 l_TempDirection{ 0,0,0 }, l_TempNormal{ 0,0,0 };
		D3DXVECTOR3 l_KnightLoc{ 0,0,0 };
		m_KnightPointer->getPosition(&l_KnightLoc);

		//Calculate vector pointing from dragon to knight
		D3DXVec3Subtract(&l_TempDirection, &l_KnightLoc, &_pos);
		float l_dragon_knight_distance = D3DXVec3Length(&l_TempDirection);

		if(l_dragon_knight_distance <= (2*speed) )
		{
			m_mood = Dragon::idle;
			printf("Dragon ate you; O' the indignity & travesty!\n");
		}

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
