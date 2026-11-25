#include "game/aimstuff.h"

#include "game/util.h"

#include <d3dx9.h>

#include <string.h>

static const float g_aimCameraZoomRange = 35.0f;

static const float g_aimCameraAspectBase = 1.0f;

CameraAim* g_aimInternal = (CameraAim*)0xb6f32c;

float* g_aimInternalStats = (float*)0xb79380;

float* g_aimInternalAspect = (float*)0xc3efa4;

float* g_aimInternalZoom = (float*)0xb6f250;

BYTE* g_aimInternalCameraMode = (BYTE*)0xb6f1a8;

CameraAim g_aimLocal;

CameraAim g_aimPlayers[210];

float g_aimLocalZoom;

float g_aimLocalAspect;

float g_aimPlayerZoom[210];

float g_aimPlayerAspect[210];

BYTE g_aimPlayerCameraMode[210];

float g_aimLocalSkills[11];

float g_aimPlayerSkills[210][11];

void __stdcall AimStuff::UpdateCamera()
{
    memcpy(&g_aimLocalZoom, g_aimInternalZoom, 4);
    memcpy(&g_aimLocalAspect, g_aimInternalAspect, 4);
}

void __stdcall AimStuff::ApplyLocalCamera()
{
    *g_aimInternalZoom = g_aimLocalZoom;
    *g_aimInternalAspect = g_aimLocalAspect;
}

void __stdcall AimStuff::SetPlayerCamera(BYTE player, float zoom, float aspect)
{
    memcpy(&g_aimPlayerZoom[player], &zoom, 4);
    memcpy(&g_aimPlayerAspect[player], &aspect, 4);
}

float __stdcall AimStuff::GetAspectRatio()
{
    return *g_aimInternalAspect - 1.0f;
}

float __stdcall AimStuff::GetCameraExtZoom()
{
    return (*g_aimInternalZoom - 35.0f) / 35.0f;
}

void __stdcall AimStuff::ApplyPlayerCamera(BYTE player)
{
    const volatile double zoom = (double)g_aimPlayerZoom[player] * g_aimCameraZoomRange;
    *g_aimInternalZoom = (float)(zoom + g_aimCameraZoomRange);
    *g_aimInternalAspect = g_aimPlayerAspect[player] + g_aimCameraAspectBase;
}

void __stdcall AimStuff::SetPlayerCameraMode(BYTE mode, BYTE player)
{
    g_aimPlayerCameraMode[player] = mode;
}

BYTE __stdcall AimStuff::GetPlayerCameraMode(BYTE player)
{
    return g_aimPlayerCameraMode[player];
}

BYTE __stdcall AimStuff::GetCameraMode()
{
    return *g_aimInternalCameraMode;
}

void __stdcall AimStuff::Initialize()
{
    memset(&g_aimLocal, 0, sizeof(g_aimLocal));
    memset(g_aimPlayers, 0, sizeof(g_aimPlayers));
    memset(g_aimPlayerCameraMode, 4, sizeof(g_aimPlayerCameraMode));
    for (int i = 0; i < 210; i++)
    {
        for (int skill = 0; skill < 11; skill++)
            g_aimPlayerSkills[i][skill] = 999.0f;
    }
    for (int p = 0; p < 210; p++)
        g_aimPlayerAspect[p] = 0.333333f;
    for (int n = 0; n < 210; n++)
        g_aimPlayerZoom[n] = 1.0f;
}

void __stdcall AimStuff::UpdateAim()
{
    memcpy(&g_aimLocal, g_aimInternal, sizeof(CameraAim));
}

void __stdcall AimStuff::ApplyLocalAim()
{
    memcpy(g_aimInternal, &g_aimLocal, sizeof(CameraAim));
}

CameraAim* __stdcall AimStuff::GetInternalAim()
{
    return g_aimInternal;
}

void __stdcall AimStuff::SetAim(int player, const CameraAim* aim)
{
    memcpy(&g_aimPlayers[player], aim, sizeof(CameraAim));
}

void __stdcall AimStuff::ApplyPlayerAim(int player)
{
    memcpy(g_aimInternal, &g_aimPlayers[player], sizeof(CameraAim));
}

CameraAim* __stdcall AimStuff::GetPlayerAim(int player)
{
    return &g_aimPlayers[player];
}

void __stdcall AimStuff::SetPlayerSkill(int player, int skill, WORD value)
{
    if (skill < 11 && player < 210)
        g_aimPlayerSkills[player][skill] = (float)value;
}

