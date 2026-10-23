#include "main.h"
#include <string.h>
// Native BSS storage and command-line settings. DLL startup remains open.
// samp.dll RVA: 0x002aca3c
CGame *g_pGame;
// samp.dll RVA: 0x002aca14
CInput *g_pInput;
// samp.dll RVA: 0x002aca10
CChat *g_pChat;
// samp.dll RVA: 0x002aca24
void *g_pNetGame;
// samp.dll RVA: 0x002abe78
ClientSettings g_clientSettings;
// Native parsers have no destination bound; missing opening quotes also have
// no terminator guard. Retain that behavior instead of silently truncating.
// samp.dll RVA: 0x000c5590
void Settings::GetFromCommandLine(const char *line,char *output) {
    while(*line==' ')++line;
    while(*line && *line!=' ' && *line!='-' && *line!='/')*output++=*line++;
    *output=0;
}
// samp.dll RVA: 0x000c55e0
void Settings::GetFromQuotes(const char *line,char *output) {
    while(*line!='"')++line;
    ++line;while(*line && *line!='"')*output++=*line++;
    *output=0;
}
// The scanner keeps walking through argument values, including quoted paths.
// Later -c/-d occurrences overwrite earlier debug/online selection.
// samp.dll RVA: 0x000c5c90
void Settings::Initialize() {
    const char *line=GetCommandLineA();memset(&g_clientSettings,0,sizeof(g_clientSettings));
    while(*line) {
        if(*line=='-' || *line=='/') {
            ++line;
            switch(*line) {
            case 'd':g_clientSettings.debug=TRUE;g_clientSettings.online=FALSE;break;
            case 'c':g_clientSettings.online=TRUE;g_clientSettings.debug=FALSE;break;
            case 'h':GetFromCommandLine(++line,g_clientSettings.host);break;
            case 'p':GetFromCommandLine(++line,g_clientSettings.port);break;
            case 'n':GetFromCommandLine(++line,g_clientSettings.nickname);break;
            case 'z':GetFromCommandLine(++line,g_clientSettings.password);break;
            case 'l':GetFromQuotes(++line,g_clientSettings.debugScript);break;
            }
        }
        ++line;
    }
}

#include "archive/provider.h"
#include "filehooks.h"
#include "game/hooks.h"
#include <process.h>
// samp.dll RVA: 0x002ac9c8
HMODULE g_clientModule;
// samp.dll RVA: 0x002ac9d4
CArchiveFS *g_pArchive;
// samp.dll RVA: 0x000c6780
BOOL WINAPI ClientDllMain(HINSTANCE module,DWORD reason,LPVOID reserved) {
    if(reason==DLL_PROCESS_ATTACH) {
        g_clientModule=module;
        Settings::Initialize();
        if(g_clientSettings.debug || g_clientSettings.online) {
            SetUnhandledExceptionFilter(ClientExceptionFilter);
            g_gameLoopAddress=(DWORD)ClientGameLoop;
            char archivePath[MAX_PATH];
            GetModuleFileNameA(g_clientModule,archivePath,MAX_PATH);
            int end=strlen(archivePath);
            while(archivePath[end]!='\\')--end;
            memcpy(archivePath+end+1,"samp.saa",9);
            // Native computes archivePath but opens the relative name below.
            g_pArchive=new CArchiveFS;
            if(!g_pArchive->Load("samp.saa")) {
                __asm { int 3 }
            }
            AddFontResourceA("gtaweap3.ttf");AddFontResourceA("sampaux3.ttf");
            ArchiveInstallFileHooks();ArchiveInstallCursorHook();
            g_pGame=new CGame;
            _beginthread(ClientStartupThread,0,0);
        }
    } else if(reason==DLL_PROCESS_DETACH && (g_clientSettings.debug || g_clientSettings.online)) {
        ArchiveRemoveHooks();
    }
    return TRUE;
}
// samp.dll RVA: 0x000c6930
BOOL WINAPI ClientDllMainForward(HINSTANCE module,DWORD reason,LPVOID reserved) {
    return ClientDllMain(module,reason,reserved);
}

#include <sys/stat.h>
#include <stdio.h>
// samp.dll RVA: 0x002ac7a8
char g_clientDataDirectory[261];
// samp.dll RVA: 0x002ac8b0
char g_clientCacheDirectory[261];
// samp.dll RVA: 0x000c51a0
const char *ClientGetDataDirectory() {return g_clientDataDirectory;}
// samp.dll RVA: 0x000c51b0
const char *ClientGetCacheDirectory() {return g_clientCacheDirectory;}

