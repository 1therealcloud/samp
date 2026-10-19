#include "game/custommodels.h"
#include "game/custom_model_io.h"
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
