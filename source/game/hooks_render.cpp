#include "game/hooks_render.h"
#include "game/input.h"
#include "game/playerped.h"
#include "game/building_removal.h"
// samp.dll RVA: 0x0018f504
DWORD g_hookHudScaleX;
// samp.dll RVA: 0x0018f508
DWORD g_hookHudScaleY;
// samp.dll RVA: 0x0018f50c
float g_hookHudWidth;
// samp.dll RVA: 0x0018f510
float g_hookHudHeight;
// samp.dll RVA: 0x00151a6e
BYTE g_hookHudAspectFix=1;
// samp.dll RVA: 0x0011c728
const float g_hookHudAspectThreshold=1.6f;
// samp.dll RVA: 0x0011eb68
const float g_hookHudScaleNumerator=0.00221999991f;
// samp.dll RVA: 0x002aca6c
DWORD g_hookFogEnabled;
// samp.dll RVA: 0x002aca70
DWORD g_hookDisableFog;
// samp.dll RVA: 0x0018e560
DWORD g_hookCollisionPed;
// samp.dll RVA: 0x0018e8e0
DWORD g_hookSavedMatrix[16];
// samp.dll RVA: 0x0018eab4
DWORD g_hookSavedSpeed[3];
// samp.dll RVA: 0x0018f52c
DWORD g_hookHornArgument;
// samp.dll RVA: 0x0018f530
DWORD g_hookHornThis;
// samp.dll RVA: 0x0018eb24
DWORD g_hookHornVehicle;
// samp.dll RVA: 0x0018f534
BYTE g_hookHornStatus;
// samp.dll RVA: 0x0018f4c4
DWORD g_hookAnimationReturn;
// samp.dll RVA: 0x0018e584
DWORD g_hookAnimationClump;
// samp.dll RVA: 0x0018f548
DWORD g_hookAnimationGroup;
// samp.dll RVA: 0x0018f54c
DWORD g_hookAnimationId;
// samp.dll RVA: 0x0018f550
DWORD g_hookAnimationBlend;
// samp.dll RVA: 0x0018eb90
DWORD g_hookAnimationPed;
// samp.dll RVA: 0x0018eb88
DWORD g_hookAnimationLocalClump;
// samp.dll RVA: 0x0018f4ec
DWORD g_hookAnimationType;
// samp.dll RVA: 0x0018f4f0
DWORD g_hookQueuedAnimationGroup;
// samp.dll RVA: 0x0018f4f4
DWORD g_hookQueuedAnimationId;
// samp.dll RVA: 0x0018f4f8
DWORD g_hookQueuedAnimationBlend;
// samp.dll RVA: 0x00151aa4
DWORD g_hookAnimationContinueA=0x4d3aaa;
// samp.dll RVA: 0x00151aa8
DWORD g_hookAnimationContinueB=0x4d4617;
// samp.dll RVA: 0x0018f5cc
DWORD g_hookRenderObject;
// samp.dll RVA: 0x00151acc
BYTE g_hookObjectRestore[5]={0xe8,0x3c,0xff,0xff,0xff};
// samp.dll RVA: 0x0018f5b4
DWORD g_hookMarker;
// samp.dll RVA: 0x0018f5b8
DWORD g_hookMarkerArgument;
// samp.dll RVA: 0x0018f5bc
DWORD g_hookSuppressAllMarkers;
// The comparison and repeated division retain x87 precision until the final
// float store. A rounded float ratio changes the native 16:10 boundary.
// samp.dll RVA: 0x000a2220
void GamePrepareHudDraw() {
    g_hookHudScaleX=*(DWORD*)0x859520;g_hookHudScaleY=*(DWORD*)0x859524;
    BYTE enabled=g_hookHudAspectFix;
    g_hookHudWidth=(float)*(int*)0xc17044;g_hookHudHeight=(float)*(int*)0xc17048;
    if(enabled) {
        __asm {
            fld g_hookHudWidth
            fdiv g_hookHudHeight
            fcomp g_hookHudAspectThreshold
            fnstsw ax
            test ah,1
            jnz done
            mov dword ptr ds:[0x859524],0x3b1e98dd
            fld g_hookHudWidth
            fdiv g_hookHudHeight
            fdivr g_hookHudScaleNumerator
            fstp dword ptr ds:[0x859520]
        done:
        }
    }
}
// samp.dll RVA: 0x000c5400
void GameSetupD3DFog(DWORD enabled) {
    if(g_hookDisableFog)enabled=0;
    if(g_gameD3DDevice) {
        g_gameD3DDevice->SetRenderState(D3DRS_FOGENABLE,enabled);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_NONE);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
    }
}
// samp.dll RVA: 0x000a22c0
__declspec(naked) void GameHook_000a22c0() {
    __asm {
        pushad
        call GamePrepareHudDraw
        popad
        mov edx,0x58a330
        call edx
        pushad
        mov eax,g_hookHudScaleX
        mov dword ptr ds:[0x859520],eax
        mov ecx,g_hookHudScaleY
        mov dword ptr ds:[0x859524],ecx
        popad
        ret
    }
}
// samp.dll RVA: 0x000a29d0
__declspec(naked) void GameHook_000a29d0() {
    __asm {
        mov g_hookCameraPed,ecx
        pushad
        mov ecx,g_hookCameraPed
        mov edx,ecx
        mov g_hookCollisionPed,edx
        mov eax,[ecx+0x40]
        xor ebx,ebx
        test eax,0x40000
        jz save
        test eax,0x800000
        jz save
        test eax,0x200000
        jz save
        mov [ecx+0x44],ebx
        mov eax,g_hookCollisionPed
        mov [eax+0x48],ebx
        mov edx,g_hookCollisionPed
    save:
        mov esi,[edx+0x14]
        cmp esi,ebx
        jz original
        mov ecx,16
        mov edi,offset g_hookSavedMatrix
        rep movsd
        mov ecx,[edx+0x44]
        mov g_hookSavedSpeed[0],ecx
        mov eax,[edx+0x48]
        mov g_hookSavedSpeed[4],eax
        mov ecx,[edx+0x4c]
        mov g_hookSavedSpeed[8],ecx
    original:
        mov ecx,g_hookCameraPed
        mov edx,0x5ddbf0
        call edx
        mov ecx,g_hookCollisionPed
        mov eax,[ecx+0x40]
        test eax,0x40000
        jz done
        test eax,0x800000
        jz done
        test eax,0x200000
        jz done
        mov eax,[ecx+0x14]
        cmp eax,ebx
        jz zero_speed
        mov edx,g_hookSavedMatrix[48]
        mov [eax+0x30],edx
        mov eax,g_hookCollisionPed
        mov ecx,[eax+0x14]
        mov edx,g_hookSavedMatrix[52]
        mov [ecx+0x34],edx
        mov eax,g_hookCollisionPed
        mov ecx,[eax+0x14]
        mov edx,g_hookSavedMatrix[56]
        mov [ecx+0x38],edx
        mov ecx,g_hookCollisionPed
    zero_speed:
        mov [ecx+0x44],ebx
        mov eax,g_hookCollisionPed
        mov [eax+0x48],ebx
    done:
        popad
        ret
    }
}
// Original thiscall consumes one stack argument and restores only the status
// byte it saved, leaving the GTA callee's other results intact.
// samp.dll RVA: 0x000a33c0
__declspec(naked) void GameHook_000a33c0() {
    __asm {
        mov g_hookHornThis,ecx
        mov edx,[esp+4]
        mov g_hookHornArgument,edx
        mov eax,[edx+0x10]
        mov g_hookHornVehicle,eax
        mov eax,g_hookHornVehicle
        mov cl,[eax+0x36]
        mov g_hookHornStatus,cl
        mov ecx,[eax+0x460]
        test ecx,ecx
        jz non_player
        mov edx,[ecx+0x46c]
        test dh,1
        jz non_player
        mov byte ptr [eax+0x36],2
        jmp original
    non_player:
        mov byte ptr [eax+0x36],0x22
    original:
        push g_hookHornArgument
        mov ecx,g_hookHornThis
        mov edx,0x5002c0
        call edx
        mov edx,g_hookHornVehicle
        mov al,g_hookHornStatus
        mov [edx+0x36],al
        ret 4
    }
}
// samp.dll RVA: 0x000a3b90
__declspec(naked) void GameHook_000a3b90() {
    __asm {
        pushad
        push 0
        call GameSetupD3DFog
        add esp,4
        popad
        mov edx,0x5e7680
        call edx
        pushad
        mov eax,g_hookFogEnabled
        test eax,eax
        jz disabled
        push 1
        jmp restore
    disabled:
        push 0
    restore:
        call GameSetupD3DFog
        add esp,4
        popad
        ret
    }
}
// samp.dll RVA: 0x000a3f30
__declspec(naked) void GameHook_000a3f30() {
    __asm {
        mov edx,[esp]
        mov g_hookAnimationReturn,edx
        mov edx,[esp+4]
        mov g_hookAnimationClump,edx
        mov edx,[esp+8]
        mov g_hookAnimationGroup,edx
        mov edx,[esp+12]
        mov g_hookAnimationId,edx
        pushad
        call GameFindLocalPed
        mov g_hookAnimationPed,eax
        cmp g_hookAnimationPed,0
        jz missing
        mov eax,g_hookAnimationPed
        mov ecx,[eax+0x18]
        mov g_hookAnimationLocalClump,ecx
        jmp test_clump
    missing:
        mov g_hookAnimationLocalClump,0
    test_clump:
        mov edx,g_hookAnimationClump
        cmp edx,g_hookAnimationLocalClump
        jne original
        cmp g_hookAnimationGroup,0
        jne queue
        cmp g_hookAnimationId,0xa0
        je original
    queue:
        mov g_hookAnimationType,1
        mov eax,g_hookAnimationGroup
        mov g_hookQueuedAnimationGroup,eax
        mov ecx,g_hookAnimationId
        mov g_hookQueuedAnimationId,ecx
    original:
        popad
        mov eax,[esp+12]
        mov edx,dword ptr ds:[0xb4ea34]
        jmp dword ptr [g_hookAnimationContinueA]
    }
}
// samp.dll RVA: 0x000a3fe0
__declspec(naked) void GameHook_000a3fe0() {
    __asm {
        mov edx,[esp]
        mov g_hookAnimationReturn,edx
        mov edx,[esp+4]
        mov g_hookAnimationClump,edx
        mov edx,[esp+8]
        mov g_hookAnimationGroup,edx
        mov edx,[esp+12]
        mov g_hookAnimationId,edx
        mov edx,[esp+16]
        mov g_hookAnimationBlend,edx
        pushad
        call GameFindLocalPed
        mov g_hookAnimationPed,eax
        cmp g_hookAnimationPed,0
        jz missing
        mov eax,g_hookAnimationPed
        mov ecx,[eax+0x18]
        mov g_hookAnimationLocalClump,ecx
        jmp test_clump
    missing:
        mov g_hookAnimationLocalClump,0
    test_clump:
        mov edx,g_hookAnimationClump
        cmp edx,g_hookAnimationLocalClump
        jne original
        cmp g_hookAnimationGroup,0
        jne queue
        cmp g_hookAnimationId,0xa0
        je original
    queue:
        mov g_hookAnimationType,2
        mov eax,g_hookAnimationGroup
        mov g_hookQueuedAnimationGroup,eax
        mov ecx,g_hookAnimationId
        mov g_hookQueuedAnimationId,ecx
        mov edx,g_hookAnimationBlend
        mov g_hookQueuedAnimationBlend,edx
    original:
        popad
        sub esp,0x14
        mov ecx,[esp+0x18]
        jmp dword ptr [g_hookAnimationContinueB]
    }
}
// samp.dll RVA: 0x000a47d0
__declspec(naked) void GameHook_000a47d0() {
    __asm {
        mov eax,[esp+4]
        mov g_hookMarker,eax
        mov eax,[esp+8]
        mov g_hookMarkerArgument,eax
        pushad
        cmp g_hookMarker,0
        je override_all
        mov eax,g_hookMarker
        mov ecx,[eax+8]
        push ecx
        mov edx,g_hookMarker
        mov eax,[edx+4]
        push eax
        mov ecx,g_hookMarker
        mov edx,[ecx]
        push edx
        mov eax,g_hookMarker
        mov ecx,[eax+0x1c]
        push ecx
        call GameShouldSuppressMarker
        add esp,16
        test eax,eax
        je override_all
        mov edx,g_hookMarker
        mov dword ptr [edx+0x1c],0x4b64
    override_all:
        cmp g_hookSuppressAllMarkers,0
        je original
        mov eax,g_hookMarker
        mov dword ptr [eax+0x1c],0x4b64
    original:
        popad
        push -1
        push 0x83c931
        mov eax,0x538097
        jmp eax
    }
}
// samp.dll RVA: 0x000a48c0
__declspec(naked) void GameHook_000a48c0() {
    __asm {
        mov eax,[esp]
        mov g_hookRenderReturn,eax
        mov g_hookRenderObject,ecx
        pushad
        mov eax,g_hookRenderObject
        movsx ecx,word ptr [eax+0x22]
        cmp ecx,0xe02
        je patch
        mov edx,g_hookRenderObject
        movsx eax,word ptr [edx+0x22]
        cmp eax,0xe9f
        je patch
        mov ecx,g_hookRenderObject
        movsx edx,word ptr [ecx+0x22]
        cmp edx,0x2314
        je patch
        mov eax,g_hookRenderObject
        movsx ecx,word ptr [eax+0x22]
        cmp ecx,0x2313
        jne original
    patch:
        push 5
        push 0x71791f
        call GameUnprotect
        add esp,8
        mov edx,0x90909090
        mov eax,0x71791f
        mov [eax],edx
        mov [eax+4],dl
        popad
        mov eax,0x534030
        call eax
        pushad
        mov ecx,0x71791f
        mov edx,dword ptr g_hookObjectRestore
        mov [ecx],edx
        mov al,byte ptr g_hookObjectRestore[4]
        mov [ecx+4],al
        popad
        ret
    original:
        popad
        mov eax,0x534030
        jmp eax
    }
}
