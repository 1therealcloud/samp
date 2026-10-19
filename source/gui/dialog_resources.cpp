#include "gui/dialog_resources.h"
#include "gui/device_lifecycle.h"
#include "game/input.h"
#include <DXUT.h>
#include <DXUTgui.h>
// Allocation extent and packed manager layout observed in the original constructor.
typedef char DialogManagerExtent[(sizeof(CDXUTDialogResourceManager)==36)?1:-1];
// samp.dll RVA: 0x000c6a60
void ClientInitializeDialogResources() {
    g_deviceDialogResources=new CDXUTDialogResourceManager;
    g_deviceDialogResources->OnCreateDevice(g_gameD3DDevice);
    g_deviceDialogResources->OnResetDevice();
}
