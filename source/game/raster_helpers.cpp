#include "game/raster_helpers.h"
#include "game/native_scratch.h"

// samp.dll RVA: 0x002a9d98
int g_rwImageDepth;
// samp.dll RVA: 0x002a9d9c
int g_rwImageWidth;
// samp.dll RVA: 0x002a9a40
int g_rwImageHeight;
// samp.dll RVA: 0x002aae10
void * g_rwImageScratch;
// samp.dll RVA: 0x00155400
void ** g_rwSceneCameraSlot=(void**)0xc1703c;

// Shared scratch views only; no independent global storage is introduced.
// RVAs 2ABE40, 2ABE48 and 2ABE4C.
#define SCENE_SOURCE_RASTER (*(void**)(g_entityCollisionScratch+0x1028))
#define SCENE_COLOR_RASTER (*(void**)(g_entityCollisionScratch+0x1030))
#define SCENE_DEPTH_RASTER (*(void**)(g_entityCollisionScratch+0x1034))

// Native ignores all failures and reloads the published image between calls.
// GTA SA virtual addresses: image create/allocate/from-raster/BMP write/destroy.
// samp.dll RVA: 0x000b5690
void GameWriteRasterBmp(void *raster,const char *filename) {
    g_rwImageScratch=((void*(__cdecl*)(int,int,int))0x8026e0)(g_rwImageWidth,g_rwImageHeight,g_rwImageDepth);
    ((void*(__cdecl*)(void*))0x8027a0)(g_rwImageScratch);
    ((void*(__cdecl*)(void*,void*))0x804250)(g_rwImageScratch,raster);
    ((void*(__cdecl*)(void*,const char*))0x7ce990)(g_rwImageScratch,filename);
    ((BOOL(__cdecl*)(void*))0x802740)(g_rwImageScratch);
}

// Allocate only when both raster slots are null. A partial success is retained.
// GTA SA virtual addresses: source raster slot C402D8; RwRasterCreate 7FB230.
// samp.dll RVA: 0x000b57d0
void GameCreateSceneRasters() {
    if(SCENE_COLOR_RASTER || SCENE_DEPTH_RASTER)return;
    SCENE_SOURCE_RASTER=*(void**)0xc402d8;
    BYTE *source=(BYTE*)SCENE_SOURCE_RASTER;
    g_rwImageDepth=*(int*)(source+0x14);
    g_rwImageHeight=*(int*)(source+0x10);
    g_rwImageWidth=*(int*)(source+0xc);
    SCENE_COLOR_RASTER=((void*(__cdecl*)(int,int,int,int))0x7fb230)(2048,1024,g_rwImageDepth,5);
    SCENE_DEPTH_RASTER=((void*(__cdecl*)(int,int,int,int))0x7fb230)(2048,1024,g_rwImageDepth,1);
}

// Preserve all PUSHAD registers, camera publication, call order and saved-slot
// reloads. Shared saved slots at RVAs 2ABE50/54 make this path non-reentrant.
// GTA SA virtual address C9BCC0 is written but not restored by the native body.
// samp.dll RVA: 0x000b5860
__declspec(naked) void GameRenderWithSceneRasters() {
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        push ebx
        push esi
        push edi
        pushad
        call GameCreateSceneRasters
        mov eax,g_rwSceneCameraSlot
        mov eax,dword ptr [eax]
        mov dword ptr [ebp + -0x4],eax
        mov dword ptr ds:[0x00c9bcc0],eax
        mov ebx,dword ptr [ebp + -0x4]
        mov edx,dword ptr [ebx + 0x60]
        mov dword ptr [g_entityCollisionScratch+0x1038],edx
        mov edx,dword ptr [ebx + 0x64]
        mov dword ptr [g_entityCollisionScratch+0x103c],edx
        mov edx,dword ptr [g_entityCollisionScratch+0x1030]
        mov dword ptr [ebx + 0x60],edx
        mov edx,dword ptr [g_entityCollisionScratch+0x1034]
        mov dword ptr [ebx + 0x64],edx
        mov edx,0x734650
        call edx
        mov edx,0x53df40
        call edx
        mov edx,0x732f30
        call edx
        mov ebx,dword ptr [ebp + -0x4]
        mov edx,dword ptr [g_entityCollisionScratch+0x1038]
        mov dword ptr [ebx + 0x60],edx
        mov edx,dword ptr [g_entityCollisionScratch+0x103c]
        mov dword ptr [ebx + 0x64],edx
        popad
        pop edi
        pop esi
        pop ebx
        mov esp,ebp
        pop ebp
        ret
    }
}
