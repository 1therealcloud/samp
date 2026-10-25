#include "game/custommodels.h"

// samp.dll RVA: 0x002aca28
CCustomModels *g_customModels;
// samp.dll RVA: 0x0000c950
bool CCustomModels::HasEarlierResourceRequest(int endIndex,DWORD hash) {
    BYTE *state=(BYTE*)this;if(!state[0x21f]||endIndex<=0)return false;
    DWORD count=*(DWORD*)(state+4);
    for(int i=0;i<endIndex;++i)if((DWORD)i<count) {
        BYTE *entry=(*(BYTE***)state)[i];if(!entry)continue;
        if((*(DWORD*)(entry+0x46)==hash&&entry[2])||(*(DWORD*)(entry+0x4a)==hash&&entry[3]))return true;
    }
    return false;
}
// samp.dll RVA: 0x0000c9b0
int CCustomModels::MarkResourceRequests() {
    BYTE *state=(BYTE*)this;if(!state[0x21f])return -1;
    DWORD index=0,pending=0,count=*(DWORD*)(state+4);if(!count)return 0;
    do {
        BYTE *entry=index<count?(*(BYTE***)state)[index]:0;
        if((entry[7]==1||(entry[7]==2&&(*(DWORD*)(entry+8)==*(DWORD*)(state+0x217)||*(DWORD*)(entry+8)==0xffffffff)))&&!entry[6]) {
            if(!entry[0]){entry[2]=1;if(!HasEarlierResourceRequest((int)index,*(DWORD*)(entry+0x46)))++pending;}
            if(!entry[1]){entry[3]=1;if(!HasEarlierResourceRequest((int)index,*(DWORD*)(entry+0x4a)))++pending;}
        }
        count=*(DWORD*)(state+4);
    }while(++index!=count);
    return (int)pending;
}

// samp.dll RVA: 0x0000cc80
bool CCustomModels::IsPedEntry(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;i++) {
        BYTE *entry=(*(BYTE***)state)[i];
        if(*(int*)(entry+0x10)==model && entry[7]==1)return true;
    }
    return false;
}
// samp.dll RVA: 0x0000ccc0
bool CCustomModels::IsObjectEntry(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;i++) {
        BYTE *entry=(*(BYTE***)state)[i];
        if(*(int*)(entry+0x10)==model && entry[7]==2)return true;
    }
    return false;
}
// samp.dll RVA: 0x0000cd00
BYTE CCustomModels::GetLoadedFlag(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;i++) {
        BYTE *entry=(*(BYTE***)state)[i];
        if(*(int*)(entry+0x10)==model)return entry[0x58];
    }
    return 0;
}
// samp.dll RVA: 0x0000ce30
BOOL CCustomModels::DoesExist(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;i++) {
        BYTE *entry=(*(BYTE***)state)[i];
        if(*(int*)(entry+0x10)==model && entry[7]==1) {
            if(!LoadPedModel(*(DWORD*)(entry+0xc),*(DWORD*)(entry+0x10),*(DWORD*)(entry+0x46),*(DWORD*)(entry+0x4a)))return FALSE;
            entry[0x59]=0;entry[0x58]=1;return TRUE;
        }
    }
    return FALSE;
}
// samp.dll RVA: 0x0000ce90
BOOL CCustomModels::LoadObjectEntry(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;i++) {
        BYTE *entry=(*(BYTE***)state)[i];
        // This method alone has the observed null-record guard.
        if(entry && *(int*)(entry+0x10)==model && entry[7]==2) {
            if(!entry[0] || !entry[1])return FALSE;
            if(!LoadObjectModel(*(DWORD*)(entry+0xc),*(DWORD*)(entry+0x10),*(DWORD*)(entry+0x46),*(DWORD*)(entry+0x4a)))return FALSE;
            entry[0x59]=0;entry[0x58]=1;return TRUE;
        }
    }
    return FALSE;
}

// The reference entry is exactly RET4; this empty behavior is native.
// samp.dll RVA: 0x0000cd40
void CCustomModels::ReleaseObjectEntry(int model) { (void)model; }

#include "game/custom_model_io.h"
#include "game/game.h"
#include "game/effects.h"
#include "main.h"
#include "chatwindow.h"
#include <stdlib.h>
#include <string.h>

// samp.dll RVA: 0x001e0448
DWORD g_gameLoadingCustomModel;

