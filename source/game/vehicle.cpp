#include "game/vehicle.h"

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/entity.h"
#include "game/game.h"
#include "game/hooks.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/scripting.h"
#include "game/util.h"
#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "main.h"
#include "net/vehiclepool.h"

#include <string.h>
// R5 address to verify
void* g_vehicleTrainCursor;
// R5 address to verify
DWORD g_vehicleLastCreatedHandle;
// 0x000ec314
const float g_vehicleCreationLift = 0.25f;
// 0x000ed6dc
const char g_vehicleBadCarriageWarning[29] = "Warning: bad train carriages";
// 0x000ed6fc
const char g_vehicleCreationWarning[42] = "Warning: couldn't create vehicle type: %u";
// R5 address to verify
DWORD g_vehicleDetachingTrailer;

const float g_vehicleTrainSpeedMinimum = -100.0f;
// 0x000e6114
const float g_vehicleTrainSpeedMaximum = 100.0f;
// 0x000e60a0
const float g_vehicleMarkerDistance = 200.0f;
// R5 address to verify
const Scripting::OpcodeInfo g_vehicleLockedCommand = {0x519, "ii"};
// R5 address to verify
const Scripting::OpcodeInfo g_vehicleWreckedCommand = {0x119, "i"};

// R5 address to verify
void CVehicle::SetObjective(BYTE objective)
{
    m_byteObjective = objective;
}

// 0x000b72e0
void CVehicle::SetColor(int primary, int secondary)
{
    BYTE color = (BYTE)secondary;
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle))
    {
        ((BYTE*)m_pGameVehicle)[0x434] = (BYTE)primary;
        ((BYTE*)m_pGameVehicle)[0x435] = color;
    }
    m_nSecondaryColor = color;
    m_nPrimaryColor = (BYTE)primary;
    m_bColorChanged = TRUE;
}

// 0x000b88f0
CVehicle::~CVehicle()
{
    m_pGameVehicle = GamePoolVehicleGetAt(m_nHandle);
    if (m_pGameVehicle)
    {
        if (m_dwMarker)
        {
            Scripting::ProcessCommand(&g_vehicleDestroyMarkerCommand, m_dwMarker);
            m_dwMarker = 0;
        }
        RemovePassengers();
        if (m_pTrailer)
        {
            DetachTrailer();
            m_pTrailer = 0;
        }
        if (m_pLicensePlateTexture)
        {
            m_pLicensePlateTexture->Release();
            m_pLicensePlateTexture = 0;
        }
        short model = *(short*)((BYTE*)m_pGameVehicle + 0x22);
        if (model != 0x21a && model != 0x219)
        {
            int modelSnapshot = model;
            Scripting::ProcessCommand(&g_vehicleDestroyCommand, m_nHandle);
            if (GameGetModelReferenceCount(modelSnapshot) == 0 && !m_dwKeepLoaded && g_pGame->CountBackendVehicleModelRecords() > 80 && g_pGame->IsModelAvailable(modelSnapshot))
            {
                // clang-format off
                __asm {
                    push modelSnapshot
                    mov edx,0x4089a0
                    call edx
                    pop edx
                }
                // clang-format on
            }
        }
        else
            Scripting::ProcessCommand(&g_vehicleDestroyTrainCommand, m_nHandle);
    }
}

// Preserve base qualification and virtual dispatch to an existing trailer.
// 0x000b8a10
void CVehicle::Add()
{
    if (!CEntity::DoesExist())
    {
        CEntity::Add();
        CVehicle* trailer = GetTrailer();
        if (trailer)
            trailer->Add();
    }
}

// Trailer is removed before the qualified base vehicle removal.
// 0x000b8a40
void CVehicle::Remove()
{
    if (CEntity::DoesExist())
    {
        CVehicle* trailer = GetTrailer();
        if (trailer)
            trailer->Remove();
        CEntity::Remove();
    }
}

