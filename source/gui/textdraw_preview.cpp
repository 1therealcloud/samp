#include "gui/textdraw.h"
#include "gui/textdraw_texture.h"
#include "game/model.h"
#include "game/model_preview.h"
#include "game/custommodels.h"

// samp.dll RVA: 0x000b30a0
void CTextDraw::Native0B30A0() {
    BYTE *state=(BYTE*)this;
    if(*(DWORD*)(state+0x987)!=5 || *(int*)(state+0x9a3)!=-1)return;
    short *model=(short*)(state+0x9a8);
    if(g_customModels) {
        if(g_customModels->IsPedEntry(*model)) {
            if(!GameIsPedModel(*model) && !g_customModels->DoesExist(*model))*model=0;
        } else if(g_customModels->IsObjectEntry(*model) && !g_customModels->GetLoadedFlag(*model) && !g_customModels->LoadObjectEntry(*model)) {
            *model=0x48c7;
        }
    }
    void *created;
    if(GameIsPedModel(*model)) {
        if(!GameHasModelInfo(*model))*model=0;
        created=g_modelPreview->RenderPed(*model,*(DWORD*)(state+0x97f),(CVector*)(state+0x9aa),*(float*)(state+0x9b6));
    } else if(*model>=400 && *model<=611) {
        created=g_modelPreview->RenderVehicle(*model,*(DWORD*)(state+0x97f),(CVector*)(state+0x9aa),*(float*)(state+0x9b6),*(short*)(state+0x9ba),*(short*)(state+0x9bc));
    } else if((*model>=321 && *model<=373) || *model>=615) {
        if(!GameHasModelInfo(*model))*model=0x48c7;
        created=g_modelPreview->RenderObject(*model,*(DWORD*)(state+0x97f),(CVector*)(state+0x9aa),*(float*)(state+0x9b6));
    } else {
        if(*model>=0)return;
        if(!GameModelQueryB4170(*model))*model=0x48c7;
        created=g_modelPreview->RenderObject(*model,*(DWORD*)(state+0x97f),(CVector*)(state+0x9aa),*(float*)(state+0x9b6));
    }
    if(created) {
        int slot=GameAllocateTextDrawTexture();
        *(int*)(state+0x9a3)=slot;
        // Native publishes even slot=-1. Adjacent storage is not established here.
        g_textDrawTextures[slot]=created;
    }
}
