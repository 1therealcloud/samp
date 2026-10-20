#include "game/entity_collision.h"
#include "game/model.h"
#include "game/object_math.h"

// samp.dll RVA: 0x0018de60
GameEntityCollisionShape g_entityCollisionShapeA;
// samp.dll RVA: 0x0018de4c
GameEntityCollisionShape g_entityCollisionShapeB;
// samp.dll RVA: 0x0018de7c
void *g_entityCollisionEntity;
// samp.dll RVA: 0x00155404
GameEntityCollisionBackend g_entityCollisionBackend=(GameEntityCollisionBackend)0x419f00; // GTA SA virtual address.
// samp.dll RVA: 0x00155408
void **g_entityCollisionContextSlot=(void**)0x9655cc; // GTA SA virtual address.
// samp.dll RVA: 0x002abe5c
void *g_entityCollisionSavedContext;
// samp.dll RVA: 0x002abe60
BYTE g_entityCollisionResult;
// samp.dll RVA: 0x002aae18
BYTE g_entityCollisionScratch[0x1044];

// samp.dll RVA: 0x0009fa40
__declspec(naked) bool CEntity::TestCollisionShapes(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,BYTE mode) {
    __asm {
        push esi
        mov esi,ecx
        mov eax,dword ptr [esi + 0x40]
        test eax,eax
        jz branch_1009fb0e
        cmp dword ptr [eax],0x863c40
        jz branch_1009fb0e
        movsx eax,word ptr [eax + 0x22]
        push eax
        call GameGetModelCollisionRadius
        fcomp float ptr g_objectScaleLowerBound
        add esp,0x4
        fnstsw ax
        test ah,0x41
        jnp branch_1009fb0e
        mov ecx,dword ptr [esp + 0x8]
        mov edx,dword ptr [ecx]
        mov dword ptr g_entityCollisionShapeA,edx
        mov eax,dword ptr [ecx + 0x4]
        mov [g_entityCollisionShapeA+4],eax
        mov edx,dword ptr [ecx + 0x8]
        mov dword ptr [g_entityCollisionShapeA+8],edx
        mov eax,dword ptr [ecx + 0xc]
        mov edx,dword ptr [esp + 0xc]
        mov [g_entityCollisionShapeA+12],eax
        mov ecx,dword ptr [ecx + 0x10]
        mov dword ptr [g_entityCollisionShapeA+16],ecx
        mov eax,dword ptr [edx]
        mov g_entityCollisionShapeB,eax
        mov ecx,dword ptr [edx + 0x4]
        mov dword ptr [g_entityCollisionShapeB+4],ecx
        mov eax,dword ptr [edx + 0x8]
        mov [g_entityCollisionShapeB+8],eax
        mov ecx,dword ptr [edx + 0xc]
        mov al,byte ptr [esp + 0x10]
        test al,al
        mov dword ptr [g_entityCollisionShapeB+12],ecx
        mov edx,dword ptr [edx + 0x10]
        mov dword ptr [g_entityCollisionShapeB+16],edx
        mov esi,dword ptr [esi + 0x40]
        push esi
        push offset g_entityCollisionShapeB
        mov dword ptr g_entityCollisionEntity,esi
        push offset g_entityCollisionShapeA
        jz branch_1009faff
        call GameEntityCollisionQueryDirect
        test al,al
        jz branch_1009fb0e
        mov al,0x1
        pop esi
        ret 0xc
    branch_1009faff:
        call GameEntityCollisionQueryScoped
        test al,al
        jz branch_1009fb0e
        mov al,0x1
        pop esi
        ret 0xc
    branch_1009fb0e:
        xor al,al
        pop esi
        ret 0xc
    }
}

// samp.dll RVA: 0x000b5ea0
__declspec(naked) float __cdecl GameGetModelCollisionRadius(int model) {
    __asm {
        sub esp,0x8
        push esi
        mov esi,dword ptr [esp + 0x10]
        push esi
        call GameGetModelInfo
        test eax,eax
        jz branch_100b5ed7
        push esi
        call GameGetModelInfo
        mov dword ptr [esp + 0x8],eax
        mov dword ptr [esp + 0x4],0x0
        mov eax,dword ptr [esp + 0x8]
        mov edx,dword ptr [eax + 0x14]
        mov dword ptr [esp + 0x4],edx
        mov eax,dword ptr [esp + 0x4]
        test eax,eax
        jnz branch_100b5ee2
    branch_100b5ed7:
        fld float ptr g_objectScaleLowerBound
        pop esi
        add esp,0x8
        ret
    branch_100b5ee2:
        fld float ptr [eax + 0x24]
        pop esi
        add esp,0x8
        ret
    }
}

// samp.dll RVA: 0x000b6070
__declspec(naked) bool __stdcall GameEntityCollisionQueryDirect(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,void *entity) {
    __asm {
        mov eax,dword ptr [esp + 0xc]
        mov ecx,dword ptr [esp + 0x8]
        mov edx,dword ptr [esp + 0x4]
        push eax
        push ecx
        push edx
        call dword ptr g_entityCollisionBackend
        add esp,0xc
        ret 0xc
    }
}

// samp.dll RVA: 0x000b6090
__declspec(naked) bool __stdcall GameEntityCollisionQueryScoped(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,void *entity) {
    __asm {
        mov eax,g_entityCollisionContextSlot
        mov ecx,dword ptr [eax]
        mov edx,dword ptr [esp + 0xc]
        mov dword ptr g_entityCollisionSavedContext,ecx
        mov ecx,dword ptr [esp + 0x4]
        mov dword ptr [eax],offset g_entityCollisionScratch
        mov eax,dword ptr [esp + 0x8]
        push edx
        push eax
        push ecx
        call dword ptr g_entityCollisionBackend
        mov edx,dword ptr g_entityCollisionSavedContext
        mov g_entityCollisionResult,al
        mov eax,g_entityCollisionContextSlot
        mov dword ptr [eax],edx
        mov al,g_entityCollisionResult
        add esp,0xc
        ret 0xc
    }
}
