#include "net/playerpool.h"

#include "game/game.h"
#include "game/playerped.h"
#include "main.h"
#include "net/actorpool.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/playerinfo.h"
#include "net/remoteplayer.h"

#include <string.h>

DWORD g_playerPoolProcessCount;

const char g_playerPoolRemoteException[] = "s001";

const char g_playerPoolLocalException[] = "s002";

DWORD g_cachedActivePlayerCount = 0;

DWORD g_activePlayerCountCountdown = 0;

CPlayerInfo* CPlayerPool::GetPlayerInfo(WORD player)
{
    if (player < 1004)
    {
        CPlayerInfo* entry = *(CPlayerInfo**)((BYTE*)this + 0x26 + (DWORD)player * 4);
        if (entry)
            return entry;
    }
    return 0;
}

const char* CPlayerPool::GetLocalPlayerName()
{
    return localName.c_str();
}

void CPlayerPool::SetLocalPlayerName(const char* text)
{
    localName.assign(text, strlen(text));
}

void CPlayerPool::SetAt(WORD player, CPlayerInfo* info)
{
    if (player < 1004)
        *(CPlayerInfo**)((BYTE*)this + 0x26 + (DWORD)player * 4) = info;
}

void CPlayerPool::SetName(WORD player, const char* text)
{
    if (player < 1004 && players[player])
        players[player]->name.assign(text, strlen(text));
}

BOOL CPlayerPool::Process()
{
    ++g_playerPoolProcessCount;
    for (WORD id = 0; id < 1004; ++id)
        if (connected[id] == 1)
        {
            __try
            {
                GetPlayer(id)->Process();
            }
            __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), g_playerPoolRemoteException))
            {
                if (GetPlayer(id))
                    GetPlayer(id)->Remove();
            }
        }
    __try
    {
        if (local)
            local->Process();
    }
    __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), g_playerPoolLocalException))
    {
        // Native local-player handler performs no cleanup and returns normally.
    }
    return TRUE;
}

WORD CPlayerPool::FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord)
{
    struct RayPoint
    {
        CVector point;
        DWORD word;
    };
    RayPoint from = {*origin, rayWord}, to = {*end, rayWord};
    WORD selected = 0xffff;
    float closest = 100000.0f;
    for (int i = 0; i <= largestId; ++i)
    {
        if (!connected[i])
            continue;
        // Native GetPlayer admits low-WORD ID1004; remote itself is not guarded.
        CRemotePlayer* remote = GetPlayer((WORD)i);
        CPed* ped = remote->m_state.ped;
        if (!ped || !ped->DoesExist())
            continue;
        if (!(ped->GetDistanceToCamera() < maxCameraDistance))
            continue;
        CVector centre;
        ped->GetBoundCentre(&centre);
        if (!GameTestEntityLineSphere(&from.point, &to.point, ped->m_pGameEntity, &centre, 1.5f, 1))
            continue;
        const double distance = ped->GetDistanceToPoint(origin->x, origin->y, origin->z);
        if (distance < closest)
        {
            closest = (float)distance;
            selected = (WORD)i;
        }
    }
    return selected;
}

void CPlayerPool::Deactivate()
{
    local->m_state.active = 0;
    *(DWORD*)&this->local->m_state.selectedClass = 0;
    for (WORD id = 0; id < 1004; ++id)
        if (IsConnected(id) == 1)
        {
            CRemotePlayer* remote = players[id] ? players[id]->remote : 0;
            remote->m_state.playerState = 0;
            remote->m_state.showNameTag = 1;
        }
}

CPlayerPool::CPlayerPool()
{
    for (WORD id = 0; id < 1004; id++)
    {
        if (id < 1004)
            players[id] = 0;
        connected[id] = 0;
    }
    local = new CLocalPlayer;
    localScore = 0;
    localPing = 0;
    localId = 0;
    largestId = 0;
}

BOOL CPlayerPool::Create(WORD id, const char* name, DWORD npc)
{
    CPlayerInfo* info = new CPlayerInfo(name, npc);
    if (id >= 1004)
        return FALSE;
    BYTE* state = (BYTE*)this;
    CPlayerInfo** slot = (CPlayerInfo**)(state + 0x26 + id * 4);
    *slot = info;
    if (!info)
        return FALSE;
    info->remote->m_state.id = id;
    if (npc)
    {
        CRemotePlayer* remote = *slot ? (*slot)->remote : 0;
        remote->m_state.isNpc = 1;
    }
    *(DWORD*)(state + 0xfd6 + id * 4) = 1;
    UpdateLargestId();
    return TRUE;
}

