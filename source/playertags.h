#ifndef SAMP_PLAYERTAGS_H
#define SAMP_PLAYERTAGS_H
#include <d3dx9.h>
#include "game/entity.h"
#include "game/scripting.h"
struct PlayerTagVertex {float x,y,z,rhw;DWORD color;};
class CPlayerTags {
public:
    // samp.dll RVA: 0x0006cae0
    void DrawHealthBar(const CVector *position,float health,float armour,float distance);
    // samp.dll RVA: 0x0006d0f0
    void BeginHealthBar();
    // samp.dll RVA: 0x0006c760
    ~CPlayerTags();
    // samp.dll RVA: 0x0006c790
    void EndHealthBar();
    // samp.dll RVA: 0x0006c7c0
    void BeginLabel();
    // samp.dll RVA: 0x0006c7d0
    void EndLabel();
    // Raw status byte and integer words are retained.
    // samp.dll RVA: 0x0006c7e0
    void DrawLabel(const CVector *position,const char *text,DWORD color,float distance,BYTE drawStatus,int status);
    // samp.dll RVA: 0x0006c730
    CPlayerTags(IDirect3DDevice9 *device);
    IDirect3DDevice9 *device;IDirect3DStateBlock9 *state;ID3DXSprite *sprite;
    // samp.dll RVA: 0x0006d090
    void OnLostDevice();
    // samp.dll RVA: 0x0006d0c0
    void OnResetDevice();
};
// samp.dll RVA: 0x0011c670
extern const float g_playerTagDistanceHeight;
// samp.dll RVA: 0x00118388
extern const char g_playerTagStatusBack[2];
// samp.dll RVA: 0x00118370
extern const char g_playerTagStatusFront[2];
// samp.dll RVA: 0x0017e988
extern CMatrix g_nametagVehicleMatrix;
// samp.dll RVA: 0x0017eb08
extern char g_nametagName[128];
// samp.dll RVA: 0x0017eb88
extern CMatrix g_nametagLocalMatrix;
// samp.dll RVA: 0x0017ebcc
extern CVector g_nametagBone;
// samp.dll RVA: 0x0017ebf4
extern CVector g_nametagDrawPosition;
// samp.dll RVA: 0x0011c7a4
extern const Scripting::OpcodeInfo g_nametagLineOfSightCommand;
// samp.dll RVA: 0x0011c7b8
extern const char g_nametagNameFormat[8];
// samp.dll RVA: 0x00074dc0
void ClientDrawLabels();
// samp.dll RVA: 0x00073cb0
void ClientDrawPlayerTags();
// Border, background and fill: four vertices each, sharing native mutable storage.
// samp.dll RVA: 0x00140878
extern PlayerTagVertex g_playerTagVertices[12];
// samp.dll RVA: 0x0011c674
extern const float g_playerTagFillScale;
// samp.dll RVA: 0x0011c678
extern const float g_playerTagInnerBottom;
// samp.dll RVA: 0x0011c67c
extern const float g_playerTagHalfWidth;
// samp.dll RVA: 0x0011c680
extern const float g_playerTagOuterRight;
// samp.dll RVA: 0x0011c684
extern const float g_playerTagOuterBottom;
// samp.dll RVA: 0x0011c688
extern const float g_playerTagOuterTop;
// samp.dll RVA: 0x00117874
extern const float g_playerTagOuterLeft;
// samp.dll RVA: 0x0011797c
extern const float g_playerTagArmourOffset;
#endif