// samp.dll RVA: 0x0011b928
const char g_screenshotFilenamePattern[]="\\screens\\sa-mp-%03i.png";
// samp.dll RVA: 0x0011c7dc
const char g_screenshotTakenMessage[]="Screenshot Taken - sa-mp-%03i.png";
// samp.dll RVA: 0x0011c7c0
const char g_screenshotFailureMessage[]="Unable to save screenshot.";

// samp.dll RVA: 0x00061cf0
int ClientFindScreenshotFilename(std::string *path) {
    path->assign(ClientGetDataDirectory());
    char candidate[260]={0};int index;
    for(index=0;index<1000;index++) {
        sprintf(candidate,(*path+g_screenshotFilenamePattern).c_str(),index);
        WIN32_FIND_DATAA data;HANDLE found=FindFirstFileA(candidate,&data);
        if(found==INVALID_HANDLE_VALUE)break;
        FindClose(found);
    }
    path->assign(candidate);
    // If all names exist, native reports1000 but retains the final999 filename.
    return index;
}

#include "chatwindow.h"
#include "gui/render_dependencies.h"
#include "game/gta_globals.h"
#include <d3dx9.h>
#ifdef GetSystemMetrics
#undef GetSystemMetrics
#endif
// samp.dll RVA: 0x00075040
void ClientCaptureScreenshot() {
    g_renderScreenshotRequested=0;
    std::string path;int index=ClientFindScreenshotFilename(&path);
    // Native captures this vtable slot before both metric calls, then reloads this.
    typedef HRESULT (WINAPI *CreateSurfaceFunction)(IDirect3DDevice9*,UINT,UINT,D3DFORMAT,D3DPOOL,IDirect3DSurface9**,HANDLE*);
    CreateSurfaceFunction create=(CreateSurfaceFunction)(*(void***)g_gameD3DDevice)[36];
    int height=GetSystemMetrics(SM_CYSCREEN),width=GetSystemMetrics(SM_CXSCREEN);
    IDirect3DSurface9 *surface;
    create(g_gameD3DDevice,width,height,D3DFMT_A8R8G8B8,D3DPOOL_SCRATCH,&surface,0);
    if(SUCCEEDED(g_gameD3DDevice->GetFrontBufferData(0,surface))) {
        POINT origin={0,0};ClientToScreen(g_inputGameWindow,&origin);
        RECT rect;GetClientRect(g_inputGameWindow,&rect);
        rect.left+=origin.x;rect.right+=origin.x;
        rect.top+=origin.y;rect.bottom+=origin.y;
        D3DXSaveSurfaceToFileA(path.c_str(),D3DXIFF_PNG,surface,0,&rect);
        g_pChat->AddInfoMessage(g_screenshotTakenMessage,index);
    } else g_pChat->AddDebugMessage(g_screenshotFailureMessage);
    // Original does not release surface and does not check create/save HRESULTs.
}
// samp.dll RVA: 0x000b5900
BOOL ClientPathExists(const char *path) {
    struct _stat info;return _stat(path,&info)==0;
}
// samp.dll RVA: 0x000c51c0
void ClientInitializeCacheDirectory() {
    memset(g_clientCacheDirectory,0,261);
    BYTE configured[261];memset(configured,0,sizeof(configured));
    DWORD length=260,type;HKEY key;
    sprintf(g_clientCacheDirectory,"%s\\cache",g_clientDataDirectory);
    if(RegOpenKeyExA(HKEY_CURRENT_USER,"Software\\SAMP",0,KEY_READ,&key)==ERROR_SUCCESS) {
        if(RegQueryValueExA(key,"model_cache",0,&type,configured,&length)==ERROR_SUCCESS)
            strncpy(g_clientCacheDirectory,(const char*)configured,261);
    }
    // Native does not close key and does not validate value type or copied termination.
    if(!ClientPathExists(g_clientCacheDirectory))CreateDirectoryA(g_clientCacheDirectory,0);
    char local[268];sprintf(local,"%s\\local",g_clientCacheDirectory);
    if(!ClientPathExists(local))CreateDirectoryA(local,0);
}
// samp.dll RVA: 0x000c5340
void ClientInitializePaths() {
    memset(g_clientDataDirectory,0,261);
    const char *documents=(const char*)0xc92368; // GTA SA virtual address.
    if(!strlen(documents)){GetCurrentDirectoryA(261,g_clientDataDirectory);return;}
    sprintf(g_clientDataDirectory,"%s\\SAMP",documents);
    if(!ClientPathExists(g_clientDataDirectory))CreateDirectoryA(g_clientDataDirectory,0);
    char screens[264];sprintf(screens,"%s\\screens",g_clientDataDirectory);
    if(!ClientPathExists(screens))CreateDirectoryA(screens,0);
    ClientInitializeCacheDirectory();
}