// Preserve base construction, untouched early-return bytes and native global reloads.
// 0x000b83d0
CVehicle::CVehicle(int model, float x, float y, float z, float rotation, DWORD keepLoaded, BOOL siren)
{
    DWORD handle = 0;
    m_pGameVehicle = 0;
    m_nHandle = 0;
    m_pTrailer = 0;
    if (model != 0x21a && model != 0x219 && model != 0x23a && model != 0x239 && model != 0x1c1)
    {
        if (!g_pGame->IsModelAvailable(model))
        {
            g_pGame->RequestModel(model, 2);
            g_pGame->LoadRequestedModels();
            if (!g_pGame->IsModelAvailable(model))
            {
                void(WINAPI * sleep)(DWORD) = Sleep;
                do
                {
                    sleep(0);
                }
                while (!g_pGame->IsModelAvailable(model));
            }
        }
        if (siren)
        {
            g_hookSirenOnce = 1;
            m_dwSiren = 1;
        }
        else
            m_dwSiren = 0;
        Scripting::ProcessCommand(&g_vehicleCreateCommand, model, (double)x, (double)y, (double)z, &handle);
        Scripting::ProcessCommand(&g_vehicleRotationCommand, handle, (double)rotation);
        Scripting::ProcessCommand(&g_vehicleGasTankExplosionCommand, handle, 0);
        Scripting::ProcessCommand(&g_vehicleHydraulicsCommand, handle, 0);
        Scripting::ProcessCommand(&g_vehicleDamageableCommand, handle, 0);
        m_pGameVehicle = GamePoolVehicleGetAt(handle);
        m_pGameEntity = m_pGameVehicle;
        m_nHandle = handle;
        if (!m_pGameVehicle)
        {
            g_pChat->AddDebugMessage(g_vehicleCreationWarning, (unsigned int)model);
            return;
        }
        g_vehicleLastCreatedHandle = handle;
        *(DWORD*)((BYTE*)m_pGameVehicle + 0x4f8) = 0;
        *(DWORD*)((BYTE*)m_pGameVehicle + 0x4b0) = 0;
        m_dwUnknown5c = 0;
        CMatrix matrix;
        GetMatrix(&matrix);
        // Keep both paths on x87 through the final float store.
        matrix.position.z = (float)(GetSubtype() != 2 && GetSubtype() != 6 ? (double)(z + g_vehicleCreationLift) : (double)z);
        matrix.position.x = x;
        matrix.position.y = y;
        SetMatrix(matrix);
        m_dwKeepLoaded = keepLoaded;
    }
    else if (model == 0x23a || model == 0x239)
    {
        if (g_vehicleTrainCursor)
        {
            m_pGameVehicle = *(void**)((BYTE*)g_vehicleTrainCursor + 0x5d4);
            if (!m_pGameVehicle)
            {
                g_pChat->AddDebugMessage(g_vehicleBadCarriageWarning);
            }
            else
            {
                handle = GamePoolVehicleGetIndex(m_pGameVehicle);
                m_nHandle = handle;
                m_pGameEntity = m_pGameVehicle;
                g_vehicleLastCreatedHandle = handle;
                g_vehicleTrainCursor = m_pGameVehicle;
                goto initialized;
            }
        }
        m_pGameEntity = 0;
        m_pGameVehicle = 0;
        g_vehicleTrainCursor = 0;
        return;
    }
    else
    {
        int trainType = model == 0x21a ? 5 : model == 0x219 ? 3 : 9;
        DWORD direction = 0;
        if (rotation > g_angleUpperBound)
            direction = 1;
        g_pGame->RequestModel(0x21a, 2);
        g_pGame->RequestModel(0x23a, 2);
        g_pGame->RequestModel(0x219, 2);
        g_pGame->RequestModel(0x239, 2);
        g_pGame->RequestModel(0x1c1, 2);
        g_pGame->LoadRequestedModels();
        BOOL available = g_pGame->IsModelAvailable(0x21a);
        // The five train waits share one resolved Sleep target.
        void(WINAPI * sleep)(DWORD) = Sleep;
        while (!available)
        {
            sleep(0);
            available = g_pGame->IsModelAvailable(0x21a);
        }
        while (!g_pGame->IsModelAvailable(0x23a))
            sleep(0);
        while (!g_pGame->IsModelAvailable(0x219))
            sleep(0);
        while (!g_pGame->IsModelAvailable(0x239))
            sleep(0);
        while (!g_pGame->IsModelAvailable(0x1c1))
            sleep(0);
        Scripting::ProcessCommand(&g_vehicleCreateTrainCommand, trainType, (double)x, (double)y, (double)z, direction, &handle);
        m_pGameVehicle = GamePoolVehicleGetAt(handle);
        m_pGameEntity = m_pGameVehicle;
        m_nHandle = handle;
        g_vehicleLastCreatedHandle = handle;
        g_vehicleTrainCursor = m_pGameVehicle;
        GameRemoveDriverByState598(m_pGameVehicle);
    }
initialized:
    m_dwInvulnerable = 0;
    m_byteObjective = 0;
    m_dwObjectiveMarker = 0;
    m_dwMarker = 0;
    m_dwHasBeenDriven = 0;
    m_dwLastDrivenTime = GetTickCount();
    m_dwField50 = 0xffffffff;
    m_dwField54 = 0xffffffff;
    m_dwLocked = 0;
    m_bColorChanged = FALSE;
    m_bInModShop = FALSE;
    m_dwEngineOverride75 = 0;
    m_pLicensePlateTexture = 0;
    memset(m_szLicensePlateText, 0, 33);
}

// 0x000b7e00
BOOL CVehicle::IsRC()
{
    if (!m_pGameVehicle)
        return FALSE;
    if (!GamePoolVehicleGetAt(m_nHandle))
        return FALSE;
    short model = *(short*)((BYTE*)m_pGameVehicle + 0x22);
    return model == 0x1b9 || model == 0x1d0 || model == 0x1d1 || model == 0x252 || model == 0x1f5 || model == 0x234;
}

