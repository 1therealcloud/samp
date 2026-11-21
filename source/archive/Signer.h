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
    // 0x000659c0
    CSigner(void);
    // R5 address to verify
    ~CSigner(void);

#ifdef ARCTOOL
    void SignHash(CHasher* pHasher);
    BYTE* GetSignature();
    DWORD GetSignatureLength();
#endif

    // 0x000659e0
    void SetSignature(DWORD dwLength, BYTE* pbSignature);
    // 0x00065a30
    BOOL VerifySignature(CHasher* pHasher, CKeyPair* pKeyPair);
};
