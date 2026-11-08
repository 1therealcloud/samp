#ifndef SAMP_RECOVERED_GAME_H
#define SAMP_RECOVERED_GAME_H
#include "game/audio.h"
#include "game/camera.h"
#include "game/scripting.h"
#include <d3d9.h>
#include <stddef.h>
#include <windows.h>
class CPed;
class CVehicle;
struct CVector;
class CEntity;
class CObject;
class CMatrix;
#pragma pack(push, 1)
// Region at CGame+0C; no standalone allocation or native constructor.
struct GameCheckpointState
{
    CVector racePosition, raceNext;
    float raceSize;
    BYTE raceType;
    DWORD raceEnabled, raceMarker, raceCheckpoint;
    CVector position, size;
    DWORD enabled, marker;
};
// Exact allocated extent and observed fields. Opaque regions deliberately retain native bytes.
class CGame
{
public:
    // GTA SA VA: 0x00c17044; raw screen-width global, without a device query.
    // 0x000607b0
    int GetScreenWidth();
    // GTA SA VA: 0x00c17048; same raw signed screen metric as the 0.2x wrapper.
    // 0x00065ee0
    int GetScreenHeight();
    // GTA SA VA: 0x00859524; returns the raw HUD vertical scale in ST0.
    // 0x000b26d0
    float GetHudVertScale();
    // GTA SA VA: 0x00859520; returns the raw HUD horizontal scale in ST0.
    // 0x000b26e0
    float GetHudHorizScale();
    // 0x00060d20
    int GetCursorMode();
    // Clears the delay only; no immediate cursor/input backend calls.
    // 0x0006d430
    void ResetInputEnableWait();
    // Stores the raw BOOL DWORD without normalization.
    // 0x00067f10
    void SetHeadMovement(BOOL enabled);
    // Raw DWORD setters; no marker creation/deletion or bool normalization.
    // 0x000080f0
    void SetCheckpointEnabled(DWORD enabled);
    // 0x00008100
    void SetRacingCheckpointEnabled(DWORD enabled);
    // Uses the native preview-ped buffer and a temporary vehicle when requested.
    // 0x000a15b0
    void ApplyPreviewAnimation(DWORD first, DWORD second, BOOL useVehicle, int model, DWORD color);
    // 0x0009fea0
    void SetTimeScale(float scale);
    // Native free means occupied DWORD !=1, not only zero.
    // 0x0009ff00
    BYTE FindFreePlayerSlot();
    // 0x0009ff30
    BYTE CountUsedPlayerSlots();
    // Descriptive name; native string consumers remain to be attributed.
    // 0x000a0120
    void SetEightByteName(const char* name);
    // Virtual scalar deletion; no pool-slot bookkeeping is performed.
    // 0x000a0150
    BOOL DeleteEntity(CEntity* entity);
    // Native forwarding entry to the recovered weapon-model switch.
    // 0x000a0210
    int GetWeaponModel(int weapon);
    // Native key index is unchecked, including negative/out-of-range values.
    // 0x000a0220
    BOOL IsKeyPressed(int key);
    // Invalid IDs and already-loaded models return TRUE without streaming calls.
    // 0x000a0870
    BOOL LoadModelNow(int model);
    // 0x000a09a0
    DWORD GetWeather();
    // 0x000a09b0
    BYTE GetFrontendByteBA6769();
    // 0x000a09d0
    BOOL IsFrontendFlagBA6794Set();
    // 0x000a09e0
    void SetFrontendFlagBA6794();
    // 0x000a1570
    void SetTimer(DWORD value);
    // 0x000a15a0
    DWORD GetBackendWordB71804();
    // Native reserves slots2..209; sixth argument is ignored.
    // 0x0009ff50
    CPed* CreatePed(int model, CVector position, float rotation, DWORD unused, BOOL playerFlag);

    // 0x000a0ad0
    void ReleaseAnimation(const char* name);
    // 0x000a0b00
    void EnableRadar(BOOL enabled);
    // 0x000a0d20
    void UpdateFarClippingPlane();
    // 0x000a0dc0
    const char* GetWeaponName(int weapon);
    // 0x000a11d0
    void Restart();
    // 0x000a1250
    void SetNumberOfIntroTextLinesThisFrame(WORD value);
    // 0x000a1590
    int GetTimer();
    // 0x000a19e0
    BOOL UsingGamepad();

