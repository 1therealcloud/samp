#include "net/remoteplayer.h"

#include "game/aimstuff.h"
#include "game/custommodels.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "gui/chatwindow.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/objectpool.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"

#include <math.h>
#include <string.h>

const float g_remoteVehicleAxisTeleportDistance = 6.0f;

const double g_remoteUnoccupiedCorrectionThreshold = 0.05;

const float g_remoteTrailerCorrection = 0.025f;

const float g_remoteBulletOffsetMinimum = -300.0f;

const float g_remoteBulletOffsetMaximum = 300.0f;

const char g_remoteVendingAnimation[] = "VEND_EAT_P";

const char g_remoteVendingLibrary[] = "VENDING";

const char g_remoteCarryAnimation[] = "crry_prtial";

const char g_remoteCarryLibrary[] = "CARRY";

const float g_remoteHeadDistance = 20.0f;

const char g_remoteHeadTask[] = "FollowPedSA";

float g_remoteOnfootCorrection = 0.1f;

float g_remoteIncarCorrection = 0.06f;

const float g_remoteIncarTeleportDistance = 8.0f;

DWORD g_remoteClampTurnSpeed = 1;

const float g_remoteTurnSpeedMinimum = -0.02f;

const float g_remoteAimZoomScale = 0.01587301678955555f;

const float g_remoteAimAspectScale = 0.003921568859368563f;

const float g_remoteAbsentDistance = 10000.0f;

const char g_remoteCarJackedText[] = "~r~Car Jacked~w~!";

void CRemotePlayer::ShowNameTag(BYTE show)
{
    m_state.showNameTag = show != 0;
}

float CRemotePlayer::GetReportedHealth()
{
    return m_state.reportedHealth;
}

float CRemotePlayer::GetReportedArmour()
{
    return m_state.reportedArmour;
}

void CRemotePlayer::SetTeam(BYTE team)
{
    m_state.team = team;
}

BYTE CRemotePlayer::GetTeam()
{
    return m_state.team;
}

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
        if (!(FloatOffset((&target->x)[axis], (&matrix.position.x)[axis]) <= g_objectMutationStep))
        {
            significant = true;
            break;
        }
    }
    if (!significant)
        return;
    for (int axis = 0; axis < 3; ++axis)
    {
        const float* limit = axis < 2 ? &g_remoteVehicleAxisTeleportDistance : &g_frameScaleMultiplier;
        if (FloatOffset((&target->x)[axis], (&matrix.position.x)[axis]) > *limit)
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
        if (FloatOffset(*wanted, *current) > g_remoteUnoccupiedCorrectionThreshold)
            *value = (float)(((double)*wanted - *current) * g_remoteIncarCorrection + *value);
    }
    vehicle->SetSpeed(speed);
    vehicle->m_dwRemoteUnoccupiedSync = 1;
}

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
        if (!(FloatOffset((&target->x)[axis], (&matrix.position.x)[axis]) <= g_textDrawHalf))
        {
            significant = true;
            break;
        }
    }
    if (!significant)
        return;
    for (int axis = 0; axis < 3; ++axis)
    {
        const float* limit = axis < 2 ? &g_remoteVehicleAxisTeleportDistance : &g_frameScaleMultiplier;
        if (FloatOffset((&target->x)[axis], (&matrix.position.x)[axis]) > *limit)
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
        if (FloatOffset(*wanted, *current) > g_pedDrunkSpeedThreshold)
            *value = (float)(((double)*wanted - *current) * g_remoteTrailerCorrection + *value);
    }
    trailer->SetSpeed(speed);
}

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
            const bool accepted = !(*component > g_remoteBulletOffsetMaximum || *component < g_remoteBulletOffsetMinimum);
            if (!accepted)
                return;
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

