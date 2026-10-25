#include "game/custommodels.h"
#include <stdlib.h>
#include <string.h>
// samp.dll RVA: 0x0000b990
void CCustomModels::ClearTransferStorage() {
    memset(nativeState+0x222,0,0x11e);memset(nativeState+0x1345,0,0xd20);
}
// samp.dll RVA: 0x0000d890
CCustomModels::CCustomModels() {
    *(DWORD*)nativeState=0;*(DWORD*)(nativeState+4)=0;nativeState[8]=0;
    memset(nativeState+9,0,261);
    *(DWORD*)(nativeState+0x213)=0;nativeState[0x21f]=0;nativeState[0x221]=0;nativeState[0x220]=0;
    *(DWORD*)(nativeState+0x21b)=0;*(DWORD*)(nativeState+0x217)=0;
    ClearTransferStorage();*(DWORD*)(nativeState+0x2065)=0;*(DWORD*)(nativeState+0x2069)=0;nativeState[0x1344]=0;
}
// samp.dll RVA: 0x0000c4e0
bool CCustomModels::ResizeRecords(DWORD count) {
    BYTE **&records=*(BYTE***)nativeState;DWORD &previous=*(DWORD*)(nativeState+4);
    if(!count){if(records){free(records);records=0;}previous=0;return true;}
    BYTE **replacement=(BYTE**)realloc(records,count*4);if(!replacement)return false;
    records=replacement;for(DWORD i=previous;i<count;++i)records[i]=0;previous=count;return true;
}
// samp.dll RVA: 0x0000c6a0
void CCustomModels::TrimRecords() {
    DWORD count=*(DWORD*)(nativeState+4),last=0xffffffff;BYTE **records=*(BYTE***)nativeState;
    for(DWORD i=0;i<count;++i)if(records[i])last=i;
    ResizeRecords(last==0xffffffff?0:last+1);
}
// samp.dll RVA: 0x0000cfc0
bool CCustomModels::EraseRecord(DWORD index) {
    if(index>=*(DWORD*)(nativeState+4))return false;
    (*(BYTE***)nativeState)[index]=0;TrimRecords();return true;
}
// samp.dll RVA: 0x0000cff0
bool CCustomModels::SetRecord(DWORD index,void *record) {
    if(index>=*(DWORD*)(nativeState+4)&&!ResizeRecords(index+1))return false;
    (*(void***)nativeState)[index]=record;return true;
}
// samp.dll RVA: 0x0000d550
DWORD CCustomModels::AddRecord(DWORD index,const void *record) {
    void *copy=calloc(1,94);int base=*(const int*)((const BYTE*)record+0xc);
    if(base<0||base>20000)return 0xffffffff;
    memcpy(copy,record,94);SetRecord(index,copy);return index;
}
// samp.dll RVA: 0x0000d5a0
DWORD CCustomModels::DeleteRecord(DWORD index) {
    if(index>=*(DWORD*)(nativeState+4))return 0xffffffff;
    void *record=(*(void***)nativeState)[index];if(!record)return 0xffffffff;
    free(record);EraseRecord(index);return index;
}
// samp.dll RVA: 0x0000d5e0
void CCustomModels::ClearRecords() {ResizeRecords(0);}
// samp.dll RVA: 0x0000bd10
void CCustomModels::StopWorkers() {
    for(int i=0;i<6;++i)nativeState[0x134c+i*0x230]=1;nativeState[0x1344]=0;
}
// samp.dll RVA: 0x0000bd90
void CCustomModels::ResumeReadyWorker() {
    if(!nativeState[0x1344])return;
    for(int i=0;i<6;++i) {
        BYTE *worker=nativeState+0x1349+i*0x230;
        if(worker[2]&&!worker[0]&&worker[1]){worker[1]=0;return;}
    }
}

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
#include "BitStream.h"
#include "RakClientInterface.h"
#include "net/netgame.h"
#include "gui/render_dependencies.h"
// samp.dll RVA: 0x00117c48
const char g_customDffCachePattern[]="%s/0x%X.dff";
// samp.dll RVA: 0x00117c54
const char g_customTxdCachePattern[]="%s/0x%X.txd";
// samp.dll RVA: 0x00117d60
int g_customRequestDffRpc=181;
// samp.dll RVA: 0x00117d5c
int g_customRequestTxdRpc=182;
// samp.dll RVA: 0x00117d64
int g_customRequestsCompleteRpc=184;
// samp.dll RVA: 0x0000bb10
bool CCustomModels::IsDffCached(DWORD hash) {
    char path[264];_snprintf(path,260,g_customDffCachePattern,(char*)this+9,hash);
    if(GameFileExists(path))return true;
    _snprintf(path,260,g_customDffCachePattern,(char*)this+0x10e,hash);return GameFileExists(path)!=0;
}
// samp.dll RVA: 0x0000bba0
bool CCustomModels::IsTxdCached(DWORD hash) {
    char path[264];_snprintf(path,260,g_customTxdCachePattern,(char*)this+9,hash);
    if(GameFileExists(path))return true;
    _snprintf(path,260,g_customTxdCachePattern,(char*)this+0x10e,hash);return GameFileExists(path)!=0;
}
// samp.dll RVA: 0x0000bd40
bool CCustomModels::PauseReadyWorker() {
    BYTE *state=(BYTE*)this;if(!state[0x1344])return false;
    for(int i=0;i<6;++i) {
        BYTE *worker=state+0x1349+i*0x230;
        if(worker[2]&&!worker[0]&&!worker[1]){worker[1]=1;return true;}
    }
    return false;
}
// samp.dll RVA: 0x0000bde0
int CCustomModels::CountReadyWorkers() {
    BYTE *state=(BYTE*)this;int count=0;
    for(int i=0;i<6;++i) {
        BYTE *worker=state+0x1349+i*0x230;if(worker[2]&&!worker[0]&&!worker[1])++count;
    }
    return count;
}
// samp.dll RVA: 0x0000c6e0
bool CCustomModels::IsDffRequested(DWORD hash) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;++i) {
        BYTE *entry=(*(BYTE***)state)[i];if(entry&&*(DWORD*)(entry+0x46)==hash&&entry[4])return true;
    }
    return false;
}
// samp.dll RVA: 0x0000c720
bool CCustomModels::IsTxdRequested(DWORD hash) {
    BYTE *state=(BYTE*)this;DWORD count=*(DWORD*)(state+4);
    for(DWORD i=0;i<count;++i) {
        BYTE *entry=(*(BYTE***)state)[i];if(entry&&*(DWORD*)(entry+0x4a)==hash&&entry[5])return true;
    }
    return false;
}
// samp.dll RVA: 0x0000d270
BOOL CCustomModels::RequestResource(DWORD index,BYTE type,DWORD hash,DWORD size) {
    (void)index;(void)size;if(!g_pNetGame)return FALSE;
    if(!*(DWORD*)(g_renderDownloadWindow->nativeState+0x30))g_renderDownloadWindow->Show(MarkResourceRequests());
    RakNet::BitStream stream;stream.WriteBits((BYTE*)&hash,32,true);
    if(type==1)(*(RakClientInterface**)(((CNetGame*)g_pNetGame)->nativeState+0x2c))->RPC(&g_customRequestDffRpc,&stream,(PacketPriority)1,(PacketReliability)9,0,false);
    else if(type==2)(*(RakClientInterface**)(((CNetGame*)g_pNetGame)->nativeState+0x2c))->RPC(&g_customRequestTxdRpc,&stream,(PacketPriority)1,(PacketReliability)9,0,false);
    *(DWORD*)((BYTE*)this+0x213)=1;PauseReadyWorker();return TRUE;
}
// samp.dll RVA: 0x0000d390
void CCustomModels::Process() {
    BYTE *state=(BYTE*)this;if(*(DWORD*)(state+0x213)!=2||state[0x220]||!state[0x21f])return;
    if(state[0x1344]&&!CountReadyWorkers())return;
    DWORD count=*(DWORD*)(state+4),index=0;
    if(count)do {
        BYTE *entry=index<count?(*(BYTE***)state)[index]:0;
        if(entry&&entry[6]!=1&&(entry[7]==1||(entry[7]==2&&(*(DWORD*)(entry+8)==*(DWORD*)(state+0x217)||*(DWORD*)(entry+8)==0xffffffff)))) {
            if(!entry[0]) {
                if(IsDffCached(*(DWORD*)(entry+0x46)))entry[0]=1;
                else if(!IsDffRequested(*(DWORD*)(entry+0x46))) {
                    RequestResource(index,1,*(DWORD*)(entry+0x46),*(DWORD*)(entry+0x4e));entry[4]=1;return;
                }
            }
            if(!entry[1]) {
                if(IsTxdCached(*(DWORD*)(entry+0x4a)))entry[1]=1;
                else if(!IsTxdRequested(*(DWORD*)(entry+0x4a))) {
                    RequestResource(index,2,*(DWORD*)(entry+0x4a),*(DWORD*)(entry+0x52));entry[5]=1;return;
                }
            }
        }
        count=*(DWORD*)(state+4);
    }while(++index!=count);
    if(state[0x21f]&&MarkResourceRequests()<=0) {
        state[0x220]=1;RakNet::BitStream stream;stream.Write(true);
        if(g_pNetGame)(*(RakClientInterface**)(((CNetGame*)g_pNetGame)->nativeState+0x2c))->RPC(&g_customRequestsCompleteRpc,&stream,(PacketPriority)1,(PacketReliability)9,0,false);
    }
}

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
