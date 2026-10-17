#ifndef SAMP_RECOVERED_AIMSTUFF_H
#define SAMP_RECOVERED_AIMSTUFF_H
#include <windows.h>
struct AimVector { float x,y,z; };
struct CameraAim { AimVector front,source,sourceBeforeLookBehind,up; };
typedef char CameraAimExtent[(sizeof(CameraAim)==48)?1:-1];
// samp.dll RVA: 0x001419b0
extern CameraAim *g_aimInternal;
// samp.dll RVA: 0x001419b4
extern float *g_aimInternalStats;
// samp.dll RVA: 0x001419b8
extern float *g_aimInternalAspect;
// samp.dll RVA: 0x001419bc
extern float *g_aimInternalZoom;
// samp.dll RVA: 0x0015195c
extern BYTE *g_aimInternalCameraMode;
// samp.dll RVA: 0x00182288
extern CameraAim g_aimLocal;
// samp.dll RVA: 0x001822b8
extern CameraAim g_aimPlayers[210];
// samp.dll RVA: 0x00181e60
extern float g_aimLocalZoom;
// samp.dll RVA: 0x00184a18
extern float g_aimLocalAspect;
// samp.dll RVA: 0x00181f40
extern float g_aimPlayerZoom[210];
// samp.dll RVA: 0x00184a48
extern float g_aimPlayerAspect[210];
// samp.dll RVA: 0x00181e68
extern BYTE g_aimPlayerCameraMode[210];
// samp.dll RVA: 0x00184a1c
extern float g_aimLocalSkills[11];
// samp.dll RVA: 0x00184d90
extern float g_aimPlayerSkills[210][11];
namespace AimStuff {
    // samp.dll RVA: 0x0009c240
    void __stdcall UpdateCamera();
    // samp.dll RVA: 0x0009c260
    void __stdcall ApplyLocalCamera();
    // samp.dll RVA: 0x0009c280
    void __stdcall SetPlayerCamera(BYTE player,float zoom,float aspect);
    // samp.dll RVA: 0x0009c2a0
    float __stdcall GetAspectRatio();
    // samp.dll RVA: 0x0009c2b0
    float __stdcall GetCameraExtZoom();
    // samp.dll RVA: 0x0009c2d0
    void __stdcall ApplyPlayerCamera(BYTE player);
    // samp.dll RVA: 0x0009c310
    void __stdcall SetPlayerCameraMode(BYTE mode,BYTE player);
    // samp.dll RVA: 0x0009c330
    BYTE __stdcall GetPlayerCameraMode(BYTE player);
    // samp.dll RVA: 0x0009c340
    BYTE __stdcall GetCameraMode();
    // samp.dll RVA: 0x0009c350
    void __stdcall Initialize();
    // samp.dll RVA: 0x0009c3c0
    void __stdcall UpdateAim();
    // samp.dll RVA: 0x0009c3e0
    void __stdcall ApplyLocalAim();
    // samp.dll RVA: 0x0009c400
    CameraAim *__stdcall GetInternalAim();
    // samp.dll RVA: 0x0009c410
    void __stdcall SetAim(int player,const CameraAim *aim);
    // samp.dll RVA: 0x0009c440
    void __stdcall ApplyPlayerAim(int player);
    // samp.dll RVA: 0x0009c470
    CameraAim *__stdcall GetPlayerAim(int player);
    // samp.dll RVA: 0x0009c490
    void __stdcall SetPlayerSkill(int player,int skill,WORD value);
    // samp.dll RVA: 0x0009c5b0
    void __stdcall UpdateLocalSkills();
    // samp.dll RVA: 0x0009c640
    void __stdcall ApplyLocalSkills();
    // samp.dll RVA: 0x0009c710
    void __stdcall ResetLocalSkills();
    // samp.dll RVA: 0x0009c7a0
    void __stdcall ApplyPlayerSkills(int player);
}
#endif
