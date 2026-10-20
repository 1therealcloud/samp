#include "client/bootstrap.h"
// Standard CRT integration adapter; no established standalone native entry.
// samp.dll RVA: unknown (native entry not established)
extern "C" BOOL WINAPI DllMain(HINSTANCE module,DWORD reason,LPVOID reserved) {
    return ClientDllMainForward(module,reason,reserved);
}
