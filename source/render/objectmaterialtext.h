#ifndef OBJECTMATERIALTEXT_H
#define OBJECTMATERIALTEXT_H

#include "render/fontrender.h"

#include <d3dx9.h>
struct CVector;
class CObjectMaterialText
{
public:

    IDirect3DTexture9* Create(const char* text, const char* font, unsigned fontSize, int width, int height, D3DCOLOR color, D3DCOLOR background, BOOL bold, BYTE alignment);

    CObjectMaterialText(IDirect3DDevice9* device);

    ~CObjectMaterialText();
    IDirect3DDevice9* device;
    ID3DXSprite *first, *second;

    void OnLostDevice();

    void OnResetDevice();
};

static_assert(sizeof(CObjectMaterialText) == 12);

#endif // OBJECTMATERIALTEXT_H
