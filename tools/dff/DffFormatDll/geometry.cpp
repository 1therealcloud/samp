#include "geometry.h"
#include <stdlib.h>
#include <string.h>

// 0x00001ee0
CGeometry::CGeometry()
    : Modified(false), Unknown01(false), Error(0), ErrorDetail(0), Flags(0), Format(0),
      TriangleCount(0), VertexCount(0), MorphCount(0), HasColors(false), HasNightColors(false),
      Colors(0), NightColors(0), UV(0), Triangles(0), Radius(0), HasVertices(0), HasNormals(0),
      Vertices(0), Normals(0), MaterialChunk(0), ExtensionChunk(0), MaterialCount(0),
      HasBinMesh(false), BinMeshFlags(0), SplitCount(0), IndexCount(0)
{
    Center.x = Center.y = Center.z = 0;
    memset(&Skin, 0, sizeof(Skin));
}

// 0x00001f60
CGeometry::~CGeometry()
{
    if (Normals) { free(Normals); Normals = 0; }
    if (Vertices) { free(Vertices); Vertices = 0; }
    if (Triangles) { free(Triangles); Triangles = 0; }
    if (UV) { free(UV); UV = 0; }
    if (Colors) { free(Colors); Colors = 0; }
    if (NightColors) { free(NightColors); NightColors = 0; }
    if (Skin.Bones) { free(Skin.Bones); Skin.Bones = 0; }
    if (Skin.Indices) { free(Skin.Indices); Skin.Indices = 0; }
    if (Skin.Weights) { free(Skin.Weights); Skin.Weights = 0; }
    if (Skin.Matrices) { free(Skin.Matrices); Skin.Matrices = 0; }
    if (Skin.Unknown20) { free(Skin.Unknown20); Skin.Unknown20 = 0; }
    if (Skin.Unknown24) { free(Skin.Unknown24); Skin.Unknown24 = 0; }
    if (Skin.Unknown28) { free(Skin.Unknown28); Skin.Unknown28 = 0; }
    unsigned int i;
    unsigned int Count = Materials.Count;
    for (i = 0; i < Count; ++i)
        if (Materials.Get(i)) free(Materials.Get(i));
    Count = Splits.Count;
    for (i = 0; i < Count; ++i) {
        RwSplit* Split = static_cast<RwSplit*>(Splits.Get(i));
        if (Split) {
            if (Split->Indices) { free(Split->Indices); Split->Indices = 0; }
            free(Split);
        }
    }
    delete MaterialChunk;
    MaterialChunk = 0;
    delete ExtensionChunk;
    ExtensionChunk = 0;
    // Member destructors free Splits and Materials pointer tables, in that order.
}

// 0x00004420
bool CGeometry::Read(CMemoryStream* Stream, unsigned short Version)
{
    unsigned int Value = 0;
    Stream->Read(&Value, 2); Flags = static_cast<unsigned short>(Value);
    Value = 0; Stream->Read(&Value, 2); Format = static_cast<unsigned short>(Value);
    TriangleCount = Stream->ReadUInt();
    VertexCount = Stream->ReadUInt();
    MorphCount = Stream->ReadUInt();
    if (static_cast<unsigned short>((Version >> 6) + 0x300) < 0x340) {
        Stream->ReadUInt(); Stream->ReadUInt(); Stream->ReadUInt();
    }
    if (Stream->End) { if (!Error) Error = 0xabe1; return false; }
    if (static_cast<int>(TriangleCount) < 0 || TriangleCount > 100000) {
        if (!Error) Error = 0xabe5;
        return false;
    }
    if (static_cast<int>(VertexCount) < 0 || VertexCount > 200000) {
        if (!Error) Error = 0xabe4;
        return false;
    }
    if (Flags & 8) {
        HasColors = true;
        Colors = static_cast<RwColor*>(calloc(VertexCount, 4));
        if (!Colors || Stream->Size - Stream->Position < VertexCount * 4) goto BadRead;
        Stream->Read(Colors, VertexCount * 4);
    }
    if (Flags & 0x84) {
        UV = static_cast<RwUV*>(calloc(VertexCount, 8));
        if (!UV || Stream->Size - Stream->Position < VertexCount * 8) goto BadRead;
        Stream->Read(UV, VertexCount * 8);
    }
    Triangles = static_cast<RwTriangle*>(calloc(TriangleCount, 8));
    if (!Triangles || Stream->Size - Stream->Position < TriangleCount * 8) goto BadRead;
    Stream->Read(Triangles, TriangleCount * 8);
    if (Stream->End) goto BadRead;
    Stream->Read(&Center, 16);
    HasVertices = Stream->ReadUInt();
    HasNormals = Stream->ReadUInt();
    if (HasVertices) {
        Vertices = static_cast<CVector*>(calloc(VertexCount, 12));
        if (!Vertices || Stream->Size - Stream->Position < VertexCount * 12) goto BadRead;
        Stream->Read(Vertices, VertexCount * 12);
    }
    if (Flags & 0x10) {
        Normals = static_cast<CVector*>(calloc(VertexCount, 12));
        if (!Normals || Stream->Size - Stream->Position < VertexCount * 12) goto BadRead;
        Stream->Read(Normals, VertexCount * 12);
    }
    if (!ReadMaterials(Stream)) { if (!Error) Error = 0xabea; return false; }
    if (!ReadExtensions(Stream)) { if (!Error) Error = 0xabeb; return false; }
    return true;
BadRead:
    if (!Error) Error = 0xabe1;
    return false;
}

