#include "game/textdraw.h"

// samp.dll RVA: 0x000b3080
void CTextDraw::Draw()
{
    if (*(int*)((BYTE*)this + 0x9a3) != -1)
        Native0B2C90();
    else
        Native0B2DA0();
}
// samp.dll RVA: 0x000b32b0
void CTextDraw::OnLostDevice()
{
    BYTE* state = (BYTE*)this;
    if (*(DWORD*)(state + 0x987) == 5 && *(int*)(state + 0x9a3) != -1)
    {
        GameReleaseTextDrawTexture(*(int*)(state + 0x9a3));
        *(int*)(state + 0x9a3) = -1;
    }
}

#include "d3dhook/texture_binding.h"
#include "game/font.h"
#include "gui/textdraw_texture.h"
extern "C"
{
    // MSVC CRT helper, supplied by the selected static runtime, not recovered here.
    // samp.dll RVA: 0x000f9324
    __int64 __cdecl _ftol2();
}

// samp.dll RVA: 0x002a9400
void* g_textDrawTextures[200];
// samp.dll RVA: 0x002a9720
DWORD g_textDrawTextureUsed[200];
// samp.dll RVA: 0x00117884
const float g_textDrawHalf = 0.5f;
// samp.dll RVA: 0x0011f564
const float g_textDrawVirtualWidth = 640.0f;
// samp.dll RVA: 0x0011f568
const float g_textDrawVirtualHeight = 448.0f;

