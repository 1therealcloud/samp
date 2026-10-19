#include "game/hooks_control.h"
using AimStuff::UpdateLocalSkills;
using AimStuff::ApplyPlayerSkills;
using AimStuff::ApplyLocalSkills;
using KeyStuff::UpdateKeys;
using KeyStuff::ApplyPlayerKeys;
using AimStuff::GetPlayerCameraMode;
using AimStuff::UpdateCamera;
using AimStuff::ApplyPlayerCamera;
using AimStuff::UpdateAim;
using AimStuff::ApplyPlayerAim;
using AimStuff::ApplyLocalCamera;
using AimStuff::ApplyLocalAim;
using KeyStuff::ApplyLocalKeys;
using KeyStuff::GetInternalKeys;
// Raw entry ABI: weapon ECX=ped; ped control ECX=this, [ESP+4]=ped;
// vehicle ECX=vehicle. Vehicle control preserves PUSHAD registers.
// samp.dll RVA: 0x00151964
WORD * g_hookCameraModeWord=(WORD*)0xb6f858;
// samp.dll RVA: 0x0018f4ac
BYTE g_hookSavedPlayer;
// samp.dll RVA: 0x0018f4b4
BYTE g_hookPlayerIndex;
// samp.dll RVA: 0x0018f4c0
DWORD g_hookPedControlThis;
// samp.dll RVA: 0x0018f51c
DWORD g_hookWeaponResult;
// samp.dll RVA: 0x0018f520
DWORD g_hookRestoreSkills;
// samp.dll RVA: 0x0018f524
DWORD g_hookLocalPedControl;
// samp.dll RVA: 0x0018f528
DWORD g_hookRestoreKeys;
// samp.dll RVA: 0x0018ea58
BYTE g_hookSavedCameraMode;
// samp.dll RVA: 0x0018e5c8
WORD g_hookSavedCameraModeWord;
// samp.dll RVA: 0x0018e810
DWORD g_hookVehicleProcess;
// samp.dll RVA: 0x0018e8d0
DWORD g_hookControlVehicle;
// samp.dll RVA: 0x0018ea54
DWORD g_hookVehicleVtable;
// samp.dll RVA: 0x0018e934
DWORD g_hookVehicleKeys;
// samp.dll RVA: 0x0018e938
BYTE g_hookVehicleStatus;
// samp.dll RVA: 0x0018eb20
DWORD g_hookVehicleLean;
// samp.dll RVA: 0x0018e8cc
DWORD g_hookVehicleLeanTarget;
// samp.dll RVA: 0x002a9a44
int g_gameVehicleModel;
// samp.dll RVA: 0x002a9a48
DWORD g_gameLinkedVehicle;
// samp.dll RVA: 0x002a9a4c
float g_gameVehicleDistanceSquared;
// samp.dll RVA: 0x002a9da0
DWORD g_gameLocalPedMatrix;
// samp.dll RVA: 0x002a9da4
DWORD g_gameVehicleMatrix;
// samp.dll RVA: 0x0011f5dc
const float g_gameStreamingDistanceSquared=40000.0f;
// samp.dll RVA: 0x000b3f60
BYTE __stdcall GameFindPlayerPedIndex(void *ped) {
    for(BYTE player=0;player<210;++player)if(g_gamePlayerPeds[player]==ped)return player;
    return 0;
}
// samp.dll RVA: 0x000b4410
BOOL __stdcall GameIsTrainVehicle(void *vehicle) {
    if(!vehicle)return FALSE;
    g_gameVehicleModel=*(short*)((BYTE*)vehicle+0x22);
    return g_gameVehicleModel==0x21a || g_gameVehicleModel==0x23a || g_gameVehicleModel==0x219 || g_gameVehicleModel==0x239 || g_gameVehicleModel==0x1c1;
}
// samp.dll RVA: 0x000b4460
BOOL __stdcall GameHasOccupiedLinkedVehicle(void *vehicle) {
    if(!vehicle)return FALSE;
    BYTE *linked=*(BYTE**)((BYTE*)vehicle+0x4c4);
    if(!linked)return FALSE;
    g_gameLinkedVehicle=(DWORD)linked;
    BYTE *driver=*(BYTE**)(linked+0x460);
    return driver && (*(DWORD*)(driver+0x46c)&0x100)?TRUE:FALSE;
}
// samp.dll RVA: 0x000a2ab0
__declspec(naked) void GameHook_000a2ab0() {
    __asm {
        mov dword ptr [g_hookCameraPed],ecx
        mov eax,[g_hookCameraPed]
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov [g_hookCollisionPed],eax
        mov dl,byte ptr [ecx]
        push eax
        mov byte ptr [g_hookSavedPlayer],dl
        call GameFindPlayerPedIndex
        mov ecx,dword ptr [g_hookCameraPed]
        test ecx,ecx
        mov [g_hookPlayerIndex],al
        jz branch_100a2b0c
        test al,al
        jz branch_100a2b0c
        mov al,[g_hookSavedPlayer]
        test al,al
        jnz branch_100a2b0c
        call UpdateLocalSkills
        movzx eax,byte ptr [g_hookPlayerIndex]
        push eax
        call ApplyPlayerSkills
        mov dword ptr [g_hookRestoreSkills],0x1
    branch_100a2b0c:
        mov ecx,dword ptr [g_hookCameraPed]
        movsx eax,byte ptr [ecx + 0x718]
        imul eax,eax,0x1c
        mov eax,dword ptr [eax + ecx*0x1 + 0x5a0]
        push eax
        mov edx,0x5e3b60
        call edx
        mov [g_hookWeaponResult],eax
        mov eax,[g_hookRestoreSkills]
        test eax,eax
        jz branch_100a2b48
        call ApplyLocalSkills
        mov dword ptr [g_hookRestoreSkills],0x0
    branch_100a2b48:
        mov eax,[g_hookWeaponResult]
        ret
    }
}
// samp.dll RVA: 0x000a2b50
__declspec(naked) void GameHook_000a2b50() {
    __asm {
        mov dword ptr [g_hookPedControlThis],ecx
        mov eax,dword ptr [esp + 0x4]
        mov [g_hookCameraPed],eax
        mov eax,[g_clientCrashPedContext]
        mov edx,dword ptr [g_hookCameraPed]
        mov cl,byte ptr [eax]
        push edx
        mov byte ptr [g_hookSavedPlayer],cl
        call GameFindPlayerPedIndex
        mov ecx,dword ptr [g_hookCameraPed]
        test ecx,ecx
        mov [g_hookPlayerIndex],al
        jz branch_100a2ca4
        test al,al
        jz branch_100a2ca4
        mov al,[g_hookSavedPlayer]
        test al,al
        jnz branch_100a2ca4
        call UpdateKeys
        movzx eax,byte ptr [g_hookPlayerIndex]
        push eax
        call ApplyPlayerKeys
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov dl,byte ptr [ecx]
        xor eax,eax
        mov al,[g_hookPlayerIndex]
        add esp,0x4
        mov byte ptr [g_hookSavedCameraMode],dl
        push eax
        call GetPlayerCameraMode
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [ecx],al
        mov edx,dword ptr [g_hookCameraModeWord]
        mov ax,word ptr [edx]
        xor ecx,ecx
        mov cl,byte ptr [g_hookPlayerIndex]
        mov [g_hookSavedCameraModeWord],ax
        push ecx
        call GetPlayerCameraMode
        movzx dx,al
        mov eax,[g_hookCameraModeWord]
        mov word ptr [eax],dx
        mov eax,[g_hookCameraModeWord]
        cmp word ptr [eax],0x4
        jnz branch_100a2c11
        mov word ptr [eax],0x0
    branch_100a2c11:
        call UpdateCamera
        xor ecx,ecx
        mov cl,byte ptr [g_hookPlayerIndex]
        push ecx
        call ApplyPlayerCamera
        call UpdateAim
        movzx edx,byte ptr [g_hookPlayerIndex]
        push edx
        call ApplyPlayerAim
        call UpdateLocalSkills
        movzx eax,byte ptr [g_hookPlayerIndex]
        push eax
        call ApplyPlayerSkills
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov dl,byte ptr [g_hookPlayerIndex]
        mov byte ptr [ecx],dl
        mov ecx,dword ptr [g_hookPedControlThis]
        push dword ptr [g_hookCameraPed]
        mov eax,0x624ed0
        call eax
        mov al,[g_hookSavedCameraMode]
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [ecx],al
        mov dx,word ptr [g_hookSavedCameraModeWord]
        mov eax,[g_hookCameraModeWord]
        mov word ptr [eax],dx
        call ApplyLocalSkills
        call ApplyLocalCamera
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov byte ptr [ecx],0x0
        call ApplyLocalAim
        call ApplyLocalKeys
        jmp branch_100a2ccb
    branch_100a2ca4:
        mov dword ptr [g_hookLocalPedControl],0x1
        mov ecx,dword ptr [g_hookPedControlThis]
        push dword ptr [g_hookCameraPed]
        mov eax,0x624ed0
        call eax
        mov dword ptr [g_hookLocalPedControl],0x0
    branch_100a2ccb:
        ret 0x4
    }
}
// samp.dll RVA: 0x000a2e70
__declspec(naked) void GameHook_000a2e70() {
    __asm {
        mov dword ptr [g_hookControlVehicle],ecx
        mov eax,dword ptr [ecx]
        mov [g_hookVehicleVtable],eax
        pushad
        mov eax,[g_hookVehicleVtable]
        cmp eax,0x871120
        mov edi,0x871680
        jnz branch_100a2e9e
        mov dword ptr [g_hookVehicleProcess],0x6b1880
        jmp branch_100a2f4a
    branch_100a2e9e:
        cmp eax,0x8721a0
        jnz branch_100a2eb4
        mov dword ptr [g_hookVehicleProcess],0x6f1770
        jmp branch_100a2f4a
    branch_100a2eb4:
        cmp eax,0x871360
        jnz branch_100a2eca
        mov dword ptr [g_hookVehicleProcess],0x6b9250
        jmp branch_100a2f4a
    branch_100a2eca:
        cmp eax,0x871948
        jnz branch_100a2edd
        mov dword ptr [g_hookVehicleProcess],0x6c9260
        jmp branch_100a2f4a
    branch_100a2edd:
        cmp eax,edi
        jnz branch_100a2eed
        mov dword ptr [g_hookVehicleProcess],0x6c7050
        jmp branch_100a2f4a
    branch_100a2eed:
        cmp eax,0x871528
        jnz branch_100a2f00
        mov dword ptr [g_hookVehicleProcess],0x6bfa30
        jmp branch_100a2f4a
    branch_100a2f00:
        cmp eax,0x8717d8
        jnz branch_100a2f13
        mov dword ptr [g_hookVehicleProcess],0x6c8250
        jmp branch_100a2f4a
    branch_100a2f13:
        cmp eax,0x871ae8
        jnz branch_100a2f26
        mov dword ptr [g_hookVehicleProcess],0x6cdcc0
        jmp branch_100a2f4a
    branch_100a2f26:
        cmp eax,0x872370
        jnz branch_100a2f39
        mov dword ptr [g_hookVehicleProcess],0x6f86a0
        jmp branch_100a2f4a
    branch_100a2f39:
        cmp eax,0x871c28
        jnz branch_100a2f4a
        mov dword ptr [g_hookVehicleProcess],0x6ced20
    branch_100a2f4a:
        mov al,ds:[0x00b7cd74]
        mov ecx,dword ptr [g_hookControlVehicle]
        mov [g_hookSavedPlayer],al
        mov esi,dword ptr [ecx + 0x460]
        xor ebx,ebx
        cmp esi,ebx
        jz branch_100a3331
        cmp dword ptr [esi + 0x598],ebx
        jnz branch_100a327a
        call GameFindLocalPed
        cmp esi,eax
        jz branch_100a3274
        cmp byte ptr [g_hookSavedPlayer],bl
        jnz branch_100a3274
        push esi
        call GameFindPlayerPedIndex
        mov [g_hookPlayerIndex],al
        call UpdateKeys
        movzx ecx,byte ptr [g_hookPlayerIndex]
        push ecx
        call ApplyPlayerKeys
        mov edx,dword ptr [g_aimInternalCameraMode]
        mov al,byte ptr [edx]
        xor ecx,ecx
        mov cl,byte ptr [g_hookPlayerIndex]
        add esp,0x4
        mov [g_hookSavedCameraMode],al
        push ecx
        call GetPlayerCameraMode
        mov edx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [edx],al
        mov eax,[g_hookCameraModeWord]
        mov cx,word ptr [eax]
        xor edx,edx
        mov dl,byte ptr [g_hookPlayerIndex]
        mov word ptr [g_hookSavedCameraModeWord],cx
        push edx
        call GetPlayerCameraMode
        mov ecx,dword ptr [g_hookCameraModeWord]
        movzx ax,al
        mov word ptr [ecx],ax
        mov eax,[g_hookCameraModeWord]
        cmp word ptr [eax],0x4
        jnz branch_100a300a
        mov word ptr [eax],bx
    branch_100a300a:
        call UpdateAim
        movzx edx,byte ptr [g_hookPlayerIndex]
        push edx
        call ApplyPlayerAim
        mov eax,[g_hookControlVehicle]
        test byte ptr [eax + 0x428],0x10
        jnz branch_100a3063
        cmp dword ptr [g_hookVehicleVtable],edi
        jnz branch_100a3036
        or byte ptr [eax + 0x36],0x20
    branch_100a3036:
        call GetInternalKeys
        mov [g_hookVehicleKeys],eax
        mov word ptr [eax + 0x1c],bx
        mov eax,[g_hookVehicleKeys]
        mov word ptr [eax + 0x4c],bx
        mov ecx,dword ptr [g_hookVehicleKeys]
        mov word ptr [ecx + 0x20],bx
        mov edx,dword ptr [g_hookVehicleKeys]
        mov word ptr [edx + 0x50],bx
        jmp branch_100a3067
    branch_100a3063:
        and byte ptr [eax + 0x36],0xdf
    branch_100a3067:
        mov eax,[g_hookControlVehicle]
        mov ecx,dword ptr [eax + 0x648]
        mov dword ptr [g_hookVehicleLean],ecx
        mov edx,dword ptr [eax + 0x64c]
        mov eax,[g_clientCrashPedContext]
        mov dword ptr [g_hookVehicleLeanTarget],edx
        mov byte ptr [eax],bl
        mov ecx,dword ptr [g_hookControlVehicle]
        mov edx,dword ptr [ecx + 0x460]
        mov dword ptr [edx + 0x598],0x4
        mov eax,[g_hookControlVehicle]
        mov cl,byte ptr [eax + 0x36]
        mov byte ptr [g_hookVehicleStatus],cl
        mov byte ptr [eax + 0x36],0x1a
        mov edx,dword ptr [g_hookControlVehicle]
        lea ecx,[edx + 0x138]
        mov edx,0x502280
        call edx
        mov al,[g_hookVehicleStatus]
        mov edx,dword ptr [g_hookControlVehicle]
        mov byte ptr [edx + 0x36],al
        mov ecx,dword ptr [g_hookControlVehicle]
        mov edx,dword ptr [ecx + 0x460]
        mov dword ptr [edx + 0x598],ebx
        mov al,[g_hookPlayerIndex]
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov byte ptr [ecx],al
        mov eax,[g_hookControlVehicle]
        mov esi,dword ptr [eax + 0x14]
        cmp esi,ebx
        jz branch_100a3126
        mov ecx,0x10
        mov edi,offset g_hookSavedMatrix
        rep movsd
        mov edx,dword ptr [eax + 0x44]
        mov dword ptr [g_hookSavedSpeed],edx
        mov ecx,dword ptr [eax + 0x48]
        mov dword ptr [g_hookSavedSpeed+4],ecx
        mov edx,dword ptr [eax + 0x4c]
        mov dword ptr [g_hookSavedSpeed+8],edx
    branch_100a3126:
        mov ecx,dword ptr [g_hookControlVehicle]
        mov eax,[g_hookVehicleProcess]
        call eax
        mov eax,[g_hookControlVehicle]
        cmp dword ptr [eax + 0x14],ebx
        jz branch_100a3169
        push eax
        call GameIsVehicleInStreamingArea
        test eax,eax
        jnz branch_100a3169
        mov eax,[g_hookControlVehicle]
        push eax
        call GameIsTrainVehicle
        test eax,eax
        jnz branch_100a3169
        mov ecx,dword ptr [g_hookControlVehicle]
        mov edx,dword ptr [ecx + 0x14]
        mov eax,[g_hookSavedMatrix+56]
        mov dword ptr [edx + 0x38],eax
        jmp branch_100a3198
    branch_100a3169:
        mov edx,dword ptr [g_hookControlVehicle]
        mov ecx,dword ptr [edx + 0x14]
        cmp ecx,ebx
        jz branch_100a31a7
        mov eax,[g_hookVehicleProcess]
        cmp eax,0x6f1770
        jz branch_100a3190
        cmp eax,0x6c9260
        jz branch_100a3190
        cmp eax,0x6c7050
        jnz branch_100a31ac
    branch_100a3190:
        mov eax,[g_hookSavedMatrix+56]
        mov dword ptr [ecx + 0x38],eax
    branch_100a3198:
        fld dword ptr [g_hookSavedSpeed+8]
        mov ecx,dword ptr [g_hookControlVehicle]
        fstp dword ptr [ecx + 0x4c]
    branch_100a31a7:
        mov eax,[g_hookVehicleProcess]
    branch_100a31ac:
        cmp eax,0x6bfa30
        jz branch_100a31be
        cmp eax,0x6b9250
        jnz branch_100a323f
    branch_100a31be:
        mov eax,[g_hookControlVehicle]
        mov edx,dword ptr [g_hookVehicleLean]
        mov dword ptr [eax + 0x648],edx
        fld dword ptr [g_hookVehicleLeanTarget]
        mov ecx,dword ptr [g_hookControlVehicle]
        fstp dword ptr [ecx + 0x64c]
        mov ecx,dword ptr [g_hookControlVehicle]
        fld dword ptr [ecx + 0x64c]
        fcomp dword ptr [ecx + 0x648]
        fld dword ptr [ecx + 0x64c]
        fnstsw ax
        test ah,0x41
        jnz branch_100a3214
        fsub dword ptr [ecx + 0x648]
        fmul dword ptr [g_textDrawHalf]
        fadd dword ptr [ecx + 0x648]
        jmp branch_100a3239
    branch_100a3214:
        fcomp dword ptr [ecx + 0x648]
        fnstsw ax
        test ah,0x5
        jp branch_100a323f
        fld dword ptr [ecx + 0x648]
        fsub dword ptr [ecx + 0x64c]
        fmul dword ptr [g_textDrawHalf]
        fsubr dword ptr [ecx + 0x648]
    branch_100a3239:
        fstp dword ptr [ecx + 0x648]
    branch_100a323f:
        mov edx,dword ptr [g_clientCrashPedContext]
        mov byte ptr [edx],bl
        call ApplyLocalKeys
        mov al,[g_hookSavedCameraMode]
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [ecx],al
        mov dx,word ptr [g_hookSavedCameraModeWord]
        mov eax,[g_hookCameraModeWord]
        mov word ptr [eax],dx
        call ApplyLocalAim
        popad
        ret
    branch_100a3274:
        mov ecx,dword ptr [g_hookControlVehicle]
    branch_100a327a:
        cmp esi,ebx
        jz branch_100a3331
        cmp dword ptr [esi + 0x598],ebx
        jnz branch_100a3331
        call GameFindLocalPed
        mov ecx,dword ptr [g_hookControlVehicle]
        cmp dword ptr [ecx + 0x460],eax
        jnz branch_100a3331
        test byte ptr [ecx + 0x428],0x10
        jnz branch_100a32f6
        cmp dword ptr [g_hookVehicleVtable],edi
        jnz branch_100a32ba
        or byte ptr [ecx + 0x36],0x20
    branch_100a32ba:
        mov dword ptr [g_hookRestoreKeys],0x1
        call UpdateKeys
        call GetInternalKeys
        mov [g_hookVehicleKeys],eax
        mov word ptr [eax + 0x1c],bx
        mov ecx,dword ptr [g_hookVehicleKeys]
        mov word ptr [ecx + 0x4c],bx
        mov edx,dword ptr [g_hookVehicleKeys]
        mov word ptr [edx + 0x20],bx
        mov eax,[g_hookVehicleKeys]
        mov word ptr [eax + 0x50],bx
        jmp branch_100a32fa
    branch_100a32f6:
        and byte ptr [ecx + 0x36],0xdf
    branch_100a32fa:
        mov edx,dword ptr [g_hookControlVehicle]
        lea ecx,[edx + 0x138]
        mov edx,0x502280
        call edx
        mov ecx,dword ptr [g_hookControlVehicle]
        mov eax,[g_hookVehicleProcess]
        call eax
        cmp dword ptr [g_hookRestoreKeys],ebx
        jz branch_100a332d
        mov dword ptr [g_hookRestoreKeys],ebx
        call ApplyLocalKeys
    branch_100a332d:
        popad
        ret
    branch_100a3331:
        push ecx
        call GameHasOccupiedLinkedVehicle
        test eax,eax
        jnz branch_100a335b
        mov ecx,dword ptr [g_hookControlVehicle]
        push ecx
        call GameIsTrainVehicle
        test eax,eax
        jnz branch_100a335b
        mov edx,dword ptr [g_hookControlVehicle]
        push edx
        call GameIsVehicleInStreamingArea
        test eax,eax
        jz branch_100a33ad
    branch_100a335b:
        mov eax,[g_hookControlVehicle]
        mov esi,dword ptr [eax + 0x14]
        cmp esi,ebx
        jz branch_100a338d
        mov ecx,0x10
        mov edi,offset g_hookSavedMatrix
        rep movsd
        mov ecx,dword ptr [eax + 0x44]
        mov dword ptr [g_hookSavedSpeed],ecx
        mov edx,dword ptr [eax + 0x48]
        mov dword ptr [g_hookSavedSpeed+4],edx
        mov eax,dword ptr [eax + 0x4c]
        mov [g_hookSavedSpeed+8],eax
    branch_100a338d:
        mov edx,dword ptr [g_hookControlVehicle]
        lea ecx,[edx + 0x138]
        mov edx,0x502280
        call edx
        mov ecx,dword ptr [g_hookControlVehicle]
        mov eax,[g_hookVehicleProcess]
        call eax
    branch_100a33ad:
        popad
        ret
    }
}
// Retain the unrounded x87 threshold test and native argument cleanup.
// samp.dll RVA: 0x000b6490
__declspec(naked) BOOL __stdcall GameIsVehicleInStreamingArea(void *vehicle) {
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        mov eax,ds:[0x00b7cd98]
        mov eax,dword ptr [eax + 0x14]
        test eax,eax
        mov ecx,dword ptr [ebp + 0x8]
        mov [g_gameLocalPedMatrix],eax
        mov ecx,dword ptr [ecx + 0x14]
        mov dword ptr [g_gameVehicleMatrix],ecx
        jz branch_100b6519
        test ecx,ecx
        jz branch_100b6519
        fld dword ptr [ecx + 0x34]
        mov edx,dword ptr [eax + 0x34]
        fld dword ptr [ecx + 0x30]
        add ecx,0x30
        fsubr dword ptr [eax + 0x30]
        mov dword ptr [ebp + 0x8],edx
        fld dword ptr [ebp + 0x8]
        fsub st(0),st(2)
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fst dword ptr [g_gameVehicleDistanceSquared]
        fstp st(3)
        fstp st(0)
        fstp st(0)
        fcomp dword ptr [g_gameStreamingDistanceSquared]
        fnstsw ax
        test ah,0x41
        jp branch_100b6519
        mov dword ptr [ebp + -0x4],ecx
        mov byte ptr [ebp + 0xb],0x0
        push 0x0
        push dword ptr [ebp + -0x4]
        mov edx,0x410ce0
        call edx
        mov byte ptr [ebp + 0xb],al
        pop edx
        pop edx
        mov al,byte ptr [ebp + 0xb]
        test al,al
        jz branch_100b6519
        mov eax,0x1
        mov esp,ebp
        pop ebp
        ret 0x4
    branch_100b6519:
        xor eax,eax
        mov esp,ebp
        pop ebp
        ret 0x4
    }
}
