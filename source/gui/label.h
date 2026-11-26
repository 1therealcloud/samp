#ifndef LABEL_H
#define LABEL_H

#include "game/entity.h"
#include "game/scripting.h"

#include <d3dx9.h>
class CLabel
{
public:
    CLabel(IDirect3DDevice9* device);

    ~CLabel();
    IDirect3DDevice9* device;
    ID3DXSprite* sprite;

    void OnLostDevice();

    void OnResetDevice();

    BOOL HasLineOfSight(float x, float y, float z);

    void Begin();

    void End();

    void Draw(const CVector* position, const char* text, DWORD color, BOOL shadow, BOOL status);
};

static_assert(sizeof(CLabel) == 8);

extern const Scripting::OpcodeInfo g_labelLineOfSightCommand;

#endif // LABEL_H
