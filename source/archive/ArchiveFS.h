//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#pragma once
#include <static_assert.h>

#include <stddef.h>
#include <windows.h>

#include "ArchiveCommon.h"
#include "Obfuscator.h"
#include "Stream.h"

//#include "../../archive/pkey.h"
//#include "../../archive/skey.h"
#include "keys.h"

#ifndef ARCTOOL

// Load the original CFileSystem class
#include "filesystem.h"

#endif

#define FS_INVALID_FILE 0xFFFFFFFF

typedef struct _AFS_ENTRYBT_NODE
{
    SAA_ENTRY* pEntry;
    _AFS_ENTRYBT_NODE* pLNode;
    _AFS_ENTRYBT_NODE* pRNode;
    BYTE* pbData;

    // 0x000641f0
    _AFS_ENTRYBT_NODE()
    {
        this->pEntry = NULL;
        this->pLNode = NULL;
        this->pRNode = NULL;
        this->pbData = NULL;
    }

    // 0x00064200
    _AFS_ENTRYBT_NODE(SAA_ENTRY* pSAAEntry)
    {
        this->pEntry = pSAAEntry;
        this->pLNode = NULL;
        this->pRNode = NULL;
        this->pbData = NULL;
    }

    // 0x00064220
    void AddEntry(SAA_ENTRY* pSAAEntry)
    {
        if (this->pEntry == NULL)
        {
            this->pEntry = pSAAEntry;
        }
        else
        {
            if (pSAAEntry->dwFileNameHash < this->pEntry->dwFileNameHash)
            {
                if (this->pLNode == NULL)
                    this->pLNode = new _AFS_ENTRYBT_NODE(pSAAEntry);
                else
                    this->pLNode->AddEntry(pSAAEntry);
            }
            else
            {
                if (this->pRNode == NULL)
                    this->pRNode = new _AFS_ENTRYBT_NODE(pSAAEntry);
                else
                    this->pRNode->AddEntry(pSAAEntry);
            }
        }
    }

    // 0x000642b0
    _AFS_ENTRYBT_NODE* FindEntry(DWORD dwHash)
    {
        if (this->pEntry->dwFileNameHash == dwHash)
        {
            return this;
        }
        else
        {
            if (dwHash < this->pEntry->dwFileNameHash)
            {
                if (this->pLNode == NULL)
                    return NULL;
                else
                    return this->pLNode->FindEntry(dwHash);
            }
            else
            {
                if (this->pRNode == NULL)
                    return NULL;
                else
                    return this->pRNode->FindEntry(dwHash);
            }
        }
    }

    // 0x00064d80
    ~_AFS_ENTRYBT_NODE()
    {
        if (this->pLNode != NULL)
            delete this->pLNode;
        if (this->pRNode != NULL)
            delete this->pRNode;
        if (this->pbData != NULL)
            delete[] this->pbData;
    }

} AFS_ENTRYBT_NODE;

STATIC_ASSERT(sizeof(AFS_ENTRYBT_NODE) == 16);
STATIC_ASSERT(offsetof(AFS_ENTRYBT_NODE, pEntry) == 0);
STATIC_ASSERT(offsetof(AFS_ENTRYBT_NODE, pLNode) == 4);
STATIC_ASSERT(offsetof(AFS_ENTRYBT_NODE, pRNode) == 8);
STATIC_ASSERT(offsetof(AFS_ENTRYBT_NODE, pbData) == 12);

const DWORD FILES_FORCE_FS[] = {
    OBFUSCATE_DATA(0x5440792e), //ar_stats.dat
    OBFUSCATE_DATA(0x57d2bfe5), //carmods.dat
    OBFUSCATE_DATA(0x4e643d5e), //default.dat
    OBFUSCATE_DATA(0x2acf3319), //default.ide
    OBFUSCATE_DATA(0xe9a7df22), //gta.dat
    OBFUSCATE_DATA(0x2cc7ce25), //handling.cfg
    OBFUSCATE_DATA(0xd83f24dd), //main.scm
    OBFUSCATE_DATA(0xcbc78e39), //melee.dat
    OBFUSCATE_DATA(0xb7ffa1cb), //object.dat
    OBFUSCATE_DATA(0x6fdca2be), //ped.dat
    OBFUSCATE_DATA(0x6c62978a), //peds.ide
    OBFUSCATE_DATA(0x11a462d1), //script.img
    OBFUSCATE_DATA(0x131fad35), //shopping.dat
    OBFUSCATE_DATA(0x4633bceb), //stream.ini
    OBFUSCATE_DATA(0xc1d9e789), //timecyc.dat
    OBFUSCATE_DATA(0xee6dfcb7), //vehicles.ide
    OBFUSCATE_DATA(0xebfa9ab6), //weapon.dat
    OBFUSCATE_DATA(0x7a504fb9), //loadscv.txd  (remote process AC dll)
    OBFUSCATE_DATA(0xa848b69a), //bindat.bin
};

class CArchiveFS
#ifndef ARCTOOL
    : public CFileSystem
#endif
{
private:
    bool m_bLoaded;
    CAbstractStream* m_pStream;
    bool m_bEntriesLoaded;
    SAA_FILE_HEADER m_Header;
    SAA_ENTRY m_pEntries[SAA_MAX_ENTRIES];
    AFS_ENTRYBT_NODE m_EntryBTreeRoot;
    DWORD m_dwObfsMask;
    DWORD m_dwNumEntries;

    // 0x00064550
    void LoadEntries();

    // 0x00140778
    static DWORD ms_dwHashInit;
    // 0x00064300
    DWORD HashString(PCHAR szString);

public:
    // 0x00064dd0
    CArchiveFS(void);
    // 0x00064e20
    CArchiveFS(DWORD dwNumEntries, DWORD dwFDSize);
    // 0x00064e80
    virtual ~CArchiveFS(void);

    // 0x00064f20
    virtual bool Load(char* szFileName);
    // 0x000647b0
    virtual bool Load(BYTE* pbData, DWORD nLength);
    // 0x00064890
    virtual void Unload();

    // 0x000648c0
    virtual DWORD GetFileIndex(DWORD dwFileHash);
    // 0x00064940
    virtual DWORD GetFileIndex(char* szFileName);
    // 0x000649a0
    virtual DWORD GetFileSize(DWORD dwFileIndex);
    // 0x000649e0
    virtual BYTE* GetFileData(DWORD dwFileIndex);

    // 0x00064bd0
    virtual void UnloadData(DWORD dwFileIndex);
};

// 0x002ac9d4
extern CArchiveFS* g_pArchive;
