#include "game/game.h"
#include "game/patches.h"
#include "game/hooks.h"
#include "game/scripting.h"
#include "game/animation.h"
// samp.dll RVA: 0x000a0720
void CGame::StartGame() {
    GameApplyInGamePatches();GameInstallHooks();Scripting::Initialize();GameInitializeAnimationKeys();
    *(DWORD*)0xc8d4c0=8;*(BYTE*)0xba6831=1;*(BYTE*)0xba67a4=0;*(BYTE*)0xba677b=0;
}
