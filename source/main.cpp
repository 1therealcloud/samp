#include <static_assert.h>
#include "main.h"
#include "RakClientInterface.h"
#include "anticheat.h"
#include "chatwindow.h"
#include "cmdprocs.h"
#include "cmdwindow.h"
#include "config.h"
#include "d3dhook/ID3DXFontHook.h"
#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/game.h"
#include "game/gta_globals.h"
#include "game/hooks.h"
#include "game/model_preview.h"
#include "gui.h"
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Client runtime globals, initialization and frame coordination.
// 0x002aca3c
CGame* g_pGame;
// 0x002aca14
CInput* g_pInput;
// 0x002aca10
CChat* g_pChat;
// 0x002aca24
void* g_pNetGame;

// 0x002abe70
IDirect3DDevice9* g_clientSavedGameDevice;
// 0x002aca40
DWORD g_clientDeviceInitialized;
// 0x002aca44
DWORD g_clientNetworkInitialized;
STATIC_ASSERT(sizeof(CNetGame) == 0x3e2);
STATIC_ASSERT(sizeof(CTextDrawSelection) == 10);
// 0x000c5e70
void ClientInitializeDeviceAndGui()
{
    if (g_clientDeviceInitialized)
    {
        if (!g_clientNetworkInitialized && g_clientSettings.online)
        {
            g_pNetGame = new CNetGame(g_clientSettings.host, atol(g_clientSettings.port), g_clientSettings.nickname, g_clientSettings.password);
            g_clientNetworkInitialized = 1;
        }
        return;
    }
    ClientInitializePaths();
    sprintf(g_clientConfigFilename, "%s\\sa-mp.cfg", g_clientDataDirectory);
    sprintf(g_clientChatFilename, "%s\\chatlog.txt", g_clientDataDirectory);
    g_pConfig = new CConfig(g_clientConfigFilename);
    timeBeginPeriod(1);
    GameMarkStartupStage();
    ClientInstallWindowProcedure();
    g_clientSavedGameDevice = GameGetCurrentD3DDevice();
    g_gameD3DDevice = g_pGame->GetDevice();
    *(IDirect3DDevice9**)0xc97c28 = new IDirect3DDevice9Hook;
    g_gameD3DDevice->ShowCursor(FALSE);
    g_deviceFonts = new CFonts(g_gameD3DDevice);
    g_pChat = new CChat(g_gameD3DDevice, g_deviceFonts, g_clientChatFilename);
    g_pInput = new CInput(g_gameD3DDevice);
    g_fontOriginalText = (char*)calloc(1, 100001);
    g_fontStrippedText = (char*)calloc(1, 100001);
    if (g_pConfig->GetIntValue("timestamp"))
    {
        g_pChat->m_bTimestamps = true;
        g_pChat->m_bRedraw = TRUE;
    }
    if (g_pConfig->GetIntValue("pagesize"))
        g_pChat->SetPageSize(g_pConfig->GetIntValue("pagesize"));
    ClientInitializeDialogResources();
    if (g_clientSettings.online)
    {
        g_deviceDeathWindow = (BYTE*)new CDeathWindow(g_gameD3DDevice);
        g_deviceSpawnScreen = new CSpawnScreen(g_gameD3DDevice);
        g_devicePlayerTags = new CPlayerTags(g_gameD3DDevice);
        g_deviceScoreboard = new CScoreboard(g_gameD3DDevice);
        g_renderDialog = new CDialog(g_gameD3DDevice);
        g_renderPanel = new CRenderPanel(g_gameD3DDevice);
        g_renderDownloadWindow = new CDownloadWindow(g_gameD3DDevice);
        g_renderNetStats = new CNetStats(g_gameD3DDevice);
        g_renderServerNetStats = new CSrvNetStats(g_gameD3DDevice);
        g_renderHelp = new CHelpDialog(g_gameD3DDevice);
        g_renderChatBubble = new CChatBubble;
    }
    g_deviceLabel = new CLabel(g_gameD3DDevice);
    g_deviceObjectEdit = (BYTE*)new CObjectEdit(g_gameD3DDevice);
    g_renderObjectSelection = new CObjectSelection;
    g_renderTextDrawSelection = new CTextDrawSelection;
    g_devicePlateManager = (BYTE*)new CLicensePlate(g_gameD3DDevice);
    g_deviceMaterialText = new CObjectMaterialText(g_gameD3DDevice);
    g_modelPreview = new CModelPreview;
    ClientInitializeGui();
    Commands::Setup();
    ClientInstallRenderStateHook();
    g_pGame->SetMaxStats();
    g_pGame->LetTimeGo(0);
    if (g_clientSettings.debug)
    {
        CCamera* camera = g_pGame->m_pCamera;
        camera->Detach();
        camera->Restore();
        g_pGame->EnableHUD(TRUE);
        g_pGame->EnableClock(false);
        if (strlen(g_clientSettings.debugScript))
            DebugScript::Initialize(g_clientSettings.debugScript);
    }
    int limit = g_pConfig->GetIntValue("fpslimit");
    if (!limit)
    {
        g_pConfig->WriteIntValue("fpslimit", 48, FALSE);
        limit = 48;
    }
    if (limit >= 20 && limit <= 90)
        g_pGame->SetFrameLimiter(limit);
    if (!g_pConfig->DoesExist("multicore"))
        g_pConfig->WriteIntValue("multicore", 1, FALSE);
    if (!g_pConfig->GetIntValue("multicore"))
        SetProcessAffinityMask(GetCurrentProcess(), 1);
    g_renderAudioStream = new CAudioStream;
    if (g_renderAudioStream)
        g_renderAudioStream->initialized = 0;
    g_renderAudioStream->Reset();
    g_pGame->m_bHeadMove = g_pConfig->GetIntValue("disableheadmove") == 0;
    g_clientDeviceInitialized = 1;
}

