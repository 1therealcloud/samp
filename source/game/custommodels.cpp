#include "game/custommodels.h"
#include "game/modelfile.h"
#include <stdlib.h>
#include <string.h>
// samp.dll RVA: 0x00117cd8
const char g_customDeleteDffPattern[]="%s\\0x%X.dff";
// samp.dll RVA: 0x00117ce4
const char g_customDeleteTxdPattern[]="%s\\0x%X.txd";
// samp.dll RVA: 0x00117cf0
const char g_customDeleteTempPattern[]="%s\\%s";
// samp.dll RVA: 0x00117d24
const char g_customFindTempPattern[]="%s/*.temp";
// samp.dll RVA: 0x00117d18
const char g_customFindDffPattern[]="%s/*.dff";
// samp.dll RVA: 0x00117d0c
const char g_customFindTxdPattern[]="%s/*.txd";
// samp.dll RVA: 0x0000c0b0
DWORD CCustomModels::ParseCacheHash(const char *filename) {
    if(filename[0]!='0'||filename[1]!='x')return 0;
    char digits[12];strncpy(digits,filename+2,8);return strtoul(digits,0,16);
}
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
#include "game/hooks.h"
#include "net/netgame.h"
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
// samp.dll RVA: 0x00152ae8
const GameModelReferencePatch g_gameModelReferencePatchesUS[707]={
    {0x0040122a,0x8b},{0x00403da4,0x8b},{0x00403ecc,0x8b},{0x00405cdf,0x8b},{0x00405e50,0x8b},{0x004063f1,0x8b},
    {0x00406b11,0x8b},{0x00407b21,0x8b},{0x00407b72,0x8b},{0x00407b92,0x8b},{0x004080b4,0x8b},{0x004082f4,0x8b},
    {0x004083a4,0x8b},{0x00408894,0x8b},{0x004088a8,0x8b},{0x00408903,0x8b},{0x004089cf,0x8b},{0x00408cd0,0x8b},
    {0x00408d1d,0x8b},{0x00408ec0,0x8b},{0x004093f4,0x8b},{0x004097b3,0x8b},{0x004098a6,0x8b},{0x004098fc,0x8b},
    {0x0040999e,0x8b},{0x00409a4b,0x8b},{0x00409c74,0x8b},{0x00409cf5,0x8b},{0x00409d19,0x8b},{0x00409ffd,0x8b},
    {0x0040a2b8,0x8b},{0x0040a2e0,0x8b},{0x0040a308,0x8b},{0x0040a330,0x8b},{0x0040a358,0x8b},{0x0040a380,0x8b},
    {0x0040a5b2,0x8b},{0x0040a6cc,0x8b},{0x0040a731,0x8b},{0x0040a8e5,0x8b},{0x0040ac1c,0x8b},{0x0040ae8b,0x8b},
    {0x0040bb22,0x8b},{0x0040bedd,0x8b},{0x0040c04b,0x8b},{0x0040c0bc,0x8b},{0x0040c12a,0x8b},{0x0040c300,0x8b},
    {0x0040c4c2,0x8b},{0x0040c6fe,0x8b},{0x0040cd64,0x8b},{0x0040ce6e,0x8b},{0x0040ce83,0x8b},{0x0040d05d,0x8b},
    {0x0040e207,0x8b},{0x0040e23b,0x8b},{0x0041a8de,0x8b},{0x0041aa49,0x8b},{0x0041aa79,0x8b},{0x0041ab98,0x8b},
    {0x0041d77e,0x8b},{0x0042145a,0x8b},{0x0042187b,0x8b},{0x00424204,0x8b},{0x00425467,0x8b},{0x00425506,0x8b},
    {0x00425b82,0x8b},{0x00425bad,0x8b},{0x00425fa9,0x8b},{0x00425fbe,0x8b},{0x004262be,0x8b},{0x004262cc,0x8b},
    {0x00426498,0x8b},{0x004264f4,0x8b},{0x00426561,0x8b},{0x004265c7,0x8b},{0x00426662,0x8b},{0x004266ba,0x8b},
    {0x00426727,0x8b},{0x00426790,0x8b},{0x004267f4,0x8b},{0x00426806,0x8b},{0x00426a2e,0x8b},{0x00426c50,0x8b},
    {0x00426d6d,0x8b},{0x0042c48d,0x8b},{0x0042c797,0x8b},{0x0042db81,0x8b},{0x0042dc2a,0x8b},{0x0042dd34,0x8b},
    {0x0042dd97,0x8b},{0x0042dded,0x8b},{0x0042eca2,0x8b},{0x00430633,0x8b},{0x00430735,0x8b},{0x00430947,0x8b},
    {0x00431b46,0x8b},{0x00431bd2,0x8b},{0x00431f0e,0x8b},{0x00436b31,0x8b},{0x00436b42,0x8b},{0x0043772c,0x8b},
    {0x0043773d,0x8b},{0x00437797,0x8b},{0x004377a8,0x8b},{0x004377e7,0x8b},{0x004377f8,0x8b},{0x00437842,0x8b},
    {0x00437853,0x8b},{0x0043a162,0x8b},{0x0043a345,0x8b},{0x0043b3e3,0x8b},{0x0043d546,0x8b},{0x0043d7d3,0x8b},
    {0x0043d883,0x8b},{0x0043d89e,0x8b},{0x00447eeb,0x8b},{0x00448af9,0x8b},{0x00448eee,0x8b},{0x00449c34,0x8b},
    {0x0044a52a,0x8b},{0x0044b11e,0x8b},{0x00453173,0x8b},{0x004531ae,0x8b},{0x004531e4,0x8b},{0x00453220,0x8b},
    {0x00453490,0x8b},{0x004534cb,0x8b},{0x00453501,0x8b},{0x0045353d,0x8b},{0x00454b1e,0x8b},{0x00455c0e,0x8b},
    {0x00456802,0x8b},{0x00456816,0x8b},{0x004613fd,0x8b},{0x00461c2d,0x8b},{0x004624f5,0x8b},{0x004657a2,0x8b},
    {0x0046978d,0x8b},{0x0046a773,0x8b},{0x0046b5f7,0x8b},{0x00472950,0x8b},{0x00474806,0x8b},{0x00474aad,0x8b},
    {0x00475d91,0x8b},{0x00478505,0x8b},{0x00478552,0x8b},{0x004788d7,0x8b},{0x004794df,0x8b},{0x0047aea3,0x8b},
    {0x0047f50f,0x8b},{0x0047fdc0,0x8b},{0x0047fdfc,0x8b},{0x0047fe52,0x8b},{0x00481e29,0x8b},{0x0048229f,0x8b},
    {0x00482560,0x8b},{0x004829d6,0x8b},{0x00483365,0x8b},{0x0048337f,0x8b},{0x00484d39,0x8b},{0x00486b1d,0x8b},
    {0x0048aca2,0x8b},{0x00494edc,0x8b},{0x0049835e,0x8b},{0x004983a6,0x8b},{0x004986fb,0x8b},{0x00499a10,0x8b},
    {0x0049b0ec,0x8b},{0x0049b0f9,0x8b},{0x0049db82,0x8b},{0x004af46b,0x8b},{0x004b313c,0x8b},{0x004b49b3,0x8b},
    {0x004c0c13,0x8b},{0x004c4860,0x8b},{0x004c4bed,0x8b},{0x004c5538,0x89},{0x004c5960,0x8b},{0x004c5980,0x8b},
    {0x004c59c0,0x8b},{0x004c5a40,0x8b},{0x004c5a75,0x8b},{0x004c5a89,0x8b},{0x004c5aa5,0x8b},{0x004c5ab9,0x8b},
    {0x004c5ad5,0x8b},{0x004c5ae9,0x8b},{0x004c5b05,0x8b},{0x004c5b19,0x8b},{0x004c5b35,0x8b},{0x004c5b49,0x8b},
    {0x004c5b65,0x8b},{0x004c5b79,0x8b},{0x004c5b95,0x8b},{0x004c5ba9,0x8b},{0x004c5bc5,0x8b},{0x004c5bd9,0x8b},
    {0x004c5bf5,0x8b},{0x004c5c09,0x8b},{0x004c5c25,0x8b},{0x004c5c39,0x8b},{0x004c5c55,0x8b},{0x004c5c69,0x8b},
    {0x004c5c90,0x8b},{0x004c63bb,0xbe},{0x004c6642,0x89},{0x004c6671,0x89},{0x004c66a1,0x89},{0x004c66d1,0x89},
    {0x004c6701,0x89},{0x004c6731,0x89},{0x004c6761,0x89},{0x004c6795,0x89},{0x004c67c2,0x89},{0x004c681a,0xbf},
    {0x004c85d6,0x39},{0x004c89b8,0x8b},{0x004d3d52,0x8b},{0x004dacae,0x8b},{0x004e4204,0x8b},{0x004e58c4,0x8b},
    {0x004e693a,0x8b},{0x004e6970,0x8b},{0x00509cae,0x8b},{0x0050d59f,0x8b},{0x005135a2,0x8b},{0x00513634,0x8b},
    {0x00514c80,0x8b},{0x00516e1a,0x8b},{0x005172f2,0x8b},{0x005180ef,0x8b},{0x0051d233,0x8b},{0x0052034f,0x8b},
    {0x00532a58,0x8b},{0x00532b7b,0x8b},{0x00532d01,0x8b},{0x00532d44,0x8b},{0x00533162,0x8b},{0x00533247,0x8b},
    {0x00533389,0x8b},{0x0053341d,0x8b},{0x00533706,0x8b},{0x005337aa,0x8b},{0x00533bfa,0x8b},{0x00533d46,0x8b},
    {0x00533f30,0x8b},{0x00533f86,0x8b},{0x00533fbe,0x8b},{0x00533fdf,0x8b},{0x005340a5,0x8b},{0x005340d7,0x8b},
    {0x0053412a,0x8b},{0x00534254,0x8b},{0x00534294,0x8b},{0x005342bd,0x8b},{0x005343fa,0x8b},{0x0053441d,0x8b},
    {0x0053443b,0x8b},{0x00534449,0x8b},{0x005344d8,0x8b},{0x0053454a,0x8b},{0x00534568,0x8b},{0x005345f2,0x8b},
    {0x00534fb3,0x8b},{0x00535326,0x8b},{0x005354c8,0x8b},{0x00535fb0,0x8b},{0x00536c0b,0x8b},{0x00537155,0x8b},
    {0x005371f6,0x8b},{0x005372d7,0x8b},{0x005373f4,0x8b},{0x00537457,0x8b},{0x0053748d,0x8b},{0x005380b2,0x8b},
    {0x00538124,0x8b},{0x005383bb,0x8b},{0x005384ff,0x8b},{0x0053a70d,0x8b},{0x0053c449,0x8b},{0x005449c4,0x8b},
    {0x00546680,0x8b},{0x00546d28,0x8b},{0x00547fdb,0x8b},{0x005483e7,0x8b},{0x0054a998,0x8b},{0x0054ba95,0x8b},
    {0x0054c6c2,0x8b},{0x0054c817,0x8b},{0x0054d030,0x8b},{0x005537f0,0x8b},{0x00553898,0x8b},{0x00553a64,0x8b},
    {0x00553b14,0x8b},{0x0055423d,0x8b},{0x00554463,0x8b},{0x005544bd,0x8b},{0x00554663,0x8b},{0x005546b9,0x8b},
    {0x005546f5,0x8b},{0x005549f3,0x8b},{0x00554ed8,0x8b},{0x00554f39,0x8b},{0x00556768,0x8b},{0x005569f7,0x8b},
    {0x00558164,0x8b},{0x00558406,0x8b},{0x005584ca,0x8b},{0x00558688,0x8b},{0x005588e0,0x8b},{0x00558961,0x8b},
    {0x005589d6,0x8b},{0x00558a60,0x8b},{0x00558b4e,0x8b},{0x0055c011,0x8b},{0x0056508c,0x8b},{0x0056512c,0x8b},
    {0x00565266,0x8b},{0x0056537e,0x8b},{0x005662d2,0x8b},{0x0056709e,0x8b},{0x005683cd,0x8b},{0x00569999,0x8b},
    {0x00569bb3,0x8b},{0x00569bde,0x8b},{0x00569cbb,0x8b},{0x00569dc6,0x8b},{0x00571eaa,0x8b},{0x0057206f,0x8b},
    {0x00585ec7,0x8b},{0x0058d80b,0x8b},{0x0058e855,0x8b},{0x00593508,0x8b},{0x00594cce,0x8b},{0x00594fc0,0x8b},
    {0x00595390,0x8b},{0x00596756,0x8b},{0x0059843c,0x8b},{0x005986a5,0x8b},{0x00598dfc,0x8b},{0x0059e7fe,0x8b},
    {0x0059e826,0x8b},{0x0059ef37,0x8b},{0x0059efb1,0x8b},{0x0059f1b4,0x8b},{0x0059f33b,0x8b},{0x0059f6f2,0x8b},
    {0x0059f74d,0x8b},{0x0059f8aa,0x8b},{0x0059f8e0,0x8b},{0x0059f9b9,0x8b},{0x0059fa3d,0x8b},{0x0059fa53,0x8b},
    {0x0059fb5a,0x8b},{0x005a04a2,0x8b},{0x005a06d4,0x8b},{0x005a1b84,0x8b},{0x005a1bb5,0x8b},{0x005a213d,0x8b},
    {0x005a2d06,0x8b},{0x005a2dea,0x8b},{0x005a332f,0x8b},{0x005a3525,0x8b},{0x005a41c6,0x8b},{0x005a81eb,0x8b},
    {0x005b02d0,0x8b},{0x005b02ef,0x8b},{0x005b0610,0x8b},{0x005b062b,0x8b},{0x005b1ae5,0x8b},{0x005b1b69,0x8b},
    {0x005b1b7c,0x8b},{0x005b2d09,0x8b},{0x005b2f7a,0x8b},{0x005b2f9f,0x8b},{0x005b2fca,0x8b},{0x005b2fd1,0x8b},
    {0x005b3088,0x8b},{0x005b30d0,0x8b},{0x005b30d7,0x8b},{0x005b50a1,0x8b},{0x005b5270,0x8b},{0x005b529d,0x8b},
    {0x005b52aa,0x8b},{0x005b76bd,0x8b},{0x005b8c18,0xbf},{0x005b9299,0xbe},{0x005bec46,0x8b},{0x005c010b,0x8b},
    {0x005c0157,0x8b},{0x005d2a8f,0x8b},{0x005dc7f7,0x8b},{0x005df949,0x8b},{0x005dfd98,0x8b},{0x005e255f,0x8b},
    {0x005e39e3,0x8b},{0x005e3a9d,0x8b},{0x005e48b2,0x8b},{0x005e5f07,0x8b},{0x005e72f2,0x8b},{0x005e8ef9,0x8b},
    {0x005e8f0a,0x8b},{0x005f201f,0x8b},{0x005f2f20,0x8b},{0x005f336b,0x8b},{0x005f38b2,0x8b},{0x005f39c1,0x8b},
    {0x005f5720,0x8b},{0x005f5733,0x8b},{0x005fcade,0x8b},{0x005fcbb9,0x8b},{0x005fd093,0x8b},{0x005fd546,0x8b},
    {0x0060039a,0x8b},{0x00603509,0x8b},{0x00605474,0x8b},{0x00606a5b,0x8b},{0x00609a2c,0x8b},{0x0060ffb5,0x8b},
    {0x006100ef,0x8b},{0x00610154,0x8b},{0x00610738,0x8b},{0x006110c4,0x8b},{0x006110f3,0x8b},{0x00611206,0x8b},
    {0x00611454,0x8b},{0x00611474,0x8b},{0x00611494,0x8b},{0x00611764,0x8b},{0x00611794,0x8b},{0x006117b4,0x8b},
    {0x00611cc7,0x8b},{0x00611d04,0x8b},{0x00611d65,0x8b},{0x00611d92,0x8b},{0x00611dd0,0x8b},{0x00611e49,0x8b},
    {0x00611e53,0x8b},{0x00611ed4,0x8b},{0x00611eef,0x8b},{0x0061235c,0x8b},{0x006123af,0x8b},{0x0061259a,0x8b},
    {0x00612eea,0x8b},{0x0061300b,0x8b},{0x0061304c,0x8b},{0x00613065,0x8b},{0x0061307e,0x8b},{0x00613093,0x8b},
    {0x006130ab,0x8b},{0x006130da,0x8b},{0x006130f3,0x8b},{0x006131f2,0x8b},{0x0061321d,0x8b},{0x00613227,0x8b},
    {0x0061326d,0x8b},{0x006132b1,0x8b},{0x0061331d,0x8b},{0x0061333f,0x8b},{0x00613b43,0x8b},{0x00613b5a,0x8b},
    {0x00613cf7,0x8b},{0x00613e15,0x8b},{0x00613e9b,0x8b},{0x00613eed,0x8b},{0x0061417e,0x8b},{0x006142e8,0x8b},
    {0x0061451b,0x8b},{0x0061454b,0x8b},{0x006145dd,0x8b},{0x006146ad,0x8b},{0x006146d5,0x8b},{0x00614a90,0x8b},
    {0x00614c6d,0x8b},{0x00614cf4,0x8b},{0x00614d42,0x8b},{0x00614fe9,0x8b},{0x0061511b,0x8b},{0x006152c3,0x8b},
    {0x00616a9d,0x8b},{0x00616aae,0x8b},{0x00616ae8,0x8b},{0x00616af9,0x8b},{0x0062428b,0x8b},{0x0062439a,0x8b},
    {0x00627d2b,0x8b},{0x00628a3b,0x8b},{0x006291bb,0x8b},{0x0064087e,0x8b},{0x00640b3b,0x8b},{0x00642431,0x8b},
    {0x00642c41,0x8b},{0x006432bf,0x8b},{0x00643357,0x8b},{0x0064916d,0x8b},{0x0064d04d,0x8b},{0x0064e74f,0x8b},
    {0x0064fc21,0x8b},{0x006504de,0x8b},{0x006509bc,0x8b},{0x00650e09,0x8b},{0x00656573,0x8b},{0x0065d35a,0x8b},
    {0x006641b5,0x8b},{0x0066477a,0x8b},{0x00664bf2,0x8b},{0x0066f4d1,0x8b},{0x0066f92a,0x8b},{0x0066f99e,0x8b},
    {0x0066fa11,0x8b},{0x0066fa85,0x8b},{0x0066faf9,0x8b},{0x0066fb6d,0x8b},{0x0066fbe1,0x8b},{0x0066fc55,0x8b},
    {0x0067c82f,0x8b},{0x0067c83d,0x8b},{0x0067d5c6,0x8b},{0x0067dfce,0x8b},{0x0068d9f5,0x8b},{0x0069368a,0x8b},
    {0x0069598c,0x8b},{0x006969c5,0x8b},{0x00696af3,0x8b},{0x0069912e,0x8b},{0x00699268,0x8b},{0x006995ec,0x8b},
    {0x00699701,0x8b},{0x0069a77e,0x8b},{0x0069b909,0x8b},{0x006a07ab,0x8b},{0x006a1577,0x8b},{0x006a26c2,0x8b},
    {0x006a2792,0x8b},{0x006a2871,0x8b},{0x006a2936,0x8b},{0x006a2f88,0x8b},{0x006a307f,0x8b},{0x006a3bdb,0x8b},
    {0x006a452a,0x8b},{0x006a4805,0x8b},{0x006a609e,0x8b},{0x006a65dd,0x8b},{0x006a7178,0x8b},{0x006a8b6e,0x8b},
    {0x006aa2e2,0x8b},{0x006abd16,0x8b},{0x006ae28b,0x8b},{0x006ae2ce,0x8b},{0x006ae302,0x8b},{0x006ae332,0x8b},
    {0x006ae5d5,0x8b},{0x006ae60e,0x8b},{0x006af1ea,0x8b},{0x006af320,0x8b},{0x006af37e,0x8b},{0x006b0581,0x8b},
    {0x006b0b22,0x8b},{0x006b1386,0x8b},{0x006b14b6,0x8b},{0x006b16a7,0x8b},{0x006b16ef,0x8b},{0x006b3f7d,0x8b},
    {0x006b4100,0x8b},{0x006b67ea,0x8b},{0x006b6965,0x8b},{0x006b804c,0x8b},{0x006b89bc,0x8b},{0x006b927d,0x8b},
    {0x006bbc76,0x8b},{0x006bc93c,0x8b},{0x006bd683,0x8b},{0x006be009,0x8b},{0x006bf48a,0x8b},{0x006bf857,0x8b},
    {0x006c0b3f,0x8b},{0x006c1483,0x8b},{0x006c440e,0x8b},{0x006c5446,0x8b},{0x006c7e83,0x8b},{0x006c7fbc,0x8b},
    {0x006c8e60,0x8b},{0x006c94d5,0x8b},{0x006ce3b9,0x8b},{0x006ceaf5,0x8b},{0x006ceea4,0x8b},{0x006cf1aa,0x8b},
    {0x006cfae5,0x8b},{0x006cfd92,0x8b},{0x006d0b79,0x8b},{0x006d0c4c,0x8b},{0x006d0e49,0x8b},{0x006d0e76,0x8b},
    {0x006d2315,0x8b},{0x006d2b02,0x8b},{0x006d31ec,0x8b},{0x006d32df,0x8b},{0x006d3875,0x8b},{0x006d3971,0x8b},
    {0x006d3a21,0x8b},{0x006d429a,0x8b},{0x006d46e9,0x8b},{0x006d5431,0x8b},{0x006d63f4,0x8b},{0x006d649d,0x8b},
    {0x006d64d4,0x8b},{0x006d64fe,0x8b},{0x006d6a4e,0x8b},{0x006d7ed5,0x8b},{0x006db171,0x8b},{0x006de2ea,0x8b},
    {0x006df455,0x8b},{0x006df936,0x8b},{0x006df9c7,0x8b},{0x006dfa2f,0x8b},{0x006dfa3a,0x8b},{0x006dfb48,0x8b},
    {0x006dfb8a,0x8b},{0x006dfbfa,0x8b},{0x006e0a62,0x8b},{0x006e0e2d,0x8b},{0x006e144b,0x8b},{0x006e1607,0x8b},
    {0x006e17b9,0x8b},{0x006e1f35,0x8b},{0x006e22c6,0x8b},{0x006e3296,0x8b},{0x006e3312,0x8b},{0x006e3396,0x8b},
    {0x006f0d46,0x8b},{0x006f2987,0x8b},{0x006f2ff1,0x8b},{0x006f32ea,0x8b},{0x006f3398,0x8b},{0x006f33b2,0x8b},
    {0x006f3596,0x8b},{0x006f3753,0x8b},{0x006f376b,0x8b},{0x006f3990,0x8b},{0x006f6073,0x8b},{0x006f6fbb,0x8b},
    {0x006f7735,0x8b},{0x006f7761,0x8b},{0x006f9132,0x8b},{0x006f9380,0x8b},{0x006f9953,0x8b},{0x006fc805,0x8b},
    {0x007086e9,0x8b},{0x0070a068,0x8b},{0x0070a586,0x8b},{0x0070a73c,0x8b},{0x0070a8c5,0x8b},{0x00711bda,0x8b},
    {0x00711cf6,0x8b},{0x007179e3,0x8b},{0x0071aff4,0x8b},{0x0071bc64,0x8b},{0x0071c0e9,0x8b},{0x0071cff8,0x8b},
    {0x0071d032,0x8b},{0x0071fb44,0x8b},{0x0071fb88,0x8b},{0x0071fc9a,0x8b},{0x0071fd47,0x8b},{0x0071fe09,0x8b},
    {0x0071feda,0x8b},{0x0071ffc2,0x8b},{0x0073227a,0x8b},{0x00732b55,0x8b},{0x00736f18,0x8b},{0x00738409,0x8b},
    {0x0073a398,0x8b},{0x0073a3b9,0x8b},{0x0073b525,0x8b},{0x0073b537,0x8b},{0x0073d5d0,0x8b},{0x0073ec5d,0x8b},
    {0x0074103b,0x8b},{0x01562a35,0x8b},{0x01562b94,0x8b},{0x01564612,0x8b},{0x01564ec2,0x8b},{0x0156526c,0x8b},
    {0x015662f5,0x8b},{0x0156650d,0x8b},{0x0156658a,0x8b},{0x015665c9,0x8b},{0x01566f25,0x8b},{0x015691de,0x8b},
    {0x015691e5,0x8b},{0x0156c247,0x8b},{0x0156c275,0x8b},{0x0156c5b5,0x8b},{0x0156ca5d,0x8b},{0x0156caa2,0x8b},
    {0x0156d089,0x8b},{0x0156d0b6,0x8b},{0x0156d0e6,0x8b},{0x01570248,0x8b},{0x01570270,0x8b},{0x01570298,0x8b},
    {0x015702c0,0x8b},{0x015702e8,0x8b},{0x01570310,0x8b},{0x01570404,0x8b},{0x0157045d,0x8b},
};
// samp.dll RVA: 0x001538b8
const GameModelReferencePatch g_gameModelReferencePatchesEU[707]={
    {0x0040121f,0x8b},{0x00403da4,0x8b},{0x00403ecc,0x8b},{0x00405cdf,0x8b},{0x00405e50,0x8b},{0x004063f1,0x8b},
    {0x00406b0d,0x8b},{0x00407b21,0x8b},{0x00407b72,0x8b},{0x00407b92,0x8b},{0x004080b4,0x8b},{0x004082f4,0x8b},
    {0x004083a4,0x8b},{0x00408894,0x8b},{0x004088a8,0x8b},{0x00408903,0x8b},{0x004089cf,0x8b},{0x00408cd0,0x8b},
    {0x00408d1d,0x8b},{0x00408ec0,0x8b},{0x004093f4,0x8b},{0x004097b3,0x8b},{0x004098a6,0x8b},{0x004098fc,0x8b},
    {0x0040999e,0x8b},{0x00409a4b,0x8b},{0x00409c74,0x8b},{0x00409cf5,0x8b},{0x00409d19,0x8b},{0x00409ffd,0x8b},
    {0x0040a2b8,0x8b},{0x0040a2e0,0x8b},{0x0040a308,0x8b},{0x0040a330,0x8b},{0x0040a358,0x8b},{0x0040a380,0x8b},
    {0x0040a5b2,0x8b},{0x0040a6cc,0x8b},{0x0040a731,0x8b},{0x0040a8e5,0x8b},{0x0040ac1c,0x8b},{0x0040ae8b,0x8b},
    {0x0040bb22,0x8b},{0x0040bedd,0x8b},{0x0040c04b,0x8b},{0x0040c0bc,0x8b},{0x0040c12a,0x8b},{0x0040c300,0x8b},
    {0x0040c4c2,0x8b},{0x0040c6fe,0x8b},{0x0040cd64,0x8b},{0x0040ce6e,0x8b},{0x0040ce83,0x8b},{0x0040d05d,0x8b},
    {0x0040e207,0x8b},{0x0040e23b,0x8b},{0x0041a8de,0x8b},{0x0041aa49,0x8b},{0x0041aa79,0x8b},{0x0041ab98,0x8b},
    {0x0041d77e,0x8b},{0x0042145a,0x8b},{0x0042187b,0x8b},{0x00424204,0x8b},{0x00425467,0x8b},{0x00425506,0x8b},
    {0x00425b82,0x8b},{0x00425bad,0x8b},{0x00425fa9,0x8b},{0x00425fbe,0x8b},{0x004262be,0x8b},{0x004262cc,0x8b},
    {0x00426498,0x8b},{0x004264f4,0x8b},{0x00426561,0x8b},{0x004265c7,0x8b},{0x00426662,0x8b},{0x004266ba,0x8b},
    {0x00426727,0x8b},{0x00426790,0x8b},{0x004267f4,0x8b},{0x00426806,0x8b},{0x00426a2e,0x8b},{0x00426c50,0x8b},
    {0x00426d6d,0x8b},{0x0042c48d,0x8b},{0x0042c797,0x8b},{0x0042db81,0x8b},{0x0042dc2a,0x8b},{0x0042dd34,0x8b},
    {0x0042dd97,0x8b},{0x0042dded,0x8b},{0x0042eca2,0x8b},{0x00430633,0x8b},{0x00430735,0x8b},{0x00430947,0x8b},
    {0x00431b46,0x8b},{0x00431bd2,0x8b},{0x00431f0e,0x8b},{0x00436b31,0x8b},{0x00436b42,0x8b},{0x0043772c,0x8b},
    {0x0043773d,0x8b},{0x00437797,0x8b},{0x004377a8,0x8b},{0x004377e7,0x8b},{0x004377f8,0x8b},{0x00437842,0x8b},
    {0x00437853,0x8b},{0x0043a162,0x8b},{0x0043a345,0x8b},{0x0043b3e3,0x8b},{0x0043d546,0x8b},{0x0043d7d3,0x8b},
    {0x0043d883,0x8b},{0x0043d89e,0x8b},{0x00447eeb,0x8b},{0x00448af9,0x8b},{0x00448eee,0x8b},{0x00449c34,0x8b},
    {0x0044a52a,0x8b},{0x0044b11e,0x8b},{0x00453173,0x8b},{0x004531ae,0x8b},{0x004531e4,0x8b},{0x00453220,0x8b},
    {0x00453490,0x8b},{0x004534cb,0x8b},{0x00453501,0x8b},{0x0045353d,0x8b},{0x00454b1e,0x8b},{0x00455c0e,0x8b},
    {0x00456802,0x8b},{0x00456816,0x8b},{0x004613fd,0x8b},{0x00461c2d,0x8b},{0x004624f5,0x8b},{0x004657a2,0x8b},
    {0x0046978d,0x8b},{0x0046a773,0x8b},{0x0046b5f7,0x8b},{0x00472950,0x8b},{0x00474806,0x8b},{0x00474aad,0x8b},
    {0x00475d91,0x8b},{0x00478505,0x8b},{0x00478552,0x8b},{0x004788d7,0x8b},{0x004794df,0x8b},{0x0047aea3,0x8b},
    {0x0047f50f,0x8b},{0x0047fdc0,0x8b},{0x0047fdfc,0x8b},{0x0047fe52,0x8b},{0x00481e29,0x8b},{0x0048229f,0x8b},
    {0x00482560,0x8b},{0x004829d6,0x8b},{0x00483365,0x8b},{0x0048337f,0x8b},{0x00484d39,0x8b},{0x00486b1d,0x8b},
    {0x0048aca2,0x8b},{0x00494edc,0x8b},{0x0049835e,0x8b},{0x004983a6,0x8b},{0x004986fb,0x8b},{0x00499a10,0x8b},
    {0x0049b0ec,0x8b},{0x0049b0f9,0x8b},{0x0049db82,0x8b},{0x004af46b,0x8b},{0x004b313c,0x8b},{0x004b49b3,0x8b},
    {0x004c0c13,0x8b},{0x004c4860,0x8b},{0x004c4bed,0x8b},{0x004c5538,0x89},{0x004c5960,0x8b},{0x004c5980,0x8b},
    {0x004c59c0,0x8b},{0x004c5a40,0x8b},{0x004c5a75,0x8b},{0x004c5a89,0x8b},{0x004c5aa5,0x8b},{0x004c5ab9,0x8b},
    {0x004c5ad5,0x8b},{0x004c5ae9,0x8b},{0x004c5b05,0x8b},{0x004c5b19,0x8b},{0x004c5b35,0x8b},{0x004c5b49,0x8b},
    {0x004c5b65,0x8b},{0x004c5b79,0x8b},{0x004c5b95,0x8b},{0x004c5ba9,0x8b},{0x004c5bc5,0x8b},{0x004c5bd9,0x8b},
    {0x004c5bf5,0x8b},{0x004c5c09,0x8b},{0x004c5c25,0x8b},{0x004c5c39,0x8b},{0x004c5c55,0x8b},{0x004c5c69,0x8b},
    {0x004c5c90,0x8b},{0x004c63bb,0xbe},{0x004c6642,0x89},{0x004c6671,0x89},{0x004c66a1,0x89},{0x004c66d1,0x89},
    {0x004c6701,0x89},{0x004c6731,0x89},{0x004c6761,0x89},{0x004c6795,0x89},{0x004c67c2,0x89},{0x004c681a,0xbf},
    {0x004c85d6,0x39},{0x004c89b8,0x8b},{0x004d3d52,0x8b},{0x004dacae,0x8b},{0x004e4204,0x8b},{0x004e58c4,0x8b},
    {0x004e693a,0x8b},{0x004e6970,0x8b},{0x00509cae,0x8b},{0x0050d59f,0x8b},{0x005135a2,0x8b},{0x00513634,0x8b},
    {0x00514c80,0x8b},{0x00516e1a,0x8b},{0x005172f2,0x8b},{0x005180ef,0x8b},{0x0051d233,0x8b},{0x0052034f,0x8b},
    {0x00532a58,0x8b},{0x00532b7b,0x8b},{0x00532d01,0x8b},{0x00532d44,0x8b},{0x00533162,0x8b},{0x00533247,0x8b},
    {0x00533389,0x8b},{0x0053341d,0x8b},{0x00533706,0x8b},{0x005337aa,0x8b},{0x00533bfa,0x8b},{0x00533d46,0x8b},
    {0x00533f30,0x8b},{0x00533f86,0x8b},{0x00533fbe,0x8b},{0x00533fdf,0x8b},{0x005340a5,0x8b},{0x005340d7,0x8b},
    {0x0053412a,0x8b},{0x00534254,0x8b},{0x00534294,0x8b},{0x005342bd,0x8b},{0x005343fa,0x8b},{0x0053441d,0x8b},
    {0x0053443b,0x8b},{0x00534449,0x8b},{0x005344d8,0x8b},{0x0053454a,0x8b},{0x00534568,0x8b},{0x005345f2,0x8b},
    {0x00534fb3,0x8b},{0x00535326,0x8b},{0x005354c8,0x8b},{0x00535fb0,0x8b},{0x00536c0b,0x8b},{0x00537155,0x8b},
    {0x005371f6,0x8b},{0x005372d7,0x8b},{0x005373f4,0x8b},{0x00537457,0x8b},{0x0053748d,0x8b},{0x005380b2,0x8b},
    {0x00538124,0x8b},{0x005383bb,0x8b},{0x005384ff,0x8b},{0x0053a70d,0x8b},{0x0053c449,0x8b},{0x005449c4,0x8b},
    {0x00546680,0x8b},{0x00546d28,0x8b},{0x00547fdb,0x8b},{0x005483e7,0x8b},{0x0054a998,0x8b},{0x0054ba95,0x8b},
    {0x0054c6c2,0x8b},{0x0054c817,0x8b},{0x0054d030,0x8b},{0x005537f0,0x8b},{0x00553898,0x8b},{0x00553a64,0x8b},
    {0x00553b14,0x8b},{0x0055423d,0x8b},{0x00554463,0x8b},{0x005544bd,0x8b},{0x00554663,0x8b},{0x005546b9,0x8b},
    {0x005546f5,0x8b},{0x005549f3,0x8b},{0x00554ed8,0x8b},{0x00554f39,0x8b},{0x00556768,0x8b},{0x005569f7,0x8b},
    {0x00558164,0x8b},{0x00558406,0x8b},{0x005584ca,0x8b},{0x00558688,0x8b},{0x005588e0,0x8b},{0x00558961,0x8b},
    {0x005589d6,0x8b},{0x00558a60,0x8b},{0x00558b4e,0x8b},{0x0055c011,0x8b},{0x0056508c,0x8b},{0x0056512c,0x8b},
    {0x00565266,0x8b},{0x0056537e,0x8b},{0x005662d2,0x8b},{0x0056709e,0x8b},{0x005683cd,0x8b},{0x00569999,0x8b},
    {0x00569bb3,0x8b},{0x00569bde,0x8b},{0x00569cbb,0x8b},{0x00569dc6,0x8b},{0x00571eaa,0x8b},{0x0057206f,0x8b},
    {0x00585ec7,0x8b},{0x0058d80b,0x8b},{0x0058e855,0x8b},{0x00593508,0x8b},{0x00594cce,0x8b},{0x00594fc0,0x8b},
    {0x00595390,0x8b},{0x00596756,0x8b},{0x0059843c,0x8b},{0x005986a5,0x8b},{0x00598dfc,0x8b},{0x0059e7fe,0x8b},
    {0x0059e826,0x8b},{0x0059ef37,0x8b},{0x0059efb1,0x8b},{0x0059f1b4,0x8b},{0x0059f33b,0x8b},{0x0059f6f2,0x8b},
    {0x0059f74d,0x8b},{0x0059f8aa,0x8b},{0x0059f8e0,0x8b},{0x0059f9b9,0x8b},{0x0059fa3d,0x8b},{0x0059fa53,0x8b},
    {0x0059fb5a,0x8b},{0x005a04a2,0x8b},{0x005a06d4,0x8b},{0x005a1b84,0x8b},{0x005a1bb5,0x8b},{0x005a213d,0x8b},
    {0x005a2d06,0x8b},{0x005a2dea,0x8b},{0x005a332f,0x8b},{0x005a3525,0x8b},{0x005a41c6,0x8b},{0x005a81eb,0x8b},
    {0x005b02d0,0x8b},{0x005b02ef,0x8b},{0x005b0610,0x8b},{0x005b062b,0x8b},{0x005b1ae5,0x8b},{0x005b1b69,0x8b},
    {0x005b1b7c,0x8b},{0x005b2d09,0x8b},{0x005b2f7a,0x8b},{0x005b2f9f,0x8b},{0x005b2fca,0x8b},{0x005b2fd1,0x8b},
    {0x005b3088,0x8b},{0x005b30d0,0x8b},{0x005b30d7,0x8b},{0x005b50a1,0x8b},{0x005b5270,0x8b},{0x005b529d,0x8b},
    {0x005b52aa,0x8b},{0x005b76bd,0x8b},{0x005b8c18,0xbf},{0x005b9299,0xbe},{0x005bec46,0x8b},{0x005c010b,0x8b},
    {0x005c0157,0x8b},{0x005d2a8f,0x8b},{0x005dc7f7,0x8b},{0x005df949,0x8b},{0x005dfd98,0x8b},{0x005e255f,0x8b},
    {0x005e39e3,0x8b},{0x005e3a9d,0x8b},{0x005e48b2,0x8b},{0x005e5f07,0x8b},{0x005e72f2,0x8b},{0x005e8ef9,0x8b},
    {0x005e8f0a,0x8b},{0x005f201f,0x8b},{0x005f2f20,0x8b},{0x005f336b,0x8b},{0x005f38b2,0x8b},{0x005f39c1,0x8b},
    {0x005f5720,0x8b},{0x005f5733,0x8b},{0x005fcade,0x8b},{0x005fcbb9,0x8b},{0x005fd093,0x8b},{0x005fd546,0x8b},
    {0x0060039a,0x8b},{0x00603509,0x8b},{0x00605474,0x8b},{0x00606a5b,0x8b},{0x00609a2c,0x8b},{0x0060ffb5,0x8b},
    {0x006100ef,0x8b},{0x00610154,0x8b},{0x00610738,0x8b},{0x006110c4,0x8b},{0x006110f3,0x8b},{0x00611206,0x8b},
    {0x00611454,0x8b},{0x00611474,0x8b},{0x00611494,0x8b},{0x00611764,0x8b},{0x00611794,0x8b},{0x006117b4,0x8b},
    {0x00611cc7,0x8b},{0x00611d04,0x8b},{0x00611d65,0x8b},{0x00611d92,0x8b},{0x00611dd0,0x8b},{0x00611e49,0x8b},
    {0x00611e53,0x8b},{0x00611ed4,0x8b},{0x00611eef,0x8b},{0x0061235c,0x8b},{0x006123af,0x8b},{0x0061259a,0x8b},
    {0x00612eea,0x8b},{0x0061300b,0x8b},{0x0061304c,0x8b},{0x00613065,0x8b},{0x0061307e,0x8b},{0x00613093,0x8b},
    {0x006130ab,0x8b},{0x006130da,0x8b},{0x006130f3,0x8b},{0x006131f2,0x8b},{0x0061321d,0x8b},{0x00613227,0x8b},
    {0x0061326d,0x8b},{0x006132b1,0x8b},{0x0061331d,0x8b},{0x0061333f,0x8b},{0x00613b43,0x8b},{0x00613b5a,0x8b},
    {0x00613cf7,0x8b},{0x00613e15,0x8b},{0x00613e9b,0x8b},{0x00613eed,0x8b},{0x0061417e,0x8b},{0x006142e8,0x8b},
    {0x0061451b,0x8b},{0x0061454b,0x8b},{0x006145dd,0x8b},{0x006146ad,0x8b},{0x006146d5,0x8b},{0x00614a90,0x8b},
    {0x00614c6d,0x8b},{0x00614cf4,0x8b},{0x00614d42,0x8b},{0x00614fe9,0x8b},{0x0061511b,0x8b},{0x006152c3,0x8b},
    {0x00616a9d,0x8b},{0x00616aae,0x8b},{0x00616ae8,0x8b},{0x00616af9,0x8b},{0x0062428b,0x8b},{0x0062439a,0x8b},
    {0x00627d2b,0x8b},{0x00628a3b,0x8b},{0x006291bb,0x8b},{0x0064087e,0x8b},{0x00640b3b,0x8b},{0x00642431,0x8b},
    {0x00642c41,0x8b},{0x006432bf,0x8b},{0x00643357,0x8b},{0x0064916d,0x8b},{0x0064d04d,0x8b},{0x0064e74f,0x8b},
    {0x0064fc21,0x8b},{0x006504de,0x8b},{0x006509bc,0x8b},{0x00650e09,0x8b},{0x00656573,0x8b},{0x0065d35a,0x8b},
    {0x006641b5,0x8b},{0x0066477a,0x8b},{0x00664bf2,0x8b},{0x0066f4d1,0x8b},{0x0066f92a,0x8b},{0x0066f99e,0x8b},
    {0x0066fa11,0x8b},{0x0066fa85,0x8b},{0x0066faf9,0x8b},{0x0066fb6d,0x8b},{0x0066fbe1,0x8b},{0x0066fc55,0x8b},
    {0x0067c82f,0x8b},{0x0067c83d,0x8b},{0x0067d5c6,0x8b},{0x0067dfce,0x8b},{0x0068d9f5,0x8b},{0x0069368a,0x8b},
    {0x0069598c,0x8b},{0x006969c5,0x8b},{0x00696af3,0x8b},{0x0069912e,0x8b},{0x00699268,0x8b},{0x006995ec,0x8b},
    {0x00699701,0x8b},{0x0069a77e,0x8b},{0x0069b909,0x8b},{0x006a07ab,0x8b},{0x006a1577,0x8b},{0x006a26c2,0x8b},
    {0x006a2792,0x8b},{0x006a2871,0x8b},{0x006a2936,0x8b},{0x006a2f88,0x8b},{0x006a307f,0x8b},{0x006a3bdb,0x8b},
    {0x006a452a,0x8b},{0x006a4805,0x8b},{0x006a609e,0x8b},{0x006a65dd,0x8b},{0x006a7178,0x8b},{0x006a8b6e,0x8b},
    {0x006aa2e2,0x8b},{0x006abd16,0x8b},{0x006ae28b,0x8b},{0x006ae2ce,0x8b},{0x006ae302,0x8b},{0x006ae332,0x8b},
    {0x006ae5d5,0x8b},{0x006ae60e,0x8b},{0x006af1ea,0x8b},{0x006af320,0x8b},{0x006af37e,0x8b},{0x006b0581,0x8b},
    {0x006b0b22,0x8b},{0x006b1386,0x8b},{0x006b14b6,0x8b},{0x006b16a7,0x8b},{0x006b16ef,0x8b},{0x006b3f7d,0x8b},
    {0x006b4100,0x8b},{0x006b67ea,0x8b},{0x006b6965,0x8b},{0x006b804c,0x8b},{0x006b89bc,0x8b},{0x006b927d,0x8b},
    {0x006bbc76,0x8b},{0x006bc93c,0x8b},{0x006bd683,0x8b},{0x006be009,0x8b},{0x006bf48a,0x8b},{0x006bf857,0x8b},
    {0x006c0b3f,0x8b},{0x006c1483,0x8b},{0x006c440e,0x8b},{0x006c5446,0x8b},{0x006c7e83,0x8b},{0x006c7fbc,0x8b},
    {0x006c8e60,0x8b},{0x006c94d5,0x8b},{0x006ce3b9,0x8b},{0x006ceaf5,0x8b},{0x006ceea4,0x8b},{0x006cf1aa,0x8b},
    {0x006cfae5,0x8b},{0x006cfd92,0x8b},{0x006d0b79,0x8b},{0x006d0c4c,0x8b},{0x006d0e49,0x8b},{0x006d0e76,0x8b},
    {0x006d2315,0x8b},{0x006d2b02,0x8b},{0x006d31ec,0x8b},{0x006d32df,0x8b},{0x006d3875,0x8b},{0x006d3971,0x8b},
    {0x006d3a21,0x8b},{0x006d429a,0x8b},{0x006d46e9,0x8b},{0x006d5431,0x8b},{0x006d63f4,0x8b},{0x006d649d,0x8b},
    {0x006d64d4,0x8b},{0x006d64fe,0x8b},{0x006d6a4e,0x8b},{0x006d7ed5,0x8b},{0x006db171,0x8b},{0x006de2ea,0x8b},
    {0x006df455,0x8b},{0x006df936,0x8b},{0x006df9c7,0x8b},{0x006dfa2f,0x8b},{0x006dfa3a,0x8b},{0x006dfb48,0x8b},
    {0x006dfb8a,0x8b},{0x006dfbfa,0x8b},{0x006e0a62,0x8b},{0x006e0e2d,0x8b},{0x006e144b,0x8b},{0x006e1607,0x8b},
    {0x006e17b9,0x8b},{0x006e1f35,0x8b},{0x006e22c6,0x8b},{0x006e3296,0x8b},{0x006e3312,0x8b},{0x006e3396,0x8b},
    {0x006f0d46,0x8b},{0x006f2987,0x8b},{0x006f2ff1,0x8b},{0x006f32ea,0x8b},{0x006f3398,0x8b},{0x006f33b2,0x8b},
    {0x006f3596,0x8b},{0x006f3753,0x8b},{0x006f376b,0x8b},{0x006f3990,0x8b},{0x006f6073,0x8b},{0x006f6fbb,0x8b},
    {0x006f7735,0x8b},{0x006f7761,0x8b},{0x006f9132,0x8b},{0x006f9380,0x8b},{0x006f9953,0x8b},{0x006fc805,0x8b},
    {0x007086e9,0x8b},{0x0070a068,0x8b},{0x0070a586,0x8b},{0x0070a73c,0x8b},{0x0070a8c5,0x8b},{0x00711bda,0x8b},
    {0x00711cf6,0x8b},{0x007179e3,0x8b},{0x0071aff4,0x8b},{0x0071bc64,0x8b},{0x0071c0e9,0x8b},{0x0071cff8,0x8b},
    {0x0071d032,0x8b},{0x0071fb44,0x8b},{0x0071fb88,0x8b},{0x0071fc9a,0x8b},{0x0071fd47,0x8b},{0x0071fe09,0x8b},
    {0x0071feda,0x8b},{0x0071ffc2,0x8b},{0x0073227a,0x8b},{0x00732b55,0x8b},{0x00736f18,0x8b},{0x00738409,0x8b},
    {0x0073a398,0x8b},{0x0073a3b9,0x8b},{0x0073b525,0x8b},{0x0073b537,0x8b},{0x0073d5d0,0x8b},{0x0073ec5d,0x8b},
    {0x0074103b,0x8b},{0x01562a25,0x8b},{0x01562b84,0x8b},{0x01564572,0x8b},{0x01564e82,0x8b},{0x0156524c,0x8b},
    {0x015662d5,0x8b},{0x015664fd,0x8b},{0x0156657a,0x8b},{0x015665b9,0x8b},{0x01566f05,0x8b},{0x015691be,0x8b},
    {0x015691c5,0x8b},{0x0156c22a,0x8b},{0x0156c258,0x8b},{0x0156c5c5,0x8b},{0x0156ca6d,0x8b},{0x0156cab2,0x8b},
    {0x0156d0c9,0x8b},{0x0156d0f6,0x8b},{0x0156d126,0x8b},{0x015702d8,0x8b},{0x01570300,0x8b},{0x01570328,0x8b},
    {0x01570350,0x8b},{0x01570378,0x8b},{0x015703a0,0x8b},{0x01570494,0x8b},{0x015704ed,0x8b},
};
// samp.dll RVA: 0x00117c30
const char g_customServerCachePattern[]="%s\\%s.%d";
// samp.dll RVA: 0x00117c3c
const char g_customLocalCachePattern[]="%s\\local";
// samp.dll RVA: 0x0011edc8
const char g_customCollisionWrongType[]="[Error] not a COL3 collision";
// samp.dll RVA: 0x0011ed9c
const char g_customCollisionLoadFailed[]="[Error] model loader failed to load col.";
// samp.dll RVA: 0x0011ed68
const char g_customCollisionAllocationFailed[]="[Error] model loader failed to allocated a new col.";
// samp.dll RVA: 0x000a7560
BOOL GamePatchModelInfoReferences() {
    for(int i=0;i<707;++i) {
        const GameModelReferencePatch *site=g_gameVersion==1?g_gameModelReferencePatchesUS+i:g_gameModelReferencePatchesEU+i;
        BYTE expected=site->opcode;BYTE *address=(BYTE*)site->address;
        if(*address!=expected)continue;
        BYTE *operand;
        if(expected==0x8b||expected==0x89||expected==0x39)operand=address+3;
        else if(expected==0xbe||expected==0xbf)operand=address+1;
        else continue;
        GameUnprotect(operand,4);*(void***)operand=g_gameModelInfoTable;
    }
    g_gameUseCustomModelInfo=1;return TRUE;
}
// samp.dll RVA: 0x000a75f0
BOOL GameInitializeCustomModelInfo() {
    // Native clears65535 DWORDs starting at1A0448, ending before enable1E0444.
    memset(g_gameCustomModelPrefix,0,sizeof(g_gameCustomModelPrefix));
    memset(g_gameCustomModelInfo,0,sizeof(g_gameCustomModelInfo));
    // GTA SA virtual address: original model-info array0x00a9b0c8, exactly20000 entries.
    memcpy(g_gameModelInfoTable,(const void*)0xa9b0c8,20000*4);
    GamePatchModelInfoReferences();GameInstallModelInfoHook();return TRUE;
}
// samp.dll RVA: 0x000b6390
void * __cdecl GameCreateCollisionModel() {
    // GTA SA virtual addresses: allocator0x0040fc30 and thiscall constructor0x0040fb60.
    void *collision=((void*(__cdecl*)(DWORD))0x40fc30)(0x30);
    __asm {mov ecx,collision
        mov eax,0x40fb60
        call eax}
    return collision;
}
// samp.dll RVA: 0x000b63c0
BOOL __cdecl GameLoadCollisionData(const void *data,DWORD length,void *collision) {
    DWORD encoded=*((const DWORD*)data+1);if(encoded>length)return FALSE;
    // Native subtraction may underflow; GTA SA cdecl loader0x00537ce0 result ignored.
    ((void(__cdecl*)(const void*,DWORD,void*,DWORD))0x537ce0)((const BYTE*)data+0x20,encoded-0x18,collision,0);
    return TRUE;
}
// samp.dll RVA: 0x000a78b0
DWORD __cdecl GameReadCustomCollision(void *stream,DWORD length) {
    if(length>0x40000)return 0;
    BYTE *data=(BYTE*)calloc(1,length);GameReadStream(stream,data,length);
    if(*(DWORD*)data!=0x334c4f43) {
        if(g_pChat)g_pChat->AddDebugMessage(g_customCollisionWrongType);free(data);return 0;
    }
    void *collision=GameCreateCollisionModel();
    if(collision) {
        if(GameLoadCollisionData(data,length,collision)) {GameSetModelCollision(g_gameLoadingCustomModel,collision);free(data);return 1;}
        if(g_pChat)g_pChat->AddDebugMessage(g_customCollisionLoadFailed);
    }else if(g_pChat)g_pChat->AddDebugMessage(g_customCollisionAllocationFailed);
    free(data);return 0;
}
// samp.dll RVA: 0x000a7980
void GameRegisterCustomCollisionPlugin() {
    GameRwRegisterClumpPlugin(0x253f2ff);GameRwRegisterClumpPluginStream(0x253f2ff,(DWORD)GameReadCustomCollision);
}
// samp.dll RVA: 0x0000b9c0
DWORD CCustomModels::SanitizeCacheHost(char *output,DWORD capacity,const char *host) {
    DWORD length=(DWORD)strlen(host),written=0;
    for(DWORD i=0;i<length&&capacity;++i) {
        char c=host[i];if(c=='.'||(c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')){output[written++]=c;--capacity;}
    }
    output[written]=0;return written;
}
// samp.dll RVA: 0x0000ba40
void CCustomModels::Initialize() {
    if(nativeState[8])return;
    GameInitializeCustomModelInfo();GameRegisterCustomCollisionPlugin();
    _snprintf((char*)nativeState+0x10e,260,g_customLocalCachePattern,ClientGetCacheDirectory());
    char host[26]={0};SanitizeCacheHost(host,25,(const char*)((CNetGame*)g_pNetGame)->nativeState+0x30);
    _snprintf((char*)nativeState+9,260,g_customServerCachePattern,ClientGetCacheDirectory(),host,*(int*)(((CNetGame*)g_pNetGame)->nativeState+0x235));
    if(!GameFileExists((char*)nativeState+9))CreateDirectoryA((char*)nativeState+9,0);
    nativeState[8]=1;*(DWORD*)(nativeState+0x213)=2;
}
#include "BitStream.h"
#include "RakClientInterface.h"
#include "net/netgame.h"
#include "gui/render_dependencies.h"
#include "LzmaDec.h"
#include "Alloc.h"
#include "config.h"
#include <process.h>
#include <curl/curl.h>
typedef char NativeLzmaSizeExtent[(sizeof(SizeT)==4)?1:-1];
// samp.dll RVA: 0x00117e1c
const char g_customDownloadSizeError[]="[error] file 0x%X bad length got:%u expected:%u. Deleting..";
// samp.dll RVA: 0x00117de8
const char g_customDownloadCrcError[]="[error] file 0x%X bad checksum (0x%X). Deleting..";
// samp.dll RVA: 0x002ac390
char g_customProxy[261];
// samp.dll RVA: 0x0011f9c0
const char g_customRegistryKey[]="Software\\SAMP";
// samp.dll RVA: 0x0011f9dc
const char g_customProxyValue[]="artwork_proxy";
// samp.dll RVA: 0x00117e58
const char g_customUserAgent[]="SAMP/0.3";
// samp.dll RVA: 0x00117e9c
const char g_customLogUrlsOption[]="logurls";
// samp.dll RVA: 0x00117e8c
const char g_customUrlLogFormat[]="[url 0x%X]: %s";
// samp.dll RVA: 0x00117e64
const char g_customNoFreeWorkerError[]="[error] AddURLRequest: No free slots!";
// samp.dll RVA: 0x00117ed8
const char g_customUrlInfoError[]="DownloadRedirect: Can't fill download info!";
// samp.dll RVA: 0x00117ea4
const char g_customUrlOpenError[]="DownloadRedirect: Can't open temp file for writing!";
// samp.dll RVA: 0x000c52d0
const char *CustomModelGetProxy() {
    memset(g_customProxy,0,261);HKEY key;DWORD length=260,type;
    if(RegOpenKeyExA(HKEY_CURRENT_USER,g_customRegistryKey,0,0x20019,&key))return 0;
    // Native intentionally does not close the opened key.
    return RegQueryValueExA(key,g_customProxyValue,0,&type,(BYTE*)g_customProxy,&length)==0?g_customProxy:0;
}
// samp.dll RVA: 0x0000de60
BOOL __cdecl CustomModelFetchUrl(CustomModelWorker *worker,const char *proxy) {
    CURL *curl=curl_easy_init();if(!curl)return FALSE;
    worker->failed=0;worker->result=0;
    curl_easy_setopt(curl,(CURLoption)10002,worker->url);
    curl_easy_setopt(curl,(CURLoption)10102,g_netEmptyShoppingSubsection);
    curl_easy_setopt(curl,(CURLoption)10018,g_customUserAgent);
    curl_easy_setopt(curl,(CURLoption)43,1L);
    curl_easy_setopt(curl,(CURLoption)20011,CustomModelDownloadWrite);
    curl_easy_setopt(curl,(CURLoption)10001,&worker->download);
    curl_easy_setopt(curl,(CURLoption)45,1L);
    if(proxy&&strlen(proxy))curl_easy_setopt(curl,(CURLoption)10004,proxy);
    CURLcode result=curl_easy_perform(curl);
    if(result){worker->failed=1;worker->result=result;}
    curl_easy_cleanup(curl);
    if(worker->failed) {
        const char *detail=curl_easy_strerror(result);g_customDownloadManager->AbortDownload(&worker->download,(int)worker->result,detail);
    }else g_customDownloadManager->FinishDownload(&worker->download);
    return TRUE;
}
// samp.dll RVA: 0x0000df90
void __cdecl CustomModelDownloadThread(void *argument) {
    const char *proxy=CustomModelGetProxy();curl_global_init(3);
    CustomModelWorker *worker=(CustomModelWorker*)argument;
    while(!worker->stop) {
        if(worker->active){CustomModelFetchUrl(worker,proxy);worker->active=0;}
        Sleep(1);
    }
    curl_global_cleanup();worker->enabled=0;
}
// samp.dll RVA: 0x0000dff0
bool CCustomModels::StartWorkers() {
    for(int i=0;i<6;++i) {
        CustomModelWorker *worker=(CustomModelWorker*)(nativeState+0x1345+i*0x230);if(worker->enabled)continue;
        worker->thread=(DWORD)_beginthread(CustomModelDownloadThread,0,worker);
        if(worker->thread==0xffffffff){worker->enabled=0;worker->failed=1;worker->result=0xffffffff;return false;}
        worker->id=i+1;worker->enabled=1;worker->stop=0;worker->active=0;worker->failed=0;worker->result=0xffffffff;
        memset(worker->url,0,257);memset(&worker->download,0,0x11e);
    }
    nativeState[0x1344]=1;return true;
}
// samp.dll RVA: 0x0000e0a0
void CCustomModels::QueueUrl(CustomModelDownload *download,const char *url) {
    if(g_pChat&&g_pConfig&&g_pConfig->GetIntValue(g_customLogUrlsOption))g_pChat->AddInfoMessage(g_customUrlLogFormat,download->hash,url);
    g_customDownloadManager=this;if(!nativeState[0x1344])StartWorkers();
    for(int i=0;i<6;++i) {
        CustomModelWorker *worker=(CustomModelWorker*)(nativeState+0x1345+i*0x230);
        if(worker->enabled&&!worker->active) {
            worker->download=*download;strncpy(worker->url,url,256);worker->active=1;worker->paused=0;return;
        }
    }
    if(g_pChat)g_pChat->AddDebugMessage(g_customNoFreeWorkerError);
}
// samp.dll RVA: 0x0000e180
void CCustomModels::BeginUrlDownload(BYTE type,DWORD hash,const char *url) {
    CustomModelDownload download;
    if(!FillDownloadInfo(type,hash,&download)){if(g_pChat)g_pChat->AddDebugMessage(g_customUrlInfoError);return;}
    *(DWORD*)(nativeState+0x1340)=0;download.file=fopen(download.temporaryPath,g_customDownloadWriteMode);
    if(!download.file){if(g_pChat)g_pChat->AddDebugMessage(g_customUrlOpenError);return;}
    QueueUrl(&download,url);
    if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download.type,download.hash,0,0,download.total,-1,0);
    *(DWORD*)(nativeState+0x213)=1;if(CountReadyWorkers()>0)*(DWORD*)(nativeState+0x213)=2;
}
// samp.dll RVA: 0x000c50d0
int __stdcall CustomModelLzmaDecode(BYTE *destination,DWORD *destinationLength,const BYTE *source,DWORD *sourceLength,const BYTE *properties,DWORD propertySize) {
    ELzmaStatus status;
    return LzmaDecode(destination,(SizeT*)destinationLength,source,(SizeT*)sourceLength,properties,propertySize,LZMA_FINISH_ANY,&status,&g_Alloc);
}
// samp.dll RVA: 0x0000d070
void CCustomModels::ProcessCompressedChunk(BYTE stage,DWORD unpackedLength,const void *data,DWORD length) {
    BYTE properties[5]={0x5d,0,0,1,0},output[4096];DWORD outputLength=4096;
    if(unpackedLength>4096)return;
    DWORD &compressed=*(DWORD*)(nativeState+0x1340);if(compressed+length>4096)return;
    memcpy(nativeState+0x340+compressed,data,length);compressed+=length;
    if(stage==2) {
        int result=CustomModelLzmaDecode(output,&outputLength,nativeState+0x340,&compressed,properties,5);
        if(result!=0&&result!=6){AbortDownload(0,5001,0);compressed=0;return;}
        WriteDownload(0,output,unpackedLength);
        DWORD unpacked=*(DWORD*)(nativeState+0x2065)+unpackedLength,packed=*(DWORD*)(nativeState+0x2069)+compressed;
        compressed=0;*(DWORD*)(nativeState+0x2065)=unpacked;*(DWORD*)(nativeState+0x2069)=packed;
    }else if(stage!=1)AbortDownload(0,5002,0);
}
// samp.dll RVA: 0x0000da80
bool CCustomModels::VerifyDff(const char *filename,DWORD hash) {
    CDffFile file;
    if(!file.LoadFile(filename)){g_pChat->AddInfoMessage(g_customDffVerifyError,hash,file.error);return false;}
    if(!file.ReadFrames()||!file.ReadGeometries()) {
        g_pChat->AddInfoMessage(g_customDffParseError,hash,file.error,file.detail);return false;
    }
    return true;
}
// samp.dll RVA: 0x0000bfd0
bool CCustomModels::VerifyTxd(const char *filename,DWORD hash) {
    CTxdFile file;
    if(!file.LoadFile(filename)){g_pChat->AddInfoMessage(g_customTxdVerifyError,hash,file.error);return false;}
    if(file.textureCount>1024){g_pChat->AddInfoMessage(g_customTxdCountError,hash,(DWORD)file.textureCount);return false;}
    return true;
}
// samp.dll RVA: 0x0000dbd0
void CCustomModels::FinishDownload(CustomModelDownload *download) {
    if(!download)download=(CustomModelDownload*)(nativeState+0x222);
    BYTE *record=download->index<*(DWORD*)(nativeState+4)?(*(BYTE***)nativeState)[download->index]:0;
    if(download->file){fclose((FILE*)download->file);download->file=0;}
    if(download->downloaded!=download->total) {
        if(g_pChat)g_pChat->AddDebugMessage(g_customDownloadSizeError,download->hash,download->downloaded,download->total);
        DeleteFileA(download->temporaryPath);record[6]=1;
        if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,3,download->downloaded,0,100,0);
        *(DWORD*)(nativeState+0x213)=2;return;
    }
    if(download->crc!=download->hash) {
        if(g_pChat)g_pChat->AddDebugMessage(g_customDownloadCrcError,download->hash,download->crc);
        DeleteFileA(download->temporaryPath);record[6]=1;
        if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,3,download->downloaded,0,101,0);
        *(DWORD*)(nativeState+0x213)=2;return;
    }
    char destination[264];
    if(download->type==1) {
        if(!VerifyDff(download->temporaryPath,download->hash)) {
            DeleteFileA(download->temporaryPath);record[6]=1;
            if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,3,download->downloaded,0,102,0);
            *(DWORD*)(nativeState+0x213)=2;return;
        }
    }else if(download->type==2) {
        if(!VerifyTxd(download->temporaryPath,download->hash)) {
            DeleteFileA(download->temporaryPath);record[6]=1;
            if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,3,download->downloaded,0,103,0);
            *(DWORD*)(nativeState+0x213)=2;return;
        }
    }else {
        DeleteFileA(download->temporaryPath);record[6]=1;
        if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,3,download->downloaded,0,104,0);
        *(DWORD*)(nativeState+0x213)=2;return;
    }
    if(download->type==1)_snprintf(destination,260,g_customDffCachePattern,(char*)this+9,download->hash);
    else if(download->type==2)_snprintf(destination,260,g_customTxdCachePattern,(char*)this+9,download->hash);
    MoveFileA(download->temporaryPath,destination);
    if(download->type==1)record[0]=1;else if(download->type==2)record[1]=1;
    if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,4,download->downloaded,0,-1,0);
    *(DWORD*)(nativeState+0x213)=2;
}
// samp.dll RVA: 0x00140378
const DWORD g_customCrcTable[256]={
    0x00000000,0x77073096,0xee0e612c,0x990951ba,0x076dc419,0x706af48f,0xe963a535,0x9e6495a3,
    0x0edb8832,0x79dcb8a4,0xe0d5e91e,0x97d2d988,0x09b64c2b,0x7eb17cbd,0xe7b82d07,0x90bf1d91,
    0x1db71064,0x6ab020f2,0xf3b97148,0x84be41de,0x1adad47d,0x6ddde4eb,0xf4d4b551,0x83d385c7,
    0x136c9856,0x646ba8c0,0xfd62f97a,0x8a65c9ec,0x14015c4f,0x63066cd9,0xfa0f3d63,0x8d080df5,
    0x3b6e20c8,0x4c69105e,0xd56041e4,0xa2677172,0x3c03e4d1,0x4b04d447,0xd20d85fd,0xa50ab56b,
    0x35b5a8fa,0x42b2986c,0xdbbbc9d6,0xacbcf940,0x32d86ce3,0x45df5c75,0xdcd60dcf,0xabd13d59,
    0x26d930ac,0x51de003a,0xc8d75180,0xbfd06116,0x21b4f4b5,0x56b3c423,0xcfba9599,0xb8bda50f,
    0x2802b89e,0x5f058808,0xc60cd9b2,0xb10be924,0x2f6f7c87,0x58684c11,0xc1611dab,0xb6662d3d,
    0x76dc4190,0x01db7106,0x98d220bc,0xefd5102a,0x71b18589,0x06b6b51f,0x9fbfe4a5,0xe8b8d433,
    0x7807c9a2,0x0f00f934,0x9609a88e,0xe10e9818,0x7f6a0dbb,0x086d3d2d,0x91646c97,0xe6635c01,
    0x6b6b51f4,0x1c6c6162,0x856530d8,0xf262004e,0x6c0695ed,0x1b01a57b,0x8208f4c1,0xf50fc457,
    0x65b0d9c6,0x12b7e950,0x8bbeb8ea,0xfcb9887c,0x62dd1ddf,0x15da2d49,0x8cd37cf3,0xfbd44c65,
    0x4db26158,0x3ab551ce,0xa3bc0074,0xd4bb30e2,0x4adfa541,0x3dd895d7,0xa4d1c46d,0xd3d6f4fb,
    0x4369e96a,0x346ed9fc,0xad678846,0xda60b8d0,0x44042d73,0x33031de5,0xaa0a4c5f,0xdd0d7cc9,
    0x5005713c,0x270241aa,0xbe0b1010,0xc90c2086,0x5768b525,0x206f85b3,0xb966d409,0xce61e49f,
    0x5edef90e,0x29d9c998,0xb0d09822,0xc7d7a8b4,0x59b33d17,0x2eb40d81,0xb7bd5c3b,0xc0ba6cad,
    0xedb88320,0x9abfb3b6,0x03b6e20c,0x74b1d29a,0xead54739,0x9dd277af,0x04db2615,0x73dc1683,
    0xe3630b12,0x94643b84,0x0d6d6a3e,0x7a6a5aa8,0xe40ecf0b,0x9309ff9d,0x0a00ae27,0x7d079eb1,
    0xf00f9344,0x8708a3d2,0x1e01f268,0x6906c2fe,0xf762575d,0x806567cb,0x196c3671,0x6e6b06e7,
    0xfed41b76,0x89d32be0,0x10da7a5a,0x67dd4acc,0xf9b9df6f,0x8ebeeff9,0x17b7be43,0x60b08ed5,
    0xd6d6a3e8,0xa1d1937e,0x38d8c2c4,0x4fdff252,0xd1bb67f1,0xa6bc5767,0x3fb506dd,0x48b2364b,
    0xd80d2bda,0xaf0a1b4c,0x36034af6,0x41047a60,0xdf60efc3,0xa867df55,0x316e8eef,0x4669be79,
    0xcb61b38c,0xbc66831a,0x256fd2a0,0x5268e236,0xcc0c7795,0xbb0b4703,0x220216b9,0x5505262f,
    0xc5ba3bbe,0xb2bd0b28,0x2bb45a92,0x5cb36a04,0xc2d7ffa7,0xb5d0cf31,0x2cd99e8b,0x5bdeae1d,
    0x9b64c2b0,0xec63f226,0x756aa39c,0x026d930a,0x9c0906a9,0xeb0e363f,0x72076785,0x05005713,
    0x95bf4a82,0xe2b87a14,0x7bb12bae,0x0cb61b38,0x92d28e9b,0xe5d5be0d,0x7cdcefb7,0x0bdbdf21,
    0x86d3d2d4,0xf1d4e242,0x68ddb3f8,0x1fda836e,0x81be16cd,0xf6b9265b,0x6fb077e1,0x18b74777,
    0x88085ae6,0xff0f6a70,0x66063bca,0x11010b5c,0x8f659eff,0xf862ae69,0x616bffd3,0x166ccf45,
    0xa00ae278,0xd70dd2ee,0x4e048354,0x3903b3c2,0xa7672661,0xd06016f7,0x4969474d,0x3e6e77db,
    0xaed16a4a,0xd9d65adc,0x40df0b66,0x37d83bf0,0xa9bcae53,0xdebb9ec5,0x47b2cf7f,0x30b5ffe9,
    0xbdbdf21c,0xcabac28a,0x53b39330,0x24b4a3a6,0xbad03605,0xcdd70693,0x54de5729,0x23d967bf,
    0xb3667a2e,0xc4614ab8,0x5d681b02,0x2a6f2b94,0xb40bbe37,0xc30c8ea1,0x5a05df1b,0x2d02ef8d,
};
// samp.dll RVA: 0x00156b44
CCustomModels *g_customDownloadManager;
// samp.dll RVA: 0x00117cfc
const char g_customTemporaryPathPattern[]="%s/%05d_%X.temp";
// samp.dll RVA: 0x00117d30
const char g_customDownloadWriteMode[]="wb";
// samp.dll RVA: 0x00118e1c
const char g_customDownloadReadMode[]="rb";
// samp.dll RVA: 0x00117d34
const char g_customFillDownloadError[]="Can't fill download info for CRC: 0x%X";
// samp.dll RVA: 0x000628a0
DWORD __cdecl CustomModelCrc(DWORD seed,const void *data,DWORD length) {
    DWORD crc=~seed;const BYTE *bytes=(const BYTE*)data;
    while(length--){crc=(crc>>8)^g_customCrcTable[(crc^*bytes++)&0xff];}
    return ~crc;
}
// samp.dll RVA: 0x000628e0
DWORD __cdecl CustomModelFileCrc(const char *filename) {
    FILE *file=fopen(filename,g_customDownloadReadMode);DWORD crc=0;if(!file)return crc;
    BYTE data[1024];while(!(file->_flag&_IOEOF)) {
        DWORD length=(DWORD)fread(data,1,1024,file);crc=CustomModelCrc(crc,data,length);
    }
    fclose(file);return crc;
}
// samp.dll RVA: 0x0000c7e0
int CCustomModels::FindResourceIndex(BYTE type,DWORD hash) {
    DWORD count=*(DWORD*)(nativeState+4);
    for(DWORD i=0;i<count;++i) {
        BYTE *record=(*(BYTE***)nativeState)[i];if(!record)continue;
        if((type==1&&*(DWORD*)(record+0x46)==hash)||(type==2&&*(DWORD*)(record+0x4a)==hash))return (int)i;
    }
    return -1;
}
// samp.dll RVA: 0x0000c830
bool CCustomModels::FillDownloadInfo(BYTE type,DWORD hash,CustomModelDownload *download) {
    int index=FindResourceIndex(type,hash);if(index<0)return false;
    BYTE *record=(DWORD)index<*(DWORD*)(nativeState+4)?(*(BYTE***)nativeState)[index]:0;
    memset(download,0,0x11e);download->index=index;download->type=type;download->downloaded=0;download->crc=0;
    if(type==1){download->hash=*(DWORD*)(record+0x46);download->total=*(DWORD*)(record+0x4e);}
    else if(type==2){download->hash=*(DWORD*)(record+0x4a);download->total=*(DWORD*)(record+0x52);}
    _snprintf(download->temporaryPath,260,g_customTemporaryPathPattern,(char*)this+9,index,download->hash);return true;
}
// samp.dll RVA: 0x0000bc30
void CCustomModels::WriteDownload(CustomModelDownload *download,const void *data,DWORD length) {
    if(!download)download=(CustomModelDownload*)(nativeState+0x222);if(!download->file)return;
    download->crc=CustomModelCrc(download->crc,data,length);fwrite(data,1,length,(FILE*)download->file);
    download->downloaded+=length;
    if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,1,download->downloaded,0,-1,0);
}
// samp.dll RVA: 0x0000c760
void CCustomModels::AbortDownload(CustomModelDownload *download,int error,const char *detail) {
    if(!download)download=(CustomModelDownload*)(nativeState+0x222);
    if(download->file){fclose((FILE*)download->file);download->file=0;DeleteFileA(download->temporaryPath);}
    DWORD index=download->index;
    if(index<*(DWORD*)(nativeState+4)) {
        BYTE *record=(*(BYTE***)nativeState)[index];if(record)record[6]=1;
    }
    if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,3,download->downloaded,0,error,detail);
    *(DWORD*)(nativeState+0x213)=2;
}
// samp.dll RVA: 0x0000d1c0
BOOL CCustomModels::BeginDirectDownload(BYTE type,DWORD hash) {
    CustomModelDownload *download=(CustomModelDownload*)(nativeState+0x222);
    if(!FillDownloadInfo(type,hash,download)) {
        if(g_pChat)g_pChat->AddDebugMessage(g_customFillDownloadError,hash);return FALSE;
    }
    *(DWORD*)(nativeState+0x1340)=0;download->file=fopen(download->temporaryPath,g_customDownloadWriteMode);if(!download->file)return FALSE;
    if(g_renderDownloadWindow)g_renderDownloadWindow->UpdateRecord(download->type,download->hash,0,0,download->total,-1,0);
    *(DWORD*)(nativeState+0x213)=1;return TRUE;
}
// samp.dll RVA: 0x0000bca0
int CCustomModels::FindWorkerByFile(void *file) {
    for(int i=0;i<6;++i)if(*(void**)(nativeState+0x1457+i*0x230)==file)return i;return -1;
}
// samp.dll RVA: 0x0000bcd0
DWORD __cdecl CustomModelDownloadWrite(void *data,DWORD size,DWORD count,void *download) {
    CCustomModels *manager=g_customDownloadManager;
    if(manager&&download)manager->WriteDownload((CustomModelDownload*)download,data,size*count);
    return size*count;
}
// samp.dll RVA: 0x0000c8e0
int CCustomModels::CountMissingResources() {
    if(!nativeState[0x21f])return -1;DWORD count=*(DWORD*)(nativeState+4),missing=0;
    for(DWORD i=0;i<count;++i) {
        BYTE *record=(*(BYTE***)nativeState)[i];if(!record||record[6])continue;
        if(record[7]==1||(record[7]==2&&(*(DWORD*)(record+8)==*(DWORD*)(nativeState+0x217)||*(DWORD*)(record+8)==0xffffffff)))
            if(!record[0]||!record[1])++missing;
    }
    return (int)missing;
}
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
// Cache mutation and manifest completion stay with the same manager class.
// samp.dll RVA: 0x0000c100
BOOL CCustomModels::DeleteCachedDff(DWORD hash) {
    char path[264];_snprintf(path,260,g_customDeleteDffPattern,(char*)this+9,hash);return DeleteFileA(path);
}
// samp.dll RVA: 0x0000c140
BOOL CCustomModels::DeleteCachedTxd(DWORD hash) {
    char path[264];_snprintf(path,260,g_customDeleteTxdPattern,(char*)this+9,hash);return DeleteFileA(path);
}
// samp.dll RVA: 0x0000c180
BOOL CCustomModels::DeleteTemporaryFile(const char *filename) {
    char path[264];_snprintf(path,260,g_customDeleteTempPattern,(char*)this+9,filename);return DeleteFileA(path);
}
// samp.dll RVA: 0x0000ca40
bool CCustomModels::HasResourceHash(DWORD hash) {
    if(!nativeState[0x21f])return true;
    int count=*(int*)(nativeState+4);
    for(int i=0;i<count;++i) {
        BYTE *entry=(*(BYTE***)nativeState)[i];
        if(entry&&(*(DWORD*)(entry+0x46)==hash||*(DWORD*)(entry+0x4a)==hash))return true;
    }
    return false;
}
// samp.dll RVA: 0x0000ca90
void CCustomModels::PurgeUnusedCache() {
    char temp[264],dff[264],txd[264];const char *directory=(char*)this+9;
    _snprintf(temp,260,g_customFindTempPattern,directory);
    _snprintf(dff,260,g_customFindDffPattern,directory);
    _snprintf(txd,260,g_customFindTxdPattern,directory);
    WIN32_FIND_DATAA file;HANDLE search=FindFirstFileA(temp,&file);
    if(search!=INVALID_HANDLE_VALUE) {
        do {if(!(file.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))DeleteTemporaryFile(file.cFileName);}while(FindNextFileA(search,&file));
        FindClose(search);
    }
    search=FindFirstFileA(dff,&file);
    if(search!=INVALID_HANDLE_VALUE) {
        do {
            if(!(file.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
                DWORD hash=ParseCacheHash(file.cFileName);if(hash&&!HasResourceHash(hash))DeleteCachedDff(hash);
            }
        }while(FindNextFileA(search,&file));
        FindClose(search);
    }
    search=FindFirstFileA(txd,&file);
    if(search!=INVALID_HANDLE_VALUE) {
        do {
            if(!(file.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
                DWORD hash=ParseCacheHash(file.cFileName);if(hash&&!HasResourceHash(hash))DeleteCachedTxd(hash);
            }
        }while(FindNextFileA(search,&file));
        FindClose(search);
    }
}
// samp.dll RVA: 0x0000cbf0
void CCustomModels::RefreshCachedResources() {
    if(!nativeState[0x21f])return;DWORD count=*(DWORD*)(nativeState+4),index=0;
    if(count)do {
        BYTE *entry=index<count?(*(BYTE***)nativeState)[index]:0;
        if(entry[7]==1||(entry[7]==2&&(*(DWORD*)(entry+8)==*(DWORD*)(nativeState+0x217)||*(DWORD*)(entry+8)==0xffffffff))) {
            if(!entry[0]&&IsDffCached(*(DWORD*)(entry+0x46)))entry[0]=1;
            if(!entry[1]&&IsTxdCached(*(DWORD*)(entry+0x4a)))entry[1]=1;
        }
        count=*(DWORD*)(nativeState+4);
    }while(++index!=count);
    PurgeUnusedCache();
}
// samp.dll RVA: 0x0000d030
void CCustomModels::SetExpectedRecordCount(DWORD count) {
    *(DWORD*)(nativeState+0x21b)=count;
    if(*(DWORD*)(nativeState+4)==count) {
        nativeState[0x21f]=1;RefreshCachedResources();if(!*(DWORD*)(nativeState+0x213))*(DWORD*)(nativeState+0x213)=2;
    }
}
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

// samp.dll RVA: 0x0000c430
DWORD CCustomModels::GetCount() {
    return *(DWORD*)(nativeState+4);
}

// samp.dll RVA: 0x0000c410
void * CCustomModels::GetRecord(DWORD index) {
    return index<*(DWORD*)(nativeState+4)?(*(void***)nativeState)[index]:0;
}
// samp.dll RVA: 0x00008200
DWORD CCustomModels::GetActiveVirtualWorld() {
    return *(DWORD*)(nativeState+0x217);
}
// samp.dll RVA: 0x00008210
BYTE CCustomModels::GetStopFlag() {
    return nativeState[0x221];
}
// samp.dll RVA: 0x0000e400
void CCustomModels::RequestStop() {
    nativeState[0x221]=1;StopWorkers();
}

// samp.dll RVA: 0x0000cf90
void CCustomModels::ClearRecordStorage() {
    ResizeRecords(0);
}
// samp.dll RVA: 0x0000beb0
DWORD CCustomModels::CountEnabledActiveWorkers() {
    CustomModelWorker *workers=(CustomModelWorker*)(nativeState+0x1345);DWORD count=0;
    for(int i=0;i<6;++i)if(workers[i].enabled&&workers[i].active)++count;
    return count;
}
// samp.dll RVA: 0x0000bf40
DWORD CCustomModels::CountEnabledPausedWorkers() {
    CustomModelWorker *workers=(CustomModelWorker*)(nativeState+0x1345);DWORD count=0;
    for(int i=0;i<6;++i)if(workers[i].enabled&&workers[i].paused)++count;
    return count;
}
