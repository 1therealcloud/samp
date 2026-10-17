#include "game/game.h"
#include "game/initialization.h"
#include "game/preinit.h"
#include "game/keystuff.h"
#include "game/aimstuff.h"
#include "game/player_colors.h"
#include <stdlib.h>
// samp.dll RVA: 0x0018e1cc
char *g_gameTextBuffer;
// samp.dll RVA: 0x000a06d0
void CGame::InitGame() {
    g_gameTextBuffer=(char*)calloc(1,513);
    KeyStuff::Initialize();AimStuff::Initialize();PlayerColors::Initialize();
    if(!GameInitializeVersionPatches()) {
        MessageBoxA(0,"I can't determine your GTA version.\r\nSA-MP only supports GTA:SA v1.0 USA/EU","Version Error",MB_ICONWARNING);
        ExitProcess(1);
    }
}