// samp.dll RVA: 0x000b2720
int GameAllocateTextDrawTexture()
{
    for (int i = 0; i < 200; i++)
        if (!g_textDrawTextureUsed[i])
        {
            g_textDrawTextureUsed[i] = 1;
            return i;
        }
    return -1;
}
// samp.dll RVA: 0x000b2750
void GameReleaseTextDrawTexture(int slot)
{
    if (slot >= 0 && slot < 200)
    {
        g_textDrawTextureUsed[slot] = 0;
        if (g_textDrawTextures[slot])
            GameDestroyTextDrawTexture(g_textDrawTextures[slot]);
        g_textDrawTextures[slot] = 0;
    }
}
// samp.dll RVA: 0x000b3560
void GameDestroyTextDrawTexture(void* texture)
{
    // GTA SA virtual address; native supplies the address of a local texture word.
    ((void(__fastcall*)(void*, void*))0x7281e0)(&texture, 0);
}
// samp.dll RVA: 0x000b28a0
void GameDrawTextDrawTexture(void* texture, TextDrawRect* rect, DWORD* color)
{
    if (texture)
    {
        ClientSetRenderState(9, 2);
        // GTA SA virtual address; raw handle remains the this argument.
        ((void(__fastcall*)(void*, void*, TextDrawRect*, DWORD*))0x728350)(texture, 0, rect, color);
    }
}
// samp.dll RVA: 0x000b2c90
void CTextDraw::Native0B2C90()
{
    BYTE* state = (BYTE*)this;
    BYTE highlighted = state[0x9d1];
    float scaleX, scaleY;
    TextDrawRect rect;
    // clang-format off
    __asm {
        fild dword ptr ds:[0xc17044]
        fmul dword ptr ds:[0x859520]
        fstp scaleX
        fild dword ptr ds:[0xc17048]
        fmul dword ptr ds:[0x859524]
        fstp scaleY
        mov eax,state
        fld dword ptr [eax+0x98b]
        fmul scaleX
        fstp rect.left
        fld dword ptr [eax+0x98f]
        fmul scaleY
        fstp rect.top
        fld dword ptr [eax+0x98b]
        fadd dword ptr [eax+0x972]
        fmul scaleX
        fstp rect.right
        fld dword ptr [eax+0x98f]
        fadd dword ptr [eax+0x976]
        fmul scaleY
        fstp rect.bottom
    }
    // clang-format on
    DWORD* color = (DWORD*)(state + (highlighted ? 0x9d2 : 0x96b));
    // Native has no index validation here; out-of-bank adjacency is not established.
    GameDrawTextDrawTexture(g_textDrawTextures[*(int*)(state + 0x9a3)], &rect, color);
    int left, top;
    // clang-format off
    __asm {
        mov edi,state
        fld rect.left
        call _ftol2
        mov left,eax
        mov [edi+0x9c1],eax
        fld scaleX
        fmul dword ptr [edi+0x972]
        fiadd left
        call _ftol2
        mov [edi+0x9c9],eax
        fld rect.top
        call _ftol2
        mov top,eax
        mov [edi+0x9c5],eax
        fld scaleY
        fmul dword ptr [edi+0x976]
        fiadd top
        call _ftol2
        mov [edi+0x9cd],eax
    }
    // clang-format on
    state[0x9bf] = 1;
}
// samp.dll RVA: 0x000b2da0
void CTextDraw::Native0B2DA0()
{
    BYTE* state = (BYTE*)this;
    if (!state)
        return;
    char *text = (char*)state, *end = text;
    while (*end++)
    {
    }
    if (end == text + 1)
        return;
    char *buffer = text + 0x321, *from = text, *to = buffer;
    do
    {
        *to++ = *from;
    }
    while (*from++);
    float screenW = (float)*(int*)0xc17044, screenH = (float)*(int*)0xc17048;
    float hudX = *(float*)0x859520, hudY = *(float*)0x859524;
    float scaleX, scaleY, wrapX, wrapY, posX, posY;
    // clang-format off
    __asm {
        mov eax,state
        fld screenH
        fmul dword ptr [eax+0x967]
        fmul hudY
        fmul g_textDrawHalf
        fstp scaleY
        fld screenW
        fmul dword ptr [eax+0x963]
        fmul hudX
        fstp scaleX
    }
    // clang-format on
    Font_SetScale(scaleX, scaleY);
    Font_SetColor(*(DWORD*)(state + (state[0x9d1] ? 0x9d2 : 0x96b)));
    Font_Unk12(0);
    Font_SetJustify(state[0x986] ? 2 : (state[0x970] ? 0 : 1));
    // clang-format off
    __asm {
        mov eax,state
        fld screenW
        fmul dword ptr [eax+0x972]
        fmul hudX
        fstp wrapX
    }
    // clang-format on
    Font_SetLineWidth(wrapX);
    // clang-format off
    __asm {
        mov eax,state
        fld screenW
        fmul dword ptr [eax+0x976]
        fmul hudX
        fstp wrapY
    }
    // clang-format on
    Font_SetLineHeight(wrapY);
    Font_UseBox(state[0x971], 0);
    Font_UseBoxColor(*(DWORD*)(state + 0x97a));
    Font_SetProportional(state[0x97e]);
    Font_SetDropColor(*(DWORD*)(state + 0x97f));
    if (state[0x984])
        Font_SetOutline(state[0x984]);
    else
        Font_SetShadow(state[0x983]);
    Font_SetFontStyle(*(int*)(state + 0x987));
    Font_UnkConv(buffer, -1, -1, -1, -1, -1, -1, buffer);
    if (state[0x9c0])
        Font_UnkConv2(buffer);
    // clang-format off
    __asm {
        mov eax,state
        fld g_textDrawVirtualHeight
        fsub dword ptr [eax+0x98f]
        fld screenH
        fmul hudY
        fmulp st(1),st(0)
        fsubr screenH
        fstp posY
        fld g_textDrawVirtualWidth
        fsub dword ptr [eax+0x98b]
        fld screenW
        fmul hudX
        fmulp st(1),st(0)
        fsubr screenW
        fstp posX
    }
    // clang-format on
    Font_PrintString(posX, posY, buffer);
    Font_SetOutline(0);
    if (state[0x986])
    {
        // clang-format off
        __asm {
            mov edi,state
            fld wrapX
            fsub posX
            fsubr posX
            call _ftol2
            mov [edi+0x9c1],eax
            fld posX
            call _ftol2
            mov [edi+0x9c9],eax
        }
        // clang-format on
    }
    else if (state[0x970])
    {
        int left;
        // clang-format off
        __asm {
            mov edi,state
            fld wrapY
            fmul g_textDrawHalf
            fsubr posX
            call _ftol2
            mov left,eax
            mov [edi+0x9c1],eax
            fld wrapY
            call _ftol2
            add eax,left
            mov [edi+0x9c9],eax
        }
        // clang-format on
    }
    else
    {
        // clang-format off
        __asm {
            mov edi,state
            fld posX
            call _ftol2
            mov [edi+0x9c1],eax
            fld wrapX
            fsub posX
            fadd posX
            call _ftol2
            mov [edi+0x9c9],eax
        }
        // clang-format on
    }
    // clang-format off
    __asm {
        mov edi,state
        fld posY
        call _ftol2
        mov [edi+0x9c5],eax
    }
    // clang-format on
    if (!state[0x986] && state[0x970])
    {
        // clang-format off
        __asm {
            mov edi,state
            fld posY
            fadd wrapX
            call _ftol2
            mov [edi+0x9cd],eax
        }
        // clang-format on
    }
    else
    {
        // clang-format off
        __asm {
            mov edi,state
            fld posY
            fadd wrapY
            call _ftol2
            mov [edi+0x9cd],eax
        }
        // clang-format on
    }
    state[0x9bf] = 1;
}

