#include "collision.h"
#include <stdlib.h>
#include <string.h>

// 0x00002350
CColModel::CColModel() : Faces(0), FaceCount(0), Vertices(0), VertexCount(0), GroupCount(0)
{
    memset(&Header, 0, sizeof(Header));
    Header.Unknown1e = 0xffff;
    Header.Magic = 0x334c4f43;
    // Native constructor leaves Groups untouched while GroupCount is zero.
}

// 0x000023b0
CColModel::~CColModel()
{
    Spheres.Resize(0);
    Boxes.Resize(0);
    // Native destructor does not release face/vertex buffers or array payloads.
}

// 0x000011a0
bool CColModel::GenerateEmpty(const CGeometry* Geometry)
{
    GetGeometryBounds(Geometry, &Header.Min, &Header.Max);
    Header.Radius = Geometry->Radius;
    return true;
}

// 0x000011d0
bool CColModel::GenerateFaces(const CGeometry* Geometry)
{
    GetGeometryBounds(Geometry, &Header.Min, &Header.Max);
    Header.Radius = Geometry->Radius;
    if (!Geometry->HasVertices)
        return false;
    if (Faces)
    {
        free(Faces);
        Faces = 0;
    }
    Faces = static_cast<ColFace*>(calloc(Geometry->TriangleCount, 8));
    FaceCount = Geometry->TriangleCount;
    for (unsigned int i = 0; i < Geometry->TriangleCount; ++i)
    {
        Faces[i].a = Geometry->Triangles[i].Vertex3;
        Faces[i].b = Geometry->Triangles[i].Vertex1;
        Faces[i].c = Geometry->Triangles[i].Vertex2;
        Faces[i].Material = 0;
        Faces[i].Light = 0xff;
    }
    if (Vertices)
    {
        free(Vertices);
        Vertices = 0;
    }
    Vertices = static_cast<ColVertex*>(calloc(Geometry->VertexCount, 6));
    VertexCount = Geometry->VertexCount;
    for (unsigned int i = 0; i < Geometry->VertexCount; ++i)
    {
        // Native x87 product goes directly into CRT _ftol2 (truncate).
        Vertices[i].x = static_cast<short>(static_cast<int>(Geometry->Vertices[i].x * 128.0f));
        Vertices[i].y = static_cast<short>(static_cast<int>(Geometry->Vertices[i].y * 128.0f));
        Vertices[i].z = static_cast<short>(static_cast<int>(Geometry->Vertices[i].z * 128.0f));
    }
    return true;
}

// 0x000015c0
bool CColModel::GenerateBox(const CGeometry* Geometry)
{
    GetGeometryBounds(Geometry, &Header.Min, &Header.Max);
    Header.Radius = Geometry->Radius;
    ColBox* Box = static_cast<ColBox*>(calloc(1, sizeof(ColBox)));
    Box->Min = Header.Min;
    Box->Max = Header.Max;
    Box->Brightness = 0xff;
    Box->Light = 0xff;
    Box->Material = 0;
    Boxes.Add(Box);
    return true;
}

// 0x00001360
bool CColModel::Save(const char* FileName, const char* ModelName)
{
    CMemoryStream Output;
    strncpy(Header.Name, ModelName, 20);
    Header.Size = 0x74;
    Output.Write(&Header, 0x78);
    unsigned int i;
    unsigned int Count = Spheres.Count;
    if (Count)
    {
        unsigned int Offset = Output.Position - 4;
        Header.SphereCount = static_cast<unsigned short>(Count);
        for (i = 0; i < Count; ++i)
            if (Spheres.Get(i))
                Output.Write(Spheres.Get(i), 20);
        Header.Size += Output.Position - Offset - 4;
        Header.SphereOffset = Offset;
    }
    Count = Boxes.Count;
    if (Count)
    {
        unsigned int Offset = Output.Position - 4;
        Header.BoxCount = static_cast<unsigned short>(Count);
        for (i = 0; i < Count; ++i)
            if (Boxes.Get(i))
                Output.Write(Boxes.Get(i), 28);
        Header.Size += Output.Position - Offset - 4;
        Header.BoxOffset = Offset;
    }
    if (GroupCount)
    {
        unsigned int Start = Output.Position;
        Header.Flags |= 8;
        Output.Write(Groups, GroupCount * 28);
        Header.Size += Output.Position - Start;
        Output.Write(&GroupCount, 4);
        Header.Size += 4;
    }
    if (FaceCount)
    {
        Header.FaceCount = static_cast<unsigned short>(FaceCount);
        unsigned int Offset = Output.Position - 4;
        Output.Write(Faces, FaceCount * 8);
        Header.FaceOffset = Offset;
        Header.Size += Output.Position - Offset - 4;
    }
    if (VertexCount)
    {
        unsigned int Offset = Output.Position - 4;
        Output.Write(Vertices, VertexCount * 6);
        Header.VertexOffset = Offset;
        Header.Size += Output.Position - Offset - 4;
    }
    Output.Position = 0;
    Output.End = false;
    Output.Write(&Header, 0x78);
    Output.Save(FileName);
    // Original writer ignores the underlying file-save result.
    return true;
}
