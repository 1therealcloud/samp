#include "game/animation.h"

#include "runutil.h"

#include <stdio.h>
#include <string.h>
// 0x001419d0
char g_animationNames[1812][36] = {
#include "game/animation_names.inc"
};
// 0x001871a8
DWORD g_animationKeys[1812];
// 0x0009c870
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

// 0x0009c950
char* GameGetAnimationName(short index)
{
    if (index > 1812 || index < 1)
        return 0;
    return g_animationNames[index - 1];
}

// 0x000b4080
DWORD __stdcall GameAnimationKey(const char* name)
{
    return ((DWORD(__cdecl*)(const char*))0x53cf30)(name);
}

// 0x0009c980
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

// 0x0009ca50
WORD GameFindAnimationByKey(DWORD key)
{
    for (int i = 0; i != 1812; i++)
        if (g_animationKeys[i] == key)
            return (WORD)(i + 1);
    return 0xffff;
}

// 0x000b4030
DWORD __stdcall GameGetAnimationGroupKey(int group, int animation)
{
    // GTA SA virtual address: lookup returns an opaque association descriptor.
    BYTE* entry = ((BYTE * (__cdecl*)(int, int))0x4d3a60)(group, animation);
    return entry ? **(DWORD**)(entry + 0x10) : 0;
}