BOOL CPlayerPool::Delete(WORD id, BYTE reason)
{
    (void)reason;
    if (id >= 1004 || !connected[id] || !players[id])
        return FALSE;
    if (local && local->m_state.spectating && local->m_state.spectateId == id)
        local->ToggleSpectating(FALSE);
    connected[id] = 0;
    delete players[id];
    players[id] = 0;
    UpdateLargestId();
    return TRUE;
}

CPlayerPool::~CPlayerPool()
{
    if (local)
        delete local;
    local = 0;
    for (WORD id = 0; id < 1004; id++)
        Delete(id, 0);
}

void CPlayerPool::UpdateLargestId()
{
    int largest = 0;
    for (int id = 0; id < 1004; id++)
        if (connected[id])
            largest = id;
    largestId = largest;
}

void CPlayerPool::ForceCollision()
{
    int highest = largestId;
    for (WORD id = 0; (int)id <= highest; id++)
    {
        if (id >= 1004 || connected[id] != 1)
            continue;
        CRemotePlayer* remote = players[id] ? players[id]->remote : 0;
        CPed* ped = remote->m_state.ped;
        if (ped && !ped->IsInVehicle())
        {
            savedCollision[id] = ped->GetCollisionFlag();
            ped->SetCollisionFlag(TRUE);
        }
    }
}

void CPlayerPool::RestoreCollision()
{
    int highest = largestId;
    for (WORD id = 0; (int)id <= highest; id++)
    {
        if (id >= 1004 || connected[id] != 1)
            continue;
        CRemotePlayer* remote = players[id] ? players[id]->remote : 0;
        CPed* ped = remote->m_state.ped;
        if (ped && !ped->IsInVehicle())
        {
            if (savedCollision[id])
                ped->SetCollisionFlag(TRUE);
            else
                ped->SetCollisionFlag(FALSE);
        }
    }
}

CLocalPlayer* CPlayerPool::GetLocalPlayer()
{
    return local;
}

int CPlayerPool::GetCount(BOOL includeFlagged)
{
    int count = 0;
    if (includeFlagged)
    {
        for (int id = 0; id < 1004; ++id)
            if (IsConnected((WORD)id) == 1)
                ++count;
    }
    else
    {
        for (int id = 0; id < 1004; ++id)
            if (IsConnected((WORD)id) == 1 && !players[(WORD)id]->npc)
                ++count;
    }
    return count;
}

int CPlayerPool::GetScore(WORD player)
{
    BYTE* entry;
    if (player >= 1004)
        goto noEntry;
    entry = *(BYTE**)((BYTE*)this + 0x26 + player * 4);
    if (!entry)
    {
    noEntry:
        return 0;
    }
    return *(int*)entry;
}

DWORD CPlayerPool::GetPing(WORD player)
{
    BYTE* entry;
    if (player >= 1004)
        goto noEntry;
    entry = *(BYTE**)((BYTE*)this + 0x26 + player * 4);
    if (!entry)
    {
    noEntry:
        return 0;
    }
    return *(DWORD*)(entry + 0xc);
}

int CPlayerPool::GetLocalScore()
{
    return localScore;
}

DWORD CPlayerPool::GetLocalPing()
{
    return localPing;
}

CRemotePlayer* CPlayerPool::GetPlayer(WORD player)
{
    if (player < 1004)
    {
        BYTE* entry = *(BYTE**)((BYTE*)this + 0x26 + player * 4);
        if (entry)
            return *(CRemotePlayer**)(entry + 8);
    }
    return 0;
}

DWORD CPlayerPool::IsConnected(WORD player)
{
    if (player >= 1004)
        return 0;
    return *(DWORD*)((BYTE*)this + 0xfd6 + player * 4);
}

DWORD CPlayerPool::GetPlayerInfoFlag(WORD player)
{
    BYTE* entry;
    if (player >= 1004)
        goto noEntry;
    entry = *(BYTE**)((BYTE*)this + 0x26 + player * 4);
    if (!entry)
    {
    noEntry:
        return 0;
    }
    return *(DWORD*)(entry + 4);
}

WORD CPlayerPool::Find(void* ped)
{
    for (WORD player = 0; (int)player <= largestId; ++player)
    {
        if (player < 1004 && connected[player] == 1)
        {
            CPlayerInfo* entry = players[player];
            CRemotePlayer* remote = entry ? entry->remote : 0;
            // Native dereferences remote+4 even for inconsistent/null records.
            CPed* wrapper = remote->m_state.ped;
            if (wrapper && wrapper->m_pGamePed && wrapper->m_pGamePed == ped)
            {
                entry = players[player];
                if (!entry)
                    return *(WORD*)0;
                return entry->remote->m_state.id;
            }
        }
    }
    return 0xffff;
}

