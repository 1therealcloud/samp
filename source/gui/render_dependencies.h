#ifndef SAMP_RENDER_DEPENDENCIES_H
#define SAMP_RENDER_DEPENDENCIES_H
#include <windows.h>
#include <unknwn.h>
// Address interfaces only; never allocate these partial classes.
class CNetGame {
public:
    // samp.dll RVA: 0x0000b290
    void Process();
    // Native destructor entry. The full networking lifecycle remains open.
    // samp.dll RVA: 0x00009570
    void Destroy();
};
class CAudioStream {
public:
    // samp.dll RVA: 0x00066530
    void DrawInfo();
    // samp.dll RVA: 0x00066500
    void ControlGameRadio();
};
class CChatBubble {public:
    // samp.dll RVA: 0x00066950
    void Draw();
};
class CLabelPool {public:
    // samp.dll RVA: 0x00001350
    void Draw();
};
class CDialog {public:
    // samp.dll RVA: 0x0006f2d0
    void Draw();
};
class CObjectEdit {public:
    // samp.dll RVA: 0x00073220
    void Render();
};
class CObjectSelection {public:
    // samp.dll RVA: 0x0006d640
    void Draw();
};
class CTextDrawSelection {public:
    // samp.dll RVA: 0x00070eb0
    void Process();
};
class CDeathWindow {public:
    // samp.dll RVA: 0x00069d20
    void Draw();
};
class CRenderPanel {public:
    // samp.dll RVA: 0x00071560
    void Draw();
};
class CDownloadWindow {public:
    // samp.dll RVA: 0x0006a350
    void Draw();
};
class CNetStats {public:
    // samp.dll RVA: 0x000607f0
    void Draw();
};
class CSrvNetStats {public:
    // samp.dll RVA: 0x00070d00
    void Draw();
};
class CHelpDialog {public:
    // samp.dll RVA: 0x0006b570
    void Show();
};
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
