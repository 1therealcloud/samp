#ifndef SAMP_RECOVERED_PED_H
#define SAMP_RECOVERED_PED_H
#include "game/entity.h"
#include "game/scripting.h"

struct CameraAim;
#pragma pack(push, 1)
// Observed firing-data extent; the first DWORD is not copied by CPed::SetBulletData.
struct PedBulletData
{
    DWORD unknown0;
    CVector origin, target, offset;
    void* entity;
};
static_assert(sizeof(PedBulletData) == 44);
struct PedAccessoryState
{
    int model, bone;
    CVector offset, rotation, scale;
    DWORD materialColor1, materialColor2;
};
static_assert(sizeof(PedAccessoryState) == 52);
// samp.dll vtable RVA: 0x0011f24c
class CPed : public CEntity
{
public:

    PedBulletData* GetBulletData();

    WORD GetCurrentWeaponTotalAmmo();

    void ProcessCameraAimMarker();

    void ProcessVehicleHorn();

    void SetWeaponSkill(int skill, WORD value);

    void UpdateClumpAnimations(float timeStep, DWORD onScreen);

    BYTE* GetControlFlags();
    // Descriptive distance between mapped bone slots5 and6; result remains in ST0.

    float GetArmBoneDistance();

    void SetBulletData(PedBulletData* data);

    void GiveWeapon(int weapon, int ammo);

    void EnableGoggles();
    // Returns the raw GTA hit entity; the output stays untouched on a miss.

    void* GetAimHit(CVector* position);

    void GetCameraAimRay(CVector* origin, CVector* target);

    void* GetCameraAimHit(CVector* position);
    // Preserves native publication/clearing of shared firing context; always returns TRUE.

    BOOL FireWeapon();

    void DrawBoneIds();

    void DrawMappedBoneIds();

    void DrawAnimationFrameIds();

    void UpdateAndDrawBoneIds();

    void ProcessAimMarker();

    void ClearAimTarget();
    // Opaque task fields, not an established complete GTA animation-task class.

    DWORD PackAnimationTask(const void* task);

    DWORD PackQueuedAnimation();

    void* FindAnimationTask();

    DWORD GetPackedAnimation();

    void ApplyPackedAnimation(DWORD packed);

    int GetBoneId(int bone);
    // Raw hierarchy field +0; higher-level flag meaning is not assumed.

    DWORD GetBoneHierarchyFlags();

    void UpdateBoneMatrices();
    // Descriptive backend-name accessor, distinct from the global shopping subsection.

    const char* GetBackendShoppingName();

    void SetBackendShoppingName(const char* name);

    void GetAimOffsetMatrix(CMatrix* matrix);
    // Native always returns TRUE; invalid output/backend paths are not repaired.

    BOOL GetWeaponAimOffsets(BOOL alternate, CVector* position, CVector* weaponOffsetOut);
    // Descriptive task-state name; original numeric return categories retained.

    int GetVehicleTransitionTaskState();

    BOOL HasLeaveCarTask();

    BOOL IsDucking();

    void ClearTasksIfOnFoot();

    void ClearDuckingFlag();

    BOOL HasJumpTask();

    BOOL HasFightTask();

    BOOL HasFallAndGetUpTask();
    // Raw backend +500 or wrapper +2e6 gate; existing field names retained.

    BOOL HasAimTarget();

    void SetSecondaryPoseState(DWORD state);

    void ProcessSecondaryPoseWeapon();
    // Preserves the full state DWORD, not only a normalized BOOL.

    void SetDancePoseState(DWORD state);

    void ProcessDancePoseWeapon();

    BOOL HasGangDrivebyTask();

    void StopGangDrivebyTask();

    void SetMoney(int amount);

    void CheckParachuteWeapon();

    BOOL HasNamedAnimationTask();

    DWORD GetFightingStyle();

    DWORD GetTransformedBonePosition(DWORD first, DWORD second);

    void ResetLocalControlWord();

    void ApplyClumpAnimation(int mode, DWORD first, DWORD second, DWORD third);

    int GetBoneCount();

    CMatrix* GetBoneMatrices();

    void SetBoneMatrixById(const CMatrix* matrix, int bone);

    void SetBoneMatrix(const CMatrix* matrix, int bone);

    float GetRootPoseFactor();

    void ProcessDancing();

    void GiveStuff(int type);

    BYTE GetDeathInfo(WORD* killer);

    BOOL EnablePassengerDrivebyMode();

    void SetWalkStyle(const char* name);

    BOOL OpenParachute();

    void ProcessParachuteEvent(const char* name);

    BOOL IsOnScreen();

    void* GetContactEntity();

    void* GetContactVehicle();

    void SetCameraMode(BYTE mode);

    void SetCameraExtZoomAndAspectRatio(float zoom, float aspect);

    BYTE GetCameraMode();

