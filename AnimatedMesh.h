#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

#include "AnimatedMeshHierarchy.h"

#define kMoveTransitionTime  0.25f

class AnimatedMesh
{
public:
	AnimatedMesh();
	~AnimatedMesh();

	void UpdateFrameMatrices(const D3DXFRAME *frameBase, const D3DXMATRIX *parentMatrix);
	void DrawFrame(LPD3DXFRAME frame) const;
	void DrawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase) const;
	void SetupBoneMatrices(D3D::D3DXFRAME_EXTENDED *pFrame, LPD3DXMATRIX pParentMatrix);

	bool Load(char *fileName);
	void FrameMove(float elapsedTime, const D3DXMATRIX *matWorld);

	void Render() const;
	void SetAnimationSet(unsigned int index);

	void NextAnimation();
	void AnimateFaster();
	void AnimateSlower();

	unsigned int GetCurrentAnimationSet() const { return m_currentAnimationSet; }
	std::string GetAnimationSetName(unsigned int index);
	std::string GetFilename() const { return m_filename; }

	LPD3DXFRAME m_frameRoot;
	LPD3DXANIMATIONCONTROLLER m_animController;
	D3D::D3DXMESHCONTAINER_EXTENDED* m_firstMesh;

	D3DXMATRIX *m_boneMatrices;
	int m_maxBones;

	unsigned int m_currentAnimationSet;
	unsigned int m_numAnimationSets;
	unsigned int m_currentTrack;
	float m_currentTime;
	float m_speedAdjust;

	std::string m_filename;

	D3DXVECTOR3 m_sphereCentre;
	float m_sphereRadius;
};

#endif