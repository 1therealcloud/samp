//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

// This is intended to be a wrapper for Obfuscating calls
// to CryptoAPI. Make sure that LoadLibrary("ADVAPI32.DLL") is
// somewhere (i.e. access some random reg key), otherwise this
// will fail.. miserably.

#pragma once

#include <windows.h>

#ifndef ARCTOOL

/*
const CHAR* CRYPT_MODULE				= "advapi32";
const CHAR* CRYPT_FN_AcquireContext		= "CryptAcquireContextA";
const CHAR* CRYPT_FN_ReleaseContext		= "CryptReleaseContext";
const CHAR* CRYPT_FN_GenRandom			= "CryptGenRandom";
const CHAR* CRYPT_FN_CreateHash			= "CryptCreateHash";
const CHAR* CRYPT_FN_DestroyHash		= "CryptDestroyHash";
const CHAR* CRYPT_FN_HashData			= "CryptHashData";
const CHAR* CRYPT_FN_DestroyKey			= "CryptDestroyKey";
const CHAR* CRYPT_FN_ImportKey			= "CryptImportKey";
const CHAR* CRYPT_FN_VerifySignature	= "CryptVerifySignatureA";
*/

const CHAR CRYPT_MODULE[]				= "41#4%<fgU";
// samp.dll RVA: 0x0011bb18
const CHAR CRYPT_FN_AcquireContext[]	= {0x16,0x27,0x2C,0x25,0x21,0x14,0x36,0x24,
											0x20,0x3C,0x27,0x30,0x16,0x3A,0x3B,0x21,
											0x30,0x2D,0x21,0x14,0x55,0x00,0x00,0x00};
// samp.dll RVA: 0x0011bb30
const CHAR CRYPT_FN_ReleaseContext[]	= {0x16,0x27,0x2C,0x25,0x21,0x07,0x30,0x39,
											0x30,0x34,0x26,0x30,0x16,0x3A,0x3B,0x21,
											0x30,0x2D,0x21,0x55,0x00,0x00,0x00,0x00,};
// samp.dll RVA: 0x0011bb48
const CHAR CRYPT_FN_CreateHash[]		= {0x16,0x27,0x2C,0x25,0x21,0x16,0x27,0x30,
											0x34,0x21,0x30,0x1D,0x34,0x26,0x3D,0x55};
