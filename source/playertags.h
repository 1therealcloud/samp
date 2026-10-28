#ifndef SAMP_PLAYERTAGS_H
#define SAMP_PLAYERTAGS_H
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
    // 0x0006cae0
    void DrawHealthBar(const CVector* position, float health, float armour, float distance);
    // 0x0006d0f0
    void BeginHealthBar();
    // 0x0006c760
    ~CPlayerTags();
    // 0x0006c790
    void EndHealthBar();
    // 0x0006c7c0
    void BeginLabel();
    // 0x0006c7d0
    void EndLabel();
    // Raw status byte and integer words are retained.
    // 0x0006c7e0
    void DrawLabel(const CVector* position, const char* text, DWORD color, float distance, BYTE drawStatus, int status);
    // 0x0006c730
    CPlayerTags(IDirect3DDevice9* device);
    IDirect3DDevice9* device;
    IDirect3DStateBlock9* state;
    ID3DXSprite* sprite;
    // 0x0006d090
    void OnLostDevice();
    // 0x0006d0c0
    void OnResetDevice();
};
// 0x0011c670
extern const float g_playerTagDistanceHeight;
// 0x00118388
extern const char g_playerTagStatusBack[2];
// 0x00118370
extern const char g_playerTagStatusFront[2];
// 0x0017e988
extern CMatrix g_nametagVehicleMatrix;
// 0x0017eb08
extern char g_nametagName[128];
// 0x0017eb88
extern CMatrix g_nametagLocalMatrix;
// 0x0017ebcc
extern CVector g_nametagBone;
// 0x0017ebf4
extern CVector g_nametagDrawPosition;
// 0x0011c7a4
extern const Scripting::OpcodeInfo g_nametagLineOfSightCommand;
// 0x0011c7b8
extern const char g_nametagNameFormat[8];
// 0x00074dc0
void ClientDrawLabels();
// 0x00073cb0
void ClientDrawPlayerTags();
// Border, background and fill: four vertices each, sharing native mutable storage.
// 0x00140878
extern PlayerTagVertex g_playerTagVertices[12];
// 0x0011c674
extern const float g_playerTagFillScale;
// 0x0011c678
extern const float g_playerTagInnerBottom;
// 0x0011c67c
extern const float g_playerTagHalfWidth;
// 0x0011c680
extern const float g_playerTagOuterRight;
// 0x0011c684
extern const float g_playerTagOuterBottom;
// 0x0011c688
extern const float g_playerTagOuterTop;
// 0x00117874
extern const float g_playerTagOuterLeft;
// 0x0011797c
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
        // 0x0006d8b0
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
    // 0x0006d8e0
    CLegacyPlayerTags(IDirect3DDevice9* pDevice);
    // 0x0006d910
    ~CLegacyPlayerTags();

    // 0x0006df10
    void Begin();
    // 0x0006df50
    void End();
    // 0x0006df80
    void Draw(D3DXMATRIX* pWorldTransform, char* pNameText, DWORD dwColor, float fHealth);
    // 0x0006d950
    void InitQuad(char c, float x, float y2, NameTextVertex_s* pVB);
    // 0x0006da80
    void DeleteDeviceObjects();
    // 0x0006dad0
    void RestoreDeviceObjects();
};

// Contiguous health vertices192 bytes followed by metrics256. Signed character
// indexing reaches the preceding mutable vertices in the original binary.
// 0x00140998
extern BYTE g_legacyPlayerTagStorage[448];
// 0x0011c6bc
extern const char g_legacyPlayerTagTextureName[];
// 0x0011c6b4
extern const float g_legacyPlayerTagInv256;
// 0x0011c6b8
extern const float g_legacyPlayerTagInv16;
#endif
