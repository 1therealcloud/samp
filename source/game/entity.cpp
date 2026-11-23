#include "game/entity.h"

#include "game/camera.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/scripting.h"
#include "game/util.h"
#include "gui/chatwindow.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/playerpool.h"

#include <math.h>
#include <string.h>
// 0x001181b8
extern const float g_entityRadiansToDegrees = 57.2957763671875f;
// 0x0011c660
extern const float g_entityEulerSign = -1.0f;
// 0x0011b31c
extern const float g_entityBoundaryLift = 0.03999999910593033f;

// 0x0011e644
const float g_entityUnavailableDistance = 100000.0f;
GameEntityCollisionShape g_entityCollisionShapeA;
// 0x0018de4c
GameEntityCollisionShape g_entityCollisionShapeB;
// 0x0018de7c
void* g_entityCollisionEntity;
// 0x00155404
GameEntityCollisionBackend g_entityCollisionBackend = (GameEntityCollisionBackend)0x419f00; // GTA SA virtual address.
// 0x00155408
void** g_entityCollisionContextSlot = (void**)0x9655cc; // GTA SA virtual address.
// 0x002abe5c
void* g_entityCollisionSavedContext;
// 0x002abe60
BYTE g_entityCollisionResult;
// 0x0011e64c
const char g_entityDetachExceptionLabel[5] = "s113";

// 0x0009e950
void CEntity::GetMatrix(CMatrix* matrix)
{
    if (!m_pEntity || !m_pEntity->matrix)
        return;
    matrix->right.x = m_pEntity->matrix->right.x;
    matrix->right.y = m_pEntity->matrix->right.y;
    matrix->right.z = m_pEntity->matrix->right.z;
    matrix->up.x = m_pEntity->matrix->up.x;
    matrix->up.y = m_pEntity->matrix->up.y;
    matrix->up.z = m_pEntity->matrix->up.z;
    matrix->at.x = m_pEntity->matrix->at.x;
    matrix->at.y = m_pEntity->matrix->at.y;
    matrix->at.z = m_pEntity->matrix->at.z;
    matrix->position.x = m_pEntity->matrix->position.x;
    matrix->position.y = m_pEntity->matrix->position.y;
    matrix->position.z = m_pEntity->matrix->position.z;
}

// 0x0009ee80
void CEntity::Teleport(float x, float y, float z)
{
    void* entity = m_pGameEntity;
    if (entity && *(DWORD*)entity != 0x863c40)
    {
        short model = *(short*)((BYTE*)entity + 0x22);
        if (model == 0x21a || model == 0x219 || model == 0x1c1)
            Scripting::ProcessCommand(&g_entityTeleportCommand, m_nHandle, (double)x, (double)y, (double)z);
        else
        {
            // clang-format off
            __asm {
                mov ecx,entity
                mov edx,[ecx]
                push 0
                push z
                push y
                push x
                call dword ptr [edx+0x38]
            }
            // clang-format on
        }
    }
}

// 0x0009f140
void CEntity::Add()
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40 && !*(DWORD*)((BYTE*)m_pGameEntity + 0xb4))
    {
        CVector zero = {0.0f, 0.0f, 0.0f};
        SetSpeed(zero);
        SetTurnSpeed(zero);
        GameWorldAdd(m_pGameEntity);
        CMatrix matrix;
        GetMatrix(&matrix);
        Teleport(matrix.position.x, matrix.position.y, matrix.position.z);
    }
}

// 0x0009f240
void CEntity::Remove()
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40 && *(DWORD*)((BYTE*)m_pGameEntity + 0xb4))
        GameWorldRemove(m_pGameEntity);
}

// 0x0009ea00
void CEntity::SetMatrix(CMatrix matrix)
{
    if (!m_pEntity || !m_pEntity->matrix)
        return;
    m_pEntity->matrix->right.x = matrix.right.x;
    m_pEntity->matrix->right.y = matrix.right.y;
    m_pEntity->matrix->right.z = matrix.right.z;
    m_pEntity->matrix->up.x = matrix.up.x;
    m_pEntity->matrix->up.y = matrix.up.y;
    m_pEntity->matrix->up.z = matrix.up.z;
    m_pEntity->matrix->at.x = matrix.at.x;
    m_pEntity->matrix->at.y = matrix.at.y;
    m_pEntity->matrix->at.z = matrix.at.z;
    m_pEntity->matrix->position.x = matrix.position.x;
    m_pEntity->matrix->position.y = matrix.position.y;
    m_pEntity->matrix->position.z = matrix.position.z;
}