void CRemotePlayer::SetState(BYTE state)
{
    BYTE previous = m_state.playerState;
    if (state != previous)
    {
        ChangeState(state, previous);
        m_state.playerState = state;
    }
}

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
        if (!m_state.scriptedAnimation && !animation && m_state.ped->HasNamedAnimationTask())
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
            m_state.vehicle->SetBikeDesiredLeanAngle(*(float*)&m_state.incar.modelData);
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
            if ((m_state.passenger.seatFlags & 0x40) && !m_state.ped->HasGangDrivebyTask() && m_state.ped->DoesExist() && m_state.ped->EnablePassengerDrivebyMode())
                m_state.passengerDriveby = 1;
            if (!(m_state.passenger.seatFlags & 0x40) && m_state.ped->HasGangDrivebyTask() && m_state.ped->DoesExist())
            {
                m_state.ped->StopGangDrivebyTask();
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
        if (m_state.isNpc && speed->x == 0 && speed->y == 0 && speed->z == 0 && m_state.vehicle)
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

void CRemotePlayer::ProcessHead()
{
    CPed* ped = m_state.ped;
    if (!ped || !ped->DoesExist() || m_state.isNpc)
        return;
    if (g_pGame && !*(DWORD*)((BYTE*)g_pGame + 0x59))
        return;
    if (m_state.ped->GetHeldObjectType())
        return;
    CMatrix matrix;
    m_state.ped->GetMatrix(&matrix);
    DWORD now = GetTickCount();
    if (now - m_state.lastHeadUpdateTick > 1000)
    {
        CameraAim* aim = AimStuff::GetPlayerAim(*((BYTE*)m_state.ped + 0x2b0));
        const double x = (double)aim->front.x * g_remoteHeadDistance + matrix.position.x;
        const double y = (double)aim->front.y * g_remoteHeadDistance + matrix.position.y;
        m_state.lastHeadUpdateTick = now;
        const volatile float z = (float)((double)aim->front.z * g_remoteHeadDistance + matrix.position.z);
        m_state.headDirection.x = (float)(x - matrix.position.x);
        m_state.headDirection.y = (float)(y - matrix.position.y);
        m_state.headDirection.z = z - matrix.position.z;
    }
    if (now - m_state.lastHeadTaskTick > 200)
    {
        ped = m_state.ped;
        CVector target;
        target.x = matrix.position.x + m_state.headDirection.x;
        target.y = matrix.position.y + m_state.headDirection.y;
        target.z = matrix.position.z + m_state.headDirection.z;
        ped->ApplyCommandTask(g_remoteHeadTask, 0, 1000, -1, &target, 0, 0.1f, 500, 3, 0);
        m_state.lastHeadTaskTick = now;
    }
}

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

BOOL CRemotePlayer::HasSurfingTarget()
{
    if (m_state.playerState != 17)
        return FALSE;
    WORD id = m_state.onfoot.surfingVehicle;
    return id && id != 0xffff && id < 4100 ? TRUE : FALSE;
}

BOOL CRemotePlayer::SurfingOnObject()
{
    if (m_state.playerState != 17)
        return FALSE;
    WORD id = m_state.onfoot.surfingVehicle;
    return id && id != 0xffff && id >= 2000 && id < 4100 ? TRUE : FALSE;
}

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
    if (m_state.specialAction == 20 && m_state.ped->GetHeldObjectType() != 1)
        m_state.ped->GiveStuff(1);
    if (m_state.specialAction == 22 && m_state.ped->GetHeldObjectType() != 2)
        m_state.ped->GiveStuff(2);
    if (m_state.specialAction == 23 && m_state.ped->GetHeldObjectType() != 3)
        m_state.ped->GiveStuff(3);
    if (m_state.specialAction == 21 && m_state.ped->GetHeldObjectType() != 4)
        m_state.ped->GiveStuff(4);
    if (m_state.ped->GetHeldObjectType())
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
    for (int axis = 0; axis < 3; ++axis)
        if (delta[axis] > g_localQuaternionSyncThreshold)
        {
            const double difference = (double)(&target->x)[axis] - (&matrix.position.x)[axis];
            float& component = (&speed.x)[axis];
            component = (float)(difference * g_remoteOnfootCorrection + component);
        }
    m_state.ped->SetSpeed(speed);
}

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
    const double y = FloatOffset(target->y, matrix.position.y);
    delta[1] = (float)y;
    if (delta[0] <= g_objectTurnVelocityScale && y <= g_objectTurnVelocityScale)
        return;
    if (delta[0] > g_quaternionMatrixTwo || y > g_quaternionMatrixTwo)
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
            *value = (float)(((double)*wanted - *current) * g_objectMutationStep + *value);
        }
    m_state.ped->SetSpeed(speed);
}

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
    const double z = FloatOffset(target->z, matrix.position.z);
    delta[2] = (float)z;
    if (delta[0] <= g_pedDrunkSpeedThreshold && delta[1] <= g_pedDrunkSpeedThreshold && z <= g_pedDrunkSpeedThreshold)
        return;
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
            *value = (float)(((double)*wanted - *current) * g_remoteIncarCorrection + *value);
        }
    if (FloatOffset(speed.x, 0) > g_objectTurnVelocityScale || FloatOffset(speed.y, 0) > g_objectTurnVelocityScale || FloatOffset(speed.z, 0) > g_objectTurnVelocityScale)
        m_state.vehicle->SetSpeed(speed);
}

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
    const double degrees = atan2(-(double)matrix.up.x, (double)matrix.up.y) * g_entityRadiansToDegrees;
    const volatile float roundedDegrees = (float)degrees;
    float angle = roundedDegrees;
    if (degrees < g_objectScaleLowerBound)
        angle = (float)((double)roundedDegrees + g_anglePeriod);
    else if (roundedDegrees >= g_anglePeriod)
        angle = (float)((double)roundedDegrees - g_anglePeriod);
    m_state.ped->SetRotation(angle);
}

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

void CRemotePlayer::SetOnfootTarget(const CVector* position, const CVector* speed)
{
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.onfootTargetPosition + i) = *(const DWORD*)((const BYTE*)position + i);
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.onfootTargetSpeed + i) = *(const DWORD*)((const BYTE*)speed + i);
    m_state.ped->SetSpeed(m_state.onfootTargetSpeed);
}

