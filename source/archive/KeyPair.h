//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#pragma once

#include "CryptoContext.h"

#include <windows.h>

class CKeyPair
{
private:
    // legacy
    static DWORD ms_dwRSAKeySize;

    HCRYPTKEY m_hCryptKey;
    CCryptoContext* m_pContext;

public:
    CKeyPair(CCryptoContext* pContext);

    ~CKeyPair(void);

#ifdef ARCTOOL
    void GenerateKey();
    void LoadFromFile(PCHAR szFileName);
    void WriteToFile(PCHAR szFileName);
    void WriteCHeaderFile(PCHAR szFileName);
#endif

    void LoadFromMemory(DWORD dwPubKeySize, BYTE* pbPubKeyBlob, BYTE bytXORKey);

    void ReleaseKey();

    HCRYPTKEY GetContainer();
};
