#ifndef BRIDGE_H
#define BRIDGE_H

#include "Mesh.h"

class Bridge
{
public:
	Bridge();
	~Bridge();

	void Init();
	void Render();

	Mesh mesh;
	D3D::BoundingBox box;

	D3DXMATRIX T, S;
};

#endif