// 0x0009eb20
void CEntity::GetSpeed(CVector* speed)
{
    speed->x = m_pEntity->speed.x;
    speed->y = m_pEntity->speed.y;
    speed->z = m_pEntity->speed.z;
}

// 0x0009eb50
void CEntity::SetSpeed(CVector speed)
{
    m_pEntity->speed.x = speed.x;
    m_pEntity->speed.y = speed.y;
    m_pEntity->speed.z = speed.z;
}

// 0x0009ec70
void CEntity::GetTurnSpeed(CVector* speed)
{
    speed->x = m_pEntity->turnSpeed.x;
    speed->y = m_pEntity->turnSpeed.y;
    speed->z = m_pEntity->turnSpeed.z;
}

// 0x0009eca0
void CEntity::SetTurnSpeed(CVector speed)
{
    m_pEntity->turnSpeed.x = speed.x;
    m_pEntity->turnSpeed.y = speed.y;
    m_pEntity->turnSpeed.z = speed.z;
}

// 0x0009ecd0
void CEntity::ApplyTurnSpeed()
{
    void* entity = m_pGameEntity;
    if (!entity)
        return;
    // GTA SA virtual address: ECX thiscall backend.
    typedef void(__fastcall * BackendFn)(void*, int);
    reinterpret_cast<BackendFn>(0x542e20)((void*)entity, 0);
}

// GTA returns raw float bits in EAX, not an ST0 float result.
// 0x0009ecf0
float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
    void* entity = m_pGameEntity;
    float distance = 0.0f;
    if (!entity)
        return 0.0f;
    // GTA SA virtual address: result is raw float bits in EAX.
    // clang-format off
    __asm {
        mov ecx,entity
        mov edx,0x536be0
        call edx
        mov distance,eax
    }
    // clang-format on
    return distance;
}

// 0x0009ed30
void CEntity::GetBoundCentre(CVector* centre)
{
    void* entity = m_pGameEntity;
    if (!entity)
        return;
    // GTA SA virtual address: ECX entity, one callee-popped pointer.
    typedef void(__fastcall * BackendFn)(void*, int, void*);
    reinterpret_cast<BackendFn>(0x534250)((void*)entity, 0, (void*)centre);
}

// 0x0009ee70
int CEntity::GetModelIndex()
{
    return (int)*(short*)((BYTE*)m_pGameEntity + 0x22);
}

// 0x0009f210
BOOL CEntity::DoesExist()
{
    return m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40 && *(DWORD*)((BYTE*)m_pGameEntity + 0xb4) ? TRUE : FALSE;
}

// 0x0009f260
BOOL CEntity::EnforceWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY)
{
    if (!m_pEntity)
        return FALSE;
    CMatrix matrix;
    CVector speed;
    GetMatrix(&matrix);
    GetSpeed(&speed);
    if (matrix.position.x > positiveX)
    {
        if (speed.x != 0.0f)
        {
            speed.x = -0.2f;
            speed.z = 0.1f;
        }
        SetSpeed(speed);
        matrix.position.z += g_entityBoundaryLift;
        SetMatrix(matrix);
        return TRUE;
    }
    else if (matrix.position.x < negativeX)
    {
        if (speed.x != 0.0f)
        {
            speed.x = 0.2f;
            speed.z = 0.1f;
        }
        SetSpeed(speed);
        matrix.position.z += g_entityBoundaryLift;
        SetMatrix(matrix);
        return TRUE;
    }
    else if (matrix.position.y > positiveY)
    {
        if (speed.y != 0.0f)
        {
            speed.y = -0.2f;
            speed.z = 0.1f;
        }
        SetSpeed(speed);
        matrix.position.z += g_entityBoundaryLift;
        SetMatrix(matrix);
        return TRUE;
    }
    else if (matrix.position.y < negativeY)
    {
        if (speed.y != 0.0f)
        {
            speed.y = 0.2f;
            speed.z = 0.1f;
        }
        SetSpeed(speed);
        matrix.position.z += g_entityBoundaryLift;
        SetMatrix(matrix);
        return TRUE;
    }
    return FALSE;
}

// 0x0009f400
BOOL CEntity::HasExceededWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY)
{
    if (!m_pEntity)
        return FALSE;
    CMatrix matrix;
    GetMatrix(&matrix);
    if (matrix.position.x > positiveX)
        return TRUE;
    else if (matrix.position.x < negativeX)
        return TRUE;
    else if (matrix.position.y > positiveY)
        return TRUE;
    else if (matrix.position.y < negativeY)
        return TRUE;
    return FALSE;
}

