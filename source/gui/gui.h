#ifndef SAMP_GUI_NATIVE_H
#define SAMP_GUI_NATIVE_H
#include <windows.h>
class CDXUTDialog;
class CDXUTControl;
// samp.dll RVA: 0x002acab8
extern CDXUTDialog* g_guiScoreboardDialog;
// samp.dll RVA: 0x002acac0
extern CDXUTDialog* g_guiServerDialog;
// samp.dll RVA: 0x002acac4
extern CDXUTDialog* g_guiPanelDialog;
// samp.dll RVA: 0x002acac8
extern CDXUTDialog* g_guiDownloadDialog;
// samp.dll RVA: 0x000c6af0
void ClientRefreshGuiFonts();
// samp.dll RVA: 0x000c6ce0
void ClientInitializeGui();
// samp.dll RVA: 0x000c6c30
void CALLBACK ClientGuiScoreboardEvent(UINT event, int id, CDXUTControl* control, void* context);
// samp.dll RVA: 0x000c6bf0
void CALLBACK ClientGuiInputEvent(UINT event, int id, CDXUTControl* control, void* context);
// samp.dll RVA: 0x000c6c60
void CALLBACK ClientGuiSpawnEvent(UINT event, int id, CDXUTControl* control, void* context);
// samp.dll RVA: 0x000c6b60
void CALLBACK ClientGuiDialogEvent(UINT event, int id, CDXUTControl* control, void* context);
// samp.dll RVA: 0x000c6cc0
void CALLBACK ClientGuiPanelEvent(UINT event, int id, CDXUTControl* control, void* context);
// samp.dll RVA: 0x000c6cd0
void CALLBACK ClientGuiDownloadEvent(UINT event, int id, CDXUTControl* control, void* context);
#endif
