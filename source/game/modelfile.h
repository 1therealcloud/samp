#ifndef MODELFILE_H
#define MODELFILE_H

#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <windows.h>

#pragma pack(push, 1)
struct CModelFileStream
{
    // Borrowed buffer; scratch/compression fields start cleared.

    CModelFileStream(BYTE* buffer, DWORD size);

    float ReadFloat();

    int ReadInt();

    void SetBuffer(BYTE* buffer, DWORD size);

    DWORD WriteByte(BYTE value);

    WORD ReadWord();

    BYTE* GetBuffer();

    DWORD GetLength();
    BYTE* data;
    DWORD position, length;
    BYTE ended, owned;
    BYTE* scratch;
    DWORD scratchLength;
    BYTE compressed;

    CModelFileStream();

    ~CModelFileStream();

    int Compress();

    int Decompress(DWORD outputLength);

    DWORD WriteFileChunks(FILE* file);

    DWORD SaveFile(const char* filename);

    DWORD SaveHandle(FILE* file);

    bool CopyBuffer();

    DWORD Write(const void* input, DWORD count);

    DWORD Read(void* output, DWORD count);

    DWORD LoadFile(const char* filename);
    // Equivalent native forwarding entry at RVA 0x000b9590; original duplicate identity unknown.

    DWORD WriteDword(DWORD value);

    DWORD WriteWord(WORD value);

    DWORD ReadDword();

    BYTE ReadByte();

    BYTE* Current();

    DWORD Remaining();

    DWORD Skip(DWORD count);

    void Reset();
    // Returns the raw BYTE EOF flag.

    BYTE HasEnded();
};
struct CModelFileChunk
{
    CModelFileChunk();

    void SetBuffer(BYTE* buffer, DWORD size);

    void UpdateLength();
    DWORD type, length;
    WORD build, version;
    CModelFileStream stream;

    ~CModelFileChunk();

    DWORD GetType();

    DWORD GetLength();
};
struct CTxdFile
{
    DWORD error;
    CModelFileStream stream;
    DWORD fileLength;
    CModelFileChunk *root, *structure;
    WORD textureCount, parsedCount;

    CTxdFile();

    ~CTxdFile();

    DWORD GetTextureCount();

    DWORD GetError();

    CModelFileChunk* ReadChunk(CModelFileStream* input);

    DWORD ReadStructure(CModelFileChunk* chunk);

    DWORD ReadTexture(CModelFileChunk* chunk);

    DWORD ReadChildren(CModelFileChunk* chunk);

    DWORD LoadFile(const char* filename);
};
struct CModelGeometry;
struct CModelGeometryArray
{
    CModelGeometryArray();
    CModelGeometry* Get(DWORD index);
    DWORD GetCount();

    void Clear();

    ~CModelGeometryArray();

    CModelGeometry** items;
    DWORD count;

    bool Resize(DWORD size);

    DWORD Insert(CModelGeometry* geometry);
};
struct CModelMaterialArray
{
    CModelMaterialArray();
    void* Get(DWORD index);
    DWORD GetCount();

    void Clear();

    ~CModelMaterialArray();

    void** items;
    DWORD count;

    bool Resize(DWORD size);

    DWORD Insert(void* material);
};
struct CModelMeshArray
{
    CModelMeshArray();
    BYTE* Get(DWORD index);
    DWORD GetCount();

    void Clear();

    ~CModelMeshArray();

    BYTE** items;
    DWORD count;

    bool Resize(DWORD size);

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
    CModelFileChunk* BuildStructure();

    bool RemoveExtension(DWORD type);

    bool CopySecondaryColors(const CModelGeometry* other);

