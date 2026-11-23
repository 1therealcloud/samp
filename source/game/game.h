#ifndef GAME_H
#define GAME_H

#include "game/audio.h"
#include "game/camera.h"
#include "game/scripting.h"

#include <d3d9.h>

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

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
    // R5 address to verify
    int GetScreenWidth();
    // GTA SA VA: 0x00c17048; same raw signed screen metric as the 0.2x wrapper.
    // R5 address to verify
    int GetScreenHeight();
    // GTA SA VA: 0x00859524; returns the raw HUD vertical scale in ST0.
    // R5 address to verify
    float GetHudVertScale();
    // GTA SA VA: 0x00859520; returns the raw HUD horizontal scale in ST0.
    // R5 address to verify
    float GetHudHorizScale();
    // R5 address to verify
    int GetCursorMode();
    // Clears the delay only; no immediate cursor/input backend calls.
    // R5 address to verify
    void ResetInputEnableWait();
    // Stores the raw BOOL DWORD without normalization.
    // R5 address to verify
    void SetHeadMovement(BOOL enabled);
    // Raw DWORD setters; no marker creation/deletion or bool normalization.
    // R5 address to verify
    void SetCheckpointEnabled(DWORD enabled);
    // R5 address to verify
    void SetRacingCheckpointEnabled(DWORD enabled);
    // Uses the native preview-ped buffer and a temporary vehicle when requested.
    // 0x000a1790
    void ApplyPreviewAnimation(DWORD first, DWORD second, BOOL useVehicle, int model, DWORD color);
    // R5 address to verify
    void SetTimeScale(float scale);
    // Native free means occupied DWORD !=1, not only zero.
    // R5 address to verify
    BYTE FindFreePlayerSlot();
    // 0x000a00f0
    BYTE CountUsedPlayerSlots();
    // R5 address to verify
    void SetEightByteName(const char* name);
    // Virtual scalar deletion; no pool-slot bookkeeping is performed.
    // R5 address to verify
    BOOL DeleteEntity(CEntity* entity);
    // Native forwarding entry to the recovered weapon-model switch.
    // R5 address to verify
    int GetWeaponModel(int weapon);
    // Native key index is unchecked, including negative/out-of-range values.
    // R5 address to verify
    BOOL IsKeyPressed(int key);
    // Invalid IDs and already-loaded models return TRUE without streaming calls.
    // 0x000a0a30
    BOOL LoadModelNow(int model);
    // R5 address to verify
    DWORD GetWeather();
    // R5 address to verify
    BYTE GetShowHudPreference();
    // R5 address to verify
    BOOL IsVSyncEnabled();
    // R5 address to verify
    void EnableVSync();
    // 0x000a1750
    void SetTimer(DWORD value);
    // R5 address to verify
    DWORD GetRenderedBuildingCount();
    // Native reserves slots2..209; sixth argument is ignored.
    // 0x000a0110
    CPed* CreatePed(int model, CVector position, float rotation, DWORD unused, BOOL playerFlag);

    // 0x000a0c90
    void ReleaseAnimation(const char* name);
    // 0x000a0cc0
    void EnableRadar(BOOL enabled);
    // 0x000a0f00
    void UpdateFarClippingPlane();
    // 0x000a0fa0
    const char* GetWeaponName(int weapon);
    // 0x000a13b0
    void Restart();
    // 0x000a1430
    void SetNumberOfIntroTextLinesThisFrame(WORD value);
    // 0x000a1770
    int GetTimer();
    // 0x000a1bc0
    BOOL UsingGamepad();

    // 0x000a0330
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
    // 0x0009ff80
    CGame();
    // 0x000a18a0
    void LoadAnimationsAndModels();
    // 0x000a09a0
    void ReleaseModel(int model, BYTE force);
    // 0x000a1610
    void SetRequiredVehicleModels(BYTE* priorities);
    // 0x000a1440
    void DrawGangZone(const float* position, DWORD color);
    // 0x000a11f0
    DWORD CreatePickup(int model, int type, float x, float y, float z, DWORD* index);
    // 0x000a12d0
    DWORD CreateWeaponPickup(int model, int ammo, float x, float y, float z);
    // 0x000a0ce0
    void DisplayGameText(const char* text, int time, int style);
    // 0x000a1f20
    void ProcessCheckpoints();
    // 0x000a2100
    void SetRacingCheckpoint(BYTE type, const CVector* position, const CVector* next, float size);
    // 0x000a1ea0
    void CreateRacingCheckpoint();
    // 0x000a1de0
    void SetCheckpoint(const CVector* position, const CVector* size);
    // 0x000a0d90
    DWORD CreateMarker(int icon, float x, float y, float z, DWORD color, int type);
    // 0x000a0d60
    void DeleteRacingCheckpoint();
    // 0x000a20c0
    void ResetMoney();
    // 0x000a0210
    BOOL RemovePed(CPed* ped);
    // 0x000a0250
    CVehicle* CreateVehicle(int model, float x, float y, float z, float rotation, BOOL siren);
    // 0x000a0c20
    void RefreshRenderer(float x, float y);
    // 0x000a0400
    float FindGroundZ(float x, float y, float z);
    // 0x000a1560
    void DisableEnterExits();
    // 0x000a1bd0
    void DisableAutoAiming();
    // 0x000a13e0
    void* GetWeaponInfo(int weapon, int skill);
    // 0x000a1ba0
    void LoadCullZone(const char* name);
    // 0x000a1b80
    void LoadCollisionFile(const char* name);
    // 0x000a15b0
    void LoadScene(const char* name);
    // 0x000a1400
    void SetWorldGravity(float gravity);
    // 0x000a1540
    void EnableStuntBonus(BYTE enabled);
    // 0x000a1520
    void EnableZoneDisplaying(BYTE enabled);
    // 0x000a0f70
    void IncreasePlayerMoney(int amount);
    // 0x000a0ec0
    void DeleteMarker(DWORD marker);
    // 0x000a0c70
    DWORD IsAnimationLoaded(const char* name);
    // 0x000a0c50
    void RequestAnimation(const char* name);
    // 0x000a0c10
    void DisableTrainTraffic();
    // 0x000a0ae0
    void GetWorldTime(BYTE* hour, BYTE* minute);
    // 0x000a1420
    void SetWantedLevel(BYTE level);
    // 0x000a15e0
    DWORD GetStreamingMemory();
    // 0x000a15d0
    DWORD GetUsedMemory();
    // 0x000a0f90
    int GetPlayerMoney();
    // Technical fixed-table scan; full backend record meaning remains unresolved.
    // 0x000a15f0
    int CountBackendVehicleModelRecords();
    // 0x000a0be0
    void SetMaxStats();
    // 0x000a0b00
    void LetTimeGo(char enabled);
    // 0x000a0b40
    void SetWorldWeather(int weather);
    // 0x000a0ab0
    void SetWorldTime(int hour, int minute);
    // 0x00001010
    CPed* GetPlayerPed();
    // 0x000a0890
    void InitGame();
    // 0x000a08e0
    void StartGame();
    // Native consumes two stack arguments but serializes only model.
    // 0x000a0940
    void RequestModel(int model, int loadingStream);
    // 0x000a0960
    void LoadRequestedModels();
    // 0x000a0970
    BOOL IsModelAvailable(int model);
    // 0x000a0920
    BOOL IsMenuVisible();
    // 0x000a0ee0
    BYTE GetCurrentInterior();
    // 0x000a06f0
    void SetCursorMode(int, BOOL);
    // 0x000a05d0
    void ProcessInputEnabling();
    // 0x000a1370
    IDirect3DDevice9* GetDevice();
    // 0x000a0930
    BOOL IsStarted();
    // 0x000a0bb0
    void SetFrameLimiter(int);
    // 0x000a1c10
    void ProcessFrameLimiter();
    // 0x000a0010
    void UpdateKeyCycle();
    // 0x000a0090
    void Sleep(int elapsed, int limit);
    // 0x000a1460
    void EnableClock(bool);
    // 0x00002d10
    void* GetWindowHandle();
    // 0x00002d20
    CAudio* GetAudio();
    // 0x00002d30
    CCamera* GetCamera();
    // 0x00002d40
    BOOL DoesHeadMoves();
    // 0x000a1db0
    void EnableHUD(BOOL enabled);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CGame) == 0x142);
