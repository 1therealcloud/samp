//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#pragma once

#include "Hasher.h"
#include "KeyPair.h"
#include <windows.h>

class CSigner
{
private:
    BYTE* m_pbSignature;
    DWORD m_dwLength;

public:
    // 0x00065440
    CSigner(void);
    // 0x00065450
    ~CSigner(void);

#ifdef ARCTOOL
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void SignHash(CHasher* pHasher);
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    BYTE* GetSignature();
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    DWORD GetSignatureLength();
#endif

    // 0x00065460
    void SetSignature(DWORD dwLength, BYTE* pbSignature);
    // 0x000654b0
    BOOL VerifySignature(CHasher* pHasher, CKeyPair* pKeyPair);
};
