#ifndef SAMP_MODEL_FILE_H
#define SAMP_MODEL_FILE_H
#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
// Descriptive names for the native model-file parser types.
#pragma pack(push, 1)
struct CModelFileStream
{
    // Borrowed buffer; scratch/compression fields start cleared.
    // 0x000b3630
    CModelFileStream(BYTE* buffer, DWORD size);
    // 0x000b3660
    float ReadFloat();
    // 0x000b3680
    int ReadInt();

    // 0x000bb040
    void SetBuffer(BYTE* buffer, DWORD size);
    // 0x000bb060
    DWORD WriteByte(BYTE value);
    // 0x000bb070
    WORD ReadWord();
    // 0x000bb090
    BYTE* GetBuffer();
    // 0x000bb0a0
    DWORD GetLength();
    BYTE* data;
    DWORD position, length;
    BYTE ended, owned;
    BYTE* scratch;
    DWORD scratchLength;
    BYTE compressed;
    // 0x0000b800
    CModelFileStream();
    // 0x0000b820
    ~CModelFileStream();
    // 0x000bc250
    int Compress();
    // 0x000bc300
    int Decompress(DWORD outputLength);
    // 0x000bc060
    DWORD WriteFileChunks(FILE* file);
    // 0x000bc0c0
    DWORD SaveFile(const char* filename);
    // 0x000bc100
    DWORD SaveHandle(FILE* file);
    // 0x000bc110
    bool CopyBuffer();
    // 0x000bc150
    DWORD Write(const void* input, DWORD count);
    // 0x000bc1d0
    DWORD Read(void* output, DWORD count);
    // 0x000bc390
    DWORD LoadFile(const char* filename);
    // Equivalent native forwarding entry at RVA 0x000b9590; original duplicate identity unknown.
    // 0x000bc450
    DWORD WriteDword(DWORD value);
    // 0x000bc460
    DWORD WriteWord(WORD value);
    // 0x000bc470
    DWORD ReadDword();
    // 0x000b36a0
    BYTE ReadByte();
    // 0x000bc490
    BYTE* Current();
    // 0x000bc4a0
    DWORD Remaining();
    // 0x000b36c0
    DWORD Skip(DWORD count);
    // 0x000b36f0
    void Reset();
    // Returns the raw BYTE EOF flag.
    // 0x000b3700
    BYTE HasEnded();
};
struct CModelFileChunk
{
    // 0x000bb0b0
    CModelFileChunk();
    // 0x000bb0e0
    void SetBuffer(BYTE* buffer, DWORD size);
    // 0x000bb100
    void UpdateLength();
    DWORD type, length;
    WORD build, version;
    CModelFileStream stream;
    // 0x0000b850
    ~CModelFileChunk();
    // 0x0000b8a0
    DWORD GetType();
    // 0x0000b8b0
    DWORD GetLength();
};
struct CTxdFile
{
    DWORD error;
    CModelFileStream stream;
    DWORD fileLength;
    CModelFileChunk *root, *structure;
    WORD textureCount, parsedCount;
    // 0x0000b8c0
    CTxdFile();
    // 0x0000b8f0
    ~CTxdFile();
    // 0x0000b970
    DWORD GetTextureCount();
    // 0x0000b980
    DWORD GetError();
    // 0x000bda10
    CModelFileChunk* ReadChunk(CModelFileStream* input);
    // 0x000bdb20
    DWORD ReadStructure(CModelFileChunk* chunk);
    // 0x000bdb60
    DWORD ReadTexture(CModelFileChunk* chunk);
    // 0x000bdd80
    DWORD ReadChildren(CModelFileChunk* chunk);
    // 0x000bde10
    DWORD LoadFile(const char* filename);
};
struct CModelGeometry;
struct CModelGeometryArray
{
    // samp.dll RVA: unknown (identical native array initializer entries; typed binding not established)
    CModelGeometryArray();
    // samp.dll RVA: unknown (identical native pointer-array getter entries; typed binding not established)
    CModelGeometry* Get(DWORD index);
    // samp.dll RVA: unknown (identical native array count entries; typed binding not established)
    DWORD GetCount();
    // 0x0000cf80
    void Clear();
    // 0x0000d610
    ~CModelGeometryArray();

    CModelGeometry** items;
    DWORD count;
    // 0x0000c630
    bool Resize(DWORD size);
    // 0x000bbe20
    DWORD Insert(CModelGeometry* geometry);
};
struct CModelMaterialArray
{
    // samp.dll RVA: unknown (identical native array initializer entries; typed binding not established)
    CModelMaterialArray();
    // samp.dll RVA: unknown (identical native pointer-array getter entries; typed binding not established)
    void* Get(DWORD index);
    // samp.dll RVA: unknown (identical native array count entries; typed binding not established)
    DWORD GetCount();
    // 0x0000cfa0
    void Clear();
    // 0x0000d5f0
    ~CModelMaterialArray();

