#include "game/task.h"
#include "exceptions.h"
#include "game/playerped.h"
// The default constructor deliberately leaves m_bSelfCreated untouched.
// 0x000b2180
CTask::CTask()
{
    m_pTaskType = 0;
    m_pPlayerPed = 0;
}
// 0x000b2560
CTask::CTask(DWORD size)
{
    m_pTaskType = 0;
    m_pPlayerPed = 0;
    Create(size);
}
// 0x000b2590
CTask::CTask(BYTE* task)
{
    m_pTaskType = 0;
    m_pPlayerPed = 0;
    Create(task);
}
// Native destruction only resets the vptr; Destroy is a separate operation.
// 0x000b21a0
CTask::~CTask() {}
// GTA SA virtual address: 0x0061A5A0, backend task allocation.
// 0x000b21b0
void CTask::Create(DWORD bytes)
{
    BYTE* task;
    // clang-format off
    __asm {
        push bytes
        mov eax,0x61a5a0
        call eax
        add esp,4
        mov task,eax
    }
    // clang-format on
    m_bSelfCreated = TRUE;
    m_pTaskType = task;
}
// 0x000b21e0
void CTask::Create(BYTE* task)
{
    m_bSelfCreated = FALSE;
    m_pTaskType = task;
}
// 0x000b25b0
CTask* CTask::CreateCopy()
{
    BYTE* task = m_pTaskType;
    // clang-format off
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        call dword ptr [eax+4]
        mov task,eax
        pop ecx
    }
    // clang-format on
    return new CTask(task);
}
// Native self-created tasks use deletion flag 0; attached tasks use flag 1.
// 0x000b2200
void CTask::Destroy()
{
    if (IsDestroyed())
        return;
    BYTE* task = m_pTaskType;
    DWORD release = m_bSelfCreated ? 0 : 1;
    // clang-format off
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        push release
        call dword ptr [eax]
        pop ecx
    }
    // clang-format on
    m_pTaskType = 0;
    m_pPlayerPed = 0;
}
// GTA SA virtual address: 0x00681AF0, task-manager thiscall, three arguments.
// Existing shared player-context pointer at RVA 151960 is reloaded on reset.
// 0x000b2260
void CTask::ApplyToPed(CPed* ped)
{
    m_pPlayerPed = ped;
    BYTE* manager = *(BYTE**)((BYTE*)ped->m_pGamePed + 0x47c);
    BYTE* task = m_pTaskType;
    *g_clientCrashPedContext = ped->m_nPlayerNumber;
    // clang-format off
    __asm {
        push ecx
        mov ecx,manager
        push 0
        push 3
        push task
        add ecx,4
        mov eax,0x681af0
        call eax
        pop ecx
    }
    // clang-format on
    *g_clientCrashPedContext = 0;
}
// 0x000b22c0
DWORD CTask::GetID()
{
    BYTE* task = m_pTaskType;
    DWORD result;
    // clang-format off
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        call dword ptr [eax+0x10]
        mov result,eax
        pop ecx
    }
    // clang-format on
    return result;
}
// GTA SA virtual vtable address: 0x0086D48C, backend base-task sentinel.
// 0x000b22e0
BOOL CTask::IsDestroyed()
{
    return m_pTaskType == 0 || *(DWORD*)m_pTaskType == 0x86d48c;
}
// Native widens AL without normalizing it to a C++ bool.
// 0x000b2300
BOOL CTask::IsSimple()
{
    BYTE* task = m_pTaskType;
    DWORD result;
    // clang-format off
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        call dword ptr [eax+0xc]
        movzx eax,al
        mov result,eax
        pop ecx
    }
    // clang-format on
    return result;
}

// GTA SA virtual address: 0x0067B4E0. Wrapper remains 16 bytes; backend is 112.
// 0x000b2330
CTaskJetpack::CTaskJetpack()
{
    m_pPlayerPed = 0;
    Create(112);
    BYTE* task = m_pTaskType;
    // clang-format off
    __asm {
        push 0
        push 0
        push 0x41200000
        push 0
        mov ecx,task
        mov eax,0x67b4e0
        call eax
    }
    // clang-format on
}
// 0x000b2380
CTaskJetpack::CTaskJetpack(BYTE* task)
{
    m_pPlayerPed = 0;
    Create(task);
}
// Clear the attached ped's jetpack task slot before the explicit Destroy call.
// No backend/manager null guards beyond the native wrapper-ped check.
// 0x000b23b0
CTaskJetpack::~CTaskJetpack()
{
    if (m_pPlayerPed)
    {
        BYTE* manager = *(BYTE**)((BYTE*)m_pPlayerPed->m_pGamePed + 0x47c);
        *(DWORD*)(manager + 0x10) = 0;
    }
    CTask::Destroy();
}
// GTA SA virtual address: 0x006786C0. Backend args: fallType, zero, number.
// 0x000b2410
CTaskTakeDamageFall::CTaskTakeDamageFall(DWORD fallType, DWORD number)
{
    m_pPlayerPed = 0;
    Create(24);
    BYTE* task = m_pTaskType;
    // clang-format off
    __asm {
        push number
        push 0
        push fallType
        mov ecx,task
        mov eax,0x6786c0
        call eax
    }
    // clang-format on
}
// Native thunk delegates to base destruction only; it does not call Destroy.
// 0x000b2630
CTaskTakeDamageFall::~CTaskTakeDamageFall() {}

// GTA SA virtual address: 0x00634EF0; no stack arguments, backend extent12.
// 0x000b2460
CTaskUseGoggles::CTaskUseGoggles()
{
    m_pPlayerPed = 0;
    Create(12);
    BYTE* task = m_pTaskType;
    // clang-format off
    __asm {
        push ecx
        mov ecx,task
        mov eax,0x634ef0
        call eax
        pop ecx
    }
    // clang-format on
}
// Native thunk only resets the base vptr; does not call Destroy.
// 0x000b2660
CTaskUseGoggles::~CTaskUseGoggles() {}
// GTA SA virtual address: 0x00667CD0; backend extent48.
// 0x000b24a0
CTaskGoToPoint::CTaskGoToPoint(DWORD moveState, const CVector* target, float radius, DWORD moveTowards, DWORD finalFlag)
{
    m_pPlayerPed = 0;
    Create(48);
    BYTE* task = m_pTaskType;
    // clang-format off
    __asm {
        push finalFlag
        push moveTowards
        push radius
        push target
        push moveState
        mov ecx,task
        mov eax,0x667cd0
        call eax
    }
    // clang-format on
}
// 0x000b2690
CTaskGoToPoint::~CTaskGoToPoint() {}
// GTA SA virtual address: 0x00621190; backend extent92. Native argument order:
// target, duckingConditions, duckTime, duckChance, competence (full raw words).
// 0x000b24f0
CTaskKillPedOnFootArmed::CTaskKillPedOnFootArmed(DWORD competence, DWORD duckTime, DWORD duckChance, DWORD duckingConditions, void* target)
{
    m_pPlayerPed = 0;
    Create(92);
    BYTE* task = m_pTaskType;
    // clang-format off
    __asm {
        push competence
        push duckChance
        push duckTime
        push duckingConditions
        push target
        mov ecx,task
        mov eax,0x621190
        call eax
    }
    // clang-format on
}
// 0x000b26c0
CTaskKillPedOnFootArmed::~CTaskKillPedOnFootArmed() {}
