#ifndef ANIMATEDMESHHIERARCHY_H
#define ANIMATEDMESHHIERARCHY_H

#include "D3D.h"
#include <string>

class AnimatedMeshHierarchy : public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(LPCSTR NAME, LPD3DXFRAME *retNewFrame);

	STDMETHOD(CreateMeshContainer)(LPCSTR Name, CONST D3DXMESHDATA * meshData, CONST D3DXMATERIAL * materials, CONST D3DXEFFECTINSTANCE * effectInstances, DWORD numMaterials, CONST DWORD * adjacency, LPD3DXSKININFO skinInfo, LPD3DXMESHCONTAINER * retNewMeshContainer);

	STDMETHOD(DestroyFrame)(LPD3DXFRAME frameToFree);

	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER meshContainerToFree);
};

#endif