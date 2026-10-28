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
    // samp.dll RVA: unknown (native global not established; unused legacy declaration)
    static DWORD ms_dwRSAKeySize;

    HCRYPTKEY m_hCryptKey;
    CCryptoContext* m_pContext;

public:
    // 0x00065360
    CKeyPair(CCryptoContext* pContext);
    // 0x00065430
    ~CKeyPair(void);

#ifdef ARCTOOL
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void GenerateKey();
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void LoadFromFile(PCHAR szFileName);
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void WriteToFile(PCHAR szFileName);
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void WriteCHeaderFile(PCHAR szFileName);
#endif

    // 0x000653a0
    void LoadFromMemory(DWORD dwPubKeySize, BYTE* pbPubKeyBlob, BYTE bytXORKey);
    // 0x00065380
    void ReleaseKey();

    // 0x00065420
    HCRYPTKEY GetContainer();
};
