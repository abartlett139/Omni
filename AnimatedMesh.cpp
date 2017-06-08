#include "AnimatedMesh.h"



AnimatedMesh::AnimatedMesh() : m_speedAdjust(1.0f), m_firstMesh(0), m_currentTrack(0), m_currentTime(0), m_numAnimationSets(0), m_currentAnimationSet(0), m_maxBones(0), m_sphereRadius(0), m_sphereCentre(0,0,0), m_boneMatrices(0)
{
}


AnimatedMesh::~AnimatedMesh()
{
	if (m_animController) {
		m_animController->Release();
		m_animController = 0;
	}

	if (m_frameRoot) {
		AnimatedMeshHierarchy memoryAllocator;
		D3DXFrameDestroy(m_frameRoot, &memoryAllocator);
		m_frameRoot = 0;
	}

	if (m_boneMatrices) {
		delete[]m_boneMatrices;
		m_boneMatrices = 0;
	}
}

void AnimatedMesh::UpdateFrameMatrices(const D3DXFRAME * frameBase, const D3DXMATRIX * parentMatrix)
{
	D3D::D3DXFRAME_EXTENDED *currentFrame = (D3D::D3DXFRAME_EXTENDED*)frameBase;

	if (parentMatrix != NULL) {
		D3DXMatrixMultiply(&currentFrame->exCombinedTransformationMatrix, &currentFrame->TransformationMatrix, parentMatrix);
	}
	else {
		currentFrame->exCombinedTransformationMatrix = currentFrame->TransformationMatrix;
	}

	if (currentFrame->pFrameSibling != NULL)
		UpdateFrameMatrices(currentFrame->pFrameSibling, parentMatrix);

	if (currentFrame->pFrameFirstChild != NULL)
		UpdateFrameMatrices(currentFrame->pFrameFirstChild, &currentFrame->exCombinedTransformationMatrix);
}

void AnimatedMesh::DrawFrame(LPD3DXFRAME frame) const
{
	LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;
	while (meshContainer) {
		DrawMeshContainer(meshContainer, frame);
		meshContainer = meshContainer->pNextMeshContainer;
	}

	if (frame->pFrameSibling != NULL)
		DrawFrame(frame->pFrameSibling);

	if (frame->pFrameFirstChild != NULL)
		DrawFrame(frame->pFrameFirstChild);
}

void AnimatedMesh::DrawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase) const
{
	D3D::D3DXFRAME_EXTENDED *frame = (D3D::D3DXFRAME_EXTENDED*)frameBase;

	D3D::D3DXMESHCONTAINER_EXTENDED *meshContainer = (D3D::D3DXMESHCONTAINER_EXTENDED*)meshContainerBase;

	Device->SetTransform(D3DTS_WORLD, &frame->exCombinedTransformationMatrix);

	for (unsigned int iMaterial = 0; iMaterial < meshContainer->NumMaterials; iMaterial++) {
		Device->SetMaterial(&meshContainer->exMaterials[iMaterial]);
		Device->SetTexture(0, meshContainer->exTextures[iMaterial]);

		LPD3DXMESH pDrawMesh = (meshContainer->pSkinInfo) ? meshContainer->exSkinMesh : meshContainer->MeshData.pMesh;

		pDrawMesh->DrawSubset(iMaterial);
	}
}

void AnimatedMesh::SetupBoneMatrices(D3D::D3DXFRAME_EXTENDED * pFrame, LPD3DXMATRIX pParentMatrix)
{
	D3D::D3DXMESHCONTAINER_EXTENDED* pMesh = (D3D::D3DXMESHCONTAINER_EXTENDED*)pFrame->pMeshContainer;

	if (pMesh) {
		if (!m_firstMesh)
			m_firstMesh = pMesh;

		if (pMesh->pSkinInfo && pMesh->MeshData.pMesh) {
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
			pMesh->MeshData.pMesh->GetDeclaration(Declaration);

			pMesh->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED, Declaration, Device, &pMesh->exSkinMesh);


			m_maxBones = max(m_maxBones, (int)pMesh->pSkinInfo->GetNumBones());

			for (unsigned int i = 0; i < pMesh->pSkinInfo->GetNumBones(); i++) {
				D3D::D3DXFRAME_EXTENDED* pTempFrame = (D3D::D3DXFRAME_EXTENDED*)D3DXFrameFind(m_frameRoot, pMesh->pSkinInfo->GetBoneName(i));

				pMesh->exFrameCombinedMatrixPointer[i] = &pTempFrame->exCombinedTransformationMatrix;
			}
		}
	}

	if (pFrame->pFrameSibling)
		SetupBoneMatrices((D3D::D3DXFRAME_EXTENDED*)pFrame->pFrameSibling, pParentMatrix);

	if (pFrame->pFrameFirstChild)
		SetupBoneMatrices((D3D:: D3DXFRAME_EXTENDED*)pFrame->pFrameFirstChild, &pFrame->exCombinedTransformationMatrix);

}

