#include "anticheat.h"
#include "game/util.h"
#include "main.h"
#include <stdlib.h>
// 0x0018dc24
DWORD g_integrityVehicleChecksum;
// 0x0018dc28
DWORD g_integrityPedChecksum;
// 0x0018dc2c
DWORD g_integrityComparisons;
// 0x0018dc30
DWORD g_integrityPending;
// 0x0018dc34
BYTE g_integritySuppressCount;
// 0x0009d690
DWORD* GameIntegrityVehicleArray()
{
    return **(DWORD***)0xb74494;
}
// 0x0009d6b0
DWORD* GameIntegrityPedArray()
{
    return **(DWORD***)0xb74490;
}
// 0x0009d6d0
DWORD GameIntegrityXor(const DWORD* data, DWORD count)
{
    DWORD value = 0;
    for (DWORD i = 0; i != count; ++i)
        value ^= data[i];
    return value;
}
// 0x0009d6f0
void GameDeviceIntegrityCycle()
{
    DWORD vehicles = 0, peds = 0;
    DWORD* data;
    if (g_integrityPending)
    {
        data = GameIntegrityVehicleArray();
        if (data)
            vehicles = GameIntegrityXor(data, 2000);
        data = GameIntegrityPedArray();
        if (data)
            peds = GameIntegrityXor(data, 500);
        if (g_integrityVehicleChecksum)
        {
            if (g_integrityVehicleChecksum != vehicles)
            {
                ++g_clientIntegrityFailures;
                if (GameIntegrityMismatch(12, 1, 6, 12, 17, 2, 50) != 69)
                    exit(0);
            }
            if (!g_integritySuppressCount)
                ++g_integrityComparisons;
        }
        if (g_integrityPedChecksum)
        {
            if (g_integrityPedChecksum != peds)
            {
                ++g_clientIntegrityFailures;
                if (GameIntegrityMismatch(11, 1, 6, 12, 17, 2, 50) != 69)
                    exit(0);
            }
            if (!g_integritySuppressCount)
                ++g_integrityComparisons;
        }
        g_integrityPending = 0;
    }
    else
    {
        data = GameIntegrityVehicleArray();
        g_integrityVehicleChecksum = data ? GameIntegrityXor(data, 2000) : 0;
        data = GameIntegrityPedArray();
        g_integrityPedChecksum = data ? GameIntegrityXor(data, 500) : 0;
        g_integrityPending = 1;
    }
}

// 0x0015548c
DWORD g_clientIntegrityFrameMarker = 69;

// 0x002aca78
DWORD g_clientIntegrityFramePrevious;

// 0x002aca7c
DWORD g_clientIntegrityFrameFirst;

// 0x002aca80
DWORD g_clientIntegrityFrameCounter;

// 0x002aca88
double g_clientTimerPreviousSeconds;

// 0x002aca90
__int64 g_clientTimerPreviousCounter;

// 0x002aca98
DWORD g_clientTimerFlags;

// 0x002acaa0
__int64 g_clientTimerFrequency;

// 0x002acaa8
DWORD g_clientTimerUsePerformanceCounter;

// 0x002acaac
DWORD g_clientTimerFrequencyAttempted;

// 0x0011a920
const double g_clientTimerUnsignedBias = 4294967296.0;

// 0x0011a930
const double g_clientTimerMillisecondScale = 0.001;

// 0x000c5d90
DWORD __cdecl ClientIntegrityFrame(BYTE check)
{
    if (g_clientIntegrityFrameMarker == 69)
    {
        g_clientIntegrityFrameFirst = g_integrityComparisons;
        g_clientIntegrityFramePrevious = g_integrityComparisons;
        g_clientIntegrityFrameMarker = 70;
        return ++g_clientIntegrityFrameCounter;
    }
    if (check)
    {
        if (g_integrityComparisons <= g_clientIntegrityFrameFirst && GameIntegrityMismatch(0x3b, 1, 6, 12, 17, 2, 50) != 69)
            exit(0);
        if (g_clientIntegrityFramePrevious >= g_integrityComparisons && GameIntegrityMismatch(0x3c, 1, 6, 12, 17, 2, 50) != 69)
            exit(0);
        g_clientIntegrityFramePrevious = g_integrityComparisons;
    }
    if (g_clientIntegrityFailures > 5 && GameIntegrityMismatch(0x41, 1, 6, 12, 17, 2, 50) != 69)
        exit(0);
    return ++g_clientIntegrityFrameCounter;
}