// 0x0009f4a0
DWORD CEntity::GetCollisionFlag()
{
    if (!m_pEntity || (DWORD)m_pEntity->vtable == 0x863c40)
        return 1;
    return *(BYTE*)&m_pEntity->flags & 1;
}

// 0x0009f470
void CEntity::SetCollisionFlag(BOOL enabled)
{
    if (m_pEntity && (DWORD)m_pEntity->vtable != 0x863c40)
    {
        if (enabled)
            m_pEntity->flags |= 1;
        else
            m_pEntity->flags &= ~1u;
    }
}

// 0x0009f4c0
void CEntity::SetCollisionProcessed(BOOL processed)
{
    if (m_pEntity && (DWORD)m_pEntity->vtable != 0x863c40)
    {
        if (processed)
            m_pEntity->flags &= 0x7ffffffd;
        else
            m_pEntity->flags |= 0x80000002;
    }
}

// 0x0009f730
void CEntity::GetEulerInverted(float* x, float* y, float* z)
{
    if (m_pGameEntity)
    {
        void* matrix = *(void**)((BYTE*)m_pGameEntity + 0x14);
        if (matrix)
        {
            // GTA SA virtual address: ECX matrix and four callee-popped words.
            typedef void(__fastcall * BackendFn)(void*, int, void*, void*, void*, DWORD);
            reinterpret_cast<BackendFn>(0x59a840)((void*)matrix, 0, (void*)x, (void*)y, (void*)z, 0x15);
        }
        *x = (float)((double)*x * g_entityRadiansToDegrees * g_entityEulerSign);
        *y = (float)((double)*y * g_entityRadiansToDegrees * g_entityEulerSign);
        *z = (float)((double)*z * g_entityRadiansToDegrees * g_entityEulerSign);
    }
}

// 0x0009f8a0
DWORD* CEntity::GetRwObject()
{
    if (!m_pEntity)
        return 0;
    return m_pEntity->rwObject;
}

// 0x0009f9f0
void CEntity::DeleteRwObject()
{
    if (m_pEntity && m_pEntity->rwObject)
    {
        void* entity = m_pGameEntity;
        // GTA SA vtable slot8, ECX entity.
        // clang-format off
        __asm {
            mov ecx,entity
            mov eax,[ecx]
            call dword ptr [eax+0x20]
        }
        // clang-format on
    }
}

// 0x0009fb20
bool CEntity::IsIgnored()
{
    if (m_pGameEntity && m_pGameEntity == *(void**)0xb7cd68)
        return true;
    return false;
}

// 0x0009fc20
BOOL CEntity::IsStationary()
{
    if (!DoesExist())
        return FALSE;
    if (m_pEntity->speed.x == 0.0f && m_pEntity->speed.y == 0.0f && m_pEntity->speed.z == 0.0f)
        return TRUE;
    return FALSE;
}

// 0x0009eac0
void CEntity::UpdateRwFrame()
{
    if (m_pEntity && (DWORD)m_pEntity->vtable != 0x863c40)
    {
        void* object = m_pEntity->rwObject;
        CMatrix* matrix = m_pEntity->matrix;
        void* entity = m_pGameEntity;
        if (!entity || !object || !matrix)
            return;
        // GTA SA virtual addresses: matrix copy and backend frame update.
        // clang-format off
        __asm {
            mov edx,object
            mov eax,[edx+4]
            add eax,0x10
            push eax
            mov ecx,matrix
            mov edx,0x59ad70
            call edx
            mov ecx,entity
            mov edx,0x532b00
            call edx
        }
        // clang-format on
    }
}

namespace ApplyMatrix
{
enum
{
    Backend = offsetof(CEntity, m_pGameEntity),
    Matrix = offsetof(EntityBackendPrefix, matrix),
    MatrixBytes = sizeof(CMatrix),
    MatrixWords = sizeof(CMatrix) / sizeof(DWORD),
    FrameBytes = 8,
    TableLocal = 8,
    EntityLocal = 4,
    Slot3 = 3 * sizeof(DWORD),
    Slot2 = 2 * sizeof(DWORD)
};
} // namespace ApplyMatrix

