#ifndef MODELFILE_H
#define MODELFILE_H

#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <windows.h>

#include <static_assert.h>

#pragma pack(push, 1)
struct CModelFileStream
{
    // Borrowed buffer; scratch/compression fields start cleared.
    // R5 address to verify
    CModelFileStream(BYTE* buffer, DWORD size);
    // R5 address to verify
    float ReadFloat();
    // R5 address to verify
    int ReadInt();

    // R5 address to verify
    void SetBuffer(BYTE* buffer, DWORD size);
    // R5 address to verify
    DWORD WriteByte(BYTE value);
    // R5 address to verify
    WORD ReadWord();
    // R5 address to verify
    BYTE* GetBuffer();
    // R5 address to verify
    DWORD GetLength();
    BYTE* data;
    DWORD position, length;
    BYTE ended, owned;
    BYTE* scratch;
    DWORD scratchLength;
    BYTE compressed;
    // R5 address to verify
    CModelFileStream();
    // 0x0000bb40
    ~CModelFileStream();
    // 0x000ba850
    int Compress();
    // 0x000ba900
    int Decompress(DWORD outputLength);
    // 0x000ba660
    DWORD WriteFileChunks(FILE* file);
    // 0x000ba6c0
    DWORD SaveFile(const char* filename);
    // R5 address to verify
    DWORD SaveHandle(FILE* file);
    // 0x000ba710
    bool CopyBuffer();
    // 0x000ba750
    DWORD Write(const void* input, DWORD count);
    // 0x000ba7d0
    DWORD Read(void* output, DWORD count);
    // 0x000ba990
    DWORD LoadFile(const char* filename);
    // Equivalent native forwarding entry at RVA 0x000b9590; original duplicate identity unknown.
    // R5 address to verify
    DWORD WriteDword(DWORD value);
    // R5 address to verify
    DWORD WriteWord(WORD value);
    // 0x000baa70
    DWORD ReadDword();
    // 0x000b3aa0
    BYTE ReadByte();
    // R5 address to verify
    BYTE* Current();
    // R5 address to verify
    DWORD Remaining();
    // 0x000b3ac0
    DWORD Skip(DWORD count);
    // R5 address to verify
    void Reset();
    // Returns the raw BYTE EOF flag.
    // R5 address to verify
    BYTE HasEnded();
};
struct CModelFileChunk
{
    // 0x000b9570
    CModelFileChunk();
    // R5 address to verify
    void SetBuffer(BYTE* buffer, DWORD size);
    // R5 address to verify
    void UpdateLength();
    DWORD type, length;
    WORD build, version;
    CModelFileStream stream;
    // 0x0000bb70
    ~CModelFileChunk();
    // R5 address to verify
    DWORD GetType();
    // R5 address to verify
    DWORD GetLength();
};
struct CTxdFile
{
    DWORD error;
    CModelFileStream stream;
    DWORD fileLength;
    CModelFileChunk *root, *structure;
    WORD textureCount, parsedCount;
    // R5 address to verify
    CTxdFile();
    // 0x0000bc10
    ~CTxdFile();
    // R5 address to verify
    DWORD GetTextureCount();
    // R5 address to verify
    DWORD GetError();
    // 0x000bc340
    CModelFileChunk* ReadChunk(CModelFileStream* input);
    // 0x000bc450
    DWORD ReadStructure(CModelFileChunk* chunk);
    // 0x000bc490
    DWORD ReadTexture(CModelFileChunk* chunk);
    // 0x000bc6b0
    DWORD ReadChildren(CModelFileChunk* chunk);
    // 0x000bc740
    DWORD LoadFile(const char* filename);
};
struct CModelGeometry;
struct CModelGeometryArray
{
    // inline
    CModelGeometryArray();
    // inline
    CModelGeometry* Get(DWORD index);
    // inline
    DWORD GetCount();
    // R5 address to verify
    void Clear();
    // R5 address to verify
    ~CModelGeometryArray();

    CModelGeometry** items;
    DWORD count;
    // 0x0000cac0
    bool Resize(DWORD size);
    // R5 address to verify
    DWORD Insert(CModelGeometry* geometry);
};
struct CModelMaterialArray
{
    // inline
    CModelMaterialArray();
    // inline
    void* Get(DWORD index);
    // inline
    DWORD GetCount();
    // R5 address to verify
    void Clear();
    // R5 address to verify
    ~CModelMaterialArray();

    void** items;
    DWORD count;
    // 0x0000c9e0
    bool Resize(DWORD size);
    // 0x000bb6e0
    DWORD Insert(void* material);
};
struct CModelMeshArray
{
    // inline
    CModelMeshArray();
    // inline
    BYTE* Get(DWORD index);
    // inline
    DWORD GetCount();
    // R5 address to verify
    void Clear();
    // R5 address to verify
    ~CModelMeshArray();