void __stdcall AimStuff::SetLocalSkill(int skill, WORD value)
{
    if (skill < 11)
    {
        float converted = (float)value;
        g_aimLocalSkills[skill] = converted;
        *(float*)((BYTE*)g_aimInternalStats + 0x114 + skill * 4) = converted;
    }
}

void __stdcall AimStuff::UpdateLocalSkills()
{
    g_aimLocalSkills[0] = g_aimInternalStats[69];
    g_aimLocalSkills[1] = g_aimInternalStats[70];
    g_aimLocalSkills[2] = g_aimInternalStats[71];
    g_aimLocalSkills[3] = g_aimInternalStats[72];
    g_aimLocalSkills[4] = g_aimInternalStats[73];
    g_aimLocalSkills[5] = g_aimInternalStats[74];
    g_aimLocalSkills[6] = g_aimInternalStats[75];
    g_aimLocalSkills[7] = g_aimInternalStats[76];
    g_aimLocalSkills[8] = g_aimInternalStats[77];
    g_aimLocalSkills[9] = g_aimInternalStats[78];
    g_aimLocalSkills[10] = g_aimInternalStats[79];
}

void __stdcall AimStuff::ApplyLocalSkills()
{
    g_aimInternalStats[69] = g_aimLocalSkills[0];
    g_aimInternalStats[70] = g_aimLocalSkills[1];
    g_aimInternalStats[71] = g_aimLocalSkills[2];
    g_aimInternalStats[72] = g_aimLocalSkills[3];
    g_aimInternalStats[73] = g_aimLocalSkills[4];
    g_aimInternalStats[74] = g_aimLocalSkills[5];
    g_aimInternalStats[75] = g_aimLocalSkills[6];
    g_aimInternalStats[76] = g_aimLocalSkills[7];
    g_aimInternalStats[77] = g_aimLocalSkills[8];
    g_aimInternalStats[78] = g_aimLocalSkills[9];
    g_aimInternalStats[79] = g_aimLocalSkills[10];
}

void __stdcall AimStuff::ResetLocalSkills()
{
    g_aimInternalStats[69] = 999.0f;
    g_aimInternalStats[70] = 999.0f;
    g_aimInternalStats[71] = 999.0f;
    g_aimInternalStats[72] = 999.0f;
    g_aimInternalStats[73] = 999.0f;
    g_aimInternalStats[74] = 999.0f;
    g_aimInternalStats[75] = 999.0f;
    g_aimInternalStats[76] = 999.0f;
    g_aimInternalStats[77] = 999.0f;
    g_aimInternalStats[78] = 999.0f;
    g_aimInternalStats[79] = 999.0f;
    UpdateLocalSkills();
}

void __stdcall AimStuff::ApplyPlayerSkills(int player)
{
    g_aimInternalStats[69] = g_aimPlayerSkills[player][0];
    g_aimInternalStats[70] = g_aimPlayerSkills[player][1];
    g_aimInternalStats[71] = g_aimPlayerSkills[player][2];
    g_aimInternalStats[72] = g_aimPlayerSkills[player][3];
    g_aimInternalStats[73] = g_aimPlayerSkills[player][4];
    g_aimInternalStats[74] = g_aimPlayerSkills[player][5];
    g_aimInternalStats[75] = g_aimPlayerSkills[player][6];
    g_aimInternalStats[76] = g_aimPlayerSkills[player][7];
    g_aimInternalStats[77] = g_aimPlayerSkills[player][8];
    g_aimInternalStats[78] = g_aimPlayerSkills[player][9];
    g_aimInternalStats[79] = g_aimPlayerSkills[player][10];
}

void __stdcall AimStuff::GetCameraExtZoomAndAspectRatio(float* zoom, float* aspect)
{
    *zoom = GetCameraExtZoom();
    *aspect = GetAspectRatio();
}

void GameAimLookAtPartial(CameraAim* aim, float x, float y, float z)
{
    CVector direction;
    D3DXVECTOR3 eye(aim->source.x, aim->source.y, aim->source.z);
    D3DXVECTOR3 target(x, y, z);
    D3DXVECTOR3 vertical(0, 0, 1);
    D3DXMATRIX matrix;
    D3DXMatrixLookAtRH(&matrix, &eye, &target, &vertical);
    CVector up;
    direction.x = -matrix._13;
    direction.y = -matrix._23;
    direction.z = -matrix._33;
    GameBuildAimUpVector(&direction, &up);
    // Preserve the four observed DWORD publications, including shifted Y sources.
    memcpy(&aim->front.x, &direction.y, 4);
    memcpy(&aim->front.z, &direction.z, 4);
    memcpy(&aim->up.x, &up.y, 4);
    memcpy(&aim->up.z, &up.z, 4);
}
