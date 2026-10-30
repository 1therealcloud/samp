#include "archive/ArchiveFS.h"
#include "filehooks.h"
#include "game/hooks.h"
#include "main.h"
#include <process.h>
#include <string.h>

// Standard CRT integration adapter; no established standalone native entry.
// samp.dll RVA: unknown (standard VC7 CRT integration adapter; no established standalone native entry)
extern "C" BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
    return ClientDllMainForward(module, reason, reserved);
}

// 0x002ac9c8
HMODULE g_clientModule;

// 0x002ac9d4
CArchiveFS* g_pArchive;

// 0x0018dc38
DWORD g_clientStartupMarker;

// 0x000c6780
BOOL WINAPI ClientDllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        g_clientModule = module;
        Settings::Initialize();
        if (g_clientSettings.debug || g_clientSettings.online)
        {
            SetUnhandledExceptionFilter(ClientExceptionFilter);
            g_gameLoopAddress = (DWORD)ClientGameLoop;
            char archivePath[MAX_PATH];
            GetModuleFileNameA(g_clientModule, archivePath, MAX_PATH);
            int end = strlen(archivePath);
            while (archivePath[end] != '\\')
                --end;
            memcpy(archivePath + end + 1, "samp.saa", 9);
            // Native computes archivePath but opens the relative name below.
            g_pArchive = new CArchiveFS;
            if (!g_pArchive->Load("samp.saa"))
            {
                // clang-format off
                __asm { int 3 }
                // clang-format on
            }
            AddFontResourceA("gtaweap3.ttf");
            AddFontResourceA("sampaux3.ttf");
            ArchiveInstallFileHooks();
            ArchiveInstallCursorHook();
            g_pGame = new CGame;
            _beginthread(ClientStartupThread, 0, 0);
        }
    }
    else if (reason == DLL_PROCESS_DETACH && (g_clientSettings.debug || g_clientSettings.online))
    {
        ArchiveRemoveHooks();
    }
    return TRUE;
}

// 0x000c6930
BOOL WINAPI ClientDllMainForward(HINSTANCE module, DWORD reason, LPVOID reserved)
{
    return ClientDllMain(module, reason, reserved);
}

// 0x0009db90
void GameMarkStartupStage()
{
    g_clientStartupMarker = 1;
}

// 0x000c5150
void __cdecl ClientStartupThread(void* argument)
{
    (void)argument;
    if (g_pGame)
        g_pGame->InitGame();
    while (*(volatile DWORD*)0xc8d4c0 != 7)
        Sleep(5);
    GameMarkStartupStage();
    g_pGame->StartGame();
    ExitThread(0);
}