// 0x00004310
bool CGeometry::ReadMaterials(CMemoryStream* Stream)
{
    MaterialChunk = ReadChunk(Stream);
    if (MaterialChunk->Header.Type != 8) {
        if (!Error) Error = 0xabf3;
        return false;
    }
    RwChunkHeader Header;
    MaterialChunk->Stream.Read(&Header, 12);
    if (Header.Type == 1) {
        MaterialCount = MaterialChunk->Stream.ReadUInt();
        MaterialChunk->Stream.Skip(Header.Size - 4);
        if (static_cast<int>(MaterialCount) < 1 || static_cast<int>(MaterialCount) > 255) {
            if (!Error) Error = 0xabe9;
            return false;
        }
        for (int i = 0; i < static_cast<int>(MaterialCount); ++i) {
            MaterialChunk->Stream.Read(&Header, 12);
            if (Header.Type == 7 && !ReadMaterial(&MaterialChunk->Stream)) {
                if (!Error) Error = 0xabe9;
                return false;
            }
        }
    }
    return true;
}

// 0x00003ea0
bool CGeometry::ReadMaterial(CMemoryStream* Stream)
{
    RwMaterial* Material = static_cast<RwMaterial*>(calloc(1, sizeof(RwMaterial)));
    RwChunkHeader Header;
    Stream->Read(&Header, 12);
    if (Header.Type != 1 || Header.Size != 28) {
        if (!Error) Error = 0xabe7;
        ErrorDetail = 9;
        if (Material) free(Material);
        return false;
    }
    unsigned int* Fields = reinterpret_cast<unsigned int*>(Material);
    for (unsigned int i = 0; i < 7; ++i) Fields[i] = Stream->ReadUInt();
    if (Material->TextureCount >= 3) {
        if (!Error) Error = 0xabe8;
        ErrorDetail = Material->TextureCount;
        free(Material);
        return false;
    }
    for (unsigned int i = 0; i < Material->TextureCount; ++i) {
        Stream->Read(&Header, 12);
        if (Header.Type == 6) {
            Stream->Read(&Header, 12);
            if (Header.Type != 1 || Header.Size != 4) { ErrorDetail = 4; goto BadHeader; }
            Material->Textures[i].Filter = Stream->ReadUInt();
            Stream->Read(&Header, 12);
            if (Header.Type != 2 || Header.Size > 32) { ErrorDetail = 5; goto BadHeader; }
            Stream->Read(Material->Textures[i].Name, Header.Size);
            Stream->Read(&Header, 12);
            if (Header.Type != 2 || Header.Size > 32) { ErrorDetail = 6; goto BadHeader; }
            Stream->Read(Material->Textures[i].Mask, Header.Size);
            Stream->Read(&Header, 12);
            if (Header.Type == 3) {
                if (Header.Size) Stream->Skip(Header.Size);
            }
            else if (Header.Size) { ErrorDetail = 7; goto BadHeader; }
        }
    }
    Stream->Read(&Header, 12);
    if (Header.Type != 3) { ErrorDetail = 8; goto BadHeader; }
    {
        int Remaining = Header.Size;
        while (Remaining) {
            Stream->Read(&Header, 12);
            switch (Header.Type) {
            case 0x253f2fc: case 0x253f2f6: case 0x120: case 0x135:
                Stream->Skip(Header.Size);
                break;
            default:
                if (!Error) Error = 0xabe2;
                ErrorDetail = Header.Type;
                free(Material);
                return false;
            }
            Remaining -= 12 + Header.Size;
        }
    }
    Materials.Add(Material);
    return true;
BadHeader:
    if (!Error) Error = 0xabe7;
    free(Material);
    return false;
}

