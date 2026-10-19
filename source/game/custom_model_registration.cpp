#include "game/custom_model_io.h"
#include "game/model.h"
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
