//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#pragma once

#include <windows.h>

class CCryptoContext
{
private:
    // 0x0016c37c
    static DWORD ms_dwRefCount;
    // 0x0014077c
    static DWORD ms_dwProviderType;
    // 0x0016c380
    static LPSTR ms_szProviderName;
    // 0x00140780
    static LPSTR ms_szContainerName;

    // samp.dll RVA: unknown (native global not established; unused legacy declaration)
    static HMODULE ms_hAdvApi32;

    HCRYPTPROV m_hCryptProv;

public:
    // 0x00065030
    CCryptoContext(void);
    // 0x000650a0
    ~CCryptoContext(void);

    // 0x000650e0
    HCRYPTPROV GetProvider();

#ifdef ARCTOOL
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void GenerateRandom(DWORD dwLength, BYTE* pbBuffer);
#endif
};
