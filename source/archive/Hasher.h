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
    // 0x00140790
    static DWORD ms_dwHashAlgorithm;

    HCRYPTHASH m_hCryptHash;
    CCryptoContext* m_pContext;

public:
    // 0x000652f0
    CHasher(CCryptoContext* pContext);
    // 0x00065320
    ~CHasher(void);

    // 0x00065330
    void AddData(DWORD dwDataLength, BYTE* pbData);
    // 0x00065350
    HCRYPTHASH GetContainer();
};
