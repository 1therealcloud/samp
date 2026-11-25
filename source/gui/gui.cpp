#include "gui/gui.h"

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/game.h"
#include "game/hooks.h"
#include "game/playerped.h"
#include "game/scripting.h"
#include "gui/chatwindow.h"
#include "gui/cmdwindow.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/vehiclepool.h"

#include <DXUT.h>
#include <DXUTgui.h>
#include <GetTime.h>

#include <string.h>

CDXUTDialog* g_guiScoreboardDialog;

CDXUTDialog* g_guiServerDialog;

CDXUTDialog* g_guiPanelDialog;

CDXUTDialog* g_guiDownloadDialog;
static_assert(sizeof(CDXUTDialog) == 0x1ea);

CPlayerTags* g_devicePlayerTags;

CSpawnScreen* g_deviceSpawnScreen;

CLabel* g_deviceLabel;

CObjectMaterialText* g_deviceMaterialText;

BYTE* g_deviceDeathWindow;

BYTE* g_deviceObjectEdit;

CScoreboard* g_deviceScoreboard;

CDXUTDialogResourceManager* g_deviceDialogResources;

IDirect3DSurface9* g_deviceCursorSurface;

DWORD g_deviceResetMarker;

BYTE g_renderMenuWasVisible;

IUnknown* g_gameLastReleasedResource;

DWORD g_gameDebugStage;

HWND g_gameInputCooperativeWindow;

void ClientInitializeGui()
{
    if (g_guiScoreboardDialog)
    {
        delete g_guiScoreboardDialog;
        g_guiScoreboardDialog = 0;
    }
    if (g_renderFirstDialog)
    {
        delete g_renderFirstDialog;
        g_renderFirstDialog = 0;
    }
    if (g_deviceCursorSurface)
    {
        g_deviceCursorSurface->Release();
        g_deviceCursorSurface = 0;
    }
    if (g_renderSecondDialog)
    {
        delete g_renderSecondDialog;
        g_renderSecondDialog = 0;
    }
    if (g_guiServerDialog)
    {
        delete g_guiServerDialog;
        g_guiServerDialog = 0;
    }
    if (g_guiPanelDialog)
    {
        delete g_guiPanelDialog;
        g_guiPanelDialog = 0;
    }
    if (g_guiDownloadDialog)
    {
        delete g_guiDownloadDialog;
        g_guiDownloadDialog = 0;
    }
    g_guiScoreboardDialog = new CDXUTDialog;
    g_guiScoreboardDialog->Init(g_deviceDialogResources);
    g_guiScoreboardDialog->SetCallback(ClientGuiScoreboardEvent, 0);
    g_guiScoreboardDialog->SetSize(640, 480);
    g_guiScoreboardDialog->SetLocation(0, 0);
    g_guiScoreboardDialog->SetBackgroundColors(0x960a0a0a, 0x960a0a0a, 0x960a0a0a, 0x960a0a0a);
    g_guiScoreboardDialog->EnableMouseInput(true);
    g_guiScoreboardDialog->EnableKeyboardInput(true);
    g_guiScoreboardDialog->SetVisible(false);
    if (g_deviceScoreboard)
        g_deviceScoreboard->ResetDialogControls(g_guiScoreboardDialog);
    g_renderFirstDialog = new CDXUTDialog;
    g_renderFirstDialog->Init(g_deviceDialogResources);
    g_renderFirstDialog->SetCallback(ClientGuiInputEvent, 0);
    g_renderFirstDialog->SetLocation(0, 0);
    g_renderFirstDialog->SetSize(*(int*)0xc17044, *(int*)0xc17048);
    g_renderFirstDialog->EnableMouseInput(true);
    g_renderFirstDialog->EnableKeyboardInput(true);
    g_renderSecondDialog = new CDXUTDialog;
    g_renderSecondDialog->Init(g_deviceDialogResources);
    g_renderSecondDialog->SetCallback(ClientGuiSpawnEvent, 0);
    g_renderSecondDialog->SetLocation(0, 0);
    g_renderSecondDialog->SetSize(310, 40);
    g_renderSecondDialog->SetBackgroundColors(0x960a0a0a, 0x960a0a0a, 0x960a0a0a, 0x960a0a0a);
    g_renderSecondDialog->EnableMouseInput(true);
    g_renderSecondDialog->EnableKeyboardInput(false);
    g_renderSecondDialog->SetVisible(false);
    ((CDXUTDialogDL*)g_renderSecondDialog)->AddButton(6, "<<", 10, 5, 90, 30, 0, false, 0);
    ((CDXUTDialogDL*)g_renderSecondDialog)->AddButton(5, ">>", 110, 5, 90, 30, 0, false, 0);
    ((CDXUTDialogDL*)g_renderSecondDialog)->AddButton(4, "Spawn", 210, 5, 90, 30, 0, false, 0);
    if (g_pChat)
        g_pChat->ResetDialogControls(g_renderFirstDialog);
    if (g_pInput)
        g_pInput->ResetDialogControls(g_renderFirstDialog);
    g_guiServerDialog = new CDXUTDialog;
    g_guiServerDialog->Init(g_deviceDialogResources);
    g_guiServerDialog->SetCallback(ClientGuiDialogEvent, 0);
    g_guiServerDialog->SetLocation(0, 0);
    g_guiServerDialog->SetSize(600, 300);
    g_guiServerDialog->EnableMouseInput(true);
    g_guiServerDialog->EnableKeyboardInput(true);
    g_guiServerDialog->SetBackgroundColors(0xdc050505, 0xdc050505, 0xdc050505, 0xdc050505);
    g_guiServerDialog->SetVisible(false);
    if (g_renderDialog)
        g_renderDialog->ResetDialogControls(g_guiServerDialog);
    g_guiPanelDialog = new CDXUTDialog;
    g_guiPanelDialog->Init(g_deviceDialogResources);
    g_guiPanelDialog->SetCallback(ClientGuiPanelEvent, 0);
    g_guiPanelDialog->SetLocation(0, 0);
    g_guiPanelDialog->SetSize(500, 160);
    g_guiPanelDialog->EnableMouseInput(true);
    g_guiPanelDialog->EnableKeyboardInput(true);
    g_guiPanelDialog->SetBackgroundColors(0xdc050505, 0xdc050505, 0xdc050505, 0xdc050505);
    g_guiPanelDialog->SetVisible(false);
    if (g_renderPanel)
        g_renderPanel->ResetDialogControls(g_guiPanelDialog);
    g_guiDownloadDialog = new CDXUTDialog;
    g_guiDownloadDialog->Init(g_deviceDialogResources);
    g_guiDownloadDialog->SetCallback(ClientGuiDownloadEvent, 0);
    g_guiDownloadDialog->SetLocation(0, 0);
    g_guiDownloadDialog->SetSize(500, 160);
    g_guiDownloadDialog->EnableMouseInput(true);
    g_guiDownloadDialog->EnableKeyboardInput(false);
    g_guiDownloadDialog->SetBackgroundColors(0xdc050505, 0xdc050505, 0xdc050505, 0xdc050505);
    g_guiDownloadDialog->SetVisible(false);
    if (g_renderDownloadWindow)
        g_renderDownloadWindow->ResetDialogControls(g_guiDownloadDialog);
    if (!g_deviceCursorSurface)
    {
        g_gameD3DDevice->CreateOffscreenPlainSurface(32, 32, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &g_deviceCursorSurface, 0);
        D3DXLoadSurfaceFromFileA(g_deviceCursorSurface, 0, 0, "mouse.png", 0, D3DX_FILTER_NONE, 0, 0);
        if (g_deviceCursorSurface)
            g_gameD3DDevice->SetCursorProperties(0, 0, g_deviceCursorSurface);
    }
}