// Native clears usage flags first, then texture words, without releasing them.
// samp.dll RVA: 0x000b26f0
BOOL GameInitializeTextDrawTextures()
{
    memset(g_textDrawTextureUsed, 0, sizeof(g_textDrawTextureUsed));
    memset(g_textDrawTextures, 0, sizeof(g_textDrawTextures));
    return TRUE;
}

#include "game/custommodels.h"
#include "game/game.h"
#include "game/model_preview.h"

// samp.dll RVA: 0x000b30a0
void CTextDraw::Native0B30A0()
{
    BYTE* state = (BYTE*)this;
    if (*(DWORD*)(state + 0x987) != 5 || *(int*)(state + 0x9a3) != -1)
        return;
    short* model = (short*)(state + 0x9a8);
    if (g_customModels)
    {
        if (g_customModels->IsPedEntry(*model))
        {
            if (!GameIsPedModel(*model) && !g_customModels->DoesExist(*model))
                *model = 0;
        }
        else if (g_customModels->IsObjectEntry(*model) && !g_customModels->GetLoadedFlag(*model) && !g_customModels->LoadObjectEntry(*model))
        {
            *model = 0x48c7;
        }
    }
    void* created;
    if (GameIsPedModel(*model))
    {
        if (!GameHasModelInfo(*model))
            *model = 0;
        created = g_modelPreview->RenderPed(*model, *(DWORD*)(state + 0x97f), (CVector*)(state + 0x9aa), *(float*)(state + 0x9b6));
    }
    else if (*model >= 400 && *model <= 611)
    {
        created = g_modelPreview->RenderVehicle(*model, *(DWORD*)(state + 0x97f), (CVector*)(state + 0x9aa), *(float*)(state + 0x9b6), *(short*)(state + 0x9ba),
                                                *(short*)(state + 0x9bc));
    }
    else if ((*model >= 321 && *model <= 373) || *model >= 615)
    {
        if (!GameHasModelInfo(*model))
            *model = 0x48c7;
        created = g_modelPreview->RenderObject(*model, *(DWORD*)(state + 0x97f), (CVector*)(state + 0x9aa), *(float*)(state + 0x9b6));
    }
    else
    {
        if (*model >= 0)
            return;
        if (!GameModelQueryB4170(*model))
            *model = 0x48c7;
        created = g_modelPreview->RenderObject(*model, *(DWORD*)(state + 0x97f), (CVector*)(state + 0x9aa), *(float*)(state + 0x9b6));
    }
    if (created)
    {
        int slot = GameAllocateTextDrawTexture();
        *(int*)(state + 0x9a3) = slot;
        // Native publishes even slot=-1. Adjacent storage is not established here.
        g_textDrawTextures[slot] = created;
    }
}

