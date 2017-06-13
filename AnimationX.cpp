#include "AnimationX.h"

/**
* \brief callback called when a new frame is encountered during the .x file load
* \param Name - name of the frame
* \param ppNewFrame - output pointer assign our newly created frame
* \return success code
* \author Keith Ditchburn \date 17 July 2005
*/
HRESULT CMeshHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *retNewFrame)
{
    // Always a good idea to initialise a return pointer before proceeding
    *retNewFrame = 0;

    // Create a new frame using the derived version of the structure
    D3DXFRAME_EXTENDED *newFrame = new D3DXFRAME_EXTENDED;
    ZeroMemory(newFrame, sizeof(D3DXFRAME_EXTENDED));

    // Now fill in the data members in the frame structure

    // Now initialize other data members of the frame to defaults
    D3DXMatrixIdentity(&newFrame->TransformationMatrix);
    D3DXMatrixIdentity(&newFrame->exCombinedTransformationMatrix);

    newFrame->pMeshContainer = 0;
    newFrame->pFrameSibling = 0;
    newFrame->pFrameFirstChild = 0;

    // Assign the return pointer to our newly created frame
    *retNewFrame = newFrame;

    // The frame name (note: may be 0 or zero length)
    if (Name && strlen(Name))
    {
        newFrame->Name = CUtility::DuplicateCharString(Name);
        CUtility::DebugString("Added frame: " + ToString(Name) + "\n");
    }
    else
    {
        CUtility::DebugString("Added frame: no name given\n");
    }
    return S_OK;
}

