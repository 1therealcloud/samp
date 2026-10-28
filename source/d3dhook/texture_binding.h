#ifndef SAMP_DEVICE_TEXTURE_BINDING_H
#define SAMP_DEVICE_TEXTURE_BINDING_H
#include <windows.h>
// samp.dll RVA: 0x001e0490
extern BOOL g_deviceMaterialOverrideEnabled;
// samp.dll RVA: 0x001e0494
extern BYTE* g_deviceMaterialOverrides;
// samp.dll RVA: 0x0017ebf0
extern DWORD g_deviceMaterialIndex;
// samp.dll RVA: 0x0018f564
extern void* g_devicePlateTexture;
// samp.dll RVA: 0x002aca30
extern BYTE* g_devicePlateManager;
// samp.dll RVA: 0x000a41d0
void* GameGetVehiclePlateTexture();
// samp.dll RVA: 0x000c54f0
void ClientSetRenderState(DWORD state, DWORD value);
#endif
