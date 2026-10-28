#ifndef SAMP_DEVICE_LIFECYCLE_H
#define SAMP_DEVICE_LIFECYCLE_H
#include "gui/device_objects.h"
class CDXUTDialogResourceManager;
// 0x002ac9d8
extern CPlayerTags* g_devicePlayerTags;
// 0x002aca20
extern CSpawnScreen* g_deviceSpawnScreen;
// 0x002ac9ec
extern CLabel* g_deviceLabel;
// 0x002aca34
extern CObjectMaterialText* g_deviceMaterialText;
// These unresolved UI prefixes are used for the established resource slots only.
// 0x002aca18
extern BYTE* g_deviceDeathWindow;
// 0x002ac9f0
extern BYTE* g_deviceObjectEdit;
// 0x002ac9dc
extern CScoreboard* g_deviceScoreboard;
// 0x002acab0
extern CDXUTDialogResourceManager* g_deviceDialogResources;
// 0x002acacc
extern IDirect3DSurface9* g_deviceCursorSurface;
// 0x0016be80
extern DWORD g_deviceResetMarker;
// 0x000c5620
void ClientDeviceObjectsLost();
// 0x000c5880
void ClientDeviceObjectsReset();
#endif
