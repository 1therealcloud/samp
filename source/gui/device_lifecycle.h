#ifndef SAMP_DEVICE_LIFECYCLE_H
#define SAMP_DEVICE_LIFECYCLE_H
#include "gui/device_objects.h"
class CDXUTDialogResourceManager;
// samp.dll RVA: 0x002ac9d8
extern CPlayerTags* g_devicePlayerTags;
// samp.dll RVA: 0x002aca20
extern CSpawnScreen* g_deviceSpawnScreen;
// samp.dll RVA: 0x002ac9ec
extern CLabel* g_deviceLabel;
// samp.dll RVA: 0x002aca34
extern CObjectMaterialText* g_deviceMaterialText;
// These unresolved UI prefixes are used for the established resource slots only.
// samp.dll RVA: 0x002aca18
extern BYTE* g_deviceDeathWindow;
// samp.dll RVA: 0x002ac9f0
extern BYTE* g_deviceObjectEdit;
// samp.dll RVA: 0x002ac9dc
extern CScoreboard* g_deviceScoreboard;
// samp.dll RVA: 0x002acab0
extern CDXUTDialogResourceManager* g_deviceDialogResources;
// samp.dll RVA: 0x002acacc
extern IDirect3DSurface9* g_deviceCursorSurface;
// samp.dll RVA: 0x0016be80
extern DWORD g_deviceResetMarker;
// samp.dll RVA: 0x000c5620
void ClientDeviceObjectsLost();
// samp.dll RVA: 0x000c5880
void ClientDeviceObjectsReset();
#endif
