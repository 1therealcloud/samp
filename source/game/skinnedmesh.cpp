#include "game/skinnedmesh.h"
#include "main.h"
#include "chatwindow.h"
#include <new>
#include <string.h>
#include <stdio.h>
#include "game/util.h"
#include "d3dhook/texture_binding.h"

// samp.dll RVA: 0x000b9340
CSkinHierarchyAllocator::CSkinHierarchyAllocator() {}

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

// samp.dll RVA: 0x0011f8a4
const char g_skinBoneNameFormat[]="Bone(%u) name: %s";
// samp.dll RVA: 0x0011f8b8
const char g_skinBoneCountFormat[]="Num bones: %u";

// samp.dll RVA: 0x000b8bc0
CSkinnedMesh::CSkinnedMesh(IDirect3DDevice9 *inputDevice) {
    device=inputDevice;center=D3DXVECTOR3(0,0,0);
    root=0;boneMatrices=0;radius=0;currentAnimation=0xffffffff;
    animationCapacity=0;maximumBones=0;controller=0;mesh=0;
}
// samp.dll RVA: 0x000b9510
CSkinnedMesh::~CSkinnedMesh() {
    if(controller){controller->Release();controller=0;}
    if(root){CSkinHierarchyAllocator allocator;D3DXFrameDestroy(root,&allocator);root=0;}
    if(boneMatrices){::operator delete[](boneMatrices);boneMatrices=0;}
    device=0;
}
// samp.dll RVA: 0x000b8c30
void CSkinnedMesh::SetupBonePointers(SkinFrame *frame,const D3DXMATRIX *parent) {
    do {
        SkinMeshContainer *container=(SkinMeshContainer*)frame->pMeshContainer;
        if(container) {
            if(!mesh)mesh=container;
            if(container->pSkinInfo) {
                typedef HRESULT (WINAPI *CloneFunction)(ID3DXMesh*,DWORD,DWORD,IDirect3DDevice9*,ID3DXMesh**);
                CloneFunction clone=(CloneFunction)(*(void***)container->MeshData.pMesh)[11];
                IDirect3DDevice9 *cloneDevice=device;
                DWORD fvf=container->MeshData.pMesh->GetFVF();
                clone(container->MeshData.pMesh,0x220,fvf,cloneDevice,&container->workingMesh);
                if(maximumBones<container->pSkinInfo->GetNumBones())
                    maximumBones=container->pSkinInfo->GetNumBones();
                for(DWORD i=0;i<container->pSkinInfo->GetNumBones();++i) {
                    const char *name=container->pSkinInfo->GetBoneName(i);
                    D3DXFRAME *found=D3DXFrameFind(root,name);
                    container->boneFrames[i]=(D3DXMATRIX*)((DWORD)found+0x50);
                }
            }
        }
        if(frame->pFrameSibling)SetupBonePointers((SkinFrame*)frame->pFrameSibling,parent);
        parent=&frame->combined;
        frame=(SkinFrame*)frame->pFrameFirstChild;
    } while(frame);
}
// samp.dll RVA: 0x000b8d10
void CSkinnedMesh::SetAnimation(DWORD index) {
    if(index==currentAnimation||index>=animationCapacity)return;
    ID3DXAnimationSet *set=0;
    currentAnimation=index;
    controller->GetAnimationSet(index,&set);
    controller->SetTrackAnimationSet(0,set);
    if(set)set->Release();
}
// samp.dll RVA: 0x000b9190
void CSkinnedMesh::DrawFrame(SkinFrame *frame) {
    do {
        for(SkinMeshContainer *container=(SkinMeshContainer*)frame->pMeshContainer;
            container;container=(SkinMeshContainer*)container->pNextMeshContainer) {
            ID3DXMesh *draw=container->pSkinInfo?container->workingMesh:container->MeshData.pMesh;
            for(DWORD i=0;i<container->NumMaterials;++i) {
                device->SetMaterial(container->materialStorage+i);
                device->SetTexture(0,container->textures[i]);
                draw->DrawSubset(i);
            }
        }
        if(frame->pFrameSibling)DrawFrame((SkinFrame*)frame->pFrameSibling);
        frame=(SkinFrame*)frame->pFrameFirstChild;
    } while(frame);
}
// samp.dll RVA: 0x000b9250
void CSkinnedMesh::NativeNoOp3Words(DWORD,DWORD,DWORD) {}
// samp.dll RVA: 0x000b9260
void CSkinnedMesh::DumpBones() {
    if(!root||!mesh||!mesh->pSkinInfo)return;
    SkinMeshContainer *container=mesh;
    DWORD count=container->pSkinInfo->GetNumBones();
    if(g_pChat)g_pChat->AddDebugMessage(g_skinBoneCountFormat,count);
    for(DWORD i=0;i<count;++i)if(g_pChat) {
        const char *name=container->pSkinInfo->GetBoneName(i);
        g_pChat->AddDebugMessage(g_skinBoneNameFormat,i,name);
    }
}
// samp.dll RVA: 0x000b92e0
void CSkinnedMesh::UpdateFrameMatrices(SkinFrame *frame,const D3DXMATRIX *parent) {
    do {
        if(parent)D3DXMatrixMultiply(&frame->combined,&frame->TransformationMatrix,parent);
        else memcpy(&frame->combined,&frame->TransformationMatrix,64);
        if(frame->pFrameSibling)UpdateFrameMatrices((SkinFrame*)frame->pFrameSibling,parent);
        parent=&frame->combined;
        frame=(SkinFrame*)frame->pFrameFirstChild;
    } while(frame);
}
// samp.dll RVA: 0x000b9350
int CSkinnedMesh::Load(const char *filename) {
    CSkinHierarchyAllocator allocator;
    if(FAILED(D3DXLoadMeshHierarchyFromXA(filename,0x220,device,&allocator,0,
        (D3DXFRAME**)&root,&controller)))return 1;
    if(controller)animationCapacity=controller->GetMaxNumAnimationSets();
    if(root) {
        SetupBonePointers(root,0);
        boneMatrices=new D3DXMATRIX[maximumBones];
        memset(boneMatrices,0,maximumBones*64);
        D3DXFrameCalculateBoundingSphere(root,&center,&radius);
    }
    return 0;
}
// samp.dll RVA: 0x000b9450
void CSkinnedMesh::UpdateSkin(DWORD) {
    if(!root)return;
    UpdateFrameMatrices(root,0);
    SkinMeshContainer *container=mesh;
    if(!container||!container->pSkinInfo)return;
    DWORD count=container->pSkinInfo->GetNumBones();
    for(DWORD i=0;i<count;++i)
        D3DXMatrixMultiply(boneMatrices+i,container->boneOffsets+i,container->boneFrames[i]);
    void *source,*destination;
    container->MeshData.pMesh->LockVertexBuffer(0x10,&source);
    container->workingMesh->LockVertexBuffer(0,&destination);
    container->pSkinInfo->UpdateSkinnedMesh(boneMatrices,0,source,destination);
    container->workingMesh->UnlockVertexBuffer();
    container->MeshData.pMesh->UnlockVertexBuffer();
}
// samp.dll RVA: 0x000b8d60
int CSkinnedMesh::Render(const CMatrix *matrix) {
    if(!device)return 1;
    SkinMeshContainer *container=mesh;
    if(!container)return 2;
    DWORD oldRenderState=0;
    GameRwInvokeEngineSlot24(30,(DWORD)&oldRenderState);
    ClientSetRenderState(6,1);ClientSetRenderState(8,1);
    ClientSetRenderState(7,2);ClientSetRenderState(20,0);
    float angle;
    __asm {
        mov ecx,matrix
        fld dword ptr [ecx+0x10]
        fchs
        fld dword ptr [ecx+0x14]
        fpatan
        fmul dword ptr g_entityRadiansToDegrees
        fcom dword ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,5
        jp skin_render_check_upper
        fadd dword ptr g_anglePeriod
        jmp skin_render_angle_done
    skin_render_check_upper:
        fcom dword ptr g_anglePeriod
        fnstsw ax
        test ah,1
        jne skin_render_angle_done
        fsub dword ptr g_anglePeriod
    skin_render_angle_done:
        fmul dword ptr g_objectDegreesToRadians
        fstp angle
    }
    D3DXMATRIX rotation,translation,product,world,oldWorld;
    D3DXMatrixIdentity(&rotation);D3DXMatrixRotationZ(&rotation,angle);
    D3DXMatrixIdentity(&translation);
    D3DXMatrixTranslation(&translation,matrix->position.x,matrix->position.y,matrix->position.z);
    D3DXMatrixIdentity(&product);D3DXMatrixMultiply(&product,&rotation,&translation);
    memcpy(&world,&product,64);
    device->GetTransform(D3DTS_WORLD,&oldWorld);
    device->SetTransform(D3DTS_WORLD,&world);
    do {
        ID3DXMesh *draw=container->pSkinInfo?container->workingMesh:container->MeshData.pMesh;
        for(DWORD i=0;i<container->NumMaterials;++i) {
            D3DMATERIAL9 *material=container->materialStorage+i;
            material->Ambient.r=1.0f;material->Ambient.g=1.0f;
            material->Ambient.b=1.0f;material->Ambient.a=1.0f;
            material->Diffuse.r=1.0f;material->Diffuse.g=1.0f;
            material->Diffuse.b=1.0f;material->Diffuse.a=1.0f;
            material->Emissive.r=1.0f;material->Emissive.g=1.0f;
            material->Emissive.b=1.0f;material->Emissive.a=1.0f;
            device->SetMaterial(container->materialStorage+i);
            device->SetTexture(0,container->textures[i]);
            draw->DrawSubset(i);
        }
        container=(SkinMeshContainer*)container->pNextMeshContainer;
    } while(container);
    device->SetTransform(D3DTS_WORLD,&oldWorld);
    ClientSetRenderState(30,oldRenderState);
    return 0;
}
