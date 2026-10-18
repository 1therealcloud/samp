//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#include "CryptoContext.h"
#include "CryptoFns.h"

//------------------------------------

// samp.dll RVA: 0x0016c37c
DWORD CCryptoContext::ms_dwRefCount = 0;
// samp.dll RVA: 0x0014077c
DWORD CCryptoContext::ms_dwProviderType = PROV_RSA_FULL;
// samp.dll RVA: 0x0016c380
LPSTR CCryptoContext::ms_szProviderName = NULL;
// samp.dll RVA: 0x00140780
LPSTR CCryptoContext::ms_szContainerName = (LPSTR)"SAMP";
// samp.dll RVA: unknown (native global not established; unused legacy declaration)
HMODULE CCryptoContext::ms_hAdvApi32 = NULL;

//------------------------------------

// samp.dll RVA: 0x00065030
CCryptoContext::CCryptoContext(void)
{
	/*
	if (!ms_hAdvApi32)
	{
		ms_hAdvApi32 = LoadLibrary("advapi32.dll");
	}
	*/

	// Open existing context, if not found, create one!
	if ( !CRYPT(AcquireContext)(&m_hCryptProv, ms_szContainerName, ms_szProviderName, ms_dwProviderType, 0) ) {
		if( !CRYPT(AcquireContext)(&m_hCryptProv, ms_szContainerName, ms_szProviderName, ms_dwProviderType, CRYPT_NEWKEYSET) ) {
			throw(1);
		}
	}

	ms_dwRefCount++;

}

//------------------------------------

// samp.dll RVA: 0x000650a0
CCryptoContext::~CCryptoContext(void)
{

	// Release the context
	CRYPT(ReleaseContext)(m_hCryptProv, 0);
	ms_dwRefCount--;

	if (ms_dwRefCount == 0) {
		/*
		// Free the library
		if (ms_hAdvApi32)
			FreeLibrary(ms_hAdvApi32);
		*/

		// Delete the context
		CRYPT(AcquireContext)(&m_hCryptProv, ms_szContainerName, ms_szProviderName, ms_dwProviderType, CRYPT_DELETEKEYSET);
	}

}

//------------------------------------

// samp.dll RVA: 0x000650e0
HCRYPTPROV CCryptoContext::GetProvider()
{
	return m_hCryptProv;
}

//------------------------------------

#ifdef ARCTOOL
// samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
void CCryptoContext::GenerateRandom(DWORD dwLength, BYTE* pbBuffer)
{
	CryptGenRandom(m_hCryptProv, dwLength, pbBuffer);
}
#endif
