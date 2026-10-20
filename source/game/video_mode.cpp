#include "game/video_mode.h"
// GTA SA virtual address: 0x00745C70; no additional validation is native.
// samp.dll RVA: 0x000b5d10
void GameSetVideoMode(DWORD mode) {
    ((void(__cdecl*)(DWORD))0x745c70)(mode);
}
