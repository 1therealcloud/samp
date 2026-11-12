#ifndef KDFF_GEOMETRY_H
#define KDFF_GEOMETRY_H

#include "stream.h"
#include "vector.h"

#pragma pack(push, 1)
struct RwTriangle
{
    unsigned short Vertex1, Vertex2, Material, Vertex3;
};
struct RwColor
{
    unsigned char r, g, b, a;
};
struct RwUV
{
    float u, v;
};
struct RwTexture
{
    unsigned int Filter;
    char Name[32], Mask[32];
};
struct RwMaterial
{
    unsigned int Flags;
    RwColor Color;
    unsigned int Unused, TextureCount;
    float Surface[3];
    RwTexture Textures[2];
};
struct RwSplit
{
    unsigned int IndexCount, Material;
    unsigned int* Indices;
};
struct RwSkin
{
    unsigned char BoneCount, UsedBoneCount, MaxWeights, Unused;
    unsigned char* Bones;
    unsigned int* Indices;
    float* Weights;
    float* Matrices;
    unsigned int BoneLimit, MeshCount, RleCount;
    void* Unknown20;
    void* Unknown24;
    void* Unknown28;
};

// Instance size0xa1; field names recovered descriptively from reads and writes.
class CGeometry
{
public:
    bool Modified, Unknown01;
    unsigned int Error, ErrorDetail;
    unsigned short Flags, Format;
    unsigned int TriangleCount, VertexCount, MorphCount;
    bool HasColors, HasNightColors;
    RwColor* Colors;
    RwColor* NightColors;
    RwUV* UV;
    RwTriangle* Triangles;
    CVector Center;
    float Radius;
    unsigned int HasVertices, HasNormals;
    CVector* Vertices;
    CVector* Normals;
    CRwChunk* MaterialChunk;
    CRwChunk* ExtensionChunk;
    unsigned int MaterialCount;
    CPointerArray Materials;
    bool HasBinMesh;
    unsigned int BinMeshFlags, SplitCount, IndexCount;
    CPointerArray Splits;
    RwSkin Skin;

    // 0x00001ee0
    CGeometry();
    // 0x00001f60
    ~CGeometry();
    // 0x00004420
    bool Read(CMemoryStream* Stream, unsigned short Version);
    // 0x00004310
    bool ReadMaterials(CMemoryStream* Stream);
    // 0x00003ea0
    bool ReadMaterial(CMemoryStream* Stream);
    // 0x00003b90
    bool ReadExtensions(CMemoryStream* Stream);
    // 0x00003470
    bool ReadSkin(CMemoryStream* Stream);
    // 0x00003610
    CRwChunk* WriteStruct();
    // 0x000037a0
    bool RemoveExtension(unsigned int Type);
    // 0x000038d0
    bool ImportNightColors(const CGeometry* Source);
    // 0x00003950
    CRwChunk* Write();
#ifdef KDFF_DLL
    // 0x00003150
    void CalculateNormals();
    // 0x00003260
    bool GenerateNormals();
    // 0x000032b0
    bool SetColors(const RwColor* Day, const RwColor* Night, const CVector* DayDirection, const CVector* NightDirection, float Minimum, float Maximum);
#endif
};
#pragma pack(pop)

// 0x000010a0
bool GetGeometryBounds(const CGeometry* Geometry, CVector* Min, CVector* Max);
#ifdef KDFF_DLL
// 0x00003050
void LightColor(RwColor* Result, const RwColor* Color, const CVector* Normal, const CVector* Direction, float Minimum, float Maximum);
#endif

#endif
