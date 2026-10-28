//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#include "Hasher.h"
#include "CryptoFns.h"

//------------------------------------

// samp.dll RVA: 0x00140788
DWORD CHasher::ms_dwHashAlgorithm = CALG_SHA1;

//------------------------------------

// samp.dll RVA: 0x000652f0
CHasher::CHasher(CCryptoContext* pContext)
{
    // Save context for later
    m_pContext = pContext;

    // Generate a hash container
    HCRYPTPROV hCryptProv = pContext->GetProvider();
    CRYPT(CreateHash)(hCryptProv, ms_dwHashAlgorithm, NULL, NULL, &m_hCryptHash);
}

//------------------------------------

// samp.dll RVA: 0x00065320
CHasher::~CHasher(void)
{
    // Destory the hash container
    CRYPT(DestroyHash)(m_hCryptHash);
}

//------------------------------------

// samp.dll RVA: 0x00065330
void CHasher::AddData(DWORD dwDataLength, BYTE* pbData)
{
    // Add the data to be hashed
    CRYPT(HashData)(m_hCryptHash, pbData, dwDataLength, 0);
}

//------------------------------------

// samp.dll RVA: 0x00065350
HCRYPTKEY CHasher::GetContainer()
{
    return m_hCryptHash;
}

//------------------------------------