    CModelFileChunk* BuildChunk();
    BYTE modified, modifiedOther;
    DWORD error, detail;
    WORD flags, textureSets;
    int triangleCount, vertexCount, morphCount;
    BYTE hasColors, hasSecondaryColors;
    void *colors, *secondaryColors, *uv, *triangles;
    float sphere[4];
    DWORD hasVertices, hasNormals;
    void *vertices, *normals;
    CModelFileChunk *material, *extension;
    int materialCount;
    CModelMaterialArray materials;
    BYTE hasBinMesh;
    DWORD binMeshFlags, binMeshCount, binMeshIndexCount;
    CModelMeshArray meshes;
    union
    {
        BYTE opaque75[44];
        CModelSkin skin;
    };

    CModelGeometry();

    ~CModelGeometry();

    bool IsModified();

    DWORD GetError();

    DWORD GetDetail();

    void SetError(DWORD value);

    void SetDetail(DWORD value);

    int GetVertexCount();

    BYTE HasVertexColors();

    void* GetVertexColors();

    DWORD SetRepeatedVertexColors(const BYTE* first, const BYTE* second);

    CModelFileChunk* ReadChunk(CModelFileStream* input);

    bool ReadMaterial(CModelFileStream* input);

    bool ReadMaterials(CModelFileStream* input);

    bool ReadSkin(CModelFileStream* input);

    bool ReadExtensions(CModelFileStream* input);

    bool Read(CModelFileStream* input, WORD version);
};
struct CDffFile
{
    bool RebuildGeometryList();

    DWORD SaveFile(const char* filename);

    DWORD SaveCompressedFile(const char* filename);

    void SetError(DWORD value);

    void SetDetail(DWORD value);

    DWORD AppendChunk(CModelFileChunk* parent, const CModelFileChunk* child);

    DWORD AppendCollisionFile(const char* filename);
    DWORD error, detail;
    CModelFileStream stream;
    DWORD fileLength;
    BYTE hasCollision;
    CModelGeometryArray geometries;
    CModelFileChunk *root, *structure, *frames, *geometryList, *atomic, *extension;
    DWORD frameCount;
    void* frameData;

    CDffFile();

    ~CDffFile();
    // Native dereferences every indexed geometry without a null check.

    bool IsModified();

    CModelFileChunk* ReadChunk(CModelFileStream* input);

    DWORD StoreFrames(CModelFileChunk* chunk);

    DWORD StoreGeometryList(CModelFileChunk* chunk);

    DWORD StoreAtomic(CModelFileChunk* chunk);

    DWORD StoreStructure(CModelFileChunk* chunk);

    DWORD StoreExtension(CModelFileChunk* chunk);

    DWORD ReadChildren(CModelFileChunk* chunk);

    DWORD LoadFile(const char* filename);

    DWORD LoadCollisionFile(const char* filename);

    DWORD VerifyFrameExtension(CModelFileChunk* chunk);

    DWORD ReadFrames();

    DWORD ReadGeometries();
    // not native, checks the atomic indices against the frames and geometries
    DWORD ReadAtomic();
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

    DWORD SaveFile(const char* filename, const char* modelName);
    // Reorders the existing triangle buffer and replaces the group pointer.

    DWORD BuildFaceGroups();
    // Pointer publication only; no copy, validation or cleanup on insert failure.

    DWORD AddSphere(ModelCollisionSphere* sphere);

    DWORD AddBox(ModelCollisionBox* box);
    // Creates one28-byte box from bounds; ignores bounds/insert failure results.

    DWORD BuildBoxFromGeometry(const CModelGeometry* geometry);

    DWORD SetInitialHeaderSize();
    // Native entry does not consume this. Writes sentinel bounds before checking vertices.

    DWORD CalculateGeometryBounds(const CModelGeometry* geometry, float* minimum, float* maximum);

    DWORD SetGeometryBounds(const CModelGeometry* geometry);

    DWORD BuildGeometry(const CModelGeometry* geometry);

    bool ContainsVertex(const ModelCollisionVertex* vertex, const ModelCollisionBounds* bounds);
    // Tests any vertex inside, not full triangle/box intersection.

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

