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
    // 0x0012e364
    static DWORD ms_dwRefCount;
    // 0x00102794
    static DWORD ms_dwProviderType;
    // 0x0012e368
    static LPSTR ms_szProviderName;
    // 0x00102798
    static LPSTR ms_szContainerName;

    // legacy
    static HMODULE ms_hAdvApi32;

    HCRYPTPROV m_hCryptProv;

public:
    // 0x000655b0
    CCryptoContext(void);
    // 0x00065620
    ~CCryptoContext(void);

    // 0x00065660
    HCRYPTPROV GetProvider();

#ifdef ARCTOOL
    void GenerateRandom(DWORD dwLength, BYTE* pbBuffer);
#endif
};