// 0x000b8330
BOOL CVehicle::HasSiren()
{
    return m_dwSiren != 0;
}

// 0x000b8340
BYTE CVehicle::GetMaxPassengers()
{
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle))
        return *((BYTE*)m_pGameVehicle + 0x488);
    return 0;
}

// 0x000b72a0
float CVehicle::GetHealth()
{
    return m_pGameVehicle ? *(float*)((BYTE*)m_pGameVehicle + 0x4c0) : g_objectScaleLowerBound;
}

// 0x000b7390
int CVehicle::GetSubtype()
{
    return m_pGameVehicle ? GameGetVehicleSubtype(m_pGameVehicle) : 0;
}

// 0x000b70c0
void CVehicle::ResetPointers()
{
    if (m_pGameVehicle)
    {
        m_pGameVehicle = GamePoolVehicleGetAt(m_nHandle);
        m_pGameEntity = m_pGameVehicle;
    }
}

// 0x000b70e0
BOOL CVehicle::HasDriver()
{
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && m_pGameVehicle)
    {
        BYTE* driver = *(BYTE**)((BYTE*)m_pGameVehicle + 0x460);
        if (driver && (*(DWORD*)(driver + 0x46c) & 0x100) && !*(DWORD*)(driver + 0x598))
            return TRUE;
    }
    return FALSE;
}

// 0x000b7130
BOOL CVehicle::IsOccupied()
{
    if (m_pGameVehicle)
    {
        void** occupants = (void**)((BYTE*)m_pGameVehicle + 0x460);
        if (occupants[0] || occupants[1] || occupants[2] || occupants[3] || occupants[4] || occupants[5] || occupants[6] || occupants[7])
            return TRUE;
    }
    return FALSE;
}

// 0x000b72c0
void CVehicle::SetHealth(float health)
{
    if (m_pGameVehicle)
        *(DWORD*)((BYTE*)m_pGameVehicle + 0x4c0) = *(DWORD*)&health;
}

// 0x000b73f0
BOOL CVehicle::DriverIsPlayerPed()
{
    if (m_pGameVehicle)
    {
        void* ped = GameFindLocalPed();
        if (*(void**)((BYTE*)m_pGameVehicle + 0x460) == ped)
            return TRUE;
    }
    return FALSE;
}

// 0x000b7420
BOOL CVehicle::HasPlayerPed()
{
    if (m_pGameVehicle)
    {
        BYTE* ped = (BYTE*)GameFindLocalPed();
        if (ped && (*(DWORD*)(ped + 0x46c) & 0x100) && m_pGameVehicle == *(void**)(ped + 0x58c))
            return TRUE;
    }
    return FALSE;
}

// 0x000b7540
void CVehicle::EnableSiren(BYTE enabled)
{
    BYTE* vehicle = (BYTE*)m_pGameVehicle;
    vehicle[0x42d] = (BYTE)((vehicle[0x42d] & 0x7f) | (enabled << 7));
}

// 0x000b7560
BOOL CVehicle::SirenEnabled()
{
    return (BYTE)(*(BYTE*)((BYTE*)m_pGameVehicle + 0x42d) & 0x80) == 0x80;
}

// 0x000b7580
void CVehicle::SetBackendWord45c(WORD value)
{
    *(WORD*)((BYTE*)m_pGameVehicle + 0x45c) = value;
}

// 0x000b76a0
void CVehicle::SetHydraThrusters(DWORD value)
{
    if (m_pGameVehicle)
        *(DWORD*)((BYTE*)m_pGameVehicle + 0x86c) = value;
}

// 0x000b76c0
DWORD CVehicle::GetHydraThrusters()
{
    return m_pGameVehicle ? *(DWORD*)((BYTE*)m_pGameVehicle + 0x86c) : 0;
}

// 0x000b78e0
float CVehicle::GetTrainSpeed()
{
    if (!m_pGameVehicle)
        return g_objectScaleLowerBound;
    DWORD speed = *(DWORD*)((BYTE*)m_pGameVehicle + 0x5a4);
    return *(float*)&speed;
}

// 0x000b7b80
DWORD CVehicle::GetPanelsDamage()
{
    if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 1)
        return *(DWORD*)((BYTE*)m_pGameVehicle + 0x5b4);
    return 0;
}

// 0x000b7bb0
DWORD CVehicle::GetDoorsDamage()
{
    if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 1)
        return *(DWORD*)((BYTE*)m_pGameVehicle + 0x5a9);
    return 0;
}

// 0x000b7be0
BYTE CVehicle::GetLightsDamage()
{
    if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 1)
        return *(BYTE*)((BYTE*)m_pGameVehicle + 0x5b0);
    return 0;
}