/**
* \brief callback called when a mesh data is encountered during the .x file load
* \param Name - name of the Mesh (const char*)
* \param meshData - the mesh data
* \param materials - material array
* \param effectInstances - effect files / settings for the mesh
* \param numMaterials - number of materials in the mesh
* \param adjacency - adjacency array
* \param pSkinInfo - skin info.
* \param retNewMeshContainer - output pointer to assign our newly created mesh container
* \return success code
* \author Keith Ditchburn \date 17 July 2005
*/
HRESULT CMeshHierarchy::CreateMeshContainer(
    LPCSTR Name,
    CONST D3DXMESHDATA *meshData,
    CONST D3DXMATERIAL *materials,
    CONST D3DXEFFECTINSTANCE *effectInstances,
    DWORD numMaterials,
    CONST DWORD *adjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER* retNewMeshContainer)
{
    // Create a mesh container structure to fill and initilaise to zero values
    // Note: I use my extended version of the structure (D3DXMESHCONTAINER_EXTENDED) defined in MeshStructures.h
    D3DXMESHCONTAINER_EXTENDED *newMeshContainer = new D3DXMESHCONTAINER_EXTENDED;
    ZeroMemory(newMeshContainer, sizeof(D3DXMESHCONTAINER_EXTENDED));

    // Always a good idea to initialise return pointer before proceeding
    *retNewMeshContainer = 0;

    // The mesh name (may be 0) needs copying over
    if (Name && strlen(Name))
    {
        newMeshContainer->Name = CUtility::DuplicateCharString(Name);
        CUtility::DebugString("Added mesh: " + ToString(Name) + "\n");
    }
    else
    {
        CUtility::DebugString("Added Mesh: no name given\n");
    }

    // The mesh type (D3DXMESHTYPE_MESH, D3DXMESHTYPE_PMESH or D3DXMESHTYPE_PATCHMESH)
    if (meshData->Type != D3DXMESHTYPE_MESH)
    {
        // This demo does not handle mesh types other than the standard
        // Other types are D3DXMESHTYPE_PMESH (progressive mesh) and D3DXMESHTYPE_PATCHMESH (patch mesh)
        DestroyMeshContainer(newMeshContainer);
        return E_FAIL;
    }

    newMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

    // Adjacency data - holds information about triangle adjacency, required by the ID3DMESH object
    DWORD dwFaces = meshData->pMesh->GetNumFaces();
    newMeshContainer->pAdjacency = new DWORD[dwFaces * 3];
    memcpy(newMeshContainer->pAdjacency, adjacency, sizeof(DWORD) * dwFaces * 3);

    // Get the Direct3D device, luckily this is held in the mesh itself (Note: must release it when done with it)
    LPDIRECT3DDEVICE9 pd3dDevice = 0;
    meshData->pMesh->GetDevice(&pd3dDevice);

    // Changed 24/09/07 - can just assign pointer and add a ref rather than need to clone
    newMeshContainer->MeshData.pMesh = meshData->pMesh;
    newMeshContainer->MeshData.pMesh->AddRef();

    // Create material and texture arrays. Note that I always want to have at least one
    newMeshContainer->NumMaterials = max(numMaterials, 1);
    newMeshContainer->exMaterials = new D3DMATERIAL9[newMeshContainer->NumMaterials];
    newMeshContainer->exTextures = new LPDIRECT3DTEXTURE9[newMeshContainer->NumMaterials];

    ZeroMemory(newMeshContainer->exTextures, sizeof(LPDIRECT3DTEXTURE9) * newMeshContainer->NumMaterials);

    if (numMaterials>0)
    {
        // Load all the textures and copy the materials over		
        for (DWORD i = 0; i < numMaterials; ++i)
        {
            newMeshContainer->exTextures[i] = 0;
            newMeshContainer->exMaterials[i] = materials[i].MatD3D;

            if (materials[i].pTextureFilename)
            {
                std::string texturePath(materials[i].pTextureFilename);
                if (CUtility::FindFile(&texturePath))
                {
                    // Use the D3DX function to load the texture
                    if (FAILED(D3DXCreateTextureFromFile(pd3dDevice, texturePath.c_str(),
                        &newMeshContainer->exTextures[i])))
                    {
                        CUtility::DebugString("Could not load texture: " + texturePath + "\n");
                    }
                }
                else
                {
                    CUtility::DebugString("Could not find texture: " + ToString(materials[i].pTextureFilename) + "\n");
                }
            }
        }
    }
    else
        // make a default material in the case where the mesh did not provide one
    {
        ZeroMemory(&newMeshContainer->exMaterials[0], sizeof(D3DMATERIAL9));
        newMeshContainer->exMaterials[0].Diffuse.r = 0.5f;
        newMeshContainer->exMaterials[0].Diffuse.g = 0.5f;
        newMeshContainer->exMaterials[0].Diffuse.b = 0.5f;
        newMeshContainer->exMaterials[0].Specular = newMeshContainer->exMaterials[0].Diffuse;
        newMeshContainer->exTextures[0] = 0;
    }

    // If there is skin data associated with the mesh copy it over
    if (pSkinInfo)
    {
        // save off the SkinInfo
        newMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        // Need an array of offset matrices to move the vertices from the figure space to the bone's space
        UINT numBones = pSkinInfo->GetNumBones();
        newMeshContainer->exBoneOffsets = new D3DXMATRIX[numBones];

        // Create the arrays for the bones and the frame matrices
        newMeshContainer->exFrameCombinedMatrixPointer = new D3DXMATRIX*[numBones];

        // get each of the bone offset matrices so that we don't need to get them later
        for (UINT i = 0; i < numBones; i++)
            newMeshContainer->exBoneOffsets[i] = *(newMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));

        CUtility::DebugString("Mesh has skinning info.\n Number of bones is: " + ToString(numBones) + "\n");
        // Note: in the Microsoft samples a GenerateSkinnedMesh function is called here in order to prepare
        // the skinned mesh data for optimial hardware acceleration. As mentioned in the notes this sample
        // does not do hardware skinning but instead uses software skinning.
    }
    else
    {
        // No skin info so 0 all the pointers
        newMeshContainer->pSkinInfo = 0;
        newMeshContainer->exBoneOffsets = 0;
        newMeshContainer->exSkinMesh = 0;
        newMeshContainer->exFrameCombinedMatrixPointer = 0;
    }

    // When we got the device we caused an internal reference count to be incremented
    // So we now need to release it
    pd3dDevice->Release();

    // The mesh may contain a reference to an effect file
    if (effectInstances)
    {
        if (effectInstances->pEffectFilename)
            CUtility::DebugString("This .x file references an effect file. Effect files are not handled by this demo\n");
    }

    // Set the output mesh container pointer to our newly created one
    *retNewMeshContainer = newMeshContainer;

    return S_OK;
}

