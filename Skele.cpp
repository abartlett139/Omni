#include "Skele.h"

Skele::Skele( ): m_modelId( 0 ), m_file_path( "models/bones_all.x" )
{
}

Skele::~Skele( )
{
}

// We are not using this, but have to override it from base class
bool Skele::Initialize()
{
	return false;
}

bool Skele::Initialize(std::shared_ptr<IXAnimator> xAnimator)
{
    //	set speed variables
    speed = 50.0f;
    strafeSpeed = 50.0f;
    turnSpeed = 2.0f;

    //	set start position
	Reset();

    //	Use the animation librayr to initialize the physical mesh object
	m_animator = xAnimator;
	if( !m_animator->LoadXFile( m_file_path.c_str( ), &m_modelId) )
	{
		printf("IXAnimator library could not load %s", m_file_path.c_str() );
		return false;
	}

	m_numberOfAnimationSets = m_animator->GetNumberOfAnimationSets(m_modelId);

    //	set the scale
    D3DXMatrixScaling( &S, 7.0f, 7.0f, 7.0f );

    magic = new PSYS::ParticleGun( this );
    magic->Init( "particles/flare.dds" );

    return true;
}

void Skele::Render( )
{
    //	render particle effect
    //magic->Render( );

    //	the position and rotation translation matrix is the inverse of the characters's view matrix
    getViewMatrix( &T );
    D3DXMatrixInverse( &T, NULL, &T );
    P = S*T;

	// The render function expects the time to be in milliseconds
	// timer.DeltaTime() returns units in seconds
	m_animator->Render(m_modelId, P, timer.DeltaTime()*1000.0f );

    //	update the bounding box coordinates to match the mesh
    D3DXVec3TransformCoord( &box.MIN, &characterMesh.min, &P );
    D3DXVec3TransformCoord( &box.MAX, &characterMesh.max, &P );
}

void Skele::Update( )
{
    //	update third person camera position (rear view)
    thirdPersonCamera._pos = ((-_look * 20.0) + (_up * 10.0f)) + _pos;

    //	update particle effect
    //magic->Update( );
}

void Skele::GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data )
{
    if( isAuto )
    {
		switch (msg)
		{
		case WM_CHAR:
			switch( wParam )
			{
			case 'z':
				m_animator->ChangeAnimationSet(m_modelId, Skele::shaking_head);
				break;
			case 'x':
				m_animator->ChangeAnimationSet(m_modelId, Skele::walking);
				break;
			case 'c':
				m_animator->ChangeAnimationSet(m_modelId, Skele::taking_damage);
				break;
			case 'v':
				m_animator->ChangeAnimationSet(m_modelId, Skele::dancing);
				break;
			case 'b':
				m_animator->ChangeAnimationSet(m_modelId, Skele::dying);
				break;
			default:
				break;
			} 
			break;
		default:
			break;
		}
	}
    else 
	{
		printf("Skeleton error: Skeleton should be AI driven\n");
	}
}

void Skele::Reset( )
{
    //	set start position
    _pos = { 900.0f, 0.0f, -850.0f };
}

D3DXMATRIX Skele::getRearView( )
{
    D3DXMATRIX V;
    D3DXMatrixLookAtLH( &V, &thirdPersonCamera._pos, &D3DXVECTOR3( _pos.x, box.MAX.y, _pos.z ), &D3DXVECTOR3( 0, 1, 0 ) );
    return V;
}

D3DXMATRIX Skele::getSideView( )
{
    D3DXMATRIX V;
    thirdPersonCamera._pos = ((_right * 500.0f) + (_up * 50.0f)) + _pos;
    D3DXMatrixLookAtLH( &V, &thirdPersonCamera._pos, &D3DXVECTOR3( _pos.x, box.MAX.y, _pos.z ), &D3DXVECTOR3( 1, 0, 0 ) );

    return V;
}
