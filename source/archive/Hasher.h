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

    static DWORD ms_dwHashAlgorithm;

    HCRYPTHASH m_hCryptHash;

public:

    CHasher(CCryptoContext* pContext);

    ~CHasher(void);

    void AddData(DWORD dwDataLength, BYTE* pbData);

    HCRYPTHASH GetContainer();
};
