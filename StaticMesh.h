#ifndef STATICMESH_H
#define STATICMESH_H

#include "Mesh.h"
#include "Character.h"

class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();

	bool Initialize(char *fileName, float scale);
	void Render(D3DXVECTOR3 *pos);
	bool PlatformCollision(Character *character);

	D3D::BoundingBox box;

	D3DXMATRIX W, T, R, S;

	Mesh objectMesh;

	float scale;
};

#endif