STATIC_ASSERT(sizeof(GameCheckpointState) == 0x49);
STATIC_ASSERT(offsetof(GameCheckpointState, marker) == 0x45);
STATIC_ASSERT(offsetof(GameCheckpointState, raceMarker) == 0x21);
STATIC_ASSERT(offsetof(CGame, m_checkpoints) == 0x0c);
STATIC_ASSERT(offsetof(CGame, m_nTrackedNameDigestMatches) == 0x55);
STATIC_ASSERT(offsetof(CGame, m_nCursorMode) == 0x61);
STATIC_ASSERT(offsetof(CGame, m_nInputEnableWaitFrames) == 0x65);
STATIC_ASSERT(offsetof(CGame, m_bClockEnabled) == 0x69);
// 0x00150340
extern BOOL g_gameUsedPlayerSlots[210];

// 0x000aa4c0
void UnFuck(void* address, SIZE_T size);
// 0x000a04b0
void GameClearMouseDelta();
// R5 address to verify
void GameUpdateMouse();
// 0x000a04e0
void GameDisableMousePointers();
// 0x000a0530
void GameRestoreMousePointers();
// 0x000a0580
void GameDisableMouseCalls();
// 0x0026eb40
extern IDirect3DDevice9* g_gameD3DDevice;
// 0x0026eb28
extern int g_gameVersion;
// R5 address to verify
extern DWORD g_gameMouseDummy;
// 0x00113930
extern BYTE g_gameKeyCall[5];
// 0x00113938
extern BYTE g_gameMouseCallEU[5];
// 0x00113940
extern BYTE g_gameMouseCallUS[5];

