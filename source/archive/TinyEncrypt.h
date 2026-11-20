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
    // 0x00140798
    static DWORD ms_dwRounds;
    // 0x0014079c
    static DWORD ms_dwInitDelta;
    // 0x0016c3a8
    static DWORD ms_dwInitSum;
    // 0x0016c3ac
    static BOOL ms_bInitDone;

    DWORD m_pdwKey[TEA_KEY_SIZE / sizeof(DWORD)];

#ifdef ARCTOOL
    void EncryptBlock(DWORD& dwV0, DWORD& dwV1);
#endif

    // 0x00065580
    void DecryptBlock(DWORD& dwV0, DWORD& dwV1);

public:
    // 0x000654e0
    CTinyEncrypt(void);
    // 0x00065530
    ~CTinyEncrypt(void);

    // 0x00065540
    void SetKey(BYTE* pbKey, BYTE bytXORKey);

#ifdef ARCTOOL
    void LoadKey(PCHAR szFileName);
    void WriteKey(PCHAR szFileName);
    void WriteCHeaderFile(PCHAR szFileName);

    void EncryptData(DWORD dwLength, BYTE* pbData);
#endif

    // 0x00065640
    void DecryptData(DWORD dwLength, BYTE* pbData);
};
