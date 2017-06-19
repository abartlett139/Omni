#include "Bridge.h"



Bridge::Bridge()
{
}


Bridge::~Bridge()
{
}

void Bridge::Init()
{
	mesh.Initialize("models/bridge.x");

	D3DXMatrixScaling(&S, 8.0f, 8.0f, 8.0f);
	D3DXMatrixTranslation(&T, -350.0f, 20.0f, -380.0f);
}

void Bridge::Render()
{
	//	set the world transform and render
	Device->SetTransform(D3DTS_WORLD, &(S*T));
	mesh.Render();

	//	update the bounding box coordinates to match the mesh
	D3DXVec3TransformCoord(&box.MIN, &mesh.min, &(S*T));
	D3DXVec3TransformCoord(&box.MAX, &mesh.max, &(S*T));
}