/**
* \brief callback called to deallocate the frame data
* \param the frame to free
* \return success result
* \author Keith Ditchburn \date 17 July 2005
*/
HRESULT CMeshHierarchy::DestroyFrame(LPD3DXFRAME frameToFree)
{
    // Convert to our extended type. OK to do this as we know for sure it is:
    D3DXFRAME_EXTENDED *frame = (D3DXFRAME_EXTENDED*)frameToFree;

    if (frame->Name)
        delete[]frame->Name;
    delete frame;

    return S_OK;
}

/**
* \brief callback called to deallocate the mesh container data
* \param the mesh data to free
* \return success result
* \author Keith Ditchburn \date 17 July 2005
*/
HRESULT CMeshHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER meshContainerBase)
{
    // Convert to our extended type. OK as we know for sure it is:
    D3DXMESHCONTAINER_EXTENDED* meshContainer = (D3DXMESHCONTAINER_EXTENDED*)meshContainerBase;
    if (!meshContainer)
        return S_OK;

    // name
    if (meshContainer->Name)
    {
        delete[]meshContainer->Name;
        meshContainer->Name = 0;
    }

    // material array
    if (meshContainer->exMaterials)
    {
        delete[]meshContainer->exMaterials;
        meshContainer->exMaterials = 0;
    }

    // release the textures before deleting the array
    if (meshContainer->exTextures)
    {
        for (UINT i = 0; i < meshContainer->NumMaterials; ++i)
        {
            if (meshContainer->exTextures[i])
                meshContainer->exTextures[i]->Release();
        }
    }

    // texture array
    if (meshContainer->exTextures)
        delete[]meshContainer->exTextures;

    // adjacency data
    if (meshContainer->pAdjacency)
        delete[]meshContainer->pAdjacency;

    // bone parts
    if (meshContainer->exBoneOffsets)
    {
        delete[]meshContainer->exBoneOffsets;
        meshContainer->exBoneOffsets = 0;
    }

    // frame matrices
    if (meshContainer->exFrameCombinedMatrixPointer)
    {
        delete[]meshContainer->exFrameCombinedMatrixPointer;
        meshContainer->exFrameCombinedMatrixPointer = 0;
    }

    // release skin mesh
    if (meshContainer->exSkinMesh)
    {
        meshContainer->exSkinMesh->Release();
        meshContainer->exSkinMesh = 0;
    }

    // release the main mesh
    if (meshContainer->MeshData.pMesh)
    {
        meshContainer->MeshData.pMesh->Release();
        meshContainer->MeshData.pMesh = 0;
    }

    // release skin information
    if (meshContainer->pSkinInfo)
    {
        meshContainer->pSkinInfo->Release();
        meshContainer->pSkinInfo = 0;
    }

    // finally delete the mesh container itself
    delete meshContainer;
    meshContainer = 0;

    return S_OK;
}


// The time to change from one animation set to another
// To see how the merging works - increase this time value to slow it down
const float kMoveTransitionTime = 0.25f;

// Constructor
AnimationX::AnimationX(IDirect3DDevice9* d3dDevice) : m_d3dDevice(d3dDevice), m_speedAdjust(1.0f), m_firstMesh(0),
m_currentTrack(0), m_currentTime(0), m_numAnimationSets(0), m_currentAnimationSet(0), m_maxBones(0), m_sphereRadius(0),
m_sphereCentre(0, 0, 0), m_boneMatrices(0) , m_world(0 )
{
}

