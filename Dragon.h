#ifndef DRAGON_H
#define DRAGON_H
#include <vector>
#include "Character.h"

class Dragon : public Character
{
public:
	Dragon();
	~Dragon();
	bool Initialize();
	void Render();
    void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
    void Update( );
	void Reset();
	bool Initialize(std::shared_ptr<IXAnimator>);

	int m_modelId;
	int m_numberOfAnimationSets;
	std::shared_ptr<IXAnimator> m_animator;
	const std::string m_file_path;

	///Variables needed for independent 
	Character* m_KnightPointer;
	std::vector<D3DXVECTOR3> mapLocations;
	///Dragon State Functions
	void IdleState();
	void GoHome();
	void ChaseState();
	void AttackState();
	void FleeState();
	void LungeState();
	void SetEnemy(Character* Enemy) { m_KnightPointer = Enemy; }



	D3DXMATRIX getRearView();
	D3DXMATRIX getSideView();
	// Enum describing the current motivation of the dragon
	//enum Mood { idle, chase, attack, flee } m_mood;

};

#endif