// samp.dll RVA: 0x000a7670
void *GameClonePedModelInfo(int base,int model) {
    void *copy=calloc(0x44,1);
    void *source=GameGetModelInfo(base);
    if(source && GameIsPedModel(base)) {
        memcpy(copy,source,0x44);
        g_gameModelInfoTable[model]=copy;
        return copy;
    }
    // Native validation failure leaks the allocation.
    return 0;
}
// samp.dll RVA: 0x000a76c0
void *GameCloneObjectModelInfo(int base,int model) {
    void *existing=g_gameModelInfoTable[model];
    if(existing)return existing;
    void *copy=calloc(0x20,1);
    void *source=GameGetModelInfo(base);
    if(source && GameIsObjectModelInfo(source)) {
        memcpy(copy,source,0x20);
        g_gameModelInfoTable[model]=copy;
        return copy;
    }
    return 0;
}
// samp.dll RVA: 0x000a7750
BOOL GameAttachModelTxd(int model,const char *name,const char *path) {
    int index=GameTxdFind(name);
    if(index==-1) {
        index=GameTxdAdd(name);
        if(!GameFileExists(path))return FALSE;
        GameTxdLoad(index,path);
    }
    GameSetModelTxd(model,index);
    GameTxdPush();
    GameTxdSetCurrent(index);
    return TRUE;
}
// samp.dll RVA: 0x000a77c0
BOOL GameRegisterCustomPed(int base,int model,const char *name,const char *dff,const char *txd) {
    if(GameClonePedModelInfo(base,model) && GameAttachModelTxd(model,name,txd)) {
        void *clump=GameLoadPedDff(dff);
        if(clump) { GamePublishPedClump(model,clump);return TRUE; }
    }
    return FALSE;
}
// samp.dll RVA: 0x000a7820
BOOL GameRegisterCustomObject(int base,int model,const char *name,const char *dff,const char *txd) {
    g_gameLoadingCustomModel=model;
    if(GameCloneObjectModelInfo(base,model)) {
        if(!GameAttachModelTxd(model,name,txd)) {
            if(g_pChat)g_pChat->AddDebugMessage("[error] Failed to load texture for model ID: %d",model);
        } else {
            if(GameLoadObjectDff(dff,model))return TRUE;
            if(g_pChat)g_pChat->AddDebugMessage("[error] Failed to load model ID: %d",model);
        }
    }
    return FALSE;
}
// samp.dll RVA: 0x000b41d0
void __stdcall GamePublishPedClump(int model,void *clump) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    *(void**)(info+0x1c)=clump;
    ((void(__fastcall*)(void*,void*,void*))(*(DWORD**)info)[16])(info,0,clump);
}
// samp.dll RVA: 0x000b34a0
int GameTxdAdd(const char *name) {
    // GTA SA virtual address.
    return ((int(__cdecl*)(const char*))0x731c80)(name);
}
// samp.dll RVA: 0x000b34c0
DWORD GameTxdLoad(int index,const char *path) {
    // Native wrapper zero-extends the GTA result's AL.
    return ((BYTE(__cdecl*)(int,const char*))0x7320b0)(index,path);
}
// samp.dll RVA: 0x000b4260
void __stdcall GameSetModelTxd(int model,int index) {
    *(WORD*)((BYTE*)GameGetModelInfo(model)+0xa)=(WORD)index;
}
// samp.dll RVA: 0x000b4330
void __stdcall GameSetModelField8(int model,WORD value) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    if(info)*(WORD*)(info+8)=value;
}
// samp.dll RVA: 0x000b40e0
BOOL __stdcall GameIsObjectModelInfo(void *info) {
    if(!info)return FALSE;
    // GTA SA vtable virtual addresses.
    DWORD table=*(DWORD*)info;
    return table==0x85bbf0 || table==0x85bc30 || table==0x85bc70 ||
        table==0x85bcb0 || table==0x85bcf0;
}

#include <stdio.h>
#include <sys/stat.h>

// samp.dll RVA: 0x000b5900
BOOL GameFileExists(const char *path) { struct _stat state;return _stat(path,&state)==0; }

// samp.dll RVA: 0x0000c1c0
BOOL CCustomModels::LoadPedModel(DWORD base,DWORD model,DWORD dffHash,DWORD txdHash) {
    char name[28],txd[264],dff[264];
    const char *primary=(const char*)this+9,*fallback=(const char*)this+0x10e;
    sprintf(name,"%X",txdHash);
    sprintf(dff,"%s\\0x%X.dff",primary,dffHash);
    if(!GameFileExists(dff)) {
        sprintf(dff,"%s\\0x%X.dff",fallback,dffHash);
        if(!GameFileExists(dff))return FALSE;
    }
    sprintf(txd,"%s\\0x%X.txd",primary,txdHash);
    if(!GameFileExists(txd)) {
        sprintf(txd,"%s\\0x%X.txd",fallback,txdHash);
        if(!GameFileExists(txd))return FALSE;
    }
    return GameRegisterCustomPed(base,model,name,dff,txd)!=0;
}

