//----------------------------------------------------------
//
//   SA:MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2007 SA:MP team
//
//----------------------------------------------------------

#pragma once

// This file defines a basic interface for data streams
// and provides implementations for a filebased stream
// as well as as a memory based stream.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class CAbstractStream
{
public:
    enum eSeek
    {
        SeekStart,
        SeekCurrent,
        SeekEnd,
    };

    // R5 address to verify
    CAbstractStream() {}

    // R5 address to verify
    virtual ~CAbstractStream() {}
    // R5 address to verify
    // MSVC scalar deleting destructor emitted for ~CAbstractStream.

    // inline
    virtual unsigned int Read(void* pbBuffer, int nNumberOfBytes) = 0;

    // R5 address to verify
    virtual unsigned int Read(void* pbBuffer, int nElementSize, int nNumberOfElements)
    {
        return Read(pbBuffer, nNumberOfElements * nElementSize);
    }

    // inline
    virtual unsigned int Write(const void* pbBuffer, int nNumberOfBytes) = 0;

    // R5 address to verify
    virtual unsigned int Write(const void* pbBuffer, int nElementSize, int nNumberOfElements)
    {
        return Write(pbBuffer, nNumberOfElements * nElementSize);
    }

    // inline
    virtual void Seek(int nOffset, eSeek seekMode) = 0;

    // R5 address to verify
    virtual void Seek(int nOffset)
    {
        Seek(nOffset, SeekStart);
    }

    // inline
    virtual unsigned int Tell() = 0;
};

class CFileStream : public CAbstractStream
{
protected:
    FILE* m_fiFile;

public:
    enum eMode
    {
        ModeRead,
        ModeReadWrite,
        ModeAppend,
        ModeWrite,
    };

    enum eType
    {
        TypeText,
        TypeBinary,
    };

    // inline
    CFileStream()
    {
        m_fiFile = NULL;
    }

    // 0x00065170
    CFileStream(const char* szFilename, eType type, eMode mode)
    {
        m_fiFile = NULL;
        Open(szFilename, type, mode);
    }

    // 0x000652d0
    virtual ~CFileStream()
    {
        if (m_fiFile)
            Close();
    }

    // 0x00064480
    virtual bool Open(const char* szFilename, eType type, eMode mode)
    {
        if (m_fiFile)
            Close();

        char szMode[4];
        int nPos = 0;
        szMode[0] = szMode[1] = szMode[2] = szMode[3] = 0;

        switch (mode)
        {
            case ModeRead:
                szMode[nPos++] = 'r';
                break;
            case ModeWrite:
                szMode[nPos++] = 'w';
                break;
            case ModeReadWrite:
                szMode[nPos++] = 'r';
                szMode[nPos++] = '+';
                break;
            case ModeAppend:
                szMode[nPos++] = 'a';
                break;
        }

        switch (type)
        {
            case TypeText:
                szMode[nPos++] = 't';
                break;
            case TypeBinary:
                szMode[nPos++] = 'b';
                break;
        }

        m_fiFile = fopen(szFilename, szMode);
        if (!m_fiFile)
            return false;

        return true;
    }

    // R5 address to verify
    virtual bool IsOpen()
    {
        return (m_fiFile != NULL);
    }

    // R5 address to verify
    virtual void Close()
    {
        if (m_fiFile)
        {
            fclose(m_fiFile);
            m_fiFile = NULL;
        }
    }

    // R5 address to verify
    virtual unsigned int Read(void* pbBuffer, int nNumberOfBytes)
    {
        return (unsigned int)fread(pbBuffer, 1, nNumberOfBytes, m_fiFile);
    }

    // R5 address to verify
    virtual unsigned int Read(void* pbBuffer, int nElementSize, int nNumberOfElements)
    {
        return (unsigned int)fread(pbBuffer, nElementSize, nNumberOfElements, m_fiFile);
    }

    // R5 address to verify
    virtual unsigned int Write(const void* pbBuffer, int nNumberOfBytes)
    {
        return (unsigned int)fwrite(pbBuffer, 1, nNumberOfBytes, m_fiFile);
    }

    // R5 address to verify
    virtual unsigned int Write(const void* pbBuffer, int nElementSize, int nNumberOfElements)
    {
        return (unsigned int)fwrite(pbBuffer, nElementSize, nNumberOfElements, m_fiFile);
    }

    // R5 address to verify
    virtual void Seek(int nOffset, eSeek seekMode)
    {
        int mode = 0;
        switch (seekMode)
        {
            case SeekCurrent:
                mode = SEEK_CUR;
                break;
            case SeekStart:
                mode = SEEK_SET;
                break;
            case SeekEnd:
                mode = SEEK_END;
                break;
        }

        fseek(m_fiFile, nOffset, seekMode);
    }

    // R5 address to verify
    virtual unsigned int Tell()
    {
        return (unsigned int)ftell(m_fiFile);
    }
};

class CMemoryStream : public CAbstractStream
{
protected:
    unsigned char* m_pbBuffer;
    unsigned int m_nOffset;
    unsigned int m_nLength;
    bool m_bCopy;

public:
    // R5 address to verify
    CMemoryStream(unsigned char* pbBuffer, unsigned int nLength, bool bCopy = true)
    {
        m_bCopy = bCopy;
        m_nOffset = 0;
        m_nLength = nLength;

        if (bCopy)
        {
            m_pbBuffer = (unsigned char*)malloc(nLength);
            memcpy(m_pbBuffer, pbBuffer, nLength);
        }
        else
        {
            m_pbBuffer = pbBuffer;
        }
    }

    // 0x000646a0
    virtual ~CMemoryStream()
    {
        if (m_bCopy && m_pbBuffer)
        {
            free(m_pbBuffer);
        }
    }

    // 0x00064590
    virtual unsigned int Read(void* pbBuffer, int nNumberOfBytes)
    {
        if (m_nOffset + nNumberOfBytes > m_nLength)
            nNumberOfBytes = m_nLength - m_nOffset;

        memcpy(pbBuffer, m_pbBuffer + m_nOffset, nNumberOfBytes);
        m_nOffset += nNumberOfBytes;

        return nNumberOfBytes;
    }

    // R5 address to verify
    virtual unsigned int Write(const void* pbBuffer, int nNumberOfBytes)
    {
        if (m_nOffset + nNumberOfBytes > m_nLength)
            nNumberOfBytes = m_nLength - m_nOffset;

        memcpy(m_pbBuffer + m_nOffset, pbBuffer, nNumberOfBytes);
        m_nOffset += nNumberOfBytes;

        return nNumberOfBytes;
    }

    // R5 address to verify
    virtual void Seek(int nOffset, eSeek seekMode)
    {
        switch (seekMode)
        {
            case SeekStart:
                m_nOffset = nOffset;
                break;
            case SeekCurrent:
                m_nOffset += nOffset;
                break;
            case SeekEnd:
                m_nOffset = m_nLength + nOffset;
                break;
        }

        if (m_nOffset > m_nLength)
            m_nOffset = m_nLength;
    }

    // R5 address to verify
    virtual unsigned int Tell()
    {
        return m_nOffset;
    }
};
