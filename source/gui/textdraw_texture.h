#ifndef SAMP_TEXTDRAW_TEXTURE_H
#define SAMP_TEXTDRAW_TEXTURE_H
#include <windows.h>
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
