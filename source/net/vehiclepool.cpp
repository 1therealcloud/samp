#include "net/vehiclepool.h"

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/game.h"
#include "game/object.h"
#include "game/util.h"
#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/playerpool.h"

#include <BitStream.h>
#include <RakClientInterface.h>

#include <string.h>
// 0x000e6468
const char g_vehiclePoolNotDeletedMessage[] = "Warning: vehicle %u was not deleted";
// R5 address to verify
int g_vehiclePoolDestroyRpc = 136;
// 0x000e6434
const char g_vehiclePoolQueueFullMessage[] = "All vehicle waiting slots are consumed!";

// R5 address to verify
void CVehiclePool::SetUnoccupiedSyncOwner(WORD vehicle, WORD player)
{
    *(WORD*)((BYTE*)this + 0x8e34 + vehicle * 2) = player;
}

// 0x0001ef30
WORD CVehiclePool::FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord)
{
    struct RayPoint
    {
        CVector point;
        DWORD word;
    };
    RayPoint from = {*origin, rayWord}, to = {*end, rayWord};
    WORD selected = 0xffff;
    float closest = 100000.0f;
    for (int i = 0; i <= m_nLargestId; ++i)
    {
        if (!m_bNotEmpty[i])
            continue;
        CVehicle* vehicle = m_pObjects[i];
        if (!vehicle || !vehicle->CEntity::DoesExist())
            continue;
        BYTE nearer;
        // clang-format off
        __asm {
            mov ecx,vehicle
            call CEntity::GetDistanceToCamera
            fcomp maxCameraDistance
            fnstsw ax
            test ah,5
            setnp nearer
        }
        // clang-format on
        if (!nearer || vehicle->HasPlayerPed()) continue;
        CVector centre;
        vehicle->GetBoundCentre(&centre);
        if (!GameTestEntityLineSphere(&from.point, &to.point, vehicle->m_pGameEntity, &centre, 1.0f, 1))
            continue;
        // clang-format off
        __asm {
            mov eax,origin
            push dword ptr [eax+8]
            push dword ptr [eax+4]
            push dword ptr [eax]
            mov ecx,vehicle
            call CEntity::GetDistanceToPoint
            fcom closest
            fnstsw ax
            test ah,5
            jp vehicle_ray_not_closer
            fstp closest
            mov nearer,1
            jmp vehicle_ray_distance_done
        vehicle_ray_not_closer:
            fstp st(0)
            mov nearer,0
        vehicle_ray_distance_done:
        }
        // clang-format on
        if (nearer) selected = (WORD)i;
    }
    return selected;
}
// 0x0001f3c0
void CVehiclePool::Process()
{
    DWORD now = GetTickCount();
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    int sent = 0;
    if (m_nLargestId < 0)
        return;
    WORD id = 0;
    do
    {
        if (id < 2000 && m_bNotEmpty[id] == 1)
        {
            CVehicle* vehicle = m_pObjects[id];
            if (stateBD14[id])
            {
                BYTE* state = (BYTE*)this;
                if (*(DWORD*)(state + 0xdc54 + id * 4) && (int)(now - *(DWORD*)(state + 0xfb94 + id * 4)) > 15000)
                {
                    Delete(id);
                    goto next_vehicle;
                }
                vehicle->SetInvulnerable(vehicle->DriverIsPlayerPed() ? FALSE : TRUE);
                int destroyed = 0;
                // clang-format off
                __asm {
                    mov ecx,vehicle
                    call CVehicle::GetHealth
                    fld dword ptr [g_objectScaleLowerBound]
                    fucompp
                    fnstsw ax
                    test ah,44h
                    jp health_not_zero
                    mov destroyed,1
                health_not_zero:
                }
                // clang-format on
                if (destroyed)
                {
                    SendDestroyNotification(id);
                    if (!*(DWORD*)(state + 0xdc54 + id * 4))
                    {
                        *(DWORD*)(state + 0xdc54 + id * 4) = 1;
                        *(DWORD*)(state + 0xfb94 + id * 4) = GetTickCount();
                    }
                    goto next_vehicle;
                }
                volatile float distance = vehicle->GetDistanceToLocalPlayer();
                if (vehicle->GetSubtype() != 4 && distance < g_vehicleMarkerDistance && vehicle->IsSunk())
                {
                    SendDestroyNotification(id);
                    goto next_vehicle;
                }
                if (!vehicle->HasPlayerPed() && (!vehicle->IsTrailer() || !vehicle->GetTractor()) && sent < 3 && (int)(now - lastTick[id]) > 100)
                {
                    if (local->ProcessUnoccupiedSync(id, vehicle))
                    {
                        lastTick[id] = GetTickCount();
                        ++sent;
                    }
                }
                BYTE* settings = *(BYTE**)((BYTE*)g_pNetGame + 0x3d5);
                if (settings[0x2b])
                {
                    vehicle->EnableEngine((int)vehicle->GetField50() > 0);
                    vehicle->EnableLights((BYTE)((int)vehicle->GetField54() > 0));
                }
                else
                {
                    if (vehicle->GetField50() == 0xffffffff)
                        vehicle->EnableEngine(vehicle->HasDriver() ? TRUE : FALSE);
                    else if (vehicle->GetField50() == 0)
                        vehicle->EnableEngine(FALSE);
                    else if (vehicle->GetField50() == 1)
                        vehicle->EnableEngine(TRUE);
                    if (vehicle->GetField54() == 0)
                        vehicle->EnableLights(0);
                    else if (vehicle->GetField54() == 1)
                        vehicle->EnableLights(1);
                }
                if (vehicle->m_pGameVehicle != m_pGameObjects[id])
                    m_pGameObjects[id] = vehicle->m_pGameVehicle;
                vehicle->UpdateColor();
                vehicle->ProcessMarkers();
            }
        }
    next_vehicle:
        ++id;
    }
    while ((int)id <= m_nLargestId);
}
// 0x0001f080
BOOL CVehiclePool::Create(const VehiclePoolQueueEntry* entry)
{
    if (m_pObjects[entry->id])
    {
        g_pChat->AddDebugMessage(g_vehiclePoolNotDeletedMessage, (unsigned)entry->id);
        Delete(entry->id);
    }
    m_pObjects[entry->id] = g_pGame->CreateVehicle(entry->model, entry->position.x, entry->position.y, entry->position.z, entry->rotation, entry->siren);
    if (!m_pObjects[entry->id])
        return FALSE;
    if (entry->primaryColor != -1 || entry->secondaryColor != -1)
        m_pObjects[entry->id]->SetColor((BYTE)entry->primaryColor, (BYTE)entry->secondaryColor);
    m_pObjects[entry->id]->SetHealth(entry->health);
    m_pGameObjects[entry->id] = m_pObjects[entry->id]->m_pGameVehicle;
    m_bNotEmpty[entry->id] = TRUE;
    BYTE* state = (BYTE*)this;
    *(float*)(state + 0x11ad4 + entry->id * 12) = entry->position.x;
    *(float*)(state + 0x11ad8 + entry->id * 12) = entry->position.y;
    *(float*)(state + 0x11adc + entry->id * 12) = entry->position.z;
    if (entry->interior && m_bNotEmpty[entry->id])
        m_pObjects[entry->id]->ChangeInterior(entry->interior);
    if (entry->panels || entry->doors || entry->lights)
        m_pObjects[entry->id]->UpdateDamage(entry->panels, entry->doors, entry->lights);
    m_pObjects[entry->id]->SetTires(entry->tires);
    stateBD14[entry->id] = 1;
    *(DWORD*)(state + 0xdc54 + entry->id * 4) = 0;
    *(DWORD*)(state + 0xfb94 + entry->id * 4) = 0;
    UpdateLargestId();
    return TRUE;
}