// samp.dll RVA: 0x0000c2e0
BOOL CCustomModels::LoadObjectModel(DWORD base,DWORD model,DWORD dffHash,DWORD txdHash) {
    char name[28],txd[264],dff[264];
    const char *primary=(const char*)this+9,*fallback=(const char*)this+0x10e;
    sprintf(name,"%X",txdHash);
    sprintf(dff,"%s\\0x%X.dff",primary,dffHash);
    if(!GameFileExists(dff)) {
        sprintf(dff,"%s\\0x%X.dff",fallback,dffHash);
        if(!GameFileExists(dff))return FALSE;
    }
    sprintf(txd,"%s\\0x%X.txd",primary,txdHash);
    if(!GameFileExists(txd)) {
        sprintf(txd,"%s\\0x%X.txd",fallback,txdHash);
        if(!GameFileExists(txd))return FALSE;
    }
    return GameRegisterCustomObject(base,model,name,dff,txd)!=0;
}


// samp.dll RVA: 0x000b19c0
void *GameRwStreamOpen(int type,int access,const void *data) {
    // GTA SA virtual addresses; every version other than 1 uses the EU path.
    DWORD target=g_gameVersion==1?0x7ecef0:0x7ecf30;
    return ((void*(__cdecl*)(int,int,const void*))target)(type,access,data);
}
// samp.dll RVA: 0x000b1a00
BOOL GameRwStreamClose(void *stream,void *data) {
    DWORD target=g_gameVersion==1?0x7ece20:0x7ece60;
    return ((BOOL(__cdecl*)(void*,void*))target)(stream,data);
}
// samp.dll RVA: 0x000b1a80
BOOL GameRwFindChunk(void *stream,DWORD type,DWORD *length,DWORD *version) {
    DWORD target=g_gameVersion==1?0x7ed2d0:0x7ed310;
    return ((BOOL(__cdecl*)(void*,DWORD,DWORD*,DWORD*))target)(stream,type,length,version);
}
// samp.dll RVA: 0x000b1ad0
void *GameRwReadClump(void *stream) {
    DWORD target=g_gameVersion==1?0x74b420:0x74b470;
    return ((void*(__cdecl*)(void*))target)(stream);
}
// samp.dll RVA: 0x000b1ba0
DWORD GameLoadAtomicStream(void *stream,int model) {
    // GTA SA virtual address; preserve zero-extension of AL.
    return ((BYTE(__cdecl*)(void*,int))0x5371f0)(stream,model);
}
// samp.dll RVA: 0x000b1b10
void *GameLoadPedDff(const char *path) {
    void *stream=GameRwStreamOpen(2,1,path);
    if(!stream)return 0;
    // GTA SA virtual address, restored to the native fixed byte, not a snapshot.
    BYTE *patch=(BYTE*)0x41b1d0;
    GameUnprotect(patch,1);
    *patch=0xc3;
    g_gameCustomStreamRead=true;
    void *clump=0;
    if(GameRwFindChunk(stream,0x10,0,0))clump=GameRwReadClump(stream);
    GameRwStreamClose(stream,0);
    *patch=0x64;
    g_gameCustomStreamRead=false;
    return clump;
}
// samp.dll RVA: 0x000b1bc0
BOOL GameLoadObjectDff(const char *path,int model) {
    void *stream=GameRwStreamOpen(2,1,path);
    if(!stream)return FALSE;
    BYTE *patch=(BYTE*)0x41b1d0;
    GameUnprotect(patch,1);
    *patch=0xc3;
    g_gameCustomStreamRead=true;
    if(!GameLoadAtomicStream(stream,model)) {
        // Native failure leaves the stream open.
        *patch=0x64;
        g_gameCustomStreamRead=false;
        return FALSE;
    }
    GameRwStreamClose(stream,0);
    GameSetModelField8(model,0);
    *patch=0x64;
    g_gameCustomStreamRead=false;
    return TRUE;
}

// samp.dll RVA: 0x0000cdf0
DWORD CCustomModels::GetObjectTextureHash(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;++i) {
        BYTE *entry=(*(BYTE***)state)[i];
        if(entry && *(int*)(entry+0x10)==model && entry[7]==2)return *(DWORD*)(entry+0x4a);
    }
    return 0;
}

// samp.dll RVA: 0x0000cf00
BYTE CCustomModels::GetObjectFirstHour(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;++i) {
        BYTE *entry=(*(BYTE***)state)[i];
        if(entry && *(int*)(entry+0x10)==model && entry[7]==2)return entry[0x56];
    }
    return 0;
}

// samp.dll RVA: 0x0000cf40
BYTE CCustomModels::GetObjectLastHour(int model) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;++i) {
        BYTE *entry=(*(BYTE***)state)[i];
        if(entry && *(int*)(entry+0x10)==model && entry[7]==2)return entry[0x57];
    }
    return 0;
}
