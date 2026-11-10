#include "events.h"

#include "BitStream.h"
#include "RakClientInterface.h"
#include "game/playerped.h"
#include "game/vehicle.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/vehiclepool.h"
#include <static_assert.h>
// 0x00001aa0
void GameProcessIncomingEvent(WORD player, DWORD eventType, WORD vehicle, DWORD param2, DWORD param3)
{
    if (!g_pNetGame || g_localSkipInitialState)
        return;
    CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
    CVehiclePool* vehicles = pools->vehicles;
    CPlayerPool* players = pools->players;
    if (!players || !vehicles)
        return;
    CVehicle* car;
    switch (eventType)
    {
        case 1:
            car = vehicles->Get(vehicle);
            if (car)
                car->SetPaintjob((BYTE)param2);
            break;
        case 2:
            car = vehicles->Get(vehicle);
            if (car)
                car->AddComponent((WORD)param2);
            break;
        case 3:
            car = vehicles->Get(vehicle);
            if (car)
                car->SetColor((BYTE)param2, (BYTE)param3);
            break;
        case 4:
            if (player <= 1004)
            {
                CRemotePlayer* remote = players->GetPlayer(player);
                CLocalPlayer* local = players->local;
                car = vehicles->Get(vehicle);
                if (!car || !remote || !local)
                    return;
                CPed* localPed = local->m_state.ped;
                CPed* remotePed = remote->m_state.ped;
                if (!localPed || !remotePed)
                    return;
                if (!remotePed->IsInVehicle() || !localPed->IsInVehicle() || remotePed->IsPassenger() || !localPed->IsPassenger())
                    return;
                DWORD remoteRef = remotePed->GetVehicleRef();
                if (remoteRef != localPed->GetVehicleRef())
                    return;
                CVehicle* remoteVehicle = remote->m_state.vehicle;
                if (param2)
                {
                    if (remoteVehicle)
                        remoteVehicle->ChangeInterior(param3);
                    localPed->LinkToInterior((BYTE)param3, FALSE);
                    localPed->SetControllable(FALSE);
                }
                else
                {
                    if (remoteVehicle)
                        remoteVehicle->ChangeInterior(0);
                    localPed->LinkToInterior(0, FALSE);
                    localPed->SetControllable(TRUE);
                }
            }
            break;
    }
}
#pragma pack(push, 1)
// Partial pointer views; allocation/lifecycle and other members remain unrecovered.
struct ScmPoolsView
{
    BYTE unknown00[0xc];
    CVehiclePool* vehicles;
};
struct ScmNetGameView
{
    BYTE unknown00[0x2c];
    RakClientInterface* client;
    BYTE unknown30[0x3ae];
    ScmPoolsView* pools;
};
#pragma pack(pop)
STATIC_ASSERT(offsetof(ScmNetGameView, pools) == 0x3de);
// 0x00117690
int g_scmEventRpcId = 96;
// 0x00156988
DWORD g_scmEventParameters[4];
// 0x00001d10
void GameProcessOutgoingEvent(DWORD vehicleHandle, DWORD param2, DWORD param3, DWORD eventType)
{
    if (g_pNetGame)
    {
        CVehiclePool* pool = ((ScmNetGameView*)g_pNetGame)->pools->vehicles;
        RakNet::BitStream stream;
        if (eventType < 1 || eventType > 4)
            return;
        WORD id = pool->Find(GamePoolVehicleGetAt((int)vehicleHandle));
        CVehicle* vehicle = eventType == 4 ? pool->Get(id) : 0;
        if (id == 0xffff)
            return;
        stream.Write((DWORD)id);
        stream.Write(param2);
        stream.Write(param3);
        stream.Write(eventType);
        ((ScmNetGameView*)g_pNetGame)->client->RPC(&g_scmEventRpcId, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
        if (eventType == 3)
        {
            vehicle = pool->Get(id);
            if (vehicle)
                vehicle->SetColor((BYTE)param2, (BYTE)param3);
        }
        else if (eventType == 4 && vehicle && vehicle->m_pGameVehicle)
        {
            if (param2 == 0)
            {
                vehicle->SetColor(((BYTE*)vehicle->m_pGameVehicle)[0x434], ((BYTE*)vehicle->m_pGameVehicle)[0x435]);
                vehicle->m_bInModShop = FALSE;
            }
            else
                vehicle->m_bInModShop = TRUE;
        }
    }
}
// 0x000020c0
__declspec(naked) void GameSetEventOpcode()
{
    // clang-format off
    __asm {
        pushad
        push 4
        mov ecx,esi
        mov ebx,0x464080
        call ebx
        mov eax,dword ptr ds:[0xa43c7c]
        mov g_scmEventParameters[4],eax
        mov ecx,dword ptr ds:[0xa43c80]
        mov g_scmEventParameters[8],ecx
        mov edx,dword ptr ds:[0xa43c84]
        mov g_scmEventParameters[12],edx
        mov eax,dword ptr ds:[0xa43c78]
        mov g_scmEventParameters[0],eax
        push g_scmEventParameters[0]
        push g_scmEventParameters[12]
        push g_scmEventParameters[8]
        push g_scmEventParameters[4]
        call GameProcessOutgoingEvent
        add esp,16
        popad
        mov ebx,0x47bf2b
        jmp ebx
    }
    // clang-format on
}

#include "game/patches.h"
#include "game/playerped.h"
// 0x00156980
CPed* g_driverPed;
// 0x00156998
BOOL g_driverCompareFlag;
// 0x0015699c
void* g_driverScriptThread;
// 0x00001c60
__declspec(naked) void GamePlayerDriverOpcode()
{
    // clang-format off
    __asm {
        pushad
        mov g_driverScriptThread,esi
        mov g_driverCompareFlag,1
        cmp g_pGame,0
        je compare
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        mov g_driverPed,eax
        mov ecx,g_driverPed
        call CPed::IsInVehicle
        test eax,eax
        je compare
        mov ecx,g_driverPed
        call CPed::IsPassenger
        test eax,eax
        jne compare
        mov g_driverCompareFlag,0
    compare:
        push g_driverCompareFlag
        mov ecx,g_driverScriptThread
        mov ebx,0x4859d0
        call ebx
        popad
        mov ebx,0x47bf2b
        jmp ebx
    }
    // clang-format on
}