// Constructor-only opaque storage is intentional: the full UI interfaces remain open.
static_assert(sizeof(CDeathWindow) == 0x157);
static_assert(sizeof(CScoreboard) == 0x44);
static_assert(sizeof(CDialog) == 0x29d);
static_assert(sizeof(CRenderPanel) == 0x28);
static_assert(sizeof(CDownloadWindow) == 0x260);
static_assert(sizeof(CNetStats) == 0x18);
static_assert(sizeof(CSrvNetStats) == 0x18);
static_assert(sizeof(CHelpDialog) == 0x4);
static_assert(sizeof(CChatBubble) == 0x44a20);

// Allocation extent and packed manager layout observed in the original constructor.
static_assert(sizeof(CDXUTDialogResourceManager) == 36);

void ClientInitializeDialogResources()
{
    g_deviceDialogResources = new CDXUTDialogResourceManager;
    g_deviceDialogResources->OnCreateDevice(g_gameD3DDevice);
    g_deviceDialogResources->OnResetDevice();
}

void ClientRefreshGuiFonts()
{
    if (g_guiScoreboardDialog)
        g_guiScoreboardDialog->InitFont();
    if (g_renderFirstDialog)
        g_renderFirstDialog->InitFont();
    if (g_renderSecondDialog)
        g_renderSecondDialog->InitFont();
    if (g_guiServerDialog)
        g_guiServerDialog->InitFont();
    if (g_guiPanelDialog)
        g_guiPanelDialog->InitFont();
    if (g_guiDownloadDialog)
        g_guiDownloadDialog->InitFont();
    if (g_renderDownloadWindow)
        g_renderDownloadWindow->ResetFonts();
}

