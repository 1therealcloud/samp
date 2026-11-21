#ifndef ANTICHEAT_H
#define ANTICHEAT_H

#include <windows.h>
// R5 address to verify
extern DWORD g_integrityVehicleChecksum;
// R5 address to verify
extern DWORD g_integrityPedChecksum;
// 0x0014fd98
extern DWORD g_integrityComparisons;
// R5 address to verify
extern DWORD g_integrityPending;
// R5 address to verify
extern BYTE g_integritySuppressCount;
// 0x0009d850
DWORD* GameIntegrityVehicleArray();
// 0x0009d870
DWORD* GameIntegrityPedArray();
// 0x0009d890
DWORD GameIntegrityXor(const DWORD* data, DWORD count);
// Seven DWORD words are supplied by reviewed callers; native body ignores them.
// 0x000c4530
DWORD __stdcall GameIntegrityMismatch(DWORD code, DWORD a, DWORD b, DWORD c, DWORD d, DWORD e, DWORD f);
// 0x0009d8b0
void GameDeviceIntegrityCycle();
// Native ST0 result retains its extended precision until the caller stores it.
// 0x000c4420
double __cdecl ClientGetFrameElapsed();
// 0x000c46b0
DWORD __cdecl ClientIntegrityFrame(BYTE check);
// 0x00117478
extern DWORD g_clientIntegrityFrameMarker;
// R5 address to verify
extern DWORD g_clientIntegrityFramePrevious;
// R5 address to verify
extern DWORD g_clientIntegrityFrameFirst;
// R5 address to verify
extern DWORD g_clientIntegrityFrameCounter;
// R5 address to verify
extern double g_clientTimerPreviousSeconds;
// R5 address to verify
extern __int64 g_clientTimerPreviousCounter;
// R5 address to verify
extern DWORD g_clientTimerFlags;
// R5 address to verify
extern __int64 g_clientTimerFrequency;
// R5 address to verify
extern DWORD g_clientTimerUsePerformanceCounter;
// R5 address to verify
extern DWORD g_clientTimerFrequencyAttempted;
// 0x000e8890
extern const double g_clientTimerUnsignedBias;
// 0x000e88a0
extern const double g_clientTimerMillisecondScale;

#endif // ANTICHEAT_H
