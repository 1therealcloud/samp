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

#include "chatwindow.h"
#include "game/pool_helpers.h"
#include "game/object_math.h"
#include "game/hooks_vehicle.h"
#include <string.h>

// samp.dll RVA: 0x002abe64
void *g_vehicleTrainCursor;
// samp.dll RVA: 0x002abe68
DWORD g_vehicleLastCreatedHandle;
// samp.dll RVA: 0x0011e33c
const float g_vehicleCreationLift=0.25f;
// samp.dll RVA: 0x0011f7e0
const char g_vehicleBadCarriageWarning[29]="Warning: bad train carriages";
// samp.dll RVA: 0x0011f800
const char g_vehicleCreationWarning[42]="Warning: couldn't create vehicle type: %u";

// Preserve base construction, untouched early-return bytes and native global reloads.
// samp.dll RVA: 0x000b7f00
CVehicle::CVehicle(int model,float x,float y,float z,float rotation,DWORD keepLoaded,BOOL siren) {
    DWORD handle=0;
    m_pGameVehicle=0;
    m_nHandle=0;
    m_pTrailer=0;
    if(model!=0x21a && model!=0x219 && model!=0x23a && model!=0x239 && model!=0x1c1) {
        if(!g_pGame->IsModelAvailable(model)) {
            g_pGame->RequestModel(model,2);
            g_pGame->LoadRequestedModels();
            if(!g_pGame->IsModelAvailable(model)) {
                // Keep the resolved Sleep target for the entire native wait loop.
                void (WINAPI *volatile sleep)(DWORD)=Sleep;
                do { sleep(0); } while(!g_pGame->IsModelAvailable(model));
            }
        }
        if(siren) {
            g_hookSirenOnce=1;
            *(DWORD*)((BYTE*)this+0x8b)=1;
        } else *(DWORD*)((BYTE*)this+0x8b)=0;
        Scripting::ProcessCommand(&g_vehicleCreateCommand,model,(double)x,(double)y,(double)z,&handle);
        Scripting::ProcessCommand(&g_vehicleRotationCommand,handle,(double)rotation);
        Scripting::ProcessCommand(&g_vehicleGasTankExplosionCommand,handle,0);
        Scripting::ProcessCommand(&g_vehicleHydraulicsCommand,handle,0);
        Scripting::ProcessCommand(&g_vehicleDamageableCommand,handle,0);
        m_pGameVehicle=GamePoolVehicleGetAt(handle);
        m_pGameEntity=m_pGameVehicle;
        m_nHandle=handle;
        if(!m_pGameVehicle) {
            g_pChat->AddDebugMessage(g_vehicleCreationWarning,(unsigned int)model);
            return;
        }
        g_vehicleLastCreatedHandle=handle;
        *(DWORD*)((BYTE*)m_pGameVehicle+0x4f8)=0;
        *(DWORD*)((BYTE*)m_pGameVehicle+0x4b0)=0;
        *(DWORD*)((BYTE*)this+0x5c)=0;
        CMatrix matrix;
        GetMatrix(&matrix);
        BOOL lift=GetSubtype()!=2 && GetSubtype()!=6;
        DWORD finalZ;
        __asm {
            fld z
            cmp lift,0
            je noLift
            fadd dword ptr [g_vehicleCreationLift]
noLift:
            fstp dword ptr finalZ
        }
        *(DWORD*)&matrix.position.x=*(DWORD*)&x;
        *(DWORD*)&matrix.position.y=*(DWORD*)&y;
        *(DWORD*)&matrix.position.z=finalZ;
        SetMatrix(matrix);
        *(DWORD*)((BYTE*)this+0x87)=keepLoaded;
    } else if(model==0x23a || model==0x239) {
        if(g_vehicleTrainCursor) {
            m_pGameVehicle=*(void**)((BYTE*)g_vehicleTrainCursor+0x5d4);
            if(!m_pGameVehicle) {
                g_pChat->AddDebugMessage(g_vehicleBadCarriageWarning);
            } else {
                handle=GamePoolVehicleGetIndex(m_pGameVehicle);
                m_nHandle=handle;
                m_pGameEntity=m_pGameVehicle;
                g_vehicleLastCreatedHandle=handle;
                g_vehicleTrainCursor=m_pGameVehicle;
                goto initialized;
            }
        }
        m_pGameEntity=0;
        m_pGameVehicle=0;
        g_vehicleTrainCursor=0;
        return;
    } else {
        int trainType=model==0x21a?5:model==0x219?3:9;
        DWORD direction;
        __asm {
            mov direction,0
            fld rotation
            fcomp dword ptr [g_angleUpperBound]
            fnstsw ax
            test ah,0x41
            jnz directionDone
            mov direction,1
 directionDone:
        }
        g_pGame->RequestModel(0x21a,2);
        g_pGame->RequestModel(0x23a,2);
        g_pGame->RequestModel(0x219,2);
        g_pGame->RequestModel(0x239,2);
        g_pGame->RequestModel(0x1c1,2);
        g_pGame->LoadRequestedModels();
        BOOL available=g_pGame->IsModelAvailable(0x21a);
        // The five train waits share one resolved Sleep target.
        void (WINAPI *volatile sleep)(DWORD)=Sleep;
        while(!available) { sleep(0);available=g_pGame->IsModelAvailable(0x21a); }
        while(!g_pGame->IsModelAvailable(0x23a))sleep(0);
        while(!g_pGame->IsModelAvailable(0x219))sleep(0);
        while(!g_pGame->IsModelAvailable(0x239))sleep(0);
        while(!g_pGame->IsModelAvailable(0x1c1))sleep(0);
        Scripting::ProcessCommand(&g_vehicleCreateTrainCommand,trainType,(double)x,(double)y,(double)z,direction,&handle);
        m_pGameVehicle=GamePoolVehicleGetAt(handle);
        m_pGameEntity=m_pGameVehicle;
        m_nHandle=handle;
        g_vehicleLastCreatedHandle=handle;
        g_vehicleTrainCursor=m_pGameVehicle;
        GameRemoveDriverByState598(m_pGameVehicle);
    }
initialized:
    *(DWORD*)((BYTE*)this+0x58)=0;
    *((BYTE*)this+0x64)=0;
    *(DWORD*)((BYTE*)this+0x65)=0;
    *(DWORD*)((BYTE*)this+0xb4)=0;
    *(DWORD*)((BYTE*)this+0x6d)=0;
    *(DWORD*)((BYTE*)this+0x69)=GetTickCount();
    *(DWORD*)((BYTE*)this+0x50)=0xffffffff;
    *(DWORD*)((BYTE*)this+0x54)=0xffffffff;
    *(DWORD*)((BYTE*)this+0x60)=0;
    m_bColorChanged=FALSE;
    m_bInModShop=FALSE;
    *(DWORD*)((BYTE*)this+0x75)=0;
    *(DWORD*)((BYTE*)this+0x8f)=0;
    memset((BYTE*)this+0x93,0,33);
}
