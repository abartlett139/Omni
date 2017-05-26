#ifndef KNIGHT_H
#define KNIGHT_H

#include "Character.h"

class Knight : public Character
{
public:
	Knight();
	~Knight();

	bool Initialize();
	void Render();
	void Update();
	void Reset();

	D3DXMATRIX getRearView();
	D3DXMATRIX getSideView();

	Camera thirdPersonCamera;

};

#endif
