#include "main.h"
// Standard CRT integration adapter; no established standalone native entry.
// samp.dll RVA: unknown (standard VC7 CRT integration adapter; no established standalone native entry)
extern "C" BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
    return ClientDllMainForward(module, reason, reserved);
}
