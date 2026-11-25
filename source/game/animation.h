#ifndef ANIMATION_H
#define ANIMATION_H

#include <string.h>
#include <windows.h>

// limits come from the script engine string buffers (library 16, name 24)
const size_t kAnimationLibraryMaxLength = 15;
const size_t kAnimationNameMaxLength = 23;
inline bool GameIsAnimationNameValid(const char* library, const char* name)
{
    if (!library || !name)
        return false;
    size_t libraryLength = strlen(library), nameLength = strlen(name);
    return libraryLength >= 1 && libraryLength <= kAnimationLibraryMaxLength && nameLength >= 1 && nameLength <= kAnimationNameMaxLength;
}

extern char g_animationNames[1812][36];

extern DWORD g_animationKeys[1812];

WORD GameFindAnimation(const char* library, const char* name);

char* GameGetAnimationName(short index);
// One-based index; a miss returns 0xffff in AX.

WORD GameFindAnimationByKey(DWORD key);

DWORD __stdcall GameGetAnimationGroupKey(int group, int animation);

DWORD __stdcall GameAnimationKey(const char* name);

void GameInitializeAnimationKeys();

#endif // ANIMATION_H
