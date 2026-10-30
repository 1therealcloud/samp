#ifndef SAMP_GUI_H
#define SAMP_GUI_H
#include "audiostream.h"
#include "chatbubble.h"
#include "deathwindow.h"
#include "dialog.h"
#include "downloadwindow.h"
#include "fontrender.h"
#include "game/debug.h"
#include "helpdialog.h"
#include "label.h"
#include "licenseplate.h"
#include "net/labelpool.h"
#include "net/netgame.h"
#include "net/objectpool.h"
#include "net/textdrawpool.h"
#include "netstats.h"
#include "objectedit.h"
#include "objectmaterialtext.h"
#include "objectselection.h"
#include "playertags.h"
#include "renderpanel.h"
#include "scoreboard.h"
#include "spawnscreen.h"
#include "svrnetstats.h"
#include "textdrawselection.h"
#include <d3dx9.h>
#include <unknwn.h>
#include <windows.h>
class CDXUTDialog;
class CDXUTControl;
// 0x002acab8
extern CDXUTDialog* g_guiScoreboardDialog;
// 0x002acac0
extern CDXUTDialog* g_guiServerDialog;
// 0x002acac4
extern CDXUTDialog* g_guiPanelDialog;
// 0x002acac8
extern CDXUTDialog* g_guiDownloadDialog;
// 0x000c6af0
void ClientRefreshGuiFonts();
// 0x000c6ce0
void ClientInitializeGui();
// 0x000c6c30
void CALLBACK ClientGuiScoreboardEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c6bf0
void CALLBACK ClientGuiInputEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c6c60
void CALLBACK ClientGuiSpawnEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c6b60
void CALLBACK ClientGuiDialogEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c6cc0
void CALLBACK ClientGuiPanelEvent(UINT event, int id, CDXUTControl* control, void* context);
// 0x000c6cd0
void CALLBACK ClientGuiDownloadEvent(UINT event, int id, CDXUTControl* control, void* context);

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

class CDXUTDialog;
struct IDirect3DDevice9;
class CDXUTDialog;
// 0x002aca1c
extern CAudioStream* g_renderAudioStream;
// 0x002aca08
extern CChatBubble* g_renderChatBubble;
// 0x002ac9e0
extern CDialog* g_renderDialog;
// 0x002ac9e4
extern CRenderPanel* g_renderPanel;
// 0x002ac9e8
extern CDownloadWindow* g_renderDownloadWindow;
// 0x002ac9f4
extern CObjectSelection* g_renderObjectSelection;
// 0x002ac9f8
extern CTextDrawSelection* g_renderTextDrawSelection;
// 0x002ac9fc
extern CNetStats* g_renderNetStats;
// 0x002aca00
extern CSrvNetStats* g_renderServerNetStats;
// 0x002aca04
extern CHelpDialog* g_renderHelp;
// 0x002acab4
extern CDXUTDialog* g_renderFirstDialog;
// 0x002acabc
extern CDXUTDialog* g_renderSecondDialog;
// 0x0016be7c
extern DWORD g_renderScreenshotRequested;
// 0x002aca48
extern BYTE g_renderShowVehicleInfo;
// 0x0017ebe0
extern BYTE g_renderMenuWasVisible;
// 0x0018e538
extern IUnknown* g_gameLastReleasedResource;
// 0x0018dde8
extern DWORD g_gameDebugStage;
typedef float(__cdecl* GroundHeightFunction)(float x, float y);
// 0x00140bdc
extern GroundHeightFunction g_renderGroundHeight;
// 0x0011bbd4
extern int g_renderMapClickRpc;
// 0x00073f10
void ClientProcessMenuTransition();
// 0x000a0280
void GameReleaseRenderResource();
// 0x0009e2d0
void GameDebugDraw();
// Later render/debug dependencies, not production stand-ins.
// 0x00075040
void ClientCaptureScreenshot();
// 0x00073cb0
void ClientDrawPlayerTags();
// 0x00074dc0
void ClientDrawLabels();

// 0x0018e1d0
extern HWND g_gameInputCooperativeWindow;
// 0x000a02a0
void GameReacquireInputDevice();

// 0x000c6a60
void ClientInitializeDialogResources();
#endif
