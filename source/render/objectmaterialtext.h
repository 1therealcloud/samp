#ifndef OBJECTMATERIALTEXT_H
#define OBJECTMATERIALTEXT_H

#include "render/fontrender.h"

#include <d3dx9.h>
struct CVector;
#pragma pack(push, 1)
class CObjectMaterialText
{
public:
    // 0x000708b0
    IDirect3DTexture9* Create(const char* text, const char* font, unsigned fontSize, int width, int height, D3DCOLOR color, D3DCOLOR background, BOOL bold, BYTE alignment);
    // 0x00070880
    CObjectMaterialText(IDirect3DDevice9* device);
    // 0x000708a0
    ~CObjectMaterialText();
    IDirect3DDevice9* device;
    ID3DXSprite *first, *second;
    // 0x00070830
    void OnLostDevice();
    // 0x00070860
    void OnResetDevice();
};
#pragma pack(pop)

#endif // OBJECTMATERIALTEXT_H