    CModelCollisionNode();

    ~CModelCollisionNode();
    // Asymmetric traversal assumes paired children; no added null-right guard.

    void CountLeaves(DWORD* count);

    void WriteLeaves(CModelCollisionFile* file);
    // No added depth limit or progress guard; native recurses for child count>80.

    void Split(CModelCollisionFile* file);
};
// Native tail adapters establish boxes at78 and spheres at80. Original template
// identifiers and owning lifecycle remain unestablished; both arrays hold pointers.
struct CModelCollisionBoxArray
{
    void** items;
    DWORD count;

    void* Get(DWORD index);

    DWORD GetCount();

    bool Resize(DWORD size);

    DWORD Insert(void* value);
};
struct CModelCollisionSphereArray
{
    void** items;
    DWORD count;

    void* Get(DWORD index);

    DWORD GetCount();

    bool Resize(DWORD size);

    DWORD Insert(void* value);
};
// Does not use incoming ECX.

void __stdcall ModelCollisionMinVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value);

void __stdcall ModelCollisionMaxVertex(ModelCollisionVertex* target, const ModelCollisionVertex* value);
#pragma pack(pop)

static_assert(sizeof(ModelCollisionVertex) == 6);
static_assert(sizeof(ModelCollisionTriangle) == 8);
static_assert(sizeof(ModelCollisionBounds) == 24);
static_assert(sizeof(ModelCollisionFaceGroup) == 28);
static_assert(sizeof(ModelCollisionBox) == 28);
static_assert(sizeof(ModelCollisionSphere) == 20);
//static_assert(sizeof(ModelCollisionTriangleVector) == 16);
//static_assert(sizeof(CModelCollisionNode) == 0x34);
static_assert(offsetof(CModelCollisionNode, left) == 0x1c);
static_assert(offsetof(CModelCollisionNode, triangles) == 0x24);
static_assert(sizeof(CModelCollisionBoxArray) == 8);
static_assert(sizeof(CModelCollisionSphereArray) == 8);

extern const float g_modelCollisionVertexScale;

extern const float g_modelCollisionVertexInvScale;
static_assert(sizeof(CModelFileStream) == 0x17);
static_assert(sizeof(CModelFileChunk) == 0x23);
static_assert(sizeof(CTxdFile) == 0x2b);
static_assert(sizeof(CDffFile) == 0x4c);
static_assert(sizeof(CModelGeometry) == 0xa1);
static_assert(sizeof(CModelMaterialRecord) == 0xa4);
static_assert(sizeof(CModelSkin) == 44);
static_assert(offsetof(CModelGeometry, materials) == 0x58);
static_assert(offsetof(CModelGeometry, meshes) == 0x6d);
static_assert(offsetof(CDffFile, frames) == 0x34);
static_assert(offsetof(CModelFileStream, owned) == 0xd);

extern const char g_customTxdVerifyError[];

extern const char g_customTxdCountError[];

extern const char g_customDffVerifyError[];

extern const char g_customDffParseError[];

extern const BYTE g_modelLzmaProperties[5];
// Uses the selected LZMA backend and allocator; preserves the native stdcall adapter.

int __stdcall ModelLzmaEncode(BYTE* destination, DWORD* destinationLength, const BYTE* source, DWORD sourceLength, BYTE* properties, DWORD* propertySize, int level,
                              DWORD dictionary, int lc, int lp, int pb, int fastBytes, int threads);
// Stack ABI is stdcall-compatible; incoming ECX is unused. Original owner/name unknown.

DWORD __stdcall ModelAppendChunk(CModelFileChunk* parent, const CModelFileChunk* child);
// Does not use incoming ECX.

DWORD __stdcall ModelSaveChunk(const CModelFileChunk* chunk, const char* filename);
// Other model editing/writing paths remain under recovery.

void GameParseStreamData(const void* data, DWORD length, void* destination);

#endif // MODELFILE_H
