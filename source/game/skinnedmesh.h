#ifndef SAMP_SKINNED_MESH_H
#define SAMP_SKINNED_MESH_H
#include <d3dx9.h>
#include <stddef.h>

// Native hierarchy extensions; names are descriptive, not original identifiers.
struct SkinFrame : D3DXFRAME
{
    D3DXMATRIX combined;
};
struct SkinMeshContainer : D3DXMESHCONTAINER
{
    IDirect3DTexture9** textures;
    D3DMATERIAL9* materialStorage;
    ID3DXMesh* workingMesh;
    D3DXMATRIX* boneOffsets;
    D3DXMATRIX** boneFrames;
    void* unknown38;
    DWORD unknown3c;
};

// samp.dll vtable RVA: 0x0011f8c8
class CSkinHierarchyAllocator : public ID3DXAllocateHierarchy
{
public:
    // 0x000b9340
    CSkinHierarchyAllocator();
    // 0x000b8ad0
    HRESULT __stdcall CreateFrame(LPCSTR name, D3DXFRAME** out);
    // Native does not validate mesh type, allocation or mesh/skin API results.
    // 0x000b8800
    HRESULT __stdcall CreateMeshContainer(LPCSTR name, const D3DXMESHDATA* mesh, const D3DXMATERIAL* materials, const D3DXEFFECTINSTANCE* effects, DWORD materialCount,
                                          const DWORD* adjacency, ID3DXSkinInfo* skin, D3DXMESHCONTAINER** out);
    // 0x000b86b0
    HRESULT __stdcall DestroyFrame(D3DXFRAME* frame);
    // 0x000b86e0
    HRESULT __stdcall DestroyMeshContainer(D3DXMESHCONTAINER* container);
};

class CMatrix;
// Descriptive class name. Native has only a virtual scalar destructor slot.
// samp.dll vtable RVA: 0x0011f8a0
class CSkinnedMesh
{
public:
    IDirect3DDevice9* device;
    SkinMeshContainer* mesh;
    SkinFrame* root;
    D3DXMATRIX* boneMatrices;
    D3DXVECTOR3 center;
    float radius;
    DWORD maximumBones, currentAnimation, animationCapacity;
    ID3DXAnimationController* controller;
    // 0x000b8bc0
    CSkinnedMesh(IDirect3DDevice9* device);
    // 0x000b9510
    virtual ~CSkinnedMesh();
    // Stores pointers even when FrameFind returns null (native result becomes50).
    // 0x000b8c30
    void SetupBonePointers(SkinFrame* frame, const D3DXMATRIX* parent);
    // Guard uses controller capacity, not actual registered animation-set count.
    // 0x000b8d10
    void SetAnimation(DWORD index);
    // Returns1 without device,2 without mesh,0 after drawing.
    // 0x000b8d60
    int Render(const CMatrix* matrix);
    // 0x000b9190
    void DrawFrame(SkinFrame* frame);
    // Actual native RET12; original parameter meanings/identifier not established.
    // 0x000b9250
    void NativeNoOp3Words(DWORD first, DWORD second, DWORD third);
    // 0x000b9260
    void DumpBones();
    // 0x000b92e0
    void UpdateFrameMatrices(SkinFrame* frame, const D3DXMATRIX* parent);
    // Failure1/success0. Existing hierarchy/storage is not cleaned before loading.
    // 0x000b9350
    int Load(const char* filename);
    // Native ignores its one stack word; it does not advance animation time.
    // 0x000b9450
    void UpdateSkin(DWORD unused);
};

typedef char SkinnedMeshExtent[(sizeof(CSkinnedMesh) == 0x34) ? 1 : -1];
typedef char SkinnedMeshRootOffset[(offsetof(CSkinnedMesh, root) == 0xc) ? 1 : -1];
typedef char SkinnedMeshBonesOffset[(offsetof(CSkinnedMesh, boneMatrices) == 0x10) ? 1 : -1];
typedef char SkinnedMeshControllerOffset[(offsetof(CSkinnedMesh, controller) == 0x30) ? 1 : -1];
typedef char SkinFrameExtent[(sizeof(SkinFrame) == 0x90) ? 1 : -1];
typedef char SkinFrameCombinedOffset[(offsetof(SkinFrame, combined) == 0x50) ? 1 : -1];
typedef char SkinContainerExtent[(sizeof(SkinMeshContainer) == 0x40) ? 1 : -1];
typedef char SkinContainerTexturesOffset[(offsetof(SkinMeshContainer, textures) == 0x24) ? 1 : -1];
typedef char SkinContainerWorkingOffset[(offsetof(SkinMeshContainer, workingMesh) == 0x2c) ? 1 : -1];
typedef char SkinContainerBonesOffset[(offsetof(SkinMeshContainer, boneOffsets) == 0x30) ? 1 : -1];
typedef char SkinMaterialExtent[(sizeof(D3DMATERIAL9) == 68) ? 1 : -1];
typedef char SkinAllocatorExtent[(sizeof(CSkinHierarchyAllocator) == 4) ? 1 : -1];

// Original development path is retained; no alternate texture search is invented.
// 0x0011f860
extern const char g_skinTextureDirectory[];
// 0x0011f84c
extern const char g_skinTextureFailed[];
// 0x0011f838
extern const char g_skinTextureLoaded[];
// 0x0011c92c
extern const char g_skinTexturePathFormat[];
// 0x0011f8a4
extern const char g_skinBoneNameFormat[];
// 0x0011f8b8
extern const char g_skinBoneCountFormat[];
#endif
