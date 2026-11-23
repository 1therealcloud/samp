#include "game/textdraw.h"

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/custommodels.h"
#include "game/font.h"
#include "game/game.h"
#include "game/model_preview.h"
#include "game/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 0x002a9400
void* g_textDrawTextures[200];
// 0x002a9720
DWORD g_textDrawTextureUsed[200];
// 0x00117884
const float g_textDrawHalf = 0.5f;
// 0x0011f564
const float g_textDrawVirtualWidth = 640.0f;
// 0x0011f568
const float g_textDrawVirtualHeight = 448.0f;

// 0x0011f54c
const char g_textDrawUnmatchedTilde[23] = "Error: unmatched tilde";
// 0x0011f524
const char g_textDrawLongKeyString[40] = "Error: can't use k-codes in long string";

// 0x000b3080
void CTextDraw::Draw()
{
    if (data.format.textureSlot != -1)
        DrawTexture();
    else
        DrawText();
}

// 0x000b32b0
void CTextDraw::OnLostDevice()
{
    if (data.format.style == 5 && data.format.textureSlot != -1)
    {
        GameReleaseTextDrawTexture(data.format.textureSlot);
        data.format.textureSlot = -1;
    }
}

// 0x000b2720
int GameAllocateTextDrawTexture()
{
    for (int i = 0; i != 200; i++)
        if (!g_textDrawTextureUsed[i])
        {
            g_textDrawTextureUsed[i] = 1;
            return i;
        }
    return -1;
}

// 0x000b2750
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

// 0x000b3560
void GameDestroyTextDrawTexture(void* texture)
{
    void* spriteTexture = texture;
    // GTA CSprite2d deletion receives the address of a one-pointer local.
    // clang-format off
    __asm {
        lea ecx,spriteTexture
        mov edx,0x7281e0
        call edx
    }
    // clang-format on
}

// 0x000b28a0
void GameDrawTextDrawTexture(void* texture, TextDrawRect* rect, DWORD* color)
{
    if (texture)
    {
        ClientSetRenderState(9, 2);
        // GTA SA virtual address; raw handle remains the this argument.
        typedef void (__fastcall *BackendFn)(void*, int, void*, void*);
        reinterpret_cast<BackendFn>(0x728350)((void*)texture, 0, (void*)rect, (void*)color);
    }
}

// 0x000b2c90
void CTextDraw::DrawTexture()
{
    const BYTE highlighted = data.hovered;
    const volatile float scaleX = (float)((double)*(int*)0xc17044 * *(float*)0x859520);
    const volatile float scaleY = (float)((double)*(int*)0xc17048 * *(float*)0x859524);
    TextDrawRect rect;
    rect.left = data.format.x * scaleX;
    rect.top = data.format.y * scaleY;
    rect.right = (float)(((double)data.format.x + data.format.lineWidth) * scaleX);
    rect.bottom = (float)(((double)data.format.y + data.format.lineHeight) * scaleY);
    DWORD* color = (highlighted ? &data.hoverColor : &data.format.letterColor);
    // The original leaves the index unchecked.
    GameDrawTextDrawTexture(g_textDrawTextures[data.format.textureSlot], &rect, color);
    RECT& selection = data.selectableRect;
    const LONG left = (LONG)(__int64)rect.left;
    selection.left = left;
    selection.right = (LONG)(__int64)((double)scaleX * data.format.lineWidth + left);
    const LONG top = (LONG)(__int64)rect.top;
    selection.top = top;
    selection.bottom = (LONG)(__int64)((double)scaleY * data.format.lineHeight + top);
    data.hasSelectableRect = 1;
}

