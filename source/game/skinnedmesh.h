#ifndef SKINNEDMESH_H
#define SKINNEDMESH_H

#include <d3dx9.h>

#include <stddef.h>

#include <static_assert.h>

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
    // R5 address to verify
    CSkinHierarchyAllocator();
    // R5 address to verify
    HRESULT __stdcall CreateFrame(LPCSTR name, D3DXFRAME** out);
    // Native does not validate mesh type, allocation or mesh/skin API results.
    // R5 address to verify
    HRESULT __stdcall CreateMeshContainer(LPCSTR name, const D3DXMESHDATA* mesh, const D3DXMATERIAL* materials, const D3DXEFFECTINSTANCE* effects, DWORD materialCount,
                                          const DWORD* adjacency, ID3DXSkinInfo* skin, D3DXMESHCONTAINER** out);
    // R5 address to verify
    HRESULT __stdcall DestroyFrame(D3DXFRAME* frame);
    // R5 address to verify
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
    // R5 address to verify
    CSkinnedMesh(IDirect3DDevice9* device);
    // R5 address to verify
    virtual ~CSkinnedMesh();
    // Stores pointers even when FrameFind returns null (native result becomes50).
    // R5 address to verify
    void SetupBonePointers(SkinFrame* frame, const D3DXMATRIX* parent);
    // Guard uses controller capacity, not actual registered animation-set count.
    // R5 address to verify
    void SetAnimation(DWORD index);
    // Returns1 without device,2 without mesh,0 after drawing.
    // R5 address to verify
    int Render(const CMatrix* matrix);
    // R5 address to verify
    void DrawFrame(SkinFrame* frame);
    // Actual native RET12; original parameter meanings/identifier not established.
    // R5 address to verify
    void NativeNoOp3Words(DWORD first, DWORD second, DWORD third);
    // R5 address to verify
    void DumpBones();
    // R5 address to verify
    void UpdateFrameMatrices(SkinFrame* frame, const D3DXMATRIX* parent);
    // Failure1/success0. Existing hierarchy/storage is not cleaned before loading.
    // R5 address to verify
    int Load(const char* filename);
    // Native ignores its one stack word; it does not advance animation time.
    // R5 address to verify
    void UpdateSkin(DWORD unused);
};

STATIC_ASSERT(sizeof(CSkinnedMesh) == 0x34);
STATIC_ASSERT(offsetof(CSkinnedMesh, root) == 0xc);
STATIC_ASSERT(offsetof(CSkinnedMesh, boneMatrices) == 0x10);
STATIC_ASSERT(offsetof(CSkinnedMesh, controller) == 0x30);
STATIC_ASSERT(sizeof(SkinFrame) == 0x90);
STATIC_ASSERT(offsetof(SkinFrame, combined) == 0x50);
STATIC_ASSERT(sizeof(SkinMeshContainer) == 0x40);
STATIC_ASSERT(offsetof(SkinMeshContainer, textures) == 0x24);
STATIC_ASSERT(offsetof(SkinMeshContainer, workingMesh) == 0x2c);
STATIC_ASSERT(offsetof(SkinMeshContainer, boneOffsets) == 0x30);
STATIC_ASSERT(sizeof(D3DMATERIAL9) == 68);
STATIC_ASSERT(sizeof(CSkinHierarchyAllocator) == 4);

// Original development path is retained; no alternate texture search is invented.
// R5 address to verify
extern const char g_skinTextureDirectory[];
// R5 address to verify
extern const char g_skinTextureFailed[];
// R5 address to verify
extern const char g_skinTextureLoaded[];
// 0x000ea904
extern const char g_skinTexturePathFormat[];
// R5 address to verify
extern const char g_skinBoneNameFormat[];
// R5 address to verify
extern const char g_skinBoneCountFormat[];

#endif // SKINNEDMESH_H