    // 0x000a0170
    CObject* NewObject(int model, float x, float y, float z, CVector rotation, float drawDistance, BYTE firstHour, BYTE lastHour);
    CAudio* m_pAudio;
    CCamera* m_pCamera;
    CPed* m_pPlayerPed;
    GameCheckpointState m_checkpoints;
    union
    {
        DWORD m_nTrackedNameDigestMatches;
        // Historical raw name retained for frozen archive fixtures.
        DWORD unknown55;
    };
    BOOL m_bHeadMove;
    int m_nFrameLimiter;
    int m_nCursorMode;
    int m_nInputEnableWaitFrames; // Native JG is signed; -1 is the completed state.
    BOOL m_bClockEnabled;
    BYTE unknown6d;
    bool m_aKeepLoadedVehicleModels[212];
    // 0x0009fdc0
    CGame();
    // 0x000a16c0
    void LoadAnimationsAndModels();
    // 0x000a07e0
    void ReleaseModel(int model, BYTE force);
    // 0x000a1430
    void SetRequiredVehicleModels(BYTE* priorities);
    // 0x000a1260
    void DrawGangZone(const float* position, DWORD color);
    // 0x000a1010
    DWORD CreatePickup(int model, int type, float x, float y, float z, DWORD* index);
    // 0x000a10f0
    DWORD CreateWeaponPickup(int model, int ammo, float x, float y, float z);
    // 0x000a0b20
    void DisplayGameText(const char* text, int time, int style);
    // 0x000a1d40
    void ProcessCheckpoints();
    // 0x000a1f20
    void SetRacingCheckpoint(BYTE type, const CVector* position, const CVector* next, float size);
    // 0x000a1cc0
    void CreateRacingCheckpoint();
    // 0x000a1c00
    void SetCheckpoint(const CVector* position, const CVector* size);
    // 0x000a0bb0
    DWORD CreateMarker(int icon, float x, float y, float z, DWORD color, int type);
    // 0x000a0b80
    void DeleteRacingCheckpoint();
    // 0x000a1ee0
    void ResetMoney();
    // 0x000a0050
    BOOL RemovePed(CPed* ped);
    // 0x000a0090
    CVehicle* CreateVehicle(int model, float x, float y, float z, float rotation, BOOL siren);
    // 0x000a0a60
    void RefreshRenderer(float x, float y);
    // 0x000a0240
    float FindGroundZ(float x, float y, float z);
    // 0x000a1380
    void DisableEnterExits();
    // 0x000a19f0
    void DisableAutoAiming();
    // 0x000a1200
    void* GetWeaponInfo(int weapon, int skill);
    // 0x000a19c0
    void LoadCullZone(const char* name);
    // 0x000a19a0
    void LoadCollisionFile(const char* name);
    // 0x000a13d0
    void LoadScene(const char* name);
    // 0x000a1220
    void SetWorldGravity(float gravity);
    // 0x000a1360
    void EnableStuntBonus(BYTE enabled);
    // 0x000a1340
    void EnableZoneDisplaying(BYTE enabled);
    // 0x000a0d90
    void IncreasePlayerMoney(int amount);
    // 0x000a0ce0
    void DeleteMarker(DWORD marker);
    // 0x000a0ab0
    DWORD IsAnimationLoaded(const char* name);
    // 0x000a0a90
    void RequestAnimation(const char* name);
    // 0x000a0a50
    void DisableTrainTraffic();
    // 0x000a0920
    void GetWorldTime(BYTE* hour, BYTE* minute);
    // 0x000a1240
    void SetWantedLevel(BYTE level);
    // 0x000a1400
    DWORD GetStreamingMemory();
    // 0x000a13f0
    DWORD GetUsedMemory();
    // 0x000a0db0
    int GetPlayerMoney();
    // Technical fixed-table scan; full backend record meaning remains unresolved.
    // 0x000a1410
    int CountBackendVehicleModelRecords();
    // 0x000a0a20
    void SetMaxStats();
    // 0x000a0940
    void LetTimeGo(char enabled);
    // 0x000a0980
    void SetWorldWeather(int weather);
    // 0x000a08f0
    void SetWorldTime(int hour, int minute);
    // 0x00001010
    CPed* GetPlayerPed();
    // 0x000a06d0
    void InitGame();
    // 0x000a0720
    void StartGame();
    // Native consumes two stack arguments but serializes only model.
    // 0x000a0780
    void RequestModel(int model, int loadingStream);
    // 0x000a07a0
    void LoadRequestedModels();
    // 0x000a07b0
    BOOL IsModelAvailable(int model);
    // 0x000a0760
    BOOL IsMenuVisible();
    // 0x000a0d00
    BYTE GetCurrentInterior();
    // 0x000a0530
    void SetCursorMode(int, BOOL);
    // 0x000a0410
    void ProcessInputEnabling();
    // 0x000a1190
    IDirect3DDevice9* GetDevice();
    // 0x000a0770
    BOOL IsStarted();
    // 0x000a09f0
    void SetFrameLimiter(int);
    // 0x000a1a30
    void ProcessFrameLimiter();
    // 0x0009fed0
    void Sleep(int elapsed, int limit);
    // 0x000a1280
    void EnableClock(bool);
    // 0x00002d40
    void* GetWindowHandle();
    // 0x00002d50
    CAudio* GetAudio();
    // 0x00002d60
    CCamera* GetCamera();
    // 0x00002d70
    BOOL DoesHeadMoves();
    // 0x000a1bd0
    void EnableHUD(BOOL enabled);
};
#pragma pack(pop)
typedef char GameExtent[(sizeof(CGame) == 0x142) ? 1 : -1];
typedef char CheckpointRegionSize[(sizeof(GameCheckpointState) == 0x49) ? 1 : -1];
typedef char CheckpointMarkerOffset[(offsetof(GameCheckpointState, marker) == 0x45) ? 1 : -1];
typedef char RacingMarkerOffset[(offsetof(GameCheckpointState, raceMarker) == 0x21) ? 1 : -1];
typedef char GameCheckpointOffset[(offsetof(CGame, m_checkpoints) == 0x0c) ? 1 : -1];
typedef char GameDigestMatchOffset[(offsetof(CGame, m_nTrackedNameDigestMatches) == 0x55) ? 1 : -1];
typedef char GameCursorOffset[(offsetof(CGame, m_nCursorMode) == 0x61) ? 1 : -1];
typedef char GameWaitOffset[(offsetof(CGame, m_nInputEnableWaitFrames) == 0x65) ? 1 : -1];
typedef char GameClockOffset[(offsetof(CGame, m_bClockEnabled) == 0x69) ? 1 : -1];
// 0x0018e1d8
extern BOOL g_gameUsedPlayerSlots[210];

