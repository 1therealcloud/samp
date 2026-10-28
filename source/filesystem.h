//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
// Version: $Id: filesystem.h,v 1.5 2006/03/29 02:38:51 kyeman Exp $
//
//----------------------------------------------------------

#pragma once

//----------------------------------------------------------

#define FS_FILE_MAGIC 0x32414153 // "SAA2"
#define FS_BLOCK_SIZE 0x800      // 2kb
#define FS_INVALID_FILE 0xFFFFFFFF
#define FS_ENCKEY_VAR 37625
#define FS_ENC_CONST1 54825
#define FS_ENC_CONST2 91722

//----------------------------------------------------------

typedef struct _FS_HEADER
{
    DWORD dwSAAV;
    DWORD dwFileCount;
    WORD wKey;
} FS_HEADER;

typedef struct _FS_FILE_ENTRY
{
    DWORD dwOffset;
    DWORD dwSize;
    char szName[24];
    DWORD dwRealSize;
    WORD wKey;
} FS_FILE_ENTRY;

//----------------------------------------------------------

class CFileSystem
{
private:
    bool m_bLoaded;
    DWORD m_dwFileCount;
    WORD m_wKey;
    FS_FILE_ENTRY* m_pFileList;
    BYTE** m_pFileData;

public:
    // 0x00060500
    CFileSystem();
    // 0x00060690
    virtual ~CFileSystem();

    // 0x000606b0
    virtual bool Load(char* szFileName);
    // 0x00060520
    virtual void Unload();

    // 0x00060580
    virtual DWORD GetFileIndex(char* szFileName);
    // 0x000605e0
    virtual DWORD GetFileSize(DWORD dwFileIndex);
    // 0x00060610
    virtual BYTE* GetFileData(DWORD dwFileIndex);

    // 0x00060640
    void DecryptData(BYTE* pData, DWORD dwDataLen, WORD wKey);
};

//----------------------------------------------------------
// EOF
