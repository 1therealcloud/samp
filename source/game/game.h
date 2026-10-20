#ifndef SAMP_RECOVERED_GAME_H
#define SAMP_RECOVERED_GAME_H
#include <windows.h>
#include <d3d9.h>
#include <stddef.h>
#include "game/audio.h"
#include "game/camera.h"
class CPed;
class CEntity;
class CMatrix;
#pragma pack(push,1)
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
    // samp.dll RVA: 0x000a1220
    void SetWorldGravity(float gravity);
    // samp.dll RVA: 0x000a1360
    void EnableStuntBonus(BYTE enabled);
    // samp.dll RVA: 0x000a1340
    void EnableZoneDisplaying(BYTE enabled);
    // samp.dll RVA: 0x000a0d90
    void IncreasePlayerMoney(int amount);
    // samp.dll RVA: 0x000a0ce0
    void DeleteMarker(DWORD marker);
    // samp.dll RVA: 0x000a0ab0
    DWORD IsAnimationLoaded(const char *name);
    // samp.dll RVA: 0x000a0a90
    void RequestAnimation(const char *name);
    // samp.dll RVA: 0x000a0a50
    void DisableTrainTraffic();
    // samp.dll RVA: 0x000a0920
    void GetWorldTime(BYTE *hour,BYTE *minute);
    // samp.dll RVA: 0x000a1240
    void SetWantedLevel(BYTE level);
    // samp.dll RVA: 0x000a1400
    DWORD GetStreamingMemory();
    // samp.dll RVA: 0x000a13f0
    DWORD GetUsedMemory();
    // samp.dll RVA: 0x000a0db0
    int GetPlayerMoney();
    // Technical fixed-table scan; full backend record meaning remains unresolved.
    // samp.dll RVA: 0x000a1410
    int CountBackendVehicleModelRecords();
    // samp.dll RVA: 0x000a0a20
    void SetMaxStats();
    // samp.dll RVA: 0x000a0940
    void LetTimeGo(char enabled);
    // samp.dll RVA: 0x000a0980
    void SetWorldWeather(int weather);
    // samp.dll RVA: 0x000a08f0
    void SetWorldTime(int hour,int minute);
    // samp.dll RVA: 0x00001010
    CPed *GetPlayerPed();
    // samp.dll RVA: 0x000a06d0
    void InitGame();
    // samp.dll RVA: 0x000a0720
    void StartGame();
    // Native consumes two stack arguments but serializes only model.
    // samp.dll RVA: 0x000a0780
    void RequestModel(int model,int loadingStream);
    // samp.dll RVA: 0x000a07a0
    void LoadRequestedModels();
    // samp.dll RVA: 0x000a07b0
    BOOL IsModelAvailable(int model);
    // samp.dll RVA: 0x000a0760
    BOOL IsMenuVisible();
    // samp.dll RVA: 0x000a0d00
    BYTE GetCurrentInterior();
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
    // samp.dll RVA: 0x000a1a30
    void ProcessFrameLimiter();
    // samp.dll RVA: 0x0009fed0
    void Sleep(int elapsed,int limit);
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
    // samp.dll RVA: 0x000a1bd0
    void EnableHUD(BOOL enabled);
};
#pragma pack(pop)
typedef char GameExtent[(sizeof(CGame)==0x142)?1:-1];
typedef char GameCursorOffset[(offsetof(CGame,m_nCursorMode)==0x61)?1:-1];
typedef char GameWaitOffset[(offsetof(CGame,m_nInputEnableWaitFrames)==0x65)?1:-1];
typedef char GameClockOffset[(offsetof(CGame,m_bClockEnabled)==0x69)?1:-1];
// samp.dll RVA: 0x0018e1d8
extern BOOL g_gameUsedPlayerSlots[210];
#endif