// 0x000b7c80
void CVehicle::SetTrailer(CVehicle* trailer)
{
    m_pTrailer = trailer;
}

// 0x000b7dd0
BOOL CVehicle::IsTowtruck()
{
    if (!m_pGameVehicle)
        return FALSE;
    if (GamePoolVehicleGetAt(m_nHandle))
        return *(short*)((BYTE*)m_pGameVehicle + 0x22) == 0x20d;
    return FALSE;
}

// 0x000b8140
bool CVehicle::DoesExist()
{
    return GamePoolVehicleGetAt(m_nHandle) != 0;
}

// R5 address to verify
void CVehicle::SetField50(DWORD value)
{
    m_dwField50 = value;
}

// R5 address to verify
DWORD CVehicle::GetField50()
{
    return m_dwField50;
}

// R5 address to verify
void CVehicle::SetField54(DWORD value)
{
    m_dwField54 = value;
}

// R5 address to verify
DWORD CVehicle::GetField54()
{
    return m_dwField54;
}

// 0x000b7090
void CVehicle::ChangeInterior(int interior)
{
    if (GamePoolVehicleGetAt(m_nHandle))
        Scripting::ProcessCommand(&g_vehicleChangeInteriorCommand, m_nHandle, interior);
}

// 0x000b7330
void CVehicle::UpdateColor()
{
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && m_bColorChanged && !m_bInModShop)
    {
        if (m_nPrimaryColor != *((BYTE*)m_pGameVehicle + 0x434) || m_nSecondaryColor != *((BYTE*)m_pGameVehicle + 0x435))
        {
            *((BYTE*)m_pGameVehicle + 0x434) = m_nPrimaryColor;
            *((BYTE*)m_pGameVehicle + 0x435) = m_nSecondaryColor;
        }
    }
}

// 0x000b73b0
DWORD CVehicle::IsSunk()
{
    if (!m_pGameVehicle)
        return 0;
    return Scripting::ProcessCommand(&g_vehicleIsSunkCommand, m_nHandle);
}

// 0x000b74a0
BOOL CVehicle::HasTurret()
{
    int model = ((CEntity*)this)->GetModelIndex();
    return model == 0x1b0 || model == 0x234 || model == 0x197 || model == 0x259;
}

// 0x000b74d0
void CVehicle::SetBackendBytes1f6And514(BYTE value)
{
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle))
    {
        if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 4)
            return;
        if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 5)
            return;
        if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 3)
            return;
        *((BYTE*)m_pGameVehicle + 0x1f6) = value;
        *((BYTE*)m_pGameVehicle + 0x514) = value;
    }
}

// 0x000b7840
void CVehicle::Lock(BOOL locked)
{
    if (locked)
    {
        *(DWORD*)((BYTE*)m_pGameVehicle + 0x4f8) = 2;
        m_dwLocked = 1;
    }
    else
    {
        *(DWORD*)((BYTE*)m_pGameVehicle + 0x4f8) = 0;
        m_dwLocked = 0;
    }
}

// 0x000b7870
BOOL CVehicle::UpdateLastDrivenTime()
{
    if (m_pGameVehicle && *(void**)((BYTE*)m_pGameVehicle + 0x460))
    {
        m_dwHasBeenDriven = 1;
        m_dwLastDrivenTime = GetTickCount();
        return TRUE;
    }
    return FALSE;
}

// 0x000b7c10
void CVehicle::AttachTrailer()
{
    CVehicle* trailer = m_pTrailer;
    if (trailer)
        Scripting::ProcessCommand(&g_vehicleAttachTrailerCommand, trailer->m_nHandle, m_nHandle);
}

// 0x000b7c30
void CVehicle::DetachTrailer()
{
    g_vehicleDetachingTrailer = 1;
    if (m_pTrailer && m_pTrailer->m_nHandle && GamePoolVehicleGetAt(m_pTrailer->m_nHandle) && m_pTrailer->m_pGameVehicle)
    {
        Scripting::ProcessCommand(&g_vehicleDetachTrailerCommand, m_pTrailer->m_nHandle, m_nHandle);
    }
    g_vehicleDetachingTrailer = 0;
}

// 0x000b7e50
void CVehicle::EnableLights(BYTE enabled)
{
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle))
    {
        if (!enabled)
        {
            *((BYTE*)m_pGameVehicle + 0x4a8) |= 8;
            *((BYTE*)m_pGameVehicle + 0x428) &= 0xbf;
        }
        else
        {
            *((BYTE*)m_pGameVehicle + 0x4a8) &= 0xf7;
            *((BYTE*)m_pGameVehicle + 0x428) |= 0x40;
        }
    }
}

// 0x000b7eb0
BYTE CVehicle::GetBackendLightsDisabledFlag()
{
    if (!m_pGameVehicle)
        return 1;
    if (GamePoolVehicleGetAt(m_nHandle))
    {
        BYTE flags = *((BYTE*)m_pGameVehicle + 0x4a8);
        flags >>= 3;
        flags &= 1;
        return flags;
    }
    return 1;
}