// 0x000aa0b0
void UnFuck(void* address, SIZE_T size);
// 0x000a02f0
void GameClearMouseDelta();
// 0x000a0310
void GameUpdateMouse();
// 0x000a0320
void GameDisableMousePointers();
// 0x000a0370
void GameRestoreMousePointers();
// 0x000a03c0
void GameDisableMouseCalls();
// 0x002ac9d0
extern IDirect3DDevice9* g_gameD3DDevice;
// 0x002ac9b8
extern int g_gameVersion;
// 0x0018e1c8
extern DWORD g_gameMouseDummy;
// 0x00151918
extern BYTE g_gameKeyCall[5];
// 0x00151920
extern BYTE g_gameMouseCallEU[5];
// 0x00151928
extern BYTE g_gameMouseCallUS[5];

// Physical native bank ends at the following enable flag. The enabled lookup
// does not check indices; adjacency outside this bank is not proven equivalent.
// 0x001c0444
extern void* g_gameCustomModelInfo[32768];
// 0x001e0444
extern DWORD g_gameUseCustomModelInfo;
// 0x00152ae0
extern void** g_gameModelInfoTable;
// Opaque native storage cleared immediately before the established model bank.
// 0x001a0448
extern DWORD g_gameCustomModelPrefix[32767];
#pragma pack(push, 1)
struct GameModelReferencePatch
{
    DWORD address;
    BYTE opcode;
};
#pragma pack(pop)
typedef char NativeModelReferencePatchExtent[(sizeof(GameModelReferencePatch) == 5) ? 1 : -1];
// Addresses stored in these records are GTA SA virtual addresses.
// 0x00152ae8
extern const GameModelReferencePatch g_gameModelReferencePatchesUS[707];
// 0x001538b8
extern const GameModelReferencePatch g_gameModelReferencePatchesEU[707];
// 0x000a7560
BOOL GamePatchModelInfoReferences();
// 0x000a75f0
BOOL GameInitializeCustomModelInfo();
// 0x000b6390
void* __cdecl GameCreateCollisionModel();
// 0x000b63c0
BOOL __cdecl GameLoadCollisionData(const void* data, DWORD length, void* collision);
// Raw plugin callback: only its first two stack words are consumed by native code.
// 0x000a78b0
DWORD __cdecl GameReadCustomCollision(void* stream, DWORD length);
// 0x000a7980
void GameRegisterCustomCollisionPlugin();
// 0x0011edc8
extern const char g_customCollisionWrongType[];
// 0x0011ed9c
extern const char g_customCollisionLoadFailed[];
// 0x0011ed68
extern const char g_customCollisionAllocationFailed[];
// 0x0011e7b8
extern const Scripting::OpcodeInfo g_gameCurrentInteriorCommand;

