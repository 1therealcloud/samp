#include "game/pool_helpers.h"

// GTA SA virtual addresses: pool slot B74490, thiscall backend 4442D0.
// samp.dll RVA: 0x000b37a0
int __stdcall GamePoolPedGetIndex(void *ped) {
    int result;
    __asm {
        mov ebx,0xb74490
        mov ecx,dword ptr [ebx]
        push ped
        mov ebx,0x4442d0
        call ebx
        mov result,eax
    }
    return result;
}

// GTA SA virtual addresses: pool slot B74494, thiscall backend 424160.
// samp.dll RVA: 0x000b3800
DWORD __stdcall GamePoolVehicleGetIndex(void *vehicle) {
    DWORD result=0;
    __asm {
        mov eax,0xb74494
        mov ecx,dword ptr [eax]
        push vehicle
        mov edx,0x424160
        call edx
        mov result,eax
    }
    return result;
}

// GTA SA virtual pool-pointer address: 0x00B74498.
// samp.dll RVA: 0x000b3870
void *GamePoolBuildingGetArray() { return **(void***)0xb74498; }

// GTA SA virtual pool-pointer address: 0x00B744A0.
// samp.dll RVA: 0x000b3890
void *GamePoolDummyGetArray() { return **(void***)0xb744a0; }

// GTA SA virtual pool-pointer address: 0x00B7449C.
// samp.dll RVA: 0x000b38b0
void *GamePoolObjectGetArray() { return **(void***)0xb7449c; }

// Native scans exactly 3100 records of 412 bytes, independent of pool metadata.
// Preserve the signed model-word comparison and the 863C40 vtable exclusion.
// GTA SA virtual address: object-pool pointer B7449C.
// samp.dll RVA: 0x000b3930
bool GamePoolHasObjectModel(int model) {
    DWORD address=(DWORD)**(void***)0xb7449c;
    for(DWORD index=0;index<3100;++index,address+=0x19c) {
        if(address && *(DWORD*)address && *(DWORD*)address!=0x863c40 &&
            *(short*)(address+0x22)==model)return true;
    }
    return false;
}

// GTA SA virtual addresses: scan A9C068 inclusive through AAE948 exclusive.
// Exactly 19000 DWORD slots; do not use the relocated/custom SA-MP bank here.
// samp.dll RVA: 0x000b3a00
int GameCountEmptyBackendModelSlots() {
    int count=0;
    for(DWORD *slot=(DWORD*)0xa9c068;slot!=(DWORD*)0xaae948;++slot) {
        if(*slot==0)++count;
    }
    return count;
}
