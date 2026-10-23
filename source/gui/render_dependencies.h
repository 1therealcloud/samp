#ifndef SAMP_RENDER_DEPENDENCIES_H
#define SAMP_RENDER_DEPENDENCIES_H
#include <windows.h>
#include <unknwn.h>
class CDXUTDialog;
// samp.dll RVA: 0x0016c7f8
extern DWORD g_srvNetStatsHistory[74];
struct IDirect3DDevice9;
// The network constructor extent is known; its internal layout and lifecycle remain open.
class CVehiclePool;
class CPlayerPool;
class CNetGame {
public:
    // Native coordinator allocation extent; internal layout and lifecycle remain open.
    BYTE nativeState[0x3e2];
    // samp.dll RVA: 0x0000b610
    CNetGame(const char *host,int port,const char *nickname,const char *password);
    // Address-only command dependency.
    // samp.dll RVA: 0x00008b80
    void ToggleNameTagStatus();
    // samp.dll RVA: 0x0000b290
    void Process();
    // Native destructor entry. The full networking lifecycle remains open.
    // samp.dll RVA: 0x00009570
    void Destroy();
    // samp.dll RVA: 0x00001180
    CVehiclePool *GetVehiclePool();
    // samp.dll RVA: 0x00001170
    CPlayerPool *GetPlayerPool();
};
class CAudioStream {
public:
    BYTE initialized;
    // samp.dll RVA: 0x00065f00
    BOOL Reset();
    // samp.dll RVA: 0x00066530
    void DrawInfo();
    // samp.dll RVA: 0x00066500
    void ControlGameRadio();
};
class CChatBubble {public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x44a20];
    // samp.dll RVA: 0x00066860
    CChatBubble();
    // samp.dll RVA: 0x00066950
    void Draw();
};
class CLabelPool {public:
    // samp.dll RVA: 0x00001350
    void Draw();
};
#include "dialog.h"
#include "objectedit.h"
#include "objectselection.h"
#include "textdrawselection.h"
class CDeathWindow {public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x157];
    // samp.dll RVA: 0x0006a090
    CDeathWindow(IDirect3DDevice9 *device);
    // Font/resource reset dependency; body remains open.
    // samp.dll RVA: 0x00069b90
    void ResetFonts();
    // samp.dll RVA: 0x00069d20
    void Draw();
};
#include "renderpanel.h"
class CDownloadWindow {public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x260];
    // samp.dll RVA: 0x0006aa10
    CDownloadWindow(IDirect3DDevice9 *device);
    // Address-only font-size command dependency.
    // samp.dll RVA: 0x0006a1a0
    void ResetFonts();
    // samp.dll RVA: 0x0006a500
    void ResetDialogControls(CDXUTDialog *dialog);
    // samp.dll RVA: 0x0006a350
    void Draw();
};
class CNetStats {public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x18];
    // samp.dll RVA: 0x000607c0
    CNetStats(IDirect3DDevice9 *device);
    // samp.dll RVA: 0x000607f0
    void Draw();
};
class CSrvNetStats {public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x18];
    // samp.dll RVA: 0x00070cc0
    CSrvNetStats(IDirect3DDevice9 *device);
    // samp.dll RVA: 0x00070d00
    void Draw();
};
#include "helpdialog.h"
class CDXUTDialog;
// samp.dll RVA: 0x002aca1c
extern CAudioStream *g_renderAudioStream;
// samp.dll RVA: 0x002aca08
extern CChatBubble *g_renderChatBubble;
// samp.dll RVA: 0x002ac9e0
extern CDialog *g_renderDialog;
// samp.dll RVA: 0x002ac9e4
extern CRenderPanel *g_renderPanel;
// samp.dll RVA: 0x002ac9e8
extern CDownloadWindow *g_renderDownloadWindow;
// samp.dll RVA: 0x002ac9f4
extern CObjectSelection *g_renderObjectSelection;
// samp.dll RVA: 0x002ac9f8
extern CTextDrawSelection *g_renderTextDrawSelection;
// samp.dll RVA: 0x002ac9fc
extern CNetStats *g_renderNetStats;
// samp.dll RVA: 0x002aca00
extern CSrvNetStats *g_renderServerNetStats;
// samp.dll RVA: 0x002aca04
extern CHelpDialog *g_renderHelp;
// samp.dll RVA: 0x002acab4
extern CDXUTDialog *g_renderFirstDialog;
// samp.dll RVA: 0x002acabc
extern CDXUTDialog *g_renderSecondDialog;
// samp.dll RVA: 0x0016be7c
extern DWORD g_renderScreenshotRequested;
// samp.dll RVA: 0x002aca48
extern BYTE g_renderShowVehicleInfo;
// samp.dll RVA: 0x0017ebe0
extern BYTE g_renderMenuWasVisible;
// samp.dll RVA: 0x0018e538
extern IUnknown *g_gameLastReleasedResource;
// samp.dll RVA: 0x0018dde8
extern DWORD g_gameDebugStage;
typedef float (__cdecl *GroundHeightFunction)(float x,float y);
// samp.dll RVA: 0x00140bdc
extern GroundHeightFunction g_renderGroundHeight;
// samp.dll RVA: 0x0011bbd4
extern int g_renderMapClickRpc;
// samp.dll RVA: 0x00073f10
void ClientProcessMenuTransition();
// samp.dll RVA: 0x000a0280
void GameReleaseRenderResource();
// samp.dll RVA: 0x0009e2d0
void GameDebugDraw();
// Later render/debug dependencies, not production stand-ins.
// samp.dll RVA: 0x00075040
void ClientCaptureScreenshot();
// samp.dll RVA: 0x00073cb0
void ClientDrawPlayerTags();
// samp.dll RVA: 0x00074dc0
void ClientDrawLabels();
// samp.dll RVA: 0x0009df50
void GameDrawDebugEntities();
#endif
