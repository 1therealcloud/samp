#ifndef SAMP_DEVICE_INTEGRITY_H
#define SAMP_DEVICE_INTEGRITY_H
#include <windows.h>
// samp.dll RVA: 0x0018dc24
extern DWORD g_integrityVehicleChecksum;
// samp.dll RVA: 0x0018dc28
extern DWORD g_integrityPedChecksum;
// samp.dll RVA: 0x0018dc2c
extern DWORD g_integrityComparisons;
// samp.dll RVA: 0x0018dc30
extern DWORD g_integrityPending;
// samp.dll RVA: 0x0018dc34
extern BYTE g_integritySuppressCount;
// samp.dll RVA: 0x0009d690
DWORD *GameIntegrityVehicleArray();
// samp.dll RVA: 0x0009d6b0
DWORD *GameIntegrityPedArray();
// samp.dll RVA: 0x0009d6d0
DWORD GameIntegrityXor(const DWORD *data,DWORD count);
// Seven DWORD words are supplied by reviewed callers; native body ignores them.
// samp.dll RVA: 0x000c5c10
DWORD __stdcall GameIntegrityMismatch(DWORD code,DWORD a,DWORD b,DWORD c,DWORD d,DWORD e,DWORD f);
// samp.dll RVA: 0x0009d6f0
void GameDeviceIntegrityCycle();
// Native ST0 result retains its extended precision until the caller stores it.
// samp.dll RVA: 0x000c5b00
double __cdecl ClientGetFrameElapsed();
// samp.dll RVA: 0x000c5d90
DWORD __cdecl ClientIntegrityFrame(BYTE check);
// samp.dll RVA: 0x0015548c
extern DWORD g_clientIntegrityFrameMarker;
// samp.dll RVA: 0x002aca78
extern DWORD g_clientIntegrityFramePrevious;
// samp.dll RVA: 0x002aca7c
extern DWORD g_clientIntegrityFrameFirst;
// samp.dll RVA: 0x002aca80
extern DWORD g_clientIntegrityFrameCounter;
// samp.dll RVA: 0x002aca88
extern double g_clientTimerPreviousSeconds;
// samp.dll RVA: 0x002aca90
extern __int64 g_clientTimerPreviousCounter;
// samp.dll RVA: 0x002aca98
extern DWORD g_clientTimerFlags;
// samp.dll RVA: 0x002acaa0
extern __int64 g_clientTimerFrequency;
// samp.dll RVA: 0x002acaa8
extern DWORD g_clientTimerUsePerformanceCounter;
// samp.dll RVA: 0x002acaac
extern DWORD g_clientTimerFrequencyAttempted;
// samp.dll RVA: 0x0011a920
extern const double g_clientTimerUnsignedBias;
// samp.dll RVA: 0x0011a930
extern const double g_clientTimerMillisecondScale;
#endif
