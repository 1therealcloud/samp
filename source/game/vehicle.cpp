#include "game/vehicle.h"
// samp.dll RVA: 0x000b37d0
void *__stdcall GamePoolVehicleGetAt(int handle) {
    void *result;
    __asm {
        mov ecx,dword ptr ds:[0xb74494]
        push handle
        mov eax,0x4048e0
        call eax
        mov result,eax
    }
    return result;
}
// samp.dll RVA: 0x000b6e20
void CVehicle::SetColor(BYTE primary,BYTE secondary) {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)) {
        ((BYTE*)m_pGameVehicle)[0x434]=primary;((BYTE*)m_pGameVehicle)[0x435]=secondary;
    }
    m_nSecondaryColor=secondary;m_nPrimaryColor=primary;m_bColorChanged=TRUE;
}

#include "main.h"
#include "game/model.h"
#include "game/wrapper_commands.h"

// Constructor remains open; no default initialization or new creation behavior.
// samp.dll RVA: 0x000b8420
CVehicle::~CVehicle() {
    m_pGameVehicle=GamePoolVehicleGetAt(m_nHandle);
    if(m_pGameVehicle) {
        DWORD *marker=(DWORD*)((BYTE*)this+0xb4);
        if(*marker) {
            Scripting::ProcessCommand(&g_vehicleDestroyMarkerCommand,*marker);
            *marker=0;
        }
        RemovePassengers();
        if(m_pTrailer) {
            DetachTrailer();
            m_pTrailer=0;
        }
        ShutdownLicensePlate();
        short model=*(short*)((BYTE*)m_pGameVehicle+0x22);
        if(model==0x21a || model==0x219) {
            Scripting::ProcessCommand(&g_vehicleDestroyTrainCommand,m_nHandle);
        } else {
            int modelSnapshot=model;
            Scripting::ProcessCommand(&g_vehicleDestroyCommand,m_nHandle);
            if(GameGetModelReferenceCount(modelSnapshot)==0 && !*(DWORD*)((BYTE*)this+0x87) &&
                g_pGame->CountBackendVehicleModelRecords()>80 && g_pGame->IsModelAvailable(modelSnapshot)) {
                ((void(__cdecl*)(int))0x4089a0)(modelSnapshot);
            }
        }
    }
}

// Preserve base qualification and virtual dispatch to an existing trailer.
// samp.dll RVA: 0x000b8540
void CVehicle::Add() {
    if(!CEntity::DoesExist()) {
        CEntity::Add();
        CVehicle *trailer=GetTrailer();
        if(trailer)trailer->Add();
    }
}

// Trailer is removed before the qualified base vehicle removal.
// samp.dll RVA: 0x000b8570
void CVehicle::Remove() {
    if(CEntity::DoesExist()) {
        CVehicle *trailer=GetTrailer();
        if(trailer)trailer->Remove();
        CEntity::Remove();
    }
}
