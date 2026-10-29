#include "game/entity.h"
#include "game/game.h"
#include "game/scripting.h"
#include <string.h>
// 0x0009bce0
CEntity::CEntity()
{
    memset(m_aSpeedHistory, 0, sizeof(m_aSpeedHistory));
}
// 0x0009bd00
CEntity::~CEntity() {}
// 0x000b3720
void __stdcall GameWorldAdd(void* entity)
{
    ((void(__cdecl*)(void*))0x563220)(entity);
}
// 0x000b3740
void __stdcall GameWorldRemove(void* entity)
{
    ((void(__cdecl*)(void*))0x563280)(entity);
}
// 0x0009e950
void CEntity::GetMatrix(CMatrix* matrix)
{
    if (m_pGameEntity && *(void**)((BYTE*)m_pGameEntity + 0x14))
    {
        BYTE* source = *(BYTE**)((BYTE*)m_pGameEntity + 0x14);
        for (int i = 0; i < 4; i++)
            memcpy((BYTE*)matrix + i * 16, source + i * 16, 12);
    }
}
// 0x0009ee80
void CEntity::Teleport(CVector position)
{
    void* entity = m_pGameEntity;
    if (entity && *(DWORD*)entity != 0x863c40)
    {
        short model = *(short*)((BYTE*)entity + 0x22);
        if (model == 0x21a || model == 0x219 || model == 0x1c1)
            Scripting::ProcessCommand(&g_entityTeleportCommand, m_nHandle, (double)position.x, (double)position.y, (double)position.z);
        else
        {
            // clang-format off
            __asm {
                push 0
                push position.z
                push position.y
                push position.x
                mov ecx,entity
                mov eax,[ecx]
                call dword ptr [eax+0x38]
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
        memset((BYTE*)m_pGameEntity + 0x44, 0, 24);
        GameWorldAdd(m_pGameEntity);
        CMatrix matrix;
        GetMatrix(&matrix);
        Teleport(matrix.position);
    }
}
// 0x0009f240
void CEntity::Remove()
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40 && *(DWORD*)((BYTE*)m_pGameEntity + 0xb4))
        GameWorldRemove(m_pGameEntity);
}

