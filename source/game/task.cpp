#include "game/task.h"
#include "game/playerped.h"
#include "client/exceptions.h"
// The default constructor deliberately leaves m_bSelfCreated untouched.
// samp.dll RVA: 0x000b2180
CTask::CTask() { m_pTaskType=0; m_pPlayerPed=0; }
// samp.dll RVA: 0x000b2560
CTask::CTask(DWORD size) { m_pTaskType=0; m_pPlayerPed=0; Create(size); }
// samp.dll RVA: 0x000b2590
CTask::CTask(BYTE *task) { m_pTaskType=0; m_pPlayerPed=0; Create(task); }
// Native destruction only resets the vptr; Destroy is a separate operation.
// samp.dll RVA: 0x000b21a0
CTask::~CTask() {}
// GTA SA virtual address: 0x0061A5A0, backend task allocation.
// samp.dll RVA: 0x000b21b0
void CTask::Create(DWORD size) {
    BYTE *task=((BYTE*(__cdecl*)(DWORD))0x61a5a0)(size);
    m_bSelfCreated=TRUE; m_pTaskType=task;
}
// samp.dll RVA: 0x000b21e0
void CTask::Create(BYTE *task) { m_bSelfCreated=FALSE; m_pTaskType=task; }
// samp.dll RVA: 0x000b25b0
CTask *CTask::CreateCopy() {
    BYTE *task=m_pTaskType;
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        call dword ptr [eax+4]
        mov task,eax
        pop ecx
    }
    return new CTask(task);
}
// Native self-created tasks use deletion flag 0; attached tasks use flag 1.
// samp.dll RVA: 0x000b2200
void CTask::Destroy() {
    if(IsDestroyed())return;
    BYTE *task=m_pTaskType;
    DWORD release=m_bSelfCreated?0:1;
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        push release
        call dword ptr [eax]
        pop ecx
    }
    m_pTaskType=0; m_pPlayerPed=0;
}
// GTA SA virtual address: 0x00681AF0, task-manager thiscall, three arguments.
// Existing shared player-context pointer at RVA 151960 is reloaded on reset.
// samp.dll RVA: 0x000b2260
void CTask::ApplyToPed(CPed *ped) {
    m_pPlayerPed=ped;
    BYTE *manager=*(BYTE**)((BYTE*)ped->m_pGamePed+0x47c);
    BYTE *task=m_pTaskType;
    *g_clientCrashPedContext=ped->m_nPlayerNumber;
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
    *g_clientCrashPedContext=0;
}
// samp.dll RVA: 0x000b22c0
DWORD CTask::GetID() {
    BYTE *task=m_pTaskType; DWORD result;
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        call dword ptr [eax+0x10]
        mov result,eax
        pop ecx
    }
    return result;
}
// GTA SA virtual vtable address: 0x0086D48C, backend base-task sentinel.
// samp.dll RVA: 0x000b22e0
BOOL CTask::IsDestroyed() {
    return m_pTaskType==0 || *(DWORD*)m_pTaskType==0x86d48c;
}
// Native widens AL without normalizing it to a C++ bool.
// samp.dll RVA: 0x000b2300
BOOL CTask::IsSimple() {
    BYTE *task=m_pTaskType; DWORD result;
    __asm {
        push ecx
        mov ecx,task
        mov eax,dword ptr [ecx]
        call dword ptr [eax+0xc]
        movzx eax,al
        mov result,eax
        pop ecx
    }
    return result;
}
