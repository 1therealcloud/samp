#include "stream.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 0x00001000
void* __cdecl AllocateHeap(void* Context, unsigned int Size)
{
    if (!Size)
        return 0;
    return malloc(Size);
}

// 0x00001020
void __cdecl ReleaseHeap(void* Context, void* Address)
{
    free(Address);
}

// 0x00001030
void* __cdecl AllocateVirtual(void* Context, unsigned int Size)
{
    if (!Size)
        return 0;
    return VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);
}

// 0x00001050
void __cdecl ReleaseVirtual(void* Context, void* Address)
{
    if (Address)
        VirtualFree(Address, 0, MEM_RELEASE);
}

CAllocatorCallbacks HeapAllocator = {AllocateHeap, ReleaseHeap};
CAllocatorCallbacks VirtualAllocator = {AllocateVirtual, ReleaseVirtual};

CMemoryStream::CMemoryStream() : Data(0), Position(0), Size(0), End(false), OwnsData(false), Unknown0e(0), Unknown12(0), Unknown16(false) {}

// 0x00001070
CMemoryStream::~CMemoryStream()
{
    if (OwnsData && Data)
    {
        free(Data);
        Data = 0;
    }
}

// 0x00001630
unsigned int CMemoryStream::Skip(int Count)
{
    unsigned int Next = Position + Count;
    if (Next < Size)
    {
        Position = Next;
        return Next;
    }
    Position = Size;
    End = true;
    return Size;
}

// 0x00002d60
unsigned int CMemoryStream::Save(FILE* File)
{
    unsigned int Offset = 0;
    while (Offset < Size)
    {
        unsigned int Next = Offset + 1024;
        if (Next < Size)
            fwrite(Data + Offset, 1, 1024, File);
        else
        {
            fwrite(Data + Offset, 1, Size - Offset, File);
            Next = Size;
        }
        Offset = Next;
    }
    return Size;
}

// 0x00002dc0
unsigned int CMemoryStream::Save(const char* FileName)
{
    FILE* File = fopen(FileName, "wb");
    if (!File)
        return 0;
    Save(File);
    fclose(File);
    return Size;
}

// 0x00002e00
bool CMemoryStream::MakeOwned()
{
    unsigned char* Copy = static_cast<unsigned char*>(malloc(Size));
    if (!Copy)
        return false;
    memcpy(Copy, Data, Size);
    Data = Copy;
    OwnsData = true;
    return true;
}

// 0x00002e40
unsigned int CMemoryStream::Write(const void* Buffer, unsigned int Count)
{
    if (Size < Position + Count)
    {
        if (!OwnsData)
            MakeOwned();
        unsigned int Extra = Count - Size + Position;
        Data = static_cast<unsigned char*>(realloc(Data, Size + Extra));
        Size += Extra;
    }
    memcpy(Data + Position, Buffer, Count);
    Position += Count;
    return Count;
}

// 0x00002ec0
unsigned int CMemoryStream::Read(void* Buffer, unsigned int Count)
{
    memset(Buffer, 0, Count);
    if (Size < Position + Count)
        Count = Size - Position;
    if (!Count)
        return 0;
    memcpy(Buffer, Data + Position, Count);
    unsigned int Next = Position + Count;
    if (Next < Size)
        Position = Next;
    else
    {
        Position = Size;
        End = true;
    }
    return Count;
}

// 0x00002f40
unsigned int CMemoryStream::Load(const char* FileName)
{
    Position = 0;
    if (OwnsData && Data)
    {
        free(Data);
        Data = 0;
    }
    FILE* File = fopen(FileName, "rb");
    if (!File)
        return 0;
    OwnsData = true;
    Data = static_cast<unsigned char*>(malloc(1));
    Size = 1;
    unsigned char Buffer[2048];
    while (!feof(File))
    {
        unsigned int Count = static_cast<unsigned int>(fread(Buffer, 1, sizeof(Buffer), File));
        Write(Buffer, Count);
    }
    fclose(File);
    Position = 0;
    End = false;
    return Size;
}

// 0x00003030
unsigned int CMemoryStream::ReadUInt()
{
    unsigned int Value = 0;
    Read(&Value, sizeof(Value));
    return Value;
}

// 0x00001660
CRwChunk::CRwChunk()
{
    Header.Type = 0;
    Header.Size = 0;
    Header.Build = 0xffff;
    Header.Version = 0x1803;
}

// 0x00001690
CRwChunk::~CRwChunk()
{
    // CMemoryStream performs the native owned-buffer cleanup.
}

// 0x000016c0
CRwChunk* ReadChunk(CMemoryStream* Stream)
{
    if (!Stream->Size || Stream->End)
        return 0;
    CRwChunk* Chunk = new CRwChunk;
    unsigned int Value = 0;
    Stream->Read(&Value, 4);
    Chunk->Header.Type = Value;
    Value = 0;
    Stream->Read(&Value, 4);
    Chunk->Header.Size = Value;
    Value = 0;
    Stream->Read(&Value, 2);
    Chunk->Header.Build = static_cast<unsigned short>(Value);
    Value = 0;
    Stream->Read(&Value, 2);
    Chunk->Header.Version = static_cast<unsigned short>(Value);
    Chunk->Stream.Size = Chunk->Header.Size;
    Chunk->Stream.Data = Stream->Data + Stream->Position;
    Chunk->Stream.Position = 0;
    Chunk->Stream.End = false;
    Chunk->Stream.OwnsData = false;
    Stream->Skip(Chunk->Header.Size);
    return Chunk;
}

// 0x00001bb0
bool AppendChunk(CRwChunk* Destination, const CRwChunk* Source)
{
    if (!Destination || !Source)
        return false;
    Destination->Stream.Write(&Source->Header, sizeof(RwChunkHeader));
    Destination->Stream.Write(Source->Stream.Data, Source->Stream.Size);
    Destination->Header.Size += Source->Stream.Size + sizeof(RwChunkHeader);
    return true;
}

CPointerArray::CPointerArray() : Items(0), Count(0) {}

CPointerArray::~CPointerArray()
{
    if (Items)
    {
        free(Items);
        Items = 0;
    }
    Count = 0;
}

void* CPointerArray::Get(unsigned int Index) const
{
    if (Index < Count)
        return Items[Index];
    return 0;
}

// 0x00001d10
bool CPointerArray::Resize(unsigned int NewCount)
{
    if (!NewCount)
    {
        if (Items)
        {
            free(Items);
            Items = 0;
        }
        Count = 0;
        return true;
    }
    void** Resized = static_cast<void**>(realloc(Items, NewCount * sizeof(void*)));
    if (!Resized)
        return false;
    Items = Resized;
    for (unsigned int Index = Count; Index < NewCount; ++Index)
        Items[Index] = 0;
    Count = NewCount;
    return true;
}

// 0x00003b30
int CPointerArray::Add(void* Value)
{
    unsigned int OldCount = Count;
    for (unsigned int Index = 0; Index < OldCount; ++Index)
        if (!Items[Index])
        {
            Items[Index] = Value;
            return Index;
        }
    if (!Resize(OldCount + 1))
        return -1;
    Items[OldCount] = Value;
    return OldCount;
}
