#ifndef SAMP_PLAYERTAGS_H
#define SAMP_PLAYERTAGS_H
#include <d3dx9.h>
#include "game/entity.h"
#include "game/scripting.h"
class CPlayerTags {
public:
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
#endif
