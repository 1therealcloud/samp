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

extern CGame* g_pGame;

extern CInput* g_pInput;

extern CChat* g_pChat;

extern void* g_pNetGame;
// Opaque native startup marker; callers and write are established, broader
// meaning is not assigned from the address alone.

extern DWORD g_clientStartupMarker;

void GameMarkStartupStage();
// CRT _beginthread callback. Native termination is ExitThread, not _endthread.

void __cdecl ClientStartupThread(void* argument);

extern HMODULE g_clientModule;

BOOL WINAPI ClientDllMain(HINSTANCE module, DWORD reason, LPVOID reserved);

BOOL WINAPI ClientDllMainForward(HINSTANCE module, DWORD reason, LPVOID reserved);
// Crash diagnostics are declared in exceptions.h.

LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);
// Native frame callback.

void ClientGameLoop();

struct IDirect3DDevice9;

extern IDirect3DDevice9* g_clientSavedGameDevice;

extern DWORD g_clientDeviceInitialized;

extern DWORD g_clientNetworkInitialized;

void ClientInitializeDeviceAndGui();

int ClientFindScreenshotFilename(std::string* path);

void ClientCaptureScreenshot();
// Render-target capture; returns 1..4 for surface-operation failure, otherwise 0.

int ClientSaveRenderTarget(IDirect3DDevice9* device, const char* filename);

extern const char g_screenshotFilenamePattern[];

extern const char g_screenshotTakenMessage[];

extern const char g_screenshotFailureMessage[];

extern WNDPROC g_clientPreviousWindowProcedure;

void ClientRefreshWindowProcedure();

BOOL ClientInstallWindowProcedure();
// Native message routing; downstream input/UI dependencies remain explicit.

LRESULT CALLBACK ClientWindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

extern DWORD g_clientInputOutsideClick;

extern DWORD g_clientScoreboardOutsideClick;

BOOL ClientHandleCharacter(unsigned int character);

void ClientToggleWindowedMode();
// Keyboard release dispatcher remains a dependency.

BOOL ClientHandleKeyRelease(unsigned int key);

extern DWORD g_clientSkipGameFrame;

extern DWORD g_clientQuitRequested;

extern DWORD g_clientQuitStarted;

void ClientProcessFrame();

extern bool g_DLClient;
extern const std::string g_ClientVersionString;

#endif // MAIN_H
