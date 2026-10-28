#include "game/aimstuff.h"
#include "game/util.h"
#include <d3dx9.h>
#include <string.h>
// samp.dll RVA: 0x0011e32c
static const float g_aimCameraZoomRange = 35.0f;
// samp.dll RVA: 0x001419b0
CameraAim* g_aimInternal = (CameraAim*)0xb6f32c;
// samp.dll RVA: 0x001419b4
float* g_aimInternalStats = (float*)0xb79380;
// samp.dll RVA: 0x001419b8
float* g_aimInternalAspect = (float*)0xc3efa4;
// samp.dll RVA: 0x001419bc
float* g_aimInternalZoom = (float*)0xb6f250;
// samp.dll RVA: 0x0015195c
BYTE* g_aimInternalCameraMode = (BYTE*)0xb6f1a8;
// samp.dll RVA: 0x00182288
CameraAim g_aimLocal;
// samp.dll RVA: 0x001822b8
CameraAim g_aimPlayers[210];
// samp.dll RVA: 0x00181e60
float g_aimLocalZoom;
// samp.dll RVA: 0x00184a18
float g_aimLocalAspect;
// samp.dll RVA: 0x00181f40
float g_aimPlayerZoom[210];
// samp.dll RVA: 0x00184a48
float g_aimPlayerAspect[210];
// samp.dll RVA: 0x00181e68
BYTE g_aimPlayerCameraMode[210];
// samp.dll RVA: 0x00184a1c
float g_aimLocalSkills[11];
// samp.dll RVA: 0x00184d90
float g_aimPlayerSkills[210][11];
// samp.dll RVA: 0x0009c240
void __stdcall AimStuff::UpdateCamera()
{
    memcpy(&g_aimLocalZoom, g_aimInternalZoom, 4);
    memcpy(&g_aimLocalAspect, g_aimInternalAspect, 4);
}
// samp.dll RVA: 0x0009c260
void __stdcall AimStuff::ApplyLocalCamera()
{
    memcpy(g_aimInternalZoom, &g_aimLocalZoom, 4);
    memcpy(g_aimInternalAspect, &g_aimLocalAspect, 4);
}
// samp.dll RVA: 0x0009c280
void __stdcall AimStuff::SetPlayerCamera(BYTE player, float zoom, float aspect)
{
    memcpy(&g_aimPlayerZoom[player], &zoom, 4);
    memcpy(&g_aimPlayerAspect[player], &aspect, 4);
}
// samp.dll RVA: 0x0009c2a0
float __stdcall AimStuff::GetAspectRatio()
{
    return *g_aimInternalAspect - 1.0f;
}
// samp.dll RVA: 0x0009c2b0
float __stdcall AimStuff::GetCameraExtZoom()
{
    return (*g_aimInternalZoom - 35.0f) / 35.0f;
}
// samp.dll RVA: 0x0009c2d0
void __stdcall AimStuff::ApplyPlayerCamera(BYTE player)
{
    // clang-format off
    // VC7 reassociates zoom*35+35 into (zoom+1)*35 under /O2.
    // Preserve native x87 order, including the caller's precision control.
    __asm {
        movzx eax,player
        mov ecx,g_aimInternalZoom
        fld dword ptr g_aimPlayerZoom[eax*4]
        fmul g_aimCameraZoomRange
        fadd g_aimCameraZoomRange
        fstp dword ptr [ecx]
    }
    // clang-format on
    *g_aimInternalAspect = g_aimPlayerAspect[player] + 1.0f;
}
// samp.dll RVA: 0x0009c310
void __stdcall AimStuff::SetPlayerCameraMode(BYTE mode, BYTE player)
{
    g_aimPlayerCameraMode[player] = mode;
}
// samp.dll RVA: 0x0009c330
BYTE __stdcall AimStuff::GetPlayerCameraMode(BYTE player)
{
    return g_aimPlayerCameraMode[player];
}
// samp.dll RVA: 0x0009c340
BYTE __stdcall AimStuff::GetCameraMode()
{
    return *g_aimInternalCameraMode;
}
// samp.dll RVA: 0x0009c350
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
// samp.dll RVA: 0x0009c3c0
void __stdcall AimStuff::UpdateAim()
{
    memcpy(&g_aimLocal, g_aimInternal, sizeof(CameraAim));
}
// samp.dll RVA: 0x0009c3e0
void __stdcall AimStuff::ApplyLocalAim()
{
    memcpy(g_aimInternal, &g_aimLocal, sizeof(CameraAim));
}
// samp.dll RVA: 0x0009c400
CameraAim* __stdcall AimStuff::GetInternalAim()
{
    return g_aimInternal;
}
// samp.dll RVA: 0x0009c410
void __stdcall AimStuff::SetAim(int player, const CameraAim* aim)
{
    memcpy(&g_aimPlayers[player], aim, sizeof(CameraAim));
}
// samp.dll RVA: 0x0009c440
void __stdcall AimStuff::ApplyPlayerAim(int player)
{
    memcpy(g_aimInternal, &g_aimPlayers[player], sizeof(CameraAim));
}
// samp.dll RVA: 0x0009c470
CameraAim* __stdcall AimStuff::GetPlayerAim(int player)
{
    return &g_aimPlayers[player];
}
// samp.dll RVA: 0x0009c490
void __stdcall AimStuff::SetPlayerSkill(int player, int skill, WORD value)
{
    if (skill < 11 && player < 210)
        g_aimPlayerSkills[player][skill] = (float)value;
}
// samp.dll RVA: 0x0009c4c0
void __stdcall AimStuff::SetLocalSkill(int skill, WORD value)
{
    if (skill < 11)
    {
        float converted = (float)value;
        g_aimLocalSkills[skill] = converted;
        *(float*)((BYTE*)g_aimInternalStats + 0x114 + skill * 4) = converted;
    }
}
// samp.dll RVA: 0x0009c5b0
void __stdcall AimStuff::UpdateLocalSkills()
{
    memcpy(g_aimLocalSkills, (BYTE*)g_aimInternalStats + 0x114, sizeof(g_aimLocalSkills));
}
// samp.dll RVA: 0x0009c640
void __stdcall AimStuff::ApplyLocalSkills()
{
    memcpy((BYTE*)g_aimInternalStats + 0x114, g_aimLocalSkills, sizeof(g_aimLocalSkills));
}
// samp.dll RVA: 0x0009c710
void __stdcall AimStuff::ResetLocalSkills()
{
    for (int i = 0; i < 11; i++)
        *(float*)((BYTE*)g_aimInternalStats + 0x114 + i * 4) = 999.0f;
    UpdateLocalSkills();
}
// samp.dll RVA: 0x0009c7a0
void __stdcall AimStuff::ApplyPlayerSkills(int player)
{
    memcpy((BYTE*)g_aimInternalStats + 0x114, g_aimPlayerSkills[player], sizeof(g_aimLocalSkills));
}

