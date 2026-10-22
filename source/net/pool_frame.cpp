#include "net/pool_frame.h"
#include "net/vehiclepool.h"
#include "game/object.h"
#include "gui/device_objects.h"
#include "gui/device_lifecycle.h"
#include "game/textdraw.h"
#include "d3dhook/texture_binding.h"
#include <GetTime.h>

// samp.dll RVA: 0x00156b74
DWORD g_objectPoolLastTick;
// samp.dll RVA: 0x00118130
const float g_objectPoolSecondsPerTick=0.001f;

// samp.dll RVA: 0x00012950
void CObjectPool::Process() {
    if(!g_objectPoolLastTick)g_objectPoolLastTick=RakNet::GetTime();
    DWORD now=RakNet::GetTime();LONG delta=(LONG)(now-g_objectPoolLastTick);float seconds;
    __asm {
        fild delta
        fmul g_objectPoolSecondsPerTick
        fstp seconds
    }
    BYTE *state=(BYTE*)this;
    if(*(LONG*)state>=0) {
        DWORD i=0;
        do {
            if(*(DWORD*)(state+4+i*4))(*(CObject**)(state+0x20d4+i*4))->Process(seconds);
        } while((LONG)++i<=*(LONG*)state);
    }
    g_objectPoolLastTick=now;
}
// samp.dll RVA: 0x00012a10
void CObjectPool::ConstructMaterials() {
    BYTE *state=(BYTE*)this;
    if(*(LONG*)state>=0) {
        DWORD i=0;
        do {
            if(*(DWORD*)(state+4+i*4)) {
                CObject *object=*(CObject**)(state+0x20d4+i*4);
                if(*(DWORD*)((BYTE*)object+0x118b))object->ConstructMaterialText();
            }
        } while((LONG)++i<=*(LONG*)state);
    }
}
// samp.dll RVA: 0x00012a50
void CObjectPool::ShutdownMaterials() {
    BYTE *state=(BYTE*)this;
    if(*(LONG*)state>=0) {
        DWORD i=0;
        do {
            if(*(DWORD*)(state+4+i*4)) {
                CObject *object=*(CObject**)(state+0x20d4+i*4);
                if(*(DWORD*)((BYTE*)object+0x118b))object->ShutdownMaterialText();
            }
        } while((LONG)++i<=*(LONG*)state);
    }
}
// samp.dll RVA: 0x0001e830
void CVehiclePool::ConstructLicensePlates() {
    BYTE *state=(BYTE*)this;
    if(!*(DWORD*)(state+0x17894))return;
    if(g_devicePlateManager && !((CLicensePlate*)g_devicePlateManager)->defaultTexture) {
        IDirect3DTexture9 *created=((CLicensePlate*)g_devicePlateManager)->Create("XYZSR998");
        ((CLicensePlate*)g_devicePlateManager)->defaultTexture=created;
    }
    if(m_nLargestId>=0) {
        DWORD counter=0;
        do {
            WORD slot=(WORD)counter;
            if(*(DWORD*)(state+0x3074+slot*4))(*(CVehicle**)(state+0x1134+slot*4))->ConstructLicensePlate();
        } while((int)(WORD)++counter<=m_nLargestId);
    }
}
// samp.dll RVA: 0x0001e8a0
void CVehiclePool::ShutdownLicensePlates() {
    BYTE *state=(BYTE*)this;
    if(m_nLargestId>=0) {
        DWORD counter=0;
        do {
            WORD slot=(WORD)counter;
            if(*(DWORD*)(state+0x3074+slot*4))(*(CVehicle**)(state+0x1134+slot*4))->ShutdownLicensePlate();
        } while((int)(WORD)++counter<=m_nLargestId);
    }
}
