#include "net/localplayer.h"
#include "main.h"
// 0x00017770
void CLocalPlayer::ReturnToClassSelection()
{
    m_state.wantsAnotherClass = 1;
}
// 0x0009e940
DWORD CLocalPlayer::GetRCModeState()
{
    return m_state.rcMode;
}
// 0x00008170
DWORD CLocalPlayer::GetActiveState()
{
    return m_state.active;
}
// 0x0000e2a0
CEntity* CLocalPlayer::GetSurfingEntity()
{
    return m_state.surfingEntity;
}
// 0x00017760
void CLocalPlayer::ClearSurfingEntityAndMode()
{
    m_state.surfingEntity = 0;
    m_state.surfingMode = 0;
}
// 0x00017780
void CLocalPlayer::SetTeam(BYTE team)
{
    m_state.team = team;
}
// 0x000a21d0
BYTE CLocalPlayer::GetTeam()
{
    return m_state.team;
}
#include "BitStream.h"
#include "RakClientInterface.h"
#include "chatwindow.h"
#include "cmdprocs.h"
#include "cmdwindow.h"
#include "game/actor.h"
#include "game/aimstuff.h"
#include "game/camera.h"
#include "game/custommodels.h"
#include "game/game.h"
#include "game/gta_globals.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/textdraw.h"
#include "game/util.h"
#include "gui.h"
#include "net/actorpool.h"
#include "net/objectpool.h"
#include "net/playerinfo.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/vehiclepool.h"
#include <stdio.h>
#include <string.h>
// VC7 runtime helper; library entry, not a recovered SA-MP method.
extern "C" __int64 __cdecl _ftol2();
// 0x00117944
const float g_localQuaternionSyncThreshold = 0.00001f;
// 0x00003390
BYTE CLocalPlayer::GetSpecialAction()
{
    if (GetPed()->HasJetpack())
        return 2;
    if (GetPed()->DoesDancing())
    {
        DWORD style = *(DWORD*)((BYTE*)GetPed() + 0x2d6);
        if (style <= 3)
            return (BYTE)(style + 5);
    }
    if (GetPed()->DoesHandsUp())
        return 10;
    if (GetPed()->UsingCellphone())
        return 11;
    if (GetPed()->DoesUrinating())
        return 68;
    if (GetPed()->IsDucking())
        return 1;
    if (GetPed()->GetVehicleTransitionTaskState())
        return 3;
    if (GetPed()->HasTaskType704())
        return 4;
    if (GetPed()->GetStuff())
    {
        if (GetPed()->GetStuff() == 1)
            return 20;
        if (GetPed()->GetStuff() == 2)
            return 22;
        if (GetPed()->GetStuff() == 3)
            return 23;
        if (GetPed()->GetStuff() == 4)
            return 21;
    }
    if (GetPed()->HasDanceState())
        return 24;
    return GetPed()->HasDanceStyle() ? 25 : 0;
}
// 0x00004db0
void CLocalPlayer::SendOnfootData()
{
    RakNet::BitStream stream;
    WORD leftRight, upDown;
    WORD keys = GetPed()->GetKeys(&leftRight, &upDown);
    BYTE aim = GetPed()->GetAim();
    CMatrix matrix;
    CVector speed;
    GetPed()->GetMatrix(&matrix);
    GetPed()->GetSpeed(&speed);
    LocalOnfootSyncData data;
    data.leftRight = leftRight;
    data.upDown = upDown;
    data.keys = keys;
    data.position = matrix.position;
    data.weaponAim = (BYTE)(aim << 6);
    GameQuaternionFromMatrixComponents(&matrix, &data.rotation);
    GameQuaternionNormalize(&data.rotation);
    float* current = (float*)&data.rotation;
    float* previous = (float*)&m_state.onfoot.rotation;
    int keepPrevious = 1;
    for (int i = 0; i < 4; ++i)
    {
        // Push the native float words unchanged, including exceptional bit patterns.
        DWORD first = ((DWORD*)current)[i], second = ((DWORD*)previous)[i];
        int close = 0;
        // clang-format off
        __asm {
            push second
            push first
            call FloatOffset
            add esp,8
            fcomp dword ptr [g_localQuaternionSyncThreshold]
            fnstsw ax
            test ah,5
            jp onfoot_component_different
            mov close,1
        onfoot_component_different:
        }
        // clang-format on
        if (!close)
        {
            keepPrevious = 0;
            break;
        }
    }
    if (keepPrevious)
        memcpy(&data.rotation, previous, 16);
    CLocalPlayer* self = this;
    DWORD health, armour;
    // clang-format off
    __asm {
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetHealth
        call _ftol2
        mov health,eax
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetArmour
        call _ftol2
        mov armour,eax
    }
    // clang-format on
    data.health = (BYTE)health;
    data.armour = (BYTE)armour;
    data.weaponAim |= (BYTE)(GetPed()->GetCurrentWeapon() & 0x3f);
    data.specialAction = GetSpecialAction();
    data.speed = speed;
    if (m_state.surfingMode)
    {
        memcpy(&data.surfingOffset, &m_state.surfingOffset, 12);
        data.surfingVehicle = m_state.surfingId;
    }
    else
    {
        data.surfingOffset.x = 0;
        data.surfingOffset.y = 0;
        data.surfingOffset.z = 0;
        data.surfingVehicle = 0;
    }
    data.animation = m_state.animation;
    m_state.animationSent = 1;
    if (HasDataChanged(&m_state.onfoot, &data, 68))
    {
        stream.Write((BYTE)207);
        stream.Write((char*)&data, 68);
        ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);
        memcpy(&m_state.onfoot, &data, 68);
    }
}
// 0x00006e80
void CLocalPlayer::SendIncarData()
{
    RakNet::BitStream stream;
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    WORD leftRight, upDown;
    WORD keys = GetPed()->GetKeys(&leftRight, &upDown);
    BYTE aim = GetPed()->GetAim();
    LocalIncarSyncData data = {0};
    if (!GetPed())
        return;
    data.vehicle = vehicles->Find(GetPed()->GetVehicle());
    if (data.vehicle == 0xffff)
        return;
    data.leftRight = leftRight;
    data.upDown = upDown;
    data.keys = keys;
    data.weaponAim = (BYTE)(aim << 6);
    CVehicle* vehicle = vehicles->Get(data.vehicle);
    if (!vehicle)
        return;
    CMatrix matrix;
    CVector speed;
    GameQuaternion rotation;
    vehicle->GetMatrix(&matrix);
    vehicle->GetSpeed(&speed);
    GameQuaternionFromMatrixComponents(&matrix, &rotation);
    GameQuaternionNormalize(&rotation);
    data.rotation = rotation;
    float* current = (float*)&data.rotation;
    float* previous = (float*)&m_state.incar.rotation;
    int keepPrevious = 1;
    for (int i = 0; i < 4; ++i)
    {
        // Push the native float words unchanged, including exceptional bit patterns.
        DWORD first = ((DWORD*)current)[i], second = ((DWORD*)previous)[i];
        int close = 0;
        // clang-format off
        __asm {
            push second
            push first
            call FloatOffset
            add esp,8
            fcomp dword ptr [g_localQuaternionSyncThreshold]
            fnstsw ax
            test ah,5
            jp incar_component_different
            mov close,1
        incar_component_different:
        }
        // clang-format on
        if (!close)
        {
            keepPrevious = 0;
            break;
        }
    }
    if (keepPrevious)
        memcpy(&data.rotation, previous, 16);
    data.position = matrix.position;
    data.speed = speed;
    data.health = vehicle->GetHealth();
    CLocalPlayer* self = this;
    DWORD health, armour;
    // clang-format off
    __asm {
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetHealth
        call _ftol2
        mov health,eax
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetArmour
        call _ftol2
        mov armour,eax
    }
    // clang-format on
    data.pedHealth = (BYTE)health;
    data.armour = (BYTE)armour;
    data.weaponAim |= (BYTE)(GetPed()->GetCurrentWeapon() & 0x3f);
    data.trailer = 0;
    CVehicle* trailer = vehicle->GetTrailer();
    if (trailer && trailer->m_pGameVehicle && trailer->GetTractor() == vehicle)
    {
        vehicle->SetTrailer(trailer);
        data.trailer = vehicles->Find(trailer->m_pGameVehicle);
    }
    else
        vehicle->SetTrailer(0);
    if (vehicle->GetModelIndex() == 520)
        data.modelData = vehicle->GetHydraThrusters();
    else if (vehicle->GetModelIndex() == 538 || vehicle->GetModelIndex() == 537 || vehicle->GetModelIndex() == 449)
        *(float*)&data.modelData = vehicle->GetTrainSpeed();
    else if (vehicle->GetSubtype() == 2 || vehicle->GetSubtype() == 6)
        *(float*)&data.modelData = vehicle->GetBackendFloat648();
    else
        data.modelData = 0;
    data.siren = vehicle->SirenEnabled() != 0;
    data.landingGear = vehicle->GetLandingGearState() != 0;
    if (HasDataChanged(&m_state.incar, &data, 63))
    {
        stream.Write((BYTE)200);
        stream.Write((char*)&data, 63);
        ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);
        memcpy(&m_state.incar, &data, 63);
    }
    if (data.trailer && data.trailer < 2000)
        SendTrailerData(data.trailer);
    if (vehicle->HasTurret() || GetTickCount() - m_state.lastAimSendTick > 1000)
    {
        m_state.lastAimSendTick = GetTickCount();
        SendAimData();
    }
}
// 0x00117948
const float g_localAimZoomScale = 63.0f;
// 0x0011794c
const float g_localAimAspectScale = 255.0f;
// 0x00117950
int g_localWastedRpc = 53;
// 0x000041c0
DWORD CLocalPlayer::NeedsToSendOnfootData(WORD keys, WORD upDown, WORD leftRight)
{
    return keys != m_state.onfoot.keys || upDown != m_state.onfoot.upDown || leftRight != m_state.onfoot.leftRight ? 1 : 0;
}
// 0x000041f0
DWORD CLocalPlayer::NeedsToSendIncarData(WORD keys, WORD upDown, WORD leftRight)
{
    return keys != m_state.incar.keys || upDown != m_state.incar.upDown || leftRight != m_state.incar.leftRight ? 1 : 0;
}
// 0x00005090
void CLocalPlayer::SendAimData()
{
    RakNet::BitStream stream;
    CameraAim* aim = GetPed()->GetCameraAim();
    LocalAimSyncData data;
    BYTE player = *((BYTE*)GetPed() + 0x2b0);
    data.cameraMode = player ? AimStuff::GetPlayerCameraMode(player) : AimStuff::GetCameraMode();
    memcpy(data.vectors, aim, 24);
    data.aimZ = GetPed()->GetAimZ();
    float zoom = AimStuff::GetCameraExtZoom();
    DWORD aspectValue, zoomValue;
    // clang-format off
    __asm {
        call AimStuff::GetAspectRatio
        fmul dword ptr [g_localAimAspectScale]
        call _ftol2
        mov aspectValue,eax
        fld dword ptr [zoom]
        fmul dword ptr [g_localAimZoomScale]
        call _ftol2
        mov zoomValue,eax
    }
    // clang-format on
    data.aspect = (BYTE)aspectValue;
    data.zoomWeapon = (BYTE)(zoomValue & 0x3f);
    DWORD* slot = (DWORD*)GetPed()->GetCurrentWeaponSlot();
    DWORD weapon;
    if (slot[1] == 2)
        weapon = 3;
    else
    {
        weapon = slot[2];
        if (weapon > 1)
            weapon = 2;
    }
    data.zoomWeapon |= (BYTE)(weapon << 6);
    if (HasDataChanged(&m_state.aim, &data, 31))
    {
        stream.Write((BYTE)203);
        stream.Write((char*)&data, 31);
        ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);
        memcpy(&m_state.aim, &data, 31);
    }
}
// 0x00005240
void CLocalPlayer::SendTrailerData(WORD id)
{
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (id >= 2000 || !vehicles->m_bNotEmpty[id])
        return;
    CVehicle* vehicle = vehicles->m_pObjects[id];
    if (!vehicle)
        return;
    CMatrix matrix;
    GameQuaternion rotation;
    vehicle->GetMatrix(&matrix);
    GameQuaternionFromMatrixComponents(&matrix, &rotation);
    GameQuaternionNormalize(&rotation);
    LocalTrailerSyncData data;
    data.rotation = rotation;
    data.position = matrix.position;
    data.vehicle = id;
    vehicle->GetSpeed(&data.speed);
    vehicle->GetTurnSpeed(&data.turnSpeed);
    if (HasDataChanged(&m_state.trailer, &data, 54))
    {
        RakNet::BitStream stream;
        stream.Write((BYTE)210);
        stream.Write((char*)&data, 54);
        ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);
        memcpy(&m_state.trailer, &data, 54);
    }
}
// 0x00005400
void CLocalPlayer::SendPassengerData()
{
    RakNet::BitStream stream;
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    WORD leftRight, upDown;
    WORD keys = GetPed()->GetKeys(&leftRight, &upDown);
    BYTE aim = GetPed()->GetAim();
    LocalPassengerSyncData data = {0};
    data.vehicle = vehicles->Find(GetPed()->GetVehicle());
    if (data.vehicle == 0xffff)
        return;
    data.weaponAim = (BYTE)(aim << 6);
    data.leftRight = leftRight;
    data.upDown = upDown;
    data.keys = keys;
    CLocalPlayer* self = this;
    DWORD health, armour;
    // clang-format off
    __asm {
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetHealth
        call _ftol2
        mov health,eax
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetArmour
        call _ftol2
        mov armour,eax
    }
    // clang-format on
    data.health = (BYTE)health;
    data.armour = (BYTE)armour;
    data.seatFlags = (BYTE)(GetPed()->GetVehicleSeatIndex() & 0x3f);
    if (GetPed()->HasDanceState())
        data.seatFlags |= 0x80;
    data.seatFlags |= (BYTE)((*(BYTE*)&m_state.passengerDriveby & 1) << 6);
    data.weaponAim |= (BYTE)(GetPed()->GetCurrentWeapon() & 0x3f);
    CMatrix matrix;
    GetPed()->GetMatrix(&matrix);
    data.position = matrix.position;
    if (HasDataChanged(&m_state.passenger, &data, 24))
    {
        stream.Write((BYTE)211);
        stream.Write((char*)&data, 24);
        ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);
        memcpy(&m_state.passenger, &data, 24);
    }
    DWORD delay = m_state.passengerDriveby ? 100 : 1000;
    if (GetTickCount() - m_state.lastAimSendTick > delay)
    {
        m_state.lastAimSendTick = GetTickCount();
        SendAimData();
    }
}
// 0x00005660
void CLocalPlayer::WastedNotification()
{
    RakNet::BitStream stream;
    WORD killer;
    BYTE reason = GetPed()->GetDeathInfo(&killer);
    stream.Write(reason);
    stream.Write(killer);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localWastedRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}