// Physical native bank ends at the following enable flag. The enabled lookup
// does not check indices; adjacency outside this bank is not proven equivalent.
// R5 address to verify
extern void* g_gameCustomModelInfo[32768];
// 0x001a25ac
extern DWORD g_gameUseCustomModelInfo;
// 0x00114b08
extern void** g_gameModelInfoTable;
// Opaque native storage cleared immediately before the established model bank.
// R5 address to verify
extern DWORD g_gameCustomModelPrefix[32767];
#pragma pack(push, 1)
struct GameModelReferencePatch
{
    DWORD address;
    BYTE opcode;
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(GameModelReferencePatch) == 5);
// Addresses stored in these records are GTA SA virtual addresses.
// 0x00114b10
extern const GameModelReferencePatch g_gameModelReferencePatchesUS[707];
// 0x001158e0
extern const GameModelReferencePatch g_gameModelReferencePatchesEU[707];
// 0x000a7970
BOOL GamePatchModelInfoReferences();
// 0x000a7a00
BOOL GameInitializeCustomModelInfo();
// 0x000b6850
void* __cdecl GameCreateCollisionModel();
// 0x000b6880
BOOL __cdecl GameLoadCollisionData(const void* data, DWORD length, void* collision);
// Raw plugin callback: only its first two stack words are consumed by native code.
// 0x000a7cc0
DWORD __cdecl GameReadCustomCollision(void* stream, DWORD length);
// 0x000a7d90
void GameRegisterCustomCollisionPlugin();
// 0x000eccd0
extern const char g_customCollisionWrongType[];
// 0x000ecca4
extern const char g_customCollisionLoadFailed[];
// 0x000ecc70
extern const char g_customCollisionAllocationFailed[];
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameCurrentInteriorCommand;

