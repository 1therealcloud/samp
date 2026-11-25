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

    void DrawHealthBar(const CVector* position, float health, float armour, float distance);

    void BeginHealthBar();

    ~CPlayerTags();

    void EndHealthBar();

    void BeginLabel();

    void EndLabel();
    // Raw status byte and integer words are retained.

    void DrawLabel(const CVector* position, const char* text, DWORD color, float distance, BYTE drawStatus, int status);

    CPlayerTags(IDirect3DDevice9* device);
    IDirect3DDevice9* device;
    IDirect3DStateBlock9* state;
    ID3DXSprite* sprite;

    void OnLostDevice();

    void OnResetDevice();
};

extern const float g_playerTagDistanceHeight;

extern const char g_playerTagStatusBack[2];

extern const char g_playerTagStatusFront[2];

extern CMatrix g_nametagVehicleMatrix;

extern char g_nametagName[128];

extern CMatrix g_nametagLocalMatrix;

extern CVector g_nametagBone;

extern CVector g_nametagDrawPosition;

extern const Scripting::OpcodeInfo g_nametagLineOfSightCommand;

extern const char g_nametagNameFormat[8];

void ClientDrawLabels();

void ClientDrawPlayerTags();
// Border, background and fill: four vertices each, sharing native mutable storage.

extern PlayerTagVertex g_playerTagVertices[12];

extern const float g_playerTagFillScale;

extern const float g_playerTagInnerBottom;

extern const float g_playerTagHalfWidth;

extern const float g_playerTagOuterRight;

extern const float g_playerTagOuterBottom;

extern const float g_playerTagOuterTop;

extern const float g_playerTagOuterLeft;

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

    CLegacyPlayerTags(IDirect3DDevice9* pDevice);

    ~CLegacyPlayerTags();

    void Begin();

    void End();

    void Draw(D3DXMATRIX* pWorldTransform, char* pNameText, DWORD dwColor, float fHealth);

    void InitQuad(char c, float x, float y2, NameTextVertex_s* pVB);

    void DeleteDeviceObjects();

    void RestoreDeviceObjects();
};

// Contiguous health vertices192 bytes followed by metrics256. Signed character
// indexing reaches the preceding mutable vertices in the original binary.

extern BYTE g_legacyPlayerTagStorage[448];

extern const char g_legacyPlayerTagTextureName[];

extern const float g_legacyPlayerTagInv256;

extern const float g_legacyPlayerTagInv16;

#endif // PLAYERTAGS_H
