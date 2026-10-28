#ifndef SAMP_RECOVERED_RUNTIME_GLOBALS_H
#define SAMP_RECOVERED_RUNTIME_GLOBALS_H
#include <string>
#include <windows.h>
class CInput;
class CChat;
#include "game/game.h"
// samp.dll RVA: 0x002aca3c
extern CGame* g_pGame;
// samp.dll RVA: 0x002aca14
extern CInput* g_pInput;
// samp.dll RVA: 0x002aca10
extern CChat* g_pChat;
// samp.dll RVA: 0x002aca24
extern void* g_pNetGame;
#pragma pack(push, 1)
struct ClientSettings
{
    BOOL debug, online;
    DWORD unknown08;
    char password[257], host[257], port[257], nickname[257], debugScript[257];
};
#pragma pack(pop)
typedef char ClientSettingsExtent[(sizeof(ClientSettings) == 1297) ? 1 : -1];
typedef char ClientSettingsHostOffset[(offsetof(ClientSettings, host) == 0x10d) ? 1 : -1];
typedef char ClientSettingsScriptOffset[(offsetof(ClientSettings, debugScript) == 0x410) ? 1 : -1];
// samp.dll RVA: 0x002abe78
extern ClientSettings g_clientSettings;
// Opaque native startup marker; callers and write are established, broader
// meaning is not assigned from the address alone.
// samp.dll RVA: 0x0018dc38
extern DWORD g_clientStartupMarker;
// samp.dll RVA: 0x0009db90
void GameMarkStartupStage();
// CRT _beginthread callback. Native termination is ExitThread, not _endthread.
// samp.dll RVA: 0x000c5150
void __cdecl ClientStartupThread(void* argument);
namespace Settings
{
// samp.dll RVA: 0x000c5c90
void Initialize();
// samp.dll RVA: 0x000c5590
void GetFromCommandLine(const char* line, char* output);
// samp.dll RVA: 0x000c55e0
void GetFromQuotes(const char* line, char* output);
} // namespace Settings
#include "game/gta_globals.h"
#endif

#ifndef SAMP_CLIENT_BOOTSTRAP_H
#define SAMP_CLIENT_BOOTSTRAP_H
// samp.dll RVA: 0x002ac9c8
extern HMODULE g_clientModule;
// samp.dll RVA: 0x000c6780
BOOL WINAPI ClientDllMain(HINSTANCE module, DWORD reason, LPVOID reserved);
// samp.dll RVA: 0x000c6930
BOOL WINAPI ClientDllMainForward(HINSTANCE module, DWORD reason, LPVOID reserved);
// Crash dialog/report dependency details are in client/exceptions.h.
// samp.dll RVA: 0x00060430
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);
// Stored callback only; frame/device implementation remains unrecovered.
// samp.dll RVA: 0x000c66b0
void ClientGameLoop();
#endif

#ifndef SAMP_CLIENT_INITIALIZE_H
#define SAMP_CLIENT_INITIALIZE_H
struct IDirect3DDevice9;
// Native start addresses; buffer backing windows are bounded by the next known native buffer.
// Overflow adjacency is not established by these declarations.
// samp.dll RVA: 0x002ac498
extern char g_clientConfigFilename[520];
// samp.dll RVA: 0x002ac6a0
extern char g_clientChatFilename[264];
// samp.dll RVA: 0x002abe70
extern IDirect3DDevice9* g_clientSavedGameDevice;
// samp.dll RVA: 0x002aca40
extern DWORD g_clientDeviceInitialized;
// samp.dll RVA: 0x002aca44
extern DWORD g_clientNetworkInitialized;
// samp.dll RVA: 0x000c5e70
void ClientInitializeDeviceAndGui();
#include "cmdprocs.h"
#include "game/debugscript.h"
#endif

