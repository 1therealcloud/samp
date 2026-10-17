#include "game/input.h"
#include <stddef.h>
#include <string.h>
// samp.dll RVA: 0x002ac9d0
IDirect3DDevice9 *g_gameD3DDevice;
// samp.dll RVA: 0x002ac9b8
int g_gameVersion;
// samp.dll RVA: 0x0018e1c8
DWORD g_gameMouseDummy;
// samp.dll RVA: 0x00151918
BYTE g_gameKeyCall[5]={0xe8,0x46,0xf3,0xfe,0xff};
// samp.dll RVA: 0x00151920
BYTE g_gameMouseCallEU[5]={0xe8,0x04,0x7b,0x20,0x00};
// samp.dll RVA: 0x00151928
BYTE g_gameMouseCallUS[5]={0xe8,0xb4,0x7a,0x20,0x00};

// samp.dll RVA: 0x000aa0b0
void GameUnprotect(void *address,SIZE_T size) {
    DWORD oldProtect;
    VirtualProtect(address,size,PAGE_EXECUTE_READWRITE,&oldProtect);
}
// samp.dll RVA: 0x000a02f0
void GameClearMouseDelta() {
    *(DWORD*)0xb73424=0; *(DWORD*)0xb73428=0;
    ((void (__cdecl*)())0x541bd0)();
}
// samp.dll RVA: 0x000a0310
void GameUpdateMouse() { ((void (__cdecl*)())0x541dd0)(); }
// samp.dll RVA: 0x000a0320
void GameDisableMousePointers() {
    *(DWORD*)0xb7340c=0; *(DWORD*)0xb73410=0; *(DWORD*)0xb73414=0;
    GameUnprotect((void*)0x53f47a,4);GameUnprotect((void*)0x53f49a,4);GameUnprotect((void*)0x53f4b3,4);
    *(DWORD*)0x53f47a=(DWORD)&g_gameMouseDummy;
    *(DWORD*)0x53f49a=(DWORD)&g_gameMouseDummy;
    *(DWORD*)0x53f4b3=(DWORD)&g_gameMouseDummy;
}
// samp.dll RVA: 0x000a0370
void GameRestoreMousePointers() {
    GameUnprotect((void*)0x53f47a,4);GameUnprotect((void*)0x53f49a,4);GameUnprotect((void*)0x53f4b3,4);
    *(DWORD*)0x53f47a=0xb73410;*(DWORD*)0x53f49a=0xb73414;*(DWORD*)0x53f4b3=0xb7340c;
}
// samp.dll RVA: 0x000a03c0
void GameDisableMouseCalls() {
    GameUnprotect((void*)0x53f417,5);memset((void*)0x53f417,0x90,5);
    GameUnprotect((void*)0x53f41f,4);
    *(BYTE*)0x53f41f=0x33;*(BYTE*)0x53f420=0xc0;*(BYTE*)0x53f421=0x0f;*(BYTE*)0x53f422=0x84;
}
// samp.dll RVA: 0x000a0760
BOOL CGame::IsMenuVisible() { return *(DWORD*)0xba67a4!=0; }
// samp.dll RVA: 0x000a0530
void CGame::SetCursorMode(int mode,BOOL immediately) {
    CGame *state=this;
    if(mode==2) {
        GameUnprotect((void*)0x541df5,5);memset((void*)0x541df5,0x90,5);
        GameDisableMouseCalls();GameClearMouseDelta();GameUpdateMouse();
        GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;
        g_gameD3DDevice->ShowCursor(TRUE);state->m_nCursorMode=2;
    } else if(mode==1) {
        if(state->m_nCursorMode!=1) {
            GameUnprotect((void*)0x541df5,5);memset((void*)0x541df5,0x90,5);state->m_nCursorMode=1;
        }
    } else if(mode==3) {
        if(state->m_nCursorMode!=3) {
            GameDisableMouseCalls();GameClearMouseDelta();GameUpdateMouse();
            GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;
            g_gameD3DDevice->ShowCursor(TRUE);state->m_nCursorMode=3;
        }
    } else if(mode==4) {
        if(state->m_nCursorMode!=4) {
            GameDisableMousePointers();GameClearMouseDelta();GameUpdateMouse();
            GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;state->m_nCursorMode=4;
        }
    } else if(mode==0 && state->m_nCursorMode!=0) {
        state->m_nInputEnableWaitFrames=immediately?0:10;
        g_gameD3DDevice->ShowCursor(FALSE);state->m_nCursorMode=0;
    }
}
// samp.dll RVA: 0x000a0410
void CGame::ProcessInputEnabling() {
    CGame *state=this;
    if(state->m_nCursorMode==0) {
        if(state->m_nInputEnableWaitFrames==0) {
            GameUnprotect((void*)0x541df5,5);memcpy((void*)0x541df5,g_gameKeyCall,5);
            GameUnprotect((void*)0x53f417,5);
            memcpy((void*)0x53f417,g_gameVersion==1?g_gameMouseCallUS:g_gameMouseCallEU,5);
            GameRestoreMousePointers();
            // Native changes protection at +2, then writes the four-byte instruction.
            GameUnprotect((void*)0x53f421,4);
            *(BYTE*)0x53f41f=0x85;*(BYTE*)0x53f420=0xc0;*(BYTE*)0x53f421=0x0f;*(BYTE*)0x53f422=0x8c;
            GameClearMouseDelta();GameUpdateMouse();GameClearMouseDelta();
            GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xe9;
            g_gameD3DDevice->ShowCursor(FALSE);--state->m_nInputEnableWaitFrames;
        } else if(state->m_nInputEnableWaitFrames>0) --state->m_nInputEnableWaitFrames;
    }
}
