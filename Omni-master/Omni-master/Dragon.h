#ifndef DRAGON_H
#define DRAGON_H

#include "Character.h"

class Dragon : public Character
{
public:
	Dragon();
	~Dragon();

	bool Initialize();
	void Render();
	void Update();
	void Reset();

	D3DXMATRIX getRearView();
	D3DXMATRIX getSideView();

	Camera thirdPersonCamera;
};

#endif