void CALLBACK ClientGuiScoreboardEvent(UINT event, int id, CDXUTControl* control, void* context)
{
    if (g_guiScoreboardDialog && event == 0x701 && g_deviceScoreboard)
        g_deviceScoreboard->SendNotification();
}

void CALLBACK ClientGuiInputEvent(UINT event, int id, CDXUTControl* control, void* context)
{
    switch (id)
    {
        case 1:
            if (event == 0x601 && g_pInput && !g_pInput->IsActive())
                g_pInput->ProcessInput();
            break;
    }
}

void CALLBACK ClientGuiSpawnEvent(UINT event, int id, CDXUTControl* control, void* context)
{
    // Native resolves these pointers even for events that do not dispatch a command.
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    BYTE* players = *(BYTE**)(pools + 8);
    CLocalPlayer* local = *(CLocalPlayer**)(players + 0x1e);
    switch (id)
    {
        case 4:
            if (event == 0x101)
                local->RequestSpawn();
            break;
        case 5:
            if (event == 0x101)
                local->NextClass();
            break;
        case 6:
            if (event == 0x101)
                local->PrevClass();
            break;
    }
}

void CALLBACK ClientGuiDialogEvent(UINT event, int id, CDXUTControl* control, void* context)
{
    switch (id)
    {
        case 19:
            if (event == 0x601)
            {
                if (g_renderDialog && !g_pInput->IsActive())
                    g_renderDialog->Close(1);
                return;
            }
            break;
        case 20:
            if (event == 0x101)
            {
                if (g_renderDialog)
                    g_renderDialog->Close(1);
                return;
            }
            break;
        case 21:
            if (event == 0x101)
            {
                if (g_renderDialog)
                    g_renderDialog->Close(0);
                return;
            }
            break;
    }
    if (event == 0x701 && g_renderDialog)
        g_renderDialog->Close(1);
}

// Both empty callbacks are confirmed native RET 16 bodies.

void CALLBACK ClientGuiPanelEvent(UINT event, int id, CDXUTControl* control, void* context) {}

void CALLBACK ClientGuiDownloadEvent(UINT event, int id, CDXUTControl* control, void* context) {}

void ClientDeviceObjectsLost()
{
    if (g_devicePlayerTags)
        g_devicePlayerTags->OnLostDevice();
    if (g_deviceFonts)
        g_deviceFonts->OnLostDevice();
    if (g_deviceSpawnScreen)
        g_deviceSpawnScreen->OnLostDevice();
    if (g_deviceLabel)
        g_deviceLabel->OnLostDevice();
    if (g_devicePlateManager)
        ((CLicensePlate*)g_devicePlateManager)->OnLostDevice();
    if (g_deviceMaterialText)
        g_deviceMaterialText->OnLostDevice();
    if (g_pNetGame && *(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14))
        (*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14))->ShutdownMaterials();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x137))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x137))->OnLostDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x13b))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x13b))->OnLostDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x13f))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x13f))->OnLostDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x14f))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x14f))->OnLostDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x153))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x153))->OnLostDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x143))
        (*(ID3DXSprite**)(g_deviceDeathWindow + 0x143))->OnLostDevice();
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x10b))
        (*(ID3DXFont**)(g_deviceObjectEdit + 0x10b))->OnLostDevice();
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x10f))
        (*(ID3DXFont**)(g_deviceObjectEdit + 0x10f))->OnLostDevice();
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x107))
        (*(ID3DXLine**)(g_deviceObjectEdit + 0x107))->OnLostDevice();
    if (g_pChat)
        g_pChat->OnLostDevice();
    if (g_pChat && *(DWORD*)((BYTE*)g_pChat + 0x63a6))
        (*(ID3DXSprite**)((BYTE*)g_pChat + 0x63a6))->OnLostDevice();
    if (g_pChat && *(DWORD*)((BYTE*)g_pChat + 0x63aa))
        (*(ID3DXSprite**)((BYTE*)g_pChat + 0x63aa))->OnLostDevice();
    if (g_pInput && g_pInput->m_bEnabled)
        g_pInput->Close();
    if (g_deviceScoreboard && *(DWORD*)g_deviceScoreboard)
        g_deviceScoreboard->Close(true);
    if (g_deviceDialogResources)
        g_deviceDialogResources->OnLostDevice();
    if (g_deviceCursorSurface)
    {
        g_deviceCursorSurface->Release();
        g_deviceCursorSurface = 0;
    }
    g_deviceResetMarker = 0;
    g_hookKeepBuffer = 0;
    if (g_pNetGame)
    {
        BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
        if (pool)
        {
            *(DWORD*)(pool + 0x17894) = 0;
            (*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc))->ShutdownLicensePlates();
        }
        if (g_pNetGame && *(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x20))
            (*(CTextDrawPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x20))->OnLostDevice();
    }
}

