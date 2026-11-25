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
static_assert(sizeof(CameraAim) == 48);

extern CameraAim* g_aimInternal;

extern float* g_aimInternalStats;

extern float* g_aimInternalAspect;

extern float* g_aimInternalZoom;

extern BYTE* g_aimInternalCameraMode;

extern CameraAim g_aimLocal;

extern CameraAim g_aimPlayers[210];

extern float g_aimLocalZoom;

extern float g_aimLocalAspect;

extern float g_aimPlayerZoom[210];

extern float g_aimPlayerAspect[210];

extern BYTE g_aimPlayerCameraMode[210];

extern float g_aimLocalSkills[11];

extern float g_aimPlayerSkills[210][11];
namespace AimStuff
{

void __stdcall GetCameraExtZoomAndAspectRatio(float* zoom, float* aspect);

void __stdcall UpdateCamera();

void __stdcall ApplyLocalCamera();

void __stdcall SetPlayerCamera(BYTE player, float zoom, float aspect);

float __stdcall GetAspectRatio();

float __stdcall GetCameraExtZoom();

void __stdcall ApplyPlayerCamera(BYTE player);

void __stdcall SetPlayerCameraMode(BYTE mode, BYTE player);

BYTE __stdcall GetPlayerCameraMode(BYTE player);

BYTE __stdcall GetCameraMode();

void __stdcall Initialize();

void __stdcall UpdateAim();

void __stdcall ApplyLocalAim();

CameraAim* __stdcall GetInternalAim();

void __stdcall SetAim(int player, const CameraAim* aim);

void __stdcall ApplyPlayerAim(int player);

CameraAim* __stdcall GetPlayerAim(int player);

void __stdcall SetPlayerSkill(int player, int skill, WORD value);
// Native checks only skill<11, including negative indices.

void __stdcall SetLocalSkill(int skill, WORD value);

void __stdcall UpdateLocalSkills();

void __stdcall ApplyLocalSkills();

void __stdcall ResetLocalSkills();

void __stdcall ApplyPlayerSkills(int player);
} // namespace AimStuff
#endif

// Native writes only front.x/front.z and up.x/up.z; other aim words are untouched.

void GameAimLookAtPartial(CameraAim* aim, float x, float y, float z);
