#include "game/custommodels.h"

// samp.dll RVA: 0x002aca28
CCustomModels *g_customModels;

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