// 0x000b80b0
BOOL CVehicle::RemoveComponent(WORD component)
{
    if (!m_pGameVehicle)
    {
    unavailable:
        return FALSE;
    }
    if (!GamePoolVehicleGetAt(m_nHandle))
        goto unavailable;
    Scripting::ProcessCommand(&g_vehicleRemoveComponentCommand, m_nHandle, (unsigned int)component);
    return TRUE;
}

// 0x000b80f0
void CVehicle::SetPaintjob(BYTE paintjob)
{
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 1 && paintjob <= 3)
    {
        Scripting::ProcessCommand(&g_vehiclePaintjobCommand, m_nHandle, (unsigned int)paintjob);
    }
}

// 0x000b8150
void CVehicle::SetLicensePlateText(const char* text)
{
    strncpy(m_szLicensePlateText, text, 32);
}

// 0x000b8170
void CVehicle::SetRotation(float angle)
{
    if (GamePoolVehicleGetAt(m_nHandle))
        Scripting::ProcessCommand(&g_vehicleRotationCommand, m_nHandle, (double)angle);
}

// 0x000b82b0
float CVehicle::GetBackendFloat648()
{
    return m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) ? *(float*)((BYTE*)m_pGameVehicle + 0x648) : g_objectScaleLowerBound;
}

// 0x000b8370
void CVehicle::SetWindowOpenFlag(BYTE window)
{
    if (m_pGameVehicle)
    {
        void* backend = m_pGameVehicle;
        // clang-format off
        __asm {
            mov ecx,backend
            xor edx,edx
            mov dl,window
            push edx
            mov edx,0x6d3080
            call edx
        }
        // clang-format on
    }
}

// 0x000b83a0
void CVehicle::ClearWindowOpenFlag(BYTE window)
{
    if (m_pGameVehicle)
    {
        void* backend = m_pGameVehicle;
        // clang-format off
        __asm {
            mov ecx,backend
            xor edx,edx
            mov dl,window
            push edx
            mov edx,0x6d30b0
            call edx
        }
        // clang-format on
    }
}

// 0x000e6110

// 0x000b7190
void CVehicle::SetInvulnerable(BOOL enabled)
{
    if (m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && *(DWORD*)m_pGameVehicle != 0x863c40)
    {
        if (enabled)
        {
            Scripting::ProcessCommand(&g_vehicleProofsCommand, m_nHandle, 1, 1, 1, 1, 1);
            Scripting::ProcessCommand(&g_vehicleDamageableCommand, m_nHandle, 0);
            m_dwInvulnerable = 1;
        }
        else
        {
            Scripting::ProcessCommand(&g_vehicleProofsCommand, m_nHandle, 0, 0, 0, 0, 0);
            Scripting::ProcessCommand(&g_vehicleDamageableCommand, m_nHandle, 1);
            m_dwInvulnerable = 0;
        }
    }
}

// 0x000b75a0
void CVehicle::SetLandingGearState(int state)
{
    if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 5)
    {
        void* backend = m_pGameVehicle;
        float position = 0.0f;
        // clang-format off
        __asm {
            mov eax,backend
            mov edx,[eax + 0x9cc]
            mov position,edx
        }
        // clang-format on
        // Invalid state must not load/compare the float in x87.
        if (state == 1)
        {
            if (position == g_objectScaleLowerBound)
            {
                // clang-format off
                __asm {
                    mov ecx,backend
                    mov edx,0x6cac20
                    call edx
                }
                // clang-format on
            }
        }
        else if (state == 0 && position == g_quaternionMatrixOne)
        {
            // clang-format off
            __asm {
                mov ecx,backend
                mov edx,0x6cac70
                call edx
            }
            // clang-format on
        }
    }
}

// 0x000b7630
BOOL CVehicle::GetLandingGearState()
{
    if (!m_pGameVehicle)
        return FALSE;
    if (GameGetVehicleSubtype(m_pGameVehicle) != 5)
        return FALSE;
    void* backend = m_pGameVehicle;
    float position = 0.0f;
    // clang-format off
    __asm {
        mov eax,backend
        mov edx,[eax + 0x9cc]
        mov position,edx
    }
    // clang-format on
    if (position == g_objectScaleLowerBound) return FALSE;
    return TRUE;
}

