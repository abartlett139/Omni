#ifndef KNIGHT_H
#define KNIGHT_H

#include "Character.h"

class UIDevice;

class Knight: public Character
{
public:
    Knight( );
    ~Knight( );
	UIDevice* m_Print;
	bool Initialize(std::shared_ptr<IXAnimator>);
	bool Initialize( );
    void Render( );
    void Update( );
    void Reset( );
    void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
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

    int m_prevYaw, m_prevPitch, m_CurrentYaw, m_CurrentPitch;
	int m_modelId;
	int m_numberOfAnimationSets;
	std::shared_ptr<IXAnimator> m_animator;
	const std::string m_file_path;


};

#endif
