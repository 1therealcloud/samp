//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#include "Hasher.h"

#include "CryptoFns.h"

//------------------------------------

// 0x001027a8
DWORD CHasher::ms_dwHashAlgorithm = CALG_SHA1;

//------------------------------------

// 0x00065870
CHasher::CHasher(CCryptoContext* pContext)
{
    // Save context for later
    m_pContext = pContext;

    // Generate a hash container
    HCRYPTPROV hCryptProv = pContext->GetProvider();
    CRYPT(CreateHash)(hCryptProv, ms_dwHashAlgorithm, NULL, NULL, &m_hCryptHash);
}

//------------------------------------

// R5 address to verify
CHasher::~CHasher(void)
{
    // Destory the hash container
    CRYPT(DestroyHash)(m_hCryptHash);
}

//------------------------------------

// 0x000658b0
void CHasher::AddData(DWORD dwDataLength, BYTE* pbData)
{
    // Add the data to be hashed
    CRYPT(HashData)(m_hCryptHash, pbData, dwDataLength, 0);
}

//------------------------------------

// R5 address to verify
HCRYPTKEY CHasher::GetContainer()
{
    return m_hCryptHash;
}

//------------------------------------
