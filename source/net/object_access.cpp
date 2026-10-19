#include "gui/device_objects.h"
#include "game/entity.h"
// samp.dll RVA: 0x00002e10
CEntity *CObjectPool::Get(WORD id) {
    BYTE *state=(BYTE*)this;
    if(id<2101&&*(DWORD*)(state+4+id*4))return *(CEntity**)(state+0x20d4+id*4);
    return 0;
}
