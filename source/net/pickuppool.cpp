#include "net/pickuppool.h"
#include "game/game.h"
#include "game/scripting.h"
#include <string.h>
// samp.dll RVA: 0x00118158
const Scripting::OpcodeInfo g_pickupDestroyCommand={0x215,"i"};
// samp.dll RVA: 0x00008190
CPickupPool::CPickupPool(){memset(pickups,0,sizeof(pickups));count=0;for(int i=0;i<4096;++i){handles[i]=0;cooldown[i]=0;indices[i]=-1;}}
// samp.dll RVA: 0x00013170
void CPickupPool::Create(const Pickup *pickup,int id){
    if(count>=4096 || id<0 || id>=4096)return;
    if(handles[id])Scripting::ProcessCommand(&g_pickupDestroyCommand,handles[id]);
    for(int i=0;i<5;++i)((DWORD*)&pickups[id])[i]=((const DWORD*)pickup)[i];
    weapons[id].enabled=0;
    DWORD index;
    DWORD handle=g_pGame->CreatePickup(pickup->model,pickup->type,pickup->x,pickup->y,pickup->z,&index);
    handles[id]=handle;indices[id]=(int)index;++count;
}
// samp.dll RVA: 0x00013220
void CPickupPool::Delete(int id){
    if(count<=0 || id<0 || id>=4096 || !handles[id])return;
    Scripting::ProcessCommand(&g_pickupDestroyCommand,handles[id]);
    handles[id]=0;cooldown[id]=0;indices[id]=-1;--count;
}
// samp.dll RVA: 0x00013280
void CPickupPool::DeleteWeapon(WORD owner){for(int i=0;i<4096;++i)if(handles[i]&&weapons[i].enabled&&weapons[i].owner==owner){Scripting::ProcessCommand(&g_pickupDestroyCommand,handles[i]);handles[i]=0;cooldown[i]=0;--count;}}
// samp.dll RVA: 0x000132e0
int CPickupPool::GetIndex(int index){for(int i=0;i<4096;++i)if(indices[i]==index)return i;return -1;}
// samp.dll RVA: 0x00013310
CPickupPool::~CPickupPool(){for(int i=0;i<4096;++i)if(handles[i])Scripting::ProcessCommand(&g_pickupDestroyCommand,handles[i]);}
