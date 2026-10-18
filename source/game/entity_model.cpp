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
