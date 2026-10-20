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

// Texture dictionary reference/slot adapters. Only the native -1 gates are
// applied; other indices pass through unchanged to their GTA backends.
// samp.dll RVA: 0x000b34f0
void *GameTxdAddRef(int index) { return ((void*(__cdecl*)(int))0x731a00)(index); }
// samp.dll RVA: 0x000b3500
void GameTxdRemoveRef(int index) { ((void(__cdecl*)(int))0x731a30)(index); }
// samp.dll RVA: 0x000b35b0
void GameTxdRemove(int index) { if(index!=-1)((void(__cdecl*)(int))0x731e90)(index); }
// samp.dll RVA: 0x000b35d0
void GameTxdRemoveSlot(int index) { if(index!=-1)((void(__cdecl*)(int))0x731cd0)(index); }
// samp.dll RVA: 0x000b35f0
DWORD GameTxdGetReferenceCount(int index) { return index==-1?0xffffffffu:((DWORD(__cdecl*)(int))0x731aa0)(index); }

// Native performs name lookup, then removes the slot only when it is not -1.
// samp.dll RVA: 0x000b3580
void GameTxdRemoveSlotByName(const char *name) {
    int index=GameTxdFind(name);
    if(index!=-1)((void(__cdecl*)(int))0x731cd0)(index);
}

#include "game/input.h"
// GTA SA virtual addresses: USA7C7550, fallback7C7590. Exactly one argument.
// samp.dll RVA: 0x000b1ce0
void *GameRwGetGeometrySkin(void *geometry) {
    DWORD target=g_gameVersion==1?0x7c7550:0x7c7590;
    return ((void*(__cdecl*)(void*))target)(geometry);
}
// GTA SA virtual addresses: backend74BDD0/+50; initialization flag C97B20.
// Callback addresses are opaque GTA code pointers; no invented callable types.
// samp.dll RVA: 0x000b1da0
int GameRwRegisterClumpPlugin(DWORD pluginId) {
    DWORD target=g_gameVersion==1?0x74bdd0:0x74be20;
    *(DWORD*)0xc97b20=0;
    int result=((int(__cdecl*)(DWORD,DWORD,DWORD,DWORD,DWORD))target)(0,pluginId,0x41b1a0,0x41b1c0,0x41b1b0);
    *(DWORD*)0xc97b20=1;
    return result;
}
// GTA SA virtual addresses: backend74BE70/+50, callbacks41B2F0/41B300.
// Native forces the flag back to1, not to its previous value, even on failure.
// samp.dll RVA: 0x000b1e00
int GameRwRegisterClumpPluginStream(DWORD pluginId,DWORD readCallback) {
    DWORD target=g_gameVersion==1?0x74be70:0x74bec0;
    *(DWORD*)0xc97b20=0;
    int result=((int(__cdecl*)(DWORD,DWORD,DWORD,DWORD))target)(pluginId,readCallback,0x41b2f0,0x41b300);
    *(DWORD*)0xc97b20=1;
    return result;
}
// GTA SA virtual addresses: USA7F3560, fallback7F35A0; fixed enable argument1.
// samp.dll RVA: 0x000b1e60
void GameRwEnableAutoMipmapping() {
    DWORD target=g_gameVersion==1?0x7f3560:0x7f35a0;
    ((void(__cdecl*)(DWORD))target)(1);
}