    BYTE** items;
    DWORD count;
    // 0x0000ca50
    bool Resize(DWORD size);
    // 0x000bb740
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
    // 0x000bb230
    CModelFileChunk* BuildStructure();
    // 0x000bb3c0
    bool RemoveExtension(DWORD type);
    // R5 address to verify
    bool CopySecondaryColors(const CModelGeometry* other);
    // 0x000bb570
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
    // 0x000ba340
    CModelGeometry();
    // 0x0000dbb0
    ~CModelGeometry();
    // R5 address to verify
    bool IsModified();
    // R5 address to verify
    DWORD GetError();
    // R5 address to verify
    DWORD GetDetail();
    // R5 address to verify
    void SetError(DWORD value);
    // R5 address to verify
    void SetDetail(DWORD value);
    // R5 address to verify
    int GetVertexCount();
    // R5 address to verify
    BYTE HasVertexColors();
    // R5 address to verify
    void* GetVertexColors();
    // R5 address to verify
    DWORD SetRepeatedVertexColors(const BYTE* first, const BYTE* second);
    // 0x000baf30
    CModelFileChunk* ReadChunk(CModelFileStream* input);
    // 0x000bbab0
    bool ReadMaterial(CModelFileStream* input);
    // 0x000bbf20
    bool ReadMaterials(CModelFileStream* input);
    // 0x000bb090
    bool ReadSkin(CModelFileStream* input);
    // 0x000bb7a0
    bool ReadExtensions(CModelFileStream* input);
    // 0x000bc030
    bool Read(CModelFileStream* input, WORD version);
};
struct CDffFile
{
    // 0x000ba180
    bool RebuildGeometryList();
    // 0x000b9b00
    DWORD SaveFile(const char* filename);
    // 0x000b9ce0
    DWORD SaveCompressedFile(const char* filename);
    // R5 address to verify
    void SetError(DWORD value);
    // R5 address to verify
    void SetDetail(DWORD value);
    // 0x000b9e00
    DWORD AppendChunk(CModelFileChunk* parent, const CModelFileChunk* child);
    // 0x000b9e50
    DWORD AppendCollisionFile(const char* filename);
    DWORD error, detail;
    CModelFileStream stream;
    DWORD fileLength;
    BYTE hasCollision;
    CModelGeometryArray geometries;
    CModelFileChunk *root, *structure, *frames, *geometryList, *atomic, *extension;
    DWORD unknown44;
    void* frameData;
    // R5 address to verify
    CDffFile();
    // 0x0000de80
    ~CDffFile();
    // Native dereferences every indexed geometry without a null check.
    // R5 address to verify
    bool IsModified();
    // 0x000b9630
    CModelFileChunk* ReadChunk(CModelFileStream* input);
    // 0x000b9740
    DWORD StoreFrames(CModelFileChunk* chunk);
    // 0x000b9760
    DWORD StoreGeometryList(CModelFileChunk* chunk);
    // 0x000b9780
    DWORD StoreAtomic(CModelFileChunk* chunk);
    // 0x000b97a0
    DWORD StoreStructure(CModelFileChunk* chunk);
    // 0x000b97c0
    DWORD StoreExtension(CModelFileChunk* chunk);
    // 0x000b97e0
    DWORD ReadChildren(CModelFileChunk* chunk);
    // 0x000b9920
    DWORD LoadFile(const char* filename);
    // R5 address to verify
    DWORD LoadCollisionFile(const char* filename);
    // 0x000b9f60
    DWORD VerifyFrameExtension(CModelFileChunk* chunk);
    // 0x000b9ff0
    DWORD ReadFrames();
    // R5 address to verify
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
    // 0x000b9150
    DWORD SaveFile(const char* filename, const char* modelName);
    // Reorders the existing triangle buffer and replaces the group pointer.
    // R5 address to verify
    DWORD BuildFaceGroups();
    // Pointer publication only; no copy, validation or cleanup on insert failure.
    // R5 address to verify
    DWORD AddSphere(ModelCollisionSphere* sphere);
    // R5 address to verify
    DWORD AddBox(ModelCollisionBox* box);
    // Creates one28-byte box from bounds; ignores bounds/insert failure results.
    // R5 address to verify
    DWORD BuildBoxFromGeometry(const CModelGeometry* geometry);
    // 0x000b8b90
    DWORD SetInitialHeaderSize();
    // Native entry does not consume this. Writes sentinel bounds before checking vertices.
    // 0x000b8ba0
    DWORD CalculateGeometryBounds(const CModelGeometry* geometry, float* minimum, float* maximum);
    // 0x000b8ca0
    DWORD SetGeometryBounds(const CModelGeometry* geometry);
    // 0x000b8cd0
    DWORD BuildGeometry(const CModelGeometry* geometry);
    // 0x000b8e70
    bool ContainsVertex(const ModelCollisionVertex* vertex, const ModelCollisionBounds* bounds);
    // Tests any vertex inside, not full triangle/box intersection.
    // 0x000b8f20
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
    // R5 address to verify
    CModelCollisionNode();
    // R5 address to verify
    ~CModelCollisionNode();
    // Asymmetric traversal assumes paired children; no added null-right guard.
    // R5 address to verify
    void CountLeaves(DWORD* count);
    // R5 address to verify
    void WriteLeaves(CModelCollisionFile* file);
    // No added depth limit or progress guard; native recurses for child count>80.
    // R5 address to verify
    void Split(CModelCollisionFile* file);
};
// Native tail adapters establish boxes at78 and spheres at80. Original template
// identifiers and owning lifecycle remain unestablished; both arrays hold pointers.
struct CModelCollisionBoxArray
{
    void** items;
    DWORD count;
    // R5 address to verify
    void* Get(DWORD index);
    // R5 address to verify
    DWORD GetCount();
    // R5 address to verify
    bool Resize(DWORD size);
    // R5 address to verify
    DWORD Insert(void* value);
};
struct CModelCollisionSphereArray
{
    void** items;
    DWORD count;
    // R5 address to verify
    void* Get(DWORD index);
    // R5 address to verify
    DWORD GetCount();
    // R5 address to verify
    bool Resize(DWORD size);
    // R5 address to verify
    DWORD Insert(void* value);
};
// Does not use incoming ECX.
// 0x000b8f90
void __stdcall ModelCollisionMinVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value);
// 0x000b8fd0
void __stdcall ModelCollisionMaxVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value);
#pragma pack(pop)

