#include "game/vehicle.h"
#include "gui/device_objects.h"
#include "d3dhook/texture_binding.h"

// samp.dll RVA: 0x000b7cd0
void CVehicle::ConstructLicensePlate() {
    BYTE *state=(BYTE*)this;
    if(!*(IDirect3DTexture9**)(state+0x8f)) {
        const char *text=(char*)state+0x93,*end=text;
        while(*end++){}
        if(end!=text+1) {
            IDirect3DTexture9 *created=((CLicensePlate*)g_devicePlateManager)->Create(text);
            *(IDirect3DTexture9**)(state+0x8f)=created;
        }
    }
}
// samp.dll RVA: 0x000b7d20
void CVehicle::ShutdownLicensePlate() {
    IDirect3DTexture9 **texture=(IDirect3DTexture9**)((BYTE*)this+0x8f);
    if(*texture){(*texture)->Release();*texture=0;}
}
