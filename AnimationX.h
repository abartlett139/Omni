#ifndef ANIMATIONX_H
#define ANIMATIONX_H
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Utility.h"

/**
* \struct D3DXMESHCONTAINER_EXTENDED
* \brief Structure derived from D3DXMESHCONTAINER and extended with some app-specific
* info that will be stored with each mesh. To help detect which data is extended and which is
* from the base each variable begins with ex
* \author Keith Ditchburn
*/
struct D3DXMESHCONTAINER_EXTENDED : public D3DXMESHCONTAINER
{
    // The base D3DXMESHCONTAINER has a pMaterials member which is a D3DXMATERIAL structure 
    // that contains a texture filename and material data. It is easier to ignore this and 
    // instead store the data in arrays of textures and materials in this extended structure:
    IDirect3DTexture9**  exTextures;		// Array of texture pointers  
    D3DMATERIAL9*		 exMaterials;		// Array of materials

                                            // Skinned mesh variables
    ID3DXMesh*           exSkinMesh;			// The skin mesh
    D3DXMATRIX*			 exBoneOffsets;			// The bone matrix Offsets, one per bone
    D3DXMATRIX**		 exFrameCombinedMatrixPointer;	// Array of frame matrix pointers
};

/**
* \struct D3DXFRAME_EXTENDED
* \brief Structure derived from D3DXFRAME and extended so we can add some app-specific
* info that will be stored with each frame. To help detect which data is extended and which is
* from the base each variable begins with ex
* \author Keith Ditchburn
*/
struct D3DXFRAME_EXTENDED : public D3DXFRAME
{
    D3DXMATRIX exCombinedTransformationMatrix;
};


/**
* \class CMeshHierarchy
* \brief This class defines a number of callbacks required by the D3DXLoadMeshHierarchyFromX function.
* The required functions are defined by the abstract interface class: ID3DXAllocateHierarchy
* \author Keith Ditchburn
*/
class CMeshHierarchy : public ID3DXAllocateHierarchy
{
public:
    // callback to create a D3DXFRAME extended object and initialize it
    STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME *retNewFrame);

    // callback to create a D3DXMESHCONTAINER extended object and initialize it
    STDMETHOD(CreateMeshContainer)(LPCSTR Name, CONST D3DXMESHDATA * meshData,
        CONST D3DXMATERIAL * materials, CONST D3DXEFFECTINSTANCE * effectInstances,
        DWORD numMaterials, CONST DWORD * adjacency, LPD3DXSKININFO skinInfo,
        LPD3DXMESHCONTAINER * retNewMeshContainer);

    // callback to release a D3DXFRAME extended object
    STDMETHOD(DestroyFrame)(LPD3DXFRAME frameToFree);

    // callback to release a D3DXMESHCONTAINER extended object
    STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER meshContainerToFree);
};

/*
This class represents an x file animation
It loads the .x file and carries out the animation update and rendering
*/
class AnimationX
{
private:
	LPDIRECT3DDEVICE9 m_d3dDevice = NULL; // note: a pointer copy (not a good idea but for simplicities sake)

                                   // Direct3D objects required for animation
    LPD3DXFRAME                 m_frameRoot;
    LPD3DXANIMATIONCONTROLLER   m_animController;
    D3DXMESHCONTAINER_EXTENDED* m_firstMesh;

    // Bone data
    D3DXMATRIX *m_boneMatrices;
    int m_maxBones;

    // Animation variables
    unsigned int m_currentAnimationSet;
    unsigned int m_numAnimationSets;
    unsigned int m_currentTrack;
    float m_currentTime;
    float m_speedAdjust;

    // Bounding sphere (for camera placement)
    D3DXVECTOR3 m_sphereCentre;
    float m_sphereRadius;
    //world matrix
    D3DXMATRIX* m_world;
    float scale;

    std::string m_filename;

    void UpdateFrameMatrices(const D3DXFRAME *frameBase, const D3DXMATRIX *parentMatrix);
    void DrawFrame(LPD3DXFRAME frame) const;
    void DrawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase) const;
    void SetupBoneMatrices(D3DXFRAME_EXTENDED *pFrame, LPD3DXMATRIX pParentMatrix);
public:
    AnimationX(IDirect3DDevice9* d3dDevice);
    ~AnimationX(void);

    D3DXVECTOR3 min, max;
    bool Load(const std::string &filename);
    void FrameMove(float elapsedTime,  D3DXMATRIX *matWorld);

    void Render() const;
    void SetAnimationSet(unsigned int index);

    void NextAnimation();
    void AnimateFaster();
    void AnimateSlower();

    D3DXVECTOR3 GetInitialCameraPosition() const;
    unsigned int GetCurrentAnimationSet() const { return m_currentAnimationSet; }
    std::string GetAnimationSetName(unsigned int index);
    std::string GetFilename() const { return m_filename; }
};

#endif // !ANIMATIONX_H
