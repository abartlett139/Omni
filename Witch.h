#ifndef WITCH_H
#define WITCH_H
#include "Character.h"
#include "PSYS.h"
#include "3rd-party\xanimator\include\XAnimator_lib.h"

class Witch: public Character
{
public:
    Witch( );
    ~Witch( );

    bool Initialize( );
	bool Initialize(std::shared_ptr<IXAnimator>);
	void Render( );
    void Update( );
    void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
    void Reset( );

    D3DXMATRIX getRearView( );
    D3DXMATRIX getSideView( );

    Camera thirdPersonCamera;

	void IdleState();
	void GoHome();
	void ChaseState();
	void AttackState();
	void FleeState();
	void LungeState();
	void SetEnemy(Character* Enemy) { m_Enemy = Enemy; }

	int m_modelId;
	int m_numberOfAnimationSets;
	std::shared_ptr<IXAnimator> m_animator;
	const std::string m_file_path;
	PSYS::ParticleGun* magic = 0;

};

#endif