// 0x0011e664
extern const Scripting::OpcodeInfo g_gameRequestModelCommand;
// 0x0011e678
extern const Scripting::OpcodeInfo g_gameLoadModelsCommand;
// 0x0011e6a0
extern const Scripting::OpcodeInfo g_gameModelAvailableCommand;
// 0x0011e618
extern const char g_gameModelTimeoutWarning[41];
// 0x000a7630
void* __stdcall GameGetModelInfo(int model);
// Observed raw DWORD at model-info +0x1c. No additional null guard is native.
// 0x000b41a0
DWORD __stdcall GameGetModelData1c(int model);
// 0x000b39b0
BOOL __stdcall GameHasModelInfo(int model);
// 0x000b39d0
BOOL __stdcall GameIsPedModel(int model);
// Observed wrapped vtable-address-plus-14 test, not a slot-content query.
// 0x000b4170
BOOL __stdcall GameModelQueryB4170(int model);
struct CVector;
// 0x000b5ef0
float __cdecl GameSetModelCollisionRadius(int model, float radius);
// 0x000b5f50
BOOL __cdecl GameGetModelCollisionCentre(int model, CVector* centre);
// 0x000b5fe0
BYTE __stdcall GameGetModelFlagsByte(int model);
// 0x000b6000
WORD __stdcall GameGetModelFlagsWord(int model);
// Raw EAX from model-info vtable +2C, gated by the existing game pointer.
// 0x000b5d90
DWORD __cdecl GameQueryModelVirtual2c(int model);
// 0x000b4150
void __stdcall GameSetModelDrawDistance(void* info, float distance);
// 0x000b4160
float __stdcall GameGetModelDrawDistance(const void* info);
// 0x000b4200
void __stdcall GameSetModelRenderObjectCall3c(int model, void* object);
// 0x000b4230
void __stdcall GameSetModelCollision(int model, void* collision);
// 0x000b4280
DWORD __stdcall GameGetModelTxdIndex(int model);
// 0x000b42d0
void* __stdcall GameGetModelCollision(int model);
// 0x000b4300
WORD __stdcall GameGetModelReferenceCount(int model);
// 0x000b1c40
BOOL GameCleanupModelAndTxd(int model);
// 0x000b1070
void GameClearModelCollision(int model);
// 0x000b3d00
DWORD __cdecl GameReadPedModelTable28(int model);
// 0x000b3d40
DWORD __cdecl GameReadPedModelTable2c(int model);
// 0x000b3d80
DWORD __cdecl GameReadPedModelTable24(int model);

// 0x0018e1cc
extern char* g_gameTextBuffer;

// 0x000aa060
void GameWaitUnprotect(BYTE* address, SIZE_T size, BYTE expected);
// 0x000aa300
BOOL GameInitializeVersionPatches();
// 0x000a5c10
DWORD __cdecl GameFrameNodeNameHook(void* arg0, DWORD length, void* arg2);
// 0x000a5c90
DWORD __cdecl GameDataReadHook(void* arg0, DWORD length, void* arg2);
// 0x000b1a40
DWORD GameReadStream(void* stream, void* buffer, DWORD length);

// 0x002a9270
extern DWORD g_gamePhysicalMegabytes;
// 0x002a9274
extern DWORD g_gameStreamingBytes;
// 0x0011ee84
extern const char g_gameStartupTitle[6];
// 0x0018f388
extern char g_gameFrameNodeError[256];
// 0x0018f6b4
extern bool g_gameCustomStreamRead;
// 0x0018f6b8
extern void* g_gameStreamBuffer;
// 0x0018e520
extern BYTE g_frameKeyCycle;
// 0x0018e524
extern DWORD g_frameKeyCycleTick;
// 0x0018e528
extern DWORD g_frameSampleCount;
// 0x0018e52c
extern DWORD g_frameSampleTick;
// 0x0018e530
extern DWORD g_frameSleepTick;
// 0x0018e534
extern float g_frameScale;
// 0x001181d8
extern const float g_frameScaleMultiplier;
// 0x0011c664
extern const float g_frameScaleNumerator;
// 0x0009fe50
void GameUpdateKeyCycle();
// 0x000b5d10
void GameSetVideoMode(DWORD mode);