// 0x0001f300
void CVehiclePool::ProcessWaitingList()
{
    for (int i = 0; i < 100; ++i)
        if (queueActive[i] && g_pGame->IsModelAvailable(queue[i].model))
        {
            VehiclePoolQueueEntry* entry = queue + i;
            Create(entry);
            if (entry->model == 537)
            {
                ++entry->id;
                entry->model = 569;
                Create(entry);
                ++entry->id;
                Create(entry);
                ++entry->id;
                Create(entry);
            }
            if (entry->model == 538)
            {
                ++entry->id;
                entry->model = 570;
                Create(entry);
                ++entry->id;
                Create(entry);
                ++entry->id;
                Create(entry);
            }
            queueActive[i] = 0;
        }
}

// 0x0001f230
void CVehiclePool::SendDestroyNotification(WORD id)
{
    CLocalPlayer* local = ((CNetGame*)g_pNetGame)->pools->players->local;
    if (local->m_state.lastVehicle == id)
    {
        RakNet::BitStream stream;
        stream.Write(id);
        (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_vehiclePoolDestroyRpc, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
        local = ((CNetGame*)g_pNetGame)->pools->players->local;
        local->m_state.lastVehicle = 0xffff;
    }
}

// 0x0001e9b0
void CVehiclePool::UpdateLargestId()
{
    int largest = 0;
    for (int i = 0; i < 2000; i++)
        if (m_bNotEmpty[i])
            largest = i;
    m_nLargestId = largest;
}

// 0x0001ea10
CVehiclePool::CVehiclePool()
{
    DWORD(WINAPI * tick)() = GetTickCount;
    for (int i = 0; i < 2000; i++)
    {
        m_bNotEmpty[i] = FALSE;
        m_pObjects[i] = 0;
        m_pGameObjects[i] = 0;
        lastTick[i] = tick();
    }
    memset(queueActive, 0, sizeof(queueActive));
    m_nLargestId = 0;
    licensePlatesReady = 1;
}

// 0x0001ea80
BOOL CVehiclePool::Delete(WORD id)
{
    if (id >= 2000 || !m_bNotEmpty[id] || !m_pObjects[id])
        return FALSE;
    m_pObjects[id]->ShutdownLicensePlate();
    CVehicle* vehicle = m_pObjects[id];
    m_bNotEmpty[id] = FALSE;
    delete vehicle;
    m_pObjects[id] = 0;
    m_pGameObjects[id] = 0;
    UpdateLargestId();
    return TRUE;
}

// 0x0001f060
CVehiclePool::~CVehiclePool()
{
    for (WORD id = 0; id < 2000; id++)
        Delete(id);
}

// 0x0001eb00
void CVehiclePool::ChangeInterior(WORD id, int interior)
{
    if (m_bNotEmpty[id])
        m_pObjects[id]->ChangeInterior(interior);
}

// 0x0001eb30
void CVehiclePool::AssignSpecialParams(WORD id, BYTE objective, BYTE locked)
{
    if (id >= 2000 || !m_bNotEmpty[id])
        return;
    CVehicle* vehicle = m_pObjects[id];
    if (!vehicle || !stateBD14[id])
        return;
    if (objective)
    {
        vehicle->m_byteObjective = 1;
        vehicle->m_dwObjectiveMarker = 0;
    }
    vehicle->Lock(locked);
}

// 0x0001ec00
WORD CVehiclePool::FindNearestToLocalPlayer()
{
    WORD selected = 0xffff;
    float best = 10000.0f;
    if (m_nLargestId < 0)
        return selected;
    WORD id = 0;
    do
    {
        if (id < 2000 && m_bNotEmpty[id] && stateBD14[id])
        {
            CVehicle* vehicle = m_pObjects[id];
            int better = 0;
            // clang-format off
            __asm {
                mov ecx,vehicle
                call CEntity::GetDistanceToLocalPlayer
                fcom dword ptr [best]
                fnstsw ax
                test ah,5
                jp reject
                fstp dword ptr [best]
                mov better,1
                jmp compared
            reject:
                fstp st(0)
            compared:
            }
            // clang-format on
            if (better) selected = id;
        }
    }
    while ((int)(WORD)++id <= m_nLargestId);
    return selected;
}

// 0x0001ec70
WORD CVehiclePool::FindNearestToPoint(float x, float y, float z)
{
    WORD selected = 0xffff;
    float best = 20000.0f;
    if (m_nLargestId < 0)
        return selected;
    WORD id = 0;
    do
    {
        if (m_bNotEmpty[id] && stateBD14[id])
        {
            CVehicle* vehicle = m_pObjects[id];
            int better = 0;
            // clang-format off
            __asm {
                push z
                push y
                push x
                mov ecx,vehicle
                call CEntity::GetDistanceToPoint
                fcom dword ptr [best]
                fnstsw ax
                test ah,5
                jp reject
                fstp dword ptr [best]
                mov better,1
                jmp compared
            reject:
                fstp st(0)
            compared:
            }
            // clang-format on
            if (better) selected = id;
        }
    }
    while ((int)(WORD)++id <= m_nLargestId);
    return selected;
}
// 0x0001ed10
void CVehiclePool::Queue(const VehiclePoolQueueEntry* entry)
{
    int slot = 0;
    do
    {
        if (!queueActive[slot])
            break;
    }
    while (++slot != 100);
    if (slot == 100)
    {
        g_pChat->AddDebugMessage(g_vehiclePoolQueueFullMessage);
        return;
    }
    queueActive[slot] = 1;
    queue[slot] = *entry;
}

// 0x0001ebc0
DWORD CVehiclePool::GetRef(int id)
{
    return m_pGameObjects[id] ? GamePoolVehicleGetIndex(m_pGameObjects[id]) : 0xffff;
}

// 0x0001ebe0
DWORD CVehiclePool::GetRef(void* backend)
{
    return backend ? GamePoolVehicleGetIndex(backend) : 0xffff;
}

// 0x0001eb90
WORD CVehiclePool::Find(void* gameObject)
{
    for (int i = 1; i <= m_nLargestId; i++)
        if (gameObject == m_pGameObjects[i])
            return (WORD)i;
    return 0xffff;
}

// 0x00001120
CVehicle* CVehiclePool::Get(WORD id)
{
    return id < 2000 && m_bNotEmpty[id] ? m_pObjects[id] : 0;
}

// Native returns the raw state DWORD; do not normalize it to bool.
// 0x00001150
BOOL CVehiclePool::DoesExist(WORD id)
{
    if (id >= 2000)
        return FALSE;
    return m_bNotEmpty[id];
}

// 0x0001ee20
bool CVehiclePool::TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second)
{
    for (int index = 0; index <= m_nLargestId; index++)
    {
        if (!m_bNotEmpty[index])
            continue;
        CVehicle* vehicle = m_pObjects[index];
        if (!vehicle || !vehicle->CEntity::DoesExist())
            continue;
        if (!(vehicle->GetDistanceToCamera() < g_objectStreamingNearDistance) || vehicle->IsIgnored())
            continue;
        if (vehicle->TestCollisionShapes(first, second, 1))
            return true;
    }
    return false;
}