#include <string.h>
// samp.dll RVA: 0x0011f54c
const char g_textDrawUnmatchedTilde[23] = "Error: unmatched tilde";
// samp.dll RVA: 0x0011f524
const char g_textDrawLongKeyString[40] = "Error: can't use k-codes in long string";
// samp.dll RVA: 0x00070d10
RECT* CTextDraw::GetSelectableRect()
{
    BYTE* state = (BYTE*)this;
    return state[0x9a7] && state[0x9bf] ? (RECT*)(state + 0x9c1) : 0;
}
// samp.dll RVA: 0x00070d30
void CTextDraw::SetHover(BYTE active, DWORD color)
{
    BYTE* state = (BYTE*)this;
    state[0x9d1] = active;
    *(DWORD*)(state + 0x9d2) = color;
}
// samp.dll RVA: 0x000b2b50
CTextDraw::~CTextDraw()
{
    GameReleaseTextDrawTexture(*(int*)((BYTE*)this + 0x9a3));
}
// samp.dll RVA: 0x000b2b60
void CTextDraw::SetText(const char* text)
{
    BYTE* state = (BYTE*)this;
    // Clear precedes input length lookup, including aliased input.
    memset(state, 0, 800);
    if (strlen(text) >= 800)
        return;
    strncpy((char*)state, text, 800);
    state[800] = 0;
    state[0x9c0] = 0;
    bool inside = false, valid = true;
    int length = 0;
    for (char* p = (char*)state; *p; ++p)
    {
        if (*p == '~')
        {
            if (inside)
            {
                length = 0;
                inside = false;
                continue;
            }
            inside = true;
            if (p[1] == 'k' || p[1] == 'K')
                state[0x9c0] = 1;
        }
        if (inside && ++length > 24)
            valid = false;
    }
    if (!valid || inside)
    {
        memcpy(state, g_textDrawUnmatchedTilde, 23);
        state[0x9c0] = 0;
    }
    if (state[0x9c0] && strlen((char*)state) > 255)
    {
        memcpy(state, g_textDrawLongKeyString, 40);
        state[0x9c0] = 0;
    }
    if (*(DWORD*)(state + 0x987) == 4 && *(int*)(state + 0x9a3) != -1)
    {
        GameReleaseTextDrawTexture(*(int*)(state + 0x9a3));
        *(int*)(state + 0x9a3) = GameAllocateTextDrawTexture();
        LoadTexture();
    }
}

#include "game/custommodels.h"
#include "game/effects.h"
#include <stdio.h>
#include <stdlib.h>
// samp.dll RVA: 0x000b28d0
void CTextDraw::LoadTexture()
{
    char dictionary[65] = {0}, texture[65] = {0}, selected[65] = {0};
    const char* text = (const char*)this;
    const char* colon = strchr(text, ':');
    if (!colon || strlen(text) >= 64 || strchr(text, '\\') || strchr(text, '/'))
        return;
    strncpy(dictionary, text, colon - text);
    strcpy(texture, colon + 1);
    BYTE* state = (BYTE*)this;
    if (*(int*)(state + 0x9a3) == -1)
        return;
    if (!memcmp(dictionary, "hud", 4))
        strcpy(selected, "hud");
    else if (!memcmp(dictionary, "samaps", 7))
    {
        strcpy(selected, "samaps");
        if (!GameLoadTextureDictionary(selected, "samaps", 1))
            return;
    }
    else if (!memcmp(dictionary, "vehicleprev", 12))
    {
        strcpy(selected, "vehicleprev");
        if (!GameLoadTextureDictionary(selected, "vehicles128", 1))
            return;
    }
    else if (g_customModels && strlen(dictionary) < 10 && !_strnicmp(dictionary, "mdl", 3))
    {
        int model = atoi(dictionary + 3);
        if (!g_customModels->IsObjectEntry(model))
            return;
        if (!g_customModels->GetLoadedFlag(model) && !g_customModels->LoadObjectEntry(model))
            return;
        DWORD hash = g_customModels->GetObjectTextureHash(model);
        if (!hash)
            return;
        sprintf(selected, "%X", hash);
    }
    else
    {
        strncpy(selected, dictionary, 10);
        if (!GameLoadTextureDictionary(selected, dictionary, 0))
            return;
    }
    void* loaded = GameLoadDictionaryTexture(selected, texture);
    g_textDrawTextures[*(int*)(state + 0x9a3)] = loaded;
}