// 0x0011e844
extern const Scripting::OpcodeInfo g_gameCreateWeaponPickupCommand;

// GTA backend collision adapters; pointers describe observed prefixes, not complete GTA types.
// 0x000b3df0
int __cdecl GameCameraSphereQuery(const void* sphere);
// 0x000b6110
BYTE __stdcall GameTestEntityLineSphere(const CVector* origin, const CVector* end, void* backend, const CVector* centre, float scale, BYTE cameraCheck);
// 0x002abe58
extern BYTE g_gameLineSphereResult;

// 0x0011e808
extern const Scripting::OpcodeInfo g_gameReleaseAnimationCommand;
// 0x0011e8e0
extern const float g_gameFarClipBase;
// 0x0011e8dc
extern const float g_gameFarClipMinimum;
// 0x0015196c
extern float g_gameFarClipDistance;
// 0x0011ea9c
extern const char g_gameWeaponName01[15];
// 0x0011ea90
extern const char g_gameWeaponName02[10];
// 0x0011ea84
extern const char g_gameWeaponName03[11];
// 0x0011ea7c
extern const char g_gameWeaponName04[6];
// 0x0011ea6c
extern const char g_gameWeaponName05[13];
// 0x0011ea64
extern const char g_gameWeaponName06[7];
// 0x0011ea58
extern const char g_gameWeaponName07[9];
// 0x0011ea50
extern const char g_gameWeaponName08[7];
// 0x0011ea44
extern const char g_gameWeaponName09[9];
// 0x0011ea3c
extern const char g_gameWeaponName10[6];
// 0x0011ea30
extern const char g_gameWeaponName12[9];
// 0x0011ea28
extern const char g_gameWeaponName14[8];
// 0x0011ea20
extern const char g_gameWeaponName15[5];
// 0x0011ea18
extern const char g_gameWeaponName16[8];
// 0x0011ea10
extern const char g_gameWeaponName17[8];
// 0x0011ea08
extern const char g_gameWeaponName18[8];
// 0x0011ea00
extern const char g_gameWeaponName22[8];
// 0x0011e9f0
extern const char g_gameWeaponName23[16];
// 0x0011e9e0
extern const char g_gameWeaponName24[13];
// 0x0011e9d8
extern const char g_gameWeaponName25[8];
// 0x0011e9c4
extern const char g_gameWeaponName26[17];
// 0x0011e9b4
extern const char g_gameWeaponName27[15];
// 0x0011e9b0
extern const char g_gameWeaponName28[4];
// 0x0011e9ac
extern const char g_gameWeaponName29[4];
// 0x0011e9a4
extern const char g_gameWeaponName30[5];
// 0x0011e9a0
extern const char g_gameWeaponName31[3];
// 0x0011e998
extern const char g_gameWeaponName32[5];
// 0x0011e990
extern const char g_gameWeaponName33[6];
// 0x0011e980
extern const char g_gameWeaponName34[13];
// 0x0011e970
extern const char g_gameWeaponName35[16];
// 0x0011e964
extern const char g_gameWeaponName36[12];
// 0x0011e954
extern const char g_gameWeaponName37[13];
// 0x0011e94c
extern const char g_gameWeaponName38[8];
// 0x0011e938
extern const char g_gameWeaponName39[19];
// 0x0011e930
extern const char g_gameWeaponName40[5];
// 0x0011e924
extern const char g_gameWeaponName41[10];
// 0x0011e910
extern const char g_gameWeaponName42[18];
// 0x0011e908
extern const char g_gameWeaponName43[7];
// 0x0011e8fc
extern const char g_gameWeaponName46[10];
// 0x0011e8f4
extern const char g_gameWeaponName49[8];
// 0x0011e8ec
extern const char g_gameWeaponName53[8];
// 0x0011e8e4
extern const char g_gameWeaponName54[6];

// Shared native name storage; only the first8 bytes are reset/read by helpers.
// 0x00151950
extern char g_gameEightByteName[9];
// 0x0018e53c
extern BYTE g_gameTimerOverridden;
// 0x000a3ed0
void GameSetEightByteName(const char* name);
// 0x000a3f00
bool GameCopyEightByteName(void* output);
// Backing storage returned by the installed GTA preview hook.
// 0x0018eb98
extern DWORD g_gamePreviewPed[488];

// Unchecked index; native reloads selected table after the real CRT free.
// 0x000a7720
void GameFreeModelInfo(int model);
// Descriptive name; original name and unused parameters are unknown.
// 0x0000e4f0
void GameLoadRequestedModelsAndEndThread();
#endif
