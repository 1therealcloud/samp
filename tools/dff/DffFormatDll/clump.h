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

    // 0x00002300
    CClump();
    // 0x000023f0
    ~CClump();
    // 0x000017d0
    bool ReadClump(CRwChunk* Chunk);
    // 0x00001910
    int Load(const char* FileName);
    // 0x000019d0
    unsigned int Save(const char* FileName);
    // 0x00001c00
    int AppendCol(const char* FileName);
    // 0x00001d80
    bool RebuildGeometry();
    // 0x00002170
    bool ReadGeometry();
#ifndef KDFF_DLL
    // 0x00001c40 (kdff.exe)
    bool ReadFrames();
#endif
};
#pragma pack(pop)

#endif
