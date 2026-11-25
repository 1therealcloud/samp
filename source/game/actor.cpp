#include "game/actor.h"

#include "game/animation.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/scripting.h"
#include "game/util.h"
#include "main.h"

#include <string.h>

const Scripting::OpcodeInfo g_actorKillCommand = {0x321, "i"};

const Scripting::OpcodeInfo g_actorImmunityCommand = {0x2ab, "iiiiii"};

const Scripting::OpcodeInfo g_actorAnimationCommand = {0x812, "issfiiiii"};

void* g_actorTaskBackend;

const Scripting::OpcodeInfo g_actorCreateCommand = {0x9a, "iifffv"};

const Scripting::OpcodeInfo g_actorHeadingCommand = {0x173, "if"};

const Scripting::OpcodeInfo g_actorConfigurationCommand = {0x446, "ii"};

const Scripting::OpcodeInfo g_actorTaskFlagsCommand = {0x60b, "ii"};

float CActor::GetHealth()
{
    if (!actorBackend)
        return 0.0f;
    return actorBackend->health;
}

void CActor::SetHealth(float health)
{
    if (!actorBackend)
        return;
    memcpy(&actorBackend->health, &health, 4);
    if (actorBackend->health <= 0.0f)
        Scripting::ProcessCommand(&g_actorKillCommand, m_nHandle);
}

float CActor::GetArmour()
{
    if (!actorBackend)
        return 0.0f;
    return actorBackend->armour;
}

void CActor::SetArmour(float armour)
{
    if (actorBackend)
        memcpy(&actorBackend->armour, &armour, 4);
}

DWORD CActor::GetBackendFlags()
{
    if (!actorBackend)
        return 0;
    return actorBackend->flags;
}

void CActor::SetBackendFlags(DWORD flags)
{
    if (actorBackend)
        actorBackend->flags = flags;
}

BOOL CActor::IsDead()
{
    return !actorBackend || !(actorBackend->health > 0.0f);
}

BYTE CActor::GetPedState()
{
    return *(BYTE*)&actorBackend->state530;
}

void CActor::SetPedState(BYTE value)
{
    if (actorBackend)
        actorBackend->state530 = value;
}

DWORD CActor::IsInVehicle()
{
    if (!actorBackend)
        return 0;
    return (actorBackend->flags >> 8) & 1;
}

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

void CActor::Destroy()
{
    void* saved = actorBackend;
    // GTA SA virtual address: excluded backend vtable.
    if (saved && GamePoolPedGetAt(m_nHandle) && *(DWORD*)actorBackend != 0x863c40)
    {
        // clang-format off
        __asm {mov ecx,saved}
        __asm {mov ebx,[ecx]}
        __asm {push 1}
        __asm {call dword ptr [ebx]} // clang-format on
        actorBackend = 0;
        m_pGameEntity = 0;
        return;
    }
    m_nHandle = 0;
    actorBackend = 0;
    m_pGameEntity = 0;
}

void CActor::SetRotation(float heading)
{
    if (!actorBackend || !GamePoolPedGetAt(m_nHandle))
        return;
    float radians = GameHeadingToRadians(heading);
    actorBackend->heading = radians;
}

CActor::~CActor()
{
    Destroy();
}

void CActor::PerformAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time)
{
    int attempts = 0;
    if (!actorBackend || !GamePoolPedGetAt(m_nHandle))
        return;
    if (!GameIsAnimationNameValid(library, name))
        return;
    if (_stricmp(library, g_pedBlockedAnimationLibrary) == 0)
        return;
    if (!g_pGame->IsAnimationLoaded(library))
    {
        g_pGame->RequestAnimation(library);
        if (!g_pGame->IsAnimationLoaded(library))
        {
            do
            {
                ::Sleep(1);
                if (++attempts == 15)
                    return;
            }
            while (!g_pGame->IsAnimationLoaded(library));
        }
    }
    Scripting::ProcessCommand(&g_actorAnimationCommand, m_nHandle, name, library, (double)speed, loop, lockX, lockY, freeze, time);
}

DWORD GameClearActorTasks()
{
    // GTA SA virtual address: task clearing backend, thiscall with argument1.
    // clang-format off
    __asm {
        mov edx,g_actorTaskBackend
        mov ecx,[edx+0x47c]
        push 1
        mov eax,0x601640
        call eax
    }
    // clang-format on
    return 1;
}

void CActor::ClearTasks()
{
    g_actorTaskBackend = actorBackend;
    if (g_actorTaskBackend)
        GameClearActorTasks();
}

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
            do
            {
                ::Sleep(1);
            }
            while (!g_pGame->IsModelAvailable(model));
        }
    }
    Scripting::ProcessCommand(&g_actorCreateCommand, 5, model, (double)x, (double)y, (double)(z - 1.0f), &handle);
    Scripting::ProcessCommand(&g_actorHeadingCommand, handle, (double)heading);
    m_nHandle = handle;
    actorBackend = (ActorBackendPrefix*)GamePoolPedGetAt(handle);
    m_pGameEntity = actorBackend;
    Scripting::ProcessCommand(&g_actorConfigurationCommand, m_nHandle, 0);
    Scripting::ProcessCommand(&g_actorTaskFlagsCommand, m_nHandle, 0x10006);
}