// 0x000ec56c
extern const Scripting::OpcodeInfo g_gameRequestModelCommand;
// 0x000ec580
extern const Scripting::OpcodeInfo g_gameLoadModelsCommand;
// 0x000ec5a8
extern const Scripting::OpcodeInfo g_gameModelAvailableCommand;
// 0x000ec520
extern const char g_gameModelTimeoutWarning[41];
// 0x000a7a40
void* __stdcall GameGetModelInfo(int model);
// Observed raw DWORD at model-info +0x1c. No additional null guard is native.
// 0x000b45a0
DWORD __stdcall GameGetModelData1c(int model);
// 0x000b3db0
BOOL __stdcall GameHasModelInfo(int model);
// 0x000b3dd0
BOOL __stdcall GameIsPedModel(int model);
// Observed wrapped vtable-address-plus-14 test, not a slot-content query.
// 0x000b4570
BOOL __stdcall GameHasModelInfoForObject(int model);
struct CVector;
// 0x000b63b0
float __cdecl GameSetModelCollisionRadius(int model, float radius);
// 0x000b6410
BOOL __cdecl GameGetModelCollisionCentre(int model, CVector* centre);
// 0x000b64a0
BYTE __stdcall GameGetModelFlagsByte(int model);
// 0x000b64c0
WORD __stdcall GameGetModelFlagsWord(int model);
// Raw EAX from model-info vtable +2C, gated by the existing game pointer.
// 0x000b6250
DWORD __cdecl GameQueryModelVirtual2c(int model);
// 0x000b4550
void __stdcall GameSetModelDrawDistance(void* info, float distance);
// R5 address to verify
float __stdcall GameGetModelDrawDistance(const void* info);
// 0x000b4600
void __stdcall GameSetModelRenderObjectCall3c(int model, void* object);
// 0x000b4630
void __stdcall GameSetModelCollision(int model, void* collision);
// 0x000b4680
DWORD __stdcall GameGetModelTxdIndex(int model);
// 0x000b46d0
void* __stdcall GameGetModelCollision(int model);
// 0x000b4700
WORD __stdcall GameGetModelReferenceCount(int model);
// 0x000b2040
BOOL GameCleanupModelAndTxd(int model);
// R5 address to verify
void GameClearModelCollision(int model);
// 0x000b4100
DWORD __cdecl GameReadPedModelTable28(int model);
// R5 address to verify
DWORD __cdecl GameReadPedModelTable2c(int model);
// 0x000b4180
DWORD __cdecl GameReadPedModelTable24(int model);

// 0x00150334
extern char* g_gameTextBuffer;

// 0x000aa470
void GameWaitUnprotect(BYTE* address, SIZE_T size, BYTE expected);
// 0x000aa710
BOOL GameInitializeVersionPatches();
// 0x000a5eb0
DWORD __cdecl GameFrameNodeNameHook(void* arg0, DWORD length, void* arg2);
// 0x000a5f30
DWORD __cdecl GameDataReadHook(void* arg0, DWORD length, void* arg2);
// 0x000b1e40
DWORD GameReadStream(void* stream, void* buffer, DWORD length);

// R5 address to verify
extern DWORD g_gamePhysicalMegabytes;
// R5 address to verify
extern DWORD g_gameStreamingBytes;
// R5 address to verify
extern const char g_gameStartupTitle[6];
// R5 address to verify
extern char g_gameFrameNodeError[256];
// 0x0015181c
extern bool g_gameCustomStreamRead;
// R5 address to verify
extern void* g_gameStreamBuffer;
// R5 address to verify
extern BYTE g_frameKeyCycle;
// R5 address to verify
extern DWORD g_frameKeyCycleTick;
// R5 address to verify
extern DWORD g_frameSampleCount;
// R5 address to verify
extern DWORD g_frameSampleTick;
// R5 address to verify
extern DWORD g_frameSleepTick;
// R5 address to verify
extern float g_frameScale;
// 0x000e6148
extern const float g_frameScaleMultiplier;
// 0x000ea63c
extern const float g_frameScaleNumerator;
// 0x000b61d0
void GameSetVideoMode(DWORD mode);

// R5 address to verify
extern const Scripting::OpcodeInfo g_gameCreateWeaponPickupCommand;

// GTA backend collision adapters; pointers describe observed prefixes, not complete GTA types.
// 0x000b41f0
int __cdecl GameCameraSphereQuery(const void* sphere);
// 0x000b65d0
BYTE __stdcall GameTestEntityLineSphere(const CVector* origin, const CVector* end, void* backend, const CVector* centre, float scale, BYTE cameraCheck);
// R5 address to verify
extern BYTE g_gameLineSphereResult;

