#ifndef SAMP_MODEL_FILE_H
#define SAMP_MODEL_FILE_H
#include <windows.h>
#include <stdio.h>
#include <stddef.h>
// Descriptive names for the native model-file parser types.
#pragma pack(push,1)
struct CModelFileStream {
    BYTE *data;DWORD position,length;BYTE ended,owned;BYTE *scratch;DWORD scratchLength;BYTE compressed;
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
#pragma pack(pop)
typedef char NativeModelStreamExtent[(sizeof(CModelFileStream)==0x17)?1:-1];
typedef char NativeModelChunkExtent[(sizeof(CModelFileChunk)==0x23)?1:-1];
typedef char NativeTxdFileExtent[(sizeof(CTxdFile)==0x2b)?1:-1];
typedef char NativeModelStreamOwnedOffset[(offsetof(CModelFileStream,owned)==0xd)?1:-1];
// samp.dll RVA: 0x00117c9c
extern const char g_customTxdVerifyError[];
// samp.dll RVA: 0x00117c60
extern const char g_customTxdCountError[];
// Stream compression and the DFF parser remain under recovery.
#endif
