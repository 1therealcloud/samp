#include "game/animation.h"

#include "runutil.h"

#include <stdio.h>
#include <string.h>

char g_animationNames[1812][36] = {
#include "game/animation_names.inc"
};

DWORD g_animationKeys[1812];

WORD GameFindAnimation(const char* library, const char* name)
{
    if (!library || !name)
        return 0xffff;
    if (strlen(library) + strlen(name) > 36)
        return 0xffff;
    char combined[40];
    snprintf(combined, sizeof(combined), "%s:%s", library, name);
    Util_strupr(combined);
    for (int i = 0; i < 1812; i++)
        if (!strcmp(combined, g_animationNames[i]))
            return (WORD)(i + 1);
    return 0xffff;
}

char* GameGetAnimationName(short index)
{
    if (index > 1812 || index < 1)
        return 0;
    return g_animationNames[index - 1];
}

DWORD __stdcall GameAnimationKey(const char* name)
{
    return ((DWORD(__cdecl*)(const char*))0x53cf30)(name);
}

void GameInitializeAnimationKeys()
{
    for (int i = 0; i < 1812; i++)
    {
        char name[32] = {0};
        char* separator = strchr(g_animationNames[i], ':');
        if (separator)
        {
            strncpy_s(name, separator + 1, _TRUNCATE);
            g_animationKeys[i] = GameAnimationKey(name);
        }
    }
}

WORD GameFindAnimationByKey(DWORD key)
{
    for (int i = 0; i != 1812; i++)
        if (g_animationKeys[i] == key)
            return (WORD)(i + 1);
    return 0xffff;
}

DWORD __stdcall GameGetAnimationGroupKey(int group, int animation)
{
    // GTA SA virtual address: lookup returns an opaque association descriptor.
    BYTE* entry = ((BYTE * (__cdecl*)(int, int))0x4d3a60)(group, animation);
    return entry ? **(DWORD**)(entry + 0x10) : 0;
}