STATIC_ASSERT(sizeof(ModelCollisionVertex) == 6);
STATIC_ASSERT(sizeof(ModelCollisionTriangle) == 8);
STATIC_ASSERT(sizeof(ModelCollisionBounds) == 24);
STATIC_ASSERT(sizeof(ModelCollisionFaceGroup) == 28);
STATIC_ASSERT(sizeof(ModelCollisionBox) == 28);
STATIC_ASSERT(sizeof(ModelCollisionSphere) == 20);
STATIC_ASSERT(sizeof(ModelCollisionTriangleVector) == 16);
STATIC_ASSERT(sizeof(CModelCollisionNode) == 0x34);
STATIC_ASSERT(offsetof(CModelCollisionNode, left) == 0x1c);
STATIC_ASSERT(offsetof(CModelCollisionNode, triangles) == 0x24);
STATIC_ASSERT(sizeof(CModelCollisionBoxArray) == 8);
STATIC_ASSERT(sizeof(CModelCollisionSphereArray) == 8);
// R5 address to verify
extern const float g_modelCollisionVertexScale;
// R5 address to verify
extern const float g_modelCollisionVertexInvScale;
STATIC_ASSERT(sizeof(CModelFileStream) == 0x17);
STATIC_ASSERT(sizeof(CModelFileChunk) == 0x23);
STATIC_ASSERT(sizeof(CTxdFile) == 0x2b);
STATIC_ASSERT(sizeof(CDffFile) == 0x4c);
STATIC_ASSERT(sizeof(CModelGeometry) == 0xa1);
STATIC_ASSERT(sizeof(CModelMaterialRecord) == 0xa4);
STATIC_ASSERT(sizeof(CModelSkin) == 44);
STATIC_ASSERT(offsetof(CModelGeometry, materials) == 0x58);
STATIC_ASSERT(offsetof(CModelGeometry, meshes) == 0x6d);
STATIC_ASSERT(offsetof(CDffFile, frames) == 0x34);
STATIC_ASSERT(offsetof(CModelFileStream, owned) == 0xd);
// 0x000e5c6c
extern const char g_customTxdVerifyError[];
// 0x000e5c30
extern const char g_customTxdCountError[];
// 0x000e5ddc
extern const char g_customDffVerifyError[];
// 0x000e5d98
extern const char g_customDffParseError[];
// R5 address to verify
extern const BYTE g_modelLzmaProperties[5];
// Uses the selected LZMA backend and allocator; preserves the native stdcall adapter.
// 0x000c3970
int __stdcall ModelLzmaEncode(BYTE* destination, DWORD* destinationLength, const BYTE* source, DWORD sourceLength, BYTE* properties, DWORD* propertySize, int level,
                              DWORD dictionary, int lc, int lp, int pb, int fastBytes, int threads);
// Stack ABI is stdcall-compatible; incoming ECX is unused. Original owner/name unknown.
// 0x000bb040
DWORD __stdcall ModelAppendChunk(CModelFileChunk* parent, const CModelFileChunk* child);
// Does not use incoming ECX.
// 0x000b9a60
DWORD __stdcall ModelSaveChunk(const CModelFileChunk* chunk, const char* filename);
// Other model editing/writing paths remain under recovery.
// 0x000b6ad0
void GameParseStreamData(const void* data, DWORD length, void* destination);

#endif // MODELFILE_H
