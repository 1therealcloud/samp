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

    SpawnScreenVertex();
    // Copies raw float words, including NaN payloads.

    SpawnScreenVertex(const CVector& position, float w, DWORD color, float tu, float tv);
};
class CSpawnScreen
{
public:

    void SetText(const char* text);

    CSpawnScreen(IDirect3DDevice9* device);

    ~CSpawnScreen();

    void Draw();
    BYTE unknown00[4];
    char* spawnText;
    CFonts* fonts;
    IDirect3DDevice9* device;
    IDirect3DTexture9* texture;
    IDirect3DStateBlock9* states[2];
    DWORD unknown1c;

    void OnLostDevice();

    void OnResetDevice();
};

static_assert(sizeof(CSpawnScreen) == 32);
static_assert(sizeof(SpawnScreenVertex) == 28);
static_assert(offsetof(CSpawnScreen, spawnText) == 4);

#endif // SPAWNSCREEN_H
