#ifndef SAMP_RECOVERED_RUNTIME_GLOBALS_H
#define SAMP_RECOVERED_RUNTIME_GLOBALS_H
#include <windows.h>
class CInput;
class CChat;
#include "game/game.h"
// samp.dll RVA: 0x002aca3c
extern CGame *g_pGame;
// samp.dll RVA: 0x002aca14
extern CInput *g_pInput;
// samp.dll RVA: 0x002aca10
extern CChat *g_pChat;
// samp.dll RVA: 0x002aca24
extern void *g_pNetGame;
#pragma pack(push,1)
struct ClientSettings {
    BOOL debug,online;
    DWORD unknown08;
    char password[257],host[257],port[257],nickname[257],debugScript[257];
};
#pragma pack(pop)
typedef char ClientSettingsExtent[(sizeof(ClientSettings)==1297)?1:-1];
typedef char ClientSettingsHostOffset[(offsetof(ClientSettings,host)==0x10d)?1:-1];
typedef char ClientSettingsScriptOffset[(offsetof(ClientSettings,debugScript)==0x410)?1:-1];
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
void __cdecl ClientStartupThread(void *argument);
namespace Settings {
    // samp.dll RVA: 0x000c5c90
    void Initialize();
    // samp.dll RVA: 0x000c5590
    void GetFromCommandLine(const char *line,char *output);
    // samp.dll RVA: 0x000c55e0
    void GetFromQuotes(const char *line,char *output);
}
// GTA SA VA: 0x00c17044 (outside samp.dll)
extern int g_chatScreenWidth; // GTA binding remains unresolved.
// GTA SA VA: 0x00c97c1c (outside samp.dll)
extern HWND g_inputGameWindow; // GTA binding remains unresolved.
#endif
