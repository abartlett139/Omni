#include "Character.h"

Character::Character( LPDIRECT3DDEVICE9 Device )
{
    m_Device = Device;
	isAuto = true;
}

Character::~Character()
{
}

bool Character::LoadXFile( const std::string & filename, int startAnimation )
{
    // We only support one entity so if it already exists delete it
    if( m_Avatar )
    {
        delete m_Avatar;
        m_Avatar = 0;
    }
    // Create the entity
    m_Avatar = new AnimationX( m_Device );
    if( !m_Avatar->Load( filename ) )
    {
        delete m_Avatar;
        m_Avatar = 0;
        return false;
    }

    m_Avatar->SetAnimationSet( startAnimation );


    return true;
}

void Character::NextAnimation( )
{
    if( m_Avatar )
        m_Avatar->NextAnimation( );
}

void Character::AnimateFaster( )
{
    if( m_Avatar )
        m_Avatar->AnimateFaster( );
}

void Character::AnimateSlower( )
{
    if( m_Avatar )
        m_Avatar->AnimateSlower( );
}