#include "game/game.h"
#include "config.h"
#include "chatwindow.h"
#include "cmdwindow.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "gui/dialog_resources.h"
#include "gui/gui.h"
#include "gui/font_wrapper.h"
#include "d3dhook/IDirect3DDevice9Hook.h"
#include "d3dhook/texture_binding.h"
#include "game/model_preview.h"
#include <mmsystem.h>
#include <stdlib.h>
// samp.dll RVA: 0x002ac498
char g_clientConfigFilename[520];
// samp.dll RVA: 0x002ac6a0
char g_clientChatFilename[264];
// samp.dll RVA: 0x002abe70
IDirect3DDevice9 *g_clientSavedGameDevice;
// samp.dll RVA: 0x002aca40
DWORD g_clientDeviceInitialized;
// samp.dll RVA: 0x002aca44
DWORD g_clientNetworkInitialized;
typedef char NativeNetworkAllocationExtent[(sizeof(CNetGame)==0x3e2)?1:-1];
typedef char NativeTextDrawSelectionExtent[(sizeof(CTextDrawSelection)==10)?1:-1];
// samp.dll RVA: 0x000c5e70
void ClientInitializeDeviceAndGui(){
    if(g_clientDeviceInitialized){
        if(!g_clientNetworkInitialized&&g_clientSettings.online){
            g_pNetGame=new CNetGame(g_clientSettings.host,atol(g_clientSettings.port),g_clientSettings.nickname,g_clientSettings.password);
            g_clientNetworkInitialized=1;
        }
        return;
    }
    ClientInitializePaths();
    sprintf(g_clientConfigFilename,"%s\\sa-mp.cfg",g_clientDataDirectory);
    sprintf(g_clientChatFilename,"%s\\chatlog.txt",g_clientDataDirectory);
    g_pConfig=new CConfig(g_clientConfigFilename);
    timeBeginPeriod(1);GameMarkStartupStage();ClientInstallWindowProcedure();
    g_clientSavedGameDevice=*(IDirect3DDevice9**)0xc97c28;
    g_gameD3DDevice=g_pGame->GetDevice();
    *(IDirect3DDevice9**)0xc97c28=new IDirect3DDevice9Hook;
    g_gameD3DDevice->ShowCursor(FALSE);
    g_deviceFonts=new CFonts(g_gameD3DDevice);
    g_pChat=new CChat(g_gameD3DDevice,g_deviceFonts,g_clientChatFilename);
    g_pInput=new CInput(g_gameD3DDevice);
    g_fontOriginalText=(char*)calloc(1,100001);g_fontStrippedText=(char*)calloc(1,100001);
    if(g_pConfig->GetIntValue("timestamp")){g_pChat->m_bTimestamps=true;g_pChat->m_bRedraw=TRUE;}
    if(g_pConfig->GetIntValue("pagesize"))g_pChat->SetPageSize(g_pConfig->GetIntValue("pagesize"));
    ClientInitializeDialogResources();
    if(g_clientSettings.online){
        g_deviceDeathWindow=(BYTE*)new CDeathWindow(g_gameD3DDevice);
        g_deviceSpawnScreen=new CSpawnScreen(g_gameD3DDevice);
        g_devicePlayerTags=new CPlayerTags(g_gameD3DDevice);
        g_deviceScoreboard=new CScoreboard(g_gameD3DDevice);
        g_renderDialog=new CDialog(g_gameD3DDevice);
        g_renderPanel=new CRenderPanel(g_gameD3DDevice);
        g_renderDownloadWindow=new CDownloadWindow(g_gameD3DDevice);
        g_renderNetStats=new CNetStats(g_gameD3DDevice);
        g_renderServerNetStats=new CSrvNetStats(g_gameD3DDevice);
        g_renderHelp=new CHelpDialog(g_gameD3DDevice);
        g_renderChatBubble=new CChatBubble;
    }
    g_deviceLabel=new CLabel(g_gameD3DDevice);
    g_deviceObjectEdit=(BYTE*)new CObjectEdit(g_gameD3DDevice);
    g_renderObjectSelection=new CObjectSelection;
    g_renderTextDrawSelection=(CTextDrawSelection*)operator new(sizeof(CTextDrawSelection));
    if(g_renderTextDrawSelection){g_renderTextDrawSelection->enabled=0;g_renderTextDrawSelection->unknown08=0xffff;g_renderTextDrawSelection->unknown04=0xffffffff;}
    g_devicePlateManager=(BYTE*)new CLicensePlate(g_gameD3DDevice);
    g_deviceMaterialText=new CObjectMaterialText(g_gameD3DDevice);
    g_modelPreview=new CModelPreview;
    ClientInitializeGui();Commands::Setup();
    g_clientPreviousRenderState=*(DWORD*)(*(BYTE**)0xc97b24+0x20);
    *(DWORD*)(*(BYTE**)0xc97b24+0x20)=(DWORD)ClientRenderStateHook;
    g_pGame->SetMaxStats();g_pGame->LetTimeGo(0);
    if(g_clientSettings.debug){
        CCamera *camera=g_pGame->m_pCamera;camera->Detach();camera->Restore();
        g_pGame->EnableHUD(TRUE);g_pGame->EnableClock(false);
        if(strlen(g_clientSettings.debugScript))DebugScript::Initialize(g_clientSettings.debugScript);
    }
    int limit=g_pConfig->GetIntValue("fpslimit");
    if(!limit){g_pConfig->WriteIntValue("fpslimit",48,FALSE);limit=48;}
    if(limit>=20&&limit<=90)g_pGame->SetFrameLimiter(limit);
    if(!g_pConfig->DoesExist("multicore"))g_pConfig->WriteIntValue("multicore",1,FALSE);
    if(!g_pConfig->GetIntValue("multicore"))SetProcessAffinityMask(GetCurrentProcess(),1);
    g_renderAudioStream=new CAudioStream;
    if(g_renderAudioStream)g_renderAudioStream->initialized=0;
    g_renderAudioStream->Reset();
    g_pGame->m_bHeadMove=g_pConfig->GetIntValue("disableheadmove")==0;
    g_clientDeviceInitialized=1;
}