// 0x000b2da0
void CTextDraw::DrawText()
{
    BYTE* state = (BYTE*)this;
    if (!state)
        return;
    char *text = data.text, *end = text;
    while (*end++)
    {
    }
    if (end == text + 1)
        return;
    char *buffer = data.processedText, *from = text, *to = buffer;
    do
    {
        *to++ = *from;
    }
    while (*from++);
    float screenW = (float)*(int*)0xc17044, screenH = (float)*(int*)0xc17048;
    float hudX = *(float*)0x859520, hudY = *(float*)0x859524;
    const float scaleY = (float)((double)screenH * data.format.letterHeight * hudY * g_textDrawHalf);
    const float scaleX = (float)((double)screenW * data.format.letterWidth * hudX);
    Font_SetScale(scaleX, scaleY);
    Font_SetColor(*(data.hovered ? &data.hoverColor : &data.format.letterColor));
    Font_SetJustify(0);
    Font_SetOrientation(data.format.alignRight ? 2 : (data.format.centered ? 0 : 1));
    const volatile float wrapX = (float)((double)screenW * data.format.lineWidth * hudX);
    Font_SetLineWidth(wrapX);
    const volatile float wrapY = (float)((double)screenW * data.format.lineHeight * hudX);
    Font_SetLineHeight(wrapY);
    Font_UseBox(data.format.box, 0);
    Font_UseBoxColor(data.format.boxColor);
    Font_SetProportional(data.format.proportional);
    Font_SetDropColor(data.format.backgroundColor);
    if (data.format.outline)
        Font_SetOutline(data.format.outline);
    else
        Font_SetShadow(data.format.shadow);
    Font_SetFontStyle(data.format.style);
    Font_GetTextSubstitutionAddress(buffer, -1, -1, -1, -1, -1, -1, buffer);
    if (data.hasKeyCodes)
        Font_ExpandKeyTokens(buffer);
    const volatile float posY = (float)(screenH - ((double)g_textDrawVirtualHeight - data.format.y) * ((double)screenH * hudY));
    const volatile float posX = (float)(screenW - ((double)g_textDrawVirtualWidth - data.format.x) * ((double)screenW * hudX));
    Font_PrintString(posX, posY, buffer);
    Font_SetOutline(0);

    RECT& selection = data.selectableRect;
    if (data.format.alignRight)
    {
        selection.left = (LONG)(__int64)(posX - ((double)wrapX - posX));
        selection.right = (LONG)(__int64)posX;
    }
    else if (data.format.centered)
    {
        const LONG left = (LONG)(__int64)(posX - (double)wrapY * g_textDrawHalf);
        selection.left = left;
        selection.right = (LONG)((DWORD)left + (DWORD)(__int64)wrapY);
    }
    else
    {
        selection.left = (LONG)(__int64)posX;
        selection.right = (LONG)(__int64)(((double)wrapX - posX) + posX);
    }
    selection.top = (LONG)(__int64)posY;
    const float height = !data.format.alignRight && data.format.centered ? wrapX : wrapY;
    selection.bottom = (LONG)(__int64)((double)posY + height);
    data.hasSelectableRect = 1;
}

// Native clears usage flags first, then texture words, without releasing them.
// 0x000b26f0
BOOL GameInitializeTextDrawTextures()
{
    memset(g_textDrawTextureUsed, 0, sizeof(g_textDrawTextureUsed));
    memset(g_textDrawTextures, 0, sizeof(g_textDrawTextures));
    return TRUE;
}

// 0x000b30a0
void CTextDraw::PrepareModelPreview()
{
    if (data.format.style != 5 || data.format.textureSlot != -1)
        return;
    short* model = &data.format.model;
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
        created = g_modelPreview->RenderPed(*model, data.format.backgroundColor, (CVector*)data.format.rotation, data.format.zoom);
    }
    else if (*model >= 400 && *model <= 611)
    {
        created = g_modelPreview->RenderVehicle(*model, data.format.backgroundColor, (CVector*)data.format.rotation, data.format.zoom, data.format.vehicleColor1,
                                                data.format.vehicleColor2);
    }
    else if ((*model > 320 && *model <= 373) || *model >= 615)
    {
        if (!GameHasModelInfo(*model))
            *model = 0x48c7;
        created = g_modelPreview->RenderObject(*model, data.format.backgroundColor, (CVector*)data.format.rotation, data.format.zoom);
    }
    else
    {
        if (*model >= 0)
            return;
        if (!GameHasModelInfoForObject(*model))
            *model = 0x48c7;
        created = g_modelPreview->RenderObject(*model, data.format.backgroundColor, (CVector*)data.format.rotation, data.format.zoom);
    }
    if (created)
    {
        int slot = GameAllocateTextDrawTexture();
        data.format.textureSlot = slot;
        // The original publishes the slot even when it is -1.
        g_textDrawTextures[slot] = created;
    }
}

// 0x00070d10
RECT* CTextDraw::GetSelectableRect()
{
    return data.format.selectable && data.hasSelectableRect ? &data.selectableRect : 0;
}

// 0x00070d30
void CTextDraw::SetHover(BYTE active, DWORD color)
{
    data.hovered = active;
    data.hoverColor = color;
}

// 0x000b2b50
CTextDraw::~CTextDraw()
{
    GameReleaseTextDrawTexture(data.format.textureSlot);
}

