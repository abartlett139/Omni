#include "AnimatedMeshHierarchy.h"

HRESULT AnimatedMeshHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *retNewFrame) 
{
	*retNewFrame = 0;

	D3D::D3DXFRAME_EXTENDED *newFrame = new D3D::D3DXFRAME_EXTENDED;
	ZeroMemory(newFrame, sizeof(D3D::D3DXFRAME_EXTENDED));

	D3DXMatrixIdentity(&newFrame->TransformationMatrix);
	D3DXMatrixIdentity(&newFrame->exCombinedTransformationMatrix);

	newFrame->pMeshContainer = 0;
	newFrame->pFrameSibling = 0;
	newFrame->pFrameFirstChild = 0;

	*retNewFrame = newFrame;

	if (Name && strlen(Name)) {
		newFrame->Name = (LPSTR)Name;
	}

	return S_OK;
}

HRESULT AnimatedMeshHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * meshData, CONST D3DXMATERIAL * materials, CONST D3DXEFFECTINSTANCE * effectInstances, DWORD numMaterials, CONST DWORD * adjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * retNewMeshContainer)
{
	D3D::D3DXMESHCONTAINER_EXTENDED *newMeshContainer = new D3D::D3DXMESHCONTAINER_EXTENDED;
	ZeroMemory(newMeshContainer, sizeof(D3D::D3DXMESHCONTAINER_EXTENDED));

	*retNewMeshContainer = 0;

	if (Name && strlen(Name)) {
		newMeshContainer->Name = (LPSTR)Name;
	}

	if (meshData->Type != D3DXMESHTYPE_MESH) {
		DestroyMeshContainer(newMeshContainer);
		return E_FAIL;
	}
	
	newMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	DWORD dwFaces = meshData->pMesh->GetNumFaces();
	newMeshContainer->pAdjacency = new DWORD[dwFaces * 3];
	memcpy(newMeshContainer->pAdjacency, adjacency, sizeof(DWORD) & dwFaces * 3);

	newMeshContainer->MeshData.pMesh = meshData->pMesh;
	newMeshContainer->MeshData.pMesh->AddRef();

	newMeshContainer->NumMaterials = max(numMaterials, 1);
	newMeshContainer->exMaterials = new D3DMATERIAL9[newMeshContainer->NumMaterials];
	newMeshContainer->exTextures = new LPDIRECT3DTEXTURE9[newMeshContainer->NumMaterials];

	ZeroMemory(newMeshContainer->exTextures, sizeof(LPDIRECT3DTEXTURE9) * newMeshContainer->NumMaterials);

	//	load textrues and copy materials over
	//	else if there are no materials, create one
	if (numMaterials > 0) {
		for (DWORD i = 0; i < numMaterials; ++i) {
			newMeshContainer->exTextures[i] = 0;
			newMeshContainer->exMaterials[i] = materials[i].MatD3D;

			if (materials[i].pTextureFilename) {
				newMeshContainer->exTextures[i] = D3D::LoadTexture(materials[i].pTextureFilename);
			}
		}
	}
	else {
		ZeroMemory(&newMeshContainer->exMaterials[0], sizeof(D3DMATERIAL9));
		newMeshContainer->exMaterials[0].Diffuse.r = 0.5f;
		newMeshContainer->exMaterials[0].Diffuse.g = 0.5f;
		newMeshContainer->exMaterials[0].Diffuse.b = 0.5f;
		newMeshContainer->exMaterials[0].Specular = newMeshContainer->exMaterials[0].Diffuse;
		newMeshContainer->exTextures[0] = 0;
	}


	if (pSkinInfo) {
		newMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		UINT numBones = pSkinInfo->GetNumBones();
		newMeshContainer->exBoneOffsets = new D3DXMATRIX[numBones];

		newMeshContainer->exFrameCombinedMatrixPointer = new D3DXMATRIX*[numBones];

		for (UINT i = 0; i < numBones; i++) {
			newMeshContainer->exBoneOffsets[i] = *(newMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}
	else {
		newMeshContainer->pSkinInfo = 0;
		newMeshContainer->exBoneOffsets = 0;
		newMeshContainer->exSkinMesh = 0;
		newMeshContainer->exFrameCombinedMatrixPointer = 0;
	}

	Device->Release();

	*retNewMeshContainer = newMeshContainer;

	return S_OK;
}

HRESULT AnimatedMeshHierarchy::DestroyFrame(LPD3DXFRAME frameToFree)
{
	D3D::D3DXFRAME_EXTENDED *frame = (D3D::D3DXFRAME_EXTENDED*)frameToFree;

	if (frame->Name)
		delete[]frame->Name;
	delete frame;

	return S_OK;
}

HRESULT AnimatedMeshHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER meshContainerBase)
{
	D3D::D3DXMESHCONTAINER_EXTENDED* meshContainer = (D3D::D3DXMESHCONTAINER_EXTENDED*)meshContainerBase;
	if (!meshContainer)
		return S_OK;

	if (meshContainer->Name) {
		delete[]meshContainer->Name;
		meshContainer->Name = 0;
	}

	if (meshContainer->exMaterials) {
		delete[]meshContainer->exMaterials;
		meshContainer->exMaterials = 0;
	}

	if (meshContainer->exTextures) {
		for (UINT i = 0; i < meshContainer->NumMaterials; i++) {
			if (meshContainer->exTextures[i])
				meshContainer->exTextures[i]->Release();
		}
	}


	if (meshContainer->exTextures)
		delete[]meshContainer->exTextures;

	if (meshContainer->pAdjacency)
		delete[]meshContainer->pAdjacency;

	if (meshContainer->exBoneOffsets) {
		delete[]meshContainer->exBoneOffsets;
		meshContainer->exBoneOffsets = 0;
	}

	if (meshContainer->exFrameCombinedMatrixPointer) {
		delete[]meshContainer->exFrameCombinedMatrixPointer;
		meshContainer->exFrameCombinedMatrixPointer = 0;
	}

	if (meshContainer->exSkinMesh) {
		meshContainer->exSkinMesh->Release();
		meshContainer->exSkinMesh = 0;
	}

	if (meshContainer->MeshData.pMesh) {
		meshContainer->MeshData.pMesh->Release();
		meshContainer->MeshData.pMesh = 0;
	}

	if (meshContainer->pSkinInfo) {
		meshContainer->pSkinInfo->Release();
		meshContainer->pSkinInfo = 0;
	}

	delete meshContainer;
	meshContainer = 0;

	return S_OK;

}


