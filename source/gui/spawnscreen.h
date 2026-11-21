#ifndef SPAWNSCREEN_H
#define SPAWNSCREEN_H

#include "render/fontrender.h"

#include <d3dx9.h>

#include <static_assert.h>

struct CVector;
#pragma pack(push, 1)
struct SpawnScreenVertex
{
    float x, y, z, rhw;
    DWORD color;
    float u, v;
    // Native deliberately leaves all vertex bytes untouched.
    // R5 address to verify
    SpawnScreenVertex();
    // Copies raw float words, including NaN payloads.
    // R5 address to verify
    SpawnScreenVertex(const CVector& position, float w, DWORD color, float tu, float tv);
};
class CSpawnScreen
{
public:
    // 0x00070b90
    void SetText(const char* text);
    // 0x00070ef0
    CSpawnScreen(IDirect3DDevice9* device);
    // 0x00070f30
    ~CSpawnScreen();
    // 0x00070f90
    void Draw();
    BYTE unknown00[8];
    CFonts* fonts;
    IDirect3DDevice9* device;
    IDirect3DTexture9* texture;
    IDirect3DStateBlock9* states[2];
    DWORD unknown1c;
    // 0x00070ea0
    void OnLostDevice();
    // 0x00070bf0
    void OnResetDevice();
};
#pragma pack(pop)
// R5 address to verify
extern const char g_spawnVersionText[];
STATIC_ASSERT(sizeof(SpawnScreenVertex) == 28);

#endif // SPAWNSCREEN_H
