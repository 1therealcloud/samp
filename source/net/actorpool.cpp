#include "net/actorpool.h"
#include "game/actor.h"
// samp.dll RVA: 0x00001610
CActor* CActorPool::Get(WORD id)
{
    return id < 1000 && active[id] ? actors[id] : 0;
}
// samp.dll RVA: 0x00001640
BOOL CActorPool::DoesExist(WORD id)
{
    return id < 1000 ? active[id] : 0;
}
// samp.dll RVA: 0x00001660
void CActorPool::UpdateLargestId()
{
    int largest = 0;
    for (int i = 0; i < 1000; ++i)
        if (active[i])
            largest = i;
    largestId = largest;
}
// samp.dll RVA: 0x000016c0
CActorPool::CActorPool()
{
    for (int i = 0; i < 1000; ++i)
    {
        active[i] = 0;
        actors[i] = 0;
        peds[i] = 0;
    }
    largestId = 0;
}
// samp.dll RVA: 0x000016f0
BOOL CActorPool::Delete(WORD id)
{
    if (id >= 1000 || !active[id] || !actors[id])
        return FALSE;
    CActor* actor = actors[id];
    active[id] = 0;
    delete actor;
    actors[id] = 0;
    peds[id] = 0;
    UpdateLargestId();
    return TRUE;
}
// samp.dll RVA: 0x000018e0
CActorPool::~CActorPool()
{
    for (WORD i = 0; i < 1000; ++i)
        Delete(i);
}
// samp.dll RVA: 0x000018b0
DWORD CActorPool::Find(void* ped)
{
    int limit = largestId;
    DWORD actor = 0;
    if (limit >= 0)
    {
        do
        {
            if (peds[actor] == ped)
                return actor;
        }
        while ((LONG)++actor <= limit);
    }
    // Native OR AX,FFFF preserves the high half of the terminating counter.
    return (actor & 0xffff0000) | 0xffff;
}

#include "chatwindow.h"
#include "game/custommodels.h"
#include "main.h"
// samp.dll RVA: 0x00001900
BOOL CActorPool::Create(const ActorCreateInfo* info)
{
    // Native deliberately performs no ID bounds check.
    if (actors[info->id])
    {
        g_pChat->AddDebugMessage("Warning: actor %u was not deleted", (unsigned)info->id);
        Delete(info->id);
    }
    actors[info->id] = new CActor(info->model, info->position, info->heading);
    if (!actors[info->id])
        return FALSE;
    // Actor backend is distinct from the base CEntity pointer.
    peds[info->id] = actors[info->id]->actorBackend;
    active[info->id] = 1;
    actors[info->id]->SetHealth(info->health);
    actors[info->id]->SetInvulnerable(info->invulnerable ? 1 : 0);
    state2ee4[info->id] = 1;
    state3e84[info->id] = 0;
    UpdateLargestId();
    if (g_customModels && info->customModel > 20000 && (GameIsPedModel(info->customModel) || g_customModels->DoesExist(info->customModel)))
        actors[info->id]->SetModelIndex(info->customModel);
    return TRUE;
}

// samp.dll RVA: 0x00001770
WORD CActorPool::FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord)
{
    struct RayPoint
    {
        CVector point;
        DWORD word;
    };
    RayPoint from = {*origin, rayWord}, to = {*end, rayWord};
    WORD selected = 0xffff;
    float closest = 100000.0f;
    for (int i = 0; i <= largestId; ++i)
    {
        WORD id = (WORD)i;
        if (!active[i] || id >= 1000 || !active[id] || !actors[id])
            continue;
        CActor* actor = actors[id];
        if (!actor->DoesExist())
            continue;
        BYTE nearer;
        // clang-format off
        __asm {
            mov ecx,actor
            call CEntity::GetDistanceToCamera
            fcomp maxCameraDistance
            fnstsw ax
            test ah,5
            setnp nearer
        }
        // clang-format on
        if (!nearer) continue;
        CVector centre;
        actor->GetBoundCentre(&centre);
        if (!GameTestEntityLineSphere(&from.point, &to.point, actor->m_pGameEntity, &centre, 1.5f, 1))
            continue;
        float x = origin->x, y = origin->y, z = origin->z;
        // clang-format off
        __asm {
            push z
            push y
            push x
            mov ecx,actor
            call CEntity::GetDistanceToPoint
            fcom closest
            fnstsw ax
            test ah,5
            jp distance_not_closer
            fstp closest
            mov nearer,1
            jmp distance_done
distance_not_closer:
            fstp st(0)
            mov nearer,0
distance_done:
        }
        // clang-format on
        if (nearer) selected = (WORD)i;
    }
    return selected;
}