#include "game/device_integrity.h"
#include "game/hooks_render.h"
#include "gui/device_objects.h"
// samp.dll RVA: 0x002aca60
DWORD g_clientIntegrityFailures;
// samp.dll RVA: 0x002aca64
DWORD g_clientIntegrityFailed;
// samp.dll RVA: 0x002aca68
DWORD g_clientSkipGameFrame;
// samp.dll RVA: 0x002ac9c0
DWORD g_clientQuitRequested;
// samp.dll RVA: 0x002ac9c4
DWORD g_clientQuitStarted;
// samp.dll RVA: 0x000c5c10
DWORD __stdcall GameIntegrityMismatch(DWORD code,DWORD a,DWORD b,DWORD c,DWORD d,DWORD e,DWORD f) {
    if(g_pNetGame&&g_pGame) {
        GameUnprotect((void*)0x541df5,5);*(DWORD*)0x541df5=0x90909090;*(BYTE*)0x541df9=0x90;
        GameDisableMouseCalls();GameClearMouseDelta();GameUpdateMouse();
        GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;
        ++g_clientIntegrityFailures;
    }
    return 69;
}
// samp.dll RVA: 0x000c6630
void ClientProcessFrame() {
    ClientInitializeDeviceAndGui();
    if(g_gameD3DDevice) {
        g_gameD3DDevice->SetRenderState(D3DRS_FOGENABLE,g_hookDisableFog==0);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_NONE);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
    }
    if(g_pNetGame)((CNetGame*)g_pNetGame)->Process();
    if(g_renderAudioStream)g_renderAudioStream->ControlGameRadio();
    if(g_pGame)g_pGame->m_pAudio->Process();
}
// samp.dll RVA: 0x000c66b0
__declspec(naked) void ClientGameLoop() {
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
}

#include "cmdprocs.h"
#include "RakClientInterface.h"
// samp.dll RVA: 0x000c5550
void ClientRequestQuit(){
    if(g_pNetGame&&*(DWORD*)((BYTE*)g_pNetGame+0x3cd)==5)
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Disconnect(500,0);
    g_clientQuitRequested=1;g_clientQuitStarted=GetTickCount();
}

