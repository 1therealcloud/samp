#include "game/entity.h"
// samp.dll RVA: 0x0009f5c0
void CEntity::ClearBackendFlag8(){if(m_pGameEntity)*(DWORD*)((BYTE*)m_pGameEntity+0x40)&=0xfffffff7;}
