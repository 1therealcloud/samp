#ifndef TASK_H
#define TASK_H

#include <stddef.h>
#include <windows.h>

class CPed;
struct CVector;
// samp.dll vtable RVA: 0x0011f434; seven slots, allocated extent 16 bytes.
class CTask
{
public:
    BOOL m_bSelfCreated;
    CPed* m_pPlayerPed;
    BYTE* m_pTaskType;
    // 0x000b2180
    CTask();
    // 0x000b2560
    CTask(DWORD size);
    // 0x000b2590
    CTask(BYTE* task);
    // 0x000b21a0
    virtual ~CTask();
    // 0x000b21b0
    void Create(DWORD size);
    // 0x000b21e0
    void Create(BYTE* task);
    // 0x000b25b0
    virtual CTask* CreateCopy();
    // 0x000b2200
    virtual void Destroy();
    // 0x000b2260
    virtual void ApplyToPed(CPed* ped);
    // 0x000b22c0
    virtual DWORD GetID();
    // 0x000b22e0
    virtual BOOL IsDestroyed();
    // 0x000b2300
    virtual BOOL IsSimple();
};
static_assert(sizeof(CTask) == 16);
static_assert(offsetof(CTask, m_pTaskType) == 12);
static_assert(offsetof(CTask, m_pPlayerPed) == 8);
// samp.dll vtable RVA: 0x0011f450
class CTaskJetpack : public CTask
{
public:
    // 0x000b2330
    CTaskJetpack();
    // 0x000b2380
    CTaskJetpack(BYTE* task);
    // 0x000b23b0
    virtual ~CTaskJetpack();
};
// samp.dll vtable RVA: 0x0011f46c
class CTaskTakeDamageFall : public CTask
{
public:
    // 0x000b2410
    CTaskTakeDamageFall(DWORD fallType, DWORD number);
    // 0x000b2630
    virtual ~CTaskTakeDamageFall();
};
static_assert(sizeof(CTaskJetpack) == 16);
static_assert(sizeof(CTaskTakeDamageFall) == 16);
// samp.dll vtable RVA: 0x0011f488
class CTaskUseGoggles : public CTask
{
public:
    // 0x000b2460
    CTaskUseGoggles();
    // 0x000b2660
    virtual ~CTaskUseGoggles();
};
// samp.dll vtable RVA: 0x0011f4a4
class CTaskGoToPoint : public CTask
{
public:
    // Full native DWORD flag words are forwarded without bool normalization.
    // 0x000b24a0
    CTaskGoToPoint(DWORD moveState, const CVector* target, float radius, DWORD moveTowards, DWORD finalFlag);
    // 0x000b2690
    virtual ~CTaskGoToPoint();
};
// samp.dll vtable RVA: 0x0011f4c0
class CTaskKillPedOnFootArmed : public CTask
{
public:
    // Wrapper argument order differs from the GTA constructor. Target is opaque GTA ped.
    // 0x000b24f0
    CTaskKillPedOnFootArmed(DWORD competence, DWORD duckTime, DWORD duckChance, DWORD duckingConditions, void* target);
    // 0x000b26c0
    virtual ~CTaskKillPedOnFootArmed();
};
static_assert(sizeof(CTaskUseGoggles) == 16);
static_assert(sizeof(CTaskGoToPoint) == 16);
static_assert(sizeof(CTaskKillPedOnFootArmed) == 16);

#endif // TASK_H
