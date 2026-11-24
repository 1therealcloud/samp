#ifndef SPAWNSCREEN_H
#define SPAWNSCREEN_H

#include "render/fontrender.h"

#include <d3dx9.h>

struct CVector;
struct SpawnScreenVertex
{
    float x, y, z, rhw;
    DWORD color;
    float u, v;
    // Native deliberately leaves all vertex bytes untouched.
    // 0x000705c0
    SpawnScreenVertex();
    // Copies raw float words, including NaN payloads.
    // 0x000705d0
    SpawnScreenVertex(const CVector& position, float w, DWORD color, float tu, float tv);
};
class CSpawnScreen
{
public:
    // 0x00070630
    void SetText(const char* text);
    // 0x00070990
    CSpawnScreen(IDirect3DDevice9* device);
    // 0x000709d0
    ~CSpawnScreen();
    // 0x00070a30
    void Draw();
    BYTE unknown00[4];
    char* spawnText;
    CFonts* fonts;
    IDirect3DDevice9* device;
    IDirect3DTexture9* texture;
    IDirect3DStateBlock9* states[2];
    DWORD unknown1c;
    // 0x00070940
    void OnLostDevice();
    // 0x00070690
    void OnResetDevice();
};

static_assert(sizeof(CSpawnScreen) == 32);
static_assert(sizeof(SpawnScreenVertex) == 28);
static_assert(offsetof(CSpawnScreen, spawnText) == 4);

#endif // SPAWNSCREEN_H
