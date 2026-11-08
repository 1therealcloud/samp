#include "net/remoteplayer.h"

#include "main.h"
// 0x00017790
void CRemotePlayer::ShowNameTag(BYTE show)
{
    m_state.showNameTag = show != 0;
}
// 0x000739c0
float CRemotePlayer::GetReportedHealth()
{
    return m_state.reportedHealth;
}
// 0x000739d0
float CRemotePlayer::GetReportedArmour()
{
    return m_state.reportedArmour;
}
// 0x00008180
void CRemotePlayer::SetTeam(BYTE team)
{
    m_state.team = team;
}
// 0x000a21e0
BYTE CRemotePlayer::GetTeam()
{
    return m_state.team;
}
#include "chatwindow.h"
#include "game/aimstuff.h"
#include "game/custommodels.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/objectpool.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"
#include <string.h>
// 0x001181dc
const float g_remoteVehicleAxisTeleportDistance = 6.0f;
// 0x001181d0
const double g_remoteUnoccupiedCorrectionThreshold = 0.05;
// 0x001181e0
const float g_remoteTrailerCorrection = 0.025f;
// 0x00015600
void CRemotePlayer::UpdateUnoccupied(const void* data)
{
    const UnoccupiedSyncData* sync = (const UnoccupiedSyncData*)data;
    WORD id = sync->vehicle;
    if (!id || id == 0xffff)
        return;
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    if (!vehicles)
        return;
    CVehicle* vehicle = vehicles->Get(id);
    // Native publishes the owner even for an invalid/nonexistent vehicle index.
    *(WORD*)((BYTE*)vehicles + 0x8e34 + id * 2) = m_state.id;
    if (!vehicle || vehicle->HasDriver() || vehicle->GetTractor())
        return;
    CMatrix matrix;
    vehicle->GetMatrix(&matrix);
    memcpy(&matrix.right, &sync->right, 12);
    memcpy(&matrix.up, &sync->up, 12);
    const CVector* target = &sync->position;
    bool significant = false, teleport = false;
    for (int axis = 0; axis < 3; ++axis)
    {
        DWORD wanted = ((const DWORD*)target)[axis], current = ((DWORD*)&matrix.position)[axis];
        BYTE changed;
        // clang-format off
        __asm {
            push current
            push wanted
            call FloatOffset
            fcomp dword ptr [g_objectMutationStep]
            add esp,8
            fnstsw ax
            test ah,41h
            setp changed
        }
        // clang-format on
        if (changed)
        {
            significant = true;
            break;
        }
    }
    if (!significant)
        return;
    for (int axis = 0; axis < 3; ++axis)
    {
        DWORD wanted = ((const DWORD*)target)[axis], current = ((DWORD*)&matrix.position)[axis];
        BYTE distant;
        const float* limit = axis < 2 ? &g_remoteVehicleAxisTeleportDistance : &g_frameScaleMultiplier;
        // clang-format off
        __asm {
            push current
            push wanted
            call FloatOffset
            mov ecx,limit
            fcomp dword ptr [ecx]
            add esp,8
            fnstsw ax
            test ah,41h
            sete distant
        }
        // clang-format on
        if (distant)
        {
            teleport = true;
            break;
        }
    }
    if (teleport)
    {
        matrix.position = *target;
        vehicle->SetMatrix(matrix);
        vehicle->SetSpeed(sync->speed);
        vehicle->SetTurnSpeed(sync->turnSpeed);
        return;
    }
    vehicle->SetMatrix(matrix);
    vehicle->SetSpeed(sync->speed);
    vehicle->SetTurnSpeed(sync->turnSpeed);
    CVector speed = {0, 0, 0};
    vehicle->GetSpeed(&speed);
    for (int axis = 0; axis < 3; ++axis)
    {
        const float *wanted = &target->x + axis, *current = &matrix.position.x + axis;
        float* value = &speed.x + axis;
        // clang-format off
        __asm {
            mov ecx,current
            push dword ptr [ecx]
            mov edx,wanted
            push dword ptr [edx]
            call FloatOffset
            fcomp qword ptr [g_remoteUnoccupiedCorrectionThreshold]
            add esp,8
            fnstsw ax
            test ah,41h
            jne unoccupied_axis_done
            mov ecx,wanted
            mov edx,current
            mov eax,value
            fld dword ptr [ecx]
            fsub dword ptr [edx]
            fmul dword ptr [g_remoteIncarCorrection]
            fadd dword ptr [eax]
            fstp dword ptr [eax]
        unoccupied_axis_done:
        }
        // clang-format on
    }
    vehicle->SetSpeed(speed);
    vehicle->m_dwRemoteUnoccupiedSync = 1;
}
// 0x00015950
void CRemotePlayer::UpdateTrailer(const void* data)
{
    const LocalTrailerSyncData* sync = (const LocalTrailerSyncData*)data;
    WORD id = sync->vehicle;
    if (!id || id == 0xffff)
        return;
    CVehicle* trailer = ((CNetGame*)g_pNetGame)->pools->vehicles->Get(id);
    CVehicle* tractor = m_state.vehicle;
    if (!tractor || !trailer)
        return;
    if (!trailer->IsTrailer() && !tractor->IsTowtruck())
        return;
    if (tractor->GetTrailer() != trailer)
    {
        tractor->SetTrailer(trailer);
        tractor->AttachTrailer();
    }
    CMatrix matrix;
    trailer->GetMatrix(&matrix);
    GameQuaternionToMatrixComponents(&sync->rotation, &matrix);
    const CVector* target = &sync->position;
    bool significant = false, teleport = false;
    for (int axis = 0; axis < 3; ++axis)
    {
        DWORD wanted = ((const DWORD*)target)[axis], current = ((DWORD*)&matrix.position)[axis];
        BYTE changed;
        // clang-format off
        __asm {
            push current
            push wanted
            call FloatOffset
            fcomp dword ptr [g_textDrawHalf]
            add esp,8
            fnstsw ax
            test ah,41h
            setp changed
        }
        // clang-format on
        if (changed)
        {
            significant = true;
            break;
        }
    }
    if (!significant)
        return;
    for (int axis = 0; axis < 3; ++axis)
    {
        DWORD wanted = ((const DWORD*)target)[axis], current = ((DWORD*)&matrix.position)[axis];
        BYTE distant;
        const float* limit = axis < 2 ? &g_remoteVehicleAxisTeleportDistance : &g_frameScaleMultiplier;
        // clang-format off
        __asm {
            push current
            push wanted
            call FloatOffset
            mov ecx,limit
            fcomp dword ptr [ecx]
            add esp,8
            fnstsw ax
            test ah,41h
            sete distant
        }
        // clang-format on
        if (distant)
        {
            teleport = true;
            break;
        }
    }
    if (teleport)
    {
        matrix.position = *target;
        trailer->SetMatrix(matrix);
        trailer->SetSpeed(sync->speed);
        trailer->SetTurnSpeed(sync->turnSpeed);
        return;
    }
    trailer->SetMatrix(matrix);
    trailer->SetTurnSpeed(sync->turnSpeed);
    CVector speed = {0, 0, 0};
    trailer->GetSpeed(&speed);
    for (int axis = 0; axis < 3; ++axis)
    {
        const float *wanted = &target->x + axis, *current = &matrix.position.x + axis;
        float* value = &speed.x + axis;
        // clang-format off
        __asm {
            mov ecx,current
            push dword ptr [ecx]
            mov edx,wanted
            push dword ptr [edx]
            call FloatOffset
            fcomp dword ptr [g_pedDrunkSpeedThreshold]
            add esp,8
            fnstsw ax
            test ah,41h
            jne trailer_axis_done
            mov ecx,wanted
            mov edx,current
            mov eax,value
            fld dword ptr [ecx]
            fsub dword ptr [edx]
            fmul dword ptr [g_remoteTrailerCorrection]
            fadd dword ptr [eax]
            fstp dword ptr [eax]
        trailer_axis_done:
        }
        // clang-format on
    }
    trailer->SetSpeed(speed);
}
// 0x001181fc
const float g_remoteBulletOffsetMinimum = -300.0f;
// 0x00118200
const float g_remoteBulletOffsetMaximum = 300.0f;
// 0x00016010
void CRemotePlayer::UpdateBullet(const void* data)
{
    CPed* ped = m_state.ped;
    if (!ped || !ped->DoesExist())
        return;
    const BulletSyncData* sync = (const BulletSyncData*)data;
    PedBulletData bullet = {0};
    memcpy(&bullet.origin, &sync->origin, 12);
    memcpy(&bullet.target, &sync->target, 12);
    memcpy(&bullet.offset, &sync->offset, 12);
    BYTE type = sync->type;
    if (type)
    {
        for (int axis = 0; axis < 3; ++axis)
        {
            float* component = &bullet.offset.x + axis;
            BYTE accepted = 0;
            // clang-format off
            // Native accepts unordered offsets and includes both +/-300 endpoints.
            __asm {
                mov eax,component
                fld dword ptr [eax]
                fcomp dword ptr [g_remoteBulletOffsetMaximum]
                fnstsw ax
                test ah,41h
                jz bullet_offset_done
                mov eax,component
                fld dword ptr [eax]
                fcomp dword ptr [g_remoteBulletOffsetMinimum]
                fnstsw ax
                test ah,5
                jnp bullet_offset_done
                mov accepted,1
            bullet_offset_done:
            }
            // clang-format on
            if (!accepted) return;
        }
        if (type == 1)
        {
            CPlayerPool* players = ((CNetGame*)g_pNetGame)->pools->players;
            if (players)
            {
                WORD player = sync->targetId;
                if (player == players->localId)
                    bullet.entity = g_pGame->GetPlayerPed()->m_pGamePed;
                else
                {
                    if (player == m_state.id)
                        return;
                    if (players->IsConnected(player))
                    {
                        CPed* target = players->GetPlayer(player)->m_state.ped;
                        if (target)
                            bullet.entity = target->m_pGamePed;
                    }
                }
            }
        }
        else if (type == 2)
        {
            CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
            if (vehicles)
            {
                WORD vehicle = sync->targetId;
                if (vehicles->DoesExist(vehicle))
                    bullet.entity = vehicles->Get(vehicle)->m_pGameVehicle;
            }
        }
    }
    if (m_state.ped->DoesExist())
    {
        BYTE weapon = sync->weapon;
        if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
        {
            m_state.ped->SetArmedWeapon(weapon, 1);
            weapon = sync->weapon;
            if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
            {
                m_state.ped->GiveWeapon(weapon, 9999);
                m_state.ped->SetArmedWeapon(sync->weapon, 1);
            }
        }
    }
    m_state.ped->SetBulletData(&bullet);
    m_state.ped->FireWeapon();
}
// 0x00016230
void CRemotePlayer::SetState(BYTE state)
{
    BYTE previous = m_state.playerState;
    if (state != previous)
    {
        ChangeState(state, previous);
        m_state.playerState = state;
    }
}
// 0x00118218
const char g_remoteVendingAnimation[] = "VEND_EAT_P";
// 0x00118224
const char g_remoteVendingLibrary[] = "VENDING";
// 0x00118204
const char g_remoteCarryAnimation[] = "crry_prtial";
// 0x00118210
const char g_remoteCarryLibrary[] = "CARRY";
// 0x00016310
void CRemotePlayer::Process()
{
    if (!m_state.ped)
        return;
    if (!m_state.playerState)
    {
        ResetData();
        g_pGame->RemovePed(m_state.ped);
        m_state.ped = 0;
        return;
    }
    DWORD elapsed = GetTickCount() - m_state.lastUpdateTick;
    if (m_state.playerState == 17 && m_state.updateType == 16 && !m_state.ped->IsInVehicle())
    {
        if (!HasSurfingTarget())
        {
            SetOnfootTarget(&m_state.onfoot.position, &m_state.onfoot.speed);
            UpdateOnfootSpeedAndPosition();
        }
        else
            SetOnfootTarget(&m_state.onfoot.position, &m_state.onfoot.speed);
        if (m_state.ped->DoesExist())
        {
            BYTE weapon = m_state.onfoot.weaponAim & 0x3f;
            if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
            {
                m_state.ped->SetArmedWeapon(weapon, 0);
                weapon = m_state.onfoot.weaponAim & 0x3f;
                if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
                {
                    m_state.ped->GiveWeapon(weapon, 9999);
                    m_state.ped->SetArmedWeapon(m_state.onfoot.weaponAim & 0x3f, 0);
                }
            }
        }
        if (*(BYTE*)&m_state.onfoot.keys & 0x80)
        {
            if ((BYTE)m_state.ped->GetCurrentWeapon() == 34 || (BYTE)m_state.ped->GetCurrentWeapon() == 35 || (BYTE)m_state.ped->GetCurrentWeapon() == 36)
                m_state.ped->SetState(12);
        }
        else if (m_state.ped->GetState() == 12)
            m_state.ped->SetState(1);
        DWORD animation = m_state.onfoot.animation;
        if (m_state.appliedAnimation != animation)
        {
            if (animation)
                m_state.ped->ApplyPackedAnimation(animation);
            if (!m_state.onfoot.animation && m_state.appliedAnimation == 0x1804067c)
            {
                m_state.ped->ApplyAnimation(g_remoteVendingAnimation, g_remoteVendingLibrary, 4.0f, 0, 0, 0, 0, -1);
                m_state.onfoot.animation = 0xffffffff;
            }
            animation = m_state.onfoot.animation;
            m_state.appliedAnimation = animation;
            m_state.scriptedAnimation = 0;
        }
        if (!m_state.scriptedAnimation && !animation && m_state.ped->HasTaskType401())
            m_state.ped->ApplyAnimation(g_remoteCarryAnimation, g_remoteCarryLibrary, 4.0f, 0, 0, 0, 0, 0);
        m_state.vehicle = 0;
        m_state.updateType = 0;
    }
    else if (m_state.playerState == 19 && m_state.updateType == 17 && m_state.ped->IsInVehicle())
    {
        CVehicle* vehicle = m_state.vehicle;
        if (!vehicle || !vehicle->DoesExist())
            return;
        GameQuaternion* rotation = &m_state.incar.rotation;
        GameQuaternionNormalize(rotation);
        CMatrix matrix;
        GameQuaternionToMatrixComponents(rotation, &matrix);
        matrix.position = m_state.incar.position;
        if (m_state.vehicle->GetModelIndex() == 538 || m_state.vehicle->GetModelIndex() == 537 || m_state.vehicle->GetModelIndex() == 449)
            UpdateTrain(&matrix, &m_state.incar.speed, *(float*)&m_state.incar.modelData);
        else
        {
            SetIncarTarget(&matrix, &m_state.incar.position, &m_state.incar.speed);
            UpdateIncarSpeedAndPosition();
        }
        if (m_state.vehicle->GetModelIndex() == 520)
            m_state.vehicle->SetHydraThrusters(m_state.incar.modelData);
        if (m_state.vehicle->GetSubtype() == 2 || m_state.vehicle->GetSubtype() == 6)
            m_state.vehicle->SetBackendFloat64c(*(float*)&m_state.incar.modelData);
        m_state.vehicle->EnableSiren(m_state.incar.siren);
        m_state.vehicle->SetLandingGearState(m_state.incar.landingGear);
        m_state.vehicle->SetHealth(m_state.incar.health);
        WORD trailer = m_state.incar.trailer;
        if (!trailer || trailer >= 2000)
        {
            if (m_state.vehicle->GetTrailer())
            {
                m_state.vehicle->DetachTrailer();
                m_state.vehicle->SetTrailer(0);
            }
        }
        BYTE weapon = m_state.incar.weaponAim & 0x3f;
        if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
        {
            m_state.ped->SetArmedWeapon(weapon, 0);
            weapon = m_state.incar.weaponAim & 0x3f;
            if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
            {
                m_state.ped->GiveWeapon(weapon, 9999);
                m_state.ped->SetArmedWeapon(m_state.incar.weaponAim & 0x3f, 0);
            }
        }
        m_state.updateType = 0;
    }
    else if (m_state.playerState == 18 && m_state.updateType == 18 && m_state.ped->IsInVehicle())
    {
        CVehicle* vehicle = m_state.vehicle;
        if (!vehicle || !vehicle->DoesExist())
            return;
        if (m_state.ped->DoesExist())
        {
            BYTE weapon = m_state.passenger.weaponAim & 0x3f;
            if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
            {
                m_state.ped->SetArmedWeapon(weapon, 0);
                weapon = m_state.passenger.weaponAim & 0x3f;
                if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
                {
                    m_state.ped->GiveWeapon(weapon, 9999);
                    m_state.ped->SetArmedWeapon(m_state.passenger.weaponAim & 0x3f, 0);
                }
            }
        }
        if (m_state.ped->GetState() == 50)
        {
            if ((m_state.passenger.seatFlags & 0x40) && !m_state.ped->HasTaskType1022() && m_state.ped->DoesExist() && m_state.ped->EnablePassengerDrivebyMode())
                m_state.passengerDriveby = 1;
            if (!(m_state.passenger.seatFlags & 0x40) && m_state.ped->HasTaskType1022() && m_state.ped->DoesExist())
            {
                m_state.ped->StopTaskType1022();
                m_state.passengerDriveby = 0;
            }
        }
        if ((m_state.passenger.seatFlags & 0x80) && !m_state.ped->HasDanceState())
            m_state.ped->SetDancePoseState(1);
        else if (!(m_state.passenger.seatFlags & 0x80) && m_state.ped->HasDanceState())
            m_state.ped->SetDancePoseState(0);
        m_state.updateType = 0;
    }
    if (m_state.playerState == 17 && !m_state.ped->IsInVehicle())
    {
        UpdateOnfootRotation();
        ProcessHead();
        ProcessSpecialAction();
        m_state.passengerDriveby = 0;
        m_state.ped->SetKeys(m_state.onfoot.keys, m_state.onfoot.leftRight, m_state.onfoot.upDown);
        if (HasSurfingTarget())
        {
            if (m_state.onfoot.surfingVehicle < 2000)
            {
                m_state.ped->SetCollisionProcessed(FALSE);
                m_state.ped->SetCollisionFlag(FALSE);
            }
        }
        else
        {
            BOOL collision = m_state.specialAction != 3;
            m_state.ped->SetCollisionProcessed(collision);
            m_state.ped->SetCollisionFlag(collision);
        }
        CVector* speed = &m_state.onfoot.speed;
        if (speed->x == 0 && speed->y == 0 && speed->z == 0)
        {
            m_state.ped->SetSpeed(*speed);
            if (m_state.status != 2 && (int)elapsed >= 3000)
                m_state.status = 2;
        }
        else if (m_state.status != 2 && (int)elapsed >= 1500)
            m_state.status = 2;
        if (GetTickCount() - m_state.lastUpdateTick > 3000)
        {
            m_state.onfoot.leftRight = 0;
            m_state.onfoot.upDown = 0;
            CVector zero = {0, 0, 0};
            m_state.ped->SetSpeed(zero);
            CMatrix matrix;
            m_state.ped->GetMatrix(&matrix);
            matrix.position = m_state.onfoot.position;
            m_state.ped->SetMatrix(matrix);
        }
    }
    else if (m_state.playerState == 19 && m_state.ped->IsInVehicle())
    {
        if (m_state.vehicle && m_state.vehicle->GetModelIndex() != 538 && m_state.vehicle->GetModelIndex() != 537 && m_state.vehicle->GetModelIndex() != 449)
            UpdateIncarRotation();
        m_state.passengerDriveby = 0;
        m_state.ped->DestroyParachute();
        if (m_state.vehicle && m_state.vehicle->IsRC())
            m_state.ped->Remove();
        CVector* speed = &m_state.incar.speed;
        if (m_state.rawFlag1c && speed->x == 0 && speed->y == 0 && speed->z == 0 && m_state.vehicle)
            m_state.vehicle->SetSpeed(*speed);
        m_state.ped->SetKeys(m_state.incar.keys, m_state.incar.leftRight, m_state.incar.upDown);
        m_state.ped->ProcessVehicleHorn();
        if (speed->x == 0 && speed->y == 0 && speed->z == 0)
        {
            if (m_state.status != 2 && (int)elapsed > 3000)
                m_state.status = 2;
        }
        else if (m_state.status != 2 && (int)elapsed > 1500)
            m_state.status = 2;
        ProcessHead();
    }
    else if (m_state.playerState == 18 && m_state.ped->IsInVehicle())
    {
        m_state.ped->DestroyParachute();
        m_state.ped->SetKeys(m_state.passenger.keys, 0, 0);
        if (m_state.status != 2 && (int)elapsed >= 3000)
            m_state.status = 2;
        ProcessHead();
    }
    else
    {
        CVector zero = {0, 0, 0};
        m_state.ped->SetSpeed(zero);
        m_state.ped->SetKeys(0, 0, 0);
        m_state.passengerDriveby = 0;
    }
    if (m_state.playerState != 32)
        m_state.ped->SetHealth(1000.0f);
    if (m_state.status == 2 && (int)elapsed < 1500)
        m_state.status = 0;
}
// 0x00117874
const float g_remoteHeadDistance = 20.0f;
// 0x00117868
const char g_remoteHeadTask[] = "FollowPedSA";
// 0x00014240
void CRemotePlayer::ProcessHead()
{
    CPed* ped = m_state.ped;
    if (!ped || !ped->DoesExist() || m_state.rawFlag1c)
        return;
    if (g_pGame && !*(DWORD*)((BYTE*)g_pGame + 0x59))
        return;
    if (m_state.ped->GetStuff())
        return;
    CMatrix matrix;
    m_state.ped->GetMatrix(&matrix);
    DWORD now = GetTickCount();
    CRemotePlayer* self = this;
    if (now - m_state.lastHeadUpdateTick > 1000)
    {
        CameraAim* aim = AimStuff::GetPlayerAim(*((BYTE*)m_state.ped + 0x2b0));
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
            mov [ecx+1e1h],edx
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+56]
            fstp dword ptr [z]
            fxch st(1)
            fsub dword ptr [matrix+48]
            fstp dword ptr [ecx+1d5h]
            fsub dword ptr [matrix+52]
            fstp dword ptr [ecx+1d9h]
            fld dword ptr [z]
            fsub dword ptr [matrix+56]
            fstp dword ptr [ecx+1ddh]
        }
        // clang-format on
    }
    if (now - m_state.lastHeadTaskTick > 200)
    {
        CVector target;
        ped = m_state.ped;
        // clang-format off
        __asm {
            mov ecx,self
            fld dword ptr [matrix+48]
            fadd dword ptr [ecx+1d5h]
            fstp dword ptr [target]
            fld dword ptr [matrix+52]
            fadd dword ptr [ecx+1d9h]
            fstp dword ptr [target+4]
            fld dword ptr [matrix+56]
            fadd dword ptr [ecx+1ddh]
            fstp dword ptr [target+8]
        }
        // clang-format on
        ped->ApplyCommandTask(g_remoteHeadTask, 0, 1000, -1, &target, 0, 0.1f, 500, 3, 0);
        m_state.lastHeadTaskTick = now;
    }
}
// 0x00014390
void CRemotePlayer::SetMarkerState(BOOL enabled)
{
    if (!enabled)
    {
        DWORD marker = m_state.marker;
        if (marker)
        {
            g_pGame->DeleteMarker(marker);
            m_state.marker = 0;
        }
    }
    m_state.markerEnabled = enabled;
}
// 0x000143d0
void CRemotePlayer::SetMarkerPosition(short x, short y, short z)
{
    m_state.markerEnabled = 1;
    DWORD marker = m_state.marker;
    if (marker)
    {
        g_pGame->DeleteMarker(marker);
        m_state.marker = 0;
    }
    if (m_state.ped)
        return;
    marker = g_pGame->CreateMarker(0, (float)x, (float)y, (float)z, m_state.id, 0);
    m_state.markerZ = z;
    m_state.markerX = x;
    m_state.markerY = y;
    m_state.marker = marker;
}
// 0x00014470
BOOL CRemotePlayer::HasSurfingTarget()
{
    WORD id = m_state.onfoot.surfingVehicle;
    return m_state.playerState == 17 && id && id != 0xffff && id < 4100 ? TRUE : FALSE;
}
// 0x000144a0
BOOL CRemotePlayer::SurfingOnObject()
{
    WORD id = m_state.onfoot.surfingVehicle;
    return m_state.playerState == 17 && id && id != 0xffff && id >= 2000 && id < 4100 ? TRUE : FALSE;
}
// 0x00014780
void CRemotePlayer::ProcessSpecialAction()
{
    CPed* ped = m_state.ped;
    if (!ped || !ped->DoesExist())
        return;
    if (m_state.specialAction == 1 && !m_state.ped->IsDucking())
    {
        m_state.ped->ClearTasksIfOnFoot();
        return;
    }
    if (m_state.specialAction != 1 && m_state.ped->IsDucking())
        m_state.ped->ClearDuckingFlag();
    if (m_state.specialAction == 11 && !m_state.ped->UsingCellphone())
    {
        m_state.ped->EnableCellphone(TRUE);
        return;
    }
    if (m_state.specialAction != 11 && m_state.ped->UsingCellphone())
    {
        m_state.ped->EnableCellphone(FALSE);
        return;
    }
    if (m_state.specialAction == 2 && !m_state.ped->HasJetpack())
    {
        m_state.jetpackState = 1;
        m_state.ped->EnableJetpack();
        return;
    }
    if (m_state.specialAction != 2 && m_state.ped->HasJetpack())
    {
        m_state.jetpackState = 0;
        m_state.ped->DisableJetpack();
        return;
    }
    if (m_state.specialAction == 10 && !m_state.ped->DoesHandsUp())
        m_state.ped->HandsUp();
    if (m_state.specialAction != 10 && m_state.ped->DoesHandsUp())
        m_state.ped->StopDancing();
    if (!m_state.ped->DoesUrinating() && m_state.specialAction == 68)
        m_state.ped->StartUrinating();
    if (m_state.ped->DoesUrinating() && m_state.specialAction != 68)
        m_state.ped->StopUrinating();
    if (m_state.specialAction == 20 && m_state.ped->GetStuff() != 1)
        m_state.ped->GiveStuff(1);
    if (m_state.specialAction == 22 && m_state.ped->GetStuff() != 2)
        m_state.ped->GiveStuff(2);
    if (m_state.specialAction == 23 && m_state.ped->GetStuff() != 3)
        m_state.ped->GiveStuff(3);
    if (m_state.specialAction == 21 && m_state.ped->GetStuff() != 4)
        m_state.ped->GiveStuff(4);
    if (m_state.ped->GetStuff())
    {
        BYTE action = m_state.specialAction;
        if (action == 20 || action == 22 || action == 23 || action == 21)
            m_state.ped->ApplyStuff();
        else
            m_state.ped->DropStuff();
    }
    if (!m_state.ped->HasDanceState() && m_state.specialAction == 24)
        m_state.ped->SetDancePoseState(1);
    if (m_state.ped->HasDanceState() && m_state.specialAction != 24 && m_state.specialAction != 3 && m_state.specialAction != 4)
        m_state.ped->SetDancePoseState(0);
    if (!m_state.ped->HasDanceStyle() && m_state.specialAction == 25)
        m_state.ped->SetSecondaryPoseState(1);
    if (m_state.ped->HasDanceStyle() && m_state.specialAction != 25 && m_state.specialAction != 3 && m_state.specialAction != 4)
        m_state.ped->SetSecondaryPoseState(0);
    m_state.ped->CheckParachuteWeapon();
}
// 0x0013c498
float g_remoteOnfootCorrection = 0.1f;
// 0x0013c49c
float g_remoteIncarCorrection = 0.06f;
// 0x0011797c
const float g_remoteIncarTeleportDistance = 8.0f;
// 0x000149f0
void CRemotePlayer::UpdateOnfootSpeedAndPosition()
{
    CPed* ped = m_state.ped;
    if (!ped)
        return;
    CMatrix matrix;
    ped->GetMatrix(&matrix);
    CVector* target = &m_state.onfootTargetPosition;
    if (!m_state.ped->DoesExist())
    {
        matrix.position = *target;
        m_state.ped->SetMatrix(matrix);
        return;
    }
    volatile float* delta = (volatile float*)&m_state.positionInaccuracy;
    delta[0] = FloatOffset(target->x, matrix.position.x);
    delta[1] = FloatOffset(target->y, matrix.position.y);
    delta[2] = FloatOffset(target->z, matrix.position.z);
    if (delta[0] <= g_localQuaternionSyncThreshold && delta[1] <= g_localQuaternionSyncThreshold && delta[2] <= g_localQuaternionSyncThreshold)
        return;
    if (delta[0] > g_quaternionMatrixTwo || delta[1] > g_quaternionMatrixTwo || delta[2] > g_quaternionMatrixOne)
    {
        matrix.position = *target;
        m_state.ped->SetMatrix(matrix);
        return;
    }
    CVector speed;
    m_state.ped->GetSpeed(&speed);
    CRemotePlayer* self = this;
    // clang-format off
    __asm {
        mov ecx,self
        fld dword ptr [g_remoteOnfootCorrection]
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_localQuaternionSyncThreshold]
        fnstsw ax
        test ah,41h
        jne onfoot_y_correction
        fld dword ptr [ecx+120h]
        fsub dword ptr [matrix+48]
        fmul st(0),st(1)
        fadd dword ptr [speed]
        fstp dword ptr [speed]
    onfoot_y_correction:
        fld dword ptr [ecx+194h]
        fcomp dword ptr [g_localQuaternionSyncThreshold]
        fnstsw ax
        test ah,41h
        jne onfoot_z_correction
        fld dword ptr [ecx+124h]
        fsub dword ptr [matrix+52]
        fmul st(0),st(1)
        fadd dword ptr [speed+4]
        fstp dword ptr [speed+4]
    onfoot_z_correction:
        fld dword ptr [ecx+198h]
        fcomp dword ptr [g_localQuaternionSyncThreshold]
        fnstsw ax
        test ah,41h
        jne onfoot_correction_done
        fld dword ptr [ecx+128h]
        fsub dword ptr [matrix+56]
        fmul st(0),st(1)
        fadd dword ptr [speed+8]
        fstp dword ptr [speed+8]
    onfoot_correction_done:
        fstp st(0)
    }
    // clang-format on
    m_state.ped->SetSpeed(speed);
}
// 0x00014c00
void CRemotePlayer::UpdateOnfootXYSpeedAndPosition()
{
    CPed* ped = m_state.ped;
    if (!ped)
        return;
    CMatrix matrix;
    ped->GetMatrix(&matrix);
    CVector* target = &m_state.onfootTargetPosition;
    if (!m_state.ped->DoesExist())
    {
        matrix.position.x = target->x;
        matrix.position.y = target->y;
        m_state.ped->SetMatrix(matrix);
        return;
    }
    volatile float* delta = (volatile float*)&m_state.positionInaccuracy;
    delta[0] = FloatOffset(target->x, matrix.position.x);
    int action = 0;
    CRemotePlayer* self = this;
    // clang-format off
    __asm {
        mov ecx,self
        push dword ptr [matrix+52]
        push dword ptr [ecx+124h]
        call FloatOffset
        add esp,8
        mov ecx,self
        fst dword ptr [ecx+194h]
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_objectTurnVelocityScale]
        fnstsw ax
        test ah,41h
        jp xy_significant
        fcom dword ptr [g_objectTurnVelocityScale]
        fnstsw ax
        test ah,41h
        jnp xy_discard_y
    xy_significant:
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_quaternionMatrixTwo]
        fnstsw ax
        test ah,41h
        je xy_teleport_pop
        fcomp dword ptr [g_quaternionMatrixTwo]
        fnstsw ax
        test ah,41h
        je xy_teleport
        mov action,1
        jmp xy_decided
    xy_teleport_pop:
        fstp st(0)
    xy_teleport:
        mov action,2
        jmp xy_decided
    xy_discard_y:
        fstp st(0)
    xy_decided:
    }
    // clang-format on
    if (!action) return;
    if (action == 2)
    {
        matrix.position.x = target->x;
        matrix.position.y = target->y;
        m_state.ped->SetMatrix(matrix);
        return;
    }
    CVector speed;
    m_state.ped->GetSpeed(&speed);
    for (int i = 0; i < 2; ++i)
        if (delta[i] > g_objectPoolSecondsPerTick)
        {
            float *wanted = (float*)target + i, *current = (float*)&matrix.position + i, *value = (float*)&speed + i;
            // clang-format off
        __asm {
            mov ecx,wanted
            mov edx,current
            mov eax,value
            fld dword ptr [ecx]
            fsub dword ptr [edx]
            fmul dword ptr [g_objectMutationStep]
            fadd dword ptr [eax]
            fstp dword ptr [eax]
        }
            // clang-format on
        }
    m_state.ped->SetSpeed(speed);
}
// 0x00014ec0
void CRemotePlayer::UpdateIncarSpeedAndPosition()
{
    CVector speed = {0, 0, 0};
    CVehicle* vehicle = m_state.vehicle;
    if (!vehicle)
        return;
    CMatrix matrix;
    vehicle->GetMatrix(&matrix);
    CVector* target = &m_state.incarTargetPosition;
    if (!m_state.vehicle->DoesExist())
    {
        matrix.position = *target;
        m_state.vehicle->SetMatrix(matrix);
        return;
    }
    volatile float* delta = (volatile float*)&m_state.positionInaccuracy;
    delta[0] = FloatOffset(target->x, matrix.position.x);
    delta[1] = FloatOffset(target->y, matrix.position.y);
    int significant = 0;
    CRemotePlayer* self = this;
    // clang-format off
    __asm {
        mov ecx,self
        push dword ptr [matrix+56]
        push dword ptr [ecx+140h]
        call FloatOffset
        add esp,8
        mov ecx,self
        fst dword ptr [ecx+198h]
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_pedDrunkSpeedThreshold]
        fnstsw ax
        test ah,41h
        jp incar_significant_pop_z
        fld dword ptr [ecx+194h]
        fcomp dword ptr [g_pedDrunkSpeedThreshold]
        fnstsw ax
        test ah,41h
        jp incar_significant_pop_z
        fcomp dword ptr [g_pedDrunkSpeedThreshold]
        fnstsw ax
        test ah,41h
        jnp incar_significance_done
        jmp incar_significant
    incar_significant_pop_z:
        fstp st(0)
    incar_significant:
        mov significant,1
    incar_significance_done:
    }
    // clang-format on
    if (!significant) return;
    float zLimit = 0.5f;
    if (m_state.vehicle->GetSubtype() == 4 || m_state.vehicle->GetSubtype() == 5 || m_state.vehicle->GetSubtype() == 3)
        zLimit = 2.0f;
    if (delta[0] > g_remoteIncarTeleportDistance || delta[1] > g_remoteIncarTeleportDistance || delta[2] > zLimit)
    {
        matrix.position = *target;
        m_state.vehicle->SetMatrix(matrix);
        m_state.vehicle->SetSpeed(m_state.incarTargetSpeed);
        return;
    }
    m_state.vehicle->GetSpeed(&speed);
    for (int i = 0; i < 3; ++i)
        if (delta[i] > g_pedDrunkSpeedThreshold)
        {
            float *wanted = (float*)target + i, *current = (float*)&matrix.position + i, *value = (float*)&speed + i;
            // clang-format off
        __asm {
            mov ecx,wanted
            mov edx,current
            mov eax,value
            fld dword ptr [ecx]
            fsub dword ptr [edx]
            fmul dword ptr [g_remoteIncarCorrection]
            fadd dword ptr [eax]
            fstp dword ptr [eax]
        }
            // clang-format on
        }
    if (FloatOffset(speed.x, 0) > g_objectTurnVelocityScale || FloatOffset(speed.y, 0) > g_objectTurnVelocityScale || FloatOffset(speed.z, 0) > g_objectTurnVelocityScale)
        m_state.vehicle->SetSpeed(speed);
}
// 0x0013c4a0
DWORD g_remoteClampTurnSpeed = 1;
// 0x001181bc
const float g_remoteTurnSpeedMinimum = -0.02f;
// 0x00014d80
void CRemotePlayer::UpdateOnfootRotation()
{
    CPed* ped = m_state.ped;
    if (!ped)
        return;
    CMatrix matrix;
    GameQuaternion current, result;
    ped->GetMatrix(&matrix);
    GameQuaternionFromMatrixComponents(&matrix, &current);
    GameQuaternionSlerp(&result, &m_state.onfoot.rotation, &current, 0.75f);
    GameQuaternionToMatrixComponents(&result, &matrix);
    m_state.ped->SetMatrix(matrix);
    float angle;
    // clang-format off
    __asm {
        fld dword ptr [matrix+16]
        fchs
        fld dword ptr [matrix+20]
        fpatan
        fmul dword ptr [g_entityRadiansToDegrees]
        fst dword ptr [angle]
        fcomp dword ptr [g_objectScaleLowerBound]
        fld dword ptr [angle]
        fnstsw ax
        test ah,5
        jp onfoot_heading_nonnegative
        fadd dword ptr [g_anglePeriod]
        jmp onfoot_heading_store
    onfoot_heading_nonnegative:
        fcomp dword ptr [g_anglePeriod]
        fnstsw ax
        test ah,1
        jne onfoot_heading_ready
        fld dword ptr [angle]
        fsub dword ptr [g_anglePeriod]
    onfoot_heading_store:
        fstp dword ptr [angle]
    onfoot_heading_ready:
    }
    // clang-format on
    m_state.ped->SetRotation(angle);
}
// 0x000151c0
void CRemotePlayer::UpdateIncarRotation()
{
    CVector turnSpeed = {0, 0, 0};
    CVehicle* vehicle = m_state.vehicle;
    if (!vehicle)
        return;
    if (g_remoteClampTurnSpeed)
    {
        vehicle->GetTurnSpeed(&turnSpeed);
        float* values = (float*)&turnSpeed;
        for (int i = 0; i < 3; ++i)
        {
            if (values[i] > g_pedDrunkShakeScale)
                values[i] = g_pedDrunkShakeScale;
            else if (values[i] < g_remoteTurnSpeedMinimum)
                values[i] = g_remoteTurnSpeedMinimum;
        }
        m_state.vehicle->SetTurnSpeed(turnSpeed);
    }
    CMatrix matrix;
    GameQuaternion current, result;
    m_state.vehicle->GetMatrix(&matrix);
    GameQuaternionFromMatrix(&matrix, &current);
    GameQuaternionSlerp(&result, &m_state.incarTargetRotation, &current, 0.75f);
    GameQuaternionNormalize(&result);
    GameQuaternionToMatrix(&result, &matrix);
    m_state.vehicle->SetMatrix(matrix);
}
// 0x000153a0
void CRemotePlayer::UpdateTrain(const CMatrix* target, const CVector* targetSpeed, float trainSpeed)
{
    if (!m_state.ped || !m_state.vehicle)
        return;
    CMatrix current;
    m_state.vehicle->GetMatrix(&current);
    bool moveX = FloatOffset(target->position.x, current.position.x) > g_objectRotationFactor;
    bool moveY = FloatOffset(target->position.y, current.position.y) > g_objectRotationFactor;
    if (moveX || moveY)
        m_state.vehicle->Teleport(target->position);
    CVector speed;
    m_state.vehicle->GetSpeed(&speed);
    speed = *targetSpeed;
    m_state.vehicle->SetSpeed(speed);
    m_state.vehicle->SetTrainSpeed(trainSpeed);
}
// 0x00014e50
void CRemotePlayer::SetOnfootTarget(const CVector* position, const CVector* speed)
{
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.onfootTargetPosition + i) = *(const DWORD*)((const BYTE*)position + i);
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.onfootTargetSpeed + i) = *(const DWORD*)((const BYTE*)speed + i);
    m_state.ped->SetSpeed(m_state.onfootTargetSpeed);
}
// 0x00015330
void CRemotePlayer::SetIncarTarget(const CMatrix* matrix, const CVector* position, const CVector* speed)
{
    GameQuaternionFromMatrix(matrix, &m_state.incarTargetRotation);
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.incarTargetPosition + i) = *(const DWORD*)((const BYTE*)position + i);
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.incarTargetSpeed + i) = *(const DWORD*)((const BYTE*)speed + i);
    m_state.vehicle->SetSpeed(*speed);
}
// 0x001181c8
const float g_remoteAimZoomScale = 0.01587301678955555f;
// 0x001181cc
const float g_remoteAimAspectScale = 0.003921568859368563f;
// 0x00016c50
BOOL CRemotePlayer::Spawn(BYTE team, int model, int customModel, const CVector* position, float rotation, DWORD color, BYTE fightingStyle)
{
    (void)team;
    if (!g_pGame->IsStarted())
        return FALSE;
    CPed* ped = m_state.ped;
    if (ped)
    {
        g_pGame->RemovePed(ped);
        m_state.ped = 0;
    }
    ped = g_pGame->CreatePed(model, *position, rotation, 1, (BOOL)m_state.rawFlag1c);
    if (!ped)
    {
        if (m_state.playerState)
            m_state.playerState = 0;
        return FALSE;
    }
    if (g_customModels && customModel > 20000)
    {
        if (GameIsPedModel(customModel) || g_customModels->DoesExist(customModel))
            ped->SetModelIndex(customModel);
    }
    if (color)
        PlayerColors::Set(m_state.id, color);
    DWORD marker = m_state.marker;
    if (marker)
    {
        g_pGame->DeleteMarker(marker);
        m_state.marker = 0;
    }
    if (*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 0x30))
        ped->CreateArrow(m_state.id);
    m_state.ped = ped;
    m_state.reportedHealth = 100.0f;
    ped->SetKeys(0, 0, 0);
    if (fightingStyle != 4)
        ped->SetFightingStyle(fightingStyle);
    if (m_state.playerState != 33)
        m_state.playerState = 33;
    return TRUE;
}
// 0x00014660
void CRemotePlayer::OnEnterVehicle()
{
    CVehiclePool* vehicles = ((CNetGame*)g_pNetGame)->pools->vehicles;
    CPed* ped = m_state.ped;
    if (!ped || ped->IsInVehicle())
        return;
    CVehicle* vehicle = vehicles->Get(m_state.vehicleId);
    if (!vehicle)
        return;
    BYTE weapon = (m_state.seat ? m_state.passenger.weaponAim : m_state.incar.weaponAim) & 0x3f;
    if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
    {
        m_state.ped->SetArmedWeapon(weapon, 0);
        if ((BYTE)m_state.ped->GetCurrentWeapon() != weapon)
        {
            m_state.ped->GiveWeapon(weapon, 9999);
            m_state.ped->SetArmedWeapon(weapon, 0);
        }
    }
    DWORD ref = vehicles->GetRef((int)m_state.vehicleId);
    BYTE seat = m_state.seat;
    m_state.ped->PutIntoVehicle(ref, seat);
}
// 0x00016da0
void CRemotePlayer::UpdateOnfoot(const void* data, DWORD timestamp)
{
    const BYTE* input = (const BYTE*)data;
    const LocalOnfootSyncData* sync = (const LocalOnfootSyncData*)data;
    m_state.vehicle = 0;
    if (timestamp && (int)(timestamp - m_state.lastNetworkTimestamp) < 0)
        return;
    m_state.lastNetworkTimestamp = timestamp;
    for (int i = 0; i < 68; i += 4)
        *(DWORD*)((BYTE*)&m_state.onfoot + i) = *(const DWORD*)(input + i);
    m_state.reportedHealth = (float)sync->health;
    m_state.reportedArmour = (float)sync->armour;
    m_state.updateType = 16;
    m_state.lastUpdateTick = GetTickCount();
    m_state.specialAction = sync->specialAction;
    if (*(int*)&m_state.onfoot.animation < 0)
        m_state.onfoot.animation = 0;
    CPed* ped = m_state.ped;
    if (ped && ped->IsInVehicle() && m_state.specialAction != 3 && m_state.specialAction != 4 && !m_state.ped->HasTaskType704())
        OnExitVehicle();
    if (m_state.playerState != 17)
        m_state.playerState = 17;
}
// 0x00016e70
void CRemotePlayer::UpdateIncar(const void* data, DWORD timestamp)
{
    const BYTE* input = (const BYTE*)data;
    const LocalIncarSyncData* sync = (const LocalIncarSyncData*)data;
    if (timestamp && (int)(timestamp - m_state.lastNetworkTimestamp) < 0)
        return;
    m_state.lastNetworkTimestamp = timestamp;
    for (int i = 0; i < 60; i += 4)
        *(DWORD*)((BYTE*)&m_state.incar + i) = *(const DWORD*)(input + i);
    *(WORD*)((BYTE*)&m_state.incar.modelData + 1) = *(const WORD*)(input + 60);
    *((BYTE*)&m_state.incar.modelData + 3) = input[62];
    m_state.vehicleId = sync->vehicle;
    m_state.vehicle = ((CNetGame*)g_pNetGame)->pools->vehicles->Get(sync->vehicle);
    m_state.seat = 0;
    m_state.reportedHealth = (float)sync->pedHealth;
    m_state.reportedArmour = (float)sync->armour;
    m_state.updateType = 17;
    m_state.lastUpdateTick = GetTickCount();
    m_state.specialAction = 0;
    CPed* ped = m_state.ped;
    if (ped && !ped->IsInVehicle())
        OnEnterVehicle();
    BYTE oldState = m_state.playerState;
    if (oldState != 19)
    {
        ChangeState(19, oldState);
        m_state.playerState = 19;
    }
}
// 0x00016f60
void CRemotePlayer::UpdatePassenger(const void* data)
{
    const BYTE* input = (const BYTE*)data;
    const LocalPassengerSyncData* sync = (const LocalPassengerSyncData*)data;
    for (int i = 0; i < 24; i += 4)
        *(DWORD*)((BYTE*)&m_state.passenger + i) = *(const DWORD*)(input + i);
    m_state.vehicleId = sync->vehicle;
    m_state.seat = sync->seatFlags & 0x3f;
    m_state.vehicle = ((CNetGame*)g_pNetGame)->pools->vehicles->Get(m_state.vehicleId);
    m_state.reportedHealth = (float)sync->health;
    m_state.reportedArmour = (float)sync->armour;
    m_state.updateType = 18;
    m_state.lastUpdateTick = GetTickCount();
    m_state.specialAction = 0;
    CPed* ped = m_state.ped;
    if (ped && !ped->IsInVehicle())
        OnEnterVehicle();
    if (m_state.playerState != 18)
        m_state.playerState = 18;
}
// 0x000154a0
void CRemotePlayer::UpdateAim(const void* data)
{
    const LocalAimSyncData* sync = (const LocalAimSyncData*)data;
    CPed* ped = m_state.ped;
    if (!ped)
        return;
    AimStuff::SetPlayerCameraMode(sync->cameraMode, *((BYTE*)ped + 0x2b0));
    CameraAim aim;
    memcpy(&aim.front, sync->vectors, 12);
    CVector direction;
    memcpy(&direction, sync->vectors, 12);
    CVector up = {0, 0, 0};
    GameBuildAimUpVector(&direction, &up);
    memcpy(&aim.up, &up, 12);
    memcpy(&aim.source, sync->vectors + 12, 12);
    memcpy(&aim.sourceBeforeLookBehind, sync->vectors + 12, 12);
    m_state.ped->SetAim(&aim);
    m_state.ped->SetAimZ(sync->aimZ);
    int aspect = sync->aspect, zoom = sync->zoomWeapon & 0x3f;
    float aspectValue, zoomValue;
    // clang-format off
    __asm {
        fild dword ptr [aspect]
        fmul dword ptr [g_remoteAimAspectScale]
        fstp dword ptr [aspectValue]
        fild dword ptr [zoom]
        fmul dword ptr [g_remoteAimZoomScale]
        fstp dword ptr [zoomValue]
    }
    // clang-format on
    ped = m_state.ped;
    AimStuff::SetPlayerCamera(*((BYTE*)ped + 0x2b0), zoomValue, aspectValue);
    DWORD* slot = (DWORD*)m_state.ped->GetCurrentWeaponSlot();
    BYTE weapon = sync->zoomWeapon >> 6;
    if (weapon == 3)
        slot[1] = 2;
    else if (weapon != 2)
        slot[2] = weapon;
    else if (slot[2] < 2)
        slot[2] = 2;
}
// 0x00014730
void CRemotePlayer::OnExitVehicle()
{
    CPed* ped = m_state.ped;
    if (ped && ped->IsInVehicle())
    {
        CMatrix matrix;
        m_state.ped->GetMatrix(&matrix);
        m_state.ped->WarpFromVehicle(matrix.position.x, matrix.position.y, matrix.position.z);
        m_state.vehicle = 0;
    }
}
// 0x00017120
void CRemotePlayer::Chat(char* text)
{
    WORD id = m_state.id;
    const char* name = ((CNetGame*)g_pNetGame)->pools->players->GetName(id);
    DWORD color = (PlayerColors::Get(id) >> 8) | 0xff000000;
    g_pChat->AddChatMessage(name, color, text);
}
// 0x001181e4
const float g_remoteAbsentDistance = 10000.0f;
// 0x001181e8
const char g_remoteCarJackedText[] = "~r~Car Jacked~w~!";
// 0x00016250
CRemotePlayer::CRemotePlayer()
{
    DWORD(WINAPI * tick)() = GetTickCount;
    m_state.vehicleId = 0;
    m_state.playerState = 0;
    m_state.id = 0xffff;
    m_state.ped = 0;
    m_state.updateType = 0;
    m_state.team = 0xff;
    m_state.showNameTag = 1;
    m_state.unknown0c = 0;
    m_state.jetpackState = 0;
    m_state.marker = 0;
    m_state.specialAction = 0;
    m_state.rawFlag1c = 0;
    m_state.scriptedAnimation = 0;
    m_state.lastHeadUpdateTick = tick();
    m_state.lastHeadTaskTick = tick();
    m_state.lastUpdateTick = tick();
    ResetData();
}
// 0x000162c0
CRemotePlayer::~CRemotePlayer()
{
    DWORD marker = m_state.marker;
    if (marker)
    {
        g_pGame->DeleteMarker(marker);
        m_state.marker = 0;
    }
    CPed* ped = m_state.ped;
    m_state.markerEnabled = 0;
    if (ped)
    {
        g_pGame->RemovePed(ped);
        m_state.ped = 0;
    }
}
// 0x00015c60
void CRemotePlayer::ResetData()
{
    m_state.vehicleId = 0;
    *(DWORD*)&m_state.reportedHealth = 0;
    *(DWORD*)&m_state.reportedArmour = 0;
    m_state.vehicle = 0;
    m_state.seat = 0;
    m_state.passengerDriveby = 0;
    m_state.markerX = 0;
    m_state.markerY = 0;
    m_state.markerZ = 0;
    m_state.specialAction = 0;
    m_state.appliedAnimation = 0;
    m_state.status = 2;
    memset(&m_state.onfoot, 0, 68);
    memset(&m_state.incar, 0, 63);
    memset(&m_state.passenger, 0, 24);
    memset(m_state.unknownf5, 0, 31);
    memset(&m_state.headDirection, 0, 12);
    DWORD marker = m_state.marker;
    if (marker)
    {
        g_pGame->DeleteMarker(marker);
        m_state.marker = 0;
    }
    m_state.markerEnabled = 0;
    if (g_pNetGame)
        m_state.lastNetworkTimestamp = (DWORD)((CNetGame*)g_pNetGame)->GetCounter();
}
// 0x00017040
void CRemotePlayer::Remove()
{
    if (m_state.ped)
    {
        ResetData();
        g_pGame->RemovePed(m_state.ped);
        m_state.ped = 0;
    }
    m_state.playerState = 0;
    m_state.showNameTag = 1;
}
// 0x00017080
void CRemotePlayer::Kill()
{
    if (m_state.ped)
    {
        m_state.ped->SetKeys(0, 0, 0);
        m_state.ped->Kill();
    }
    if (m_state.playerState != 32)
        m_state.playerState = 32;
    ResetData();
}
// 0x00015e00
void CRemotePlayer::SetColor(DWORD color)
{
    PlayerColors::Set(m_state.id, color);
}
// 0x00015e20
DWORD CRemotePlayer::GetColorAsRGBA()
{
    return PlayerColors::Get(m_state.id);
}
// 0x00015d50
__declspec(naked) float CRemotePlayer::GetDistanceToPlayer(CRemotePlayer* other)
{
    // clang-format off
    __asm {
        mov ecx,dword ptr [ecx+4]
        sub esp,80h
        test ecx,ecx
        jnz have_ped
        fld g_remoteAbsentDistance
        add esp,80h
        ret 4
    have_ped:
        lea eax,[esp]
        push eax
        call CEntity::GetMatrix
        mov edx,dword ptr [esp+84h]
        lea ecx,[esp+40h]
        push ecx
        mov ecx,dword ptr [edx+4]
        call CEntity::GetMatrix
        fld dword ptr [esp+38h]
        fsub dword ptr [esp+78h]
        fld dword ptr [esp+34h]
        fsub dword ptr [esp+74h]
        fld dword ptr [esp+30h]
        fsub dword ptr [esp+70h]
        fld st(0)
        fmul st,st(1)
        fld st(2)
        fmul st,st(3)
        faddp st(1),st
        fld st(3)
        fmul st,st(4)
        faddp st(1),st
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        add esp,80h
        ret 4
    }
    // clang-format on
}
// 0x00015dd0
__declspec(naked) float CRemotePlayer::GetDistanceToLocalPlayer()
{
    // clang-format off
    __asm {
        mov eax,ecx
        mov ecx,dword ptr [eax+4]
        test ecx,ecx
        jnz select_entity
        fld g_remoteAbsentDistance
        ret
    select_entity:
        cmp byte ptr [eax+1ah],13h
        jne selected
        mov eax,dword ptr [eax+8]
        test eax,eax
        jz selected
        mov ecx,eax
    selected:
        jmp CEntity::GetDistanceToLocalPlayer
    }
    // clang-format on
}
// 0x00015e50
void CRemotePlayer::EnterVehicle(WORD id, BOOL passenger)
{
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    CVehiclePool* vehicles = *(CVehiclePool**)(pools + 0xc);
    CVehicle* vehicle = vehicles->Get(id);
    CPed* ped = m_state.ped;
    if (!ped || !vehicle || ped->IsInVehicle())
        return;
    DWORD ref = vehicles->GetRef((int)id);
    if (!ref || ref == 0xffff)
        return;
    m_state.ped->SetKeys(0, 0, 0);
    m_state.ped->EnterVehicle(ref, passenger);
}
// 0x00015ee0
void CRemotePlayer::ExitVehicle()
{
    CPed* ped = m_state.ped;
    if (!ped || !ped->IsInVehicle())
        return;
    m_state.ped->SetKeys(0, 0, 0);
    m_state.ped->ExitVehicle();
}
// 0x00015f10
void CRemotePlayer::ChangeState(BYTE newState, BYTE oldState)
{
    if (newState != 19 || oldState != 17)
        return;
    CPed* ped = g_pGame->GetPlayerPed();
    if (!ped || !ped->IsInVehicle() || ped->IsPassenger())
        return;
    CVehiclePool* vehicles = *(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
    WORD id = vehicles->Find(ped->GetVehicle());
    if (id != *(WORD*)((BYTE*)this + 2))
        return;
    CMatrix matrix;
    ped->GetMatrix(&matrix);
    float z = matrix.position.z + g_quaternionMatrixOne;
    ped->WarpFromVehicle(matrix.position.x, matrix.position.y, z);
    g_pGame->DisplayGameText(g_remoteCarJackedText, 1000, 5);
}
// 0x000144d0
void CRemotePlayer::ApplySurfingMatrix()
{
    if (m_state.playerState != 17)
        return;
    WORD id = m_state.onfoot.surfingVehicle;
    if (!id || id == 0xffff || id >= 4100)
        return;
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    CVehiclePool* vehicles = *(CVehiclePool**)(pools + 0xc);
    CObjectPool* objects = *(CObjectPool**)(pools + 0x14);
    CEntity* entity;
    if (id < 2000)
        entity = vehicles->Get(id);
    else
        entity = objects->Get((WORD)(id - 2000));
    if (!entity)
        return;
    for (int axis = 0; axis < 3; axis++)
    {
        float* component = (float*)&m_state.onfoot.surfingOffset + axis;
        BYTE accepted;
        // clang-format off
        // Preserve FCOMP unordered acceptance and native +/-100 bounds.
        __asm {
            mov edx,component
            fld dword ptr [edx]
            fcomp g_vehicleTrainSpeedMaximum
            fnstsw ax
            test ah,41h
            jz invalid_offset
            fld dword ptr [edx]
            fcomp g_vehicleTrainSpeedMinimum
            fnstsw ax
            test ah,5
            jnp invalid_offset
            mov accepted,1
            jmp checked_offset
        invalid_offset:
            mov accepted,0
        checked_offset:
        }
        // clang-format on
        if (!accepted) return;
    }
    CMatrix entityMatrix, pedMatrix;
    CVector speed, turnSpeed, position;
    entity->GetMatrix(&entityMatrix);
    entity->GetSpeed(&speed);
    entity->GetTurnSpeed(&turnSpeed);
    m_state.ped->GetMatrix(&pedMatrix);
    GameTransformPoint(&position, &entityMatrix, &m_state.onfoot.surfingOffset);
    memcpy(&pedMatrix.position, &position, 12);
    m_state.ped->ApplyMatrix(pedMatrix);
}

// 0x00001080
BOOL CRemotePlayer::IsActive()
{
    return m_state.ped && m_state.playerState != 0;
}

#include "game/util.h"
// 0x00015e30
DWORD CRemotePlayer::GetColorAsARGB()
{
    return (PlayerColors::Get(m_state.id) >> 8) | 0xff000000;
}
// 0x00015fd0
int CRemotePlayer::GetStatus()
{
    if (!m_state.ped)
        return 0;
    if (!m_state.ped->DoesExist())
        return 0;
    if (m_state.ped->GetVehicleTransitionTaskState())
        return 0;
    if (m_state.ped->HasTaskType704())
        return 0;
    return (int)m_state.status;
}

// 0x00002dc0
BYTE CRemotePlayer::GetState()
{
    return m_state.playerState;
}
// 0x000135d0
void CRemotePlayer::SetId(WORD id)
{
    m_state.id = id;
}
// 0x000135e0
WORD CRemotePlayer::GetId()
{
    return m_state.id;
}
// 0x000135f0
void CRemotePlayer::SetRawFlag1C(DWORD value)
{
    m_state.rawFlag1c = value;
}
// 0x00013600
void CRemotePlayer::ResetStateAndLabels()
{
    m_state.playerState = 0;
    m_state.showNameTag = 1;
}
// 0x00014220
BYTE CRemotePlayer::GetSpecialAction()
{
    return m_state.specialAction;
}
// 0x00014230
DWORD CRemotePlayer::GetRawFlag1C()
{
    return m_state.rawFlag1c;
}

// 0x00001070
CPed* CRemotePlayer::GetPed()
{
    return m_state.ped;
}
// 0x000010a0
CVehicle* CRemotePlayer::GetVehicle()
{
    return m_state.vehicle;
}
