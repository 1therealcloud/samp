#ifndef GUI_H
#define GUI_H

#include "audiostream.h"
#include "game/debug.h"
#include "gui/chatbubble.h"
#include "gui/deathwindow.h"
#include "gui/dialog.h"
#include "gui/downloadwindow.h"
#include "gui/helpdialog.h"
#include "gui/label.h"
#include "gui/netstats.h"
#include "gui/objectedit.h"
#include "gui/objectselection.h"
#include "gui/playertags.h"
#include "gui/renderpanel.h"
#include "gui/scoreboard.h"
#include "gui/spawnscreen.h"
#include "gui/svrnetstats.h"
#include "gui/textdrawselection.h"
#include "net/labelpool.h"
#include "net/netgame.h"
#include "net/objectpool.h"
#include "net/textdrawpool.h"
#include "render/fontrender.h"
#include "render/licenseplate.h"
#include "render/objectmaterialtext.h"

#include <d3dx9.h>

#include <unknwn.h>
#include <windows.h>
class CDXUTDialog;
class CDXUTControl;
// R5 address to verify
extern CDXUTDialog* g_guiScoreboardDialog;
// R5 address to verify
extern CDXUTDialog* g_guiServerDialog;
// R5 address to verify
extern CDXUTDialog* g_guiPanelDialog;
// R5 address to verify
extern CDXUTDialog* g_guiDownloadDialog;
// R5 address to verify
void ClientRefreshGuiFonts();
// 0x000c5620
void ClientInitializeGui();
// 0x000c5570
void CALLBACK ClientGuiScoreboardEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c5530
void CALLBACK ClientGuiInputEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c55a0
void CALLBACK ClientGuiSpawnEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c54a0
void CALLBACK ClientGuiDialogEvent(UINT event, int id, CDXUTControl* control, void* context);
// R5 address to verify
void CALLBACK ClientGuiPanelEvent(UINT event, int id, CDXUTControl* control, void* context);
// R5 address to verify
void CALLBACK ClientGuiDownloadEvent(UINT event, int id, CDXUTControl* control, void* context);

class CDXUTDialogResourceManager;
// R5 address to verify
extern CPlayerTags* g_devicePlayerTags;
// R5 address to verify
extern CSpawnScreen* g_deviceSpawnScreen;
// 0x0026eb5c
extern CLabel* g_deviceLabel;
// R5 address to verify
extern CObjectMaterialText* g_deviceMaterialText;
// These unresolved UI prefixes are used for the established resource slots only.
// 0x0026eb88
extern BYTE* g_deviceDeathWindow;
// 0x0026eb60
extern BYTE* g_deviceObjectEdit;
// 0x0026eb4c
extern CScoreboard* g_deviceScoreboard;
// 0x0026ec20
extern CDXUTDialogResourceManager* g_deviceDialogResources;
// R5 address to verify
extern IDirect3DSurface9* g_deviceCursorSurface;
// R5 address to verify
extern DWORD g_deviceResetMarker;
// 0x000c3f50
void ClientDeviceObjectsLost();
// 0x000c41b0
void ClientDeviceObjectsReset();

class CDXUTDialog;
struct IDirect3DDevice9;
class CDXUTDialog;
// 0x0026eb8c
extern CAudioStream* g_renderAudioStream;
// R5 address to verify
extern CChatBubble* g_renderChatBubble;
// 0x0026eb50
extern CDialog* g_renderDialog;
// R5 address to verify
extern CRenderPanel* g_renderPanel;
// 0x0026eb58
extern CDownloadWindow* g_renderDownloadWindow;
// 0x0026eb64
extern CObjectSelection* g_renderObjectSelection;
// 0x0026eb68
extern CTextDrawSelection* g_renderTextDrawSelection;
// R5 address to verify
extern CNetStats* g_renderNetStats;
// R5 address to verify
extern CSrvNetStats* g_renderServerNetStats;
// R5 address to verify
extern CHelpDialog* g_renderHelp;
// R5 address to verify
extern CDXUTDialog* g_renderFirstDialog;
// R5 address to verify
extern CDXUTDialog* g_renderSecondDialog;
// 0x0012de64
extern DWORD g_renderScreenshotRequested;
// R5 address to verify
extern BYTE g_renderShowVehicleInfo;
// R5 address to verify
extern BYTE g_renderMenuWasVisible;
// R5 address to verify
extern IUnknown* g_gameLastReleasedResource;
// 0x0014ff50
extern DWORD g_gameDebugStage;
typedef float(__cdecl* GroundHeightFunction)(float x, float y);
// R5 address to verify
extern GroundHeightFunction g_renderGroundHeight;
// 0x000e9b9c
extern int g_renderMapClickRpc;
// 0x00074480
void ClientProcessMenuTransition();
// 0x000a0440
void GameReleaseRenderResource();
// 0x0009e490
void GameDebugDraw();
// 0x000755c0
void ClientCaptureScreenshot();
// 0x00074210
void ClientDrawPlayerTags();
// 0x00075330
void ClientDrawLabels();

// R5 address to verify
extern HWND g_gameInputCooperativeWindow;
// R5 address to verify
void GameReacquireInputDevice();

// 0x000c53a0
void ClientInitializeDialogResources();

#endif // GUI_H
