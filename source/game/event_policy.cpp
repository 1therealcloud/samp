#include "game/task_events.h"
#include "game/playerped.h"
#include "net/playerpool.h"
#include "net/actorpool.h"
#include "net/remoteplayer.h"
#include "gui/render_dependencies.h"
#include "main.h"
// samp.dll RVA: 0x0018e81c
CVehicle *g_hookEnterVehicle;
// samp.dll RVA: 0x000a3430
BOOL GameNotifyEnterVehicle() {
    if(!g_pNetGame)return TRUE;
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    g_hookEventVehiclePool=*(CVehiclePool**)(pools+0xc);
    CPlayerPool *players=*(CPlayerPool**)(pools+8);
    g_hookEventLocalPlayer=*(CLocalPlayer**)((BYTE*)players+0x1e);
    g_hookEventVehicleId=g_hookEventVehiclePool->Find((void*)g_hookControlVehicle);
    WORD id=g_hookEventVehicleId;
    if(id==0xffff || id>=2000 || !g_hookEventVehiclePool->m_bNotEmpty[id])return FALSE;
    g_hookEnterVehicle=g_hookEventVehiclePool->m_pObjects[id];
    if(*(DWORD*)((BYTE*)g_hookEnterVehicle+0x60) ||
        *(short*)((BYTE*)g_hookEnterVehicle->m_pGameVehicle+0x22)==0x23a)return FALSE;
    if(g_hookEventLocalPlayer) {
        CPed *ped=*(CPed**)g_hookEventLocalPlayer;
        if(ped && ped->HasDanceState())return FALSE;
        if(g_hookEventLocalPlayer) {
            ped=*(CPed**)g_hookEventLocalPlayer;
            if(ped && ped->HasDanceStyle())return FALSE;
        }
    }
    BYTE *driver=*(BYTE**)((BYTE*)g_hookEnterVehicle->m_pGameVehicle+0x460);
    if(driver) {
        if(*(DWORD*)(driver+0x598))return FALSE;
        WORD player=players->Find(driver);
        if(players->IsConnected(player) && players->GetPlayerInfoFlag(player))return FALSE;
    }
    // Native deliberately has no local-player null guard on this final path.
    CPed *ped=*(CPed**)g_hookEventLocalPlayer;
    if(ped && ped->GetCurrentWeapon()==46) {
        ped=*(CPed**)g_hookEventLocalPlayer;ped->SetArmedWeapon(0,0);
    }
    g_hookEventLocalPlayer->EnterVehicle(g_hookEventVehicleId,FALSE);
    return TRUE;
}
// samp.dll RVA: 0x000a3840
WORD GameResolveDamagePlayer(void *entity) {
    if(!entity)return 0xffff;
    CPlayerPool *players=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
    WORD result=players->Find(entity);
    if(result==0xffff) {
        CVehiclePool *vehicles=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
        WORD vehicle=vehicles->Find(entity);
        if(vehicle!=0xffff) {
            BYTE *state=(BYTE*)players;LONG last=*(LONG*)(state+0x22);
            for(DWORD slot=0;(LONG)slot<=last;++slot) {
                WORD player=(WORD)slot;
                if(player<1004 && *(DWORD*)(state+0xfd6+player*4)) {
                    BYTE *entry=*(BYTE**)(state+0x26+player*4);
                    BYTE *remote=entry?*(BYTE**)(entry+8):0;
                    if(*(WORD*)(remote+2)==vehicle){result=player;break;}
                }
            }
        }
    }
    return result;
}
// samp.dll RVA: 0x000a38e0
DWORD GameResolveDamageActor(void *entity) {
    if(!entity)return 0xffff;
    CActorPool *actors=*(CActorPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+4);
    return actors->Find(entity);
}
// Observed first four DWORDs only, not a complete native allocation layout.
struct GameDamageResponsePrefix { void *entity;DWORD damageBits,bodyPart,weapon; };
// samp.dll RVA: 0x000a3910
BOOL __stdcall GameNotifyPedDamage(void *response,void *ped) {
    void *local=GameFindLocalPed();
    if(!g_pNetGame)return FALSE;
    GameDamageResponsePrefix *damage=(GameDamageResponsePrefix*)response;
    void *attacker=damage->entity;
    if(attacker!=local && ped && *(DWORD*)ped==0x86c0a8)return TRUE;
    bool taken=ped==local;
    if(!taken && attacker!=local)return FALSE;
    CPlayerPool *players=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
    WORD player;
    if(taken) {
        BYTE *localPlayer=*(BYTE**)((BYTE*)players+0x1e);
        BYTE *protectedObject=*(BYTE**)(localPlayer+0x2de);
        if(protectedObject) {
            if(*(void**)(protectedObject+0x40)==attacker || damage->weapon==54)return TRUE;
        }
        player=GameResolveDamagePlayer(attacker);
        (*(CLocalPlayer**)((BYTE*)players+0x1e))->SendTakeDamage(player,damage->damageBits,damage->weapon,damage->bodyPart);
    } else {
        player=GameResolveDamagePlayer(ped);
        if(player==0xffff) {
            DWORD actor=GameResolveDamageActor(ped);
            if((WORD)actor!=0xffff) {
                (*(CLocalPlayer**)((BYTE*)players+0x1e))->SendActorDamage((WORD)actor,damage->damageBits,damage->weapon,damage->bodyPart);
                return TRUE;
            }
        } else {
            (*(CLocalPlayer**)((BYTE*)players+0x1e))->SendGiveDamage(player,damage->damageBits,damage->weapon,damage->bodyPart);
            if(*(DWORD*)((BYTE*)players->GetPlayer(player)+0x1c))return TRUE;
        }
    }
    BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
    if(settings[0x22]) {
        BYTE team=(*(BYTE**)((BYTE*)players+0x1e))[0x130];
        if(team!=0xff && player!=0xffff) {
            CPlayerPool *reloaded=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
            if(((BYTE*)reloaded->GetPlayer(player))[0x19]==team)return TRUE;
        }
    }
    return FALSE;
}