void CPlayerPool::ProcessPlayerPosesAndAccessories()
{
    CLocalPlayer* local = this->local;
    if (local && local->m_state.ped)
    {
        local->m_state.ped->ProcessAnimationPose();
        local = this->local;
        local->m_state.ped->ProcessAccessories();
    }
    // Native captures the highest ID once; the WORD counter wraps naturally.
    int highest = largestId;
    for (WORD player = 0; (int)player <= highest; ++player)
    {
        if (player >= 1004 || connected[player] != 1)
            continue;
        CPlayerInfo* entry = players[player];
        CRemotePlayer* remote = entry ? entry->remote : 0;
        // Preserve the native unchecked remote+4 read for inconsistent records.
        if (remote->m_state.ped)
        {
            remote->m_state.ped->ProcessAnimationPose();
            remote->m_state.ped->ProcessAccessories();
        }
    }
}

CObject* CPlayerPool::FindAccessory(void* backend)
{
    CLocalPlayer* local = this->local;
    CPed* ped = local ? local->m_state.ped : 0;
    if (ped)
        for (int slot = 0; slot < 10; slot++)
        {
            if (!*(volatile BOOL*)&ped->m_accessoryOccupied[slot])
                continue;
            CEntity* object = (CEntity*)ped->m_accessoryObjects[slot];
            if (object && object->m_pGameEntity == backend)
                return *(volatile BOOL*)&ped->m_accessoryOccupied[slot] ? (CObject*)ped->m_accessoryObjects[slot] : 0;
        }
    int highest = largestId;
    for (WORD player = 0; (int)player <= highest; ++player)
    {
        if (player >= 1004 || connected[player] != 1)
            continue;
        CPlayerInfo* entry = players[player];
        CRemotePlayer* remote = entry ? entry->remote : 0;
        // Native dereferences remote+4 even for inconsistent records.
        ped = remote->m_state.ped;
        if (!ped)
            continue;
        for (int slot = 0; slot < 10; slot++)
        {
            if (!*(volatile BOOL*)&ped->m_accessoryOccupied[slot])
                continue;
            CEntity* object = (CEntity*)ped->m_accessoryObjects[slot];
            if (object && object->m_pGameEntity == backend)
                return *(volatile BOOL*)&ped->m_accessoryOccupied[slot] ? (CObject*)ped->m_accessoryObjects[slot] : 0;
        }
    }
    return 0;
}

const char* CPlayerPool::GetName(WORD player)
{
    BYTE* state = (BYTE*)this;
    if (player == *(WORD*)state)
        return *(DWORD*)(state + 0x1a) >= 16 ? *(char**)(state + 6) : (char*)state + 6;
    if (player >= 1004)
        return 0;
    BYTE* entry = *(BYTE**)(state + 0x26 + player * 4);
    if (!entry)
        return 0;
    return *(DWORD*)(entry + 0x28) >= 16 ? *(char**)(entry + 0x14) : (char*)entry + 0x14;
}

int CPlayerPool::GetLargestId()
{
    return largestId;
}

WORD CPlayerPool::GetLocalId()
{
    return localId;
}

void CPlayerPool::SetLocalId(WORD id)
{
    localId = id;
}

void CPlayerPool::SetScore(WORD id, int value)
{
    if (id == localId)
    {
        localScore = value;
        return;
    }
    if (id >= 1004)
        return;
    CPlayerInfo* info = *(CPlayerInfo**)((BYTE*)this + 0x26 + (DWORD)id * 4);
    if (info)
        info->score = value;
}

void CPlayerPool::SetPing(WORD id, DWORD value)
{
    if (id == localId)
    {
        localPing = value;
        return;
    }
    if (id >= 1004)
        return;
    CPlayerInfo* info = *(CPlayerInfo**)((BYTE*)this + 0x26 + (DWORD)id * 4);
    if (info)
        info->ping = value;
}

// Does not use ECX or stack arguments.

DWORD GetCachedActivePlayerCount()
{
    CPlayerPool* pool = ((CNetGame*)g_pNetGame)->pools->players;
    if (g_activePlayerCountCountdown)
    {
        --g_activePlayerCountCountdown;
        return g_cachedActivePlayerCount;
    }
    g_activePlayerCountCountdown = 30;
    g_cachedActivePlayerCount = 0;
    if (!pool)
        return 0;
    int largest = pool->largestId;
    DWORD count = 0;
    for (WORD id = 0; (int)id <= largest; ++id)
    {
        if (id < 1004 && pool->connected[id])
        {
            CPlayerInfo* info = pool->players[id];
            CRemotePlayer* remote = info ? info->remote : 0;
            // The native body deliberately has no null-remote guard here.
            if (*(void**)((BYTE*)remote + 4) && remote->GetState())
                g_cachedActivePlayerCount = ++count;
        }
    }
    return count;
}
