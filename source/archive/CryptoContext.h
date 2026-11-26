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
    static DWORD ms_dwRefCount;

    static DWORD ms_dwProviderType;

    static LPSTR ms_szProviderName;

    static LPSTR ms_szContainerName;

    HCRYPTPROV m_hCryptProv;

public:
    CCryptoContext(void);

    ~CCryptoContext(void);

    HCRYPTPROV GetProvider();

#ifdef ARCTOOL
    void GenerateRandom(DWORD dwLength, BYTE* pbBuffer);
#endif
};