// 0x000b7940
void CVehicle::SetTires(BYTE tires)
{
    if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 1)
    {
        if (tires & 1)
            *((BYTE*)m_pGameVehicle + 0x5a8) = 1;
        else
            *((BYTE*)m_pGameVehicle + 0x5a8) = 0;
        tires >>= 1;
        if (tires & 1)
            *((BYTE*)m_pGameVehicle + 0x5a7) = 1;
        else
            *((BYTE*)m_pGameVehicle + 0x5a7) = 0;
        tires >>= 1;
        if (tires & 1)
            *((BYTE*)m_pGameVehicle + 0x5a6) = 1;
        else
            *((BYTE*)m_pGameVehicle + 0x5a6) = 0;
        if (tires & 2)
            *((BYTE*)m_pGameVehicle + 0x5a5) = 1;
        else
            *((BYTE*)m_pGameVehicle + 0x5a5) = 0;
        return;
    }
    if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 2)
    {
        if (tires & 1)
            *((BYTE*)m_pGameVehicle + 0x65d) = 1;
        else
            *((BYTE*)m_pGameVehicle + 0x65d) = 0;
        if (tires & 2)
            *((BYTE*)m_pGameVehicle + 0x65c) = 1;
        else
            *((BYTE*)m_pGameVehicle + 0x65c) = 0;
    }
}

// 0x000b7a30
BYTE CVehicle::GetTires()
{
    BYTE result = 0;
    if (!m_pGameVehicle)
        return 0;
    if (GameGetVehicleSubtype(m_pGameVehicle) == 1)
    {
        BYTE* backend = (BYTE*)m_pGameVehicle;
        if (backend[0x5a5])
            result = 1;
        result <<= 1;
        if (backend[0x5a6])
            result |= 1;
        result <<= 1;
        if (backend[0x5a7])
            result |= 1;
        result <<= 1;
        if (backend[0x5a8])
            result |= 1;
    }
    else if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 2)
    {
        BYTE* backend = (BYTE*)m_pGameVehicle;
        if (backend[0x65c])
            result = 1;
        result <<= 1;
        if (backend[0x65d])
            result |= 1;
    }
    return result;
}

// 0x000b7ac0
void CVehicle::UpdateDamage(DWORD panels, DWORD doors, BYTE lights)
{
    if (!m_pGameVehicle)
        return;
    if (GameGetVehicleSubtype(m_pGameVehicle) != 1)
    {
        if (!m_pGameVehicle || GameGetVehicleSubtype(m_pGameVehicle) != 5)
            return;
    }
    if (!panels && !doors && !lights && (*(DWORD*)((BYTE*)m_pGameVehicle + 0x5b4) || *(DWORD*)((BYTE*)m_pGameVehicle + 0x5a9) || *(DWORD*)((BYTE*)m_pGameVehicle + 0x5b0)))
    {
        void* backend = m_pGameVehicle;
        // clang-format off
        __asm { mov ecx,backend
            mov edx,0x6a3440
            call edx }
        // clang-format on
        return;
    }
    *(DWORD*)((BYTE*)m_pGameVehicle + 0x5b4) = panels;
    *(DWORD*)((BYTE*)m_pGameVehicle + 0x5a9) = doors;
    *(DWORD*)((BYTE*)m_pGameVehicle + 0x5b0) = lights;
    void* backend = m_pGameVehicle;
    // clang-format off
    __asm { mov ecx,backend
        mov edx,0x6b3e90
        call edx }
    // clang-format on
}

// 0x000b7d70
BOOL CVehicle::IsTrailer()
{
    if (!m_pGameVehicle)
        return FALSE;
    if (!GamePoolVehicleGetAt(m_nHandle))
        return FALSE;
    short model = *(short*)((BYTE*)m_pGameVehicle + 0x22);
    return model == 0x1b3 || model == 0x1c2 || model == 0x248 || model == 0x24f || model == 0x25e || model == 0x25f || model == 0x260 || model == 0x262 || model == 0x263;
}

// 0x000b8250
void CVehicle::CallBackend6a6ae0(DWORD first, DWORD second, DWORD third)
{
    if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 1)
    {
        void* backend = m_pGameVehicle;
        int enable = 1;
        int mode = 0;
        // clang-format off
        __asm {
            push enable
            push third
            push first
            push second
            push mode
            mov ecx,backend
            mov ebx,0x6a6ae0
            call ebx
        }
        // clang-format on
    }
}

// 0x000b7900
void CVehicle::SetTrainSpeed(float speed)
{
    // The native bounds checks allow unordered inputs.
    if (!(speed > g_vehicleTrainSpeedMaximum) && !(speed < g_vehicleTrainSpeedMinimum) && m_pGameVehicle)
        *(float*)((BYTE*)m_pGameVehicle + 0x5a4) = speed;
}

// 0x000b82e0
void CVehicle::SetBackendFloat64c(float speed)
{
    if (!m_pGameVehicle || !GamePoolVehicleGetAt(m_nHandle))
        return;
    if (!(speed > g_quaternionMatrixOne) && !(speed < g_entityEulerSign))
        *(float*)((BYTE*)m_pGameVehicle + 0x64c) = speed;
}