// 0x00005b50
void CLocalPlayer::SendStats()
{
    RakNet::BitStream stream;
    int money = g_pGame->GetPlayerMoney();
    DWORD drunk = GetPed()->GetDrunkLevel();
    stream.Write((BYTE)205);
    stream.Write(money);
    stream.Write(drunk);
    ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE, 0);
}
// 0x00117978
const float g_localUnoccupiedRange = 90.0f;
// 0x00004bd0
void CLocalPlayer::SendUnoccupiedData(WORD id, BYTE seat)
{
    CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
    CPlayerPool* players = pools->players;
    CVehiclePool* vehicles = pools->vehicles;
    if (id >= 2000 || !vehicles->m_bNotEmpty[id])
        return;
    CVehicle* vehicle = vehicles->m_pObjects[id];
    RakNet::BitStream stream;
    CMatrix matrix;
    CVector speed, turnSpeed;
    vehicle->GetMatrix(&matrix);
    vehicle->GetSpeed(&speed);
    vehicle->GetTurnSpeed(&turnSpeed);
    UnoccupiedSyncData data;
    data.right = matrix.right;
    data.up = matrix.up;
    data.position = matrix.position;
    data.speed = speed;
    data.turnSpeed = turnSpeed;
    data.vehicle = id;
    data.seat = seat;
    data.health = vehicle->GetHealth();
    stream.Write((BYTE)209);
    stream.Write((char*)&data, 67);
    ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1);
    *(WORD*)((BYTE*)vehicles + 0x8e34 + id * 2) = players->localId;
    vehicle->m_dwLocalUnoccupiedSync = 1;
}
// 0x00006c10
bool CLocalPlayer::ProcessUnoccupiedSync(WORD id, CVehicle* vehicle)
{
    GetTickCount();
    BYTE* backend = (BYTE*)vehicle->m_pGameVehicle;
    CPlayerPool* players = ((CNetGame*)g_pNetGame)->pools->players;
    if (!backend || !m_state.ped || vehicle->IsTrainPart())
        return false;
    BYTE* driver = *(BYTE**)(backend + 0x460);
    bool clear = driver && (*(DWORD*)(driver + 0x46c) & 0x100);
    if (!clear)
    {
        int outsideRange = 0;
        // clang-format off
        __asm {
            mov ecx,vehicle
            call CEntity::GetDistanceToLocalPlayer
            fcomp dword ptr [g_localUnoccupiedRange]
            fnstsw ax
            test ah,41h
            jne distance_accepted
            mov outsideRange,1
        distance_accepted:
        }
        // clang-format on
        clear = outsideRange || vehicle->IsStationary() || vehicle->GetTractor() != 0;
    }
    if (clear)
    {
        vehicle->m_dwLocalUnoccupiedSync = 0;
        vehicle->m_dwRemoteUnoccupiedSync = 0;
        return false;
    }
    for (int seat = 0; seat < 8; ++seat)
    {
        BYTE* passenger = *(BYTE**)(backend + 0x464 + seat * 4);
        if (passenger && !*(DWORD*)(passenger + 0x598))
        {
            if (passenger == *(BYTE**)((BYTE*)m_state.ped + 0x2a4))
            {
                SendUnoccupiedData(id, (BYTE)(seat + 1));
                return true;
            }
            return false;
        }
    }
    int closest = 0, largest = players->largestId;
    if (largest < players->localId)
        largest = players->localId;
    float best = 100000.0f;
    for (int index = 0; index <= largest; ++index)
    {
        CPed* ped;
        if (index == players->localId)
            ped = m_state.ped;
        else
        {
            WORD player = (WORD)index;
            if (player > 1004 || !players->players[player] || !players->players[player]->remote)
                continue;
            ped = players->players[player]->remote->m_state.ped;
        }
        if (!ped || !ped->DoesExist())
            continue;
        int better = 0;
        // clang-format off
        __asm {
            push vehicle
            mov ecx,ped
            call CPed::GetDistanceToEntity
            cmp index,0
            je seed_distance
            fcom dword ptr [best]
            fnstsw ax
            test ah,5
            jp reject_distance
            fstp dword ptr [best]
            mov better,1
            jmp distance_done
        seed_distance:
            fstp dword ptr [best]
            jmp distance_done
        reject_distance:
            fstp st(0)
        distance_done:
        }
        // clang-format on
        if (better) closest = index;
    }
    if (closest == players->localId && best <= g_localUnoccupiedRange)
    {
        SendUnoccupiedData(id, 0);
        return true;
    }
    return false;
}
// 0x0000e2b0
void CLocalPlayer::ClearSurfingState()
{
    m_state.surfingMode = 0;
    SetSurfingEntity(0, 0);
}
// 0x00003fb0
void CLocalPlayer::PrepareForClassSelection_Outcome(BOOL outcome)
{
    if (outcome)
    {
        CPed* ped = m_state.ped;
        if (ped)
        {
            ped->ClearWeapons();
            m_state.ped->SetModelIndex(m_state.spawnInfo.model);
        }
        m_state.spawnPermission = 1;
    }
}
// 0x00003af0
void CLocalPlayer::SetSpawnInfo(const void* info)
{
    const BYTE* data = (const BYTE*)info;
    for (int i = 0; i < 48; i += 4)
        *(DWORD*)((BYTE*)&m_state.spawnInfo + i) = *(const DWORD*)(data + i);
    *(WORD*)((BYTE*)&m_state.spawnInfo + 48) = *(const WORD*)(data + 48);
    m_state.hasSpawnInfo = 1;
}
// 0x00117880
int g_localSpawnRpc = 52;
// 0x0013c0a0
DWORD g_localSkipInitialState = 1;
// 0x00003110
void CLocalPlayer::SetSpecialAction(BYTE action)
{
    switch (action)
    {
        case 0:
            if (GetPed()->DoesDancing())
                GetPed()->StopDancing();
            if (GetPed()->UsingCellphone())
                GetPed()->EnableCellphone(FALSE);
            if (GetPed()->DoesUrinating())
                GetPed()->StopUrinating();
            if (GetPed()->GetStuff())
                GetPed()->DropStuff();
            if (GetPed()->HasJetpack())
                GetPed()->DisableJetpack();
            if (GetPed()->HasDanceState())
                GetPed()->SetDancePoseState(0);
            if (GetPed()->HasDanceStyle())
                GetPed()->SetSecondaryPoseState(0);
            break;
        case 2:
            if (!GetPed()->HasJetpack())
                GetPed()->EnableJetpack();
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            GetPed()->StartDancing(action - 5);
            break;
        case 10:
            GetPed()->HandsUp();
            break;
        case 11:
            if (!GetPed()->IsInVehicle())
                GetPed()->EnableCellphone(TRUE);
            break;
        case 13:
            if (GetPed()->UsingCellphone())
                GetPed()->EnableCellphone(FALSE);
            break;
        case 20:
            if (!GetPed()->IsInVehicle())
                GetPed()->GiveStuff(1);
            break;
        case 21:
            if (!GetPed()->IsInVehicle())
                GetPed()->GiveStuff(4);
            break;
        case 22:
            if (!GetPed()->IsInVehicle())
                GetPed()->GiveStuff(2);
            break;
        case 23:
            if (!GetPed()->IsInVehicle())
                GetPed()->GiveStuff(3);
            break;
        case 24:
            if (!GetPed()->HasDanceState())
                GetPed()->SetDancePoseState(1);
            break;
        case 25:
            if (!GetPed()->HasDanceStyle())
                GetPed()->SetSecondaryPoseState(1);
            break;
        case 68:
            if (!GetPed()->IsInVehicle())
                GetPed()->StartUrinating();
            break;
    }
}
// 0x00003b20
BOOL CLocalPlayer::Spawn()
{
    if (!m_state.hasSpawnInfo)
        return FALSE;
    if (m_state.rawFlag30a == 1)
    {
        m_state.rawFlag30a = 0;
        if (g_renderSecondDialog)
            *((BYTE*)g_renderSecondDialog + 0x13) = 0;
        g_pGame->SetCursorMode(0, FALSE);
    }
    CCamera* camera = g_pGame->m_pCamera;
    camera->Detach();
    camera->Restore();
    g_pGame->EnableHUD(TRUE);
    GetPed()->SetControllable(TRUE);
    if (!g_localSkipInitialState)
        GetPed()->SetInitialState();
    else
        g_localSkipInitialState = 0;
    g_pGame->RefreshRenderer(m_state.spawnInfo.position.x, m_state.spawnInfo.position.y);
    if (GetPed()->HasDanceState())
        GetPed()->SetDancePoseState(0);
    GetPed()->SetSpawnInfo(&m_state.spawnInfo.position, m_state.spawnInfo.rotation);
    if (g_customModels && m_state.spawnInfo.customModel > 20000)
    {
        if (GameIsPedModel(m_state.spawnInfo.customModel))
            GetPed()->SetModelIndex(m_state.spawnInfo.customModel);
        else if (g_customModels->DoesExist(m_state.spawnInfo.customModel))
            GetPed()->SetModelIndex(m_state.spawnInfo.customModel);
        else
            GetPed()->SetModelIndex(0);
    }
    else
        GetPed()->SetModelIndex(m_state.spawnInfo.model);
    GetPed()->ClearWeapons();
    GetPed()->ResetDamageEntity();
    SetSpecialAction(0);
    int weapon = m_state.spawnInfo.weapons[2];
    m_state.surfingMode = 0;
    *(DWORD*)&m_state.surfingOffset = 0;
    *(DWORD*)&m_state.surfingOffset.y = 0;
    *(DWORD*)&m_state.surfingOffset.z = 0;
    m_state.surfingId = 0xffff;
    m_state.surfingEntity = 0;
    m_state.surfingLocked = 0;
    if (weapon != -1)
        GetPed()->GiveWeapon(weapon, m_state.spawnInfo.ammo[2]);
    weapon = m_state.spawnInfo.weapons[1];
    if (weapon != -1)
        GetPed()->GiveWeapon(weapon, m_state.spawnInfo.ammo[1]);
    weapon = m_state.spawnInfo.weapons[0];
    if (weapon != -1)
        GetPed()->GiveWeapon(weapon, m_state.spawnInfo.ammo[0]);
    g_pGame->DisableTrainTraffic();
    GameUnprotect((void*)0x50ac20, 3);
    // GTA SA virtual addresses: native three-byte permanent RET8 patch.
    *(BYTE*)0x50ac20 = 0xc2;
    *(BYTE*)0x50ac21 = 8;
    *(BYTE*)0x50ac22 = 0;
    CVector position = {m_state.spawnInfo.position.x, m_state.spawnInfo.position.y, m_state.spawnInfo.position.z + g_textDrawHalf};
    GetPed()->Teleport(position);
    GetPed()->SetRotation(m_state.spawnInfo.rotation);
    m_state.wasted = 0;
    m_state.active = 1;
    m_state.waitingForSpawnReply = 0;
    RakNet::BitStream stream;
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localSpawnRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
    m_state.zoneDisplayTick = GetTickCount() + 1000;
    return TRUE;
}
// 0x00004080
void CLocalPlayer::ToggleSpectating(BOOL enabled)
{
    if (m_state.spectating && !m_state.wantsAnotherClass)
    {
        if (!enabled)
            Spawn();
    }
    else if (m_state.wantsAnotherClass && !enabled)
    {
        m_state.active = 0;
        m_state.hasSpawnInfo = 0;
    }
    m_state.spectating = enabled;
    m_state.spectateType = 0;
    m_state.spectateProcessed = 0;
    m_state.spectateId = 0xffffffff;
}
// 0x000040f0
void CLocalPlayer::SpectateVehicle(WORD id)
{
    CVehiclePool* pool = *(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
    if (pool && id < 2000 && pool->m_bNotEmpty[id])
    {
        m_state.spectateType = 2;
        m_state.spectateId = id;
        m_state.spectateProcessed = 0;
    }
}
// 0x00004140
void CLocalPlayer::SpectatePlayer(WORD id)
{
    CPlayerPool* pool = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    if (!pool || id >= 1004 || !pool->connected[id])
        return;
    CRemotePlayer* remote = pool->GetPlayer(id);
    BYTE state = *((BYTE*)remote + 0x1a);
    if (state && *((BYTE*)pool->GetPlayer(id) + 0x1a) != 32)
    {
        m_state.spectateType = 1;
        m_state.spectateId = id;
        m_state.spectateProcessed = 0;
    }
}
// 0x00003de0
void CLocalPlayer::SetColor(DWORD color)
{
    CPlayerPool* pool = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    PlayerColors::Set(*(WORD*)pool, color);
}
// 0x00003e00
DWORD CLocalPlayer::GetColorAsRGBA()
{
    CPlayerPool* pool = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    return PlayerColors::Get(*(WORD*)pool);
}
// 0x00002ec0
void CLocalPlayer::ResetData()
{
    m_state.hasSpawnInfo = 0;
    m_state.wantsAnotherClass = 0;
    m_state.waitingForSpawnReply = 0;
    *(DWORD*)&m_state.selectedClass = 0;
    m_state.interior = 0;
    m_state.lastVehicle = 0xffff;
    m_state.rcMode = 0;
    m_state.damageVehicle = 0xffff;
    m_state.cameraTargets[0] = 0xffff;
    m_state.cameraTargets[1] = 0xffff;
    m_state.cameraTargets[2] = 0xffff;
    memset(&m_state.headDirection, 0, 12);
    memset(&m_state.aim, 0, 31);
    memset(&m_state.trailer, 0, 54);
    memset(&m_state.onfoot, 0, 68);
    memset(&m_state.incar, 0, 63);
    DWORD(WINAPI * tick)() = GetTickCount;
    memset(&m_state.passenger, 0, 24);
    m_state.animation = 0;
    m_state.animationSent = 0;
    m_state.aimedPlayer = 0xffff;
    m_state.aimedActor = 0xffff;
    m_state.weaponsUpdateTick = tick();
    m_state.currentWeapon = 0;
    DWORD last = tick();
    m_state.surfingEntity = 0;
    m_state.surfingLocked = 0;
    m_state.cameraTargetTick = last;
}
// 0x00004af0
CLocalPlayer::CLocalPlayer()
{
    DWORD(WINAPI * tick)() = GetTickCount;
    m_state.classSelectionTick = tick();
    m_state.headUpdateTick = tick();
    m_state.headTaskTick = tick();
    m_state.rcMode = 0;
    m_state.surfingMode = 0;
    m_state.surfingTick = tick();
    m_state.rawSurfState2fa = 0;
    m_state.ped = g_pGame->GetPlayerPed();
    m_state.active = 0;
    m_state.wasted = 0;
    m_state.unknown131 = 0;
    m_state.zoneDisplayTick = 0;
    m_state.lastSendTick = tick();
    m_state.lastSpectatorSendTick = tick();
    m_state.lastChangedDataTick = tick();
    m_state.lastAimSendTick = m_state.lastSendTick;
    m_state.lastStatsSendTick = m_state.lastSendTick;
    m_state.spectating = 0;
    m_state.spectateType = 0;
    m_state.spectateId = 0xffffffff;
    m_state.rawFlag30a = 0;
    ResetData();
    for (int slot = 0; slot < 13; slot++)
    {
        m_state.lastWeapons[slot] = 0;
        m_state.lastAmmo[slot] = 0;
    }
    m_state.team = 0xff;
}
// 0x00002da0
CPed* CLocalPlayer::GetPed()
{
    return m_state.ped;
}
// 0x00003e20
DWORD CLocalPlayer::GetColorAsARGB()
{
    CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    return (PlayerColors::Get(*(WORD*)players) >> 8) | 0xff000000;
}
// The selected RakNet dependency uses SA-MP's shifted reliability enumeration.
typedef char LocalRpcReliability[(HIGH_PRIORITY == 1 && RELIABLE_ORDERED == 9 && RELIABLE_SEQUENCED == 10) ? 1 : -1];
// 0x0011787c
int g_localSpawnRequestRpc = 129;
// 0x00117954
int g_localRequestClassRpc = 128;
// 0x00117958
int g_localInteriorChangeRpc = 118;
// 0x0011795c
int g_localChatRpc = 101;
// 0x00003970
DWORD CLocalPlayer::HasDataChanged(const void* previous, const void* current, DWORD length)
{
    if (GetTickCount() - m_state.lastChangedDataTick > 500 || memcmp(previous, current, length))
    {
        m_state.lastChangedDataTick = GetTickCount();
        return 1;
    }
    return 0;
}
// 0x00003650
void CLocalPlayer::ProcessSurfing()
{
    void* floor = m_state.ped->GetFloor();
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    CVehiclePool* vehicles = *(CVehiclePool**)(pools + 0xc);
    CObjectPool* objects = *(CObjectPool**)(pools + 0x14);
    WORD vehicleId = 0xffff, objectId = 0xffff;
    int mode = 0;
    if (floor)
    {
        vehicleId = vehicles->Find(floor);
        if (vehicleId && vehicleId != 0xffff)
        {
            if (vehicleId < 2000 && vehicles->m_bNotEmpty[vehicleId])
            {
                CVehicle* vehicle = vehicles->m_pObjects[vehicleId];
                if (vehicle && (vehicle->HasDriver() || vehicle->GetModelIndex() == 569 || vehicle->GetModelIndex() == 570))
                {
                    BYTE nearby;
                    // clang-format off
                    __asm {
                        mov ecx,vehicle
                        call CEntity::GetDistanceToLocalPlayer
                        fcomp g_objectStreamingVehicleMargin
                        fnstsw ax
                        test ah,5
                        setnp nearby
                    }
                    // clang-format on
                    if (nearby) mode = 1;
                }
            }
        }
        else
        {
            objectId = objects->GetId(floor);
            if (objectId && objectId != 0xffff)
            {
                CObject* object = (CObject*)objects->Get(objectId);
                if (object && object->IsMoving())
                    mode = 2;
            }
        }
    }
    if (m_state.ped->HasTaskType208() || m_state.ped->HasTaskType704() || m_state.ped->GetVehicleTransitionTaskState())
    {
        m_state.rawSurfState2fa = 0;
        m_state.surfingEntity = 0;
        m_state.surfingLocked = 0;
        return;
    }
    if (!mode)
    {
        if (!m_state.surfingMode)
        {
            m_state.rawSurfState2fa = 0;
            m_state.surfingEntity = 0;
            m_state.surfingLocked = 0;
            return;
        }
        if (m_state.surfingMode == 1 && GetTickCount() - m_state.surfingTick >= 2000)
        {
            m_state.surfingMode = 0;
            memset(&m_state.surfingOffset, 0, 12);
            m_state.surfingId = 0xffff;
            m_state.surfingEntity = 0;
            m_state.surfingLocked = 0;
            m_state.rawSurfState2fa = 0;
        }
        if (m_state.surfingMode == 1 && GetTickCount() - m_state.surfingTick < 2000)
        {
            WORD id = m_state.surfingId;
            CEntity* entity = id < 2000 ? (CEntity*)vehicles->Get(id) : objects->Get((WORD)(id - 2000));
            if (entity)
            {
                m_state.surfingEntity = entity;
                m_state.surfingLocked = 1;
            }
            m_state.rawSurfState2fa = 1;
        }
        else
            m_state.rawSurfState2fa = 0xffffffff;
        return;
    }
    if (mode == 1)
    {
        m_state.surfingId = vehicleId;
        m_state.surfingTick = GetTickCount();
        CEntity* entity = vehicles->Get(m_state.surfingId);
        if (!m_state.surfingMode)
        {
            m_state.surfingEntity = entity;
            m_state.surfingLocked = 0;
            ApplySurfingPhysics();
        }
        m_state.surfingEntity = entity;
        m_state.surfingMode = 1;
        m_state.surfingLocked = 1;
    }
    else if (mode == 2)
    {
        m_state.surfingId = (WORD)(objectId + 2000);
        m_state.surfingTick = GetTickCount();
        CEntity* entity = objects->Get((WORD)(m_state.surfingId - 2000));
        if (entity)
        {
            if (!m_state.surfingMode)
            {
                m_state.surfingEntity = entity;
                m_state.surfingLocked = 0;
                ApplySurfingPhysics();
            }
            m_state.surfingEntity = entity;
            m_state.surfingMode = 1;
            m_state.surfingLocked = 1;
        }
    }
    m_state.rawSurfState2fa = 2;
}
// 0x00003a70
void CLocalPlayer::SendSpawnRequest()
{
    RakNet::BitStream stream;
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localSpawnRequestRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
}
// 0x00003f40
void CLocalPlayer::RequestSpawn()
{
    if (m_state.rawFlag30a)
    {
        SendSpawnRequest();
        m_state.waitingForSpawnReply = 1;
    }
}
// 0x00005720
void CLocalPlayer::RequestClass(int selection)
{
    RakNet::BitStream stream;
    stream.WriteBits((const unsigned char*)&selection, 32, true);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localRequestClassRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
}
// 0x000057c0
void CLocalPlayer::SendInteriorChange(BYTE interior)
{
    ((BYTE*)this)[0x1d3] = interior;
    RakNet::BitStream stream;
    stream.WriteBits(&interior, 8, true);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localInteriorChangeRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
}
// 0x00005860
void CLocalPlayer::Chat(char* text)
{
    BYTE length = (BYTE)strlen(text);
    RakNet::BitStream stream;
    stream.WriteBits(&length, 8, true);
    stream.Write(text, length);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localChatRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
}
// 0x00005e30
void CLocalPlayer::NextClass()
{
    if (!m_state.rawFlag30a)
        return;
    m_state.spawnPermission = 0;
    CMatrix matrix;
    m_state.ped->GetMatrix(&matrix);
    CNetGameSettings* settings = ((CNetGame*)g_pNetGame)->settings;
    DWORD selection = *(DWORD*)&m_state.selectedClass;
    if (selection == settings->spawnsAvailable - 1)
        selection = 0;
    else
        selection++;
    *(DWORD*)&m_state.selectedClass = selection;
    g_pGame->m_pAudio->Play(1052, matrix.position);
    RequestClass(m_state.selectedClass);
}
// 0x00005ec0
void CLocalPlayer::PrevClass()
{
    if (!m_state.rawFlag30a)
        return;
    m_state.spawnPermission = 0;
    CMatrix matrix;
    m_state.ped->GetMatrix(&matrix);
    DWORD selection = *(DWORD*)&m_state.selectedClass;
    if (!selection)
        selection = ((CNetGame*)g_pNetGame)->settings->spawnsAvailable;
    *(DWORD*)&m_state.selectedClass = selection - 1;
    g_pGame->m_pAudio->Play(1053, matrix.position);
    RequestClass(m_state.selectedClass);
}
// 0x00117960
int g_localEnterVehicleRpc = 26;
// 0x00117964
int g_localExitVehicleRpc = 154;
// 0x0011796c
int g_localPlayerDamageRpc = 115;
// 0x00117974
int g_localActorDamageRpc = 177;
// 0x00117970
const float g_localDamageScale = 0.33f;
// 0x00117840
const Scripting::OpcodeInfo g_localTrainCameraCommand = {0x158, "iii"};
// 0x0013c0a4
DWORD g_localTrainCameraTick = 0xffffffff;
// 0x00005920
void CLocalPlayer::EnterVehicle(WORD vehicle, BOOL passenger)
{
    RakNet::BitStream stream;
    stream.WriteBits((const unsigned char*)&vehicle, 16, true);
    BYTE flag = passenger != 0;
    stream.WriteBits(&flag, 8, true);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localEnterVehicleRpc, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
    // Native looks up the vehicle only after sending, even for invalid IDs.
    CVehiclePool* vehicles = *(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
    if (vehicle < 2000 && vehicles->m_bNotEmpty[vehicle])
    {
        CVehicle* object = vehicles->m_pObjects[vehicle];
        if (object && object->IsTrainPart())
        {
            Scripting::ProcessCommand(&g_localTrainCameraCommand, object->m_nHandle, 3, 2);
            g_localTrainCameraTick = GetTickCount();
        }
    }
}
// 0x00005a40
void CLocalPlayer::ExitVehicle(int vehicle)
{
    RakNet::BitStream stream;
    CVehiclePool* vehicles = *(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
    WORD id = (WORD)vehicle;
    if (id >= 2000 || !vehicles->m_bNotEmpty[id])
        return;
    CVehicle* object = vehicles->m_pObjects[id];
    if (!object)
        return;
    if (!GetPed()->IsPassenger())
        m_state.lastVehicle = id;
    if (object->IsTrainPart())
        g_pGame->m_pCamera->Restore();
    if (!object->IsRC())
    {
        stream.WriteBits((const unsigned char*)&vehicle, 16, true);
        ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localExitVehicleRpc, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
    }
}
// 0x000066b0
void CLocalPlayer::SendTakeDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart)
{
    // Native scales and stores the float before constructing the stream.
    damage *= g_localDamageScale;
    RakNet::BitStream stream;
    stream.Write1();
    stream.WriteBits((const unsigned char*)&player, 16, true);
    stream.WriteBits((const unsigned char*)&damage, 32, true);
    stream.WriteBits((const unsigned char*)&weapon, 32, true);
    stream.WriteBits((const unsigned char*)&bodyPart, 32, true);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localPlayerDamageRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}
// 0x000067c0
void CLocalPlayer::SendGiveDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart)
{
    damage *= g_localDamageScale;
    RakNet::BitStream stream;
    stream.Write0();
    stream.WriteBits((const unsigned char*)&player, 16, true);
    stream.WriteBits((const unsigned char*)&damage, 32, true);
    stream.WriteBits((const unsigned char*)&weapon, 32, true);
    stream.WriteBits((const unsigned char*)&bodyPart, 32, true);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localPlayerDamageRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}
// 0x000068d0
void CLocalPlayer::SendActorDamage(WORD actor, float damage, DWORD weapon, DWORD bodyPart)
{
    RakNet::BitStream stream;
    stream.Write0();
    stream.WriteBits((const unsigned char*)&actor, 16, true);
    stream.WriteBits((const unsigned char*)&damage, 32, true);
    stream.WriteBits((const unsigned char*)&weapon, 32, true);
    stream.WriteBits((const unsigned char*)&bodyPart, 32, true);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localActorDamageRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

// 0x000069d0
void CLocalPlayer::SendBullet(PedBulletData* data)
{
    CPed* ped = m_state.ped;
    if (!ped)
        return;
    if ((BYTE)ped->GetCurrentWeapon() == 34 && !data->entity)
        return;
    BYTE type = 0;
    WORD id = 0xffff;
    if (data->entity)
    {
        BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
        CPlayerPool* players = *(CPlayerPool**)(pools + 8);
        CVehiclePool* vehicles = *(CVehiclePool**)(pools + 0xc);
        CObjectPool* objects = *(CObjectPool**)(pools + 0x14);
        // Native requires player and vehicle pools, but does not check objects.
        if (players && vehicles)
        {
            id = players->Find(data->entity);
            if (id != 0xffff)
                type = 1;
            else
            {
                id = vehicles->Find(data->entity);
                if (id != 0xffff)
                    type = 2;
                else
                {
                    id = objects->GetId(data->entity);
                    if (id != 0xffff)
                        type = 3;
                    else
                    {
                        BYTE* backend = (BYTE*)data->entity;
                        CMatrix* matrix = *(CMatrix**)(backend + 0x14);
                        if (matrix)
                        {
                            CVector world = {0, 0, 0};
                            GameTransformPoint(&world, matrix, &data->offset);
                            memcpy(&data->offset, &world, 12);
                        }
                        else
                        {
                            data->offset.x = *(float*)(backend + 4) + data->offset.x;
                            data->offset.y = *(float*)(backend + 8) + data->offset.y;
                            data->offset.z = *(float*)(backend + 0xc) + data->offset.z;
                        }
                    }
                }
            }
        }
    }
    BulletSyncData payload = {0};
    payload.type = type;
    memcpy(&payload.targetId, &id, 2);
    memcpy(&payload.origin, &data->origin, 12);
    memcpy(&payload.target, &data->target, 12);
    memcpy(&payload.offset, &data->offset, 12);
    payload.weapon = (BYTE)m_state.ped->GetCurrentWeapon();
    RakNet::BitStream stream;
    BYTE packet = 206;
    stream.WriteBits(&packet, 8, true);
    stream.Write((const char*)&payload, 40);
    typedef char BulletReliability[(HIGH_PRIORITY == 1 && UNRELIABLE_SEQUENCED == 7) ? 1 : -1];
    ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
}

#include "game/keystuff.h"
// Shared native surfing work buffers; matrix.position aliases are not new globals.
// 0x001569a8
CVector g_localSurfPedTurnSpeed;
// 0x001569b8
CMatrix g_localSurfPedMatrix;
// 0x001569f8
CVector g_localSurfEntitySpeed;
// 0x00156a08
CMatrix g_localSurfInverseMatrix;
// 0x00156a48
CVector g_localSurfPedSpeed;
// 0x00156a58
CMatrix g_localSurfEntityMatrix;
// 0x00156aa4
CVector g_localSurfEntityTurnSpeed;

// 0x000034b0
void CLocalPlayer::ApplySurfingPhysics()
{
    if (!m_state.surfingEntity)
        return;
    m_state.surfingEntity->GetMatrix(&g_localSurfEntityMatrix);
    m_state.ped->GetMatrix(&g_localSurfPedMatrix);
    m_state.surfingEntity->GetSpeed(&g_localSurfEntitySpeed);
    m_state.surfingEntity->GetTurnSpeed(&g_localSurfEntityTurnSpeed);
    WORD leftRight, upDown;
    m_state.ped->GetKeys(&leftRight, &upDown);
    KeyStuff::GetInternalKeys();
    if (m_state.surfingLocked && !leftRight && !upDown && !m_state.ped->HasTaskType211())
    {
        GameTransformPoint(&g_localSurfPedMatrix.position, &g_localSurfEntityMatrix, &m_state.surfingOffset);
        m_state.ped->ApplyMatrix(g_localSurfPedMatrix);
        m_state.ped->GetSpeed(&g_localSurfPedSpeed);
        memcpy(&g_localSurfPedSpeed.x, &g_localSurfEntitySpeed.x, 8);
        m_state.ped->SetSpeed(g_localSurfPedSpeed);
        m_state.ped->GetTurnSpeed(&g_localSurfPedTurnSpeed);
        memcpy(&g_localSurfPedTurnSpeed.z, &g_localSurfEntityTurnSpeed.z, 4);
        m_state.ped->SetTurnSpeed(g_localSurfPedTurnSpeed);
    }
    else
    {
        GameInvertRwMatrix(&g_localSurfInverseMatrix, &g_localSurfEntityMatrix);
        GameTransformPoint(&m_state.surfingOffset, &g_localSurfInverseMatrix, &g_localSurfPedMatrix.position);
    }
}
// 0x00003630
void CLocalPlayer::SetSurfingEntity(CEntity* entity, DWORD locked)
{
    m_state.surfingEntity = entity;
    m_state.surfingLocked = locked;
}

// 0x0013c0a8
DWORD g_localOnFootSendRate = 30;
// 0x0013c0ac
DWORD g_localInCarSendRate = 30;
// 0x000039c0
DWORD CLocalPlayer::GetInCarSendRate()
{
    if (!m_state.ped)
        return 1000;
    if (((CNetGame*)g_pNetGame)->lanMode)
        return 15;
    return g_pGame->CountUsedPlayerSlots() + g_localInCarSendRate;
}
// 0x00003a00
DWORD CLocalPlayer::GetOnFootSendRate()
{
    if (!m_state.ped)
        return 1000;
    if (((CNetGame*)g_pNetGame)->lanMode)
        return 15;
    return g_pGame->CountUsedPlayerSlots() + g_localOnFootSendRate;
}
// 0x00003a40
DWORD CLocalPlayer::GetSendRate()
{
    CPed* ped = m_state.ped;
    if (!ped)
        return 1000;
    return ped->IsInVehicle() ? GetInCarSendRate() : GetOnFootSendRate();
}

// 0x00117888
const char g_localWorldBoundaryText[] = "Stay within the ~r~world boundries";
// 0x00117968
int g_localVehicleDamageRpc = 106;
// 0x0011797c
const float g_localPassengerEntryDistance = 8.0f;
// 0x00002fd0
void CLocalPlayer::ProcessHead()
{
    CPed* ped = m_state.ped;
    if (!ped)
        return;
    if (g_pGame && !*(DWORD*)((BYTE*)g_pGame + 0x59))
        return;
    if (ped->GetStuff())
        return;
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x80))
        return;
    CMatrix matrix;
    m_state.ped->GetMatrix(&matrix);
    DWORD now = GetTickCount();
    CLocalPlayer* self = this;
    if (now - m_state.headUpdateTick > 1000)
    {
        CameraAim* aim = AimStuff::GetInternalAim();
        float z;
        // clang-format off
        __asm {
            mov eax,aim
            fld dword ptr [eax]
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+48]
            fld dword ptr [eax+4]
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+52]
            fld dword ptr [eax+8]
            mov ecx,self
            mov edx,now
            mov [ecx+120h],edx
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+56]
            fstp dword ptr [z]
            fxch st(1)
            fsub dword ptr [matrix+48]
            fstp dword ptr [ecx+114h]
            fsub dword ptr [matrix+52]
            fstp dword ptr [ecx+118h]
            fld dword ptr [z]
            fsub dword ptr [matrix+56]
            fstp dword ptr [ecx+11ch]
        }
        // clang-format on
    }
    if (now - m_state.headTaskTick > 200)
    {
        CVector target;
        ped = m_state.ped;
        // clang-format off
        __asm {
            mov ecx,self
            fld dword ptr [matrix+48]
            fadd dword ptr [ecx+114h]
            fstp dword ptr [target]
            fld dword ptr [matrix+52]
            fadd dword ptr [ecx+118h]
            fstp dword ptr [target+4]
            fld dword ptr [matrix+56]
            fadd dword ptr [ecx+11ch]
            fstp dword ptr [target+8]
        }
        // clang-format on
        ped->ApplyCommandTask(g_remoteHeadTask, 0, 2000, -1, &target, 0, 0.1f, 500, 3, 0);
        m_state.headTaskTick = now;
    }
}
// 0x00003e40
void CLocalPlayer::ProcessWorldBoundaries()
{
    if (g_pGame->GetCurrentInterior())
        return;
    CNetGameSettings* settings = ((CNetGame*)g_pNetGame)->settings;
    if (m_state.ped->EnforceWorldBoundries(settings->worldBounds[0], settings->worldBounds[1], settings->worldBounds[2], settings->worldBounds[3]))
    {
        m_state.ped->SetArmedWeapon(0, 0);
        g_pGame->DisplayGameText(g_localWorldBoundaryText, 1000, 5);
    }
}
// 0x00003ea0
void CLocalPlayer::ProcessVehicleWorldBoundaries()
{
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    WORD id = vehicles->Find(m_state.ped->GetVehicle());
    if (g_pGame->GetCurrentInterior() || id == 0xffff || id >= 2000 || !vehicles->m_bNotEmpty[id])
        return;
    CVehicle* vehicle = vehicles->m_pObjects[id];
    if (!vehicle)
        return;
    CNetGameSettings* settings = ((CNetGame*)g_pNetGame)->settings;
    if (vehicle->EnforceWorldBoundries(settings->worldBounds[0], settings->worldBounds[1], settings->worldBounds[2], settings->worldBounds[3]))
        g_pGame->DisplayGameText(g_localWorldBoundaryText, 1000, 5);
}
// 0x00003f60
void CLocalPlayer::PrepareForClassSelection()
{
    CPed* ped = m_state.ped;
    m_state.spawnPermission = 0;
    if (ped)
    {
        ped->SetInitialState();
        m_state.ped->SetHealth(100.0f);
        m_state.ped->SetControllable(FALSE);
    }
    DWORD(WINAPI * tick)() = GetTickCount;
    m_state.initialSelectionTick = tick();
    m_state.classSelectionTick = tick();
}
// 0x00005c20
void CLocalPlayer::ProcessVehicleDamage(WORD id)
{
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (id >= 2000 || !vehicles->m_bNotEmpty[id])
        return;
    CVehicle* vehicle = vehicles->m_pObjects[id];
    if (!vehicle)
        return;
    if (id != m_state.damageVehicle)
    {
        m_state.panelsDamage = vehicle->GetPanelsDamage();
        m_state.doorsDamage = vehicle->GetDoorsDamage();
        m_state.lightsDamage = vehicle->GetLightsDamage();
        m_state.tiresDamage = vehicle->GetTires();
        m_state.damageVehicle = id;
        return;
    }
    if (m_state.panelsDamage == vehicle->GetPanelsDamage() && m_state.doorsDamage == vehicle->GetDoorsDamage() && m_state.lightsDamage == vehicle->GetLightsDamage() &&
        m_state.tiresDamage == vehicle->GetTires())
        return;
    m_state.panelsDamage = vehicle->GetPanelsDamage();
    m_state.doorsDamage = vehicle->GetDoorsDamage();
    m_state.lightsDamage = vehicle->GetLightsDamage();
    m_state.tiresDamage = vehicle->GetTires();
    RakNet::BitStream stream;
    WORD sentId = m_state.damageVehicle;
    stream.WriteBits((const unsigned char*)&sentId, 16, true);
    DWORD panels = m_state.panelsDamage;
    stream.WriteBits((const unsigned char*)&panels, 32, true);
    DWORD doors = m_state.doorsDamage;
    stream.WriteBits((const unsigned char*)&doors, 32, true);
    BYTE lights = m_state.lightsDamage;
    stream.WriteBits(&lights, 8, true);
    BYTE tires = m_state.tiresDamage;
    stream.WriteBits(&tires, 8, true);
    typedef char VehicleDamageReliability[(HIGH_PRIORITY == 1 && RELIABLE == 8) ? 1 : -1];
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localVehicleDamageRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
}
// 0x00006de0
void CLocalPlayer::ProcessPassengerEntry()
{
    BYTE* keys = (BYTE*)KeyStuff::GetInternalKeys();
    WORD enter = *(WORD*)(keys + 0x10);
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (!enter || *(WORD*)(keys + 0x40))
        return;
    WORD id = vehicles->FindNearestToLocalPlayer();
    if (id >= 2000 || !vehicles->m_bNotEmpty[id])
        return;
    CVehicle* vehicle = vehicles->Get(id);
    BYTE nearEnough = 0;
    // clang-format off
    __asm {
        mov ecx,vehicle
        call CEntity::GetDistanceToLocalPlayer
        fcomp dword ptr [g_localPassengerEntryDistance]
        fnstsw ax
        test ah,5
        jp passenger_too_far
        mov nearEnough,1
    passenger_too_far:
    }
    // clang-format on
    if (!nearEnough) return;
    if ((BYTE)m_state.ped->GetCurrentWeapon() == 46)
        m_state.ped->SetArmedWeapon(0, 0);
    m_state.ped->EnterVehicle(vehicle->m_nHandle, TRUE);
    EnterVehicle(id, TRUE);
}
// 0x00004a30
WORD CLocalPlayer::GetAimedPlayer()
{
    CPed* ped = m_state.ped;
    if (!ped)
        return 0xffff;
    void* target = ped->GetAimedPed();
    if (!target)
        return 0xffff;
    CPlayerPool* players = ((CNetGame*)g_pNetGame)->pools->players;
    return players ? players->Find(target) : 0xffff;
}
// 0x00004a60
WORD CLocalPlayer::GetAimedActor()
{
    CPed* ped = m_state.ped;
    if (!ped)
        return 0xffff;
    void* target = ped->GetAimedPed();
    if (!target)
        return 0xffff;
    CActorPool* actors = ((CNetGame*)g_pNetGame)->pools->actors;
    return actors ? (WORD)actors->Find(target) : 0xffff;
}
// 0x00117854
const Scripting::OpcodeInfo g_localPedCameraCommand = {0x159, "iii"};
// 0x00005f40
void CLocalPlayer::ProcessClassSelection()
{
    if (!g_pGame->IsMenuVisible())
    {
        g_pGame->EnableHUD(FALSE);
        g_pGame->SetCursorMode(2, FALSE);
    }
    if (g_renderDialog && g_renderDialog->active)
        return;
    if (!m_state.rawFlag30a)
    {
        *((BYTE*)g_renderSecondDialog + 0x13) = 1;
        RECT rectangle;
        GetClientRect(g_inputGameWindow, &rectangle);
        BYTE* dialog = (BYTE*)g_renderSecondDialog;
        int y = rectangle.bottom - *(int*)(dialog + 0x122) - 50;
        int x = rectangle.right / 2 - *(int*)(dialog + 0x11e) / 2;
        *(int*)(dialog + 0x11a) = y;
        *(int*)(dialog + 0x116) = x;
        m_state.rawFlag30a = 1;
        RequestClass(m_state.selectedClass);
        return;
    }
    DWORD(WINAPI * tick)() = GetTickCount;
    if (tick() - m_state.initialSelectionTick < 2000)
        return;
    SHORT(WINAPI * keyState)(int) = GetAsyncKeyState;
    if (!m_state.waitingForSpawnReply)
    {
        if (!m_state.spawnPermission)
            return;
        if (keyState(VK_SHIFT) < 0 && !g_pInput->m_bEnabled)
        {
            RequestSpawn();
            return;
        }
    }
    if (!m_state.spawnPermission)
        return;
    DWORD elapsed = tick() - m_state.classSelectionTick;
    if (keyState(VK_LEFT) < 0 && elapsed > 250)
    {
        m_state.classSelectionTick = tick();
        PrevClass();
        return;
    }
    if (keyState(VK_RIGHT) < 0 && elapsed > 250)
    {
        m_state.classSelectionTick = tick();
        NextClass();
    }
}
// 0x000060d0
void CLocalPlayer::UpdateWeapons()
{
    BYTE weapon = (BYTE)m_state.ped->GetCurrentWeapon();
    bool changed = false;
    WORD player = GetAimedPlayer();
    WORD actor = GetAimedActor();
    if (player != m_state.aimedPlayer || actor != m_state.aimedActor)
    {
        m_state.aimedPlayer = player;
        m_state.aimedActor = actor;
        changed = true;
    }
    else if (weapon == m_state.currentWeapon && GetTickCount() - m_state.weaponsUpdateTick < 100)
        return;
    if (weapon != m_state.currentWeapon)
        m_state.currentWeapon = weapon;
    RakNet::BitStream stream;
    BYTE packet = 204;
    stream.WriteBits(&packet, 8, true);
    WORD target = m_state.aimedPlayer;
    stream.WriteBits((const unsigned char*)&target, 16, true);
    target = m_state.aimedActor;
    stream.WriteBits((const unsigned char*)&target, 16, true);
    for (BYTE index = 0; index < 13; ++index)
    {
        BYTE* backend = (BYTE*)m_state.ped->m_pGamePed;
        BYTE* slot = backend + index * 0x1c;
        if ((DWORD)m_state.lastWeapons[index] != *(DWORD*)(slot + 0x5a0) || m_state.lastAmmo[index] != *(DWORD*)(slot + 0x5ac))
        {
            m_state.lastWeapons[index] = *(BYTE*)(slot + 0x5a0);
            backend = (BYTE*)m_state.ped->m_pGamePed;
            m_state.lastAmmo[index] = *(DWORD*)(backend + index * 0x1c + 0x5ac);
            BYTE sentIndex = index;
            stream.WriteBits(&sentIndex, 8, true);
            BYTE type = m_state.lastWeapons[index];
            stream.WriteBits(&type, 8, true);
            WORD ammo = *(WORD*)&m_state.lastAmmo[index];
            stream.WriteBits((const unsigned char*)&ammo, 16, true);
            changed = true;
        }
    }
    if (changed)
    {
        m_state.weaponsUpdateTick = GetTickCount();
        ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE, 0);
    }
}
// 0x00006360
void CLocalPlayer::ProcessSpectating()
{
    RakNet::BitStream stream;
    WORD leftRight, upDown;
    WORD keys = m_state.ped->GetKeys(&leftRight, &upDown);
    CMatrix matrix;
    g_pGame->m_pCamera->GetMatrix(&matrix);
    CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
    CPlayerPool* players = pools->players;
    CVehiclePool* vehicles = pools->vehicles;
    if (!players || !vehicles)
        return;
    BYTE data[18];
    *(WORD*)data = leftRight;
    *(WORD*)(data + 2) = upDown;
    *(WORD*)(data + 4) = keys;
    memcpy(data + 6, &matrix.position, 12);
    DWORD rate = GetOnFootSendRate();
    DWORD(WINAPI * tick)() = GetTickCount;
    if (tick() - m_state.lastSpectatorSendTick > rate)
    {
        m_state.lastSpectatorSendTick = tick();
        BYTE packet = 212;
        stream.WriteBits(&packet, 8, true);
        stream.Write((const char*)data, 18);
        ((CNetGame*)g_pNetGame)->rakClient->Send(&stream, HIGH_PRIORITY, UNRELIABLE, 1);
        rate = GetOnFootSendRate() * 2;
        if (tick() - m_state.lastAimSendTick > rate)
        {
            m_state.lastAimSendTick = tick();
            SendAimData();
        }
    }
    g_pGame->EnableHUD(FALSE);
    m_state.ped->SetHealth(100.0f);
    CVector position = *(CVector*)(data + 6);
    // clang-format off
    // Native loads received Z before adding20; retain operand order for NaNs.
    __asm {
        fld dword ptr [position+8]
        fadd dword ptr [g_remoteHeadDistance]
        fstp dword ptr [position+8]
    }
    // clang-format on
    m_state.ped->Teleport(position);
    if (m_state.spectateType == 1)
    {
        WORD id = *(WORD*)&m_state.spectateId;
        if (id >= 1004 || !players->connected[id])
        {
            m_state.spectateType = 0;
            m_state.spectateProcessed = 0;
        }
    }
    if (m_state.spectateType == 1)
    {
        WORD id = *(WORD*)&m_state.spectateId;
        if (id < 1004 && players->connected[id])
        {
            CRemotePlayer* remote = players->GetPlayer(id);
            if (!remote->m_state.ped || !remote->m_state.playerState || players->GetPlayer(id)->m_state.playerState == 32)
            {
                m_state.spectateType = 0;
                m_state.spectateProcessed = 0;
            }
        }
    }
    if (m_state.spectateProcessed)
        return;
    if (!m_state.spectateType)
    {
        m_state.ped->WarpFromVehicle(0.0f, 0.0f, 10.0f);
        CVector cameraPosition = {50.0f, 50.0f, 50.0f}, rotation = {0, 0, 0};
        g_pGame->m_pCamera->Set(cameraPosition, rotation);
        CVector target = {60.0f, 60.0f, 50.0f};
        g_pGame->m_pCamera->PointAt(target, 2);
        m_state.spectateProcessed = 1;
    }
    else if (m_state.spectateType == 1)
    {
        WORD id = *(WORD*)&m_state.spectateId;
        if (!players->IsConnected(id))
            return;
        CPed* ped = players->GetPlayer(id)->m_state.ped;
        if (!ped)
            return;
        Scripting::ProcessCommand(&g_localPedCameraCommand, ped->m_nHandle, (DWORD)m_state.spectateCameraMode, 2);
        m_state.spectateProcessed = 1;
    }
    else if (m_state.spectateType == 2)
    {
        WORD id = *(WORD*)&m_state.spectateId;
        if (!vehicles->DoesExist(id))
            return;
        CVehicle* vehicle = vehicles->Get(id);
        if (!vehicle)
            return;
        Scripting::ProcessCommand(&g_localTrainCameraCommand, vehicle->m_nHandle, (DWORD)m_state.spectateCameraMode, 2);
        m_state.spectateProcessed = 1;
    }
}
// 0x001178ac
const float g_localCameraRayLength = 70.0f;
// 0x001178b0
int g_localCameraTargetRpc = 168;
// 0x00117920
const char g_localCameraTargetObjectText[] = "CAMERA_TARGET_TYPE_OBJECT: ID(%u)";
// 0x001178fc
const char g_localCameraTargetVehicleText[] = "CAMERA_TARGET_TYPE_VEHICLE: ID(%u)";
// 0x001178d8
const char g_localCameraTargetPlayerText[] = "CAMERA_TARGET_TYPE_PLAYER: ID(%u)";
// 0x001178b4
const char g_localCameraTargetActorText[] = "CAMERA_TARGET_TYPE_ACTOR: ID(%u)";
// 0x00004300
bool CLocalPlayer::GetCameraTarget(WORD* targets)
{
    CameraAim* aim = AimStuff::GetInternalAim();
    CVector origin, end;
    void* entity = 0;
    BYTE collision[0x2c];
    // clang-format off
    // Retain the native overwritten endpoint calculations and output write order.
    __asm {
        mov esi,aim
        fld dword ptr [esi]
        fadd dword ptr [esi+0ch]
        fstp dword ptr [origin]
        fld dword ptr [esi+4]
        fadd dword ptr [esi+10h]
        fstp dword ptr [origin+4]
        fld dword ptr [esi+8]
        fadd dword ptr [esi+14h]
        fstp dword ptr [origin+8]
        fld dword ptr [esi]
        fmul dword ptr [g_localCameraRayLength]
        fadd dword ptr [origin]
        fstp dword ptr [end]
        fld dword ptr [esi+4]
        fmul dword ptr [g_localCameraRayLength]
        fadd dword ptr [origin+4]
        fstp dword ptr [end+4]
        fld dword ptr [esi+8]
        mov edx,targets
        mov word ptr [edx],0ffffh
        fmul dword ptr [g_localCameraRayLength]
        mov word ptr [edx+2],0ffffh
        mov word ptr [edx+4],0ffffh
        mov word ptr [edx+6],0ffffh
        fadd dword ptr [origin+8]
        fstp dword ptr [end+8]
    }
    // clang-format on
    for (int stage = 0; stage < 3; ++stage)
    {
        // clang-format off
        __asm {
            mov eax,aim
            fld dword ptr [eax]
            fmul dword ptr [g_localCameraRayLength]
            fadd dword ptr [eax+0ch]
            fstp dword ptr [end]
            fld dword ptr [eax+4]
            fmul dword ptr [g_localCameraRayLength]
            fadd dword ptr [eax+10h]
            fstp dword ptr [end+4]
            fld dword ptr [eax+8]
            fmul dword ptr [g_localCameraRayLength]
            fadd dword ptr [eax+14h]
            fstp dword ptr [end+8]
        }
        // clang-format on
        // Native ignores AL and keeps the same entity/collision storage for all3 calls.
        GameProcessLineOfSightRaw(&origin, &end, collision, &entity, 1, stage == 2, 0, stage != 0, 0, 0, 0, 0);
        if (entity)
            memcpy(&end, collision, 12);
        CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
        WORD id;
        if (stage == 0)
        {
            id = pools->objects->FindInRay(40.0f, &origin, &end);
            if (id != 0xffff)
                targets[0] = id;
        }
        else if (stage == 1)
        {
            id = pools->vehicles->FindInRay(40.0f, &origin, &end, 0x3f800000);
            if (id != 0xffff)
                targets[1] = id;
        }
        else
        {
            id = pools->players->FindInRay(20.0f, &origin, &end, 0x3f400000);
            if (id != 0xffff)
                targets[2] = id;
            id = ((CNetGame*)g_pNetGame)->pools->actors->FindInRay(20.0f, &origin, &end, 0.75f);
            if (id != 0xffff)
                targets[3] = id;
        }
    }
    return true;
}
// 0x000045c0
void CLocalPlayer::UpdateCameraTarget()
{
    if (!((CNetGame*)g_pNetGame)->updateCameraTarget)
    {
        m_state.cameraTargets[0] = 0xffff;
        m_state.cameraTargets[1] = 0xffff;
        m_state.cameraTargets[2] = 0xffff;
        m_state.cameraTargets[3] = 0xffff;
        return;
    }
    DWORD(WINAPI * tick)() = GetTickCount;
    if (tick() - m_state.cameraTargetTick <= 100)
        return;
    WORD targets[4];
    GetCameraTarget(targets);
    if (targets[0] != m_state.cameraTargets[0] || targets[1] != m_state.cameraTargets[1] || targets[2] != m_state.cameraTargets[2] || targets[3] != m_state.cameraTargets[3])
    {
        RakNet::BitStream stream;
        stream.Write((const char*)targets, 8);
        ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_localCameraTargetRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
        memcpy(m_state.cameraTargets, targets, 8);
    }
    m_state.cameraTargetTick = tick();
}
// 0x00004710
void CLocalPlayer::DrawCameraTargetLabels()
{
    if (!g_commandCameraTargetLabels)
        return;
    char text[257] = {0};
    g_deviceLabel->Begin();
    const char* formats[4] = {g_localCameraTargetObjectText, g_localCameraTargetVehicleText, g_localCameraTargetPlayerText, g_localCameraTargetActorText};
    for (int kind = 0; kind < 4; ++kind)
    {
        WORD id = m_state.cameraTargets[kind];
        if (id == 0xffff)
            continue;
        sprintf(text, formats[kind], (unsigned)id);
        CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
        id = m_state.cameraTargets[kind];
        CEntity* entity = 0;
        if (kind == 0)
            entity = pools->objects->Get(id);
        else if (kind == 1)
            entity = pools->vehicles->Get(id);
        else if (kind == 2)
        {
            CRemotePlayer* remote = pools->players->GetPlayer(id);
            if (remote)
                entity = remote->m_state.ped;
        }
        else
            entity = pools->actors->Get(id);
        if (!entity)
            continue;
        CMatrix matrix;
        entity->GetMatrix(&matrix);
        if (g_deviceObjectEdit)
            ((CObjectEdit*)g_deviceObjectEdit)->RenderAxes(&matrix, 1.0f);
        CVector position = matrix.position;
        g_deviceLabel->Draw(&position, text, 0xffd47b75, TRUE, FALSE);
    }
    g_deviceLabel->End();
}
// 0x0013c0b0
DWORD g_localAimSendRate = 30;
// 0x00117980
const char g_localClassSelectionMessage[] = "Returning to class selection after next death";
// 0x000072d0
BOOL CLocalPlayer::Process()
{
    if (m_state.active && m_state.ped)
    {
        g_deviceSpawnScreen->SetText(0);
        if ((!m_state.wasted && m_state.ped->GetState() == 54) || m_state.ped->IsDead())
        {
            DWORD classSelection = m_state.wantsAnotherClass;
            if (m_state.spectating && !classSelection)
                Spawn();
            else if (classSelection)
            {
                m_state.active = 0;
                m_state.hasSpawnInfo = 0;
            }
            m_state.spectating = 0;
            m_state.spectateType = 0;
            m_state.spectateId = 0xffffffff;
            m_state.spectateProcessed = 0;
            if (m_state.ped->DoesDancing())
                m_state.ped->StopDancing();
            if (m_state.ped->UsingCellphone())
                m_state.ped->EnableCellphone(FALSE);
            if (m_state.ped->DoesUrinating())
                m_state.ped->StopUrinating();
            if (m_state.ped->GetStuff())
                m_state.ped->DropStuff();
            m_state.ped->SetDrunkLevel(0);
            m_state.ped->SetControllable(TRUE);
            if (m_state.rcMode)
            {
                m_state.rcMode = 0;
                m_state.ped->Add();
            }
            if (m_state.ped->IsInVehicle() && !m_state.ped->IsPassenger())
            {
                SendIncarData();
                CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
                m_state.lastVehicle = vehicles->Find(m_state.ped->GetVehicle());
            }
            m_state.ped->CallBackend730AndVehicle490();
            WastedNotification();
            m_state.active = 0;
            m_state.wasted = 1;
            g_pGame->EnableZoneDisplaying(0);
            return TRUE;
        }
        WORD leftRight, upDown;
        WORD keys = m_state.ped->GetKeys(&leftRight, &upDown);
        DrawCameraTargetLabels();
        if (m_state.ped->DoesDancing())
        {
            g_pGame->EnableHUD(FALSE);
            m_state.ped->ProcessDancing();
            if (*(WORD*)((BYTE*)KeyStuff::GetInternalKeys() + 0x1e))
                m_state.ped->StopDancing();
        }
        if (m_state.ped->HasDanceState())
            m_state.ped->ProcessDancePoseWeapon();
        if (m_state.ped->HasDanceStyle())
            m_state.ped->ProcessSecondaryPoseWeapon();
        if (m_state.ped->DoesHandsUp() && *(WORD*)((BYTE*)KeyStuff::GetInternalKeys() + 0x1e))
            m_state.ped->SetControllable(TRUE);
        if ((m_state.ped->DoesUrinating() && *(WORD*)((BYTE*)KeyStuff::GetInternalKeys() + 0x1e)) || (m_state.ped->DoesUrinating() && m_state.ped->IsInVehicle()))
            m_state.ped->StopUrinating();
        if (m_state.ped->GetStuff())
        {
            if (!*(WORD*)((BYTE*)KeyStuff::GetInternalKeys() + 0x1e) && !m_state.ped->IsInVehicle())
                m_state.ped->ApplyStuff();
            else
                m_state.ped->DropStuff();
        }
        m_state.ped->ProcessDrunk();
        if (m_state.ped->IsInVehicle() && m_state.ped->DoesDancing())
            m_state.ped->StopDancing();
        m_state.ped->ResetLocalControlWord();
        DWORD animation = m_state.ped->GetPackedAnimation();
        if (animation)
        {
            m_state.animation = animation;
            m_state.animationSent = 0;
        }
        else if (m_state.animation && m_state.animationSent)
            m_state.animation = 0;
        DWORD(WINAPI * tick)() = GetTickCount;
        DWORD now = tick();
        if (g_localTrainCameraTick != 0xffffffff && now - g_localTrainCameraTick > 5000 && !m_state.ped->IsInVehicle())
        {
            g_pGame->m_pCamera->Restore();
            g_localTrainCameraTick = 0xffffffff;
        }
        if ((int)now >= *(int*)&m_state.zoneDisplayTick)
            g_pGame->EnableZoneDisplaying(((CNetGame*)g_pNetGame)->settings->zoneNames);
        g_pGame->ProcessCheckpoints();
        if (now - m_state.lastStatsSendTick > 1000)
        {
            SendStats();
            m_state.lastStatsSendTick = now;
        }
        UpdateWeapons();
        BYTE interior = g_pGame->GetCurrentInterior();
        if (interior != m_state.interior)
            SendInteriorChange(interior);
        UpdateCameraTarget();
        if (m_state.spectating)
        {
            ProcessSpectating();
            m_state.passengerDriveby = 0;
        }
        else if (m_state.ped->GetState() == 50 && !m_state.ped->IsPassenger())
        {
            m_state.ped->RemoveWeaponWhenEnteringVehicle();
            ProcessHead();
            ProcessVehicleWorldBoundaries();
            if (now - m_state.lastAimSendTick > 1000)
            {
                m_state.lastAimSendTick = now;
                SendAimData();
            }
            CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
            if (vehicles)
                m_state.currentVehicle = vehicles->Find(m_state.ped->GetVehicle());
            WORD id = m_state.currentVehicle;
            CVehicle* vehicle = vehicles->Get(id);
            ProcessVehicleDamage(id);
            if (vehicle && !m_state.rcMode && vehicle->IsRC())
            {
                m_state.ped->Remove();
                m_state.rcMode = 1;
            }
            if (m_state.rcMode)
            {
                if (!vehicle)
                {
                    m_state.ped->SetHealth(0.0f);
                    m_state.ped->Kill();
                }
                if (m_state.rcMode && vehicle && vehicle->GetHealth() == 0.0f)
                {
                    m_state.ped->SetHealth(0.0f);
                    m_state.ped->Kill();
                }
            }
            DWORD rate = GetInCarSendRate();
            if (now - m_state.lastSendTick > rate)
            {
                m_state.lastSendTick = tick();
                SendIncarData();
            }
            m_state.passengerDriveby = 0;
        }
        else if (m_state.ped->GetState() == 1 || m_state.ped->GetState() == 12)
        {
            ProcessSurfing();
            ProcessHead();
            if (m_state.rcMode)
            {
                m_state.rcMode = 0;
                m_state.ped->Add();
            }
            ProcessPassengerEntry();
            ProcessWorldBoundaries();
            if (m_state.currentVehicle != 0xffff)
            {
                m_state.lastVehicle = m_state.currentVehicle;
                m_state.currentVehicle = 0xffff;
            }
            if (NeedsToSendOnfootData(keys, upDown, leftRight) || now - m_state.lastSendTick > GetOnFootSendRate())
            {
                m_state.lastSendTick = tick();
                SendOnfootData();
            }
            DWORD aimRate = !(keys & 0x84) ? 1000 : (g_netStatsShotCompMode == 2 ? g_localAimSendRate : 100);
            if (now - m_state.lastAimSendTick > aimRate)
            {
                m_state.lastAimSendTick = now;
                SendAimData();
            }
            m_state.passengerDriveby = 0;
        }
        else if (m_state.ped->GetState() == 50 && m_state.ped->IsPassenger())
        {
            ProcessHead();
            if (m_state.rcMode)
            {
                m_state.rcMode = 0;
                m_state.ped->Add();
            }
            BYTE* pad = (BYTE*)KeyStuff::GetInternalKeys();
            m_state.ped->GetCurrentWeapon();
            if (!m_state.passengerDriveby)
            {
                if (*(WORD*)(pad + 0x24) && m_state.ped->EnablePassengerDrivebyMode())
                    m_state.passengerDriveby = 1;
            }
            else if ((BYTE)m_state.ped->GetCurrentWeapon() == 0 || !m_state.ped->CurrentWeaponHasAmmo())
            {
                if (m_state.passengerDriveby)
                {
                    m_state.ped->StopTaskType1022();
                    m_state.passengerDriveby = 0;
                }
            }
            DWORD rate = GetInCarSendRate();
            if (now - m_state.lastSendTick > rate)
            {
                m_state.lastSendTick = tick();
                SendPassengerData();
            }
        }
    }
    if (!m_state.active && m_state.ped && (int)m_state.ped->GetDrunkLevel() > 0)
    {
        m_state.ped->SetDrunkLevel(0);
        m_state.ped->ProcessDrunk();
    }
    if (m_state.spectating && !m_state.active)
    {
        if (m_state.rawFlag30a == 1)
        {
            m_state.rawFlag30a = 0;
            if (g_renderSecondDialog)
                *((BYTE*)g_renderSecondDialog + 0x13) = 0;
            g_pGame->SetCursorMode(0, FALSE);
        }
        ProcessSpectating();
        return TRUE;
    }
    if (!m_state.wantsAnotherClass && GetAsyncKeyState(VK_F4) != 0)
    {
        m_state.wantsAnotherClass = 1;
        g_pChat->AddInfoMessage(g_localClassSelectionMessage);
    }
    if (m_state.wasted && m_state.ped->GetState() != 55 && m_state.ped->GetState() != 54)
    {
        if (m_state.ped->HasAccessory())
            m_state.ped->DeleteAllAccessories();
        if (m_state.ped->HasDanceState())
            m_state.ped->SetDancePoseState(0);
        if (m_state.spawnPermission && !m_state.wantsAnotherClass && ((CNetGame*)g_pNetGame)->gameState == CNetGame::StateConnected)
        {
            CPed* ped = m_state.ped;
            BYTE alive;
            // clang-format off
            __asm {
                mov ecx,ped
                call CPed::GetHealth
                fcomp dword ptr [g_objectScaleLowerBound]
                fnstsw ax
                test ah,41h
                setz alive
            }
            // clang-format on
            if (alive) Spawn();
            return TRUE;
        }
        m_state.wasted = 0;
        PrepareForClassSelection();
        m_state.wantsAnotherClass = 0;
        return TRUE;
    }
    if (m_state.ped->GetState() != 55 && m_state.ped->GetState() != 54 && ((CNetGame*)g_pNetGame)->gameState == CNetGame::StateConnected && !m_state.active && !m_state.spectating)
        ProcessClassSelection();
    return TRUE;
}

