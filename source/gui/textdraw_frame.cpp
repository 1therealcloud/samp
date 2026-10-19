#include "gui/textdraw.h"

// samp.dll RVA: 0x000b3080
void CTextDraw::Draw() {
    if(*(int*)((BYTE*)this+0x9a3)!=-1)Native0B2C90();
    else Native0B2DA0();
}
// samp.dll RVA: 0x000b32b0
void CTextDraw::OnLostDevice() {
    BYTE *state=(BYTE*)this;
    if(*(DWORD*)(state+0x987)==5 && *(int*)(state+0x9a3)!=-1) {
        GameReleaseTextDrawTexture(*(int*)(state+0x9a3));
        *(int*)(state+0x9a3)=-1;
    }
}
