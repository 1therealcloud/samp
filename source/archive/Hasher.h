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
    // 0x001027a8
    static DWORD ms_dwHashAlgorithm;

    HCRYPTHASH m_hCryptHash;
    CCryptoContext* m_pContext;

public:
    // 0x00065870
    CHasher(CCryptoContext* pContext);
    // R5 address to verify
    ~CHasher(void);

    // 0x000658b0
    void AddData(DWORD dwDataLength, BYTE* pbData);
    // R5 address to verify
    HCRYPTHASH GetContainer();
};
