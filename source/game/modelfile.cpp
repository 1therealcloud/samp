#include "game/modelfile.h"
#include "game/custommodels.h"
#include <stdlib.h>
#include <string.h>
#include <new>
#include "LzmaEnc.h"
#include "Alloc.h"
// samp.dll RVA: 0x00155470
const BYTE g_modelLzmaProperties[5]={0x5d,0,0,1,0};
// samp.dll RVA: 0x000b36f0
void CModelFileStream::Reset() {position=0;ended=0;}
// samp.dll RVA: 0x000b3700
BYTE CModelFileStream::HasEnded() {return ended;}
// samp.dll RVA: 0x000bc4e0
int CModelGeometry::GetVertexCount() {return vertexCount;}
// samp.dll RVA: 0x000bc4f0
BYTE CModelGeometry::HasVertexColors() {return hasColors;}
// samp.dll RVA: 0x000bc500
void *CModelGeometry::GetVertexColors() {return colors;}
// samp.dll RVA: 0x000bc510
DWORD CModelGeometry::SetRepeatedVertexColors(const BYTE *first,const BYTE *second) {
    modified=1;flags=(WORD)((flags&0xffdf)|8);
    if(colors){free(colors);colors=0;}
    if(unknown20){free(unknown20);unknown20=0;}
    hasColors=1;unknown1b=1;colors=calloc(vertexCount,4);unknown20=calloc(vertexCount,4);
    for(DWORD i=0;i!=(DWORD)vertexCount;++i) {
        ((BYTE*)colors)[i*4]=first[0];((BYTE*)colors)[i*4+1]=first[1];
        ((BYTE*)colors)[i*4+2]=first[2];((BYTE*)colors)[i*4+3]=first[3];
        ((BYTE*)unknown20)[i*4]=second[0];((BYTE*)unknown20)[i*4+1]=second[1];
        ((BYTE*)unknown20)[i*4+2]=second[2];((BYTE*)unknown20)[i*4+3]=second[3];
    }
    return 1;
}
// samp.dll RVA: 0x000bbc80
bool CDffFile::IsModified() {
    DWORD count=geometries.count;
    for(DWORD i=0;i!=count;++i)if(geometries.items[i]->IsModified())return true;
    return false;
}
// samp.dll RVA: 0x000bc710
DWORD __stdcall ModelAppendChunk(CModelFileChunk *parent,const CModelFileChunk *child) {
    if(!parent||!child)return 0;
    parent->stream.Write(child,12);parent->stream.Write(child->stream.data,child->stream.length);
    parent->length+=child->stream.length+12;return 1;
}
// samp.dll RVA: 0x000c5040
int __stdcall ModelLzmaEncode(BYTE *destination,DWORD *destinationLength,const BYTE *source,DWORD sourceLength,
    BYTE *properties,DWORD *propertySize,int level,DWORD dictionary,int lc,int lp,int pb,int fastBytes,int threads) {
    CLzmaEncProps options;LzmaEncProps_Init(&options);
    options.level=level;options.dictSize=dictionary;options.lc=lc;options.lp=lp;options.pb=pb;
    options.fb=fastBytes;options.numThreads=threads;
    return LzmaEncode(destination,(SizeT*)destinationLength,source,sourceLength,&options,properties,(SizeT*)propertySize,0,0,&g_Alloc,&g_Alloc);
}
// samp.dll RVA: 0x0000b800
CModelFileStream::CModelFileStream() {
    data=0;length=0;position=0;ended=0;owned=0;compressed=0;scratchLength=0;scratch=0;
}
// samp.dll RVA: 0x0000b820
CModelFileStream::~CModelFileStream() {if(owned&&data){free(data);data=0;}}
// samp.dll RVA: 0x000bc250
int CModelFileStream::Compress() {
    BYTE properties[5];DWORD propertySize=5;scratchLength=length;
    if(compressed)return -1;
    scratch=(BYTE*)malloc(length);
    int result=ModelLzmaEncode(scratch,&scratchLength,data,length,properties,&propertySize,1,0x10000,3,0,2,32,1);
    if(result){free(scratch);compressed=0;return result;}
    if(owned)free(data);data=scratch;owned=1;length=scratchLength;compressed=1;return result;
}
// samp.dll RVA: 0x000bc300
int CModelFileStream::Decompress(DWORD outputLength) {
    if(!compressed)return -1;
    scratch=(BYTE*)malloc(outputLength);
    int result=CustomModelLzmaDecode(scratch,&outputLength,data,&length,g_modelLzmaProperties,5);
    if(result){free(scratch);compressed=1;return result;}
    if(owned)free(data);length=outputLength;data=scratch;owned=1;compressed=0;return result;
}
// samp.dll RVA: 0x00117c9c
const char g_customTxdVerifyError[]="[warning] Could not verify 0x%X.txd. Error(%d) Deleting..";
// samp.dll RVA: 0x00117c60
const char g_customTxdCountError[]="[warning] Too many textures in 0x%X.txd. (%u) Deleting..";
// samp.dll RVA: 0x00117dac
const char g_customDffVerifyError[]="[warning] Could not validate 0x%X.dff. Error(%d) Deleting..";
// samp.dll RVA: 0x00117d68
const char g_customDffParseError[]="[warning] Could not validate 0x%X.dff. Error(%d:0x%X) Deleting..";
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
    // The stream member performs the native owned-buffer cleanup.
}
// samp.dll RVA: 0x0000b8a0
DWORD CModelFileChunk::GetType() {return type;}
// samp.dll RVA: 0x0000b8b0
DWORD CModelFileChunk::GetLength() {return length;}
// samp.dll RVA: 0x0000b8c0
CTxdFile::CTxdFile() {
    error=0;fileLength=0;
    root=0;structure=0;textureCount=0;parsedCount=0;
}
// samp.dll RVA: 0x0000b8f0
CTxdFile::~CTxdFile() {
    if(structure){delete structure;structure=0;}
    if(root){delete root;root=0;}
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
// samp.dll RVA: 0x0000c550
bool CModelMaterialArray::Resize(DWORD size) {
    if(!size){if(items){free(items);items=0;}count=0;return true;}
    void **replacement=(void**)realloc(items,size*4);if(!replacement)return false;
    items=replacement;for(DWORD i=count;i<size;++i)items[i]=0;count=size;return true;
}
// samp.dll RVA: 0x0000c5c0
bool CModelMeshArray::Resize(DWORD size) {
    if(!size){if(items){free(items);items=0;}count=0;return true;}
    BYTE **replacement=(BYTE**)realloc(items,size*4);if(!replacement)return false;
    items=replacement;for(DWORD i=count;i<size;++i)items[i]=0;count=size;return true;
}
// samp.dll RVA: 0x0000c630
bool CModelGeometryArray::Resize(DWORD size) {
    if(!size){if(items){free(items);items=0;}count=0;return true;}
    CModelGeometry **replacement=(CModelGeometry**)realloc(items,size*4);if(!replacement)return false;
    items=replacement;for(DWORD i=count;i<size;++i)items[i]=0;count=size;return true;
}
// samp.dll RVA: 0x000bbe20
DWORD CModelGeometryArray::Insert(CModelGeometry *geometry) {
    DWORD previous=count,index=0;
    while(index!=previous&&items[index])++index;
    if(index!=previous&&(int)index>=0){items[index]=geometry;return index;}
    if(!Resize(previous+1))return 0xffffffff;
    items[previous]=geometry;return previous;
}
// samp.dll RVA: 0x000bbe80
CModelGeometry::CModelGeometry() {
    materials.items=0;materials.count=0;meshes.items=0;meshes.count=0;
    error=0;detail=0;flags=0;textureSets=0;triangleCount=0;vertexCount=0;morphCount=0;
    hasColors=0;colors=0;unknown1b=0;unknown20=0;uv=0;triangles=0;
    memset(sphere,0,16);hasVertices=0;hasNormals=0;vertices=0;normals=0;
    materialCount=0;unknown60=0;unknown61=0;unknown65=0;unknown69=0;
    modified=0;modifiedOther=0;memset(opaque75,0,44);material=0;extension=0;
}
// samp.dll RVA: 0x0000d620
CModelGeometry::~CModelGeometry() {
    // These plugin buffer roles remain opaque; only the native ownership is established.
    void **buffers[]={&normals,&vertices,&triangles,&uv,&colors,&unknown20,
        (void**)(opaque75+4),(void**)(opaque75+8),(void**)(opaque75+12),(void**)(opaque75+16),
        (void**)(opaque75+32),(void**)(opaque75+36),(void**)(opaque75+40)};
    for(unsigned i=0;i<sizeof(buffers)/sizeof(buffers[0]);++i) {
        if(*buffers[i]){free(*buffers[i]);*buffers[i]=0;}
    }
    DWORD materialCount=materials.count;
    for(DWORD j=0;j!=materialCount;++j)if(j<materials.count&&materials.items[j])free(materials.items[j]);
    DWORD meshCount=meshes.count;
    for(DWORD k=0;k!=meshCount;++k) {
        if(k<meshes.count&&meshes.items[k]) {
            BYTE *mesh=meshes.items[k];void *&indices=*(void**)(mesh+8);
            if(indices){free(indices);indices=0;}free(mesh);
        }
    }
    if(material){delete material;material=0;}
    if(extension){delete extension;extension=0;}
    meshes.Resize(0);materials.Resize(0);
}
// samp.dll RVA: 0x000bb110
bool CModelGeometry::IsModified() {return modified!=0||modifiedOther!=0;}
// samp.dll RVA: 0x000bb130
DWORD CModelGeometry::GetError() {return error;}
// samp.dll RVA: 0x000bb140
DWORD CModelGeometry::GetDetail() {return detail;}
// samp.dll RVA: 0x000bc4b0
void CModelGeometry::SetError(DWORD value) {if(!error)error=value;}
// samp.dll RVA: 0x000bc4d0
void CModelGeometry::SetDetail(DWORD value) {detail=value;}
// samp.dll RVA: 0x0000d820
CDffFile::CDffFile() {
    geometries.items=0;geometries.count=0;
    error=0;detail=0;fileLength=0;root=0;structure=0;frames=0;geometryList=0;atomic=0;extension=0;
    hasCollision=0;unknown44=0;frameData=0;
}
// samp.dll RVA: 0x0000d8f0
CDffFile::~CDffFile() {
    if(extension){delete extension;extension=0;}
    if(atomic){delete atomic;atomic=0;}
    if(geometryList){delete geometryList;geometryList=0;}
    if(frames){delete frames;frames=0;}
    if(structure){delete structure;structure=0;}
    if(root){delete root;root=0;}
    if(frameData){::operator delete(frameData);frameData=0;}
    int count=(int)geometries.count;
    for(DWORD i=0;(int)i<count;++i)if(i<geometries.count&&geometries.items[i])delete geometries.items[i];
    geometries.Resize(0);geometries.Resize(0);
}
// samp.dll RVA: 0x000bc600
CModelFileChunk *CModelGeometry::ReadChunk(CModelFileStream *input) {
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
// samp.dll RVA: 0x000bcdb0
DWORD CModelMaterialArray::Insert(void *entry) {
    DWORD previous=count,index=0;
    while(index!=previous&&items[index])++index;
    if(index!=previous&&(int)index>=0){items[index]=entry;return index;}
    if(!Resize(previous+1))return 0xffffffff;
    items[previous]=entry;return previous;
}
// samp.dll RVA: 0x000bce10
DWORD CModelMeshArray::Insert(BYTE *entry) {
    DWORD previous=count,index=0;
    while(index!=previous&&items[index])++index;
    if(index!=previous&&(int)index>=0){items[index]=entry;return index;}
    if(!Resize(previous+1))return 0xffffffff;
    items[previous]=entry;return previous;
}
// samp.dll RVA: 0x000b36c0
DWORD CModelFileStream::Skip(DWORD count) {
    DWORD next=position+count;
    if(next<length){position=next;return next;}
    position=length;ended=1;return length;
}
// samp.dll RVA: 0x000bd180
bool CModelGeometry::ReadMaterial(CModelFileStream *input) {
    CModelMaterialRecord *record=(CModelMaterialRecord*)calloc(1,0xa4);
    DWORD header[3],nested[3],failure=9,remaining=0;
    input->Read(header,12);
    if(header[0]!=1||header[1]!=28)goto malformed;
    for(unsigned field=0;field<7;++field){DWORD value=0;input->Read(&value,4);record->values[field]=value;}
    if(record->values[3]>2) {
        if(!error)error=0xabe8;detail=record->values[3];free(record);return false;
    }
    for(DWORD i=0;i!=record->values[3];++i) {
        input->Read(header,12);if(header[0]!=6)continue;
        input->Read(header,12);
        if(header[0]!=1||header[1]!=4){failure=4;goto malformed;}
        DWORD value=0;input->Read(&value,4);record->textures[i].flags=value;
        input->Read(header,12);
        if(header[0]!=2||header[1]>32){failure=5;goto malformed;}
        input->Read(record->textures[i].name,header[1]);
        input->Read(header,12);
        if(header[0]!=2||header[1]>32){failure=6;goto malformed;}
        input->Read(record->textures[i].mask,header[1]);
        input->Read(header,12);
        if(header[0]==3){if(header[1])input->Skip(header[1]);}
        else if(header[1]){failure=7;goto malformed;}
    }
    input->Read(header,12);
    if(header[0]!=3){failure=8;goto malformed;}
    remaining=header[1];
    while(remaining) {
        input->Read(nested,12);remaining-=12;
        if(nested[0]!=0x253f2fc&&nested[0]!=0x253f2f6&&nested[0]!=0x120&&nested[0]!=0x135) {
            if(!error)error=0xabe2;detail=nested[0];free(record);return false;
        }
        input->Skip(nested[1]);remaining-=nested[1];
    }
    materials.Insert(record);return true;
malformed:
    if(!error)error=0xabe7;detail=failure;if(record)free(record);return false;
}
// samp.dll RVA: 0x000bd5f0
bool CModelGeometry::ReadMaterials(CModelFileStream *input) {
    material=ReadChunk(input);
    if(material->type!=8){if(!error)error=0xabf3;return false;}
    DWORD header[3];material->stream.Read(header,12);
    if(header[0]==1) {
        DWORD count=0;material->stream.Read(&count,4);materialCount=(int)count;
        material->stream.Skip(header[1]-4);
        if(materialCount<=0||materialCount>=256){if(!error)error=0xabe9;return false;}
        for(int i=0;i<materialCount;++i) {
            material->stream.Read(header,12);
            if(header[0]==7&&!ReadMaterial(&material->stream)){if(!error)error=0xabe9;return false;}
        }
    }
    return true;
}
// samp.dll RVA: 0x000bc760
bool CModelGeometry::ReadSkin(CModelFileStream *input) {
    if(vertexCount<=0)return false;
    BYTE value=0;input->Read(&value,1);skin.boneCount=value;
    value=0;input->Read(&value,1);skin.usedBoneCount=value;
    value=0;input->Read(&value,1);skin.maxWeights=value;
    value=0;input->Read(&value,1);skin.unknown3=value;
    if(skin.boneCount>=64||skin.usedBoneCount>skin.boneCount)return false;
    skin.usedBones=calloc(skin.usedBoneCount,1);input->Read(skin.usedBones,skin.usedBoneCount);
    skin.indices=calloc(vertexCount,4);input->Read(skin.indices,(DWORD)vertexCount*4);
    skin.weights=calloc(vertexCount,16);input->Read(skin.weights,(DWORD)vertexCount*16);
    skin.matrices=calloc(skin.boneCount,64);input->Read(skin.matrices,(DWORD)skin.boneCount*64);
    DWORD word=0;input->Read(&word,4);skin.unknown14=word;
    word=0;input->Read(&word,4);skin.unknown18=word;
    word=0;input->Read(&word,4);skin.unknown1c=word;
    return !skin.unknown14&&!skin.unknown18&&!skin.unknown1c;
}
// samp.dll RVA: 0x000bce70
bool CModelGeometry::ReadExtensions(CModelFileStream *input) {
    extension=ReadChunk(input);
    if(extension->type!=3){if(!error)error=0xabf2;detail=0;return false;}
    while(extension->stream.Remaining()>12) {
        DWORD header[3];extension->stream.Read(header,12);
        switch(header[0]) {
        case 0x50e: {
            unknown60=1;DWORD value=0;extension->stream.Read(&value,4);unknown61=value;
            value=0;extension->stream.Read(&value,4);unknown65=value;
            value=0;extension->stream.Read(&value,4);unknown69=value;
            if(unknown65>=100000){if(!error)error=0xabe5;return false;}
            for(DWORD i=0;i<unknown65;++i) {
                BYTE *mesh=(BYTE*)calloc(1,12);value=0;extension->stream.Read(&value,4);*(DWORD*)mesh=value;
                value=0;extension->stream.Read(&value,4);*(DWORD*)(mesh+4)=value;
                void *indices=calloc(*(DWORD*)mesh,4);*(void**)(mesh+8)=indices;
                extension->stream.Read(indices,*(DWORD*)mesh*4);meshes.Insert(mesh);
            }
            break;
        }
        case 0x253f2fd:extension->stream.Skip(header[1]);break;
        case 0x253f2f9:
            if(((header[1]-4)>>2)==(DWORD)vertexCount) {
                unknown1b=1;extension->stream.Skip(4);unknown20=calloc(vertexCount,4);
                if(!unknown20||extension->stream.Remaining()<(DWORD)vertexCount*4) {
                    if(!error)error=0xabe1;detail=0x253f2f9;return false;
                }
                extension->stream.Read(unknown20,(DWORD)vertexCount*4);
            }
            break;
        case 0x116: {
            DWORD previous=extension->stream.position;
            if(!ReadSkin(&extension->stream)||extension->stream.position-previous!=header[1]) {
                if(!error)error=0xabe7;detail=0x116;return false;
            }
            break;
        }
        case 0x105:case 0x253f2f8:extension->stream.Skip(header[1]);break;
        default:if(!error)error=0xabe2;detail=header[0];return false;
        }
    }
    return true;
}
// samp.dll RVA: 0x000bd700
bool CModelGeometry::Read(CModelFileStream *input,WORD version) {
    DWORD value=0;input->Read(&value,2);flags=(WORD)value;
    value=0;input->Read(&value,2);textureSets=(WORD)value;
    value=0;input->Read(&value,4);triangleCount=(int)value;
    value=0;input->Read(&value,4);vertexCount=(int)value;
    value=0;input->Read(&value,4);morphCount=(int)value;
    if((WORD)((version>>6)+0x300)<0x340) {
        value=0;input->Read(&value,4);value=0;input->Read(&value,4);value=0;input->Read(&value,4);
    }
    if(input->ended)goto incomplete;
    if(triangleCount<0||triangleCount>100000){if(!error)error=0xabe5;return false;}
    if(vertexCount<0||vertexCount>200000){if(!error)error=0xabe4;return false;}
    if(flags&8) {
        hasColors=1;colors=calloc(vertexCount,4);
        if(!colors||input->Remaining()<(DWORD)vertexCount*4)goto incomplete;
        input->Read(colors,(DWORD)vertexCount*4);
    }
    if(flags&0x84) {
        uv=calloc(vertexCount,8);if(!uv||input->Remaining()<(DWORD)vertexCount*8)goto incomplete;
        input->Read(uv,(DWORD)vertexCount*8);
    }
    triangles=calloc(triangleCount,8);if(!triangles||input->Remaining()<(DWORD)triangleCount*8)goto incomplete;
    input->Read(triangles,(DWORD)triangleCount*8);if(input->ended)goto incomplete;
    input->Read(sphere,16);hasVertices=input->ReadDword();hasNormals=input->ReadDword();
    if(hasVertices) {
        vertices=calloc(vertexCount,12);if(!vertices||input->Remaining()<(DWORD)vertexCount*12)goto incomplete;
        input->Read(vertices,(DWORD)vertexCount*12);
    }
    if(flags&0x10) {
        normals=calloc(vertexCount,12);if(!normals||input->Remaining()<(DWORD)vertexCount*12)goto incomplete;
        input->Read(normals,(DWORD)vertexCount*12);
    }
    if(!ReadMaterials(input)){if(!error)error=0xabea;return false;}
    if(!ReadExtensions(input)){if(!error)error=0xabeb;return false;}
    return true;
incomplete:
    if(!error)error=0xabe1;return false;
}
// samp.dll RVA: 0x000bbf00
DWORD CDffFile::ReadGeometries() {
    if(!geometryList)return 0;
    geometryList->stream.position=0;geometryList->stream.ended=0;
    DWORD header[3];geometryList->stream.Read(header,12);
    if(header[0]!=1||header[1]!=4){if(!error)error=0xabe7;return 0;}
    DWORD count=0;geometryList->stream.Read(&count,4);
    if(count!=1){if(!error)error=0xabe6;return 0;}
    CModelFileChunk *chunk=ReadChunk(&geometryList->stream);
    if(chunk) {
        if(chunk->type==15) {
            chunk->stream.Read(header,12);
            if(header[0]==1) {
                CModelGeometry *geometry=new CModelGeometry;
                if(!geometry->Read(&chunk->stream,chunk->version)) {
                    if(!error)error=geometry->error;detail=geometry->detail;delete geometry;return 0;
                }
                geometries.Insert(geometry);
            }
        }
        delete chunk;
    }
    return 1;
}