// Native clock initialization, QPC delta and multimedia fallback; preserve ST0.
// 0x000c5b00
__declspec(naked) double __cdecl ClientGetFrameElapsed()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [g_clientTimerFrequencyAttempted]
        sub esp, 8
        test eax, eax
        jne timer_c5b42
        lea eax, [esp]
        push eax
        mov dword ptr [g_clientTimerFrequencyAttempted], 1
        call dword ptr [QueryPerformanceFrequency]
        test eax, eax
        mov dword ptr [g_clientTimerUsePerformanceCounter], eax
        je timer_c5baf
        mov ecx, dword ptr [esp]
        mov edx, dword ptr [esp + 4]
        mov dword ptr [g_clientTimerFrequency], ecx
        mov dword ptr [g_clientTimerFrequency+4], edx
        jmp timer_c5b47
    timer_c5b42:
        mov eax, dword ptr [g_clientTimerUsePerformanceCounter]
    timer_c5b47:
        test eax, eax
        je timer_c5baf
        push esi
        push edi
        lea eax, [esp + 8]
        push eax
        call dword ptr [QueryPerformanceCounter]
        mov al, byte ptr [g_clientTimerFlags]
        mov ecx, dword ptr [esp + 8]
        test al, 1
        mov eax, dword ptr [esp + 0ch]
        jne timer_c5b76
        or dword ptr [g_clientTimerFlags], 1
        mov esi, ecx
        mov edx, eax
        jmp timer_c5b82
    timer_c5b76:
        mov edx, dword ptr [g_clientTimerPreviousCounter+4]
        mov esi, dword ptr [g_clientTimerPreviousCounter]
    timer_c5b82:
        mov edi, ecx
        sub edi, esi
        mov esi, eax
        sbb esi, edx
        mov dword ptr [esp + 8], edi
        mov dword ptr [esp + 0ch], esi
        fild qword ptr [esp + 8]
        fild qword ptr [g_clientTimerFrequency]
        pop edi
        mov dword ptr [g_clientTimerPreviousCounter], ecx
        mov dword ptr [g_clientTimerPreviousCounter+4], eax
        fdivp st(1),st(0)
        pop esi
        add esp, 8
        ret
    timer_c5baf:
        call dword ptr [timeGetTime]
        test eax, eax
        mov dword ptr [esp], eax
        fild dword ptr [esp]
        jge timer_c5bc5
        fadd qword ptr [g_clientTimerUnsignedBias]
    timer_c5bc5:
        mov cl, byte ptr [g_clientTimerFlags]
        fmul qword ptr [g_clientTimerMillisecondScale]
        mov eax, 2
        test al, cl
        jne timer_c5bfa
        mov ecx, dword ptr [g_clientTimerFlags]
        fld st(0)
        fld st(1)
        or ecx, eax
        fsubrp st(1),st(0)
        mov dword ptr [g_clientTimerFlags], ecx
        fxch st(1)
        fstp qword ptr [g_clientTimerPreviousSeconds]
        add esp, 8
        ret
    timer_c5bfa:
        fld qword ptr [g_clientTimerPreviousSeconds]
        fld st(1)
        fsubrp st(1),st(0)
        fxch st(1)
        fstp qword ptr [g_clientTimerPreviousSeconds]
        add esp, 8
        ret
    }
    // clang-format on
}

// 0x000c5c10
DWORD __stdcall GameIntegrityMismatch(DWORD code, DWORD a, DWORD b, DWORD c, DWORD d, DWORD e, DWORD f)
{
    if (g_pNetGame && g_pGame)
    {
        UnFuck((void*)0x541df5, 5);
        *(DWORD*)0x541df5 = 0x90909090;
        *(BYTE*)0x541df9 = 0x90;
        GameDisableMouseCalls();
        GameClearMouseDelta();
        GameUpdateMouse();
        UnFuck((void*)0x6194a0, 1);
        *(BYTE*)0x6194a0 = 0xc3;
        ++g_clientIntegrityFailures;
    }
    return 69;
}
