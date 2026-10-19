#include "game/object.h"
#include "gui/device_lifecycle.h"

// samp.dll RVA: 0x000a8870
bool CObject::GetMaterialSize(int code,int *width,int *height) {
    switch(code) {
    case 10:*width=32;*height=32;return true;
    case 20:*width=64;*height=32;return true;
    case 30:*width=64;*height=64;return true;
    case 40:*width=128;*height=32;return true;
    case 50:*width=128;*height=64;return true;
    case 60:*width=128;*height=128;return true;
    case 70:*width=256;*height=32;return true;
    case 80:*width=256;*height=64;return true;
    case 90:*width=256;*height=128;return true;
    case 100:*width=256;*height=256;return true;
    case 110:*width=512;*height=64;return true;
    case 120:*width=512;*height=128;return true;
    case 130:*width=512;*height=256;return true;
    case 140:*width=512;*height=512;return true;
    default:return false;
    }
}
// samp.dll RVA: 0x000a9ad0
void CObject::ConstructMaterialText() {
    BYTE *state=(BYTE*)this;
    for(int i=0;i<16;i++) {
        BYTE *type=state+0x2db+i*4;
        BYTE *record=state+0x431+i*0xd7;
        if(*(DWORD*)(type-0xc4) && *(DWORD*)type==2 && !*(DWORD*)(type+0x40)) {
            int width,height;
            if(GetMaterialSize(record[-0x4c],&width,&height)) {
                IDirect3DTexture9 *created=g_deviceMaterialText->Create(*(char**)(type+0xdf0),(char*)record-0x4b,record[-10],width,height,*(DWORD*)(record-8),*(DWORD*)(record-4),record[-9],*record);
                *(IDirect3DTexture9**)(type+0xe70)=created;
                *(DWORD*)(type+0x40)=1;
            }
        }
    }
}
// samp.dll RVA: 0x000a8ac0
void CObject::ShutdownMaterialText() {
    BYTE *state=(BYTE*)this;
    for(int i=0;i<16;i++) {
        IDirect3DTexture9 **texture=(IDirect3DTexture9**)(state+0x114b+i*4);
        if(*texture) {
            (*texture)->Release();*texture=0;
            *(DWORD*)(state+0x31b+i*4)=0;
        }
    }
}
