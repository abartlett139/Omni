#ifndef DRAGON_H
#define DRAGON_H
#include <vector>
#include "Character.h"

class Dragon : public Character
{
public:

	Dragon();
	~Dragon();

	bool Initialize(Terrain *terrain);
	void Render();
    void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data );
    void Update( );
	void Reset();
	


	///Variables needed for independent 
	Character* m_KnightPointer;
	std::vector<D3DXVECTOR3> mapLocations;
	///Dragon State Functions
	void IdleState();
	void ChaseState();
	void AttackState();
	void FleeState();
	void SetEnemy(Character* Enemy) { m_KnightPointer = Enemy; }
	D3DXMATRIX getRearView();
	D3DXMATRIX getSideView();

	Camera thirdPersonCamera;

	// Enum describing the current motivation of the dragon
	//enum Mood { idle, chase, attack, flee } m_mood;

};

#endif