// samp.dll RVA: 0x000b32e0
CTextDraw::CTextDraw(const TextDrawTransmit* info, const char* text)
{
    BYTE* state = (BYTE*)this;
    const BYTE* input = info->data;
    memset(state + 0x963, 0, 91);
    *(DWORD*)(state + 0x963) = *(const DWORD*)(input + 1);
    *(DWORD*)(state + 0x967) = *(const DWORD*)(input + 5);
    *(DWORD*)(state + 0x96b) = *(const DWORD*)(input + 9);
    state[0x96f] = 0;
    state[0x970] = (input[0] >> 3) & 1;
    state[0x971] = input[0] & 1;
    *(DWORD*)(state + 0x972) = *(const DWORD*)(input + 0xd);
    *(DWORD*)(state + 0x976) = *(const DWORD*)(input + 0x11);
    *(DWORD*)(state + 0x97a) = *(const DWORD*)(input + 0x15);
    state[0x97e] = (input[0] >> 4) & 1;
    *(DWORD*)(state + 0x97f) = *(const DWORD*)(input + 0x1b);
    state[0x983] = input[0x19];
    state[0x984] = input[0x1a];
    state[0x985] = (input[0] >> 1) & 1;
    state[0x986] = (input[0] >> 2) & 1;
    *(DWORD*)(state + 0x987) = input[0x1f];
    *(DWORD*)(state + 0x98b) = *(const DWORD*)(input + 0x21);
    *(DWORD*)(state + 0x98f) = *(const DWORD*)(input + 0x25);
    *(DWORD*)(state + 0x99b) = 0xffffffff;
    *(DWORD*)(state + 0x99f) = 0xffffffff;
    state[0x9a7] = input[0x20];
    *(WORD*)(state + 0x9a8) = *(const WORD*)(input + 0x29);
    *(DWORD*)(state + 0x9aa) = *(const DWORD*)(input + 0x2b);
    *(DWORD*)(state + 0x9ae) = *(const DWORD*)(input + 0x2f);
    *(DWORD*)(state + 0x9b2) = *(const DWORD*)(input + 0x33);
    *(DWORD*)(state + 0x9b6) = *(const DWORD*)(input + 0x37);
    *(WORD*)(state + 0x9ba) = *(const WORD*)(input + 0x3b);
    *(WORD*)(state + 0x9bc) = *(const WORD*)(input + 0x3d);
    state[0x9c0] = 0;
    *(int*)(state + 0x9a3) = -1;
    SetText(text);
    if (*(DWORD*)(state + 0x987) == 4)
    {
        *(int*)(state + 0x9a3) = GameAllocateTextDrawTexture();
        LoadTexture();
    }
    state[0x9bf] = 0;
    *(DWORD*)(state + 0x9c1) = 0;
    *(DWORD*)(state + 0x9c9) = 0;
    *(DWORD*)(state + 0x9c5) = 0;
    *(DWORD*)(state + 0x9cd) = 0;
    state[0x9d1] = 0;
    *(DWORD*)(state + 0x9d2) = 0;
}
