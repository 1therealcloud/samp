#ifndef SAMP_OBJECTMATERIALTEXT_H
#define SAMP_OBJECTMATERIALTEXT_H
#include "fontrender.h"
#include <d3dx9.h>
struct CVector;
#pragma pack(push, 1)
class CObjectMaterialText
{
public:
    // 0x00070350
    IDirect3DTexture9* Create(const char* text, const char* font, unsigned fontSize, int width, int height, D3DCOLOR color, D3DCOLOR background, BOOL bold, BYTE alignment);
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
#pragma pack(pop)
#endif
