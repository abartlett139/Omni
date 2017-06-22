#include "Dragon.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
//
Dragon::Dragon(): m_KnightPointer( nullptr )
{
	Dragon::m_State = IDLE;
	m_FleeTimer = 0.0f;
	m_AttackTimer = 0.0f;
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
				m_State = Dragon::IDLE;
				printf("Dragon taking a smoke break, sharpening her claws\n");
				break;
			case '.':
				printf("Dragon hungry; chasing silly knight\n");
				m_State = Dragon::CHASE;
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
	if (m_Enemy)
	{
		if (isAuto)
		{
			switch (m_State)
			{
			case IDLE:
				IdleState();
				break;
			case CHASE:
				ChaseState();
				break;
			case FLEE:
				FleeState();
				break;
			case ATTACK:
				AttackState();
				break;
			case LUNGE:
				LungeState();
				break;
			case GOHOME:
				GoHome();
				break;
			default:
				break;
			}
		}
		else
		{
			printf("Dragon error: Dragon should be AI driven\n");
		}

}
	//	update third person camera position (rear view)
	thirdPersonCamera._pos = ((-_look * 20.0) + (_up * 10.0f)) + _pos;
}

void Dragon::Reset()
{
	//	set start position
	_pos = { -900.0f, 0.0f, 900.0f };

}

bool Dragon::Initialize(std::shared_ptr<IXAnimator>xAnimator)
{
	//	set speed variables
	speed = 50.0f;
	strafeSpeed = 50.0f;
	turnSpeed = 2.0f;

	//	set start position
	Reset();

	//	Use the animation librayr to initialize the physical mesh object
	m_animator = xAnimator;
	if (!m_animator->LoadXFile(m_file_path.c_str(), &m_modelId))
	{
		printf("IXAnimator library could not load %s", m_file_path.c_str());
		return false;
	}

	m_numberOfAnimationSets = m_animator->GetNumberOfAnimationSets(m_modelId);

	//	set the scale
	D3DXMatrixScaling(&S, 7.0f, 7.0f, 7.0f);


	return true;
}

// Trying to get the dragon to always face the knight
void Dragon::IdleState()
{
	Character::IdleState();
}

void Dragon::GoHome()
{
	Character::GoHome();
}

void Dragon::ChaseState()
{
	Character::ChaseState();
}

void Dragon::AttackState()
{
	Character::AttackState();
	
}

void Dragon::FleeState()
{
	Character::FleeState();
}

void Dragon::LungeState()
{
	Character::LungeState();
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
