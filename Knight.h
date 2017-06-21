#ifndef KNIGHT_H
#define KNIGHT_H

#include "Character.h"

class Knight : public Character {
public:

	Knight();
	~Knight();
	bool Initialize(Terrain *terrainPTR);
	void Render();
	void Update();
	void Reset();
	void GetMessages(UINT msg, WPARAM wParam, LPARAM lParam, void * Data);
	void RemoveHealth(float amountToRemove);

	D3DXMATRIX getRearView();

	Camera thirdPersonCamera;

	Terrain * terrain;

	bool isOnPlatform;
};




#endif