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

    CTask();

    CTask(DWORD size);

    CTask(BYTE* task);

    virtual ~CTask();

    void Create(DWORD size);

    void Create(BYTE* task);

    virtual CTask* CreateCopy();

    virtual void Destroy();

    virtual void ApplyToPed(CPed* ped);

    virtual DWORD GetID();

    virtual BOOL IsDestroyed();

    virtual BOOL IsSimple();
};
static_assert(sizeof(CTask) == 16);
static_assert(offsetof(CTask, m_pTaskType) == 12);
static_assert(offsetof(CTask, m_pPlayerPed) == 8);
// samp.dll vtable RVA: 0x0011f450
class CTaskJetpack : public CTask
{
public:

    CTaskJetpack();

    CTaskJetpack(BYTE* task);

    virtual ~CTaskJetpack();
};
// samp.dll vtable RVA: 0x0011f46c
class CTaskTakeDamageFall : public CTask
{
public:

    CTaskTakeDamageFall(DWORD fallType, DWORD number);

    virtual ~CTaskTakeDamageFall();
};
static_assert(sizeof(CTaskJetpack) == 16);
static_assert(sizeof(CTaskTakeDamageFall) == 16);
// samp.dll vtable RVA: 0x0011f488
class CTaskUseGoggles : public CTask
{
public:

    CTaskUseGoggles();

    virtual ~CTaskUseGoggles();
};
// samp.dll vtable RVA: 0x0011f4a4
class CTaskGoToPoint : public CTask
{
public:
    // Full native DWORD flag words are forwarded without bool normalization.

    CTaskGoToPoint(DWORD moveState, const CVector* target, float radius, DWORD moveTowards, DWORD finalFlag);

    virtual ~CTaskGoToPoint();
};
// samp.dll vtable RVA: 0x0011f4c0
class CTaskKillPedOnFootArmed : public CTask
{
public:
    // Wrapper argument order differs from the GTA constructor. Target is opaque GTA ped.

    CTaskKillPedOnFootArmed(DWORD competence, DWORD duckTime, DWORD duckChance, DWORD duckingConditions, void* target);

    virtual ~CTaskKillPedOnFootArmed();
};
static_assert(sizeof(CTaskUseGoggles) == 16);
static_assert(sizeof(CTaskGoToPoint) == 16);
static_assert(sizeof(CTaskKillPedOnFootArmed) == 16);

#endif // TASK_H
