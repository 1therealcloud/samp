#include "anticheat.h"

#include "game/util.h"
#include "main.h"
#include "runutil.h" // CheckDuplicateD3D9Dlls (legacy)

#include <stdlib.h>
#include <tlhelp32.h> // CheckDuplicateD3D9Dlls (legacy)

// R5 address to verify
DWORD g_integrityVehicleChecksum;
// R5 address to verify
DWORD g_integrityPedChecksum;
// 0x0014fd98
DWORD g_integrityComparisons;
// R5 address to verify
DWORD g_integrityPending;
// R5 address to verify
BYTE g_integritySuppressCount;
// 0x00117478
DWORD g_clientIntegrityFrameMarker = 69;
// R5 address to verify
DWORD g_clientIntegrityFramePrevious;
// R5 address to verify
DWORD g_clientIntegrityFrameFirst;
// R5 address to verify
DWORD g_clientIntegrityFrameCounter;
// R5 address to verify
double g_clientTimerPreviousSeconds;

// R5 address to verify
__int64 g_clientTimerPreviousCounter;
// R5 address to verify
DWORD g_clientTimerFlags;
// R5 address to verify
__int64 g_clientTimerFrequency;
// R5 address to verify
DWORD g_clientTimerUsePerformanceCounter;
// R5 address to verify
DWORD g_clientTimerFrequencyAttempted;
// 0x000e8890
const double g_clientTimerUnsignedBias = 4294967296.0;
// 0x000e88a0
const double g_clientTimerMillisecondScale = 0.001;

// R5 address to verify
unsigned long g_clientD3D9BaseEncoded; // CheckDuplicateD3D9Dlls (legacy)
// R5 address to verify
unsigned long g_clientD3D9ImageSize; // CheckDuplicateD3D9Dlls (legacy)

// 0x0009d850
DWORD* GameIntegrityVehicleArray()
{
    return **(DWORD***)0xb74494;
}

// 0x0009d870
DWORD* GameIntegrityPedArray()
{
    return **(DWORD***)0xb74490;
}

// 0x0009d890
DWORD GameIntegrityXor(const DWORD* data, DWORD count)
{
    DWORD value = 0;
    for (DWORD i = 0; i != count; ++i)
        value ^= data[i];
    return value;
}

// 0x0009d8b0
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

// 0x000c46b0
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
// 0x000c4420
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

// 0x000c4530
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

// R5 address to verify
unsigned long CheckDuplicateD3D9Dlls() // legacy
{
    unsigned long matches = 0;
    char moduleName[MAX_PATH + 1];
    unsigned char d3d9[] = {0x8c, 0x66, 0x8c, 0x27, 0xc5, 0x8c, 0x8d, 0x8d, 0x0};
    char* d3d9Name = K_DecodeString(d3d9);
    unsigned char kernel[] = {0x6d, 0xac, 0x4e, 0xcd, 0xac, 0x8d, 0x66, 0x46, 0xc5, 0x8c, 0x8d, 0x8d, 0x0};
    char* kernelName = K_DecodeString(kernel);
    unsigned char snapshot[] = {0x68, 0x4e, 0xac, 0x2c, 0x8e, 0xac, 0x8a, 0xed, 0xed, 0x8d, 0xd, 0xac, 0x8d, 0xe, 0x66, 0x46, 0x6a, 0xcd, 0x2c, 0xe, 0x6e, 0xd, 0xed, 0x8e, 0x0};
    unsigned char first[] = {0xa9, 0xed, 0x8c, 0xae, 0x8d, 0xac, 0x66, 0x46, 0xc8, 0x2d, 0x4e, 0x6e, 0x8e, 0x0};
    unsigned char next[] = {0xa9, 0xed, 0x8c, 0xae, 0x8d, 0xac, 0x66, 0x46, 0xc9, 0xac, 0xf, 0x8e, 0x0};
    HMODULE kernelModule = GetModuleHandleA(kernelName);
    char* snapshotName = K_DecodeString(snapshot);
    FARPROC(WINAPI * resolve)(HMODULE, LPCSTR) = GetProcAddress;
    typedef HANDLE(WINAPI * SnapshotFunction)(DWORD, DWORD);
    typedef BOOL(WINAPI * ModuleFunction)(HANDLE, LPMODULEENTRY32);
    SnapshotFunction createSnapshot = (SnapshotFunction)resolve(kernelModule, snapshotName);
    ModuleFunction firstModule = (ModuleFunction)resolve(kernelModule, K_DecodeString(first));
    ModuleFunction nextModule = (ModuleFunction)resolve(kernelModule, K_DecodeString(next));
    HANDLE handle = createSnapshot(8, GetCurrentProcessId());
    if (handle == INVALID_HANDLE_VALUE)
        return 0;
    MODULEENTRY32 entry;
    entry.dwSize = sizeof(MODULEENTRY32);
    if (!firstModule(handle, &entry))
    {
        CloseHandle(handle);
        return 0;
    }
    do
    {
        // Native keeps this unbounded intermediate copy before its exact strcmp.
        strcpy(moduleName, entry.szModule);
        if (!strcmp(d3d9Name, moduleName))
        {
            g_clientD3D9BaseEncoded = (unsigned long)entry.modBaseAddr ^ 0xacacacac;
            g_clientD3D9ImageSize = entry.modBaseSize;
            ++matches;
        }
    }
    while (nextModule(handle, &entry));
    CloseHandle(handle);
    return matches;
}
