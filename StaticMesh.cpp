#include "StaticMesh.h"



StaticMesh::StaticMesh()
{
}


StaticMesh::~StaticMesh()
{
}

bool StaticMesh::Initialize(char * fileName, float s)
{
	objectMesh.Initialize(fileName);
	D3DXMatrixScaling(&S, s, s, s);
	scale = s;
	return true;
}

void StaticMesh::Render(D3DXVECTOR3 * pos)
{
	D3DXMatrixTranslation(&T, pos->x / scale, pos->y / scale, pos->z / scale);
	W = T*S;
	Device->SetTransform(D3DTS_WORLD, &W);
	objectMesh.Render();

	D3DXVec3TransformCoord(&box.MIN, &objectMesh.min, &W);
	D3DXVec3TransformCoord(&box.MAX, &objectMesh.max, &W);

}

bool StaticMesh::PlatformCollision(Character * character)
{
	if (character->_pos.x <= box.MAX.x && character->_pos.x >= box.MIN.x && character->_pos.z <= box.MAX.z && character->_pos.z >= box.MIN.z) {
			return true;
	}
	return false;
}