// 0x000b7c90
CVehicle* CVehicle::GetTrailer()
{
    if (!m_pGameVehicle)
        return 0;
    void* backend = *(void**)((BYTE*)m_pGameVehicle + 0x4c8);
    if (backend && g_pNetGame)
    {
        BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
        CVehiclePool* pool = *(CVehiclePool**)(pools + 0xc);
        WORD id = pool->Find(backend);
        if (id != 0xffff && pool->DoesExist(id))
            return pool->Get(id);
    }
    return 0;
}

// 0x000b7cf0
CVehicle* CVehicle::GetTractor()
{
    if (!m_pGameVehicle)
        return 0;
    int subtype = GameGetVehicleSubtype(m_pGameVehicle);
    if (subtype == 2 || subtype == 4 || subtype == 7 || subtype == 6)
        return 0;
    void* backend = *(void**)((BYTE*)m_pGameVehicle + 0x4c4);
    if (backend && g_pNetGame)
    {
        BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
        CVehiclePool* pool = *(CVehiclePool**)(pools + 0xc);
        WORD id = pool->Find(backend);
        if (id != 0xffff && pool->DoesExist(id))
            return pool->Get(id);
    }
    return 0;
}

// 0x000b7ee0
void CVehicle::RemovePassengers()
{
    if (!m_pGameVehicle || !GamePoolVehicleGetAt(m_nHandle))
        return;
    const CMatrix* matrix = ((EntityBackendPrefix*)m_pGameVehicle)->matrix;
    float x = matrix->position.x;
    float y = matrix->position.y;
    float z = matrix->position.z;
    void* volatile* driver = (void* volatile*)((BYTE*)m_pGameVehicle + 0x460);
    if (*driver)
    {
        int handle = GamePoolPedGetIndex(*(void* volatile*)((BYTE*)m_pGameVehicle + 0x460));
        Scripting::ProcessCommand(&g_vehicleWarpPedCommand, handle, (double)x, (double)y, (double)(z + 2.0f));
    }
    for (int offset = 0x464; offset < 0x480; offset += 4)
    {
        void* ped = *(void**)((BYTE*)m_pGameVehicle + offset);
        if (ped)
        {
            int handle = GamePoolPedGetIndex(ped);
            Scripting::ProcessCommand(&g_vehicleWarpPedCommand, handle, (double)x, (double)y, (double)(z + 2.0f));
        }
    }
}

// 0x000b7fc0
BOOL CVehicle::AddComponent(WORD component)
{
    DWORD result = (DWORD)this;
    if (!m_pGameVehicle || !GamePoolVehicleGetAt(m_nHandle) || !m_pGameVehicle || GameGetVehicleSubtype(m_pGameVehicle) != 1)
        return FALSE;
    g_scriptVehicleComponent = component;
    unsigned int model = component;
    if (!Scripting::ProcessCommand(&g_vehicleComponentLoadedCommand, model))
    {
        g_pGame->RequestModel(model, 2);
        g_pGame->LoadRequestedModels();
        Scripting::ProcessCommand(&g_vehicleRequestComponentCommand, model);
        void(WINAPI * sleep)(DWORD) = Sleep;
        for (int attempts = 20; attempts != 0; --attempts)
        {
            if (Scripting::ProcessCommand(&g_vehicleComponentLoadedCommand, model))
                break;
            sleep(1);
        }
    }
    if (!Scripting::ProcessCommand(&g_vehicleComponentLoadedCommand, model))
        return FALSE;
    Scripting::ProcessCommand(&g_vehicleAddComponentCommand, m_nHandle, model, &result);
    return TRUE;
}

// Raw wrapper views do not establish the full allocation extent.
// 0x000b76e0
void CVehicle::ProcessMarkers()
{
    if (!m_pGameVehicle)
        return;
    if (m_byteObjective)
    {
        if (!m_dwObjectiveMarker)
        {
            if (m_dwMarker)
            {
                Scripting::ProcessCommand(&g_vehicleDestroyMarkerCommand, m_dwMarker);
                m_dwMarker = 0;
            }
            Scripting::ProcessCommand(&g_vehicleCreateMarkerCommand, m_nHandle, 1, 3, &m_dwMarker);
            Scripting::ProcessCommand(&g_vehicleMarkerColorCommand, m_dwMarker, 0x3ee);
            Scripting::ProcessCommand(&g_vehicleMarkerModeCommand, m_dwMarker, 3);
            m_dwObjectiveMarker = 1;
        }
        return;
    }
    if (m_dwObjectiveMarker && m_dwMarker)
    {
        Scripting::ProcessCommand(&g_vehicleDestroyMarkerCommand, m_dwMarker);
        m_dwObjectiveMarker = 0;
        m_dwMarker = 0;
    }
    if (GetDistanceToLocalPlayer() < g_vehicleMarkerDistance && !IsOccupied())
    {
        if (!m_dwMarker)
        {
            Scripting::ProcessCommand(&g_vehicleCreateMarkerCommand, m_nHandle, 1, 2, &m_dwMarker);
            Scripting::ProcessCommand(&g_vehicleMarkerColorCommand, m_dwMarker, 0x3ec);
        }
        return;
    }
    if ((IsOccupied() || GetDistanceToLocalPlayer() >= g_vehicleMarkerDistance) && m_dwMarker)
    {
        Scripting::ProcessCommand(&g_vehicleDestroyMarkerCommand, m_dwMarker);
        m_dwMarker = 0;
    }
}

