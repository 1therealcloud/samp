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
    // 0x001027b0
    static DWORD ms_dwRounds;
    // 0x001027b4
    static DWORD ms_dwInitDelta;
    // 0x0012e390
    static DWORD ms_dwInitSum;
    // R5 address to verify
    static BOOL ms_bInitDone;

    DWORD m_pdwKey[TEA_KEY_SIZE / sizeof(DWORD)];

#ifdef ARCTOOL
    void EncryptBlock(DWORD& dwV0, DWORD& dwV1);
#endif

    // 0x00065b00
    void DecryptBlock(DWORD& dwV0, DWORD& dwV1);

public:
    // 0x00065a60
    CTinyEncrypt(void);
    // 0x00065ab0
    ~CTinyEncrypt(void);

    // 0x00065ac0
    void SetKey(BYTE* pbKey, BYTE bytXORKey);

#ifdef ARCTOOL
    void LoadKey(PCHAR szFileName);
    void WriteKey(PCHAR szFileName);
    void WriteCHeaderFile(PCHAR szFileName);

    void EncryptData(DWORD dwLength, BYTE* pbData);
#endif

    // 0x00065bc0
    void DecryptData(DWORD dwLength, BYTE* pbData);
};