    BOOL m_bUsingCellphone;
    BOOL m_accessoryOccupied[10];
    PedAccessoryState m_accessoryInfo[10];
    void* m_accessoryObjects[10];
    // Raw backend accessor; name follows0.2x; class binding inferred from verified2A4 layout and adjacent accessory entry.

    void* GetGtaActor();
    void* m_pGamePed;
    DWORD unknown2a8, unknown2ac;
    BYTE m_nPlayerNumber;
    BYTE unknown2b1[8];
    DWORD m_parachuteObject;
    BYTE m_urinationEffectHandle[4];
    DWORD m_stuffType, m_stuffObject, m_drunkLevel, m_arrow;
    BYTE unknown2d1;
    DWORD m_dancing;
    BYTE m_danceMoveState[8];
    BOOL m_bDancing;
    DWORD m_danceStyle, m_lastDanceMove;
    BYTE unknown2ea[8];
    BOOL m_bUrinating;
    BYTE m_blockPedInputAt1C, m_blockPedInputAt24, m_blockPedInputAt20;
    DWORD m_aimMarker;
    BYTE m_bulletData[48];

    CPed();

    CPed(BYTE player, int model, float x, float y, float z, float rotation);

    void ProcessDrunk();

    void SetKeys(WORD mask, WORD leftRight, WORD upDown);

    WORD GetKeys(WORD* leftRight, WORD* upDown);

    BOOL ApplyStuff();

    void PutIntoVehicle(int vehicle, int seat);

    void StartUrinating();

    void EnterVehicle(int vehicle, BOOL passenger);

    void* GetFloor();

    void Kill();

    void ExtinguishPedAndVehicleFire();

    void SetControllable(BOOL enabled);

    void LinkToInterior(BYTE interior, BOOL refresh);

    const char* GetAnimationForDance(int move);

    DWORD GetModelTable24();

    DWORD GetModelTable28();

    void* GetCurrentWeaponInfo();

    float GetRotation();

    void StartDancing(int style);

    BOOL IsPerformingAnimation(const char* name);

    void ApplyAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time);

    void SetModelIndex(int model);

    void ExitVehicle();

    void StopUrinating();

    int GetVehicleSeatIndex();

    void CreateArrow(DWORD color);

    void DestroyParachute();

    float GetDistanceToEntity(CEntity* entity);

    void AddAccessory(int index, const PedAccessoryState* info);

    void DeleteAccessory(int index);

    void SetRotation(float rotation);

    void ForceRotation(float rotation);

    void DisableJetpack();

    BOOL DoesHandsUp();

    void DestroyCommandTask();

    void ApplyCommandTask(const char* name, int p1, int p2, int p3, const CVector* p4, int p5, float p6, int p7, int p8, int p9);

    void SetAimZ(float value);

    void HoldObject(DWORD object);

    void SetImmunities(DWORD bullet, DWORD fire, DWORD explosion, DWORD collision, DWORD melee);

    void HandsUp();

    void EnableJetpack();

    void ClearWeapons();

    DWORD GetBonePosition(int bone, CVector* position);

    void RemoveWeaponModel(int weapon);

    BOOL CurrentWeaponHasAmmo();

    float GetAimZ();

    void StopDancing();

    BOOL HasJetpack();

    BOOL PerformingCustomAnimation();

    BYTE GetAim();
    // Native forwarding entry to the existing internal camera-aim accessor.

    CameraAim* GetCameraAim();

    void SetWeaponAmmo(BYTE weapon, WORD ammo);

    void* GetWeaponSlot(DWORD weapon);

    BOOL IsDead();

    void SetSpawnInfo(const CVector* position, float rotation);

    void LoadShoppingDataSubsection(const char* name);

    void DeleteArrow();

    void EnableCellphone(BOOL enabled);

    BOOL GetAccessoryState(int index);

    void SetFightingStyle(int style);

    void ResetDamageEntity();

    void* GetAccessory(int index);

    PedAccessoryState* GetAccessoryInfo(int index);

    void GetBoneMatrixById(CMatrix* out, int bone);
    // Accesses the backend bone table without bounds/null checks before forwarding.

    void GetBoneMatrix(CMatrix* out, int bone);
    // Raw animation-frame quaternion; the bone index is unchecked.

    void* GetBoneQuaternion(int bone);

    void ApplySecondaryPose();

    void ApplyDancePose();

    void ProcessAnimationPose();

    void ProcessAccessories();

    void* GetAimedPed();

    void RemoveWeaponWhenEnteringVehicle();

    const char* GetLoadedShoppingDataSubsection();

    void SetInitialState();

    void SetAim(const CameraAim* aim);

    void* GetCurrentWeaponSlot();

    BOOL IsOnGround();

    void SetState(BYTE state);

    float GetArmour();

    float GetHealth();

    DWORD GetVehicleRef();

    void SetFlags(DWORD flags);

    void SetArmour(float armour);

    void SetHealth(float health);

    void ResetPointers();

    DWORD GetFlags();

    BOOL DoesDancing();

    void SetDrunkLevel(DWORD level);

    void* GetVehicle();

    BYTE GetState();

    BOOL DoesUrinating();

    DWORD GetHeldObjectType();

    BOOL UsingCellphone();

    virtual ~CPed();

    void Destroy();

    BOOL HasAccessory();

    void DeleteAllAccessories();

    void DropStuff();

    void WarpFromVehicle(float x, float y, float z);

    BOOL IsInVehicle();

    BOOL IsPassenger();

    DWORD GetDrunkLevel();
    // Observed task-type test; does not establish a complete GTA task interface.

    BOOL HasInWaterTask();

    BOOL HasDanceState();

    BOOL HasDanceStyle();

    DWORD GetCurrentWeapon();

    void SetArmedWeapon(DWORD weapon, BYTE direct);
};
#pragma pack(pop)
static_assert(sizeof(CPed) == 0x32d);
static_assert(offsetof(CPed, m_pGamePed) == 0x2a4);
static_assert(offsetof(CPed, m_nPlayerNumber) == 0x2b0);

