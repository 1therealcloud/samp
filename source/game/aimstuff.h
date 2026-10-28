#ifndef SAMP_RECOVERED_AIMSTUFF_H
#define SAMP_RECOVERED_AIMSTUFF_H
#include <windows.h>
struct AimVector
{
    float x, y, z;
};
struct CameraAim
{
    AimVector front, source, sourceBeforeLookBehind, up;
};
typedef char CameraAimExtent[(sizeof(CameraAim) == 48) ? 1 : -1];
// 0x001419b0
extern CameraAim* g_aimInternal;
// 0x001419b4
extern float* g_aimInternalStats;
// 0x001419b8
extern float* g_aimInternalAspect;
// 0x001419bc
extern float* g_aimInternalZoom;
// 0x0015195c
extern BYTE* g_aimInternalCameraMode;
// 0x00182288
extern CameraAim g_aimLocal;
// 0x001822b8
extern CameraAim g_aimPlayers[210];
// 0x00181e60
extern float g_aimLocalZoom;
// 0x00184a18
extern float g_aimLocalAspect;
// 0x00181f40
extern float g_aimPlayerZoom[210];
// 0x00184a48
extern float g_aimPlayerAspect[210];
// 0x00181e68
extern BYTE g_aimPlayerCameraMode[210];
// 0x00184a1c
extern float g_aimLocalSkills[11];
// 0x00184d90
extern float g_aimPlayerSkills[210][11];
namespace AimStuff
{
// 0x00002d20
void __stdcall GetCameraExtZoomAndAspectRatio(float* zoom, float* aspect);

// 0x0009c240
void __stdcall UpdateCamera();
// 0x0009c260
void __stdcall ApplyLocalCamera();
// 0x0009c280
void __stdcall SetPlayerCamera(BYTE player, float zoom, float aspect);
// 0x0009c2a0
float __stdcall GetAspectRatio();
// 0x0009c2b0
float __stdcall GetCameraExtZoom();
// 0x0009c2d0
void __stdcall ApplyPlayerCamera(BYTE player);
// 0x0009c310
void __stdcall SetPlayerCameraMode(BYTE mode, BYTE player);
// 0x0009c330
BYTE __stdcall GetPlayerCameraMode(BYTE player);
// 0x0009c340
BYTE __stdcall GetCameraMode();
// 0x0009c350
void __stdcall Initialize();
// 0x0009c3c0
void __stdcall UpdateAim();
// 0x0009c3e0
void __stdcall ApplyLocalAim();
// 0x0009c400
CameraAim* __stdcall GetInternalAim();
// 0x0009c410
void __stdcall SetAim(int player, const CameraAim* aim);
// 0x0009c440
void __stdcall ApplyPlayerAim(int player);
// 0x0009c470
CameraAim* __stdcall GetPlayerAim(int player);
// 0x0009c490
void __stdcall SetPlayerSkill(int player, int skill, WORD value);
// Native checks only skill<11, including negative indices.
// 0x0009c4c0
void __stdcall SetLocalSkill(int skill, WORD value);
// 0x0009c5b0
void __stdcall UpdateLocalSkills();
// 0x0009c640
void __stdcall ApplyLocalSkills();
// 0x0009c710
void __stdcall ResetLocalSkills();
// 0x0009c7a0
void __stdcall ApplyPlayerSkills(int player);
} // namespace AimStuff
#endif

// Native writes only front.x/front.z and up.x/up.z; other aim words are untouched.
// 0x0009c4f0
void GameAimLookAtPartial(CameraAim* aim, float x, float y, float z);
