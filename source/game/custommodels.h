#ifndef SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#define SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#include <windows.h>
// Observed registry prefix only: pointer array at +0, DWORD count at +4.
// Complete allocation, ownership and loader lifecycle remain open. Never allocate.
class CCustomModels {public:
    // Descriptive query names follow observed record type bytes 1 and 2.
    // samp.dll RVA: 0x0000cc80
    bool IsPedEntry(int model);
    // samp.dll RVA: 0x0000ccc0
    bool IsObjectEntry(int model);
    // Returns the raw record byte, not a normalized boolean.
    // samp.dll RVA: 0x0000cd00
    BYTE GetLoadedFlag(int model);
    // Native DoesExist has loading side effects for type-1 entries.
    // samp.dll RVA: 0x0000ce30
    BOOL DoesExist(int model);
    // samp.dll RVA: 0x0000ce90
    BOOL LoadObjectEntry(int model);
    // ECX is this; four DWORD stack arguments, callee pops 16 bytes.
    // samp.dll RVA: 0x0000c1c0
    BOOL LoadPedModel(DWORD base,DWORD model,DWORD dffHash,DWORD txdHash);
    // samp.dll RVA: 0x0000c2e0
    BOOL LoadObjectModel(DWORD base,DWORD model,DWORD dffHash,DWORD txdHash);
};
// samp.dll RVA: 0x002aca28
extern CCustomModels *g_customModels;
#endif
