#include "game/modelfile.h"
#include "game/custommodels.h"
#include <stdlib.h>
#include <string.h>
#include <new>
// samp.dll RVA: 0x00117c9c
const char g_customTxdVerifyError[]="[warning] Could not verify 0x%X.txd. Error(%d) Deleting..";
// samp.dll RVA: 0x00117c60
const char g_customTxdCountError[]="[warning] Too many textures in 0x%X.txd. (%u) Deleting..";
// samp.dll RVA: 0x000bc060
DWORD CModelFileStream::WriteFileChunks(FILE *file) {
    BYTE *buffer=data;DWORD offset=0;
    while(offset<length) {
        DWORD next=offset+1024;
        if(next<length){fwrite(buffer+offset,1,1024,file);offset=next;}
        else {fwrite(buffer+offset,1,length-offset,file);offset=length;}
    }
    return length;
}
// samp.dll RVA: 0x000bc0c0
DWORD CModelFileStream::SaveFile(const char *filename) {
    FILE *file=fopen(filename,g_customDownloadWriteMode);if(!file)return 0;
    WriteFileChunks(file);fclose(file);return length;
}
// samp.dll RVA: 0x000bc100
DWORD CModelFileStream::SaveHandle(FILE *file) {return WriteFileChunks(file);}
// samp.dll RVA: 0x000bc110
bool CModelFileStream::CopyBuffer() {
    BYTE *copy=(BYTE*)malloc(length);if(!copy)return false;
    memcpy(copy,data,length);data=copy;owned=1;return true;
}
// samp.dll RVA: 0x000bc150
DWORD CModelFileStream::Write(const void *input,DWORD count) {
    if(position+count>length) {
        if(!owned)CopyBuffer();
        DWORD extra=count-length+position;data=(BYTE*)realloc(data,length+extra);length+=extra;
    }
    memcpy(data+position,input,count);position+=count;return count;
}
// samp.dll RVA: 0x000bc1d0
DWORD CModelFileStream::Read(void *output,DWORD count) {
    memset(output,0,count);
    if(position+count>length)count=length-position;
    if(!count)return 0;
    memcpy(output,data+position,count);
    DWORD next=position+count;
    if(next<length)position=next;
    else {position=length;ended=1;}
    return count;
}
// samp.dll RVA: 0x000bc390
DWORD CModelFileStream::LoadFile(const char *filename) {
    BYTE buffer[2048];position=0;
    if(owned&&data){free(data);data=0;}
    FILE *file=fopen(filename,g_customDownloadReadMode);if(!file)return 0;
    owned=1;data=(BYTE*)malloc(1);length=1;
    // Native loops on the VC7 FILE EOF bit, without a separate ferror escape.
    while(!(*(BYTE*)((BYTE*)file+0xc)&0x10)) {
        DWORD count=(DWORD)fread(buffer,1,2048,file);Write(buffer,count);
    }
    fclose(file);position=0;ended=0;return length;
}
// samp.dll RVA: 0x000bc450
DWORD CModelFileStream::WriteDword(DWORD value) {return Write(&value,4);}
// samp.dll RVA: 0x000bc460
DWORD CModelFileStream::WriteWord(WORD value) {return Write(&value,2);}
// samp.dll RVA: 0x000bc470
DWORD CModelFileStream::ReadDword() {DWORD value=0;Read(&value,4);return value;}
// samp.dll RVA: 0x000b36a0
BYTE CModelFileStream::ReadByte() {BYTE value=0;Read(&value,1);return value;}
// samp.dll RVA: 0x000bc490
BYTE *CModelFileStream::Current() {return data+position;}
// samp.dll RVA: 0x000bc4a0
DWORD CModelFileStream::Remaining() {return length-position;}
// samp.dll RVA: 0x0000b850
CModelFileChunk::~CModelFileChunk() {
    if(stream.owned&&stream.data){free(stream.data);stream.data=0;}
}
// samp.dll RVA: 0x0000b8a0
DWORD CModelFileChunk::GetType() {return type;}
// samp.dll RVA: 0x0000b8b0
DWORD CModelFileChunk::GetLength() {return length;}
// samp.dll RVA: 0x0000b8c0
CTxdFile::CTxdFile() {
    stream.data=0;stream.length=0;stream.position=0;stream.ended=0;stream.owned=0;
    stream.compressed=0;stream.scratchLength=0;stream.scratch=0;error=0;fileLength=0;
    root=0;structure=0;textureCount=0;parsedCount=0;
}
// samp.dll RVA: 0x0000b8f0
CTxdFile::~CTxdFile() {
    if(structure){delete structure;structure=0;}
    if(root){delete root;root=0;}
    if(stream.owned&&stream.data){free(stream.data);stream.data=0;}
}
// samp.dll RVA: 0x0000b970
DWORD CTxdFile::GetTextureCount() {return textureCount;}
// samp.dll RVA: 0x0000b980
DWORD CTxdFile::GetError() {return error;}
// samp.dll RVA: 0x000bda10
CModelFileChunk *CTxdFile::ReadChunk(CModelFileStream *input) {
    if(!input->length||input->ended)return 0;
    CModelFileChunk *chunk=(CModelFileChunk*)::operator new(0x23);
    if(chunk) {
        chunk->stream.data=0;chunk->stream.length=0;chunk->stream.position=0;chunk->stream.ended=0;chunk->stream.owned=0;
        chunk->stream.compressed=0;chunk->stream.scratchLength=0;chunk->stream.scratch=0;
        chunk->type=0;chunk->length=0;chunk->build=0xffff;chunk->version=0x1803;
    }
    DWORD word=0;input->Read(&word,4);chunk->type=word;
    word=0;input->Read(&word,4);chunk->length=word;
    word=0;input->Read(&word,2);chunk->build=(WORD)word;
    word=0;input->Read(&word,2);chunk->version=(WORD)word;
    chunk->stream.length=chunk->length;chunk->stream.data=input->data+input->position;
    chunk->stream.position=0;chunk->stream.ended=0;chunk->stream.owned=0;
    DWORD next=input->position+chunk->length;
    if(next<input->length)input->position=next;
    else {input->position=input->length;input->ended=1;}
    return chunk;
}
// samp.dll RVA: 0x000bdb20
DWORD CTxdFile::ReadStructure(CModelFileChunk *chunk) {
    if(structure)return 0;
    structure=chunk;DWORD count=0;chunk->stream.Read(&count,2);textureCount=(WORD)count;return 1;
}
// samp.dll RVA: 0x000bdb60
DWORD CTxdFile::ReadTexture(CModelFileChunk *chunk) {
    DWORD header[3];CModelFileStream *input=&chunk->stream;input->Read(header,12);
    if(header[0]!=1)return 0;
    // Native builds a temporary 88-byte descriptor; it is never published.
    DWORD value=0;input->Read(&value,4);
    if(value!=8&&value!=9){if(!error)error=0xd2f6;return 0;}
    value=0;input->Read(&value,4);
    char name[65]={0},mask[65]={0};input->Read(name,32);
    if(strlen(name)>31){if(!error)error=0xd2f5;return 0;}
    char savedName[32],savedMask[32];strncpy(savedName,name,31);
    input->Read(mask,32);
    if(strlen(mask)>31){if(!error)error=0xd2f5;return 0;}
    strncpy(savedMask,mask,31);
    value=0;input->Read(&value,4);value=0;input->Read(&value,4);
    DWORD width=0,height=0;input->Read(&width,2);input->Read(&height,2);
    if((WORD)width>2048||(WORD)height>2048){if(!error)error=0xd2f7;return 0;}
    input->ReadByte();input->ReadByte();input->ReadByte();input->ReadByte();
    ++parsedCount;return 1;
}
// samp.dll RVA: 0x000bdd80
DWORD CTxdFile::ReadChildren(CModelFileChunk *parent) {
    CModelFileStream *input=&parent->stream;CModelFileChunk *chunk=ReadChunk(input);
    while(chunk) {
        if(chunk->type==1) {
            if(!ReadStructure(chunk))delete chunk;
        }else if(chunk->type==0x15) {
            if(!ReadTexture(chunk)){delete chunk;if(!error)error=0xd2f2;return 0;}
            // Native retains successful texture chunks without publishing a pointer.
        }else delete chunk;
        chunk=ReadChunk(input);
    }
    return 1;
}
// samp.dll RVA: 0x000bde10
DWORD CTxdFile::LoadFile(const char *filename) {
    if(!stream.LoadFile(filename))return 0xffffffff;
    fileLength=stream.length;root=ReadChunk(&stream);
    if(root) {
        if(root->type!=0x16){if(!error)error=54000;return 0;}
        ReadChildren(root);
        if(textureCount!=parsedCount){if(!error)error=0xd2f2;return 0;}
    }
    return 1;
}
// samp.dll RVA: 0x000bb280
DWORD CDffFile::StoreFrames(CModelFileChunk *chunk) {if(frames)return 0;frames=chunk;return 1;}
// samp.dll RVA: 0x000bb2a0
DWORD CDffFile::StoreGeometryList(CModelFileChunk *chunk) {if(geometryList)return 0;geometryList=chunk;return 1;}
// samp.dll RVA: 0x000bb2c0
DWORD CDffFile::StoreAtomic(CModelFileChunk *chunk) {if(atomic)return 0;atomic=chunk;return 1;}
// samp.dll RVA: 0x000bb2e0
DWORD CDffFile::StoreStructure(CModelFileChunk *chunk) {if(structure)return 0;structure=chunk;return 1;}
// samp.dll RVA: 0x000bb300
DWORD CDffFile::StoreExtension(CModelFileChunk *chunk) {if(extension)return 0;extension=chunk;return 1;}
// samp.dll RVA: 0x000bb320
DWORD CDffFile::ReadChildren(CModelFileChunk *parent) {
    CModelFileStream *input=&parent->stream;CModelFileChunk *chunk=ReadChunk(input);
    while(chunk) {
        DWORD retained=0,failure=0;
        switch(chunk->type) {
        case 1:retained=StoreStructure(chunk);break;
        case 3:retained=StoreExtension(chunk);break;
        case 14:retained=StoreFrames(chunk);if(!retained)failure=0xabf0;break;
        case 20:retained=StoreAtomic(chunk);if(!retained)failure=0xabf1;break;
        case 26:retained=StoreGeometryList(chunk);if(!retained)failure=0xabef;break;
        }
        if(!retained)delete chunk;
        if(failure){if(!error)error=failure;return 0;}
        chunk=ReadChunk(input);
    }
    return 1;
}
// samp.dll RVA: 0x000bb460
DWORD CDffFile::LoadFile(const char *filename) {
    if(!stream.LoadFile(filename))return 0xffffffff;
    fileLength=stream.length;root=ReadChunk(&stream);
    if(root) {
        if(root->type==0x2b){delete root;root=0;root=ReadChunk(&stream);}
        // Native does not recheck root after skipping the leading chunk.
        if(root->type!=0x10){if(!error)error=44000;return 0;}
        if(!ReadChildren(root))return 0;
        if(extension&&extension->length) {
            CModelFileChunk *chunk=ReadChunk(&extension->stream);
            if(chunk&&chunk->type==0x253f2ff)hasCollision=1;
            // The collision probe chunk is deliberately not deleted by native.
        }
    }
    return 1;
}
// samp.dll RVA: 0x000bb520
DWORD CDffFile::LoadCollisionFile(const char *filename) {
    if(!stream.LoadFile(filename))return 0xffffffff;
    fileLength=stream.length;root=ReadChunk(&stream);
    if(root&&root->type==0x10) {
        ReadChildren(root);
        if(extension&&extension->length) {
            CModelFileChunk *chunk=ReadChunk(&extension->stream);
            if(chunk&&chunk->type==0x253f2ff)hasCollision=1;
        }
    }
    return 1;
}
// samp.dll RVA: 0x000bbaa0
DWORD CDffFile::VerifyFrameExtension(CModelFileChunk *chunk) {
    while(!chunk->stream.ended) {
        DWORD header[3];chunk->stream.Read(header,12);
        if(header[0]==0x253f2fe&&header[1]>=24)return 0;
        DWORD next=chunk->stream.position+header[1];
        if(next<chunk->stream.length)chunk->stream.position=next;
        else {chunk->stream.position=chunk->stream.length;chunk->stream.ended=1;}
    }
    return 1;
}
// samp.dll RVA: 0x000bbb30
DWORD CDffFile::ReadFrames() {
    if(!frames)return 0;
    frames->stream.position=0;frames->stream.ended=0;
    DWORD header[3];frames->stream.Read(header,12);
    if(header[0]!=1){if(!error)error=0xabe7;detail=1;return 0;}
    DWORD count=0;frames->stream.Read(&count,4);
    if(count>=256){if(!error)error=0xabec;detail=count;return 0;}
    frameData=calloc(count,56);frames->stream.Read(frameData,count*56);
    CModelFileChunk *chunk=ReadChunk(&frames->stream);
    while(chunk) {
        if(chunk->type==3&&chunk->length&&!VerifyFrameExtension(chunk)) {
            delete chunk;if(!error)error=0xabee;return 0;
        }
        delete chunk;chunk=ReadChunk(&frames->stream);
    }
    return 1;
}
// samp.dll RVA: 0x000bb170
CModelFileChunk *CDffFile::ReadChunk(CModelFileStream *input) {
    if(!input->length||input->ended)return 0;
    CModelFileChunk *chunk=(CModelFileChunk*)::operator new(0x23);
    if(chunk) {
        chunk->stream.data=0;chunk->stream.length=0;chunk->stream.position=0;chunk->stream.ended=0;chunk->stream.owned=0;
        chunk->stream.compressed=0;chunk->stream.scratchLength=0;chunk->stream.scratch=0;
        chunk->type=0;chunk->length=0;chunk->build=0xffff;chunk->version=0x1803;
    }
    DWORD word=0;input->Read(&word,4);chunk->type=word;
    word=0;input->Read(&word,4);chunk->length=word;
    word=0;input->Read(&word,2);chunk->build=(WORD)word;
    word=0;input->Read(&word,2);chunk->version=(WORD)word;
    chunk->stream.length=chunk->length;chunk->stream.data=input->data+input->position;
    chunk->stream.position=0;chunk->stream.ended=0;chunk->stream.owned=0;
    DWORD next=input->position+chunk->length;
    if(next<input->length)input->position=next;
    else {input->position=input->length;input->ended=1;}
    return chunk;
}
