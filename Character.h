#ifndef CHARACTER_H 
#define CHARACTER_H

#include "Camera.h"
#include "Mesh.h"
#include "GameTimer.h"
#include "Terrain.h"

struct HealthBar {
	HealthBar(D3DXVECTOR3 *position, int maxHealth) {
		bar = D3D::LoadTexture("textures/healthbar.png");
		fill = D3D::LoadTexture("textures/healthBarFill.png");
		fillPos = pos + D3DXVECTOR3(69.0f, 49.0f, 0.0f);
		maxHP = HP = maxHealth;
		rect.top = 0;
		rect.left = 0;
		rect.bottom = 30;
		rect.right = 255;
	}

	void Render(ID3DXSprite* sprite) {
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw(bar, NULL, NULL, &pos, D3D::WHITE);
		sprite->Draw(fill, &rect, NULL, &fillPos, D3D::WHITE);
		sprite->End();
	}

	void Update(float health) {
		HP = health;
		rect.right = 255 * (HP / maxHP);
	}

	IDirect3DTexture9 *bar, *fill;	//	textures for the background and fill of the healthbar
	RECT rect;	//	source rect that tells it how much of the fill bar to draw
	D3DXVECTOR3 pos, fillPos;	//	positions for the textures
	float maxHP, HP;	//	holds the characters health stats
};

class Character : public Camera {
public:
	Character();
	~Character();
	enum state { IDLE, CHASE, FLEE, ATTACK, JUMP, FALL };
	int m_State;
	virtual bool Initialize(Terrain *terrain) = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Reset() = 0;
	virtual void GetMessages( UINT msg, WPARAM wParam, LPARAM lParam, void * Data ) = 0;
	void RemoveHealth(float amountToRemove);

	Mesh characterMesh;
	D3D::BoundingBox box;

	D3DXMATRIX P, T, S;

	bool isAuto;
	int currentState;

	float speed, strafeSpeed, turnSpeed;

	D3DXVECTOR3 prevPos, velocity;

	bool canJump;

	float health, maxHealth;
	HealthBar* healthBar;
};

#endif