// clang-format off
// 0x0009fb40
__declspec(naked) void CEntity::ApplyMatrix(CMatrix matrix)
{
    using namespace ApplyMatrix;
    __asm {
        push ebp
        mov ebp,esp
        sub esp,FrameBytes
        push ebx
        mov ebx,ecx
        mov eax,[ebx+Backend]
        test eax,eax
        push esi
        push edi
        jz matrix_done
        mov ecx,[eax+Matrix]
        test ecx,ecx
        jz matrix_done
        mov ecx,eax
        mov edx,[ecx]
        mov [ebp-TableLocal],edx
        mov [ebp-EntityLocal],eax
        mov eax,[ebp-TableLocal]
        mov ecx,[ebp-EntityLocal]
        call dword ptr [eax+Slot3]
        sub esp,MatrixBytes
        mov edi,esp
        mov ecx,MatrixWords
        lea esi,[ebp+8]
        rep movsd
        mov ecx,ebx
        call CEntity::SetMatrix
        mov ecx,ebx
        call CEntity::UpdateRwFrame
        mov eax,[ebp-TableLocal]
        mov ecx,[ebp-EntityLocal]
        call dword ptr [eax+Slot2]
    matrix_done:
        pop edi
        pop esi
        pop ebx
        mov esp,ebp
        pop ebp
        ret MatrixBytes
    }
}
// clang-format on

// 0x0009f0f0
float CEntity::GetDistanceToPoint(float x, float y, float z)
{
    CMatrix matrix;
    GetMatrix(&matrix);
    float dz = matrix.position.z - z;
    float dy = matrix.position.y - y;
    float dx = matrix.position.x - x;
    return (float)sqrt(dx * dx + dy * dy + dz * dz);
}

// 0x0009f5c0
void CEntity::ClearBackendFlag8()
{
    if (m_pGameEntity)
        *(DWORD*)((BYTE*)m_pGameEntity + 0x40) &= 0xfffffff7;
}

// 0x0009ed90
BOOL CEntity::SetModelIndex(int model)
{
    if (!m_pGameEntity)
        return FALSE;
    BOOL loaded = FALSE;
    int polls = 0;
    if (!g_pGame->IsModelAvailable(model) && !GameGetModelData1c(model))
    {
        g_pGame->RequestModel(model, 2);
        g_pGame->LoadRequestedModels();
        while (!g_pGame->IsModelAvailable(model))
        {
            Sleep(1);
            if (++polls > 200)
            {
                if (g_pChat)
                    g_pChat->AddDebugMessage(g_gameModelTimeoutWarning, model);
                return FALSE;
            }
        }
        loaded = TRUE;
    }
    void* entity = m_pGameEntity;
    // GTA SA virtual slots8/5. Native retains the entity and model through both.
    // clang-format off
    __asm {
        mov esi,entity
        mov edi,model
        mov edx,[esi]
        mov ecx,esi
        call dword ptr [edx+0x20]
        mov eax,[esi]
        mov edx,edi
        push edi
        mov ecx,esi
        mov word ptr [esi+0x22],dx
        call dword ptr [eax+0x14]
    }
    // clang-format on
    return loaded;
}

// GTA SA virtual address: 0x00403EC0; ECX is the opaque GTA entity.
// Native forwards the complete argument word; backend reads its low model WORD.
// 0x000b3990
void GameChangeBackendEntityModel(void* entity, DWORD model)
{
    typedef void(__fastcall * BackendFn)(void*, int, DWORD);
    reinterpret_cast<BackendFn>(0x403ec0)((void*)entity, 0, model);
}
// Caller context/semantic object type is not established; raw first DWORD only.
// 0x000b4120
DWORD __stdcall GameReadBackendDword(const void* backend)
{
    return *(const DWORD*)backend;
}

// 0x0009ef00
float CEntity::GetDistanceToLocalPlayer()
{
    CPed* player = g_pGame->GetPlayerPed();
    if (!player)
        return g_entityUnavailableDistance;

    CMatrix entityMatrix, referenceMatrix;
    GetMatrix(&entityMatrix);
    CNetGame* netGame = static_cast<CNetGame*>(g_pNetGame);
    CLocalPlayer* local = netGame ? netGame->pools->players->local : 0;
    if (local && (local->m_state.spectating || local->m_state.rcMode))
        g_pGame->GetCamera()->GetMatrix(&referenceMatrix);
    else
        player->CEntity::GetMatrix(&referenceMatrix);

    const double z = (double)entityMatrix.position.z - referenceMatrix.position.z;
    const double y = (double)entityMatrix.position.y - referenceMatrix.position.y;
    const double x = (double)entityMatrix.position.x - referenceMatrix.position.x;
    return (float)sqrt(x * x + y * y + z * z);
}

