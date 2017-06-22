#include "Knight.h"
#include <Windowsx.h>
#include "UI.h"

Knight::Knight()
{
    m_prevYaw=0, m_prevPitch=0, m_CurrentYaw=0, m_CurrentPitch=0;
	m_Print = new UIDevice(graphics.GetDevice());
}


Knight::~Knight()
{
}

bool Knight::Initialize(std::shared_ptr<IXAnimator>xAnimator)
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

bool Knight::Initialize()
{
	return false;
}

void Knight::Render()
{
	IDirect3DDevice9* Device = graphics.GetDevice();
	//	the position and rotation translation matrix is the inverse of the characters's view matrix
	getViewMatrix(&T);
	D3DXMatrixInverse(&T, NULL, &T);
	P = S*T;

	// The render function expects the time to be in milliseconds
	// timer.DeltaTime() returns units in seconds
	m_animator->Render(m_modelId, P, timer.DeltaTime()*1000.0f);

	//	update the bounding box coordinates to match the mesh
	D3DXVec3TransformCoord(&box.MIN, &characterMesh.min, &P);
	D3DXVec3TransformCoord(&box.MAX, &characterMesh.max, &P);
}

void Knight::Update( )
{
    //	update third person camera position (rear view)
    thirdPersonCamera._pos = ((-_look * 20.0) + (_up * 10.0f)) + _pos;
}

void Knight::GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    if( !isAuto )
    {
        //using asynckeystate...
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
	//else {
		//	do ai for knight here
		//	maybe strafe back and forth or whatever
	//}	
}

void Knight::Reset()
{
	//	set start position
	_pos = { 900.0f, 0.0f, -900.0f };
}


D3DXMATRIX Knight::getRearView()
{
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box.MAX.y, _pos.z), &D3DXVECTOR3(0, 1, 0));
	return V;
}

D3DXMATRIX Knight::getSideView()
{
	D3DXMATRIX V;
	thirdPersonCamera._pos = ((_right * 500.0f) + (_up * 50.0f)) + _pos;
	D3DXMatrixLookAtLH(&V, &thirdPersonCamera._pos, &D3DXVECTOR3(_pos.x, box.MAX.y, _pos.z), &D3DXVECTOR3(1, 0, 0));

	return V;
}

void Knight::IdleState()
{
	Character::IdleState();
}

void Knight::GoHome()
{
	Character::GoHome();
}

void Knight::ChaseState()
{
	Character::ChaseState();
}

void Knight::AttackState()
{
	Character::AttackState();
}

void Knight::FleeState()
{
	Character::FleeState();
}

void Knight::LungeState()
{
	Character::LungeState();
}