// samp.dll RVA: 0x00002d20
void __stdcall AimStuff::GetCameraExtZoomAndAspectRatio(float* zoom, float* aspect)
{
    *zoom = GetCameraExtZoom();
    *aspect = GetAspectRatio();
}

// samp.dll RVA: 0x0009c4f0
void GameAimLookAtPartial(CameraAim* aim, float x, float y, float z)
{
    D3DXVECTOR3 eye, target(x, y, z), vertical(0, 0, 1);
    memcpy(&eye, &aim->source, 12);
    D3DXMATRIX matrix;
    D3DXMatrixLookAtRH(&matrix, &eye, &target, &vertical);
    CVector direction, up;
    // clang-format off
    __asm {
        lea eax,matrix
        fld dword ptr [eax+8]
        fchs
        fstp direction.x
        fld dword ptr [eax+0x18]
        fchs
        fstp direction.y
        fld dword ptr [eax+0x28]
        fchs
        fstp direction.z
    }
    // clang-format on
    GameBuildAimUpVector(&direction, &up);
    // Preserve the four observed DWORD publications, including shifted Y sources.
    memcpy(&aim->front.x, &direction.y, 4);
    memcpy(&aim->front.z, &direction.z, 4);
    memcpy(&aim->up.x, &up.y, 4);
    memcpy(&aim->up.z, &up.z, 4);
}
