#include "d3dhook/texture_binding.h"
#include "game/hooks_independent.h"
// samp.dll RVA: 0x001e0490
BOOL g_deviceMaterialOverrideEnabled;
// samp.dll RVA: 0x001e0494
BYTE *g_deviceMaterialOverrides;
// samp.dll RVA: 0x0017ebf0
DWORD g_deviceMaterialIndex;
// samp.dll RVA: 0x0018f564
void *g_devicePlateTexture;
// samp.dll RVA: 0x002aca30
BYTE *g_devicePlateManager;
// samp.dll RVA: 0x000a41d0
void *GameGetVehiclePlateTexture() {
    if(g_hookCachedBuffer) {
        g_devicePlateTexture=*(void**)((BYTE*)g_hookCachedBuffer+0x34);
        return *(void**)((BYTE*)g_hookCachedBuffer+0x34);
    }
    return 0;
}
// samp.dll RVA: 0x000c54f0
void ClientSetRenderState(DWORD state,DWORD value) {
    // GTA SA virtual address: RenderWare engine pointer, outside samp.dll.
    ((void (__cdecl*)(DWORD,DWORD))(*(DWORD**)(*(DWORD*)0xc97b24+0x20)))(state,value);
}