#ifndef SAMP_CLIENT_DEVICE_SETUP_H
#define SAMP_CLIENT_DEVICE_SETUP_H
// Observed native buffers include the 261st byte cleared by startup.
// samp.dll RVA: 0x002ac7a8
extern char g_clientDataDirectory[261];
// samp.dll RVA: 0x002ac8b0
extern char g_clientCacheDirectory[261];
// samp.dll RVA: 0x000c51a0
const char* ClientGetDataDirectory();
// samp.dll RVA: 0x000c51b0
const char* ClientGetCacheDirectory();
// samp.dll RVA: 0x00061cf0
int ClientFindScreenshotFilename(std::string* path);
// samp.dll RVA: 0x00075040
void ClientCaptureScreenshot();
// Render-target capture; returns 1..4 for surface-operation failure, otherwise 0.
// samp.dll RVA: 0x00073b80
int ClientSaveRenderTarget(IDirect3DDevice9* device, const char* filename);
// samp.dll RVA: 0x0011b928
extern const char g_screenshotFilenamePattern[];
// samp.dll RVA: 0x0011c7dc
extern const char g_screenshotTakenMessage[];
// samp.dll RVA: 0x0011c7c0
extern const char g_screenshotFailureMessage[];
// samp.dll RVA: 0x0016be78
extern WNDPROC g_clientPreviousWindowProcedure;
// samp.dll RVA: 0x002aca50
extern DWORD g_clientPreviousRenderState;
// samp.dll RVA: 0x002aca54
extern DWORD g_clientRenderCaller;
// samp.dll RVA: 0x002aca58
extern DWORD g_clientRenderState;
// samp.dll RVA: 0x002aca5c
extern DWORD g_clientRenderValue;
// Native _stat success test accepts any existing file type.
// samp.dll RVA: 0x000b5900
BOOL ClientPathExists(const char* path);
// samp.dll RVA: 0x000c51c0
void ClientInitializeCacheDirectory();
// samp.dll RVA: 0x000c5340
void ClientInitializePaths();
// samp.dll RVA: 0x00061940
void ClientRefreshWindowProcedure();
// samp.dll RVA: 0x00061970
BOOL ClientInstallWindowProcedure();
// Native message routing; downstream input/UI dependencies remain explicit.
// samp.dll RVA: 0x000610d0
LRESULT CALLBACK ClientWindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
// samp.dll RVA: 0x0016be84
extern DWORD g_clientInputOutsideClick;
// samp.dll RVA: 0x0016be88
extern DWORD g_clientScoreboardOutsideClick;
// samp.dll RVA: 0x00061010
BOOL ClientHandleCharacter(unsigned int character);
// samp.dll RVA: 0x00060d90
void ClientToggleWindowedMode();
// Keyboard release dispatcher remains a dependency.
// samp.dll RVA: 0x00060de0
BOOL ClientHandleKeyRelease(unsigned int key);
// Installs the recovered callback into the current RenderWare engine table.
// samp.dll RVA: 0x000c54c0
void ClientInstallRenderStateHook();
// Naked two-word GTA render-state callback; tail-jumps through original target.
// samp.dll RVA: 0x000c5450
void ClientRenderStateHook();
#endif

#ifndef SAMP_CLIENT_FRAME_H
#define SAMP_CLIENT_FRAME_H
// samp.dll RVA: 0x002aca60
extern DWORD g_clientIntegrityFailures;
// samp.dll RVA: 0x002aca64
extern DWORD g_clientIntegrityFailed;
// samp.dll RVA: 0x002aca68
extern DWORD g_clientSkipGameFrame;
// samp.dll RVA: 0x002ac9c0
extern DWORD g_clientQuitRequested;
// samp.dll RVA: 0x002ac9c4
extern DWORD g_clientQuitStarted;
// samp.dll RVA: 0x000c6630
void ClientProcessFrame();
// Naked callback preserves the two native PUSHAD regions and GTA return registers.
// samp.dll RVA: 0x000c66b0
void ClientGameLoop();
// Full coordinator is recovered in the optional v105 profile; downstream startup dependencies remain open.
// samp.dll RVA: 0x000c5e70
void ClientInitializeDeviceAndGui();
#endif