bool AnimatedMesh::Load(char * fileName)
{
	AnimatedMeshHierarchy *memoryAllocator = new AnimatedMeshHierarchy;

	D3DXLoadMeshHierarchyFromX(fileName, D3DXMESH_MANAGED, Device, memoryAllocator, NULL, &m_frameRoot, &m_animController);

	delete memoryAllocator;
	memoryAllocator = 0;

	if (m_animController)
		m_numAnimationSets = m_animController->GetMaxNumAnimationSets();

	if (m_frameRoot) {
		
		SetupBoneMatrices((D3D::D3DXFRAME_EXTENDED*)m_frameRoot, NULL);

		m_boneMatrices = new D3DXMATRIX[m_maxBones];
		ZeroMemory(m_boneMatrices, sizeof(D3DXMATRIX)*m_maxBones);

		D3DXFrameCalculateBoundingSphere(m_frameRoot, &m_sphereCentre, &m_sphereRadius);
	}

	return true;
}

void AnimatedMesh::FrameMove(float elapsedTime, const D3DXMATRIX * matWorld)
{
	elapsedTime /= m_speedAdjust;
	m_currentTime += elapsedTime;
	if (m_animController != NULL)
		m_animController->AdvanceTime(elapsedTime, NULL);



	UpdateFrameMatrices(m_frameRoot, matWorld);

	D3D::D3DXMESHCONTAINER_EXTENDED* pMesh = m_firstMesh;
	if (pMesh && pMesh->pSkinInfo) {
		unsigned int Bones = pMesh->pSkinInfo->GetNumBones();

		for (unsigned int i = 0; i < Bones; ++i) {
			D3DXMatrixMultiply(&m_boneMatrices[i], &pMesh->exBoneOffsets[i], pMesh->exFrameCombinedMatrixPointer[i]);
		}
		void *srcPtr = 0;
		pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);

		void *destPtr = 0;
		pMesh->exSkinMesh->LockVertexBuffer(0, (void**)&destPtr);

		pMesh->pSkinInfo->UpdateSkinnedMesh(m_boneMatrices, NULL, srcPtr, destPtr);

		pMesh->exSkinMesh->UnlockVertexBuffer();
		pMesh->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void AnimatedMesh::Render() const
{
	if (m_frameRoot)
		DrawFrame(m_frameRoot);
}

void AnimatedMesh::SetAnimationSet(unsigned int index)
{
	if (index == m_currentAnimationSet)
		return;
	
	if (index >= m_numAnimationSets)
		index = 0;

	m_currentAnimationSet = index;

	LPD3DXANIMATIONSET set;
	m_animController->GetAnimationSet(m_currentAnimationSet, &set);

	DWORD newTrack = (m_currentTrack == 0 ? 1 : 0);

	m_animController->SetTrackAnimationSet(newTrack, set);
	set->Release();

	m_animController->UnkeyAllTrackEvents(m_currentTrack);
	m_animController->UnkeyAllTrackEvents(newTrack);

	m_animController->KeyTrackEnable(m_currentTrack, FALSE, m_currentTime + kMoveTransitionTime);
	m_animController->KeyTrackSpeed(m_currentTrack, 0.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);

	m_animController->KeyTrackWeight(m_currentTrack, 0.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);

	m_animController->SetTrackEnable(newTrack, TRUE);
	m_animController->KeyTrackSpeed(newTrack, 1.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);

	m_animController->KeyTrackWeight(newTrack, 1.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);

	m_currentTrack = newTrack;
}

void AnimatedMesh::NextAnimation()
{
	unsigned int newAnimationSet = m_currentAnimationSet + 1;
	if (newAnimationSet >= m_numAnimationSets)
		newAnimationSet = 0;

	SetAnimationSet(newAnimationSet);
}

void AnimatedMesh::AnimateFaster()
{
	if (m_speedAdjust > 0.1f)
		m_speedAdjust -= 0.1f;
}

void AnimatedMesh::AnimateSlower()
{
	m_speedAdjust += 0.1f;
}

std::string AnimatedMesh::GetAnimationSetName(unsigned int index)
{
	if (index >= m_numAnimationSets)
		return "Error: No set exists";

	LPD3DXANIMATIONSET set;
	m_animController->GetAnimationSet(m_currentAnimationSet, &set);

	std::string nameString(set->GetName());

	set->Release();

	return nameString;
}
