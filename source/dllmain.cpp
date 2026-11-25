#include "archive/ArchiveFS.h"
#include "filehooks.h"
#include "game/hooks.h"
#include "exceptions.h"
#include "main.h"

#include <MinHook.h>

#include <process.h>
#include <string.h>

HMODULE g_clientModule;

CArchiveFS* g_pArchive;

static void __cdecl ClientStartupThread(void* argument);

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        g_clientModule = module;
        Settings::Initialize();
        if (g_clientSettings.debug || g_clientSettings.online)
        {
            SetUnhandledExceptionFilter(ClientExceptionFilter);

            MH_Initialize();

            g_gameLoopAddress = (DWORD)ClientGameLoop;
            char archivePath[MAX_PATH];
            GetModuleFileNameA(g_clientModule, archivePath, MAX_PATH);
            int end = strlen(archivePath);
            while (archivePath[end] != '\\')
                --end;
            memcpy(archivePath + end + 1, "samp.saa", 9);
            g_pArchive = new CArchiveFS;
            if (!g_pArchive->Load("samp.saa"))
                __debugbreak(); // _asm int 3;

            AddFontResourceExA("gtaweap3.ttf", FR_PRIVATE, NULL);
            AddFontResourceExA("sampaux3.ttf", FR_PRIVATE, NULL);

            ArchiveInstallFileHooks();
            ArchiveInstallCursorHook();
            g_pGame = new CGame;
            _beginthread(ClientStartupThread, 0, 0);
        }
    }
    else if (reason == DLL_PROCESS_DETACH && (g_clientSettings.debug || g_clientSettings.online))
    {
        //ArchiveRemoveHooks();
        if (reserved == NULL)
        {
            //MH_DisableHook(MH_ALL_HOOKS);
            //MH_RemoveHook(MH_ALL_HOOKS);

            MH_Uninitialize();
        }
    }
    return TRUE;
}

static void __cdecl ClientStartupThread(void* argument)
{
    (void)argument;

    if (g_pGame)
        g_pGame->InitGame();

    while (*(volatile DWORD*)0xc8d4c0 != 7)
        Sleep(5);

    g_pGame->StartGame();
    ExitThread(0);
}