// 0x0009efd0
float CEntity::GetDistanceToCamera()
{
    if (!m_pEntity || (DWORD)m_pEntity->vtable == 0x863c40)
        return g_entityUnavailableDistance;
    CMatrix matrix;
    GetMatrix(&matrix);
    // GTA SA camera position; native reads Z, Y, X.
    float z = matrix.position.z - *(const float*)0xb6f9d4;
    float y = matrix.position.y - *(const float*)0xb6f9d0;
    float x = matrix.position.x - *(const float*)0xb6f9cc;
    return (float)sqrt(x * x + y * y + z * z);
}

// 0x0009f030
float CEntity::GetDistanceToLocalPlayerNoHeight()
{
    CPed* player = g_pGame->GetPlayerPed();
    if (!player)
        return g_entityUnavailableDistance;

    CMatrix entityMatrix, referenceMatrix;
    GetMatrix(&entityMatrix);
    CNetGame* netGame = static_cast<CNetGame*>(g_pNetGame);
    CLocalPlayer* local = netGame ? netGame->pools->players->local : 0;
    if (local && (local->m_state.spectating || local->m_state.rcMode))
        g_pGame->GetCamera()->GetMatrix(&referenceMatrix);
    else
        player->CEntity::GetMatrix(&referenceMatrix);

    const double y = (double)entityMatrix.position.y - referenceMatrix.position.y;
    const double x = (double)entityMatrix.position.x - referenceMatrix.position.x;
    return (float)sqrt(x * x + y * y);
}

// 0x0009f560
void CEntity::SetBackendHighFlag()
{
    DWORD* backend = (DWORD*)m_pGameEntity;
    if (backend && *backend != 0x863c40)
        backend[7] |= 0x80000000;
}

// GTA vtable slot 10 (+28), ProcessControl. Native does not check backend.
// 0x0009fa20
void CEntity::ProcessControl()
{
    DWORD vtable = m_pEntity->vtable;
    void* entity = m_pGameEntity;
    // GTA SA vtable slot10; no null check in native.
    // clang-format off
    __asm {
        mov eax,vtable
        mov ecx,entity
        call dword ptr [eax+0x28]
    }
    // clang-format on
}

// GTA callees use their original ECX/stack ABI.
// 0x0009ed60
void CEntity::GetBackendBoundRect(DWORD argument)
{
    void* entity = m_pGameEntity;
    if (!entity)
        return;
    // GTA SA virtual address: 0x00534120; ECX=entity, one callee-popped word.
    typedef void(__fastcall * BackendFn)(void*, int, DWORD);
    reinterpret_cast<BackendFn>(0x534120)((void*)entity, 0, argument);
}

// GTA callees use their original ECX/stack ABI.
// 0x0009f5d0
void CEntity::SetRenderClumpAlpha(DWORD argument)
{
    if (!m_pEntity || !m_pEntity->rwObject || (DWORD)m_pEntity->vtable == 0x863c40)
        return;
    void* entity = m_pGameEntity;
    // GTA SA virtual addresses: 0x00863C40 sentinel; 0x00732B00 cdecl backend.
    // Native reloads the render pointer after its presence/sentinel checks.
    // clang-format off
    __asm {
        mov eax,entity
        mov edx,[eax+0x18]
        push argument
        push edx
        mov eax,0x732b00
        call eax
        pop edx
        pop edx
    }
    // clang-format on
}

