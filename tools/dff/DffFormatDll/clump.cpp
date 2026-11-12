#include "clump.h"
#include <stdlib.h>
#include <stdio.h>

// 0x00002300
CClump::CClump()
    : Error(0), ErrorDetail(0), FileSize(0), HasCol(false), Root(0), Struct(0),
      FrameList(0), GeometryList(0), Atomic(0), Extension(0), Unknown44(0), Unknown48(0) {}

// 0x000023f0
CClump::~CClump()
{
    delete Extension; Extension = 0;
    delete Atomic; Atomic = 0;
    delete GeometryList; GeometryList = 0;
    delete FrameList; FrameList = 0;
    delete Struct; Struct = 0;
    delete Root; Root = 0;
    if (Unknown48) { free(Unknown48); Unknown48 = 0; }
    int Count = Geometries.Count;
    for (unsigned int i = 0; static_cast<int>(i) < Count; ++i) {
        CGeometry* Geometry = static_cast<CGeometry*>(Geometries.Get(i));
        if (Geometry) delete Geometry;
    }
    Geometries.Resize(0);
}

// 0x000017d0
bool CClump::ReadClump(CRwChunk* Chunk)
{
    CRwChunk* Child = ReadChunk(&Chunk->Stream);
    while (Child) {
        switch (Child->Header.Type) {
        case 1:
            if (!Struct) Struct = Child;
            else delete Child;
            break;
        case 3:
            if (!Extension) Extension = Child;
            else delete Child;
            break;
        case 0xe:
            if (FrameList) { delete Child; if (!Error) Error = 0xabf0; return false; }
            FrameList = Child;
            break;
        case 0x14:
            if (Atomic) { delete Child; if (!Error) Error = 0xabf1; return false; }
            Atomic = Child;
            break;
        case 0x1a:
            if (GeometryList) { delete Child; if (!Error) Error = 0xabef; return false; }
            GeometryList = Child;
            break;
        default:
            delete Child;
            break;
        }
        Child = ReadChunk(&Chunk->Stream);
    }
    return true;
}

// 0x00001910
int CClump::Load(const char* FileName)
{
    if (!File.Load(FileName)) return -1;
    FileSize = File.Size;
    Root = ReadChunk(&File);
    if (!Root) return 1;
    if (Root->Header.Type == 0x2b) {
        delete Root;
        Root = 0;
        Root = ReadChunk(&File);
    }
    if (Root->Header.Type == 0x10) {
        if (ReadClump(Root)) {
            if (!Extension || !Extension->Header.Size) return 1;
            CRwChunk* Col = ReadChunk(&Extension->Stream);
            if (!Col) return 1;
            if (Col->Header.Type != 0x253f2ff) return 1;
            HasCol = true;
            // The native reader retains this borrowed chunk allocation.
            return 1;
        }
    }
    else if (!Error) Error = 44000;
    return 0;
}

// 0x000019d0
unsigned int CClump::Save(const char* FileName)
{
    CMemoryStream Output;
    if (!Root) return 0;
    Root->Header.Size = GeometryList->Header.Size + Extension->Header.Size +
        FrameList->Header.Size + Struct->Header.Size + 60 + Atomic->Header.Size;
    Output.Write(&Root->Header, 12);
    Struct->Header.Size = 12;
    Output.Write(&Struct->Header, 12);
    Struct->Stream.Position = 0;
    Struct->Stream.End = false;
    unsigned int Value = Struct->Stream.ReadUInt();
    Output.Write(&Value, 4);
    Value = 0; Output.Write(&Value, 4);
    Value = 0; Output.Write(&Value, 4);
    Output.Write(&FrameList->Header, 12);
    Output.Write(FrameList->Stream.Data, FrameList->Stream.Size);
    Output.Write(&GeometryList->Header, 12);
    Output.Write(GeometryList->Stream.Data, GeometryList->Stream.Size);
    Output.Write(&Atomic->Header, 12);
    Output.Write(Atomic->Stream.Data, Atomic->Stream.Size);
    Output.Write(&Extension->Header, 12);
    Output.Write(Extension->Stream.Data, Extension->Stream.Size);
    return Output.Save(FileName);
}

// 0x00001c00
int CClump::AppendCol(const char* FileName)
{
    CRwChunk Col;
    if (!Root || !Extension) return -1;
    if (!Col.Stream.Load(FileName)) return -2;
    Col.Header.Size = Col.Stream.Size;
    Col.Header.Type = 0x253f2ff;
    return AppendChunk(Extension, &Col);
}

// 0x00001d80
bool CClump::RebuildGeometry()
{
    if (!Geometries.Count) return false;
    CGeometry* Geometry = static_cast<CGeometry*>(Geometries.Get(0));
    if (!Geometry || (!Geometry->Modified && !Geometry->Unknown01)) return false;
    CRwChunk* List = new CRwChunk;
    CRwChunk* Header = new CRwChunk;
    List->Header.Type = 0x1a;
    Header->Header.Type = 1;
    unsigned int Count = 1;
    Header->Stream.Write(&Count, 4);
    Header->Header.Size = Header->Stream.Size;
    CRwChunk* Data = Geometry->Write();
    AppendChunk(List, Header);
    AppendChunk(List, Data);
    delete Header;
    delete Data;
    delete GeometryList;
    GeometryList = List;
    return true;
}

// 0x00002170
bool CClump::ReadGeometry()
{
    if (!GeometryList) return false;
    GeometryList->Stream.Position = 0;
    GeometryList->Stream.End = false;
    RwChunkHeader Header;
    GeometryList->Stream.Read(&Header, 12);
    if (Header.Type != 1 || Header.Size != 4) {
        if (!Error) Error = 0xabe7;
        return false;
    }
    unsigned int Count = GeometryList->Stream.ReadUInt();
    printf("[geometry:info] Geometry list contains %u geometries\n", Count);
    if (Count != 1) {
        printf("[geometry:error] Only a clump with 1 geometry is supported\n");
        if (!Error) Error = 0xabe6;
        return false;
    }
    CRwChunk* Chunk = ReadChunk(&GeometryList->Stream);
    if (Chunk) {
        if (Chunk->Header.Type == 0xf) {
            Chunk->Stream.Read(&Header, 12);
            if (Header.Type == 1) {
                CGeometry* Geometry = new CGeometry;
                if (!Geometry->Read(&Chunk->Stream, Chunk->Header.Version)) {
                    printf("[info] Read geometry failed error = %u\n", Geometry->Error);
                    if (!Error) Error = Geometry->Error;
                    ErrorDetail = Geometry->ErrorDetail;
                    delete Geometry;
                    // Native failure branch does not release the borrowed chunk.
                    return false;
                }
                Geometries.Add(Geometry);
            }
        }
        delete Chunk;
    }
    return true;
}
