#ifndef SAMP_TEXTDRAW_ADDRESS_INTERFACE_H
#define SAMP_TEXTDRAW_ADDRESS_INTERFACE_H
#include <stddef.h>
#include <windows.h>
#pragma pack(push, 1)
// Packed native transmit record; fields are transferred as raw bits by the constructor.
// DL places flags first, unlike the 0.2x record. No standalone native constructor.
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
    // 0x000b32e0
    CTextDraw(const TextDrawTransmit* info, const char* text);
    // 0x00070d10
    RECT* GetSelectableRect();
    // 0x00070d30
    void SetHover(BYTE active, DWORD color);
    // 0x000b2b50
    ~CTextDraw();
    // 0x000b2b60
    void SetText(const char* text);
    // Texture-name resolution; descriptive recovered method name.
    // 0x000b28d0
    void LoadTexture();
    // 0x000b3080
    void Draw();
    // Preview coordinator; actual GTA preview execution remains unverified.
    // 0x000b30a0
    void Native0B30A0();
    // 0x000b32b0
    void OnLostDevice();
    // Recovered drawing backends; original method names remain unestablished.
    // 0x000b2c90
    void Native0B2C90();
    // 0x000b2da0
    void Native0B2DA0();
};
#pragma pack(pop)
typedef char TextDrawTransmitExtent[(sizeof(TextDrawTransmit) == 63) ? 1 : -1];
typedef char TextDrawTransmitWidthOffset[(offsetof(TextDrawTransmit, letterWidth) == 1) ? 1 : -1];
typedef char TextDrawTransmitShadowOffset[(offsetof(TextDrawTransmit, shadow) == 0x19) ? 1 : -1];
typedef char TextDrawTransmitStyleOffset[(offsetof(TextDrawTransmit, style) == 0x1f) ? 1 : -1];
typedef char TextDrawTransmitPositionOffset[(offsetof(TextDrawTransmit, x) == 0x21) ? 1 : -1];
typedef char TextDrawTransmitModelOffset[(offsetof(TextDrawTransmit, model) == 0x29) ? 1 : -1];
typedef char TextDrawTransmitRotationOffset[(offsetof(TextDrawTransmit, rotation) == 0x2b) ? 1 : -1];
typedef char TextDrawTransmitZoomOffset[(offsetof(TextDrawTransmit, zoom) == 0x37) ? 1 : -1];
typedef char TextDrawTransmitColorsOffset[(offsetof(TextDrawTransmit, vehicleColor1) == 0x3b) ? 1 : -1];
typedef char TextDrawDataExtent[(sizeof(TextDrawData) == 91) ? 1 : -1];
typedef char TextDrawLineWidthOffset[(offsetof(TextDrawData, lineWidth) == 0x0f) ? 1 : -1];
typedef char TextDrawBackgroundOffset[(offsetof(TextDrawData, backgroundColor) == 0x1c) ? 1 : -1];
typedef char TextDrawStyleOffset[(offsetof(TextDrawData, style) == 0x24) ? 1 : -1];
typedef char TextDrawTextureSlotOffset[(offsetof(TextDrawData, textureSlot) == 0x40) ? 1 : -1];
typedef char TextDrawModelOffset[(offsetof(TextDrawData, model) == 0x45) ? 1 : -1];
typedef char TextDrawProcessedTextOffset[(offsetof(TextDrawState, processedText) == 0x321) ? 1 : -1];
typedef char TextDrawFormatOffset[(offsetof(TextDrawState, format) == 0x963) ? 1 : -1];
typedef char TextDrawRectFlagOffset[(offsetof(TextDrawState, hasSelectableRect) == 0x9bf) ? 1 : -1];
typedef char TextDrawKeyCodesOffset[(offsetof(TextDrawState, hasKeyCodes) == 0x9c0) ? 1 : -1];
typedef char TextDrawRectOffset[(offsetof(TextDrawState, selectableRect) == 0x9c1) ? 1 : -1];
typedef char TextDrawHoverOffset[(offsetof(TextDrawState, hovered) == 0x9d1) ? 1 : -1];
typedef char TextDrawStateExtent[(sizeof(TextDrawState) == 0x9d6) ? 1 : -1];
typedef char NativeTextDrawExtent[(sizeof(CTextDraw) == 0x9d6) ? 1 : -1];
// Shared texture-slot release entry.
// 0x000b2750
void GameReleaseTextDrawTexture(int slot);
// 0x0011f54c
extern const char g_textDrawUnmatchedTilde[23];
// 0x0011f524
extern const char g_textDrawLongKeyString[40];
// Observed RenderWare rectangle order; no standalone constructor is established.
struct TextDrawRect
{
    float left, bottom, right, top;
};
// 0x002a9400
extern void* g_textDrawTextures[200];
// 0x002a9720
extern DWORD g_textDrawTextureUsed[200];
// 0x00117884
extern const float g_textDrawHalf;
// 0x0011f564
extern const float g_textDrawVirtualWidth;
// 0x0011f568
extern const float g_textDrawVirtualHeight;
// 0x000b2720
int GameAllocateTextDrawTexture();
// 0x000b2750
void GameReleaseTextDrawTexture(int slot);
// 0x000b3560
void GameDestroyTextDrawTexture(void* texture);
// 0x000b28a0
void GameDrawTextDrawTexture(void* texture, TextDrawRect* rect, DWORD* color);
// 0x000b26f0
BOOL GameInitializeTextDrawTextures();
#endif