// 0x00003b90
bool CGeometry::ReadExtensions(CMemoryStream* Stream)
{
    ExtensionChunk = ReadChunk(Stream);
    if (ExtensionChunk->Header.Type != 3) {
        if (!Error) Error = 0xabf2;
        ErrorDetail = 0;
        return false;
    }
    CMemoryStream* Data = &ExtensionChunk->Stream;
    while (Data->Size - Data->Position > 12) {
        RwChunkHeader Header;
        Data->Read(&Header, 12);
        switch (Header.Type) {
        case 0x50e:
            HasBinMesh = true;
            BinMeshFlags = Data->ReadUInt();
            SplitCount = Data->ReadUInt();
            IndexCount = Data->ReadUInt();
            if (SplitCount > 99999) { if (!Error) Error = 0xabe5; return false; }
            for (unsigned int i = 0; i < SplitCount; ++i) {
                RwSplit* Split = static_cast<RwSplit*>(calloc(1, sizeof(RwSplit)));
                Split->IndexCount = Data->ReadUInt();
                Split->Material = Data->ReadUInt();
                Split->Indices = static_cast<unsigned int*>(calloc(Split->IndexCount, 4));
                Data->Read(Split->Indices, Split->IndexCount * 4);
                Splits.Add(Split);
            }
            break;
        case 0x253f2fd:
            Data->Skip(Header.Size);
            break;
        case 0x253f2f9:
            if ((Header.Size - 4) / 4 == VertexCount) {
                HasNightColors = true;
                Data->Skip(4);
                NightColors = static_cast<RwColor*>(calloc(VertexCount, 4));
                if (!NightColors || Data->Size - Data->Position < VertexCount * 4) {
                    if (!Error) Error = 0xabe1;
                    ErrorDetail = 0x253f2f9;
                    return false;
                }
                Data->Read(NightColors, VertexCount * 4);
            }
            break;
        case 0x116:
            {
                unsigned int Start = Data->Position;
                if (!ReadSkin(Data) || Header.Size != Data->Position - Start) {
                    if (!Error) Error = 0xabe7;
                    ErrorDetail = 0x116;
                    return false;
                }
            }
            break;
        case 0x105: case 0x253f2f8:
            Data->Skip(Header.Size);
            break;
        default:
            if (!Error) Error = 0xabe2;
            ErrorDetail = Header.Type;
            return false;
        }
    }
    return true;
}

// 0x00003470
bool CGeometry::ReadSkin(CMemoryStream* Stream)
{
    if (static_cast<int>(VertexCount) < 1) return false;
    Stream->Read(&Skin.BoneCount, 1);
    Stream->Read(&Skin.UsedBoneCount, 1);
    Stream->Read(&Skin.MaxWeights, 1);
    Stream->Read(&Skin.Unused, 1);
    if (Skin.BoneCount >= 64 || Skin.UsedBoneCount > Skin.BoneCount) return false;
    Skin.Bones = static_cast<unsigned char*>(calloc(Skin.UsedBoneCount, 1));
    Stream->Read(Skin.Bones, Skin.UsedBoneCount);
    Skin.Indices = static_cast<unsigned int*>(calloc(VertexCount, 4));
    Stream->Read(Skin.Indices, VertexCount * 4);
    Skin.Weights = static_cast<float*>(calloc(VertexCount, 16));
    Stream->Read(Skin.Weights, VertexCount * 16);
    Skin.Matrices = static_cast<float*>(calloc(Skin.BoneCount, 64));
    Stream->Read(Skin.Matrices, Skin.BoneCount * 64);
    Skin.BoneLimit = Stream->ReadUInt();
    Skin.MeshCount = Stream->ReadUInt();
    Skin.RleCount = Stream->ReadUInt();
    return !Skin.BoneLimit && !Skin.MeshCount && !Skin.RleCount;
}