void ClientDeviceObjectsReset()
{
    SetCursor(0);
    if (g_deviceDialogResources)
        g_deviceDialogResources->OnResetDevice();
    if (g_devicePlayerTags)
        g_devicePlayerTags->OnResetDevice();
    if (g_deviceFonts)
        g_deviceFonts->OnResetDevice();
    if (g_deviceSpawnScreen)
        g_deviceSpawnScreen->OnResetDevice();
    if (g_deviceLabel)
        g_deviceLabel->OnResetDevice();
    if (g_devicePlateManager)
        ((CLicensePlate*)g_devicePlateManager)->OnResetDevice();
    if (g_deviceMaterialText)
        g_deviceMaterialText->OnResetDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x137))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x137))->OnResetDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x13b))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x13b))->OnResetDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x13f))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x13f))->OnResetDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x14f))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x14f))->OnResetDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x153))
        (*(ID3DXFont**)(g_deviceDeathWindow + 0x153))->OnResetDevice();
    if (g_deviceDeathWindow && *(DWORD*)(g_deviceDeathWindow + 0x143))
        (*(ID3DXSprite**)(g_deviceDeathWindow + 0x143))->OnResetDevice();
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x10b))
        (*(ID3DXFont**)(g_deviceObjectEdit + 0x10b))->OnResetDevice();
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x10f))
        (*(ID3DXFont**)(g_deviceObjectEdit + 0x10f))->OnResetDevice();
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x107))
        (*(ID3DXLine**)(g_deviceObjectEdit + 0x107))->OnResetDevice();
    if (g_pChat)
        g_pChat->OnResetDevice();
    if (g_pChat && *(DWORD*)((BYTE*)g_pChat + 0x63a6))
        (*(ID3DXSprite**)((BYTE*)g_pChat + 0x63a6))->OnResetDevice();
    if (g_pChat && *(DWORD*)((BYTE*)g_pChat + 0x63aa))
        (*(ID3DXSprite**)((BYTE*)g_pChat + 0x63aa))->OnResetDevice();
    g_gameD3DDevice->CreateOffscreenPlainSurface(32, 32, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &g_deviceCursorSurface, 0);
    D3DXLoadSurfaceFromFileA(g_deviceCursorSurface, 0, 0, "mouse.png", 0, D3DX_FILTER_NONE, 0, 0);
    if (g_deviceCursorSurface)
        g_gameD3DDevice->SetCursorProperties(0, 0, g_deviceCursorSurface);
    if (g_deviceScoreboard)
        g_deviceScoreboard->Recalc();
    if (!g_pGame->IsMenuVisible())
    {
        g_pGame->SetCursorMode(2, FALSE);
        g_pGame->SetCursorMode(0, TRUE);
        g_pGame->ProcessInputEnabling();
    }
    g_hookKeepBuffer = 1;
    if (g_pNetGame)
    {
        BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
        if (pool)
            *(DWORD*)(pool + 0x17894) = 1;
    }
}

void GameReleaseRenderResource()
{
    IUnknown* resource = *(IUnknown**)0xc8cfa4;
    g_gameLastReleasedResource = resource;
    if (resource)
    {
        resource->Release();
        *(IUnknown**)0xc8cfa4 = 0;
    }
}

void ClientProcessMenuTransition()
{
    if (g_pGame)
    {
        if (g_pGame->IsMenuVisible() && !g_renderMenuWasVisible)
        {
            g_renderMenuWasVisible = 1;
            GameReleaseRenderResource();
            return;
        }
        if (!g_pGame->IsMenuVisible() && g_renderMenuWasVisible)
        {
            g_renderMenuWasVisible = 0;
            GameReleaseRenderResource();
        }
    }
}

void GameDebugDraw()
{
    DWORD stage = g_gameDebugStage;
    if (stage && stage == 10)
        GameDrawDebugEntities();
}

void GameReacquireInputDevice()
{
    // GTA SA virtual addresses: DirectInput device slot and main HWND.
    g_gameLastReleasedResource = *(IUnknown**)0xc8cfa4;
    g_gameInputCooperativeWindow = *(HWND*)0xc97c1c;
    if (g_gameLastReleasedResource && g_gameInputCooperativeWindow)
    {
        // Native COM slots20/34/1C: Unacquire, SetCooperativeLevel, Acquire.
        IUnknown* device = g_gameLastReleasedResource;
        ((HRESULT(WINAPI*)(IUnknown*))(*(DWORD**)device)[8])(device);
        device = g_gameLastReleasedResource;
        ((HRESULT(WINAPI*)(IUnknown*, HWND, DWORD))(*(DWORD**)device)[13])(device, g_gameInputCooperativeWindow, 6);
        device = g_gameLastReleasedResource;
        ((HRESULT(WINAPI*)(IUnknown*))(*(DWORD**)device)[7])(device);
    }
}
