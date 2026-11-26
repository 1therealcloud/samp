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

// main.cpp
extern HMODULE g_clientModule;

extern DWORD g_clientSkipGameFrame;
extern DWORD g_clientQuitRequested;

void ClientGameLoop();

void ClientRequestQuit();

// subclass.cpp
void ClientCaptureScreenshot();

BOOL ClientInstallWindowProcedure();

// client version
extern bool g_DLClient;
extern std::string g_ClientVersionString;

#endif // MAIN_H
