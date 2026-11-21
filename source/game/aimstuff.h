#ifndef SAMP_RECOVERED_AIMSTUFF_H
#define SAMP_RECOVERED_AIMSTUFF_H
#include <windows.h>

#include <static_assert.h>

struct AimVector
{
    float x, y, z;
};
struct CameraAim
{
    AimVector front, source, sourceBeforeLookBehind, up;
};
STATIC_ASSERT(sizeof(CameraAim) == 48);
// 0x001039c8
extern CameraAim* g_aimInternal;
// 0x001039cc
extern float* g_aimInternalStats;
// 0x001039d0
extern float* g_aimInternalAspect;
// 0x001039d4
extern float* g_aimInternalZoom;
// 0x00113974
extern BYTE* g_aimInternalCameraMode;
// 0x001443f8
extern CameraAim g_aimLocal;
// 0x00144428
extern CameraAim g_aimPlayers[210];
// R5 address to verify
extern float g_aimLocalZoom;
// R5 address to verify
extern float g_aimLocalAspect;
// 0x001440b0
extern float g_aimPlayerZoom[210];
// 0x00146bb8
extern float g_aimPlayerAspect[210];
// 0x00143fd8
extern BYTE g_aimPlayerCameraMode[210];
// 0x00146b8c
extern float g_aimLocalSkills[11];
// 0x00146f00
extern float g_aimPlayerSkills[210][11];
namespace AimStuff
{
// R5 address to verify
void __stdcall GetCameraExtZoomAndAspectRatio(float* zoom, float* aspect);

// 0x0009c7c0
void __stdcall UpdateCamera();
// 0x0009c7e0
void __stdcall ApplyLocalCamera();
// 0x0009c800
void __stdcall SetPlayerCamera(BYTE player, float zoom, float aspect);
// 0x0009c820
float __stdcall GetAspectRatio();
// 0x0009c830
float __stdcall GetCameraExtZoom();
// 0x0009c850
void __stdcall ApplyPlayerCamera(BYTE player);
// 0x0009c890
void __stdcall SetPlayerCameraMode(BYTE mode, BYTE player);
// 0x0009c8b0
BYTE __stdcall GetPlayerCameraMode(BYTE player);
// 0x0009c8c0
BYTE __stdcall GetCameraMode();
// 0x0009c8d0
void __stdcall Initialize();
// 0x0009c940
void __stdcall UpdateAim();
// 0x0009c960
void __stdcall ApplyLocalAim();
// 0x0009c980
CameraAim* __stdcall GetInternalAim();
// 0x0009c990
void __stdcall SetAim(int player, const CameraAim* aim);
// 0x0009c9c0
void __stdcall ApplyPlayerAim(int player);
// 0x0009c9f0
CameraAim* __stdcall GetPlayerAim(int player);
// 0x0009ca10
void __stdcall SetPlayerSkill(int player, int skill, WORD value);
// Native checks only skill<11, including negative indices.
// 0x0009ca40
void __stdcall SetLocalSkill(int skill, WORD value);
// 0x0009cb30
void __stdcall UpdateLocalSkills();
// 0x0009cbc0
void __stdcall ApplyLocalSkills();
// 0x0009cc90
void __stdcall ResetLocalSkills();
// 0x0009cd20
void __stdcall ApplyPlayerSkills(int player);
} // namespace AimStuff
#endif

// Native writes only front.x/front.z and up.x/up.z; other aim words are untouched.
// 0x0009ca70
void GameAimLookAtPartial(CameraAim* aim, float x, float y, float z);
