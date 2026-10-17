#include "game/effects.h"
#include "game/preinit.h"
// samp.dll RVA: 0x000bc1d0
DWORD GameBufferReader::Read(void *out,DWORD count) {
    memset(out,0,count);
    if(position+count>length)count=length-position;
    if(!count)return 0;
    memcpy(out,data+position,count);
    if(position+count<length)position+=count;else { position=length;eof=true; }
    return count;
}
// samp.dll RVA: 0x000b3480
int GameTxdFind(const char *name) { return ((int(__cdecl*)(const char*))0x731850)(name); }
// samp.dll RVA: 0x000b3510
void GameTxdPush() { ((void(__cdecl*)())0x7316a0)(); }
// samp.dll RVA: 0x000b3520
void GameTxdPop() { ((void(__cdecl*)())0x7316b0)(); }
// samp.dll RVA: 0x000b3530
void GameTxdSetCurrent(int index) { ((void(__cdecl*)(int))0x7319c0)(index); }
// samp.dll RVA: 0x000b3540
void *GameTxdReadTexture(const char *name) {
    // Native constructs a one-pointer CSprite2d on the stack; this is an out-object, not the EAX result.
    void *texture=0;
    __asm push name
    __asm lea ecx,texture
    __asm mov edx,0x727270
    __asm call edx
    return texture;
}
// samp.dll RVA: 0x000b2860
void *GameLoadDictionaryTexture(const char *dictionary,const char *name) {
    int index=GameTxdFind(dictionary);if(index==-1)return 0;
    GameTxdPush();GameTxdSetCurrent(index);
    void *texture=GameTxdReadTexture(name);GameTxdPop();return texture;
}
// samp.dll RVA: 0x000b6610
void GameParseStreamData(const void *data,DWORD length,void *destination) {
    GameBufferReader *reader=new GameBufferReader(data,length);
    DWORD count=0;reader->Read(&count,4);
    if(count==0 || count>128) { delete reader;return; }
    DWORD selected=0;
    for(DWORD i=0;i<count;i++) {
        if(reader->eof)break;
        BYTE position[12];DWORD type=0,size=0;
        reader->Read(position,12);reader->Read(&type,4);reader->Read(&size,4);
        if((type==0 && (size==76 || size==80)) || (type==10 && size==40))++selected;
        reader->Skip(size);
    }
    if(!selected)return; // Native leaks the reader on this path; preserve it.
    BYTE *result=(BYTE*)calloc(1,selected*64+4);*(DWORD*)result=selected;
    reader->position=0;reader->eof=false;
    DWORD ignoredCount=0;reader->Read(&ignoredCount,4);
    DWORD written=0;BYTE *record=result+4;
    do {
        if(reader->eof)break;
        BYTE position[12];DWORD type=0,size=0;
        reader->Read(position,12);reader->Read(&type,4);reader->Read(&size,4);
        if(type==0 && (size==76 || size==80)) {
            BYTE effect[56]={0};memcpy(effect,position,12);
            reader->Read(effect+16,4); // Color.
            reader->Read(effect+20,4);reader->Read(effect+24,4);reader->Read(effect+28,4);reader->Read(effect+32,4);
            reader->Read(effect+38,1);reader->Read(effect+39,1);reader->Read(effect+40,1);reader->Read(effect+41,1);
            BYTE low=0,high=0,discard=0;reader->Read(&low,1);
            char first[65]={0},second[65]={0};reader->Read(first,24);reader->Read(second,24);
            reader->Read(effect+42,1);reader->Read(&high,1);*(WORD*)(effect+36)=low|((WORD)high<<8);
            if(size==80) { reader->Read(effect+43,1);reader->Read(effect+44,1);reader->Read(effect+45,1);reader->Read(&discard,1); }
            else { effect[43]=0;effect[44]=0;effect[45]=100; }
            reader->Read(&discard,1);
            if(strlen(first))*(void**)(effect+48)=GameLoadDictionaryTexture("particle",first);
            if(strlen(second))*(void**)(effect+52)=GameLoadDictionaryTexture("particle",second);
            memcpy(record,effect,56);record+=64;++written;
        } else if(type==10 && size==40) {
            BYTE effect[53]={0};memcpy(effect,position,12);*(DWORD*)(effect+12)=10;
            reader->Read(effect+16,12);reader->Read(effect+28,12);reader->Read(effect+40,12);
            DWORD value=0;reader->Read(&value,4);effect[52]=(BYTE)value;
            memcpy(record,effect,53);record+=64;++written;
        } else reader->Skip(size);
    } while(written!=selected);
    if(!written)free(result);
    else { *(DWORD*)result=written;*(void**)((BYTE*)destination+0x78)=result; }
    delete reader;
}