// GTA SA backend: 0x004FCF40, ECX=original backend+138.
// GTA SA shared word 0x00B6B990 is saved only around that backend call.
// 0x000b8a70
void CVehicle::EnableEngine(BOOL enabled)
{
    if (!m_pGameVehicle)
        return;
    BYTE* initial = (BYTE*)m_pGameVehicle;
    if (!enabled)
    {
        initial[0x428] &= 0xef;
        if (!m_pGameVehicle || GameGetVehicleSubtype(m_pGameVehicle) != 3 || !m_dwEngineOverride75)
            return;
        BYTE* current = (BYTE*)m_pGameVehicle;
        if (current && *(float*)(current + 0x4c0) > g_objectScaleLowerBound)
        {
            DWORD saved = *(DWORD*)0xb6b990;
            // clang-format off
            __asm {
                mov eax,initial
                lea ecx,[eax+0x138]
                mov edx,0x4fcf40
                call edx
            }
            // clang-format on
            *(DWORD*)0xb6b990 = saved;
        }
        m_dwEngineOverride75 = 0;
    }
    else
    {
        initial[0x428] |= 0x10;
        if (m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle) == 3 && HasDriver() && !DriverIsPlayerPed())
            m_dwEngineOverride75 = 1;
    }
}

// Raw GTA driver interface, no complete ped/vtable allocation is inferred.
// 0x000b42b0
void __stdcall GameRemoveDriverByState598(void* vehicle)
{
    if (!vehicle)
        return;
    void* driver = *(void**)((BYTE*)vehicle + 0x460);
    if (!driver)
        return;
    DWORD state = *(DWORD*)((BYTE*)driver + 0x598);
    if (!state || state == 1)
        return;
    // clang-format off
    __asm {
        mov ecx,driver
        mov eax,dword ptr [ecx]
        push 1
        call dword ptr [eax]
    }
    // clang-format on
    *(void**)((BYTE*)vehicle + 0x460) = 0;
}

// 0x000b81a0
void CVehicle::ConstructLicensePlate()
{
    if (!m_pLicensePlateTexture)
    {
        const char* text = m_szLicensePlateText;
        if (strlen(text))
            m_pLicensePlateTexture = ((CLicensePlate*)g_devicePlateManager)->Create(text);
    }
}

// 0x000b81f0
void CVehicle::ShutdownLicensePlate()
{
    IDirect3DTexture9** texture = &m_pLicensePlateTexture;
    if (*texture)
    {
        (*texture)->Release();
        *texture = 0;
    }
}

// 0x000b7230
void CVehicle::SetLocked(BOOL locked)
{
    if (m_pGameVehicle)
    {
        if (locked)
            Scripting::ProcessCommand(&g_vehicleLockedCommand, m_nHandle, 1);
        else
            Scripting::ProcessCommand(&g_vehicleLockedCommand, m_nHandle, 0);
    }
}

// 0x000b73d0
BOOL CVehicle::IsWrecked()
{
    if (!m_pGameVehicle)
        return FALSE;
    return Scripting::ProcessCommand(&g_vehicleWreckedCommand, m_nHandle);
}

// R5 address to verify
void CVehicle::SetBackendFlag428Bit5(BOOL enabled)
{
    if (m_pGameVehicle)
    {
        if (!enabled)
            ((BYTE*)m_pGameVehicle)[0x428] &= 0xdf;
        else
            ((BYTE*)m_pGameVehicle)[0x428] |= 0x20;
    }
}

// R5 address to verify
float CVehicle::GetBackendFloat94C()
{
    return *(float*)((BYTE*)m_pGameVehicle + 0x94c);
}

// R5 address to verify
float CVehicle::GetBackendFloat950()
{
    return *(float*)((BYTE*)m_pGameVehicle + 0x950);
}

// R5 address to verify
void CVehicle::SetBackendFloat94CAnd950(float first, float second)
{
    memcpy((BYTE*)m_pGameVehicle + 0x94c, &first, 4);
    memcpy((BYTE*)m_pGameVehicle + 0x950, &second, 4);
}

// 0x000b7460
BOOL CVehicle::IsTrainPart()
{
    if (m_pGameVehicle)
    {
        int model = *(short*)((BYTE*)m_pGameVehicle + 0x22);
        if (model == 0x21a || model == 0x23a || model == 0x219 || model == 0x239 || model == 0x1c1)
            return TRUE;
    }
    return FALSE;
}
