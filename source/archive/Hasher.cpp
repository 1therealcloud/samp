//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#include "Hasher.h"

#include <wincrypt.h>

//------------------------------------

DWORD CHasher::ms_dwHashAlgorithm = CALG_SHA1;

//------------------------------------

CHasher::CHasher(CCryptoContext* pContext)
{
    // Generate a hash container
    HCRYPTPROV hCryptProv = pContext->GetProvider();
    CryptCreateHash(hCryptProv, ms_dwHashAlgorithm, NULL, NULL, &m_hCryptHash);
}

//------------------------------------

CHasher::~CHasher(void)
{
    // Destory the hash container
    CryptDestroyHash(m_hCryptHash);
}

//------------------------------------

void CHasher::AddData(DWORD dwDataLength, BYTE* pbData)
{
    // Add the data to be hashed
    CryptHashData(m_hCryptHash, pbData, dwDataLength, 0);
}

//------------------------------------

HCRYPTKEY CHasher::GetContainer()
{
    return m_hCryptHash;
}

//------------------------------------
