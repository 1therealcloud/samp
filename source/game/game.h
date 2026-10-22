#ifndef SAMP_RECOVERED_GAME_H
#define SAMP_RECOVERED_GAME_H
#include <windows.h>
#include <d3d9.h>
#include <stddef.h>
#include "game/audio.h"
#include "game/camera.h"
class CPed;
class CVehicle;
struct CVector;
class CEntity;
class CObject;
class CMatrix;
#pragma pack(push,1)
// Exact allocated extent and observed fields. Opaque regions deliberately retain native bytes.
class CGame {
public:
    // samp.dll RVA: 0x000a0170
    CObject *NewObject(int model,float x,float y,float z,CVector rotation,float drawDistance,BYTE firstHour,BYTE lastHour);
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
    // samp.dll RVA: 0x000a16c0
    void LoadAnimationsAndModels();
    // samp.dll RVA: 0x000a07e0
    void ReleaseModel(int model,BYTE force);
    // samp.dll RVA: 0x000a1430
    void SetRequiredVehicleModels(BYTE *priorities);
    // samp.dll RVA: 0x000a1260
    void DrawGangZone(const float *position,DWORD color);
    // samp.dll RVA: 0x000a1010
    DWORD CreatePickup(int model,int type,float x,float y,float z,DWORD *index);
    // samp.dll RVA: 0x000a10f0
    DWORD CreateWeaponPickup(int model,int ammo,float x,float y,float z);
    // samp.dll RVA: 0x000a0b20
    void DisplayGameText(const char *text,int time,int style);
    // samp.dll RVA: 0x000a1d40
    void ProcessCheckpoints();
    // samp.dll RVA: 0x000a1f20
    void SetRacingCheckpoint(BYTE type,const CVector *position,const CVector *next,float size);
    // samp.dll RVA: 0x000a1cc0
    void CreateRacingCheckpoint();
    // samp.dll RVA: 0x000a1c00
    void SetCheckpoint(const CVector *position,const CVector *size);
    // samp.dll RVA: 0x000a0bb0
    DWORD CreateMarker(int icon,float x,float y,float z,DWORD color,int type);
    // samp.dll RVA: 0x000a0b80
    void DeleteRacingCheckpoint();
    // samp.dll RVA: 0x000a1ee0
    void ResetMoney();
    // samp.dll RVA: 0x000a0050
    BOOL RemovePed(CPed *ped);
    // samp.dll RVA: 0x000a0090
    CVehicle * CreateVehicle(int model,float x,float y,float z,float rotation,BOOL siren);
    // samp.dll RVA: 0x000a0a60
    void RefreshRenderer(float x,float y);
    // samp.dll RVA: 0x000a0240
    float FindGroundZ(float x,float y,float z);
    // samp.dll RVA: 0x000a1380
    void DisableEnterExits();
    // samp.dll RVA: 0x000a19f0
    void DisableAutoAiming();
    // samp.dll RVA: 0x000a1200
    void * GetWeaponInfo(int weapon,int skill);
    // samp.dll RVA: 0x000a19c0
    void LoadCullZone(const char *name);
    // samp.dll RVA: 0x000a19a0
    void LoadCollisionFile(const char *name);
    // samp.dll RVA: 0x000a13d0
    void LoadScene(const char *name);
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

#ifndef SAMP_RECOVERED_GAME_INPUT_H
#define SAMP_RECOVERED_GAME_INPUT_H
#include "main.h"
// samp.dll RVA: 0x000aa0b0
void GameUnprotect(void *address,SIZE_T size);
// samp.dll RVA: 0x000a02f0
void GameClearMouseDelta();
// samp.dll RVA: 0x000a0310
void GameUpdateMouse();
// samp.dll RVA: 0x000a0320
void GameDisableMousePointers();
// samp.dll RVA: 0x000a0370
void GameRestoreMousePointers();
// samp.dll RVA: 0x000a03c0
void GameDisableMouseCalls();
// samp.dll RVA: 0x002ac9d0
extern IDirect3DDevice9 *g_gameD3DDevice;
// samp.dll RVA: 0x002ac9b8
extern int g_gameVersion;
// samp.dll RVA: 0x0018e1c8
extern DWORD g_gameMouseDummy;
// samp.dll RVA: 0x00151918
extern BYTE g_gameKeyCall[5];
// samp.dll RVA: 0x00151920
extern BYTE g_gameMouseCallEU[5];
// samp.dll RVA: 0x00151928
extern BYTE g_gameMouseCallUS[5];
#endif

#ifndef SAMP_GAME_MODEL_H
#define SAMP_GAME_MODEL_H
#include "game/scripting.h"
// Physical native bank ends at the following enable flag. The enabled lookup
// does not check indices; adjacency outside this bank is not proven equivalent.
// samp.dll RVA: 0x001c0444
extern void *g_gameCustomModelInfo[32768];
// samp.dll RVA: 0x001e0444
extern DWORD g_gameUseCustomModelInfo;
// samp.dll RVA: 0x00152ae0
extern void **g_gameModelInfoTable;
// samp.dll RVA: 0x0011e664
extern const Scripting::OpcodeInfo g_gameRequestModelCommand;
// samp.dll RVA: 0x0011e678
extern const Scripting::OpcodeInfo g_gameLoadModelsCommand;
// samp.dll RVA: 0x0011e6a0
extern const Scripting::OpcodeInfo g_gameModelAvailableCommand;
// samp.dll RVA: 0x0011e618
extern const char g_gameModelTimeoutWarning[41];
// samp.dll RVA: 0x000a7630
void *__stdcall GameGetModelInfo(int model);
// Observed raw DWORD at model-info +0x1c. No additional null guard is native.
// samp.dll RVA: 0x000b41a0
DWORD __stdcall GameGetModelData1c(int model);
// samp.dll RVA: 0x000b39b0
BOOL __stdcall GameHasModelInfo(int model);
// samp.dll RVA: 0x000b39d0
BOOL __stdcall GameIsPedModel(int model);
// Observed wrapped vtable-address-plus-14 test, not a slot-content query.
// samp.dll RVA: 0x000b4170
BOOL __stdcall GameModelQueryB4170(int model);
struct CVector;
// samp.dll RVA: 0x000b5ef0
float __cdecl GameSetModelCollisionRadius(int model,float radius);
// samp.dll RVA: 0x000b5f50
BOOL __cdecl GameGetModelCollisionCentre(int model,CVector *centre);
// samp.dll RVA: 0x000b5fe0
BYTE __stdcall GameGetModelFlagsByte(int model);
// samp.dll RVA: 0x000b6000
WORD __stdcall GameGetModelFlagsWord(int model);
// Raw EAX from model-info vtable +2C, gated by the existing game pointer.
// samp.dll RVA: 0x000b5d90
DWORD __cdecl GameQueryModelVirtual2c(int model);
// samp.dll RVA: 0x000b4150
void __stdcall GameSetModelDrawDistance(void *info,float distance);
// samp.dll RVA: 0x000b4160
float __stdcall GameGetModelDrawDistance(const void *info);
// samp.dll RVA: 0x000b4200
void __stdcall GameSetModelRenderObjectCall3c(int model,void *object);
// samp.dll RVA: 0x000b4230
void __stdcall GameSetModelCollision(int model,void *collision);
// samp.dll RVA: 0x000b4280
DWORD __stdcall GameGetModelTxdIndex(int model);
// samp.dll RVA: 0x000b42d0
void *__stdcall GameGetModelCollision(int model);
// samp.dll RVA: 0x000b4300
WORD __stdcall GameGetModelReferenceCount(int model);
// samp.dll RVA: 0x000b1c40
BOOL GameCleanupModelAndTxd(int model);
// samp.dll RVA: 0x000b1070
void GameClearModelCollision(int model);
// samp.dll RVA: 0x000b3d00
DWORD __cdecl GameReadPedModelTable28(int model);
// samp.dll RVA: 0x000b3d40
DWORD __cdecl GameReadPedModelTable2c(int model);
// samp.dll RVA: 0x000b3d80
DWORD __cdecl GameReadPedModelTable24(int model);
#endif

#ifndef SAMP_RECOVERED_GAME_INITIALIZATION_H
#define SAMP_RECOVERED_GAME_INITIALIZATION_H
// samp.dll RVA: 0x0018e1cc
extern char *g_gameTextBuffer;
#endif

#ifndef SAMP_RECOVERED_GAME_PREINIT_H
#define SAMP_RECOVERED_GAME_PREINIT_H
// samp.dll RVA: 0x000aa060
void GameWaitUnprotect(BYTE *address,SIZE_T size,BYTE expected);
// samp.dll RVA: 0x000aa300
BOOL GameInitializeVersionPatches();
// samp.dll RVA: 0x000a5c10
DWORD __cdecl GameFrameNodeNameHook(void *arg0,DWORD length,void *arg2);
// samp.dll RVA: 0x000a5c90
DWORD __cdecl GameDataReadHook(void *arg0,DWORD length,void *arg2);
// samp.dll RVA: 0x000b1a40
DWORD GameReadStream(void *stream,void *buffer,DWORD length);
// samp.dll RVA: 0x000b6610
void GameParseStreamData(const void *data,DWORD length,void *destination);
// samp.dll RVA: 0x002a9270
extern DWORD g_gamePhysicalMegabytes;
// samp.dll RVA: 0x002a9274
extern DWORD g_gameStreamingBytes;
// samp.dll RVA: 0x0011ee84
extern const char g_gameStartupTitle[6];
// samp.dll RVA: 0x0018f388
extern char g_gameFrameNodeError[256];
// samp.dll RVA: 0x0018f6b4
extern bool g_gameCustomStreamRead;
// samp.dll RVA: 0x0018f6b8
extern void *g_gameStreamBuffer;
#endif

// samp.dll RVA: 0x0011e844
extern const Scripting::OpcodeInfo g_gameCreateWeaponPickupCommand;

// GTA backend collision adapters; pointers describe observed prefixes, not complete GTA types.
// samp.dll RVA: 0x000b3df0
int __cdecl GameCameraSphereQuery(const void *sphere);
// samp.dll RVA: 0x000b6110
BYTE __stdcall GameTestEntityLineSphere(const CVector *origin,const CVector *end,void *backend,const CVector *centre,float scale,BYTE cameraCheck);
// samp.dll RVA: 0x002abe58
extern BYTE g_gameLineSphereResult;
