#ifndef LABEL_H
#define LABEL_H

#include "game/entity.h"
#include "game/scripting.h"

#include <d3dx9.h>
#pragma pack(push, 1)
class CLabel
{
public:
    // 0x0006bbb0
    CLabel(IDirect3DDevice9* device);
    // 0x0006bbd0
    ~CLabel();
    IDirect3DDevice9* device;
    ID3DXSprite* sprite;
    // 0x0006bbf0
    void OnLostDevice();
    // 0x0006bc00
    void OnResetDevice();
    // 0x0006bc10
    BOOL HasLineOfSight(float x, float y, float z);
    // 0x0006bc70
    void Begin();
    // 0x0006bc80
    void End();
    // 0x0006bc90
    void Draw(const CVector* position, const char* text, DWORD color, BOOL shadow, BOOL status);
};
#pragma pack(pop)
// 0x000ea618
extern const Scripting::OpcodeInfo g_labelLineOfSightCommand;

#endif // LABEL_H
