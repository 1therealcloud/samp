#include "net/pool_frame.h"
#include "net/vehiclepool.h"
#include "game/object.h"
#include "gui/device_objects.h"
#include "gui/device_lifecycle.h"
#include "game/textdraw.h"
#include "d3dhook/texture_binding.h"
#include <GetTime.h>

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