// 0x00003610
CRwChunk* CGeometry::WriteStruct()
{
    CRwChunk* Chunk = new CRwChunk;
    Chunk->Header.Type = 1;
    Chunk->Stream.Write(&Flags, 2);
    Chunk->Stream.Write(&Format, 2);
    Chunk->Stream.Write(&TriangleCount, 4);
    Chunk->Stream.Write(&VertexCount, 4);
    Chunk->Stream.Write(&MorphCount, 4);
    if ((Flags & 8) && HasColors) Chunk->Stream.Write(Colors, VertexCount * 4);
    if (Flags & 0x84) Chunk->Stream.Write(UV, VertexCount * 8);
    Chunk->Stream.Write(Triangles, TriangleCount * 8);
    Chunk->Stream.Write(&Center, 16);
    Chunk->Stream.Write(&HasVertices, 4);
    Chunk->Stream.Write(&HasNormals, 4);
    if (HasVertices && Vertices) Chunk->Stream.Write(Vertices, VertexCount * 12);
    if ((Flags & 0x10) && Normals) Chunk->Stream.Write(Normals, VertexCount * 12);
    Chunk->Header.Size = Chunk->Stream.Size;
    return Chunk;
}

// 0x000037a0
bool CGeometry::RemoveExtension(unsigned int Type)
{
    CRwChunk* Replacement = new CRwChunk;
    Replacement->Header.Type = 3;
    CMemoryStream* Data = &ExtensionChunk->Stream;
    Data->Position = 0;
    Data->End = false;
    while (Data->Size - Data->Position > 11) {
        RwChunkHeader Header;
        Data->Read(&Header, 12);
        if (Header.Type != Type) {
            Replacement->Stream.Write(&Header, 12);
            Replacement->Stream.Write(Data->Data + Data->Position, Header.Size);
        }
        Data->Skip(Header.Size);
    }
    Replacement->Header.Size = Replacement->Stream.Size;
    delete ExtensionChunk;
    ExtensionChunk = Replacement;
    return true;
}

// 0x000038d0
bool CGeometry::ImportNightColors(const CGeometry* Source)
{
    if (!Source->HasColors) return false;
    if (NightColors) { free(NightColors); NightColors = 0; }
    NightColors = static_cast<RwColor*>(calloc(VertexCount, 4));
    memcpy(NightColors, Colors, VertexCount * 4);
    int Count = static_cast<int>(Source->VertexCount);
    if (static_cast<int>(VertexCount) < Count) Count = VertexCount;
    memcpy(NightColors, Source->Colors, Count * 4);
    HasNightColors = true;
    Modified = true;
    return true;
}

// 0x00003950
CRwChunk* CGeometry::Write()
{
    CRwChunk* Chunk = new CRwChunk;
    Chunk->Header.Type = 0xf;
    CRwChunk* Struct = WriteStruct();
    if (Struct) { AppendChunk(Chunk, Struct); delete Struct; }
    if (MaterialChunk) AppendChunk(Chunk, MaterialChunk);
    if (NightColors) {
        CRwChunk* Night = new CRwChunk;
        Night->Header.Type = 0x253f2f9;
        unsigned int Marker = 0x37505050;
        Night->Stream.Write(&Marker, 4);
        Night->Stream.Write(NightColors, VertexCount * 4);
        Night->Header.Size = Night->Stream.Size;
        RemoveExtension(0x253f2f9);
        AppendChunk(ExtensionChunk, Night);
        delete Night;
    }
    if (ExtensionChunk) AppendChunk(Chunk, ExtensionChunk);
    return Chunk;
}

