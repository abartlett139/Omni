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


	D3DXMATRIX getRearView();

	Camera thirdPersonCamera;

	Terrain * terrain;

	bool isOnPlatform;
};




#endif