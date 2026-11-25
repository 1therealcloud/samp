#ifndef KDFF_STREAM_H
#define KDFF_STREAM_H

#include <stdio.h>

// Private names are descriptive. Layout and behavior come from kdff.dll.
#pragma pack(push, 1)
class CMemoryStream
{
public:
    unsigned char* Data;    // +0x00
    unsigned int Position;  // +0x04
    unsigned int Size;      // +0x08
    bool End;               // +0x0c
    bool OwnsData;          // +0x0d
    unsigned int Unknown0e; // +0x0e
    unsigned int Unknown12; // +0x12
    bool Unknown16;         // +0x16

    // Inlined in native constructors and temporary stream declarations.
    CMemoryStream();

    ~CMemoryStream();

    unsigned int Skip(int Count);

    unsigned int Save(FILE* File);

    unsigned int Save(const char* FileName);

    bool MakeOwned();

    unsigned int Write(const void* Buffer, unsigned int Count);

    unsigned int Read(void* Buffer, unsigned int Count);

    unsigned int Load(const char* FileName);

    unsigned int ReadUInt();
};

struct RwChunkHeader
{
    unsigned int Type;
    unsigned int Size;
    unsigned short Build;
    unsigned short Version;
};

class CRwChunk
{
public:
    RwChunkHeader Header;
    CMemoryStream Stream;

    CRwChunk();

    ~CRwChunk();
};

// Shared pointer-array implementation; payload ownership belongs to its users.
class CPointerArray
{
public:
    void** Items;
    unsigned int Count;
    CPointerArray();
    ~CPointerArray();
    // Inlined native indexed access returns null outside the array.
    void* Get(unsigned int Index) const;

    bool Resize(unsigned int NewCount);

    int Add(void* Value);
};
#pragma pack(pop)

CRwChunk* ReadChunk(CMemoryStream* Stream);

bool AppendChunk(CRwChunk* Destination, const CRwChunk* Source);

struct CAllocatorCallbacks
{
    void*(__cdecl* Allocate)(void* Context, unsigned int Size);
    void(__cdecl* Release)(void* Context, void* Address);
};
// Native callback pairs retained in the DLL's data, although its exports do not use them.

extern CAllocatorCallbacks HeapAllocator;

extern CAllocatorCallbacks VirtualAllocator;

#endif
