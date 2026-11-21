#ifndef PLAYERTAGS_H
#define PLAYERTAGS_H

#include "game/entity.h"
#include "game/scripting.h"

#include <d3dx9.h>
struct PlayerTagVertex
{
    float x, y, z, rhw;
    DWORD color;
};
class CPlayerTags
{
public:
    // 0x0006d0a0
    void DrawHealthBar(const CVector* position, float health, float armour, float distance);
    // 0x0006d6b0
    void BeginHealthBar();
    // 0x0006cd20
    ~CPlayerTags();
    // 0x0006cd50
    void EndHealthBar();
    // 0x0006cd80
    void BeginLabel();
    // 0x0006cd90
    void EndLabel();
    // Raw status byte and integer words are retained.
    // 0x0006cda0
    void DrawLabel(const CVector* position, const char* text, DWORD color, float distance, BYTE drawStatus, int status);
    // 0x0006ccf0
    CPlayerTags(IDirect3DDevice9* device);
    IDirect3DDevice9* device;
    IDirect3DStateBlock9* state;
    ID3DXSprite* sprite;
    // 0x0006d650
    void OnLostDevice();
    // 0x0006d680
    void OnResetDevice();
};
// 0x000ea648
extern const float g_playerTagDistanceHeight;
// 0x000e6304
extern const char g_playerTagStatusBack[2];
// 0x000e62ec
extern const char g_playerTagStatusFront[2];
// R5 address to verify
extern CMatrix g_nametagVehicleMatrix;
// R5 address to verify
extern char g_nametagName[128];
// R5 address to verify
extern CMatrix g_nametagLocalMatrix;
// R5 address to verify
extern CVector g_nametagBone;
// R5 address to verify
extern CVector g_nametagDrawPosition;
// R5 address to verify
extern const Scripting::OpcodeInfo g_nametagLineOfSightCommand;
// 0x000ea790
extern const char g_nametagNameFormat[8];
// 0x00075330
void ClientDrawLabels();
// 0x00074210
void ClientDrawPlayerTags();
// Border, background and fill: four vertices each, sharing native mutable storage.
// R5 address to verify
extern PlayerTagVertex g_playerTagVertices[12];
// 0x000ea64c
extern const float g_playerTagFillScale;
// 0x000ea650
extern const float g_playerTagInnerBottom;
// 0x000ea654
extern const float g_playerTagHalfWidth;
// 0x000ea658
extern const float g_playerTagOuterRight;
// 0x000ea65c
extern const float g_playerTagOuterBottom;
// 0x000ea660
extern const float g_playerTagOuterTop;
// 0x000e5804
extern const float g_playerTagOuterLeft;
// 0x000e590c
extern const float g_playerTagArmourOffset;
#endif

#ifndef SAMP_LEGACY_PLAYER_TAGS_H
#define SAMP_LEGACY_PLAYER_TAGS_H
// Separate original legacy renderer; descriptive name distinguishes current tags.
class CLegacyPlayerTags
{
private:
    struct NameTextVertex_s
    {
        float x, y, z, tu, tv;
        // R5 address to verify
        void Initialise(float fX, float fY, float fTu, float fTv);
    };

    IDirect3DDevice9* m_pDevice;
    IDirect3DTexture9* m_pTexture;
    IDirect3DVertexBuffer9* m_pVB;
    IDirect3DStateBlock9* m_pOldStates;
    IDirect3DStateBlock9* m_pNewStates;

    float m_fCharWidth;
    float m_fCharHeight;
    float m_fScale;

public:
    // R5 address to verify
    CLegacyPlayerTags(IDirect3DDevice9* pDevice);
    // 0x0006ded0
    ~CLegacyPlayerTags();

    // 0x0006e4d0
    void Begin();
    // 0x0006e510
    void End();
    // R5 address to verify
    void Draw(D3DXMATRIX* pWorldTransform, char* pNameText, DWORD dwColor, float fHealth);
    // 0x0006df10
    void InitQuad(char c, float x, float y2, NameTextVertex_s* pVB);
    // 0x0006e040
    void DeleteDeviceObjects();
    // 0x0006e090
    void RestoreDeviceObjects();
};

// Contiguous health vertices192 bytes followed by metrics256. Signed character
// indexing reaches the preceding mutable vertices in the original binary.
// R5 address to verify
extern BYTE g_legacyPlayerTagStorage[448];
// 0x000ea694
extern const char g_legacyPlayerTagTextureName[];
// 0x000ea68c
extern const float g_legacyPlayerTagInv256;
// 0x000ea690
extern const float g_legacyPlayerTagInv16;

#endif // PLAYERTAGS_H
