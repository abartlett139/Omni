#ifndef SKELE_H
#define SKELE_H

#include <memory>
#include <string>

#include "Character.h"
#include "PSYS.h"
#include "3rd-party\xanimator\include\XAnimator_lib.h"

class Skele: public Character
{
public:
    Skele( );
    ~Skele( );

	// We are not using this, but have to override it from base class
	bool Initialize( Terrain * terrain);

	// This initialize takes the animator object from the game world, and initializes
	// our own member copy
	bool Initialize( std::shared_ptr<IXAnimator> );
    void Render( );
    void Update( );
    void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
    void Reset( );

    D3DXMATRIX getRearView( );
    D3DXMATRIX getSideView( );

    Camera thirdPersonCamera;

    PSYS::ParticleGun* magic = 0;

	int m_modelId;
	int m_numberOfAnimationSets;
	std::shared_ptr<IXAnimator> m_animator;
	const std::string m_file_path;

	// Enum describing animations and can be used to as indices for the library
	enum model_animations{ shaking_head, walking, taking_damage, dancing, dying };

};

#endif