// 0x001181b8
extern const float g_entityRadiansToDegrees = 57.2957763671875f;
// 0x0011c660
extern const float g_entityEulerSign = -1.0f;
// 0x0011b31c
extern const float g_entityBoundaryLift = 0.03999999910593033f;
// 0x0009ea00
void CEntity::SetMatrix(CMatrix matrix)
{
    if (m_pGameEntity && *(void**)((BYTE*)m_pGameEntity + 0x14))
    {
        for (int row = 0; row < 4; row++)
            for (int word = 0; word < 3; word++)
            {
                BYTE* target = *(BYTE**)((BYTE*)m_pGameEntity + 0x14);
                *(DWORD*)(target + row * 16 + word * 4) = *(DWORD*)((BYTE*)&matrix + row * 16 + word * 4);
            }
    }
}
// 0x0009eb20
void CEntity::GetSpeed(CVector* speed)
{
    memcpy(speed, (BYTE*)m_pGameEntity + 0x44, 12);
}
// 0x0009eb50
void CEntity::SetSpeed(CVector speed)
{
    memcpy((BYTE*)m_pGameEntity + 0x44, &speed, 12);
}
// 0x0009ec70
void CEntity::GetTurnSpeed(CVector* speed)
{
    memcpy(speed, (BYTE*)m_pGameEntity + 0x50, 12);
}
// 0x0009eca0
void CEntity::SetTurnSpeed(CVector speed)
{
    memcpy((BYTE*)m_pGameEntity + 0x50, &speed, 12);
}
// 0x0009ecd0
void CEntity::ApplyTurnSpeed()
{
    if (m_pGameEntity)
        ((void(__fastcall*)(void*, void*))0x542e20)(m_pGameEntity, 0);
}
// GTA returns raw float bits in EAX, not an ST0 float result.
// 0x0009ecf0
float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
    if (!m_pGameEntity)
        return 0.0f;
    DWORD bits = ((DWORD(__fastcall*)(void*, void*))0x536be0)(m_pGameEntity, 0);
    float distance;
    memcpy(&distance, &bits, 4);
    return distance;
}
// 0x0009ed30
void CEntity::GetBoundCentre(CVector* centre)
{
    if (m_pGameEntity)
        ((void(__fastcall*)(void*, void*, CVector*))0x534250)(m_pGameEntity, 0, centre);
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
    if (!m_pGameEntity)
        return FALSE;
    CMatrix matrix;
    GetMatrix(&matrix);
    CVector speed;
    GetSpeed(&speed);
    if (matrix.position.x > positiveX)
    {
        if (speed.x != 0.0f)
        {
            speed.x = -0.2f;
            speed.z = 0.1f;
        }
    }
    else if (matrix.position.x < negativeX)
    {
        if (speed.x != 0.0f)
        {
            speed.x = 0.2f;
            speed.z = 0.1f;
        }
    }
    else if (matrix.position.y > positiveY)
    {
        if (speed.y != 0.0f)
        {
            speed.y = -0.2f;
            speed.z = 0.1f;
        }
    }
    else if (matrix.position.y < negativeY)
    {
        if (speed.y != 0.0f)
        {
            speed.y = 0.2f;
            speed.z = 0.1f;
        }
    }
    else
        return FALSE;
    SetSpeed(speed);
    matrix.position.z += g_entityBoundaryLift;
    SetMatrix(matrix);
    return TRUE;
}
// 0x0009f400
BOOL CEntity::HasExceededWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY)
{
    if (!m_pGameEntity)
        return FALSE;
    CMatrix matrix;
    GetMatrix(&matrix);
    return matrix.position.x > positiveX || matrix.position.x < negativeX || matrix.position.y > positiveY || matrix.position.y < negativeY ? TRUE : FALSE;
}
// 0x0009f4a0
bool CEntity::GetCollisionFlag()
{
    return !m_pGameEntity || *(DWORD*)m_pGameEntity == 0x863c40 || (*(DWORD*)((BYTE*)m_pGameEntity + 0x1c) & 1) != 0;
}
// 0x0009f470
void CEntity::SetCollisionFlag(BOOL enabled)
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
    {
        DWORD& flags = *(DWORD*)((BYTE*)m_pGameEntity + 0x1c);
        if (enabled)
            flags |= 1;
        else
            flags &= ~1u;
    }
}
// 0x0009f4c0
void CEntity::SetCollisionProcessed(BOOL processed)
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
    {
        DWORD& flags = *(DWORD*)((BYTE*)m_pGameEntity + 0x1c);
        if (processed)
            flags &= 0x7ffffffd;
        else
            flags |= 0x80000002;
    }
}
// 0x0009f730
void CEntity::GetEulerInverted(float* x, float* y, float* z)
{
    if (m_pGameEntity)
    {
        void* matrix = *(void**)((BYTE*)m_pGameEntity + 0x14);
        if (matrix)
            ((void(__fastcall*)(void*, void*, float*, float*, float*, int))0x59a840)(matrix, 0, x, y, z, 0x15);
        // clang-format off
        // Keep the two native x87 multiplies and the store only at the end.
        __asm {
            mov eax,x
            fld dword ptr [eax]
            fmul g_entityRadiansToDegrees
            fmul g_entityEulerSign
            fstp dword ptr [eax]
            mov eax,y
            fld dword ptr [eax]
            fmul g_entityRadiansToDegrees
            fmul g_entityEulerSign
            fstp dword ptr [eax]
            mov eax,z
            fld dword ptr [eax]
            fmul g_entityRadiansToDegrees
            fmul g_entityEulerSign
            fstp dword ptr [eax]
        }
        // clang-format on
    }
}
// 0x0009f8a0
void* CEntity::GetRwObject()
{
    return m_pGameEntity ? *(void**)((BYTE*)m_pGameEntity + 0x18) : 0;
}
// 0x0009f9f0
void CEntity::DeleteRwObject()
{
    if (m_pGameEntity && *(void**)((BYTE*)m_pGameEntity + 0x18))
        ((void(__fastcall*)(void*, void*))(*(DWORD**)m_pGameEntity)[8])(m_pGameEntity, 0);
}
// 0x0009fb20
bool CEntity::IsIgnored()
{
    return m_pGameEntity && m_pGameEntity == *(void**)0xb7cd68;
}
// 0x0009fc20
BOOL CEntity::IsStationary()
{
    if (!DoesExist())
        return FALSE;
    CVector speed;
    GetSpeed(&speed);
    return speed.x == 0.0f && speed.y == 0.0f && speed.z == 0.0f ? TRUE : FALSE;
}
// 0x0009eac0
void CEntity::UpdateRwFrame()
{
    void* entity = m_pGameEntity;
    if (entity && *(DWORD*)entity != 0x863c40)
    {
        BYTE* object = *(BYTE**)((BYTE*)entity + 0x18);
        void* matrix = *(void**)((BYTE*)entity + 0x14);
        if (object && matrix)
        {
            ((void(__fastcall*)(void*, void*, void*))0x59ad70)(matrix, 0, *(BYTE**)(object + 4) + 0x10);
            ((void(__fastcall*)(void*, void*))0x532b00)(entity, 0);
        }
    }
}

