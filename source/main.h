#ifndef SAMP_MAIN_H
#define SAMP_MAIN_H

#include "cmdprocs.h"
#include "config.h"
#include "game/debugscript.h"
#include "game/game.h"
#include "game/gta_globals.h"
#include <string>
#include <windows.h>
class CInput;
class CChat;
// 0x002aca3c
extern CGame* g_pGame;
// 0x002aca14
extern CInput* g_pInput;
// 0x002aca10
extern CChat* g_pChat;
// 0x002aca24
extern void* g_pNetGame;
// Opaque native startup marker; callers and write are established, broader
// meaning is not assigned from the address alone.
// 0x0018dc38
extern DWORD g_clientStartupMarker;
// 0x0009db90
void GameMarkStartupStage();
// CRT _beginthread callback. Native termination is ExitThread, not _endthread.
// 0x000c5150
void __cdecl ClientStartupThread(void* argument);

// 0x002ac9c8
extern HMODULE g_clientModule;
// 0x000c6780
BOOL WINAPI ClientDllMain(HINSTANCE module, DWORD reason, LPVOID reserved);
// 0x000c6930
BOOL WINAPI ClientDllMainForward(HINSTANCE module, DWORD reason, LPVOID reserved);
// Crash diagnostics are declared in exceptions.h.
// 0x00060430
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);
// Native frame callback.
// 0x000c66b0
void ClientGameLoop();

struct IDirect3DDevice9;
// 0x002abe70
extern IDirect3DDevice9* g_clientSavedGameDevice;
// 0x002aca40
extern DWORD g_clientDeviceInitialized;
// 0x002aca44
extern DWORD g_clientNetworkInitialized;
// 0x000c5e70
void ClientInitializeDeviceAndGui();

// 0x00061cf0
int ClientFindScreenshotFilename(std::string* path);
// 0x00075040
void ClientCaptureScreenshot();
// Render-target capture; returns 1..4 for surface-operation failure, otherwise 0.
// 0x00073b80
int ClientSaveRenderTarget(IDirect3DDevice9* device, const char* filename);
// 0x0011b928
extern const char g_screenshotFilenamePattern[];
// 0x0011c7dc
extern const char g_screenshotTakenMessage[];
// 0x0011c7c0
extern const char g_screenshotFailureMessage[];
// 0x0016be78
extern WNDPROC g_clientPreviousWindowProcedure;
// 0x00061940
void ClientRefreshWindowProcedure();
// 0x00061970
BOOL ClientInstallWindowProcedure();
// Native message routing; downstream input/UI dependencies remain explicit.
// 0x000610d0
LRESULT CALLBACK ClientWindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
// 0x0016be84
extern DWORD g_clientInputOutsideClick;
// 0x0016be88
extern DWORD g_clientScoreboardOutsideClick;
// 0x00061010
BOOL ClientHandleCharacter(unsigned int character);
// 0x00060d90
void ClientToggleWindowedMode();
// Keyboard release dispatcher remains a dependency.
// 0x00060de0
BOOL ClientHandleKeyRelease(unsigned int key);

// 0x002aca60
extern DWORD g_clientIntegrityFailures;
// 0x002aca64
extern DWORD g_clientIntegrityFailed;
// 0x002aca68
extern DWORD g_clientSkipGameFrame;
// 0x002ac9c0
extern DWORD g_clientQuitRequested;
// 0x002ac9c4
extern DWORD g_clientQuitStarted;
// 0x000c6630
void ClientProcessFrame();
#endif
