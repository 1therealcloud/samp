#include "game/actor.h"
#include "game/scripting.h"
#include <string.h>
// 0x0011e2d0
const Scripting::OpcodeInfo g_actorKillCommand = {0x321, "i"};
// 0x0011e294
const Scripting::OpcodeInfo g_actorImmunityCommand = {0x2ab, "iiiiii"};
// 0x0009c030
float CActor::GetHealth()
{
    return actorBackend ? actorBackend->health : 0.0f;
}
// 0x0009c050
void CActor::SetHealth(float health)
{
    if (!actorBackend)
        return;
    memcpy(&actorBackend->health, &health, 4);
    if (actorBackend->health <= 0.0f)
        Scripting::ProcessCommand(&g_actorKillCommand, m_nHandle);
}
// 0x0009c090
float CActor::GetArmour()
{
    return actorBackend ? actorBackend->armour : 0.0f;
}
// 0x0009c0b0
void CActor::SetArmour(float armour)
{
    if (actorBackend)
        memcpy(&actorBackend->armour, &armour, 4);
}
// 0x0009c0d0
DWORD CActor::GetBackendFlags()
{
    return actorBackend ? actorBackend->flags : 0;
}
// 0x0009c0e0
void CActor::SetBackendFlags(DWORD flags)
{
    if (actorBackend)
        actorBackend->flags = flags;
}
// 0x0009c100
BOOL CActor::IsDead()
{
    return !actorBackend || !(actorBackend->health > 0.0f);
}
// 0x0009c130
BYTE CActor::GetBackendByte530()
{
    return *(BYTE*)&actorBackend->state530;
}
// 0x0009c140
void CActor::SetBackendByte530(BYTE value)
{
    if (actorBackend)
        actorBackend->state530 = value;
}
// 0x0009c160
DWORD CActor::HasBackendFlag100()
{
    return actorBackend ? (actorBackend->flags >> 8) & 1 : 0;
}
// 0x0009c180
void CActor::SetInvulnerable(BYTE enabled)
{
    if (!actorBackend)
        return;
    invulnerable = enabled;
    if (enabled)
        Scripting::ProcessCommand(&g_actorImmunityCommand, m_nHandle, 1, 1, 1, 1, 1);
    else
        Scripting::ProcessCommand(&g_actorImmunityCommand, m_nHandle, 0, 0, 0, 1, 0);
}

#include "game/playerped.h"
#include "game/util.h"
// 0x0009be80
void CActor::Destroy()
{
    void* saved = actorBackend;
    // GTA SA virtual address: excluded backend vtable.
    if (saved && GamePoolPedGetAt(m_nHandle) && *(DWORD*)actorBackend != 0x863c40)
    {
        DWORD* table = *(DWORD**)saved;
        // clang-format off
        __asm {mov ecx,saved}
        __asm {mov eax,table}
        __asm {push 1}
        __asm {call dword ptr [eax]} // clang-format on
        actorBackend = 0;
        m_pGameEntity = 0;
        return;
    }
    m_nHandle = 0;
    actorBackend = 0;
    m_pGameEntity = 0;
}
// 0x0009bff0
void CActor::SetRotation(float heading)
{
    if (!actorBackend || !GamePoolPedGetAt(m_nHandle))
        return;
    // clang-format off
    // Preserve helper's extended ST0 until the native final store; reload backend.
    __asm {push heading}
    __asm {call GameHeadingToRadians}
    __asm {add esp,4}
    __asm {mov ecx,this}
    __asm {mov edx,dword ptr [ecx+0x48]}
    __asm {fstp dword ptr [edx+0x55c]}
    // clang-format on
}
// 0x0009c1d0
CActor::~CActor()
{
    Destroy();
}

#include "game/game.h"
// 0x0011e2e4
const Scripting::OpcodeInfo g_actorAnimationCommand = {0x812, "issfiiiii"};
// 0x0009bee0
void CActor::PerformAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time)
{
    if (!actorBackend || !GamePoolPedGetAt(m_nHandle))
        return;
    if (_stricmp(library, g_pedBlockedAnimationLibrary) == 0)
        return;
    if (!g_pGame->IsAnimationLoaded(library))
    {
        g_pGame->RequestAnimation(library);
        if (!g_pGame->IsAnimationLoaded(library))
        {
            void(WINAPI* volatile sleep)(DWORD) = ::Sleep;
            int attempts = 0;
            do
            {
                sleep(1);
                if (++attempts == 15)
                    return;
            }
            while (!g_pGame->IsAnimationLoaded(library));
        }
    }
    Scripting::ProcessCommand(&g_actorAnimationCommand, m_nHandle, name, library, (double)speed, loop, lockX, lockY, freeze, time);
}
// 0x00181e5c
void* g_actorTaskBackend;
// 0x0009bfb0
DWORD GameClearActorTasks()
{
    void* tasks = ((ActorBackendPrefix*)g_actorTaskBackend)->tasks;
    // GTA SA virtual address: task clearing backend, thiscall with argument1.
    ((void(__fastcall*)(void*, void*, int))0x601640)(tasks, 0, 1);
    return 1;
}
// 0x0009bfd0
void CActor::ClearTasks()
{
    g_actorTaskBackend = actorBackend;
    if (actorBackend)
        GameClearActorTasks();
}

#include "game/quaternion.h"
// 0x0011e280
const Scripting::OpcodeInfo g_actorCreateCommand = {0x9a, "iifffv"};
// 0x0011e2bc
const Scripting::OpcodeInfo g_actorHeadingCommand = {0x173, "if"};
// 0x0011e2a8
const Scripting::OpcodeInfo g_actorConfigurationCommand = {0x446, "ii"};
// 0x0011e2f8
const Scripting::OpcodeInfo g_actorTaskFlagsCommand = {0x60b, "ii"};
// 0x0009bd30
CActor::CActor(int model, float x, float y, float z, float heading)
{
    actorBackend = 0;
    m_nHandle = 0;
    invulnerable = 0;
    DWORD handle = 0;
    if (!g_pGame->IsModelAvailable(model))
    {
        g_pGame->RequestModel(model, 2);
        g_pGame->LoadRequestedModels();
        if (!g_pGame->IsModelAvailable(model))
        {
            void(WINAPI* volatile sleep)(DWORD) = ::Sleep;
            do
            {
                sleep(1);
            }
            while (!g_pGame->IsModelAvailable(model));
        }
    }
    double loweredZ;
    float* positionZ = &z;
    // clang-format off
    __asm {mov eax,positionZ}
    __asm {fld dword ptr [eax]}
    __asm {fsub g_quaternionMatrixOne}
    __asm {fstp qword ptr loweredZ}
    // clang-format on
    Scripting::ProcessCommand(&g_actorCreateCommand, 5, model, (double)x, (double)y, loweredZ, &handle);
    Scripting::ProcessCommand(&g_actorHeadingCommand, handle, (double)heading);
    m_nHandle = handle;
    actorBackend = (ActorBackendPrefix*)GamePoolPedGetAt(handle);
    m_pGameEntity = actorBackend;
    Scripting::ProcessCommand(&g_actorConfigurationCommand, m_nHandle, 0);
    Scripting::ProcessCommand(&g_actorTaskFlagsCommand, m_nHandle, 0x10006);
}
