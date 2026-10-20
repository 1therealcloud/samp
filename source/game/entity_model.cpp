#include "game/entity.h"
#include "game/model.h"
#include "main.h"
#include "chatwindow.h"
// samp.dll RVA: 0x0009ed90
BOOL CEntity::SetModelIndex(int model) {
    if(!m_pGameEntity)return FALSE;
    BOOL loaded=FALSE;
    if(!g_pGame->IsModelAvailable(model) && !GameGetModelData1c(model)) {
        g_pGame->RequestModel(model,2);g_pGame->LoadRequestedModels();int polls=0;
        while(!g_pGame->IsModelAvailable(model)) {
            Sleep(1);
            if(++polls>200) { if(g_pChat)g_pChat->AddDebugMessage(g_gameModelTimeoutWarning,model);return FALSE; }
        }
        loaded=TRUE;
    }
    void *entity=m_pGameEntity;
    ((void (__fastcall*)(void*,void*))(*(DWORD**)entity)[8])(entity,0);
    *(short*)((BYTE*)entity+0x22)=(short)model;
    ((void (__fastcall*)(void*,void*,int))(*(DWORD**)entity)[5])(entity,0,model);
    return loaded;
}

// GTA SA virtual address: 0x00403EC0; ECX is the opaque GTA entity.
// Native forwards the complete argument word; backend reads its low model WORD.
// samp.dll RVA: 0x000b3990
void GameChangeBackendEntityModel(void *entity,DWORD model) {
    __asm {
        push model
        mov ecx,entity
        mov edx,0x403ec0
        call edx
    }
}
// Caller context/semantic object type is not established; raw first DWORD only.
// samp.dll RVA: 0x000b4120
DWORD __stdcall GameReadBackendDword(const void *backend) {
    return *(const DWORD*)backend;
}
