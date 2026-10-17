#include "game/scripting.h"
#include <string.h>
// samp.dll RVA: 0x002a93f0
CRunningScript *g_scriptThread;
// samp.dll RVA: 0x000b2150
void Scripting::Initialize() {
    g_scriptThread=(CRunningScript*)operator new(224);
    memset(g_scriptThread,0,224);
}