// Destructor
AnimationX::~AnimationX(void)
{
    if (m_animController)
    {
        m_animController->Release();
        m_animController = 0;
    }

    if (m_frameRoot)
    {
        // Create a mesh heirarchy class to control the removal of memory for the frame heirarchy
        CMeshHierarchy memoryAllocator;
        D3DXFrameDestroy(m_frameRoot, &memoryAllocator);
        m_frameRoot = 0;
    }

    if (m_boneMatrices)
    {
        delete[]m_boneMatrices;
        m_boneMatrices = 0;
    }
}

/*
Load the x file
The function D3DXLoadMeshHierarchyFromX requires a support object to handle memeory allocation etc.
I have defined this in the class CMeshHierarchy
*/
bool AnimationX::Load(const std::string &filename)
{
	// Create our mesh hierarchy class to control the allocation of memory - only used temporarily
	CMeshHierarchy *memoryAllocator = new CMeshHierarchy;

	// To make it easier to find the textures change the current directory to the one containing the .x file
	// First though remember the current one to put it back afterwards
	std::string currentDirectory = CUtility::GetTheCurrentDirectory();

	std::string xfilePath;
	CUtility::SplitPath(filename, &xfilePath, &m_filename);

	SetCurrentDirectory(xfilePath.c_str());

	// This is the function that does all the .x file loading. We provide a pointer to an instance of our 
	// memory allocator class to handle memory allocationm during the frame and mesh loading
	HRESULT hr = D3DXLoadMeshHierarchyFromX(filename.c_str(), D3DXMESH_MANAGED, m_d3dDevice,
		memoryAllocator, NULL, &m_frameRoot, &m_animController);

	delete memoryAllocator;
	memoryAllocator = 0;
	
	SetCurrentDirectory(currentDirectory.c_str());

	if (CUtility::FailedHr(hr))
		return false;

    // if the x file contains any animation remember how many sets there are
    if (m_animController)
        m_numAnimationSets = m_animController->GetMaxNumAnimationSets();

    // Bones for skining
    if (m_frameRoot)
    {
        // Set the bones up
        SetupBoneMatrices((D3DXFRAME_EXTENDED*)m_frameRoot, NULL);

        // Create the bone matrices array for use during FrameMove to hold the final transform
        m_boneMatrices = new D3DXMATRIX[m_maxBones];
        ZeroMemory(m_boneMatrices, sizeof(D3DXMATRIX)*m_maxBones);

        // Calculate the Bounding Sphere for this model (used in CalculateInitialViewMatrix to position camera correctly)
        D3DXFrameCalculateBoundingSphere(m_frameRoot, &m_sphereCentre, &m_sphereRadius);
    }

    return true;
}

/*
Since this demo can load many different sizes and shapes of 3d model setting the initial
camera position so the model can be seen is a tricky task. This function uses the model's bounding sphere
to come up with an initial position for the camera.
*/
D3DXVECTOR3 AnimationX::GetInitialCameraPosition() const
{
    D3DXVECTOR3 cameraPos(0.0f, m_sphereCentre.y, -(m_sphereRadius * 3));
    return cameraPos;
}

