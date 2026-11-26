#ifndef TEXTDRAW_H
#define TEXTDRAW_H

#include <stddef.h>
#include <windows.h>

#pragma pack(push, 1)
// Packed native transmit record; fields are transferred as raw bits by the constructor.
// The DL record starts with the flags.
struct TextDrawTransmit
{
    union
    {
        BYTE data[63];
        struct
        {
            BYTE flags;
            float letterWidth, letterHeight;
            DWORD letterColor;
            float lineWidth, lineHeight;
            DWORD boxColor;
            BYTE shadow, outline;
            DWORD backgroundColor;
            BYTE style, selectable;
            float x, y;
            short model;
            float rotation[3];
            float zoom;
            short vehicleColor1, vehicleColor2;
        };
    };
};
// Native format/preview block cleared as 91 bytes by the constructor.
// Unknown bytes remain explicit; the 0.2x layout is not used as a DL offset map.
struct TextDrawData
{
    float letterWidth, letterHeight;
    DWORD letterColor;
    BYTE unknown0c, centered, box;
    float lineWidth, lineHeight;
    DWORD boxColor;
    BYTE proportional;
    DWORD backgroundColor;
    BYTE shadow, outline, alignLeft, alignRight;
    DWORD style;
    float x, y;
    BYTE unknown30[8];
    DWORD parameter1, parameter2;
    int textureSlot;
    BYTE selectable;
    short model;
    float rotation[3];
    float zoom;
    short vehicleColor1, vehicleColor2;
};
// Plain storage records have no established standalone native methods.
struct TextDrawState
{
    char text[801];
    char processedText[801];
    BYTE unknown642[801];
    TextDrawData format;
    BYTE unknown9be;
    BYTE hasSelectableRect, hasKeyCodes;
    RECT selectableRect;
    BYTE hovered;
    DWORD hoverColor;
};
class CTextDraw
{
    union
    {
        TextDrawState data;
        // Retained for the precision-sensitive native drawing assembly.
        BYTE nativeState[0x9d6];
    };

public:
    CTextDraw(const TextDrawTransmit* info, const char* text);

    RECT* GetSelectableRect();

    void SetHover(BYTE active, DWORD color);

    ~CTextDraw();

    void SetText(const char* text);
    // Texture-name resolution; descriptive recovered method name.

    void LoadTexture();

    void Draw();
    // Preview coordinator; actual GTA preview execution remains unverified.

    void PrepareModelPreview();

    void OnLostDevice();

    void DrawTexture();

    void DrawText();
};
#pragma pack(pop)
static_assert(sizeof(TextDrawTransmit) == 63);
static_assert(offsetof(TextDrawTransmit, letterWidth) == 1);
static_assert(offsetof(TextDrawTransmit, shadow) == 0x19);
static_assert(offsetof(TextDrawTransmit, style) == 0x1f);
static_assert(offsetof(TextDrawTransmit, x) == 0x21);
static_assert(offsetof(TextDrawTransmit, model) == 0x29);
static_assert(offsetof(TextDrawTransmit, rotation) == 0x2b);
static_assert(offsetof(TextDrawTransmit, zoom) == 0x37);
static_assert(offsetof(TextDrawTransmit, vehicleColor1) == 0x3b);
static_assert(sizeof(TextDrawData) == 91);
static_assert(offsetof(TextDrawData, lineWidth) == 0x0f);
static_assert(offsetof(TextDrawData, backgroundColor) == 0x1c);
static_assert(offsetof(TextDrawData, style) == 0x24);
static_assert(offsetof(TextDrawData, textureSlot) == 0x40);
static_assert(offsetof(TextDrawData, model) == 0x45);
static_assert(offsetof(TextDrawState, processedText) == 0x321);
static_assert(offsetof(TextDrawState, format) == 0x963);
static_assert(offsetof(TextDrawState, hasSelectableRect) == 0x9bf);
static_assert(offsetof(TextDrawState, hasKeyCodes) == 0x9c0);
static_assert(offsetof(TextDrawState, selectableRect) == 0x9c1);
static_assert(offsetof(TextDrawState, hovered) == 0x9d1);
static_assert(sizeof(TextDrawState) == 0x9d6);
static_assert(sizeof(CTextDraw) == 0x9d6);
// Shared texture-slot release entry.

void GameReleaseTextDrawTexture(int slot);

extern const char g_textDrawUnmatchedTilde[23];

extern const char g_textDrawLongKeyString[40];
// RenderWare rectangle field order.
struct TextDrawRect
{
    float left, bottom, right, top;
};

extern void* g_textDrawTextures[200];

extern DWORD g_textDrawTextureUsed[200];

extern const float g_textDrawHalf;

extern const float g_textDrawVirtualWidth;

extern const float g_textDrawVirtualHeight;

int GameAllocateTextDrawTexture();

void GameReleaseTextDrawTexture(int slot);

void GameDestroyTextDrawTexture(void* texture);

void GameDrawTextDrawTexture(void* texture, TextDrawRect* rect, DWORD* color);

BOOL GameInitializeTextDrawTextures();

#endif // TEXTDRAW_H