#include <math.h>
// 0x0009fb40
void CEntity::ApplyMatrix(CMatrix matrix)
{
    BYTE* entity = (BYTE*)m_pGameEntity;
    if (entity && *(void**)(entity + 0x14))
    {
        DWORD* vtable = *(DWORD**)entity;
        ((void(__fastcall*)(void*, void*))vtable[3])(entity, 0);
        SetMatrix(matrix);
        UpdateRwFrame();
        ((void(__fastcall*)(void*, void*))vtable[2])(entity, 0);
    }
}
// 0x0009f0f0
float CEntity::GetDistanceToPoint(float x, float y, float z)
{
    CMatrix matrix;
    GetMatrix(&matrix);
    // clang-format off
    // sqrt(dx^2+dy^2+dz^2), retaining unrounded differences and the extended ST0 return.
    __asm {
        lea eax,matrix
        fld dword ptr [eax+38h]
        fsub z
        fld dword ptr [eax+34h]
        fsub y
        fld dword ptr [eax+30h]
        fsub x
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
    }
    // clang-format on
}

// 0x0009f5c0
void CEntity::ClearBackendFlag8()
{
    if (m_pGameEntity)
        *(DWORD*)((BYTE*)m_pGameEntity + 0x40) &= 0xfffffff7;
}