void CRemotePlayer::SetIncarTarget(const CMatrix* matrix, const CVector* position, const CVector* speed)
{
    GameQuaternionFromMatrix(matrix, &m_state.incarTargetRotation);
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.incarTargetPosition + i) = *(const DWORD*)((const BYTE*)position + i);
    for (int i = 0; i < 12; i += 4)
        *(DWORD*)((BYTE*)&m_state.incarTargetSpeed + i) = *(const DWORD*)((const BYTE*)speed + i);
    m_state.vehicle->SetSpeed(*speed);
}

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
    ped = g_pGame->CreatePed(model, *position, rotation, 1, (BOOL)m_state.isNpc);
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
    if (ped && ped->IsInVehicle() && m_state.specialAction != 3 && m_state.specialAction != 4 && !m_state.ped->HasLeaveCarTask())
        OnExitVehicle();
    if (m_state.playerState != 17)
        m_state.playerState = 17;
}

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
    const float aspectValue = (float)((double)aspect * g_remoteAimAspectScale);
    const float zoomValue = (float)((double)zoom * g_remoteAimZoomScale);
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

void CRemotePlayer::Chat(char* text)
{
    WORD id = m_state.id;
    const char* name = ((CNetGame*)g_pNetGame)->pools->players->GetName(id);
    DWORD color = (PlayerColors::Get(id) >> 8) | 0xff000000;
    g_pChat->AddChatMessage(name, color, text);
}

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
    m_state.isNpc = 0;
    m_state.scriptedAnimation = 0;
    m_state.lastHeadUpdateTick = tick();
    m_state.lastHeadTaskTick = tick();
    m_state.lastUpdateTick = tick();
    ResetData();
}

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

void CRemotePlayer::SetColor(DWORD color)
{
    PlayerColors::Set(m_state.id, color);
}

DWORD CRemotePlayer::GetColorAsRGBA()
{
    return PlayerColors::Get(m_state.id);
}

float CRemotePlayer::GetDistanceToPlayer(CRemotePlayer* other)
{
    if (!m_state.ped)
        return g_remoteAbsentDistance;
    CMatrix first, second;
    m_state.ped->GetMatrix(&first);
    other->m_state.ped->GetMatrix(&second);
    const double z = (double)first.position.z - second.position.z;
    const double y = (double)first.position.y - second.position.y;
    const double x = (double)first.position.x - second.position.x;
    return (float)sqrt(x * x + y * y + z * z);
}

float CRemotePlayer::GetDistanceToLocalPlayer()
{
    if (!m_state.ped)
        return g_remoteAbsentDistance;
    CEntity* entity = m_state.ped;
    if (m_state.playerState == 0x13 && m_state.vehicle)
        entity = m_state.vehicle;
    return entity->GetDistanceToLocalPlayer();
}

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

void CRemotePlayer::ExitVehicle()
{
    CPed* ped = m_state.ped;
    if (!ped || !ped->IsInVehicle())
        return;
    m_state.ped->SetKeys(0, 0, 0);
    m_state.ped->ExitVehicle();
}

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
    float z = matrix.position.z + 1.0f;
    ped->WarpFromVehicle(matrix.position.x, matrix.position.y, z);
    g_pGame->DisplayGameText(g_remoteCarJackedText, 1000, 5);
}

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
        const bool accepted = !(*component > g_vehicleTrainSpeedMaximum || *component < g_vehicleTrainSpeedMinimum);
        if (!accepted)
            return;
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

BOOL CRemotePlayer::IsActive()
{
    return m_state.ped && m_state.playerState != 0;
}

DWORD CRemotePlayer::GetColorAsARGB()
{
    return (PlayerColors::Get(m_state.id) >> 8) | 0xff000000;
}

int CRemotePlayer::GetStatus()
{
    if (!m_state.ped || !m_state.ped->DoesExist() || m_state.ped->GetVehicleTransitionTaskState() || m_state.ped->HasLeaveCarTask())
        return 0;
    return (int)m_state.status;
}

BYTE CRemotePlayer::GetState()
{
    return m_state.playerState;
}

void CRemotePlayer::SetId(WORD id)
{
    m_state.id = id;
}

WORD CRemotePlayer::GetId()
{
    return m_state.id;
}

void CRemotePlayer::SetNpc(DWORD value)
{
    m_state.isNpc = value;
}

void CRemotePlayer::ResetStateAndLabels()
{
    m_state.playerState = 0;
    m_state.showNameTag = 1;
}

BYTE CRemotePlayer::GetSpecialAction()
{
    return m_state.specialAction;
}

DWORD CRemotePlayer::IsNpc()
{
    return m_state.isNpc;
}

CPed* CRemotePlayer::GetPed()
{
    return m_state.ped;
}

CVehicle* CRemotePlayer::GetVehicle()
{
    return m_state.vehicle;
}
