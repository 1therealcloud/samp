#ifndef MAIN_H
#define MAIN_H

#include "cmdprocs.h"
#include "config.h"
#include "game/debugscript.h"
#include "game/game.h"

#include <string>
#include <windows.h>
class CInput;
class CChat;
// 0x0026ebac
extern CGame* g_pGame;
// 0x0026eb84
extern CInput* g_pInput;
// 0x0026eb80
extern CChat* g_pChat;
// 0x0026eb94
extern void* g_pNetGame;
// Opaque native startup marker; callers and write are established, broader
// meaning is not assigned from the address alone.
// R5 address to verify
extern DWORD g_clientStartupMarker;
// R5 address to verify
void GameMarkStartupStage();
// CRT _beginthread callback. Native termination is ExitThread, not _endthread.
// R5 address to verify
void __cdecl ClientStartupThread(void* argument);

// 0x0026eb38
extern HMODULE g_clientModule;
// 0x000c50c0
BOOL WINAPI ClientDllMain(HINSTANCE module, DWORD reason, LPVOID reserved);
// 0x000c5270
BOOL WINAPI ClientDllMainForward(HINSTANCE module, DWORD reason, LPVOID reserved);
// Crash diagnostics are declared in exceptions.h.
// 0x00060970
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);
// Native frame callback.
// 0x000c4ff0
void ClientGameLoop();

struct IDirect3DDevice9;
// R5 address to verify
extern IDirect3DDevice9* g_clientSavedGameDevice;
// R5 address to verify
extern DWORD g_clientDeviceInitialized;
// R5 address to verify
extern DWORD g_clientNetworkInitialized;
// 0x000c4790
void ClientInitializeDeviceAndGui();

// 0x00062270
int ClientFindScreenshotFilename(std::string* path);
// 0x000755c0
void ClientCaptureScreenshot();
// Render-target capture; returns 1..4 for surface-operation failure, otherwise 0.
// 0x000740e0
int ClientSaveRenderTarget(IDirect3DDevice9* device, const char* filename);
// 0x000e98f0
extern const char g_screenshotFilenamePattern[];
// 0x000ea7b4
extern const char g_screenshotTakenMessage[];
// 0x000ea798
extern const char g_screenshotFailureMessage[];
// R5 address to verify
extern WNDPROC g_clientPreviousWindowProcedure;
// 0x00061ec0
void ClientRefreshWindowProcedure();
// 0x00061ef0
BOOL ClientInstallWindowProcedure();
// Native message routing; downstream input/UI dependencies remain explicit.
// 0x00061650
LRESULT CALLBACK ClientWindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
// R5 address to verify
extern DWORD g_clientInputOutsideClick;
// R5 address to verify
extern DWORD g_clientScoreboardOutsideClick;
// 0x00061590
BOOL ClientHandleCharacter(unsigned int character);
// 0x00061310
void ClientToggleWindowedMode();
// Keyboard release dispatcher remains a dependency.
// 0x00061360
BOOL ClientHandleKeyRelease(unsigned int key);

// 0x0026ebd0
extern DWORD g_clientIntegrityFailures;
// R5 address to verify
extern DWORD g_clientIntegrityFailed;
// R5 address to verify
extern DWORD g_clientSkipGameFrame;
// R5 address to verify
extern DWORD g_clientQuitRequested;
// R5 address to verify
extern DWORD g_clientQuitStarted;
// 0x000c4f70
void ClientProcessFrame();

#endif // MAIN_H
