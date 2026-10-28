//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#pragma once

#include <windows.h>

#define TEA_KEY_SIZE 16

class CTinyEncrypt
{
private:
    // samp.dll RVA: 0x00140798
    static DWORD ms_dwRounds;
    // samp.dll RVA: 0x0014079c
    static DWORD ms_dwInitDelta;
    // samp.dll RVA: 0x0016c3a8
    static DWORD ms_dwInitSum;
    // samp.dll RVA: 0x0016c3ac
    static BOOL ms_bInitDone;

    DWORD m_pdwKey[TEA_KEY_SIZE / sizeof(DWORD)];

#ifdef ARCTOOL
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void EncryptBlock(DWORD& dwV0, DWORD& dwV1);
#endif

    // samp.dll RVA: 0x00065580
    void DecryptBlock(DWORD& dwV0, DWORD& dwV1);

public:
    // samp.dll RVA: 0x000654e0
    CTinyEncrypt(void);
    // samp.dll RVA: 0x00065530
    ~CTinyEncrypt(void);

    // samp.dll RVA: 0x00065540
    void SetKey(BYTE* pbKey, BYTE bytXORKey);

#ifdef ARCTOOL
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void LoadKey(PCHAR szFileName);
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void WriteKey(PCHAR szFileName);
    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void WriteCHeaderFile(PCHAR szFileName);

    // samp.dll RVA: unknown (ARCTOOL-only branch; not established in the client)
    void EncryptData(DWORD dwLength, BYTE* pbData);
#endif

    // samp.dll RVA: 0x00065640
    void DecryptData(DWORD dwLength, BYTE* pbData);
};
