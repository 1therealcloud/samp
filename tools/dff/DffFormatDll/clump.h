#ifndef KDFF_CLUMP_H
#define KDFF_CLUMP_H

#include "geometry.h"

#pragma pack(push, 1)
class CClump
{
public:
    unsigned int Error, ErrorDetail;
    CMemoryStream File;
    unsigned int FileSize;
    bool HasCol;
    CPointerArray Geometries;
    CRwChunk* Root;
    CRwChunk* Struct;
    CRwChunk* FrameList;
    CRwChunk* GeometryList;
    CRwChunk* Atomic;
    CRwChunk* Extension;
    unsigned int Unknown44;
    void* Unknown48;

    CClump();

    ~CClump();

    bool ReadClump(CRwChunk* Chunk);

    int Load(const char* FileName);

    unsigned int Save(const char* FileName);

    int AppendCol(const char* FileName);

    bool RebuildGeometry();

    bool ReadGeometry();
#ifndef KDFF_DLL
    // 0x00001c40 (kdff.exe)
    bool ReadFrames();
#endif
};
#pragma pack(pop)

#endif