// 0x000b2b60
void CTextDraw::SetText(const char* text)
{
    BYTE* state = (BYTE*)this;
    // Clear precedes input length lookup, including aliased input.
    memset(state, 0, 800);
    if (strlen(text) >= 800)
        return;
    strncpy((char*)state, text, 800);
    state[800] = 0;
    data.hasKeyCodes = 0;
    char* p = (char*)state;
    bool inside = false, valid = true;
    int length = 0;
    for (; *p; ++p)
    {
        if (*p == '~')
        {
            if (!inside)
            {
                inside = true;
                if (p[1] == 'k' || p[1] == 'K')
                    data.hasKeyCodes = 1;
            }
            else
            {
                length = 0;
                inside = false;
                continue;
            }
        }
        if (inside && ++length > 24)
            valid = false;
    }
    if (!valid || inside)
    {
        memcpy(state, g_textDrawUnmatchedTilde, 23);
        data.hasKeyCodes = 0;
    }
    if (data.hasKeyCodes && strlen((char*)state) > 255)
    {
        memcpy(state, g_textDrawLongKeyString, 40);
        data.hasKeyCodes = 0;
    }
    if (data.format.style == 4 && data.format.textureSlot != -1)
    {
        GameReleaseTextDrawTexture(data.format.textureSlot);
        data.format.textureSlot = GameAllocateTextDrawTexture();
        LoadTexture();
    }
}

// 0x000b28d0
void CTextDraw::LoadTexture()
{
    char dictionary[65] = {0}, texture[65] = {0}, selected[65] = {0};
    const char* text = data.text;
    const char* colon = strchr(text, ':');
    if (!colon || strlen(text) >= 64 || strchr(text, '\\') || strchr(text, '/'))
        return;
    strncpy(dictionary, text, colon - text);
    strcpy(texture, colon + 1);
    if (data.format.textureSlot == -1)
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
    g_textDrawTextures[data.format.textureSlot] = loaded;
}

// 0x000b32e0
CTextDraw::CTextDraw(const TextDrawTransmit* info, const char* text)
{
    memset(&data.format, 0, sizeof(data.format));
    *(DWORD*)&data.format.letterWidth = *(const DWORD*)&info->letterWidth;
    *(DWORD*)&data.format.letterHeight = *(const DWORD*)&info->letterHeight;
    data.format.letterColor = info->letterColor;
    data.format.unknown0c = 0;
    data.format.centered = (info->flags >> 3) & 1;
    data.format.box = info->flags & 1;
    *(DWORD*)&data.format.lineWidth = *(const DWORD*)&info->lineWidth;
    *(DWORD*)&data.format.lineHeight = *(const DWORD*)&info->lineHeight;
    data.format.boxColor = info->boxColor;
    data.format.proportional = (info->flags >> 4) & 1;
    data.format.backgroundColor = info->backgroundColor;
    data.format.shadow = info->shadow;
    data.format.outline = info->outline;
    data.format.alignLeft = (info->flags >> 1) & 1;
    data.format.alignRight = (info->flags >> 2) & 1;
    data.format.style = info->style;
    *(DWORD*)&data.format.x = *(const DWORD*)&info->x;
    *(DWORD*)&data.format.y = *(const DWORD*)&info->y;
    data.format.parameter1 = 0xffffffff;
    data.format.parameter2 = 0xffffffff;
    data.format.selectable = info->selectable;
    data.format.model = info->model;
    *(DWORD*)&data.format.rotation[0] = *(const DWORD*)&info->rotation[0];
    *(DWORD*)&data.format.rotation[1] = *(const DWORD*)&info->rotation[1];
    *(DWORD*)&data.format.rotation[2] = *(const DWORD*)&info->rotation[2];
    *(DWORD*)&data.format.zoom = *(const DWORD*)&info->zoom;
    data.format.vehicleColor1 = info->vehicleColor1;
    data.format.vehicleColor2 = info->vehicleColor2;
    data.hasKeyCodes = 0;
    data.format.textureSlot = -1;
    SetText(text);
    if (data.format.style == 4)
    {
        data.format.textureSlot = GameAllocateTextDrawTexture();
        LoadTexture();
    }
    data.hasSelectableRect = 0;
    data.selectableRect.left = 0;
    data.selectableRect.right = 0;
    data.selectableRect.top = 0;
    data.selectableRect.bottom = 0;
    data.hovered = 0;
    data.hoverColor = 0;
}