// 0x0001ed70
void CVehiclePool::ConstructLicensePlates()
{
    BYTE* state = (BYTE*)this;
    if (!*(DWORD*)(state + 0x17894))
        return;
    if (g_devicePlateManager && !((CLicensePlate*)g_devicePlateManager)->defaultTexture)
    {
        IDirect3DTexture9* created = ((CLicensePlate*)g_devicePlateManager)->Create("XYZSR998");
        ((CLicensePlate*)g_devicePlateManager)->defaultTexture = created;
    }
    DWORD counter = 0;
    if ((int)(WORD)counter <= m_nLargestId)
    {
        do
        {
            WORD slot = (WORD)counter;
            if (*(DWORD*)(state + 0x3074 + slot * 4))
                (*(CVehicle**)(state + 0x1134 + slot * 4))->ConstructLicensePlate();
        }
        while ((int)(WORD)++counter <= m_nLargestId);
    }
}

// 0x0001ede0
void CVehiclePool::ShutdownLicensePlates()
{
    BYTE* state = (BYTE*)this;
    DWORD counter = 0;
    if ((int)(WORD)counter <= m_nLargestId)
    {
        do
        {
            WORD slot = (WORD)counter;
            if (*(DWORD*)(state + 0x3074 + slot * 4))
                (*(CVehicle**)(state + 0x1134 + slot * 4))->ShutdownLicensePlate();
        }
        while ((int)(WORD)++counter <= m_nLargestId);
    }
}

// 0x0001eeb0
WORD CVehiclePool::FindInLine(DWORD unused, const CVector* origin, const CVector* end)
{
    BYTE collision[52];
    void* entity = 0;
    GameProcessLineOfSightRaw(origin, end, collision, &entity, 0, 1, 0, 0, 0, 0, 0, 0);
    if (!entity)
        return 0xffff;
    return Find(entity);
}
