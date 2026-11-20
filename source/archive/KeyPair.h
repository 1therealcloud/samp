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
    // 0x00065360
    CKeyPair(CCryptoContext* pContext);
    // 0x00065430
    ~CKeyPair(void);

#ifdef ARCTOOL
    void GenerateKey();
    void LoadFromFile(PCHAR szFileName);
    void WriteToFile(PCHAR szFileName);
    void WriteCHeaderFile(PCHAR szFileName);
#endif

    // 0x000653a0
    void LoadFromMemory(DWORD dwPubKeySize, BYTE* pbPubKeyBlob, BYTE bytXORKey);
    // 0x00065380
    void ReleaseKey();

    // 0x00065420
    HCRYPTKEY GetContainer();
};
