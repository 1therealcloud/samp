#ifndef SAMP_ANTICHEAT_H
#define SAMP_ANTICHEAT_H
#include <windows.h>
// 0x0018dc24
extern DWORD g_integrityVehicleChecksum;
// 0x0018dc28
extern DWORD g_integrityPedChecksum;
// 0x0018dc2c
extern DWORD g_integrityComparisons;
// 0x0018dc30
extern DWORD g_integrityPending;
// 0x0018dc34
extern BYTE g_integritySuppressCount;
// 0x0009d690
DWORD* GameIntegrityVehicleArray();
// 0x0009d6b0
DWORD* GameIntegrityPedArray();
// 0x0009d6d0
DWORD GameIntegrityXor(const DWORD* data, DWORD count);
// Seven DWORD words are supplied by reviewed callers; native body ignores them.
// 0x000c5c10
DWORD __stdcall GameIntegrityMismatch(DWORD code, DWORD a, DWORD b, DWORD c, DWORD d, DWORD e, DWORD f);
// 0x0009d6f0
void GameDeviceIntegrityCycle();
// Native ST0 result retains its extended precision until the caller stores it.
// 0x000c5b00
double __cdecl ClientGetFrameElapsed();
// 0x000c5d90
DWORD __cdecl ClientIntegrityFrame(BYTE check);
// 0x0015548c
extern DWORD g_clientIntegrityFrameMarker;
// 0x002aca78
extern DWORD g_clientIntegrityFramePrevious;
// 0x002aca7c
extern DWORD g_clientIntegrityFrameFirst;
// 0x002aca80
extern DWORD g_clientIntegrityFrameCounter;
// 0x002aca88
extern double g_clientTimerPreviousSeconds;
// 0x002aca90
extern __int64 g_clientTimerPreviousCounter;
// 0x002aca98
extern DWORD g_clientTimerFlags;
// 0x002acaa0
extern __int64 g_clientTimerFrequency;
// 0x002acaa8
extern DWORD g_clientTimerUsePerformanceCounter;
// 0x002acaac
extern DWORD g_clientTimerFrequencyAttempted;
// 0x0011a920
extern const double g_clientTimerUnsignedBias;
// 0x0011a930
extern const double g_clientTimerMillisecondScale;
#endif
