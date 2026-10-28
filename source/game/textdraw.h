#ifndef SAMP_TEXTDRAW_ADDRESS_INTERFACE_H
#define SAMP_TEXTDRAW_ADDRESS_INTERFACE_H
#include <windows.h>
// Packed native transmit record; fields are transferred as raw bits by the constructor.
struct TextDrawTransmit
{
    BYTE data[63];
};
#pragma pack(push, 1)
class CTextDraw
{
    // Proven allocation extent; unresolved field regions remain opaque.
    BYTE nativeState[0x9d6];

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
    // Preview coordinator; downstream preview renderers remain open.
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
typedef char NativeTextDrawExtent[(sizeof(CTextDraw) == 0x9d6) ? 1 : -1];
// Shared texture-slot release entry.
// 0x000b2750
void GameReleaseTextDrawTexture(int slot);
// 0x0011f54c
extern const char g_textDrawUnmatchedTilde[23];
// 0x0011f524
extern const char g_textDrawLongKeyString[40];
#endif
