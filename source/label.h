#ifndef SAMP_LABEL_H
#define SAMP_LABEL_H
#include <d3dx9.h>
#include "game/entity.h"
#include "game/scripting.h"
#pragma pack(push,1)
class CLabel {
public:
    // samp.dll RVA: 0x0006b5f0
    CLabel(IDirect3DDevice9 *device);
    // samp.dll RVA: 0x0006b610
    ~CLabel();
    IDirect3DDevice9 *device;ID3DXSprite *sprite;
    // samp.dll RVA: 0x0006b630
    void OnLostDevice();
    // samp.dll RVA: 0x0006b640
    void OnResetDevice();
    // samp.dll RVA: 0x0006b650
    BOOL HasLineOfSight(float x,float y,float z);
    // samp.dll RVA: 0x0006b6b0
    void Begin();
    // samp.dll RVA: 0x0006b6c0
    void End();
    // samp.dll RVA: 0x0006b6d0
    void Draw(const CVector *position,const char *text,DWORD color,BOOL shadow,BOOL status);
};
#pragma pack(pop)
// samp.dll RVA: 0x0011c640
extern const Scripting::OpcodeInfo g_labelLineOfSightCommand;
#endif
