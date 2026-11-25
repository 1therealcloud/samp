#ifndef GAME_H
#define GAME_H

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

    int GetScreenWidth();
    // GTA SA VA: 0x00c17048; same raw signed screen metric as the 0.2x wrapper.

    int GetScreenHeight();
    // GTA SA VA: 0x00859524; returns the raw HUD vertical scale in ST0.

    float GetHudVertScale();
    // GTA SA VA: 0x00859520; returns the raw HUD horizontal scale in ST0.

    float GetHudHorizScale();

    int GetCursorMode();
    // Clears the delay only; no immediate cursor/input backend calls.

    void ResetInputEnableWait();
    // Stores the raw BOOL DWORD without normalization.

    void SetHeadMovement(BOOL enabled);
    // Raw DWORD setters; no marker creation/deletion or bool normalization.

    void SetCheckpointEnabled(DWORD enabled);

    void SetRacingCheckpointEnabled(DWORD enabled);
    // Uses the native preview-ped buffer and a temporary vehicle when requested.

    void ApplyPreviewAnimation(DWORD first, DWORD second, BOOL useVehicle, int model, DWORD color);

    void SetTimeScale(float scale);
    // Native free means occupied DWORD !=1, not only zero.

    BYTE FindFreePlayerSlot();

    BYTE CountUsedPlayerSlots();

    void SetEightByteName(const char* name);
    // Virtual scalar deletion; no pool-slot bookkeeping is performed.

    BOOL DeleteEntity(CEntity* entity);
    // Native forwarding entry to the recovered weapon-model switch.

    int GetWeaponModel(int weapon);
    // Native key index is unchecked, including negative/out-of-range values.

    BOOL IsKeyPressed(int key);
    // Invalid IDs and already-loaded models return TRUE without streaming calls.

    BOOL LoadModelNow(int model);

    DWORD GetWeather();

    BYTE GetShowHudPreference();

    BOOL IsVSyncEnabled();

    void EnableVSync();

    void SetTimer(DWORD value);

    DWORD GetRenderedBuildingCount();
    // Native reserves slots2..209; sixth argument is ignored.

    CPed* CreatePed(int model, CVector position, float rotation, DWORD unused, BOOL playerFlag);

    void ReleaseAnimation(const char* name);

    void EnableRadar(BOOL enabled);

    void UpdateFarClippingPlane();

    const char* GetWeaponName(int weapon);

    void Restart();

    void SetNumberOfIntroTextLinesThisFrame(WORD value);

    int GetTimer();

    BOOL UsingGamepad();

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

    CGame();

    void LoadAnimationsAndModels();

    void ReleaseModel(int model, BYTE force);

    void SetRequiredVehicleModels(BYTE* priorities);

    void DrawGangZone(const float* position, DWORD color);

    DWORD CreatePickup(int model, int type, float x, float y, float z, DWORD* index);

    DWORD CreateWeaponPickup(int model, int ammo, float x, float y, float z);

    void DisplayGameText(const char* text, int time, int style);

    void ProcessCheckpoints();

    void SetRacingCheckpoint(BYTE type, const CVector* position, const CVector* next, float size);

    void CreateRacingCheckpoint();

    void SetCheckpoint(const CVector* position, const CVector* size);

    DWORD CreateMarker(int icon, float x, float y, float z, DWORD color, int type);

    void DeleteRacingCheckpoint();

    void ResetMoney();

    BOOL RemovePed(CPed* ped);

    CVehicle* CreateVehicle(int model, float x, float y, float z, float rotation, BOOL siren);

    void RefreshRenderer(float x, float y);

    float FindGroundZ(float x, float y, float z);

    void DisableEnterExits();

    void DisableAutoAiming();

    void* GetWeaponInfo(int weapon, int skill);

    void LoadCullZone(const char* name);

    void LoadCollisionFile(const char* name);

    void LoadScene(const char* name);

    void SetWorldGravity(float gravity);

    void EnableStuntBonus(BYTE enabled);

    void EnableZoneDisplaying(BYTE enabled);

    void IncreasePlayerMoney(int amount);

    void DeleteMarker(DWORD marker);

    DWORD IsAnimationLoaded(const char* name);

    void RequestAnimation(const char* name);

    void DisableTrainTraffic();

    void GetWorldTime(BYTE* hour, BYTE* minute);

    void SetWantedLevel(BYTE level);

    DWORD GetStreamingMemory();

    DWORD GetUsedMemory();

    int GetPlayerMoney();
    // Technical fixed-table scan; full backend record meaning remains unresolved.

    int CountBackendVehicleModelRecords();

    void SetMaxStats();

    void LetTimeGo(char enabled);

    void SetWorldWeather(int weather);

    void SetWorldTime(int hour, int minute);

    CPed* GetPlayerPed();

    void InitGame();

    void StartGame();
    // Native consumes two stack arguments but serializes only model.

    void RequestModel(int model, int loadingStream);

    void LoadRequestedModels();

    BOOL IsModelAvailable(int model);

    BOOL IsMenuVisible();

    BYTE GetCurrentInterior();

    void SetCursorMode(int, BOOL);

    void ProcessInputEnabling();

    IDirect3DDevice9* GetDevice();

    BOOL IsStarted();

    void SetFrameLimiter(int);

    void ProcessFrameLimiter();

    void UpdateKeyCycle();

    void Sleep(int elapsed, int limit);

    void EnableClock(bool);

    void* GetWindowHandle();

    CAudio* GetAudio();

    CCamera* GetCamera();

    BOOL DoesHeadMoves();

    void EnableHUD(BOOL enabled);
};
#pragma pack(pop)
static_assert(sizeof(CGame) == 0x142);
static_assert(sizeof(GameCheckpointState) == 0x49);
static_assert(offsetof(GameCheckpointState, marker) == 0x45);
static_assert(offsetof(GameCheckpointState, raceMarker) == 0x21);
static_assert(offsetof(CGame, m_checkpoints) == 0x0c);
static_assert(offsetof(CGame, m_nTrackedNameDigestMatches) == 0x55);
static_assert(offsetof(CGame, m_nCursorMode) == 0x61);
static_assert(offsetof(CGame, m_nInputEnableWaitFrames) == 0x65);
static_assert(offsetof(CGame, m_bClockEnabled) == 0x69);

