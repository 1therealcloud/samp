#include "client/device_setup.h"
#include "main.h"
#include "cmdwindow.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
// samp.dll RVA: 0x00061010
BOOL ClientHandleCharacter(unsigned int character) {
    if(g_pInput&&g_pInput->m_bEnabled){if(character==27){g_pInput->Close();return TRUE;}}
    else if(g_deviceScoreboard&&*(DWORD*)g_deviceScoreboard){if(character==27){g_deviceScoreboard->Close(true);return TRUE;}}
    else if(g_renderDialog&&*(DWORD*)(g_renderDialog->nativeState+0x28)){if(character==27){g_renderDialog->Close(0);return TRUE;}}
    else if((character=='T'||character=='`'||character=='t')&&g_pInput&&(!g_renderDialog||!*(DWORD*)(g_renderDialog->nativeState+0x28))&&!g_pGame->IsMenuVisible()) {g_pInput->Open();return TRUE;}
    return FALSE;
}
// samp.dll RVA: 0x00060d90
void ClientToggleWindowedMode() {
    // GTA SA virtual addresses: current mode C97C18, mode table C97C48, change-mode 745C70.
    int mode=*(int*)0xc97c18;
    DWORD *windowed=(DWORD*)(*(DWORD*)0xc97c48+mode*20+16);
    *windowed=*windowed==0;
    *(int*)0xc97c18=-1;
    ((void(__cdecl*)(int))0x745c70)(mode);
    *(int*)0xc97c18=mode;
}