// 0x002aca60
DWORD g_clientIntegrityFailures;
// 0x002aca64
DWORD g_clientIntegrityFailed;
// 0x002aca68
DWORD g_clientSkipGameFrame;
// 0x002ac9c0
DWORD g_clientQuitRequested;
// 0x002ac9c4
DWORD g_clientQuitStarted;
// 0x000c6630
void ClientProcessFrame()
{
    ClientInitializeDeviceAndGui();
    if (g_gameD3DDevice)
    {
        g_gameD3DDevice->SetRenderState(D3DRS_FOGENABLE, g_hookDisableFog == 0);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    }
    if (g_pNetGame)
        ((CNetGame*)g_pNetGame)->Process();
    if (g_renderAudioStream)
        g_renderAudioStream->ControlGameRadio();
    if (g_pGame)
        g_pGame->m_pAudio->Process();
}
// 0x000c66b0
__declspec(naked) void ClientGameLoop()
{
    // clang-format off
    __asm {
        push ebx
        push esi
        push edi
        pushad
        call ClientProcessFrame
        popad
        mov eax,g_clientSkipGameFrame
        xor edi,edi
        cmp eax,edi
        jne clear_skip
        mov edx,0x53e230
        call edx
        cmp g_clientSkipGameFrame,edi
        je after_game
    clear_skip:
        mov g_clientSkipGameFrame,edi
    after_game:
        pushad
        mov ecx,g_pGame
        cmp ecx,edi
        je after_text
        call CGame::IsMenuVisible
        test eax,eax
        jne reload_game
        mov eax,g_pNetGame
        cmp eax,edi
        je reload_game
        mov eax,[eax+0x3de]
        mov ecx,[eax+0x20]
        cmp ecx,edi
        je reload_game
        call CTextDrawPool::Draw
    reload_game:
        mov ecx,g_pGame
    after_text:
        cmp g_clientQuitRequested,edi
        je continue_game
        call dword ptr [GetTickCount]
        sub eax,g_clientQuitStarted
        cmp eax,1000
        jbe finish
        mov ecx,g_pNetGame
        cmp ecx,edi
        je exit_game
        mov esi,ecx
        call CNetGame::Destroy
        push esi
        call free
        add esp,4
        mov g_pNetGame,edi
    exit_game:
        push edi
        call dword ptr [ExitProcess]
    finish:
        popad
        pop edi
        pop esi
        pop ebx
        ret
    continue_game:
        cmp ecx,edi
        je check_failures
        call CGame::ProcessInputEnabling
    check_failures:
        cmp g_clientIntegrityFailures,10
        jbe finish
        mov g_clientIntegrityFailed,1
        jmp finish
    }
    // clang-format on
}

// 0x000c5550
void ClientRequestQuit()
{
    if (g_pNetGame && *(DWORD*)((BYTE*)g_pNetGame + 0x3cd) == 5)
        (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->Disconnect(500, 0);
    g_clientQuitRequested = 1;
    g_clientQuitStarted = GetTickCount();
}
