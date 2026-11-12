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
    // 0x00001070
    ~CMemoryStream();
    // 0x00001630
    unsigned int Skip(int Count);
    // 0x00002d60
    unsigned int Save(FILE* File);
    // 0x00002dc0
    unsigned int Save(const char* FileName);
    // 0x00002e00
    bool MakeOwned();
    // 0x00002e40
    unsigned int Write(const void* Buffer, unsigned int Count);
    // 0x00002ec0
    unsigned int Read(void* Buffer, unsigned int Count);
    // 0x00002f40
    unsigned int Load(const char* FileName);
    // 0x00003030
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
    // 0x00001660
    CRwChunk();
    // 0x00001690
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
    // 0x00001d10
    bool Resize(unsigned int NewCount);
    // 0x00003b30
    int Add(void* Value);
};
#pragma pack(pop)

// 0x000016c0
CRwChunk* ReadChunk(CMemoryStream* Stream);
// 0x00001bb0
bool AppendChunk(CRwChunk* Destination, const CRwChunk* Source);

struct CAllocatorCallbacks
{
    void*(__cdecl* Allocate)(void* Context, unsigned int Size);
    void(__cdecl* Release)(void* Context, void* Address);
};
// Native callback pairs retained in the DLL's data, although its exports do not use them.
// 0x00011040
extern CAllocatorCallbacks HeapAllocator;
// 0x00011048
extern CAllocatorCallbacks VirtualAllocator;

#endif