// 0x00002db0
DWORD CLocalPlayer::GetSpawnPermission()
{
    return m_state.spawnPermission;
}
// 0x000135c0
DWORD CLocalPlayer::IsSpectating()
{
    return m_state.spectating;
}

// 0x00004230
BYTE CLocalPlayer::GetAimTargetOffset(WORD* id, CVector* offset)
{
    CVector hit;
    void* entity = m_state.ped->GetCameraAimHit(&hit);
    if (!entity)
        return 0;
    CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
    CObjectPool* objects = pools->objects;
    CPlayerPool* players = pools->players;
    CVehiclePool* vehicles = pools->vehicles;
    BYTE kind = 0;
    WORD selected = players->Find(entity);
    if (selected != 0xffff)
        kind = 1;
    else
    {
        selected = vehicles->Find(entity);
        if (selected != 0xffff)
            kind = 2;
        else
        {
            selected = objects->GetId(entity);
            if (selected != 0xffff)
                kind = 3;
        }
    }
    if (selected == 0)
        return 0;
    offset->x = hit.x - (*(CMatrix**)((BYTE*)entity + 0x14))->position.x;
    offset->y = hit.y - (*(CMatrix**)((BYTE*)entity + 0x14))->position.y;
    offset->z = hit.z - (*(CMatrix**)((BYTE*)entity + 0x14))->position.z;
    *id = selected;
    return kind;
}
