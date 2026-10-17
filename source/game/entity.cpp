#include "game/entity.h"
#include "game/scripting.h"
#include "game/ped_commands.h"
#include <string.h>
// samp.dll RVA: 0x0009bce0
CEntity::CEntity() { memset(unknown04,0,sizeof(unknown04)); }
// samp.dll RVA: 0x000b3720
void __stdcall GameWorldAdd(void *entity) { ((void (__cdecl*)(void*))0x563220)(entity); }
// samp.dll RVA: 0x000b3740
void __stdcall GameWorldRemove(void *entity) { ((void (__cdecl*)(void*))0x563280)(entity); }
// samp.dll RVA: 0x0009e950
void CEntity::GetMatrix(CMatrix *matrix) {
    if(m_pGameEntity && *(void**)((BYTE*)m_pGameEntity+0x14)) {
        BYTE *source=*(BYTE**)((BYTE*)m_pGameEntity+0x14);
        for(int i=0;i<4;i++)memcpy((BYTE*)matrix+i*16,source+i*16,12);
    }
}
// samp.dll RVA: 0x0009ee80
void CEntity::Teleport(CVector position) {
    void *entity=m_pGameEntity;
    if(entity && *(DWORD*)entity!=0x863c40) {
        short model=*(short*)((BYTE*)entity+0x22);
        if(model==0x21a || model==0x219 || model==0x1c1)Scripting::ProcessCommand(&g_entityTeleportCommand,m_nHandle,(double)position.x,(double)position.y,(double)position.z);
        else {
            __asm {
                push 0
                push position.z
                push position.y
                push position.x
                mov ecx,entity
                mov eax,[ecx]
                call dword ptr [eax+0x38]
            }
        }
    }
}
// samp.dll RVA: 0x0009f140
void CEntity::Add() {
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40 && !*(DWORD*)((BYTE*)m_pGameEntity+0xb4)) {
        memset((BYTE*)m_pGameEntity+0x44,0,24);GameWorldAdd(m_pGameEntity);
        CMatrix matrix;GetMatrix(&matrix);Teleport(matrix.position);
    }
}
// samp.dll RVA: 0x0009f240
void CEntity::Remove() {
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40 && *(DWORD*)((BYTE*)m_pGameEntity+0xb4))GameWorldRemove(m_pGameEntity);
}