extern BOOL g_gameUsedPlayerSlots[210];

void UnFuck(void* address, SIZE_T size);

void GameClearMouseDelta();

void GameUpdateMouse();

void GameDisableMousePointers();

void GameRestoreMousePointers();

void GameDisableMouseCalls();

extern IDirect3DDevice9* g_gameD3DDevice;

extern int g_gameVersion;

extern DWORD g_gameMouseDummy;

extern BYTE g_gameKeyCall[5];

extern BYTE g_gameMouseCallEU[5];

extern BYTE g_gameMouseCallUS[5];

// Physical native bank ends at the following enable flag. The enabled lookup
// does not check indices; adjacency outside this bank is not proven equivalent.

extern void* g_gameCustomModelInfo[32768];

extern DWORD g_gameUseCustomModelInfo;

extern void** g_gameModelInfoTable;
// Opaque native storage cleared immediately before the established model bank.

extern DWORD g_gameCustomModelPrefix[32767];
#pragma pack(push, 1)
struct GameModelReferencePatch
{
    DWORD address;
    BYTE opcode;
};
#pragma pack(pop)
static_assert(sizeof(GameModelReferencePatch) == 5);
// Addresses stored in these records are GTA SA virtual addresses.

extern const GameModelReferencePatch g_gameModelReferencePatchesUS[707];

extern const GameModelReferencePatch g_gameModelReferencePatchesEU[707];

BOOL GamePatchModelInfoReferences();

BOOL GameInitializeCustomModelInfo();

void* __cdecl GameCreateCollisionModel();

BOOL __cdecl GameLoadCollisionData(const void* data, DWORD length, void* collision);
// Raw plugin callback: only its first two stack words are consumed by native code.

DWORD __cdecl GameReadCustomCollision(void* stream, DWORD length);

void GameRegisterCustomCollisionPlugin();

extern const char g_customCollisionWrongType[];

extern const char g_customCollisionLoadFailed[];

extern const char g_customCollisionAllocationFailed[];

extern const Scripting::OpcodeInfo g_gameCurrentInteriorCommand;

extern const Scripting::OpcodeInfo g_gameRequestModelCommand;

extern const Scripting::OpcodeInfo g_gameLoadModelsCommand;

extern const Scripting::OpcodeInfo g_gameModelAvailableCommand;

extern const char g_gameModelTimeoutWarning[41];

void* __stdcall GameGetModelInfo(int model);
// Observed raw DWORD at model-info +0x1c. No additional null guard is native.

DWORD __stdcall GameGetModelData1c(int model);

BOOL __stdcall GameHasModelInfo(int model);

BOOL __stdcall GameIsPedModel(int model);
// Observed wrapped vtable-address-plus-14 test, not a slot-content query.

BOOL __stdcall GameHasModelInfoForObject(int model);
struct CVector;

float __cdecl GameSetModelCollisionRadius(int model, float radius);

BOOL __cdecl GameGetModelCollisionCentre(int model, CVector* centre);

BYTE __stdcall GameGetModelFlagsByte(int model);

WORD __stdcall GameGetModelFlagsWord(int model);
// Raw EAX from model-info vtable +2C, gated by the existing game pointer.

DWORD __cdecl GameQueryModelVirtual2c(int model);

void __stdcall GameSetModelDrawDistance(void* info, float distance);

float __stdcall GameGetModelDrawDistance(const void* info);

