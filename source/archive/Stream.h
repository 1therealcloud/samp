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

    CAbstractStream() {}
    virtual ~CAbstractStream() {}

    // MSVC scalar deleting destructor emitted for ~CAbstractStream.

    virtual unsigned int Read(void* pbBuffer, int nNumberOfBytes) = 0;
    virtual unsigned int Read(void* pbBuffer, int nElementSize, int nNumberOfElements)
    {
        return Read(pbBuffer, nNumberOfElements * nElementSize);
    }

    virtual void Seek(int nOffset, eSeek seekMode) = 0;
    virtual void Seek(int nOffset)
    {
        Seek(nOffset, SeekStart);
    }

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

    CFileStream()
    {
        m_fiFile = NULL;
    }

    CFileStream(const char* szFilename, eType type, eMode mode)
    {
        m_fiFile = NULL;
        Open(szFilename, type, mode);
    }

    virtual ~CFileStream()
    {
        if (m_fiFile)
            Close();
    }

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

        if (fopen_s(&m_fiFile, szFilename, szMode) != 0)
            return false;

        return true;
    }

    virtual void Close()
    {
        if (m_fiFile)
        {
            fclose(m_fiFile);
            m_fiFile = NULL;
        }
    }

    virtual unsigned int Read(void* pbBuffer, int nNumberOfBytes)
    {
        return (unsigned int)fread(pbBuffer, 1, nNumberOfBytes, m_fiFile);
    }

    virtual unsigned int Read(void* pbBuffer, int nElementSize, int nNumberOfElements)
    {
        return (unsigned int)fread(pbBuffer, nElementSize, nNumberOfElements, m_fiFile);
    }

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

    virtual unsigned int Tell()
    {
        return (unsigned int)ftell(m_fiFile);
    }
};