// GTA callees use their original ECX/stack ABI.
// 0x0009f810
void CEntity::ApplyBackendTurnForce(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6)
{
    void* entity = m_pGameEntity;
    if (!entity)
        return;
    // GTA SA virtual address: 0x00542A50; six raw words, callee pops 24 bytes.
    typedef void(__fastcall * BackendFn)(void*, int, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
    reinterpret_cast<BackendFn>(0x542a50)((void*)entity, 0, a1, a2, a3, a4, a5, a6);
}

// GTA callees use their original ECX/stack ABI.
// 0x0009f840
void CEntity::AddVehicleAudioEvent(DWORD argument)
{
    void* entity = m_pGameEntity;
    if (!entity)
        return;
    // GTA SA virtual address: ECX subobject, two callee-popped words.
    // clang-format off
    __asm {
        push 0
        push argument
        mov edx,entity
        lea ecx,[edx+0x138]
        mov edx,0x4f6420
        call edx
    }
    // clang-format on
}

namespace RenderForModelPreview
{
enum
{
    Backend = offsetof(CEntity, m_pGameEntity),
    RwObject = offsetof(EntityBackendPrefix, rwObject),
    Model = offsetof(EntityBackendPrefix, model),
    FrameBytes = 12,
    TableLocal = 12,
    RenderLocal = 8,
    EntityLocal = 4,
    Slot17 = 17 * sizeof(DWORD),
    FirstVehicle = 400,
    LastVehicle = 611,
    Backend733160 = 0x733160,
    Backend553260 = 0x553260
};
} // namespace RenderForModelPreview

// GTA callees use their original ECX/stack ABI.
// clang-format off
// 0x0009f960
__declspec(naked) void CEntity::RenderForModelPreview()
{
    using namespace RenderForModelPreview;

    // Preserve native signed model tests and captured pointers across GTA calls.
    __asm {
        push ebp
        mov ebp,esp
        sub esp,FrameBytes
        push esi
        mov esi,ecx
        mov eax,[esi+Backend]
        mov ecx,[eax]
        mov [ebp-TableLocal],ecx
        mov ecx,eax
        test ecx,ecx
        push edi
        mov [ebp-EntityLocal],ecx
        jnz render_have_backend
        xor edi,edi
        jmp render_have_object
    render_have_backend:
        mov edi,[eax+RwObject]
    render_have_object:
        mov ax,[eax+Model]
        cmp ax,FirstVehicle
        mov [ebp-RenderLocal],edi
        jl render_prepare
        cmp ax,LastVehicle
        jg render_prepare
        test edi,edi
        jz render_prepare
        push dword ptr [ebp-RenderLocal]
        mov edx,Backend733160
        call edx
        pop edx
    render_prepare:
        mov eax,[ebp-TableLocal]
        mov ecx,[ebp-EntityLocal]
        call dword ptr [eax+Slot17]
        mov edx,[esi+Backend]
        mov ax,[edx+Model]
        cmp ax,FirstVehicle
        jl render_other_model
        cmp ax,LastVehicle
        jg render_other_model
        test edi,edi
        jz render_done
        push dword ptr [ebp-EntityLocal]
        mov edx,Backend553260
        call edx
        pop edx
        pop edi
        pop esi
        mov esp,ebp
        pop ebp
        ret
    render_other_model:
        test edi,edi
        jz render_done
        push dword ptr [ebp-EntityLocal]
        mov edx,Backend553260
        call edx
        pop edx
    render_done:
        pop edi
        pop esi
        mov esp,ebp
        pop ebp
        ret
    }
}
// clang-format on

// GTA callees use their original ECX/stack ABI.
// 0x0009fba0
void CEntity::ApplyLinearSpeed()
{
    if (m_pEntity)
    {
        // GTA SA timestep VA. Capture before GetMatrix, as in native.
        float timeStep = *(const float*)0xb7cb5c;
        CMatrix matrix;
        GetMatrix(&matrix);
        matrix.position.x += timeStep * m_pEntity->speed.x;
        matrix.position.y += timeStep * m_pEntity->speed.y;
        matrix.position.z += timeStep * m_pEntity->speed.z;
        ApplyMatrix(matrix);
    }
}

// 0x0018de60

// 0x0009fa40
bool CEntity::TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, BYTE mode)
{
    const BYTE* backend = static_cast<const BYTE*>(m_pGameEntity);
    if (!backend || *(const DWORD*)backend == 0x863c40)
        return false;
    if (GameGetModelCollisionRadius(*(const short*)(backend + 0x22)) <= g_objectScaleLowerBound)
        return false;

    for (int i = 0; i < 5; ++i)
        g_entityCollisionShapeA.words[i] = first->words[i];
    for (int i = 0; i < 5; ++i)
        g_entityCollisionShapeB.words[i] = second->words[i];
    g_entityCollisionEntity = m_pGameEntity;
    if (mode)
        return GameEntityCollisionQueryDirect(&g_entityCollisionShapeA, &g_entityCollisionShapeB, g_entityCollisionEntity);
    return GameEntityCollisionQueryScoped(&g_entityCollisionShapeA, &g_entityCollisionShapeB, g_entityCollisionEntity);
}

// 0x000b5ea0
float __cdecl GameGetModelCollisionRadius(int model)
{
    if (!GameGetModelInfo(model))
        return g_objectScaleLowerBound;
    const BYTE* info = (const BYTE*)GameGetModelInfo(model);
    const BYTE* collision = *(const BYTE* const*)(info + 0x14);
    return collision ? *(const float*)(collision + 0x24) : g_objectScaleLowerBound;
}

