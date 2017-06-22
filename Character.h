#ifndef CHARACTER_H
#define CHARACTER_H
#include <memory>
#include <string>
#include "3rd-party\xanimator\include\XAnimator_lib.h"

#include "Camera.h"
#include "Mesh.h"
#include "GameTimer.h"

class Character : public Camera {
public:
	Character();
	~Character();
	enum state { IDLE, CHASE, FLEE, ATTACK, LUNGE, GOHOME };
	int m_State;
	virtual bool Initialize() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Reset() = 0;
    virtual void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data ) = 0;
	Mesh characterMesh;
	D3D::BoundingBox box;

	D3DXMATRIX P, T, S;

	Character* m_Enemy;
	void IdleState();
	void GoHome();
	void ChaseState();
	void AttackState();
	void FleeState();
	void LungeState();
	void SetEnemy(Character* Enemy) { m_Enemy = Enemy; }

	float m_FleeTimer;
	float m_AttackTimer;
	Camera thirdPersonCamera;
	float m_Health;

	bool isAuto;
	int currentState;
	//virtual enum STATES;

	float speed, strafeSpeed, turnSpeed;
};

#endif