/**
* \brief we need to go through the hierarchy and set the combined matrices
* calls itself recursively as it tareverses the hierarchy
* \param device - the Direct3D device object
* \param pFrame - current frame
* \param pParentMatrix - the parent frame matrix
* \author Keith Ditchburn \date 18 July 2005
*/
void AnimationX::SetupBoneMatrices(D3DXFRAME_EXTENDED *pFrame, LPD3DXMATRIX pParentMatrix)
{
    // Cast to our extended structure first
    D3DXMESHCONTAINER_EXTENDED* pMesh = (D3DXMESHCONTAINER_EXTENDED*)pFrame->pMeshContainer;

    // If this frame has a mesh
    if (pMesh)
    {
        // We need to remember which is the first mesh in the hierarchy for later when we 
        // update (FrameMove)
        if (!m_firstMesh)
            m_firstMesh = pMesh;

        // if there is skin info, then setup the bone matrices
        if (pMesh->pSkinInfo && pMesh->MeshData.pMesh)
        {
            // Create a copy of the mesh to skin into later
            D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
            if (FAILED(pMesh->MeshData.pMesh->GetDeclaration(Declaration)))
                return;

            pMesh->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED,
                Declaration, m_d3dDevice,
                &pMesh->exSkinMesh);

            // Max bones is calculated for later use (to know how big to make the bone matrices array)
            m_maxBones = max(m_maxBones, (int)pMesh->pSkinInfo->GetNumBones());

            // For each bone work out its matrix
            for (unsigned int i = 0; i < pMesh->pSkinInfo->GetNumBones(); i++)
            {
                // Find the frame containing the bone
                D3DXFRAME_EXTENDED* pTempFrame = (D3DXFRAME_EXTENDED*)D3DXFrameFind(m_frameRoot,
                    pMesh->pSkinInfo->GetBoneName(i));

                // set the bone part - point it at the transformation matrix
                pMesh->exFrameCombinedMatrixPointer[i] = &pTempFrame->exCombinedTransformationMatrix;
            }

        }
    }

    // Pass on to sibblings
    if (pFrame->pFrameSibling)
        SetupBoneMatrices((D3DXFRAME_EXTENDED*)pFrame->pFrameSibling, pParentMatrix);

    // Pass on to children
    if (pFrame->pFrameFirstChild)
        SetupBoneMatrices((D3DXFRAME_EXTENDED*)pFrame->pFrameFirstChild, &pFrame->exCombinedTransformationMatrix);
}

void AnimationX::NextAnimation()
{
    unsigned int newAnimationSet = m_currentAnimationSet + 1;
    if (newAnimationSet >= m_numAnimationSets)
        newAnimationSet = 0;

    SetAnimationSet(newAnimationSet);
}

/**
* \brief Called each frame update with the time and the current world matrix
* \param elapsedTime - time passed
* \param matWorld - current world matrix for the model
* \author Keith Ditchburn \date 18 July 2005
*/
void AnimationX::FrameMove(float elapsedTime,  D3DXMATRIX *matWorld)
{
	m_world = matWorld;
    // Adjust animation speed
    elapsedTime /= m_speedAdjust;

    // Advance the time and set in the controller
    if (m_animController != NULL)
        m_animController->AdvanceTime(elapsedTime, NULL);

    m_currentTime += elapsedTime;

    // Now update the model matrices in the hierarchy
    UpdateFrameMatrices(m_frameRoot, matWorld);

    // If the model contains a skinned mesh update the vertices
    D3DXMESHCONTAINER_EXTENDED* pMesh = m_firstMesh;
    if (pMesh && pMesh->pSkinInfo)
    {
        unsigned int Bones = pMesh->pSkinInfo->GetNumBones();

        // Create the bone matrices that transform each bone from bone space into character space
        // (via exFrameCombinedMatrixPointer) and also wraps the mesh around the bones using the bone offsets
        // in exBoneOffsetsArray
        for (unsigned int i = 0; i < Bones; ++i)
            D3DXMatrixMultiply(&m_boneMatrices[i], &pMesh->exBoneOffsets[i], pMesh->exFrameCombinedMatrixPointer[i]);

        // We need to modify the vertex positions based on the new bone matrices. This is achieved
        // by locking the vertex buffers and then calling UpdateSkinnedMesh. UpdateSkinnedMesh takes the
        // original vertex data (in pMesh->MeshData.pMesh), applies the matrices and writes the new vertices
        // out to skin mesh (pMesh->exSkinMesh). 

        // UpdateSkinnedMesh uses software skinning which is the slowest way of carrying out skinning 
        // but is easiest to describe and works on the majority of graphic devices. 
        // Other methods exist that use hardware to do this skinning - see the notes and the 
        // DirectX SDK skinned mesh sample for more details
        void *srcPtr = 0;
        pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);

        void *destPtr = 0;
        pMesh->exSkinMesh->LockVertexBuffer(0, (void**)&destPtr);

        // Update the skinned mesh 
        pMesh->pSkinInfo->UpdateSkinnedMesh(m_boneMatrices, NULL, srcPtr, destPtr);
        D3DXComputeBoundingBox( (D3DXVECTOR3*)destPtr, pMesh->exSkinMesh->GetNumVertices( ), D3DXGetFVFVertexSize( pMesh->exSkinMesh->GetFVF( ) ), &min, &max );

        // Unlock the meshes vertex buffers
        pMesh->exSkinMesh->UnlockVertexBuffer();
        pMesh->MeshData.pMesh->UnlockVertexBuffer();
    }
}