// 0x000b6070
__declspec(naked) bool __stdcall GameEntityCollisionQueryDirect(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity)
{
    // clang-format off
    __asm {
        mov eax,dword ptr [esp + 0xc]
        mov ecx,dword ptr [esp + 0x8]
        mov edx,dword ptr [esp + 0x4]
        push eax
        push ecx
        push edx
        call dword ptr g_entityCollisionBackend
        add esp,0xc
        ret 0xc
    }
    // clang-format on
}

// 0x000b6090
__declspec(naked) bool __stdcall GameEntityCollisionQueryScoped(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity)
{
    // clang-format off
    __asm {
        mov eax,g_entityCollisionContextSlot
        mov ecx,dword ptr [eax]
        mov edx,dword ptr [esp + 0xc]
        mov dword ptr g_entityCollisionSavedContext,ecx
        mov ecx,dword ptr [esp + 0x4]
        mov dword ptr [eax],offset g_entityCollisionScratch
        mov eax,dword ptr [esp + 0x8]
        push edx
        push eax
        push ecx
        call dword ptr g_entityCollisionBackend
        mov edx,dword ptr g_entityCollisionSavedContext
        mov g_entityCollisionResult,al
        mov eax,g_entityCollisionContextSlot
        mov dword ptr [eax],edx
        mov al,g_entityCollisionResult
        add esp,0xc
        ret 0xc
    }
    // clang-format on
}

// GTA SA virtual address: 0x00411E70; exactly two cdecl arguments.
// 0x000b60e0
BOOL GameTestCollisionSpheres(const GameCollisionSphere* first, const GameCollisionSphere* second)
{
    return ((BOOL(__cdecl*)(const GameCollisionSphere*, const GameCollisionSphere*))0x411e70)(first, second);
}

// GTA SA virtual address: 0x00410CE0; exact query semantics remain unresolved.
// Native forwards the input pointer plus a literal zero and returns raw AL.
// 0x000b43f0
BYTE GameHasCollisionLoaded(const void* data)
{
    return ((BYTE(__cdecl*)(const void*, DWORD))0x410ce0)(data, 0);
}

// GTA SA virtual address: 0x0056BA00. Native is a transparent tail bridge:
// preserve caller argument copies and the backend's raw AL return.
// 0x000b3710
__declspec(naked) BYTE GameProcessLineOfSightRaw(const CVector* origin, const CVector* target, void* colPoint, void** entity, DWORD buildings, DWORD vehicles, DWORD peds,
                                                 DWORD objects, DWORD dummies, DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough)
{
    // clang-format off
    __asm {
        mov eax,0x56ba00
        jmp eax
    }
    // clang-format on
}

// 0x0009f500
void CEntity::SetStaticWaitingForCollision(BOOL enabled)
{
    if (!m_pGameEntity || *(DWORD*)m_pGameEntity == 0x863c40)
        return;
    if (enabled)
        *(DWORD*)((BYTE*)m_pGameEntity + 0x1c) |= 0x40000;
    else
    {
        *(DWORD*)((BYTE*)m_pGameEntity + 0x1c) &= ~0x40000u;
        Add();
    }
}

// 0x0009f540
void CEntity::DisableStreamingAndDeletion()
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
        *(DWORD*)((BYTE*)m_pGameEntity + 0x1c) |= 0x80400;
}

// 0x0009f580
void CEntity::SetSpeedApplicationEnabled(BOOL clear)
{
    if (!m_pEntity || (DWORD)m_pEntity->vtable == 0x863c40)
        return;
    DWORD entity = (DWORD)m_pEntity;
    // Tests bit 13 of the physical flags.
    // clang-format off
    if (!clear)
    {
        __asm mov edx,entity
        __asm mov eax,[edx+0x40]
        __asm or ah,0x20
        __asm mov [edx+0x40],eax
    }
    else
    {
        __asm mov edx,entity
        __asm mov eax,[edx+0x40]
        __asm and ah,0xdf
        __asm mov [edx+0x40],eax
    }
    // clang-format on
}

// 0x0009f610
DWORD CEntity::GetRwObjectWord38()
{
    void* backend = m_pGameEntity;
    if (!backend)
        return 0;
    BYTE* object = *(BYTE**)((BYTE*)backend + 0x18);
    if (!object || *(DWORD*)backend == 0x863c40)
        return 0;
    return *(DWORD*)(object + 0x38);
}

