#ifndef SAMP_TEXTDRAW_TEXTURE_H
#define SAMP_TEXTDRAW_TEXTURE_H
#include <windows.h>
// Observed RenderWare rectangle order; no standalone constructor is established.
struct TextDrawRect
{
    float left, bottom, right, top;
};
// samp.dll RVA: 0x002a9400
extern void* g_textDrawTextures[200];
// samp.dll RVA: 0x002a9720
extern DWORD g_textDrawTextureUsed[200];
// samp.dll RVA: 0x00117884
extern const float g_textDrawHalf;
// samp.dll RVA: 0x0011f564
extern const float g_textDrawVirtualWidth;
// samp.dll RVA: 0x0011f568
extern const float g_textDrawVirtualHeight;
// samp.dll RVA: 0x000b2720
int GameAllocateTextDrawTexture();
// samp.dll RVA: 0x000b2750
void GameReleaseTextDrawTexture(int slot);
// samp.dll RVA: 0x000b3560
void GameDestroyTextDrawTexture(void* texture);
// samp.dll RVA: 0x000b28a0
void GameDrawTextDrawTexture(void* texture, TextDrawRect* rect, DWORD* color);
// samp.dll RVA: 0x000b26f0
BOOL GameInitializeTextDrawTextures();
#endif