// samp.dll RVA: 0x0016be78
WNDPROC g_clientPreviousWindowProcedure;
// samp.dll RVA: 0x002aca50
DWORD g_clientPreviousRenderState;
// samp.dll RVA: 0x002aca54
DWORD g_clientRenderCaller;
// samp.dll RVA: 0x002aca58
DWORD g_clientRenderState;
// samp.dll RVA: 0x002aca5c
DWORD g_clientRenderValue;
// samp.dll RVA: 0x00061970
BOOL ClientInstallWindowProcedure() {
    HWND window=g_inputGameWindow;
    if(!window)return FALSE;
    SetClassLongA(window,GCL_STYLE,GetClassLongA(window,GCL_STYLE)|CS_DBLCLKS);
    HWND current=g_inputGameWindow;
    if(current) {
        g_clientPreviousWindowProcedure=(WNDPROC)GetWindowLongA(current,GWL_WNDPROC);
        if(g_clientPreviousWindowProcedure!=ClientWindowProcedure)
            SetWindowLongA(current,GWL_WNDPROC,(LONG)ClientWindowProcedure);
    }
    SetWindowTextA(window,"GTA:SA:MP");IsWindowUnicode(window);return TRUE;
}
// samp.dll RVA: 0x000c5450
__declspec(naked) void ClientRenderStateHook() {
    __asm {
        mov eax,[esp]
        mov g_clientRenderCaller,eax
        mov eax,[esp+4]
        mov g_clientRenderState,eax
        mov eax,[esp+8]
        mov g_clientRenderValue,eax
        cmp g_clientRenderState,14
        jne forward
        cmp g_clientRenderValue,0
        je fog_off
        push 1
        call GameSetupD3DFog
        add esp,4
        mov g_hookFogEnabled,1
        jmp clear_fog
    fog_off:
        push 0
        call GameSetupD3DFog
        add esp,4
        mov g_hookFogEnabled,0
    clear_fog:
        mov byte ptr [esp+8],0
    forward:
        mov eax,g_clientPreviousRenderState
        jmp eax
    }
}

// samp.dll RVA: 0x00060de0
BOOL ClientHandleKeyRelease(unsigned int key) {
    switch(key) {
    case VK_RETURN:if(g_renderDialog&&*(DWORD*)(g_renderDialog->nativeState+0x28)){g_renderDialog->Close(1);return FALSE;}break;
    case VK_ESCAPE:if(g_pInput->m_bEnabled&&!g_pInput->IsActive()){g_pInput->Close();return FALSE;}break;
    case VK_PRIOR:if(g_pNetGame&&g_pChat&&!g_pInput->IsActive()){g_pChat->PageUp();return FALSE;}break;
    case VK_NEXT:if(g_pNetGame&&g_pChat&&!g_pInput->IsActive()){g_pChat->PageDown();return FALSE;}break;
    case VK_UP:if(g_pInput->m_bEnabled&&!g_pInput->IsActive()){g_pInput->RecallUp();return FALSE;}break;
    case VK_DOWN:if(g_pInput->m_bEnabled&&!g_pInput->IsActive())g_pInput->RecallDown();break;
    case VK_SUBTRACT:if(g_clientSettings.debug){Commands::SavePositionOnly("");return FALSE;}break;
    case VK_F6:if(!g_pInput->m_bEnabled)g_pInput->Open();else g_pInput->Close();return FALSE;
    case VK_F7:g_pChat->SwitchMode();return FALSE;
    // samp.dll RVA: 0x0016be7c
    case VK_F8:g_renderScreenshotRequested=1;return FALSE;
    case VK_F9:*(DWORD*)g_deviceDeathWindow=*(DWORD*)g_deviceDeathWindow==0;return FALSE;
    }
    return FALSE;
}

// samp.dll RVA: 0x0018dc38
DWORD g_clientStartupMarker;
// samp.dll RVA: 0x0009db90
void GameMarkStartupStage() { g_clientStartupMarker=1; }
// samp.dll RVA: 0x000c5150
void __cdecl ClientStartupThread(void *argument) {
    (void)argument;
    if(g_pGame)g_pGame->InitGame();
    while(*(volatile DWORD*)0xc8d4c0!=7)Sleep(5);
    GameMarkStartupStage();g_pGame->StartGame();ExitThread(0);
}