    void** items;
    DWORD count;
    // 0x0000c550
    bool Resize(DWORD size);
    // 0x000bcdb0
    DWORD Insert(void* material);
};
struct CModelMeshArray
{
    // samp.dll RVA: unknown (identical native array initializer entries; typed binding not established)
    CModelMeshArray();
    // samp.dll RVA: unknown (identical native pointer-array getter entries; typed binding not established)
    BYTE* Get(DWORD index);
    // samp.dll RVA: unknown (identical native array count entries; typed binding not established)
    DWORD GetCount();
    // 0x0000cfb0
    void Clear();
    // 0x0000d600
    ~CModelMeshArray();

    BYTE** items;
    DWORD count;
    // 0x0000c5c0
    bool Resize(DWORD size);
    // 0x000bce10
    DWORD Insert(BYTE* mesh);
};
struct CModelMaterialRecord
{
    DWORD values[7];
    struct Texture
    {
        DWORD flags;
        char name[32], mask[32];
    } textures[2];
};
struct CModelSkin
{
    BYTE boneCount, usedBoneCount, maxWeights, unknown3;
    void *usedBones, *indices, *weights, *matrices;
    DWORD unknown14, unknown18, unknown1c;
    void *unknown20, *unknown24, *unknown28;
};
struct CModelGeometry
{
    // 0x000bc900
    CModelFileChunk* BuildStructure();
    // 0x000bca90
    bool RemoveExtension(DWORD type);
    // 0x000bcbc0
    bool CopySecondaryColors(const CModelGeometry* other);
    // 0x000bcc40
    CModelFileChunk* BuildChunk();
    BYTE modified, modifiedOther;
    DWORD error, detail;
    WORD flags, textureSets;
    int triangleCount, vertexCount, morphCount;
    BYTE hasColors, unknown1b;
    void *colors, *unknown20, *uv, *triangles;
    float sphere[4];
    DWORD hasVertices, hasNormals;
    void *vertices, *normals;
    CModelFileChunk *material, *extension;
    int materialCount;
    CModelMaterialArray materials;
    BYTE unknown60;
    DWORD unknown61, unknown65, unknown69;
    CModelMeshArray meshes;
    union
    {
        BYTE opaque75[44];
        CModelSkin skin;
    };
    // 0x000bbe80
    CModelGeometry();
    // 0x0000d620
    ~CModelGeometry();
    // 0x000bb110
    bool IsModified();
    // 0x000bb130
    DWORD GetError();
    // 0x000bb140
    DWORD GetDetail();
    // 0x000bc4b0
    void SetError(DWORD value);
    // 0x000bc4d0
    void SetDetail(DWORD value);
    // 0x000bc4e0
    int GetVertexCount();
    // 0x000bc4f0
    BYTE HasVertexColors();
    // 0x000bc500
    void* GetVertexColors();
    // Descriptive name: each input's first four bytes are repeated for all vertices.
    // 0x000bc510
    DWORD SetRepeatedVertexColors(const BYTE* first, const BYTE* second);
    // 0x000bc600
    CModelFileChunk* ReadChunk(CModelFileStream* input);
    // 0x000bd180
    bool ReadMaterial(CModelFileStream* input);
    // 0x000bd5f0
    bool ReadMaterials(CModelFileStream* input);
    // 0x000bc760
    bool ReadSkin(CModelFileStream* input);
    // 0x000bce70
    bool ReadExtensions(CModelFileStream* input);
    // 0x000bd700
    bool Read(CModelFileStream* input, WORD version);
};
struct CDffFile
{
    // 0x000bbcc0
    bool RebuildGeometryList();
    // 0x000bb640
    DWORD SaveFile(const char* filename);
    // 0x000bb820
    DWORD SaveCompressedFile(const char* filename);
    // 0x000bb150
    void SetError(DWORD value);
    // 0x000bb160
    void SetDetail(DWORD value);
    // 0x000bb940
    DWORD AppendChunk(CModelFileChunk* parent, const CModelFileChunk* child);
    // 0x000bb990
    DWORD AppendCollisionFile(const char* filename);
    DWORD error, detail;
    CModelFileStream stream;
    DWORD fileLength;
    BYTE hasCollision;
    CModelGeometryArray geometries;
    CModelFileChunk *root, *structure, *frames, *geometryList, *atomic, *extension;
    DWORD unknown44;
    void* frameData;
    // 0x0000d820
    CDffFile();
    // 0x0000d8f0
    ~CDffFile();
    // Native dereferences every indexed geometry without a null check.
    // 0x000bbc80
    bool IsModified();
    // 0x000bb170
    CModelFileChunk* ReadChunk(CModelFileStream* input);
    // 0x000bb280
    DWORD StoreFrames(CModelFileChunk* chunk);
    // 0x000bb2a0
    DWORD StoreGeometryList(CModelFileChunk* chunk);
    // 0x000bb2c0
    DWORD StoreAtomic(CModelFileChunk* chunk);
    // 0x000bb2e0
    DWORD StoreStructure(CModelFileChunk* chunk);
    // 0x000bb300
    DWORD StoreExtension(CModelFileChunk* chunk);
    // 0x000bb320
    DWORD ReadChildren(CModelFileChunk* chunk);
    // 0x000bb460
    DWORD LoadFile(const char* filename);
    // 0x000bb520
    DWORD LoadCollisionFile(const char* filename);
    // 0x000bbaa0
    DWORD VerifyFrameExtension(CModelFileChunk* chunk);
    // 0x000bbb30
    DWORD ReadFrames();
    // 0x000bbf00
    DWORD ReadGeometries();
};
// Collision conversion records; coordinates are signed fixed-point /128.
struct ModelCollisionVertex
{
    SHORT values[3];
};
struct ModelCollisionTriangle
{
    WORD vertices[3];
    BYTE surface, lighting;
};
struct ModelCollisionBounds
{
    float minimum[3], maximum[3];
};
struct ModelCollisionFaceGroup
{
    ModelCollisionBounds bounds;
    WORD first, last;
};
struct ModelCollisionBox
{
    ModelCollisionBounds bounds;
    BYTE data[4];
};
struct ModelCollisionSphere
{
    BYTE data[20];
};
// Observed collision-file view only. Full construction/allocation/ownership is
// not established; writer/group methods do not make this a complete owning type.
class CModelCollisionFile
{
public:
    BYTE nativeState[0xa0];
    // Returns1 even if the final file save fails; secondary API results ignored.
    // 0x000b9d50
    DWORD SaveFile(const char* filename, const char* modelName);
    // Reorders the existing triangle buffer and replaces the group pointer.
    // 0x000baec0
    DWORD BuildFaceGroups();
    // Pointer publication only; no copy, validation or cleanup on insert failure.
    // 0x000ba190
    DWORD AddSphere(ModelCollisionSphere* sphere);
    // 0x000ba1a0
    DWORD AddBox(ModelCollisionBox* box);
    // Creates one28-byte box from bounds; ignores bounds/insert failure results.
    // 0x000ba240
    DWORD BuildBoxFromGeometry(const CModelGeometry* geometry);
    // 0x000b95d0
    DWORD SetInitialHeaderSize();
    // Native entry does not consume this. Writes sentinel bounds before checking vertices.
    // 0x000b95e0
    DWORD CalculateGeometryBounds(const CModelGeometry* geometry, float* minimum, float* maximum);
    // 0x000b96e0
    DWORD SetGeometryBounds(const CModelGeometry* geometry);
    // 0x000b9710
    DWORD BuildGeometry(const CModelGeometry* geometry);
    // 0x000b98a0
    bool ContainsVertex(const ModelCollisionVertex* vertex, const ModelCollisionBounds* bounds);
    // Tests any vertex inside, not full triangle/box intersection.
    // 0x000b9950
    bool TriangleHasVertexInBounds(const ModelCollisionTriangle* triangle, const ModelCollisionBounds* bounds);
};
typedef std::vector<ModelCollisionTriangle> ModelCollisionTriangleVector;
struct CModelCollisionNode
{
    ModelCollisionBounds bounds;
    DWORD unknown18;
    CModelCollisionNode *left, *right;
    ModelCollisionTriangleVector triangles;
    // Leaves bounds, unknown18 and vector allocator byte untouched.
    // 0x000bab60
    CModelCollisionNode();
    // 0x000ba750
    ~CModelCollisionNode();
    // Asymmetric traversal assumes paired children; no added null-right guard.
    // 0x000b9d10
    void CountLeaves(DWORD* count);
    // 0x000ba320
    void WriteLeaves(CModelCollisionFile* file);
    // No added depth limit or progress guard; native recurses for child count>80.
    // 0x000babf0
    void Split(CModelCollisionFile* file);
};
// Native tail adapters establish boxes at78 and spheres at80. Original template
// identifiers and owning lifecycle remain unestablished; both arrays hold pointers.
struct CModelCollisionBoxArray
{
    void** items;
    DWORD count;
    // 0x000b9a40
    void* Get(DWORD index);
    // 0x000b9a60
    DWORD GetCount();
    // 0x000b9b70
    bool Resize(DWORD size);
    // 0x000ba040
    DWORD Insert(void* value);
};
struct CModelCollisionSphereArray
{
    void** items;
    DWORD count;
    // 0x000b9a70
    void* Get(DWORD index);
    // 0x000b9a90
    DWORD GetCount();
    // 0x000b9be0
    bool Resize(DWORD size);
    // 0x000ba0a0
    DWORD Insert(void* value);
};
// Incoming ECX is unused; original owner/name not established.
// 0x000b99c0
void __stdcall ModelCollisionMinVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value);
// 0x000b9a00
void __stdcall ModelCollisionMaxVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value);
#pragma pack(pop)

