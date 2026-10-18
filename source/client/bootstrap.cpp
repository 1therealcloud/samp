#include "client/bootstrap.h"
#include "archive/provider.h"
#include "archive/filehooks.h"
#include "main.h"
#include "game/hooks.h"
#include <process.h>
#include <string.h>
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