// R5 address to verify
extern const Scripting::OpcodeInfo g_gameReleaseAnimationCommand;
// R5 address to verify
extern const float g_gameFarClipBase;
// R5 address to verify
extern const float g_gameFarClipMinimum;
// R5 address to verify
extern float g_gameFarClipDistance;
// 0x000ec9a4
extern const char g_gameWeaponName01[15];
// 0x000ec998
extern const char g_gameWeaponName02[10];
// 0x000ec98c
extern const char g_gameWeaponName03[11];
// 0x000ec984
extern const char g_gameWeaponName04[6];
// 0x000ec974
extern const char g_gameWeaponName05[13];
// 0x000ec96c
extern const char g_gameWeaponName06[7];
// 0x000ec960
extern const char g_gameWeaponName07[9];
// 0x000ec958
extern const char g_gameWeaponName08[7];
// 0x000ec94c
extern const char g_gameWeaponName09[9];
// 0x000ec944
extern const char g_gameWeaponName10[6];
// 0x000ec938
extern const char g_gameWeaponName12[9];
// 0x000ec930
extern const char g_gameWeaponName14[8];
// 0x000ec928
extern const char g_gameWeaponName15[5];
// 0x000ec920
extern const char g_gameWeaponName16[8];
// 0x000ec918
extern const char g_gameWeaponName17[8];
// 0x000ec910
extern const char g_gameWeaponName18[8];
// 0x000ec908
extern const char g_gameWeaponName22[8];
// 0x000ec8f8
extern const char g_gameWeaponName23[16];
// 0x000ec8e8
extern const char g_gameWeaponName24[13];
// 0x000ec8e0
extern const char g_gameWeaponName25[8];
// 0x000ec8cc
extern const char g_gameWeaponName26[17];
// 0x000ec8bc
extern const char g_gameWeaponName27[15];
// 0x000ec8b8
extern const char g_gameWeaponName28[4];
// 0x000ec8b4
extern const char g_gameWeaponName29[4];
// 0x000ec8ac
extern const char g_gameWeaponName30[5];
// 0x000ec8a8
extern const char g_gameWeaponName31[3];
// 0x000ec8a0
extern const char g_gameWeaponName32[5];
// 0x000ec898
extern const char g_gameWeaponName33[6];
// 0x000ec888
extern const char g_gameWeaponName34[13];
// 0x000ec878
extern const char g_gameWeaponName35[16];
// 0x000ec86c
extern const char g_gameWeaponName36[12];
// 0x000ec85c
extern const char g_gameWeaponName37[13];
// 0x000ec854
extern const char g_gameWeaponName38[8];
// 0x000ec840
extern const char g_gameWeaponName39[19];
// 0x000ec838
extern const char g_gameWeaponName40[5];
// 0x000ec82c
extern const char g_gameWeaponName41[10];
// 0x000ec818
extern const char g_gameWeaponName42[18];
// 0x000ec810
extern const char g_gameWeaponName43[7];
// 0x000ec804
extern const char g_gameWeaponName46[10];
// 0x000ec7fc
extern const char g_gameWeaponName49[8];
// 0x000ec7f4
extern const char g_gameWeaponName53[8];
// 0x000ec7ec
extern const char g_gameWeaponName54[6];

// Shared native name storage; only the first8 bytes are reset/read by helpers.
// R5 address to verify
extern char g_gameEightByteName[9];
// R5 address to verify
extern BYTE g_gameTimerOverridden;
// R5 address to verify
void GameSetEightByteName(const char* name);
// R5 address to verify
bool GameCopyEightByteName(void* output);
// Backing storage returned by the installed GTA preview hook.
// R5 address to verify
extern DWORD g_gamePreviewPed[488];

// Unchecked index; native reloads selected table after the real CRT free.
// R5 address to verify
void GameFreeModelInfo(int model);
// R5 address to verify
void GameLoadRequestedModelsAndEndThread();

#endif // GAME_H
