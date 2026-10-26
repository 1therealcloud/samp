#include "game/skinnedmesh.h"
#include "main.h"
#include "chatwindow.h"
#include <new>
#include <string.h>
#include <stdio.h>

// samp.dll RVA: 0x0011f860
const char g_skinTextureDirectory[]="C:\\Users\\New\\Desktop\\SAMP_037_Desktop\\Matite2014\\newskins\\ponz\\";
// samp.dll RVA: 0x0011f84c
const char g_skinTextureFailed[]="FAILED texture: %s";
// samp.dll RVA: 0x0011f838
const char g_skinTextureLoaded[]="LOADED texture: %s";
// samp.dll RVA: 0x0011c92c
const char g_skinTexturePathFormat[]="%s%s";

// samp.dll RVA: 0x000b8ad0
HRESULT __stdcall CSkinHierarchyAllocator::CreateFrame(LPCSTR name,D3DXFRAME **out) {
    SkinFrame *frame=(SkinFrame*)::operator new(sizeof(SkinFrame));
    memset(frame,0,sizeof(SkinFrame));
    *out=0;
    if(name) {
        size_t length=strlen(name)+1;
        frame->Name=(char*)::operator new[](length);
        memcpy(frame->Name,name,length);
    } else frame->Name=0;
    frame->pFrameFirstChild=0;frame->pFrameSibling=0;frame->pMeshContainer=0;
    D3DXMatrixIdentity(&frame->combined);
    D3DXMatrixIdentity(&frame->TransformationMatrix);
    *out=frame;
    return S_OK;
}

// samp.dll RVA: 0x000b8800
HRESULT __stdcall CSkinHierarchyAllocator::CreateMeshContainer(LPCSTR name,
    const D3DXMESHDATA *mesh,const D3DXMATERIAL *materials,
    const D3DXEFFECTINSTANCE *effects,DWORD materialCount,const DWORD *adjacency,
    ID3DXSkinInfo *skin,D3DXMESHCONTAINER **out) {
    SkinMeshContainer *container=(SkinMeshContainer*)::operator new(sizeof(SkinMeshContainer));
    memset(container,0,sizeof(SkinMeshContainer));
    char texturePath[1025]={0};
    *out=0;
    if(name) {
        size_t length=strlen(name)+1;
        container->Name=(char*)::operator new[](length);
        memcpy(container->Name,name,length);
    } else container->Name=0;
    container->MeshData.Type=D3DXMESHTYPE_MESH;
    DWORD faceCount=mesh->pMesh->GetNumFaces();
    container->NumMaterials=materialCount;
    // This separate68-byte array is deliberately left uninitialized. Native does
    // not copy the supplied materials or set the base pMaterials/pEffects fields.
    container->materialStorage=(D3DMATERIAL9*)::operator new[](materialCount*68);
    container->pAdjacency=(DWORD*)::operator new[](faceCount*3*4);
    memcpy(container->pAdjacency,adjacency,faceCount*3*4);
    IDirect3DDevice9 *device=0;
    mesh->pMesh->GetDevice(&device);
    // Native captures the clone slot before GetFVF, then reloads the mesh this.
    typedef HRESULT (WINAPI *CloneMeshFunction)(ID3DXMesh*,DWORD,DWORD,IDirect3DDevice9*,ID3DXMesh**);
    CloneMeshFunction clone=(CloneMeshFunction)(*(void***)mesh->pMesh)[11];
    IDirect3DDevice9 *cloneDevice=device;
    DWORD fvf=mesh->pMesh->GetFVF();
    clone(mesh->pMesh,0x220,fvf,cloneDevice,&container->MeshData.pMesh);
    container->textures=(IDirect3DTexture9**)::operator new[](materialCount*4);
    for(DWORD i=0;i<materialCount;++i) {
        container->textures[i]=0;
        const char *texture=materials[i].pTextureFilename;
        if(texture && strlen(texture)) {
            sprintf(texturePath,g_skinTexturePathFormat,g_skinTextureDirectory,texture);
            HRESULT result=D3DXCreateTextureFromFileA(device,texturePath,&container->textures[i]);
            if(FAILED(result)) {
                container->textures[i]=0;
                if(g_pChat)g_pChat->AddDebugMessage(g_skinTextureFailed,materials[i].pTextureFilename);
            } else if(g_pChat)g_pChat->AddDebugMessage(g_skinTextureLoaded,materials[i].pTextureFilename);
        }
    }
    if(device){device->Release();device=0;}
    if(skin) {
        container->pSkinInfo=skin;
        skin->AddRef();
        DWORD bones=skin->GetNumBones();
        container->boneOffsets=(D3DXMATRIX*)::operator new[](bones*64);
        container->boneFrames=(D3DXMATRIX**)::operator new[](bones*4);
        for(DWORD i=0;i<bones;++i)
            memcpy(container->boneOffsets+i,container->pSkinInfo->GetBoneOffsetMatrix(i),64);
    } else {
        container->pSkinInfo=0;container->boneOffsets=0;
        container->workingMesh=0;container->boneFrames=0;
    }
    container->pMaterials=0;container->pEffects=0;
    *out=container;
    return S_OK;
}

// samp.dll RVA: 0x000b86b0
HRESULT __stdcall CSkinHierarchyAllocator::DestroyFrame(D3DXFRAME *frame) {
    if(frame->Name) {
        ::operator delete[](frame->Name);
        frame->Name=0;
    }
    ::operator delete(frame);
    return S_OK;
}

// samp.dll RVA: 0x000b86e0
HRESULT __stdcall CSkinHierarchyAllocator::DestroyMeshContainer(D3DXMESHCONTAINER *base) {
    SkinMeshContainer *container=(SkinMeshContainer*)base;
    if(container->Name){::operator delete[](container->Name);container->Name=0;}
    if(container->materialStorage){::operator delete[](container->materialStorage);container->materialStorage=0;}
    if(container->textures) {
        for(DWORD i=0;i<container->NumMaterials;++i)if(container->textures[i]) {
            container->textures[i]->Release();container->textures[i]=0;
        }
    }
    if(container->textures){::operator delete[](container->textures);container->textures=0;}
    if(container->pAdjacency){::operator delete[](container->pAdjacency);container->pAdjacency=0;}
    if(container->boneOffsets){::operator delete[](container->boneOffsets);container->boneOffsets=0;}
    if(container->boneFrames){::operator delete[](container->boneFrames);container->boneFrames=0;}
    if(container->unknown38){::operator delete[](container->unknown38);container->unknown38=0;}
    if(container->workingMesh){container->workingMesh->Release();container->workingMesh=0;}
    if(container->MeshData.pMesh){container->MeshData.pMesh->Release();container->MeshData.pMesh=0;}
    if(container->pSkinInfo){container->pSkinInfo->Release();container->pSkinInfo=0;}
    ::operator delete(container);
    return S_OK;
}
