#include "game/device_integrity.h"
#include "main.h"
#include <stdlib.h>
// samp.dll RVA: 0x0018dc24
DWORD g_integrityVehicleChecksum;
// samp.dll RVA: 0x0018dc28
DWORD g_integrityPedChecksum;
// samp.dll RVA: 0x0018dc2c
DWORD g_integrityComparisons;
// samp.dll RVA: 0x0018dc30
DWORD g_integrityPending;
// samp.dll RVA: 0x0018dc34
BYTE g_integritySuppressCount;
// samp.dll RVA: 0x0009d690
DWORD* GameIntegrityVehicleArray()
{
    return **(DWORD***)0xb74494;
}
// samp.dll RVA: 0x0009d6b0
DWORD* GameIntegrityPedArray()
{
    return **(DWORD***)0xb74490;
}
// samp.dll RVA: 0x0009d6d0
DWORD GameIntegrityXor(const DWORD* data, DWORD count)
{
    DWORD value = 0;
    for (DWORD i = 0; i != count; ++i)
        value ^= data[i];
    return value;
}
// samp.dll RVA: 0x0009d6f0
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