void __stdcall GameSetModelRenderObjectCall3c(int model, void* object);

void __stdcall GameSetModelCollision(int model, void* collision);

DWORD __stdcall GameGetModelTxdIndex(int model);

void* __stdcall GameGetModelCollision(int model);

WORD __stdcall GameGetModelReferenceCount(int model);

BOOL GameCleanupModelAndTxd(int model);

void GameClearModelCollision(int model);

DWORD __cdecl GameReadPedModelTable28(int model);

DWORD __cdecl GameReadPedModelTable2c(int model);

DWORD __cdecl GameReadPedModelTable24(int model);

extern char* g_gameTextBuffer;

void GameWaitUnprotect(BYTE* address, SIZE_T size, BYTE expected);

BOOL GameInitializeVersionPatches();

DWORD __cdecl GameFrameNodeNameHook(void* arg0, DWORD length, void* arg2);

DWORD __cdecl GameDataReadHook(void* arg0, DWORD length, void* arg2);

DWORD GameReadStream(void* stream, void* buffer, DWORD length);

extern DWORD g_gamePhysicalMegabytes;

extern DWORD g_gameStreamingBytes;

extern const char g_gameStartupTitle[6];

extern char g_gameFrameNodeError[256];

extern bool g_gameCustomStreamRead;

extern void* g_gameStreamBuffer;

extern BYTE g_frameKeyCycle;

extern DWORD g_frameKeyCycleTick;

extern DWORD g_frameSampleCount;

extern DWORD g_frameSampleTick;

extern DWORD g_frameSleepTick;

extern float g_frameScale;

extern const float g_frameScaleMultiplier;

extern const float g_frameScaleNumerator;

void GameSetVideoMode(DWORD mode);

extern const Scripting::OpcodeInfo g_gameCreateWeaponPickupCommand;

// GTA backend collision adapters; pointers describe observed prefixes, not complete GTA types.

int __cdecl GameCameraSphereQuery(const void* sphere);

BYTE __stdcall GameTestEntityLineSphere(const CVector* origin, const CVector* end, void* backend, const CVector* centre, float scale, BYTE cameraCheck);

extern BYTE g_gameLineSphereResult;

extern const Scripting::OpcodeInfo g_gameReleaseAnimationCommand;

extern const float g_gameFarClipBase;

extern const float g_gameFarClipMinimum;

extern float g_gameFarClipDistance;

extern const char g_gameWeaponName01[15];

extern const char g_gameWeaponName02[10];

extern const char g_gameWeaponName03[11];

extern const char g_gameWeaponName04[6];

extern const char g_gameWeaponName05[13];

extern const char g_gameWeaponName06[7];

extern const char g_gameWeaponName07[9];

extern const char g_gameWeaponName08[7];

extern const char g_gameWeaponName09[9];

extern const char g_gameWeaponName10[6];

extern const char g_gameWeaponName12[9];

extern const char g_gameWeaponName14[8];

extern const char g_gameWeaponName15[5];

extern const char g_gameWeaponName16[8];

extern const char g_gameWeaponName17[8];

extern const char g_gameWeaponName18[8];

extern const char g_gameWeaponName22[8];

extern const char g_gameWeaponName23[16];

extern const char g_gameWeaponName24[13];

extern const char g_gameWeaponName25[8];

extern const char g_gameWeaponName26[17];

extern const char g_gameWeaponName27[15];

extern const char g_gameWeaponName28[4];

extern const char g_gameWeaponName29[4];

extern const char g_gameWeaponName30[5];

extern const char g_gameWeaponName31[3];

extern const char g_gameWeaponName32[5];

extern const char g_gameWeaponName33[6];

extern const char g_gameWeaponName34[13];

extern const char g_gameWeaponName35[16];

extern const char g_gameWeaponName36[12];

extern const char g_gameWeaponName37[13];

extern const char g_gameWeaponName38[8];

extern const char g_gameWeaponName39[19];

extern const char g_gameWeaponName40[5];

extern const char g_gameWeaponName41[10];

extern const char g_gameWeaponName42[18];

extern const char g_gameWeaponName43[7];

extern const char g_gameWeaponName46[10];

extern const char g_gameWeaponName49[8];

extern const char g_gameWeaponName53[8];

extern const char g_gameWeaponName54[6];

// Shared native name storage; only the first8 bytes are reset/read by helpers.

extern char g_gameEightByteName[9];

extern BYTE g_gameTimerOverridden;

void GameSetEightByteName(const char* name);

bool GameCopyEightByteName(void* output);
// Backing storage returned by the installed GTA preview hook.

extern DWORD g_gamePreviewPed[488];

// Unchecked index; native reloads selected table after the real CRT free.

void GameFreeModelInfo(int model);

void GameLoadRequestedModelsAndEndThread();

#endif // GAME_H
