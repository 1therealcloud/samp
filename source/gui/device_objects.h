#ifndef SAMP_DEVICE_OBJECTS_H
#define SAMP_DEVICE_OBJECTS_H
#include "fontrender.h"
#include <d3dx9.h>
struct CVector;
class CDXUTDialog;
#pragma pack(push, 1)
// Observed allocation extents and constructor/lifecycle fields; full render interfaces remain open.
#include "label.h"
#include "playertags.h"
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
class CObjectMaterialText
{
public:
    // 0x00070350
    IDirect3DTexture9* Create(const char* text, const char* font, unsigned fontSize, int width, int height, D3DCOLOR color, D3DCOLOR background, BOOL bold, unsigned alignment);
    // 0x00070320
    CObjectMaterialText(IDirect3DDevice9* device);
    // 0x00070340
    ~CObjectMaterialText();
    IDirect3DDevice9* device;
    ID3DXSprite *first, *second;
    // 0x000702d0
    void OnLostDevice();
    // 0x00070300
    void OnResetDevice();
};
class CLicensePlate
{
public:
    // 0x0006d3f0
    CLicensePlate(IDirect3DDevice9* device);
    // 0x0006d420
    ~CLicensePlate();
    IDirect3DDevice9* device;
    ID3DXRenderToSurface* renderer;
    IDirect3DTexture9* texture;
    IDirect3DSurface9* surface;
    D3DDISPLAYMODE mode;
    IDirect3DTexture9* defaultTexture;
    // 0x0006d1f0
    void OnLostDevice();
    // 0x0006d240
    void OnResetDevice();
    // 0x0006d2c0
    IDirect3DTexture9* Create(const char* text);
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
    BYTE unknown00[8];
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
// Address interfaces for later UI/pool layers; never allocate these partial types.
#include "net/objectpool.h"
#include "net/textdrawpool.h"
#include "scoreboard.h"
#pragma pack(pop)
// 0x002aca2c
extern CFonts* g_deviceFonts;
// 0x0011c74c
extern const char g_spawnVersionText[];
typedef char NativeSpawnVertexExtent[(sizeof(SpawnScreenVertex) == 28) ? 1 : -1];
#endif
