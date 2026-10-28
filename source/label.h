#ifndef SAMP_LABEL_H
#define SAMP_LABEL_H
#include "game/entity.h"
#include "game/scripting.h"
#include <d3dx9.h>
#pragma pack(push, 1)
class CLabel
{
public:
    // 0x0006b5f0
    CLabel(IDirect3DDevice9* device);
    // 0x0006b610
    ~CLabel();
    IDirect3DDevice9* device;
    ID3DXSprite* sprite;
    // 0x0006b630
    void OnLostDevice();
    // 0x0006b640
    void OnResetDevice();
    // 0x0006b650
    BOOL HasLineOfSight(float x, float y, float z);
    // 0x0006b6b0
    void Begin();
    // 0x0006b6c0
    void End();
    // 0x0006b6d0
    void Draw(const CVector* position, const char* text, DWORD color, BOOL shadow, BOOL status);
};
#pragma pack(pop)
// 0x0011c640
extern const Scripting::OpcodeInfo g_labelLineOfSightCommand;
#endif
