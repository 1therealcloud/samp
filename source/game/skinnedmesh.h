#ifndef SAMP_SKINNED_MESH_H
#define SAMP_SKINNED_MESH_H
#include <d3dx9.h>
#include <stddef.h>

// Native hierarchy extensions; names are descriptive, not original identifiers.
struct SkinFrame : D3DXFRAME {
    D3DXMATRIX combined;
};
struct SkinMeshContainer : D3DXMESHCONTAINER {
    IDirect3DTexture9 **textures;
    D3DMATERIAL9 *materialStorage;
    ID3DXMesh *workingMesh;
    D3DXMATRIX *boneOffsets;
    D3DXMATRIX **boneFrames;
    void *unknown38;
    DWORD unknown3c;
};

// samp.dll vtable RVA: 0x0011f8c8
class CSkinHierarchyAllocator : public ID3DXAllocateHierarchy {
public:
    // samp.dll RVA: 0x000b8ad0
    HRESULT __stdcall CreateFrame(LPCSTR name,D3DXFRAME **out);
    // Native does not validate mesh type, allocation or mesh/skin API results.
    // samp.dll RVA: 0x000b8800
    HRESULT __stdcall CreateMeshContainer(LPCSTR name,const D3DXMESHDATA *mesh,
        const D3DXMATERIAL *materials,const D3DXEFFECTINSTANCE *effects,
        DWORD materialCount,const DWORD *adjacency,ID3DXSkinInfo *skin,
        D3DXMESHCONTAINER **out);
    // samp.dll RVA: 0x000b86b0
    HRESULT __stdcall DestroyFrame(D3DXFRAME *frame);
    // samp.dll RVA: 0x000b86e0
    HRESULT __stdcall DestroyMeshContainer(D3DXMESHCONTAINER *container);
};

typedef char SkinFrameExtent[(sizeof(SkinFrame)==0x90)?1:-1];
typedef char SkinFrameCombinedOffset[(offsetof(SkinFrame,combined)==0x50)?1:-1];
typedef char SkinContainerExtent[(sizeof(SkinMeshContainer)==0x40)?1:-1];
typedef char SkinContainerTexturesOffset[(offsetof(SkinMeshContainer,textures)==0x24)?1:-1];
typedef char SkinContainerWorkingOffset[(offsetof(SkinMeshContainer,workingMesh)==0x2c)?1:-1];
typedef char SkinContainerBonesOffset[(offsetof(SkinMeshContainer,boneOffsets)==0x30)?1:-1];
typedef char SkinMaterialExtent[(sizeof(D3DMATERIAL9)==68)?1:-1];
typedef char SkinAllocatorExtent[(sizeof(CSkinHierarchyAllocator)==4)?1:-1];

// Original development path is retained; no alternate texture search is invented.
// samp.dll RVA: 0x0011f860
extern const char g_skinTextureDirectory[];
// samp.dll RVA: 0x0011f84c
extern const char g_skinTextureFailed[];
// samp.dll RVA: 0x0011f838
extern const char g_skinTextureLoaded[];
// samp.dll RVA: 0x0011c92c
extern const char g_skinTexturePathFormat[];
#endif