extern void* g_gamePlayerPeds[210];

extern BYTE g_gamePlayerControllerState[210][16];

void* __stdcall GameFindLocalPed();

void __stdcall GameAssignPlayerPed(BYTE player, void* ped);

void* __stdcall GamePoolPedGetAt(int handle);

extern void* g_pedCommandTaskSnapshot;

extern const float g_pedFloorRayLength;

extern DWORD g_pedVehicleEntryFlag;

extern DWORD g_pedDrunkVehicleTick;

extern const float g_pedDrunkShakeScale;

extern const float g_pedDrunkSpeedThreshold;
#endif

BOOL __stdcall GameIsBackendOnScreen(void* backend);

// Opaque player flag table; factory-selected slots are2..209.
// Byte indices210..255 reach the verified adjacent gap/cache/ped storage.

extern DWORD g_gamePlayerFlags[210];

void __stdcall GameSetPlayerFlag(BYTE player, DWORD value);

extern const Scripting::OpcodeInfo g_pedCreatePlayerCommand;

extern const Scripting::OpcodeInfo g_pedGetPlayerHandleCommand;

extern const Scripting::OpcodeInfo g_pedCommand03fe;

extern const float g_pedSpawnLift;

extern const Scripting::OpcodeInfo g_pedBehaviorWalkStyleCommand;

extern const Scripting::OpcodeInfo g_pedBehaviorDrivebyCommand;

extern const Scripting::OpcodeInfo g_pedBehaviorCreateObjectCommand;

extern const Scripting::OpcodeInfo g_pedBehaviorAttachCommand;

extern const Scripting::OpcodeInfo g_pedBehaviorObjectFlagCommand;

extern const Scripting::OpcodeInfo g_pedBehaviorObjectScalarCommand;

extern const Scripting::OpcodeInfo g_pedBehaviorObjectAnimationCommand;

extern const Scripting::OpcodeInfo g_pedBehaviorEffectCommand;

extern const char g_pedParachuteOpen[10];

extern const char g_pedParachuteDecel[11];

extern const char g_pedParachuteFloat[11];

extern const char g_pedParachuteSteerRight[12];

extern const char g_pedParachuteSteerLeft[12];

extern const char g_pedParachuteLandWater[16];

extern const char g_pedParachuteOpenObject[12];

extern const char g_pedParachuteDecelObject[13];

extern const char g_pedParachuteFloatObject[13];

extern const char g_pedParachuteSteerRightObject[14];

extern const char g_pedParachuteSteerLeftObject[14];

extern const char g_pedParachuteLandWaterObject[18];

extern const double g_pedParachuteOne;

extern const double g_pedParachuteOpeningSpeed;

extern const double g_pedDrivebyDistance;

extern const double g_pedStuffOffset1X;

extern const double g_pedStuffOffset1Y;

extern const double g_pedStuffOffset1Z;

extern const double g_pedStuffOffset2Z;

extern const double g_pedStuffOffset3X;

extern const double g_pedStuffOffset3Y;

extern const double g_pedStuffOffset3Z;

extern char g_pedShoppingName[32];

extern CPed* g_pedFiringContext;

extern void* g_pedFiringData;

extern const char g_pedBoneIdFormat[3];

BYTE* __stdcall GameGetPlayerControlFlags(BYTE player);

int __stdcall GameGetWeaponModel(int weapon);

void GameInitializePlayerPedTables();
// Native has no bound check;210..255 reaches adjacent controller state.

void* __stdcall GameGetPlayerPedUnchecked(BYTE player);

void __stdcall GameSetPlayerControllerWords(BYTE player, DWORD first, DWORD second, DWORD third, DWORD fourth);

BYTE __stdcall GameFindPlayerIndexByClump(void* clump);

DWORD __stdcall GameGetPlayerFlagsUnchecked(BYTE player);

// Shared raw raster snapshot words (+14/+0C), retaining native table adjacency.

extern DWORD g_sceneRasterDepthAndWidth[2];