// 0x0009f7a0
void CEntity::GetRwMatrixAngles(float* x, float* y, float* z)
{
    if (!m_pGameEntity)
        return;
    CMatrix* matrix = *(CMatrix**)((BYTE*)m_pGameEntity + 0x14);
    if (!matrix)
        return;
    float angle = 0;
    GameQueryRwMatrixAngle(matrix, 0, &angle);
    *x = angle;
    GameQueryRwMatrixAngle(matrix, 1, &angle);
    *y = angle;
    GameQueryRwMatrixAngle(matrix, 2, &angle);
    *z = angle;
}

// 0x0009f870
int CEntity::GetModelAtomicCount()
{
    if (m_pEntity)
    {
        DWORD clump = GameGetModelData1c(m_pEntity->model);
        if (clump)
            return GameRwCountClumpAtomics((void*)clump);
    }
    return -1;
}

// History moves from last toward first in place: overlapping copies deliberately cascade.
// 0x0009eb80
void CEntity::SetAveragedSpeed(CVector speed)
{
    for (int i = 3; i >= 0; --i)
        memcpy(&m_aSpeedHistory[i], &m_aSpeedHistory[i + 1], sizeof(CVector));
    memcpy(&m_aSpeedHistory[4], &speed, sizeof(speed));

    const CVector* history = m_aSpeedHistory;
    volatile float x = history[0].x + history[1].x;
    x = x + history[2].x;
    const volatile float yFirst = (float)((double)history[0].y + history[1].y + history[2].y);
    const volatile float y = yFirst + history[3].y;
    volatile float z = (float)((double)history[0].z + history[1].z + history[2].z + history[3].z);
    z = z + history[4].z;
    const volatile float yTotal = y + history[4].y;
    const volatile float scaledZ = z * g_chatBubbleHeightLift;

    m_pEntity->speed.x = (float)(((double)x + history[3].x + history[4].x) * g_chatBubbleHeightLift);
    m_pEntity->speed.y = yTotal * g_chatBubbleHeightLift;
    m_pEntity->speed.z = scaledZ;
}

// 0x0009f630
void CEntity::SetEulerRotation(float x, float y, float z)
{
    const double xRadians = (double)x * g_objectDegreesToRadians;
    const volatile float yRadians = y * g_objectDegreesToRadians;
    const volatile float zRadians = z * g_objectDegreesToRadians;
    const volatile float cx = (float)cos(xRadians);
    const double sx = sin(xRadians);
    const double cy = cos((double)yRadians), sy = sin((double)yRadians);
    const double cz = cos((double)zRadians), sz = sin((double)zRadians);
    const volatile float roundedSz = (float)sz;
    const volatile float czsx = (float)(cz * sx);
    const double szsx = sz * sx;

    m_pEntity->matrix->right.x = (float)(cz * cy - szsx * sy);
    m_pEntity->matrix->right.y = (float)(czsx * sy + (double)roundedSz * cy);
    m_pEntity->matrix->right.z = (float)(-sy * cx);
    m_pEntity->matrix->up.x = (float)(-(double)roundedSz * cx);
    m_pEntity->matrix->up.y = (float)(cz * cx);
    m_pEntity->matrix->up.z = (float)sx;
    m_pEntity->matrix->at.x = (float)(szsx * cy + cz * sy);
    m_pEntity->matrix->at.y = (float)(roundedSz * sy - czsx * cy);
    m_pEntity->matrix->at.z = (float)(cy * cx);
}

// Does not use incoming ECX.
// GTA SA virtual addresses: 0x00733160 and 0x00553260.
// 0x0009f8b0
void __cdecl GameDetachBackendRwObject(void* entity, void* object)
{
    if (!entity || !object)
        return;
    __try
    {
        BYTE* vtable = *(BYTE**)entity;
        short model = *(short*)((BYTE*)entity + 0x22);
        if (model >= 400 && model <= 611)
            ((void(__cdecl*)(void*))0x733160)(object);
        // clang-format off
        __asm {
            mov ecx,entity
            mov eax,vtable
            call dword ptr [eax+0x44]
            mov ecx,entity
            mov eax,vtable
            call dword ptr [eax+0x48]
        }
        // clang-format on
        ((void(__cdecl*)(void*))0x553260)(entity);
    }
    __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), g_entityDetachExceptionLabel))
    {}
}
