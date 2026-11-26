//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2006 SA:MP team
//
//----------------------------------------------------------

#pragma once

#include "ArchiveCommon.h"
#include "Obfuscator.h"
#include "Stream.h"

#include <stddef.h>
#include <windows.h>

//#include "../../archive/pkey.h"
//#include "../../archive/skey.h"
#include "keys/pkey.h"
#include "keys/skey.h"

#define FS_INVALID_FILE 0xFFFFFFFF

typedef struct _AFS_ENTRYBT_NODE
{
    SAA_ENTRY* pEntry;
    _AFS_ENTRYBT_NODE* pLNode;
    _AFS_ENTRYBT_NODE* pRNode;
    BYTE* pbData;

    _AFS_ENTRYBT_NODE()
    {
        this->pEntry = NULL;
        this->pLNode = NULL;
        this->pRNode = NULL;
        this->pbData = NULL;
    }

    _AFS_ENTRYBT_NODE(SAA_ENTRY* pSAAEntry)
    {
        this->pEntry = pSAAEntry;
        this->pLNode = NULL;
        this->pRNode = NULL;
        this->pbData = NULL;
    }

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

#pragma pack(push, 1)
class CArchiveFS
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

    void LoadEntries();

    static DWORD ms_dwHashInit;

    DWORD HashString(PCHAR szString);

public:
    CArchiveFS(void);

#ifdef ARCTOOL
    CArchiveFS(DWORD dwNumEntries, DWORD dwFDSize);
#endif // ARCTOOL

    virtual ~CArchiveFS(void);

    virtual bool Load(char* szFileName);
    virtual void Unload();

    virtual DWORD GetFileIndex(DWORD dwFileHash);
    virtual DWORD GetFileIndex(char* szFileName);
    virtual DWORD GetFileSize(DWORD dwFileIndex);
    virtual BYTE* GetFileData(DWORD dwFileIndex);
};
#pragma pack(pop)

extern CArchiveFS* g_pArchive;