#include "chatwindow.h"
#include "game/game.h"
#include "main.h"
// 0x0009ed90
BOOL CEntity::SetModelIndex(int model)
{
    if (!m_pGameEntity)
        return FALSE;
    BOOL loaded = FALSE;
    if (!g_pGame->IsModelAvailable(model) && !GameGetModelData1c(model))
    {
        g_pGame->RequestModel(model, 2);
        g_pGame->LoadRequestedModels();
        int polls = 0;
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
    ((void(__fastcall*)(void*, void*))(*(DWORD**)entity)[8])(entity, 0);
    *(short*)((BYTE*)entity + 0x22) = (short)model;
    ((void(__fastcall*)(void*, void*, int))(*(DWORD**)entity)[5])(entity, 0, model);
    return loaded;
}

// GTA SA virtual address: 0x00403EC0; ECX is the opaque GTA entity.
// Native forwards the complete argument word; backend reads its low model WORD.
// 0x000b3990
void GameChangeBackendEntityModel(void* entity, DWORD model)
{
    // clang-format off
    __asm {
        push model
        mov ecx,entity
        mov edx,0x403ec0
        call edx
    }
    // clang-format on
}
// Caller context/semantic object type is not established; raw first DWORD only.
// 0x000b4120
DWORD __stdcall GameReadBackendDword(const void* backend)
{
    return *(const DWORD*)backend;
}

#include "game/camera.h"

// 0x0011e644
const float g_entityUnavailableDistance = 100000.0f;

// Native thiscall; preserve uninitialized GetMatrix outputs and extended ST0.
// 0x0009ef00
__declspec(naked) float CEntity::GetDistanceToLocalPlayer()
{
    // clang-format off
    __asm {
        sub esp,0x80
        push esi
        mov esi,ecx
        mov ecx,dword ptr g_pGame
        push edi
        call CGame::GetPlayerPed
        mov edi,eax
        test edi,edi
        jnz branch_1009ef2a
        fld float ptr g_entityUnavailableDistance
        pop edi
        pop esi
        add esp,0x80
        ret
    branch_1009ef2a:
        lea eax,[esp + 0x48]
        push eax
        mov ecx,esi
        call CEntity::GetMatrix
        mov eax,g_pNetGame
        test eax,eax
        jz branch_1009ef7f
        mov ecx,dword ptr [eax + 0x3de]
        mov edx,dword ptr [ecx + 0x8]
        mov eax,dword ptr [edx + 0x1e]
        test eax,eax
        jz branch_1009ef78
        mov ecx,dword ptr [eax + 0x12c]
        test ecx,ecx
        jnz branch_1009ef63
        mov ecx,dword ptr [eax + 0x1d4]
        test ecx,ecx
        jz branch_1009ef78
    branch_1009ef63:
        mov ecx,dword ptr g_pGame
        mov ecx,dword ptr [ecx + 0x4]
        lea eax,[esp + 0x8]
        push eax
        call CCamera::GetMatrix
        jmp branch_1009ef8b
    branch_1009ef78:
        lea edx,[esp + 0x8]
        push edx
        jmp branch_1009ef84
    branch_1009ef7f:
        lea eax,[esp + 0x8]
        push eax
    branch_1009ef84:
        mov ecx,edi
        call CEntity::GetMatrix
    branch_1009ef8b:
        fld float ptr [esp + 0x80]
        pop edi
        fsub float ptr [esp + 0x3c]
        pop esi
        fld float ptr [esp + 0x74]
        fsub float ptr [esp + 0x34]
        fld float ptr [esp + 0x70]
        fsub float ptr [esp + 0x30]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        add esp,0x80
        ret
    }
    // clang-format on
}

// Native thiscall; preserve uninitialized GetMatrix outputs and extended ST0.
// 0x0009efd0
__declspec(naked) float CEntity::GetDistanceToCamera()
{
    // clang-format off
    __asm {
        mov eax,dword ptr [ecx + 0x40]
        sub esp,0x40
        test eax,eax
        jz branch_1009f025
        cmp dword ptr [eax],0x863c40
        jz branch_1009f025
        lea eax,[esp]
        push eax
        call CEntity::GetMatrix
        fld float ptr [esp + 0x38]
        fsub dword ptr ds:[0x00b6f9d4]
        fld float ptr [esp + 0x34]
        fsub dword ptr ds:[0x00b6f9d0]
        fld float ptr [esp + 0x30]
        fsub dword ptr ds:[0x00b6f9cc]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        add esp,0x40
        ret
    branch_1009f025:
        fld float ptr g_entityUnavailableDistance
        add esp,0x40
        ret
    }
    // clang-format on
}

// Native thiscall; preserve uninitialized GetMatrix outputs and extended ST0.
// 0x0009f030
__declspec(naked) float CEntity::GetDistanceToLocalPlayerNoHeight()
{
    // clang-format off
    __asm {
        sub esp,0x80
        push esi
        mov esi,ecx
        mov ecx,dword ptr g_pGame
        push edi
        call CGame::GetPlayerPed
        mov edi,eax
        test edi,edi
        jnz branch_1009f05a
        fld float ptr g_entityUnavailableDistance
        pop edi
        pop esi
        add esp,0x80
        ret
    branch_1009f05a:
        lea eax,[esp + 0x48]
        push eax
        mov ecx,esi
        call CEntity::GetMatrix
        mov eax,g_pNetGame
        test eax,eax
        jz branch_1009f0af
        mov ecx,dword ptr [eax + 0x3de]
        mov edx,dword ptr [ecx + 0x8]
        mov eax,dword ptr [edx + 0x1e]
        test eax,eax
        jz branch_1009f0a8
        mov ecx,dword ptr [eax + 0x12c]
        test ecx,ecx
        jnz branch_1009f093
        mov ecx,dword ptr [eax + 0x1d4]
        test ecx,ecx
        jz branch_1009f0a8
    branch_1009f093:
        mov ecx,dword ptr g_pGame
        mov ecx,dword ptr [ecx + 0x4]
        lea eax,[esp + 0x8]
        push eax
        call CCamera::GetMatrix
        jmp branch_1009f0bb
    branch_1009f0a8:
        lea edx,[esp + 0x8]
        push edx
        jmp branch_1009f0b4
    branch_1009f0af:
        lea eax,[esp + 0x8]
        push eax
    branch_1009f0b4:
        mov ecx,edi
        call CEntity::GetMatrix
    branch_1009f0bb:
        fld float ptr [esp + 0x7c]
        pop edi
        fsub float ptr [esp + 0x38]
        pop esi
        fld float ptr [esp + 0x70]
        fsub float ptr [esp + 0x30]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fsqrt
        fstp st(2)
        fstp st(0)
        add esp,0x80
        ret
    }
    // clang-format on
}

// Descriptive name: native only establishes setting backend +1C bit 31.
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
    void* backend = m_pGameEntity;
    // clang-format off
    __asm {
        mov ecx,backend
        mov eax,[ecx]
        call dword ptr [eax+0x28]
    }
    // clang-format on
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// 0x0009ed60
void CEntity::Native009ed60(DWORD argument)
{
    void* entity = m_pGameEntity;
    if (!entity)
        return;
    // clang-format off
    // GTA SA virtual address: 0x00534120; ECX=entity, one callee-popped word.
    __asm {
        push argument
        mov ecx,entity
        mov edx,0x534120
        call edx
    }
    // clang-format on
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// 0x0009f5d0
void CEntity::Native009f5d0(DWORD argument)
{
    BYTE* entity = (BYTE*)m_pGameEntity;
    if (!entity || !*(void* volatile*)(entity + 0x18) || *(DWORD*)entity == 0x863c40)
        return;
    // GTA SA virtual addresses: 0x00863C40 sentinel; 0x00732B00 cdecl backend.
    // Native reloads the render pointer after its presence/sentinel checks.
    ((void(__cdecl*)(void*, DWORD))0x732b00)(*(void* volatile*)(entity + 0x18), argument);
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// 0x0009f810
void CEntity::Native009f810(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6)
{
    void* entity = m_pGameEntity;
    if (!entity)
        return;
    // clang-format off
    // GTA SA virtual address: 0x00542A50; six raw words, callee pops 24 bytes.
    __asm {
        push a6
        push a5
        push a4
        push a3
        push a2
        push a1
        mov eax,0x542a50
        mov ecx,entity
        call eax
    }
    // clang-format on
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// 0x0009f840
void CEntity::Native009f840(DWORD argument)
{
    BYTE* entity = (BYTE*)m_pGameEntity;
    if (!entity)
        return;
    void* subobject = entity + 0x138;
    // clang-format off
    // GTA SA virtual address: 0x004F6420; ECX=subobject, argument followed by 0.
    __asm {
        push 0
        push argument
        mov ecx,subobject
        mov edx,0x4f6420
        call edx
    }
    // clang-format on
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// 0x0009f960
void CEntity::Native009f960()
{
    BYTE* entity = (BYTE*)m_pGameEntity;
    // Native reads the vtable before the null test; a null backend must fault.
    void** vtable = *(void***)entity;
    void* render = entity ? *(void**)(entity + 0x18) : 0;
    short model = *(short*)(entity + 0x22);
    // GTA SA virtual address: 0x00733160, cdecl render-object prepass.
    if (model >= 400 && model <= 611 && render)
        ((void(__cdecl*)(void*))0x733160)(render);
    // clang-format off
    // Keep the captured entity and vtable across the prepass. The slot itself
    // is read after the prepass, just as in the original indirect call.
    __asm {
        mov eax,vtable
        mov ecx,entity
        call dword ptr [eax + 0x44]
    }
    // clang-format on
    // Both native model-range branches perform the same render test/call.
    // Preserve their backend reload and WORD read, including its fault edge.
    (void)*(volatile short*)((BYTE*)m_pGameEntity + 0x22);
    // GTA SA virtual address: 0x00553260, using the original captured entity.
    if (render)
        ((void(__cdecl*)(void*))0x553260)(entity);
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// 0x0009fba0
__declspec(naked) void CEntity::ApplyLinearSpeed()
{
    // clang-format off
    __asm {
        sub esp,0x40
        push ebx
        mov ebx,ecx
        mov eax,dword ptr [ebx + 0x40]
        test eax,eax
        push esi
        push edi
        jz branch_1009fc0a
        fld dword ptr ds:[0x00b7cb5c]
        lea eax,[esp + 0xc]
        push eax
        call CEntity::GetMatrix
        fld st(0)
        mov eax,dword ptr [ebx + 0x40]
        fmul float ptr [eax + 0x44]
        sub esp,0x40
        mov edi,esp
        mov ecx,0x10
        fadd float ptr [esp + 0x7c]
        lea esi,[esp + 0x4c]
        fstp float ptr [esp + 0x7c]
        fld st(0)
        fmul float ptr [eax + 0x48]
        fadd float ptr [esp + 0x80]
        fstp float ptr [esp + 0x80]
        fmul float ptr [eax + 0x4c]
        fadd float ptr [esp + 0x84]
        fstp float ptr [esp + 0x84]
        rep movsd
        mov ecx,ebx
        call CEntity::ApplyMatrix
    branch_1009fc0a:
        pop edi
        pop esi
        pop ebx
        add esp,0x40
        ret
    }
    // clang-format on
}

#include "game/util.h"

// 0x0018de60
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

// 0x0009fa40
__declspec(naked) bool CEntity::TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, BYTE mode)
{
    // clang-format off
    __asm {
        push esi
        mov esi,ecx
        mov eax,dword ptr [esi + 0x40]
        test eax,eax
        jz branch_1009fb0e
        cmp dword ptr [eax],0x863c40
        jz branch_1009fb0e
        movsx eax,word ptr [eax + 0x22]
        push eax
        call GameGetModelCollisionRadius
        fcomp float ptr g_objectScaleLowerBound
        add esp,0x4
        fnstsw ax
        test ah,0x41
        jnp branch_1009fb0e
        mov ecx,dword ptr [esp + 0x8]
        mov edx,dword ptr [ecx]
        mov dword ptr g_entityCollisionShapeA,edx
        mov eax,dword ptr [ecx + 0x4]
        mov [g_entityCollisionShapeA+4],eax
        mov edx,dword ptr [ecx + 0x8]
        mov dword ptr [g_entityCollisionShapeA+8],edx
        mov eax,dword ptr [ecx + 0xc]
        mov edx,dword ptr [esp + 0xc]
        mov [g_entityCollisionShapeA+12],eax
        mov ecx,dword ptr [ecx + 0x10]
        mov dword ptr [g_entityCollisionShapeA+16],ecx
        mov eax,dword ptr [edx]
        mov g_entityCollisionShapeB,eax
        mov ecx,dword ptr [edx + 0x4]
        mov dword ptr [g_entityCollisionShapeB+4],ecx
        mov eax,dword ptr [edx + 0x8]
        mov [g_entityCollisionShapeB+8],eax
        mov ecx,dword ptr [edx + 0xc]
        mov al,byte ptr [esp + 0x10]
        test al,al
        mov dword ptr [g_entityCollisionShapeB+12],ecx
        mov edx,dword ptr [edx + 0x10]
        mov dword ptr [g_entityCollisionShapeB+16],edx
        mov esi,dword ptr [esi + 0x40]
        push esi
        push offset g_entityCollisionShapeB
        mov dword ptr g_entityCollisionEntity,esi
        push offset g_entityCollisionShapeA
        jz branch_1009faff
        call GameEntityCollisionQueryDirect
        test al,al
        jz branch_1009fb0e
        mov al,0x1
        pop esi
        ret 0xc
    branch_1009faff:
        call GameEntityCollisionQueryScoped
        test al,al
        jz branch_1009fb0e
        mov al,0x1
        pop esi
        ret 0xc
    branch_1009fb0e:
        xor al,al
        pop esi
        ret 0xc
    }
    // clang-format on
}

// 0x000b5ea0
__declspec(naked) float __cdecl GameGetModelCollisionRadius(int model)
{
    // clang-format off
    __asm {
        sub esp,0x8
        push esi
        mov esi,dword ptr [esp + 0x10]
        push esi
        call GameGetModelInfo
        test eax,eax
        jz branch_100b5ed7
        push esi
        call GameGetModelInfo
        mov dword ptr [esp + 0x8],eax
        mov dword ptr [esp + 0x4],0x0
        mov eax,dword ptr [esp + 0x8]
        mov edx,dword ptr [eax + 0x14]
        mov dword ptr [esp + 0x4],edx
        mov eax,dword ptr [esp + 0x4]
        test eax,eax
        jnz branch_100b5ee2
    branch_100b5ed7:
        fld float ptr g_objectScaleLowerBound
        pop esi
        add esp,0x8
        ret
    branch_100b5ee2:
        fld float ptr [eax + 0x24]
        pop esi
        add esp,0x8
        ret
    }
    // clang-format on
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
BYTE GameQueryBackend410ce0(const void* data)
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
void CEntity::SetBackendFlag40000(BOOL enabled)
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
void CEntity::SetBackendFlags80400()
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
        *(DWORD*)((BYTE*)m_pGameEntity + 0x1c) |= 0x80400;
}
// 0x0009f580
void CEntity::SetBackendFlag2000(BOOL clear)
{
    void* backend = m_pGameEntity;
    if (!backend || *(DWORD*)backend == 0x863c40)
        return;
    DWORD& flags = *(DWORD*)((BYTE*)backend + 0x40);
    if (!clear)
        flags |= 0x2000;
    else
        flags &= ~0x2000u;
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
    if (!m_pGameEntity)
        return -1;
    DWORD clump = GameGetModelData1c(*(short*)((BYTE*)m_pGameEntity + 0x22));
    if (!clump)
        return -1;
    return GameRwCountClumpAtomics((void*)clump);
}

// Native x87 precision/spill boundaries and repeated backend loads are significant.
// History moves from last toward first in place: overlapping copies deliberately cascade.
// 0x0009eb80
__declspec(naked) void CEntity::SetAveragedSpeed(CVector speed)
{
    // clang-format off
    __asm {
        push esi
        lea esi, [ecx + 0x28]
        push edi
        mov eax, esi
        mov edx, 4
        lea esp, [esp]
    native_0009eb90:
        mov edi, dword ptr [eax + 0xc]
        mov dword ptr [eax], edi
        mov edi, dword ptr [eax + 0x10]
        mov dword ptr [eax + 4], edi
        mov edi, dword ptr [eax + 0x14]
        mov dword ptr [eax + 8], edi
        sub eax, 0xc
        dec edx
        jne native_0009eb90
        mov eax, dword ptr [esp + 0xc]
        mov edx, dword ptr [esp + 0x10]
        mov dword ptr [ecx + 0x34], eax
        mov eax, dword ptr [esp + 0x14]
        mov dword ptr [ecx + 0x3c], eax
        mov dword ptr [ecx + 0x38], edx
        fld dword ptr [ecx + 4]
        fld dword ptr [ecx + 8]
        mov edx, dword ptr [ecx + 0xc]
        fxch st(1)
        mov dword ptr [esp + 0x14], edx
        fadd dword ptr [ecx + 0x10]
        mov eax, dword ptr [ecx + 0x40]
        pop edi
        fstp dword ptr [esp + 8]
        fadd dword ptr [ecx + 0x14]
        fld dword ptr [esp + 0x10]
        fadd dword ptr [ecx + 0x18]
        fld dword ptr [esp + 8]
        fadd dword ptr [ecx + 0x1c]
        fstp dword ptr [esp + 8]
        fxch st(1)
        fadd dword ptr [ecx + 0x20]
        fstp dword ptr [esp + 0xc]
        fadd dword ptr [ecx + 0x24]
        fld dword ptr [esp + 8]
        fadd dword ptr [esi]
        pop esi
        fld dword ptr [esp + 8]
        fadd dword ptr [ecx + 0x2c]
        fstp dword ptr [esp + 8]
        fxch st(1)
        fadd dword ptr [ecx + 0x30]
        fstp dword ptr [esp + 0xc]
        fadd dword ptr [ecx + 0x34]
        fld dword ptr [esp + 8]
        fadd dword ptr [ecx + 0x38]
        fstp dword ptr [esp + 8]
        fld dword ptr [esp + 0xc]
        fadd dword ptr [ecx + 0x3c]
        fstp dword ptr [esp + 0xc]
        fmul g_chatBubbleHeightLift
        fld dword ptr [esp + 8]
        fmul g_chatBubbleHeightLift
        fld dword ptr [esp + 0xc]
        fmul g_chatBubbleHeightLift
        fstp dword ptr [esp + 0xc]
        fxch st(1)
        fstp dword ptr [eax + 0x44]
        mov edx, dword ptr [ecx + 0x40]
        fstp dword ptr [edx + 0x48]
        mov eax, dword ptr [ecx + 0x40]
        mov ecx, dword ptr [esp + 0xc]
        mov dword ptr [eax + 0x4c], ecx
        ret 0xc
    }
    // clang-format on
}
// Native x87 precision/spill boundaries and repeated backend loads are significant.
// 0x0009f630
__declspec(naked) void CEntity::SetEulerRotation(float x, float y, float z)
{
    // clang-format off
    __asm {
        fld dword ptr [esp + 4]
        mov eax, dword ptr [ecx + 0x40]
        fmul g_objectDegreesToRadians
        mov edx, dword ptr [eax + 0x14]
        fld dword ptr [esp + 8]
        fmul g_objectDegreesToRadians
        fstp dword ptr [esp + 8]
        fld dword ptr [esp + 0xc]
        fmul g_objectDegreesToRadians
        fstp dword ptr [esp + 0xc]
        fld st(0)
        fcos
        fstp dword ptr [esp + 4]
        fsin
        fld dword ptr [esp + 8]
        fcos
        fld dword ptr [esp + 8]
        fsin
        fld dword ptr [esp + 0xc]
        fcos
        fld dword ptr [esp + 0xc]
        fsin
        fst dword ptr [esp + 8]
        fmul st,st(4)
        fld st(1)
        fmul st,st(5)
        fstp dword ptr [esp + 0xc]
        fld st(1)
        fmul st,st(4)
        fld st(1)
        fmul st,st(4)
        fsubp st(1),st
        fstp dword ptr [edx]
        mov eax, dword ptr [ecx + 0x40]
        fld dword ptr [esp + 0xc]
        mov edx, dword ptr [eax + 0x14]
        fmul st,st(3)
        fld dword ptr [esp + 8]
        fmul st,st(5)
        faddp st(1),st
        fstp dword ptr [edx + 4]
        mov eax, dword ptr [ecx + 0x40]
        mov edx, dword ptr [eax + 0x14]
        fld st(2)
        fmul dword ptr [esp + 4]
        fchs
        fstp dword ptr [edx + 8]
        mov eax, dword ptr [ecx + 0x40]
        fld dword ptr [esp + 8]
        mov edx, dword ptr [eax + 0x14]
        fmul dword ptr [esp + 4]
        fchs
        fstp dword ptr [edx + 0x10]
        mov eax, dword ptr [ecx + 0x40]
        mov edx, dword ptr [eax + 0x14]
        fld st(1)
        fmul dword ptr [esp + 4]
        fstp dword ptr [edx + 0x14]
        mov eax, dword ptr [ecx + 0x40]
        mov edx, dword ptr [eax + 0x14]
        fxch st(4)
        fstp dword ptr [edx + 0x18]
        fxch st(3)
        fmul st,st(2)
        fxch st(3)
        fmul st,st(1)
        faddp st(3),st
        mov eax, dword ptr [ecx + 0x40]
        fxch st(2)
        mov edx, dword ptr [eax + 0x14]
        fstp dword ptr [edx + 0x20]
        mov eax, dword ptr [ecx + 0x40]
        fld dword ptr [esp + 8]
        mov edx, dword ptr [eax + 0x14]
        fmul st,st(2)
        fld dword ptr [esp + 0xc]
        fmul st,st(2)
        fsubp st(1),st
        fstp dword ptr [edx + 0x24]
        mov eax, dword ptr [ecx + 0x40]
        mov ecx, dword ptr [eax + 0x14]
        fstp st(1)
        fmul dword ptr [esp + 4]
        fstp dword ptr [ecx + 0x28]
        ret 0xc
    }
    // clang-format on
}

// 0x0011e64c
const char g_entityDetachExceptionLabel[5] = "s113";
// Incoming ECX is unused; original owner/name is not established.
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
