#ifndef SAMP_MODEL_FILE_H
#define SAMP_MODEL_FILE_H
#include <windows.h>
#include <stdio.h>
#include <stddef.h>
// Descriptive names for the native model-file parser types.
#pragma pack(push,1)
struct CModelFileStream {
    BYTE *data;DWORD position,length;BYTE ended,owned;BYTE *scratch;DWORD scratchLength;BYTE compressed;
    // samp.dll RVA: 0x0000b800
    CModelFileStream();
    // samp.dll RVA: 0x0000b820
    ~CModelFileStream();
    // samp.dll RVA: 0x000bc250
    int Compress();
    // samp.dll RVA: 0x000bc300
    int Decompress(DWORD outputLength);
    // samp.dll RVA: 0x000bc060
    DWORD WriteFileChunks(FILE *file);
    // samp.dll RVA: 0x000bc0c0
    DWORD SaveFile(const char *filename);
    // samp.dll RVA: 0x000bc100
    DWORD SaveHandle(FILE *file);
    // samp.dll RVA: 0x000bc110
    bool CopyBuffer();
    // samp.dll RVA: 0x000bc150
    DWORD Write(const void *input,DWORD count);
    // samp.dll RVA: 0x000bc1d0
    DWORD Read(void *output,DWORD count);
    // samp.dll RVA: 0x000bc390
    DWORD LoadFile(const char *filename);
    // samp.dll RVA: 0x000bc450
    DWORD WriteDword(DWORD value);
    // samp.dll RVA: 0x000bc460
    DWORD WriteWord(WORD value);
    // samp.dll RVA: 0x000bc470
    DWORD ReadDword();
    // samp.dll RVA: 0x000b36a0
    BYTE ReadByte();
    // samp.dll RVA: 0x000bc490
    BYTE *Current();
    // samp.dll RVA: 0x000bc4a0
    DWORD Remaining();
    // samp.dll RVA: 0x000b36c0
    DWORD Skip(DWORD count);
};
struct CModelFileChunk {
    DWORD type,length;WORD build,version;CModelFileStream stream;
    // samp.dll RVA: 0x0000b850
    ~CModelFileChunk();
    // samp.dll RVA: 0x0000b8a0
    DWORD GetType();
    // samp.dll RVA: 0x0000b8b0
    DWORD GetLength();
};
struct CTxdFile {
    DWORD error;CModelFileStream stream;DWORD fileLength;CModelFileChunk *root,*structure;WORD textureCount,parsedCount;
    // samp.dll RVA: 0x0000b8c0
    CTxdFile();
    // samp.dll RVA: 0x0000b8f0
    ~CTxdFile();
    // samp.dll RVA: 0x0000b970
    DWORD GetTextureCount();
    // samp.dll RVA: 0x0000b980
    DWORD GetError();
    // samp.dll RVA: 0x000bda10
    CModelFileChunk *ReadChunk(CModelFileStream *input);
    // samp.dll RVA: 0x000bdb20
    DWORD ReadStructure(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bdb60
    DWORD ReadTexture(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bdd80
    DWORD ReadChildren(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bde10
    DWORD LoadFile(const char *filename);
};
struct CModelGeometry;
struct CModelGeometryArray {
    CModelGeometry **items;DWORD count;
    // samp.dll RVA: 0x0000c630
    bool Resize(DWORD size);
    // samp.dll RVA: 0x000bbe20
    DWORD Insert(CModelGeometry *geometry);
};
struct CModelMaterialArray {
    void **items;DWORD count;
    // samp.dll RVA: 0x0000c550
    bool Resize(DWORD size);
    // samp.dll RVA: 0x000bcdb0
    DWORD Insert(void *material);
};
struct CModelMeshArray {
    BYTE **items;DWORD count;
    // samp.dll RVA: 0x0000c5c0
    bool Resize(DWORD size);
    // samp.dll RVA: 0x000bce10
    DWORD Insert(BYTE *mesh);
};
struct CModelMaterialRecord {
    DWORD values[7];
    struct Texture {DWORD flags;char name[32],mask[32];} textures[2];
};
struct CModelSkin {
    BYTE boneCount,usedBoneCount,maxWeights,unknown3;
    void *usedBones,*indices,*weights,*matrices;DWORD unknown14,unknown18,unknown1c;
    void *unknown20,*unknown24,*unknown28;
};
struct CModelGeometry {
    BYTE modified,modifiedOther;DWORD error,detail;WORD flags,textureSets;
    int triangleCount,vertexCount,morphCount;BYTE hasColors,unknown1b;
    void *colors,*unknown20,*uv,*triangles;float sphere[4];DWORD hasVertices,hasNormals;
    void *vertices,*normals;CModelFileChunk *material,*extension;int materialCount;
    CModelMaterialArray materials;BYTE unknown60;DWORD unknown61,unknown65,unknown69;
    CModelMeshArray meshes;union {BYTE opaque75[44];CModelSkin skin;};
    // samp.dll RVA: 0x000bbe80
    CModelGeometry();
    // samp.dll RVA: 0x0000d620
    ~CModelGeometry();
    // samp.dll RVA: 0x000bb110
    bool IsModified();
    // samp.dll RVA: 0x000bb130
    DWORD GetError();
    // samp.dll RVA: 0x000bb140
    DWORD GetDetail();
    // samp.dll RVA: 0x000bc4b0
    void SetError(DWORD value);
    // samp.dll RVA: 0x000bc4d0
    void SetDetail(DWORD value);
    // samp.dll RVA: 0x000bc600
    CModelFileChunk *ReadChunk(CModelFileStream *input);
    // samp.dll RVA: 0x000bd180
    bool ReadMaterial(CModelFileStream *input);
    // samp.dll RVA: 0x000bd5f0
    bool ReadMaterials(CModelFileStream *input);
    // samp.dll RVA: 0x000bc760
    bool ReadSkin(CModelFileStream *input);
    // samp.dll RVA: 0x000bce70
    bool ReadExtensions(CModelFileStream *input);
    // samp.dll RVA: 0x000bd700
    bool Read(CModelFileStream *input,WORD version);
};
struct CDffFile {
    DWORD error,detail;CModelFileStream stream;DWORD fileLength;BYTE hasCollision;
    CModelGeometryArray geometries;
    CModelFileChunk *root,*structure,*frames,*geometryList,*atomic,*extension;
    DWORD unknown44;void *frameData;
    // samp.dll RVA: 0x0000d820
    CDffFile();
    // samp.dll RVA: 0x0000d8f0
    ~CDffFile();
    // samp.dll RVA: 0x000bb170
    CModelFileChunk *ReadChunk(CModelFileStream *input);
    // samp.dll RVA: 0x000bb280
    DWORD StoreFrames(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bb2a0
    DWORD StoreGeometryList(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bb2c0
    DWORD StoreAtomic(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bb2e0
    DWORD StoreStructure(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bb300
    DWORD StoreExtension(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bb320
    DWORD ReadChildren(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bb460
    DWORD LoadFile(const char *filename);
    // samp.dll RVA: 0x000bb520
    DWORD LoadCollisionFile(const char *filename);
    // samp.dll RVA: 0x000bbaa0
    DWORD VerifyFrameExtension(CModelFileChunk *chunk);
    // samp.dll RVA: 0x000bbb30
    DWORD ReadFrames();
    // samp.dll RVA: 0x000bbf00
    DWORD ReadGeometries();
};
#pragma pack(pop)
typedef char NativeModelStreamExtent[(sizeof(CModelFileStream)==0x17)?1:-1];
typedef char NativeModelChunkExtent[(sizeof(CModelFileChunk)==0x23)?1:-1];
typedef char NativeTxdFileExtent[(sizeof(CTxdFile)==0x2b)?1:-1];
typedef char NativeDffFileExtent[(sizeof(CDffFile)==0x4c)?1:-1];
typedef char NativeModelGeometryExtent[(sizeof(CModelGeometry)==0xa1)?1:-1];
typedef char NativeModelMaterialRecordExtent[(sizeof(CModelMaterialRecord)==0xa4)?1:-1];
typedef char NativeModelSkinExtent[(sizeof(CModelSkin)==44)?1:-1];
typedef char NativeModelGeometryMaterialsOffset[(offsetof(CModelGeometry,materials)==0x58)?1:-1];
typedef char NativeModelGeometryMeshesOffset[(offsetof(CModelGeometry,meshes)==0x6d)?1:-1];
typedef char NativeDffFramesOffset[(offsetof(CDffFile,frames)==0x34)?1:-1];
typedef char NativeModelStreamOwnedOffset[(offsetof(CModelFileStream,owned)==0xd)?1:-1];
// samp.dll RVA: 0x00117c9c
extern const char g_customTxdVerifyError[];
// samp.dll RVA: 0x00117c60
extern const char g_customTxdCountError[];
// samp.dll RVA: 0x00117dac
extern const char g_customDffVerifyError[];
// samp.dll RVA: 0x00117d68
extern const char g_customDffParseError[];
// samp.dll RVA: 0x00155470
extern const BYTE g_modelLzmaProperties[5];
// Uses the selected LZMA backend and allocator; preserves the native stdcall adapter.
// samp.dll RVA: 0x000c5040
int __stdcall ModelLzmaEncode(BYTE *destination,DWORD *destinationLength,const BYTE *source,DWORD sourceLength,
    BYTE *properties,DWORD *propertySize,int level,DWORD dictionary,int lc,int lp,int pb,int fastBytes,int threads);
// Other model editing/writing paths remain under recovery.
#endif
