#ifndef SAMP_GAME_TASK_H
#define SAMP_GAME_TASK_H
#include <windows.h>
#include <stddef.h>
class CPed;
#pragma pack(push,1)
// samp.dll vtable RVA: 0x0011f434; seven slots, allocated extent 16 bytes.
class CTask {
public:
    BOOL m_bSelfCreated;
    CPed *m_pPlayerPed;
    BYTE *m_pTaskType;
    // samp.dll RVA: 0x000b2180
    CTask();
    // samp.dll RVA: 0x000b2560
    CTask(DWORD size);
    // samp.dll RVA: 0x000b2590
    CTask(BYTE *task);
    // samp.dll RVA: 0x000b21a0
    virtual ~CTask();
    // samp.dll RVA: 0x000b21b0
    void Create(DWORD size);
    // samp.dll RVA: 0x000b21e0
    void Create(BYTE *task);
    // samp.dll RVA: 0x000b25b0
    virtual CTask *CreateCopy();
    // samp.dll RVA: 0x000b2200
    virtual void Destroy();
    // samp.dll RVA: 0x000b2260
    virtual void ApplyToPed(CPed *ped);
    // samp.dll RVA: 0x000b22c0
    virtual DWORD GetID();
    // samp.dll RVA: 0x000b22e0
    virtual BOOL IsDestroyed();
    // samp.dll RVA: 0x000b2300
    virtual BOOL IsSimple();
};
#pragma pack(pop)
typedef char TaskSize[(sizeof(CTask)==16)?1:-1];
typedef char TaskBackendOffset[(offsetof(CTask,m_pTaskType)==12)?1:-1];
typedef char TaskPedOffset[(offsetof(CTask,m_pPlayerPed)==8)?1:-1];
#endif
