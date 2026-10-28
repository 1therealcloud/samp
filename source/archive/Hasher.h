//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#pragma once

#include "CryptoContext.h"
#include <windows.h>

class CHasher
{
private:
    // samp.dll RVA: 0x00140788
    static DWORD ms_dwHashAlgorithm;

    HCRYPTHASH m_hCryptHash;
    CCryptoContext* m_pContext;

public:
    // samp.dll RVA: 0x000652f0
    CHasher(CCryptoContext* pContext);
    // samp.dll RVA: 0x00065320
    ~CHasher(void);

    // samp.dll RVA: 0x00065330
    void AddData(DWORD dwDataLength, BYTE* pbData);
    // samp.dll RVA: 0x00065350
    HCRYPTHASH GetContainer();
};