// samp.dll RVA: 0x0011bb58
const CHAR CRYPT_FN_DestroyHash[]		= {0x16,0x27,0x2C,0x25,0x21,0x11,0x30,0x26,
											0x21,0x27,0x3A,0x2C,0x1D,0x34,0x26,0x3D,
											0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
// samp.dll RVA: 0x0011bb70
const CHAR CRYPT_FN_HashData[]			= {0x16,0x27,0x2C,0x25,0x21,0x1D,0x34,0x26,
											0x3D,0x11,0x34,0x21,0x34,0x55,0x00,0x00};
// samp.dll RVA: 0x0011bb80
const CHAR CRYPT_FN_DestroyKey[]		= {0x16,0x27,0x2C,0x25,0x21,0x11,0x30,0x26,
											0x21,0x27,0x3A,0x2C,0x1E,0x30,0x2C,0x55};
// samp.dll RVA: 0x0011bb90
const CHAR CRYPT_FN_ImportKey[]			= {0x16,0x27,0x2C,0x25,0x21,0x1C,0x38,0x25,
											0x3A,0x27,0x21,0x1E,0x30,0x2C,0x55,0x00};
// samp.dll RVA: 0x0011bba0
const CHAR CRYPT_FN_VerifySignature[]	= {0x16,0x27,0x2C,0x25,0x21,0x03,0x30,0x27,
											0x3C,0x33,0x2C,0x06,0x3C,0x32,0x3B,0x34,
											0x21,0x20,0x27,0x30,0x14,0x55,0x00,0x00};

#define CRYPT_GET_NAME(a,b)					\
	CHAR* a = NULL;							\
	{										\
		DWORD dwFnLen = strlen(b);			\
		a = new CHAR[dwFnLen];				\
		for(DWORD i=0; i<dwFnLen; i++)		\
			##a[i] = (CHAR)(##b[i] ^ 0x55);	\
	}

#ifndef CRYPT_FN_CPP

#define CRYPT_IMPL_FN(n,r,p)								\
	r WINAPI xCryptCall##n##p;

#else

// This is for the actual function when in the .CPP
#define CRYPT_IMPL_FN(n,r,p)								\
	DWORD xCryptOffs##n = NULL;								\
	__declspec(naked) r WINAPI xCryptCall##n##p				\
	{														\
		if (xCryptOffs##n == NULL) {						\
			_asm { lea eax, CRYPT_FN_##n };					\
			_asm { push eax };								\
			_asm { call xCryptGetFunction };				\
			_asm { add esp, 4 };							\
			_asm { mov dword ptr xCryptOffs##n, eax };		\
		}													\
		_asm { mov eax, dword ptr xCryptOffs##n };			\
		_asm { jmp eax };									\
	}

#endif

#ifdef CRYPT_FN_CPP

// samp.dll RVA: 0x000650f0
FARPROC xCryptGetFunction(CHAR* name)
{
	CRYPT_GET_NAME(szModName, CRYPT_MODULE);
	CRYPT_GET_NAME(szProcName, name);
	HMODULE hMod = GetModuleHandleA(szModName);
	FARPROC fpProc = GetProcAddress(hMod, szProcName);
	return fpProc;
}

#endif

#define CRYPT(n)		\
	xCryptCall##n

// Native cache xCryptOffsAcquireContext: samp.dll RVA: 0x0016c388
// samp.dll RVA: 0x00065170
CRYPT_IMPL_FN(AcquireContext,  BOOL, (HCRYPTPROV* phProv, LPCSTR pszContainer, LPCSTR pszProvider, DWORD dwProvType, DWORD dwFlags))
// Native cache xCryptOffsReleaseContext: samp.dll RVA: 0x0016c38c
// samp.dll RVA: 0x000651a0
CRYPT_IMPL_FN(ReleaseContext,  BOOL, (HCRYPTPROV hProv, DWORD dwFlags))
// Native cache xCryptOffsCreateHash: samp.dll RVA: 0x0016c390
// samp.dll RVA: 0x000651d0
CRYPT_IMPL_FN(CreateHash,      BOOL, (HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH* phHash))
// Native cache xCryptOffsDestroyHash: samp.dll RVA: 0x0016c394
// samp.dll RVA: 0x00065200
CRYPT_IMPL_FN(DestroyHash,     BOOL, (HCRYPTHASH hHash))
// Native cache xCryptOffsHashData: samp.dll RVA: 0x0016c398
// samp.dll RVA: 0x00065230
CRYPT_IMPL_FN(HashData,        BOOL, (HCRYPTHASH hHash, BYTE* pbData, DWORD dwDataLen, DWORD dwFlags))
// Native cache xCryptOffsDestroyKey: samp.dll RVA: 0x0016c39c
// samp.dll RVA: 0x00065260
CRYPT_IMPL_FN(DestroyKey,      BOOL, (HCRYPTKEY hKey))
// Native cache xCryptOffsImportKey: samp.dll RVA: 0x0016c3a0
// samp.dll RVA: 0x00065290
CRYPT_IMPL_FN(ImportKey,       BOOL, (HCRYPTPROV hProv, BYTE* pbData, DWORD dwDataLen, HCRYPTKEY hPubKey, DWORD dwFlags, HCRYPTKEY* phKey))
// Native cache xCryptOffsVerifySignature: samp.dll RVA: 0x0016c3a4
// samp.dll RVA: 0x000652c0
CRYPT_IMPL_FN(VerifySignature, BOOL, (HCRYPTHASH hHash, BYTE* pbSignature, DWORD dwSigLen, HCRYPTKEY hPubKey, LPCSTR sDescription, DWORD dwFlags))

#else

#define CRYPT(n)		\
	Crypt##n

#endif