/**
* \brief Called to update the frame matrices in the hierarchy to reflect current animation stage
* \param frameBase - frame being looked at
* \param parentMatrix - the matrix of our parent (if we have one)
* \author Keith Ditchburn \date 18 July 2005
*/
void AnimationX::UpdateFrameMatrices(const D3DXFRAME *frameBase, const D3DXMATRIX *parentMatrix)
{
    D3DXFRAME_EXTENDED *currentFrame = (D3DXFRAME_EXTENDED*)frameBase;

    // If parent matrix exists multiply our frame matrix by it
    if (parentMatrix != NULL)
        D3DXMatrixMultiply(&currentFrame->exCombinedTransformationMatrix, &currentFrame->TransformationMatrix, parentMatrix);
    else
        currentFrame->exCombinedTransformationMatrix = currentFrame->TransformationMatrix;

    // If we have a sibling recurse 
    if (currentFrame->pFrameSibling != NULL)
        UpdateFrameMatrices(currentFrame->pFrameSibling, parentMatrix);

    // If we have a child recurse 
    if (currentFrame->pFrameFirstChild != NULL)
        UpdateFrameMatrices(currentFrame->pFrameFirstChild, &currentFrame->exCombinedTransformationMatrix);
}

/**
* \brief Render our mesh.
* Call the DrawFrame recursive fn on render with the root frame (see notes diagram)
* \param device - the Direct3D device object
* \author Keith Ditchburn \date 18 July 2005
*/
void AnimationX::Render() const
{
    if (m_frameRoot)
        DrawFrame(m_frameRoot);
}

/**
* \brief Called to render a frame in the hierarchy
* \param device - the Direct3D device object
* \param frame - frame to render
* \author Keith Ditchburn \date 18 July 2005
*/
void AnimationX::DrawFrame(LPD3DXFRAME frame) const
{
    // Draw all mesh containers in this frame
    LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;
    while (meshContainer)
    {
        DrawMeshContainer(meshContainer, frame);
        meshContainer = meshContainer->pNextMeshContainer;
    }

    // Recurse for sibblings
    if (frame->pFrameSibling != NULL)
        DrawFrame(frame->pFrameSibling);

    // Recurse for children
    if (frame->pFrameFirstChild != NULL)
        DrawFrame(frame->pFrameFirstChild);
}

/**
* \brief Called to render a mesh
* \param device - the Direct3D device object
* \param meshContainerBase - the mesh container
* \param frameBase - frame containing the mesh
* \author Keith Ditchburn \date 18 July 2005
*/
void AnimationX::DrawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase) const
{
    // Cast to our extended frame type
    D3DXFRAME_EXTENDED *frame = (D3DXFRAME_EXTENDED*)frameBase;

    // Cast to our extended mesh container
    D3DXMESHCONTAINER_EXTENDED *meshContainer = (D3DXMESHCONTAINER_EXTENDED*)meshContainerBase;

    // Set the world transform
    m_d3dDevice->SetTransform(D3DTS_WORLD, &frame->exCombinedTransformationMatrix);

    // Loop through all the materials in the mesh rendering each subset
    for (unsigned int iMaterial = 0; iMaterial < meshContainer->NumMaterials; iMaterial++)
    {
        // use the material in our extended data rather than the one in meshContainer->pMaterials[iMaterial].MatD3D
        m_d3dDevice->SetMaterial(&meshContainer->exMaterials[iMaterial]);
        m_d3dDevice->SetTexture(0, meshContainer->exTextures[iMaterial]);

        // Select the mesh to draw, if there is skin then use the skinned mesh else the normal one
        LPD3DXMESH pDrawMesh = (meshContainer->pSkinInfo) ? meshContainer->exSkinMesh : meshContainer->MeshData.pMesh;
		// Finally Call the mesh draw function
        pDrawMesh->DrawSubset(iMaterial);
    }
}

