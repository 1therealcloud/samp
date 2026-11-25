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

extern CDXUTDialog* g_guiScoreboardDialog;

extern CDXUTDialog* g_guiServerDialog;

extern CDXUTDialog* g_guiPanelDialog;

extern CDXUTDialog* g_guiDownloadDialog;

void ClientRefreshGuiFonts();

void ClientInitializeGui();

void CALLBACK ClientGuiScoreboardEvent(UINT event, int id, CDXUTControl* control, void* context);

void CALLBACK ClientGuiInputEvent(UINT event, int id, CDXUTControl* control, void* context);

void CALLBACK ClientGuiSpawnEvent(UINT event, int id, CDXUTControl* control, void* context);

void CALLBACK ClientGuiDialogEvent(UINT event, int id, CDXUTControl* control, void* context);

void CALLBACK ClientGuiPanelEvent(UINT event, int id, CDXUTControl* control, void* context);

void CALLBACK ClientGuiDownloadEvent(UINT event, int id, CDXUTControl* control, void* context);

class CDXUTDialogResourceManager;

extern CPlayerTags* g_devicePlayerTags;

extern CSpawnScreen* g_deviceSpawnScreen;

extern CLabel* g_deviceLabel;

extern CObjectMaterialText* g_deviceMaterialText;
// These unresolved UI prefixes are used for the established resource slots only.

extern BYTE* g_deviceDeathWindow;

extern BYTE* g_deviceObjectEdit;

extern CScoreboard* g_deviceScoreboard;

extern CDXUTDialogResourceManager* g_deviceDialogResources;

extern IDirect3DSurface9* g_deviceCursorSurface;

extern DWORD g_deviceResetMarker;

void ClientDeviceObjectsLost();

void ClientDeviceObjectsReset();

class CDXUTDialog;
struct IDirect3DDevice9;
class CDXUTDialog;

extern CAudioStream* g_renderAudioStream;

extern CChatBubble* g_renderChatBubble;

extern CDialog* g_renderDialog;

extern CRenderPanel* g_renderPanel;

extern CDownloadWindow* g_renderDownloadWindow;

extern CObjectSelection* g_renderObjectSelection;

extern CTextDrawSelection* g_renderTextDrawSelection;

extern CNetStats* g_renderNetStats;

extern CSrvNetStats* g_renderServerNetStats;

extern CHelpDialog* g_renderHelp;

extern CDXUTDialog* g_renderFirstDialog;

extern CDXUTDialog* g_renderSecondDialog;

extern DWORD g_renderScreenshotRequested;

extern BYTE g_renderShowVehicleInfo;

extern BYTE g_renderMenuWasVisible;

extern IUnknown* g_gameLastReleasedResource;

extern DWORD g_gameDebugStage;
typedef float(__cdecl* GroundHeightFunction)(float x, float y);

extern GroundHeightFunction g_renderGroundHeight;

extern int g_renderMapClickRpc;

void ClientProcessMenuTransition();

void GameReleaseRenderResource();

void GameDebugDraw();

void ClientCaptureScreenshot();

void ClientDrawPlayerTags();

void ClientDrawLabels();

extern HWND g_gameInputCooperativeWindow;

void GameReacquireInputDevice();

void ClientInitializeDialogResources();

#endif // GUI_H