// 0x000010a0
bool GetGeometryBounds(const CGeometry* Geometry, CVector* Min, CVector* Max)
{
    Min->x = Min->y = Min->z = 100000.0f;
    Max->x = Max->y = Max->z = -100000.0f;
    if (!Geometry->Vertices) return false;
    for (unsigned int i = 0; i < Geometry->VertexCount; ++i) {
        const CVector& Vertex = Geometry->Vertices[i];
        if (Vertex.x < Min->x) Min->x = Vertex.x;
        if (Vertex.y < Min->y) Min->y = Vertex.y;
        if (Vertex.z < Min->z) Min->z = Vertex.z;
        if (Max->x < Vertex.x) Max->x = Vertex.x;
        if (Max->y < Vertex.y) Max->y = Vertex.y;
        if (Max->z < Vertex.z) Max->z = Vertex.z;
    }
    return true;
}

// 0x00003050
void LightColor(RwColor* Result, const RwColor* Color, const CVector* Normal,
    const CVector* Direction, float Minimum, float Maximum)
{
    if (Direction->x == 0 && Direction->y == 0 && Direction->z == 0) {
        Result->r = Color->r;
        Result->g = Color->g;
        Result->b = Color->b;
        return;
    }
    float Brightness = DotProduct(Normal, Direction) > 0 ? DotProduct(Normal, Direction) : 0;
    if (Brightness > Maximum) Brightness = Maximum;
    if (Brightness < Minimum) Brightness = Minimum;
    Result->r = static_cast<unsigned char>(static_cast<int>(Color->r * Brightness));
    Result->g = static_cast<unsigned char>(static_cast<int>(Color->g * Brightness));
    Result->b = static_cast<unsigned char>(static_cast<int>(Color->b * Brightness));
}

// 0x00003150
void CGeometry::CalculateNormals()
{
    unsigned int i;
    for (i = 0; i < TriangleCount; ++i) {
        const RwTriangle& Triangle = Triangles[i];
        CVector A, B, Normal;
        Subtract(&A, &Vertices[Triangle.Vertex1], &Vertices[Triangle.Vertex2]);
        Subtract(&B, &Vertices[Triangle.Vertex3], &Vertices[Triangle.Vertex2]);
        CrossProduct(&Normal, &A, &B);
        Add(&Normals[Triangle.Vertex1], &Normals[Triangle.Vertex1], &Normal);
        Add(&Normals[Triangle.Vertex2], &Normals[Triangle.Vertex2], &Normal);
        Add(&Normals[Triangle.Vertex3], &Normals[Triangle.Vertex3], &Normal);
    }
    for (i = 0; i < VertexCount; ++i) Normalize(&Normals[i], &Normals[i]);
}

// 0x00003260
bool CGeometry::GenerateNormals()
{
    if (!(Flags & 0x10) && !HasNormals && !Normals) {
        Flags |= 0x10;
        HasNormals = 1;
        Normals = static_cast<CVector*>(calloc(VertexCount, 12));
        CalculateNormals();
        return true;
    }
    return false;
}

// 0x000032b0
bool CGeometry::SetColors(const RwColor* Day, const RwColor* Night,
    const CVector* DayDirection, const CVector* NightDirection,
    float Minimum, float Maximum)
{
    Modified = true;
    Flags = (Flags & 0xffdf) | 8;
    if (Colors) { free(Colors); Colors = 0; }
    if (NightColors) { free(NightColors); NightColors = 0; }
    HasColors = HasNightColors = true;
    Colors = static_cast<RwColor*>(calloc(VertexCount, 4));
    NightColors = static_cast<RwColor*>(calloc(VertexCount, 4));
    unsigned int i;
    for (i = 0; i < VertexCount; ++i) {
        Colors[i] = *Day;
        if (HasNormals && DayDirection) {
            CVector Normal = Normals[i];
            LightColor(&Colors[i], Day, &Normal, DayDirection, Minimum, Maximum);
        }
    }
    for (i = 0; i < VertexCount; ++i) {
        NightColors[i] = *Night;
        if (HasNormals && NightDirection) {
            CVector Normal = Normals[i];
            LightColor(&NightColors[i], Night, &Normal, NightDirection, Minimum, Maximum);
        }
    }
    return true;
}