/**
* \brief Change to a different animation set
* Handles transitions between animations to make it smooth and not a sudden jerk to a new position
* \param index - new animation set index
* \author Keith Ditchburn
*/
void AnimationX::SetAnimationSet(unsigned int index)
{
    if (index == m_currentAnimationSet)
        return;

    if (index >= m_numAnimationSets)
        index = 0;

    // Remember current animation
    m_currentAnimationSet = index;

    // Get the animation set from the controller
    LPD3DXANIMATIONSET set;
    m_animController->GetAnimationSet(m_currentAnimationSet, &set);

    // Note: for a smooth transition between animation sets we can use two tracks and assign the new set to the track
    // not currently playing then insert Keys into the KeyTrack to do the transition between the tracks
    // tracks can be mixed together so we can gradually change into the new animation

    // Alternate tracks
    DWORD newTrack = (m_currentTrack == 0 ? 1 : 0);

    // Assign to our track
    m_animController->SetTrackAnimationSet(newTrack, set);
    set->Release();

    // Clear any track events currently assigned to our two tracks
    m_animController->UnkeyAllTrackEvents(m_currentTrack);
    m_animController->UnkeyAllTrackEvents(newTrack);

    // Add an event key to disable the currently playing track kMoveTransitionTime seconds in the future
    m_animController->KeyTrackEnable(m_currentTrack, FALSE, m_currentTime + kMoveTransitionTime);
    // Add an event key to change the speed right away so the animation completes in kMoveTransitionTime seconds
    m_animController->KeyTrackSpeed(m_currentTrack, 0.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);
    // Add an event to change the weighting of the current track (the effect it has blended with the secon track)
    m_animController->KeyTrackWeight(m_currentTrack, 0.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);

    // Enable the new track
    m_animController->SetTrackEnable(newTrack, TRUE);
    // Add an event key to set the speed of the track
    m_animController->KeyTrackSpeed(newTrack, 1.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);
    // Add an event to change the weighting of the current track (the effect it has blended with the first track)
    // As you can see this will go from 0 effect to total effect(1.0f) in kMoveTransitionTime seconds and the first track goes from 
    // total to 0.0f in the same time.
    m_animController->KeyTrackWeight(newTrack, 1.0f, m_currentTime, kMoveTransitionTime, D3DXTRANSITION_LINEAR);

    // Remember current track
    m_currentTrack = newTrack;
}



/**
* \brief Get the name of the animation
* Note: altered to solve a D3DX memory leak caused because I was not releasing the set after getting it
* \param index - the animation set index
* \return the name
* \author Keith Ditchburn
*/
std::string AnimationX::GetAnimationSetName(unsigned int index)
{
    if (index >= m_numAnimationSets)
        return "Error: No set exists";

    // Get the animation set
    LPD3DXANIMATIONSET set;
    m_animController->GetAnimationSet(m_currentAnimationSet, &set);

    std::string nameString(set->GetName());

    set->Release();

    return nameString;
}

/**
* \brief Slow down animation
* \author Keith Ditchburn \
*/
void AnimationX::AnimateSlower()
{
    m_speedAdjust += 0.1f;
}

/**
* \brief Speed up animation
* \author Keith Ditchburn
*/
void AnimationX::AnimateFaster()
{
    if (m_speedAdjust>0.1f)
        m_speedAdjust -= 0.1f;
}