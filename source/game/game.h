#ifndef SAMP_RECOVERED_GAME_H
#define SAMP_RECOVERED_GAME_H
#include <windows.h>
#include <d3d9.h>
#include <stddef.h>
class CPed;
class CEntity;
class CMatrix;
#pragma pack(push,1)
class CAudio {
public:
    BOOL m_bSoundLoaded;
    bool m_bOutdoorAmbienceTrackDisabled;
    // samp.dll RVA: unknown (inline initialization in CGame RVA 0x0009fdc0; standalone entry not established)
    CAudio() : m_bSoundLoaded(FALSE),m_bOutdoorAmbienceTrackDisabled(false) {}
};
class CCamera {
public:
    CEntity *m_pAttachedTo;
    CMatrix *m_pMatrix;
    // samp.dll RVA: 0x0009fda0
    CCamera();
};
// Exact allocated extent and observed fields. Opaque regions deliberately retain native bytes.
class CGame {
public:
    CAudio *m_pAudio;
    CCamera *m_pCamera;
    CPed *m_pPlayerPed;
    BYTE unknown0c[0x1d];
    DWORD unknown29;
    BYTE unknown2d[4];
    DWORD unknown31;
    BYTE unknown35[0x18];
    DWORD unknown4d;
    BYTE unknown51[4];
    DWORD unknown55;
    BOOL m_bHeadMove;
    int m_nFrameLimiter;
    int m_nCursorMode;
    int m_nInputEnableWaitFrames; // Native JG is signed; -1 is the completed state.
    BOOL m_bClockEnabled;
    BYTE unknown6d;
    bool m_aKeepLoadedVehicleModels[212];
    // samp.dll RVA: 0x0009fdc0
    CGame();
    // samp.dll RVA: 0x00001010
    CPed *GetPlayerPed();
    // samp.dll RVA: 0x000a06d0
    void InitGame();
    // samp.dll RVA: 0x000a0760
    BOOL IsMenuVisible();
    // samp.dll RVA: 0x000a0530
    void SetCursorMode(int,BOOL);
    // samp.dll RVA: 0x000a0410
    void ProcessInputEnabling();
    // samp.dll RVA: 0x000a1190
    IDirect3DDevice9 *GetDevice();
    // samp.dll RVA: 0x000a0770
    BOOL IsStarted();
    // samp.dll RVA: 0x000a09f0
    void SetFrameLimiter(int);
    // samp.dll RVA: 0x000a1280
    void EnableClock(bool);
    // samp.dll RVA: 0x00002d40
    void *GetWindowHandle();
    // samp.dll RVA: 0x00002d50
    CAudio *GetAudio();
    // samp.dll RVA: 0x00002d60
    CCamera *GetCamera();
    // samp.dll RVA: 0x00002d70
    BOOL DoesHeadMoves();
};
#pragma pack(pop)
typedef char GameExtent[(sizeof(CGame)==0x142)?1:-1];
typedef char GameCursorOffset[(offsetof(CGame,m_nCursorMode)==0x61)?1:-1];
typedef char GameWaitOffset[(offsetof(CGame,m_nInputEnableWaitFrames)==0x65)?1:-1];
typedef char GameClockOffset[(offsetof(CGame,m_bClockEnabled)==0x69)?1:-1];
typedef char AudioExtent[(sizeof(CAudio)==5)?1:-1];
typedef char CameraExtent[(sizeof(CCamera)==8)?1:-1];
// samp.dll RVA: 0x0018e1d8
extern BOOL g_gameUsedPlayerSlots[210];
#endif
