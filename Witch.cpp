#include "Witch.h"
#include "stdio.h"


Witch::Witch( ):m_modelId(0), m_file_path("models/enchantress_idle.x")
{
}


Witch::~Witch( )
{
}

bool Witch::Initialize( )
{
	return false;
}

bool Witch::Initialize(std::shared_ptr<IXAnimator>xAnimator)
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
	D3DXMatrixScaling(&S, 1.0f, 1.0f, 1.0f);

	magic = new PSYS::ParticleGun(this);
	magic->Init("particles/flare.dds");

	return true;
}

void Witch::Render( )
{
	//	render particle effect
	//magic->Render( );

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

void Witch::Update( )
{
    //	update third person camera position (rear view)
    thirdPersonCamera._pos = ((-_look * 20.0) + (_up * 10.0f)) + _pos;

}

void Witch::GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{

    if( !isAuto )
    {
        //if( GetAsyncKeyState( VK_SPACE ) & 0x8000f )
        //{
        //    magic->AddParticle( );
        //}

        if( GetAsyncKeyState( VK_RIGHT ) & 0x8000f )
        {
            yaw( timer.DeltaTime( )* turnSpeed );
        }
        if( GetAsyncKeyState( VK_LEFT ) & 0x8000f )
        {
            yaw( -timer.DeltaTime( ) * turnSpeed );
        }
        if( GetAsyncKeyState( VK_UP ) & 0x8000f )
        {
            pitch( -timer.DeltaTime( )* turnSpeed );
        }
        if( GetAsyncKeyState( VK_DOWN ) & 0x8000f )
        {
            pitch( timer.DeltaTime( ) * turnSpeed );
        }
        if( GetAsyncKeyState( 'W' ) & 0x8000f )
        {
            walk( timer.DeltaTime( ) * speed );
        }
        if( GetAsyncKeyState( 'S' ) & 0x8000f )
        {
            walk( -timer.DeltaTime( ) * speed );
        }
        if( GetAsyncKeyState( 'A' ) & 0x8000f )
        {
            strafe( -timer.DeltaTime( ) * strafeSpeed );
        }
        if( GetAsyncKeyState( 'D' ) & 0x8000f )
        {
            strafe( timer.DeltaTime( ) * strafeSpeed );
        }
    }
	else
	{
		printf("Skeleton error: Skeleton should be AI driven\n");
	}
	//else {
    //	do ai for knight here
    //	maybe strafe back and forth or whatever
    //}

}

void Witch::Reset( )
{
    //	set start position
    _pos = { 900.0f, 0.0f, -900.0f };
}

D3DXMATRIX Witch::getRearView( )
{
    D3DXMATRIX V;
    D3DXMatrixLookAtLH( &V, &thirdPersonCamera._pos, &D3DXVECTOR3( _pos.x, box.MAX.y, _pos.z ), &D3DXVECTOR3( 0, 1, 0 ) );
    return V;
}

D3DXMATRIX Witch::getSideView( )
{
    D3DXMATRIX V;
    thirdPersonCamera._pos = ((_right * 500.0f) + (_up * 50.0f)) + _pos;
    D3DXMatrixLookAtLH( &V, &thirdPersonCamera._pos, &D3DXVECTOR3( _pos.x, box.MAX.y, _pos.z ), &D3DXVECTOR3( 1, 0, 0 ) );

    return V;
}

void Witch::IdleState()
{
	Character::IdleState();
}

void Witch::GoHome()
{
	Character::GoHome();
}

void Witch::ChaseState()
{
	Character::ChaseState();
}

void Witch::AttackState()
{
	Character::AttackState();
}

void Witch::FleeState()
{
	Character::FleeState();
}

void Witch::LungeState()
{
	Character::LungeState();
}