typedef char NativeCollisionVertexExtent[(sizeof(ModelCollisionVertex) == 6) ? 1 : -1];
typedef char NativeCollisionTriangleExtent[(sizeof(ModelCollisionTriangle) == 8) ? 1 : -1];
typedef char NativeCollisionBoundsExtent[(sizeof(ModelCollisionBounds) == 24) ? 1 : -1];
typedef char NativeCollisionGroupExtent[(sizeof(ModelCollisionFaceGroup) == 28) ? 1 : -1];
typedef char NativeCollisionBoxExtent[(sizeof(ModelCollisionBox) == 28) ? 1 : -1];
typedef char NativeCollisionSphereExtent[(sizeof(ModelCollisionSphere) == 20) ? 1 : -1];
typedef char NativeCollisionVectorExtent[(sizeof(ModelCollisionTriangleVector) == 16) ? 1 : -1];
typedef char NativeCollisionNodeExtent[(sizeof(CModelCollisionNode) == 0x34) ? 1 : -1];
typedef char NativeCollisionNodeLeftOffset[(offsetof(CModelCollisionNode, left) == 0x1c) ? 1 : -1];
typedef char NativeCollisionNodeVectorOffset[(offsetof(CModelCollisionNode, triangles) == 0x24) ? 1 : -1];
typedef char NativeCollisionArrayAExtent[(sizeof(CModelCollisionBoxArray) == 8) ? 1 : -1];
typedef char NativeCollisionArrayBExtent[(sizeof(CModelCollisionSphereArray) == 8) ? 1 : -1];
// 0x0011f8d8
extern const float g_modelCollisionVertexScale;
// 0x0011f8dc
extern const float g_modelCollisionVertexInvScale;
typedef char NativeModelStreamExtent[(sizeof(CModelFileStream) == 0x17) ? 1 : -1];
typedef char NativeModelChunkExtent[(sizeof(CModelFileChunk) == 0x23) ? 1 : -1];
typedef char NativeTxdFileExtent[(sizeof(CTxdFile) == 0x2b) ? 1 : -1];
typedef char NativeDffFileExtent[(sizeof(CDffFile) == 0x4c) ? 1 : -1];
typedef char NativeModelGeometryExtent[(sizeof(CModelGeometry) == 0xa1) ? 1 : -1];
typedef char NativeModelMaterialRecordExtent[(sizeof(CModelMaterialRecord) == 0xa4) ? 1 : -1];
typedef char NativeModelSkinExtent[(sizeof(CModelSkin) == 44) ? 1 : -1];
typedef char NativeModelGeometryMaterialsOffset[(offsetof(CModelGeometry, materials) == 0x58) ? 1 : -1];
typedef char NativeModelGeometryMeshesOffset[(offsetof(CModelGeometry, meshes) == 0x6d) ? 1 : -1];
typedef char NativeDffFramesOffset[(offsetof(CDffFile, frames) == 0x34) ? 1 : -1];
typedef char NativeModelStreamOwnedOffset[(offsetof(CModelFileStream, owned) == 0xd) ? 1 : -1];
// 0x00117c9c
extern const char g_customTxdVerifyError[];
// 0x00117c60
extern const char g_customTxdCountError[];
// 0x00117dac
extern const char g_customDffVerifyError[];
// 0x00117d68
extern const char g_customDffParseError[];
// 0x00155470
extern const BYTE g_modelLzmaProperties[5];
// Uses the selected LZMA backend and allocator; preserves the native stdcall adapter.
// 0x000c5040
int __stdcall ModelLzmaEncode(BYTE* destination, DWORD* destinationLength, const BYTE* source, DWORD sourceLength, BYTE* properties, DWORD* propertySize, int level,
                              DWORD dictionary, int lc, int lp, int pb, int fastBytes, int threads);
// Stack ABI is stdcall-compatible; incoming ECX is unused. Original owner/name unknown.
// 0x000bc710
DWORD __stdcall ModelAppendChunk(CModelFileChunk* parent, const CModelFileChunk* child);
// Incoming ECX is unused; original owner/name is not established.
// 0x000bb5a0
DWORD __stdcall ModelSaveChunk(const CModelFileChunk* chunk, const char* filename);
// Other model editing/writing paths remain under recovery.
#endif
