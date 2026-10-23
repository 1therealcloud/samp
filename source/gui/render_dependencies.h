#ifndef SAMP_RENDER_DEPENDENCIES_H
#define SAMP_RENDER_DEPENDENCIES_H
#include <windows.h>
#include <unknwn.h>
class CDXUTDialog;
struct IDirect3DDevice9;
#include "net/netgame.h"
#include "audiostream.h"
#include "chatbubble.h"
#include "net/labelpool.h"
#include "dialog.h"
#include "objectedit.h"
#include "objectselection.h"
#include "textdrawselection.h"
#include "deathwindow.h"
#include "renderpanel.h"
#include "downloadwindow.h"
#include "netstats.h"
#include "svrnetstats.h"
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
#include "game/debug.h"
#endif
