#ifndef KDFF_COLLISION_H
#define KDFF_COLLISION_H

#include "geometry.h"

#pragma pack(push, 1)
struct ColHeader
{
    unsigned int Magic, Size;
    char Name[20];
    unsigned short ModelId, Unknown1e;
    CVector Min, Max, Center;
    float Radius;
    unsigned short SphereCount, BoxCount, FaceCount;
    unsigned char LineCount, Unknown4f;
    unsigned int Flags;
    unsigned int SphereOffset, BoxOffset, LineOffset, VertexOffset, FaceOffset, PlaneOffset;
    unsigned int ShadowFaceCount, ShadowVertexOffset, ShadowFaceOffset;
};
struct ColFace
{
    unsigned short a, b, c;
    unsigned char Material, Light;
};
struct ColVertex
{
    short x, y, z;
};
struct ColBox
{
    CVector Min, Max;
    unsigned char Material, Flags, Brightness, Light;
};
class CColModel
{
public:
    ColHeader Header;
    CPointerArray Boxes, Spheres;
    ColFace* Faces;
    unsigned int FaceCount;
    ColVertex* Vertices;
    unsigned int VertexCount;
    void* Groups;
    unsigned int GroupCount;

    CColModel();

    ~CColModel();

    bool GenerateEmpty(const CGeometry* Geometry);

    bool GenerateFaces(const CGeometry* Geometry);

    bool GenerateBox(const CGeometry* Geometry);

    bool Save(const char* FileName, const char* ModelName);
};
#pragma pack(pop)

#endif
