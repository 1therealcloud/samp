#include "client/device_setup.h"
#include "game/hooks_render.h"
#include "main.h"
// samp.dll RVA: 0x0016be78
WNDPROC g_clientPreviousWindowProcedure;
// samp.dll RVA: 0x002aca50
DWORD g_clientPreviousRenderState;
// samp.dll RVA: 0x002aca54
DWORD g_clientRenderCaller;
// samp.dll RVA: 0x002aca58
DWORD g_clientRenderState;
// samp.dll RVA: 0x002aca5c
DWORD g_clientRenderValue;
// samp.dll RVA: 0x00061970
BOOL ClientInstallWindowProcedure() {
    HWND window=g_inputGameWindow;
    if(!window)return FALSE;
    SetClassLongA(window,GCL_STYLE,GetClassLongA(window,GCL_STYLE)|CS_DBLCLKS);
    HWND current=g_inputGameWindow;
    if(current) {
        g_clientPreviousWindowProcedure=(WNDPROC)GetWindowLongA(current,GWL_WNDPROC);
        if(g_clientPreviousWindowProcedure!=ClientWindowProcedure)
            SetWindowLongA(current,GWL_WNDPROC,(LONG)ClientWindowProcedure);
    }
    SetWindowTextA(window,"GTA:SA:MP");IsWindowUnicode(window);return TRUE;
}
// samp.dll RVA: 0x000c5450
__declspec(naked) void ClientRenderStateHook() {
    __asm {
        mov eax,[esp]
        mov g_clientRenderCaller,eax
        mov eax,[esp+4]
        mov g_clientRenderState,eax
        mov eax,[esp+8]
        mov g_clientRenderValue,eax
        cmp g_clientRenderState,14
        jne forward
        cmp g_clientRenderValue,0
        je fog_off
        push 1
        call GameSetupD3DFog
        add esp,4
        mov g_hookFogEnabled,1
        jmp clear_fog
    fog_off:
        push 0
        call GameSetupD3DFog
        add esp,4
        mov g_hookFogEnabled,0
    clear_fog:
        mov byte ptr [esp+8],0
    forward:
        mov eax,g_clientPreviousRenderState
        jmp eax
    }
}
