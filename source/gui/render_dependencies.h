#ifndef SAMP_RENDER_DEPENDENCIES_H
#define SAMP_RENDER_DEPENDENCIES_H
#include <unknwn.h>
#include <windows.h>
class CDXUTDialog;
struct IDirect3DDevice9;
#include "audiostream.h"
#include "chatbubble.h"
#include "deathwindow.h"
#include "dialog.h"
#include "downloadwindow.h"
#include "helpdialog.h"
#include "net/labelpool.h"
#include "net/netgame.h"
#include "netstats.h"
#include "objectedit.h"
#include "objectselection.h"
#include "renderpanel.h"
#include "svrnetstats.h"
#include "textdrawselection.h"
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
#include "game/debug.h"

// 0x0018e1d0
extern HWND g_gameInputCooperativeWindow;
// 0x000a02a0
void GameReacquireInputDevice();
#endif
