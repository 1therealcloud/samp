#ifndef SAMP_RECOVERED_PED_H
#define SAMP_RECOVERED_PED_H
#include "game/entity.h"
#include "game/scripting.h"

#include <static_assert.h>

struct CameraAim;
#pragma pack(push, 1)
// Observed firing-data extent; the first DWORD is not copied by CPed::SetBulletData.
struct PedBulletData
{
    DWORD unknown0;
    CVector origin, target, offset;
    void* entity;
};
STATIC_ASSERT(sizeof(PedBulletData) == 44);
struct PedAccessoryState
{
    int model, bone;
    CVector offset, rotation, scale;
    DWORD materialColor1, materialColor2;
};
STATIC_ASSERT(sizeof(PedAccessoryState) == 52);
// samp.dll vtable RVA: 0x0011f24c
class CPed : public CEntity
{
public:
    // R5 address to verify
    PedBulletData* GetBulletData();

    // 0x000ad150
    WORD GetCurrentWeaponTotalAmmo();
    // 0x000b12f0
    void ProcessCameraAimMarker();

    // 0x000ae270
    void ProcessVehicleHorn();
    // 0x000ae450
    void SetWeaponSkill(int skill, WORD value);
    // 0x000ae530
    void UpdateClumpAnimations(float timeStep, DWORD onScreen);
    // R5 address to verify
    BYTE* GetControlFlags();
    // Descriptive distance between mapped bone slots5 and6; result remains in ST0.
    // R5 address to verify
    float GetArmBoneDistance();
    // 0x000af280
    void SetBulletData(PedBulletData* data);
    // 0x000af790
    void GiveWeapon(int weapon, int ammo);
    // 0x000afe60
    void EnableGoggles();
    // Returns the raw GTA hit entity; the output stays untouched on a miss.
    // 0x000af890
    void* GetAimHit(CVector* position);
    // 0x000afc70
    void GetCameraAimRay(CVector* origin, CVector* target);
    // 0x000b10e0
    void* GetCameraAimHit(CVector* position);
    // Preserves native publication/clearing of shared firing context; always returns TRUE.
    // 0x000afa70
    BOOL FireWeapon();
    // 0x000aef90
    void DrawBoneIds();
    // R5 address to verify
    void DrawMappedBoneIds();
    // 0x000af0f0
    void DrawAnimationFrameIds();
    // 0x000af1f0
    void UpdateAndDrawBoneIds();
    // 0x000b1190
    void ProcessAimMarker();
    // 0x000aff10
    void ClearAimTarget();
    // Opaque task fields, not an established complete GTA animation-task class.
    // 0x000ae5f0
    DWORD PackAnimationTask(const void* task);
    // 0x000ae6a0
    DWORD PackQueuedAnimation();
    // 0x000ae6e0
    void* FindAnimationTask();
    // 0x000ae790
    DWORD GetPackedAnimation();
    // 0x000ae7d0
    void ApplyPackedAnimation(DWORD packed);
    // 0x000ae960
    int GetBoneId(int bone);
    // Raw hierarchy field +0; higher-level flag meaning is not assumed.
    // 0x000ae990
    DWORD GetBoneHierarchyFlags();
    // 0x000aec90
    void UpdateBoneMatrices();
    // Descriptive backend-name accessor, distinct from the global shopping subsection.
    // 0x000ae330
    const char* GetBackendShoppingName();
    // 0x000ae380
    void SetBackendShoppingName(const char* name);
    // 0x000aeee0
    void GetAimOffsetMatrix(CMatrix* matrix);
    // Native always returns TRUE; invalid output/backend paths are not repaired.
    // 0x000af9c0
    BOOL GetWeaponAimOffsets(BOOL alternate, CVector* position, CVector* weaponOffsetOut);
    // Descriptive task-state name; original numeric return categories retained.
    // 0x000ac5d0
    int GetVehicleTransitionTaskState();
    // 0x000ac640
    BOOL HasTaskType704();
    // 0x000ac690
    BOOL IsDucking();
    // 0x000ac6c0
    void ClearTasksIfOnFoot();
    // 0x000ac720
    void ClearDuckingFlag();
    // 0x000acc10
    BOOL HasTaskType211();
    // R5 address to verify
    BOOL HasTaskType1016();
    // 0x000acc90
    BOOL HasTaskType208();
    // Raw backend +500 or wrapper +2e6 gate; existing field names retained.
    // R5 address to verify
    BOOL HasAimTarget();
    // 0x000ace50
    void SetSecondaryPoseState(DWORD state);
    // 0x000ace90
    void ProcessSecondaryPoseWeapon();
    // Preserves the full state DWORD, not only a normalized BOOL.
    // 0x000acf00
    void SetDancePoseState(DWORD state);
    // 0x000acf20
    void ProcessDancePoseWeapon();
    // 0x000ad030
    BOOL HasTaskType1022();
    // 0x000ad080
    void StopTaskType1022();
    // 0x000ad200
    void SetMoney(int amount);
    // 0x000ad440
    void CheckParachuteWeapon();
    // 0x000ada40
    BOOL HasTaskType401();
    // R5 address to verify
    DWORD GetBackendByte72D();
    // 0x000ae4d0
    DWORD CallBackend5E01C0(DWORD first, DWORD second);
    // R5 address to verify
    void ResetLocalControlWord();
    // 0x000ae560
    void ApplyClumpAnimation(int mode, DWORD first, DWORD second, DWORD third);
    // 0x000ae9e0
    int GetBoneCount();
    // 0x000aea30
    CMatrix* GetBoneMatrices();
    // 0x000aeb10
    void SetBoneMatrixById(const CMatrix* matrix, int bone);
    // R5 address to verify
    void SetBoneMatrix(const CMatrix* matrix, int bone);
    // 0x000af220
    float GetRootPoseFactor();
    // 0x000b00b0
    void ProcessDancing();
    // 0x000b02d0
    void GiveStuff(int type);
    // 0x000ac850
    BYTE GetDeathInfo(WORD* killer);
    // 0x000acf90
    BOOL EnablePassengerDrivebyMode();
    // 0x000ad1d0
    void SetWalkStyle(const char* name);
    // 0x000ad4d0
    BOOL OpenParachute();
    // 0x000ad620
    void ProcessParachuteEvent(const char* name);
    // 0x000abce0
    BOOL IsOnScreen();
    // 0x000ad9a0
    void* GetContactEntity();
    // 0x000ad9b0
    void* GetContactVehicle();
    // 0x00014340
    void SetCameraMode(BYTE mode);
    // 0x00014360
    void SetCameraExtZoomAndAspectRatio(float zoom, float aspect);
    // 0x00002cd0
    BYTE GetCameraMode();

    BOOL m_bUsingCellphone;
    BOOL m_accessoryOccupied[10];
    PedAccessoryState m_accessoryInfo[10];
    void* m_accessoryObjects[10];
    // Raw backend accessor; name follows0.2x; class binding inferred from verified2A4 layout and adjacent accessory entry.
    // R5 address to verify
    void* GetGtaActor();
    void* m_pGamePed;
    DWORD unknown2a8, unknown2ac;
    BYTE m_nPlayerNumber;
    BYTE unknown2b1[8];
    DWORD m_parachuteObject;
    BYTE unknown2bd[4];
    DWORD m_stuffType, m_stuffObject, m_drunkLevel, m_arrow;
    BYTE unknown2d1;
    DWORD unknown2d2;
    BYTE unknown2d6[8];
    BOOL m_bDancing;
    DWORD m_danceStyle, m_lastDanceMove;
    BYTE unknown2ea[8];
    BOOL m_bUrinating;
    BYTE unknown2f6, unknown2f7, unknown2f8;
    DWORD unknown2f9;
    BYTE unknown2fd[48];
    // 0x000aba70
    CPed();
    // 0x000b0ce0
    CPed(BYTE player, int model, float x, float y, float z, float rotation);
    // 0x000addf0
    void ProcessDrunk();
    // 0x000af340
    void SetKeys(WORD mask, WORD leftRight, WORD upDown);
    // 0x000af5d0
    WORD GetKeys(WORD* leftRight, WORD* upDown);
    // 0x000adca0
    BOOL ApplyStuff();
    // 0x000ac290
    void PutIntoVehicle(int vehicle, int seat);
    // 0x000ae100
    void StartUrinating();
    // 0x000ac410
    void EnterVehicle(int vehicle, BOOL passenger);
    // 0x000aca10
    void* GetFloor();
    // 0x000affd0
    void Kill();
    // 0x000ad0f0
    void CallBackend730AndVehicle490();
    // 0x000ac790
    void SetControllable(BOOL enabled);
    // 0x000ad340
    void LinkToInterior(BYTE interior, BOOL refresh);
    // 0x000adb70
    const char* GetAnimationForDance(int move);
    // 0x000ad9e0
    DWORD GetModelTable24();
    // R5 address to verify
    DWORD GetModelTable28();
    // 0x000acac0
    void* GetCurrentWeaponInfo();
    // 0x000abe90
    float GetRotation();
    // 0x000adad0
    void StartDancing(int style);
    // R5 address to verify
    BOOL IsPerformingAnimation(const char* name);
    // 0x000ad230
    void ApplyAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time);
    // 0x000aff50
    void SetModelIndex(int model);
    // 0x000ac4e0
    void ExitVehicle();
    // 0x000ae1e0
    void StopUrinating();
    // 0x000ac200
    int GetVehicleSeatIndex();
    // 0x000af730
    void CreateArrow(DWORD color);
    // 0x000ad3e0
    void DestroyParachute();
    // 0x000ac1a0
    float GetDistanceToEntity(CEntity* entity);
    // 0x000b0b10
    void AddAccessory(int index, const PedAccessoryState* info);
    // 0x000aee50
    void DeleteAccessory(int index);
    // 0x000abf60
    void SetRotation(float rotation);
    // 0x000abf10
    void ForceRotation(float rotation);
    // 0x000acd60
    void DisableJetpack();
    // 0x000acb60
    BOOL DoesHandsUp();
    // 0x000ae020
    void DestroyCommandTask();
    // 0x000adfd0
    void ApplyCommandTask(const char* name, int p1, int p2, int p3, const CVector* p4, int p5, float p6, int p7, int p8, int p9);
    // 0x000ad930
    void SetAimZ(float value);
    // 0x000acbc0
    void HoldObject(DWORD object);
    // 0x000abd00
    void SetImmunities(DWORD bullet, DWORD fire, DWORD explosion, DWORD collision, DWORD melee);
    // 0x000acb10
    void HandsUp();
    // 0x000acd10
    void EnableJetpack();
    // 0x000ac010
    void ClearWeapons();
    // 0x000ae480
    DWORD GetBonePosition(int bone, CVector* position);
    // 0x000ad8b0
    void RemoveWeaponModel(int weapon);
    // 0x000ac160
    BOOL CurrentWeaponHasAmmo();
    // 0x000ad8f0
    float GetAimZ();
    // 0x000adb20
    void StopDancing();
    // 0x000acdc0
    BOOL HasJetpack();
    // 0x000ada00
    BOOL PerformingCustomAnimation();
    // 0x000abbf0
    BYTE GetAim();
    // Native forwarding entry to the existing internal camera-aim accessor.
    // 0x000abc20
    CameraAim* GetCameraAim();
    // 0x000b0080
    void SetWeaponAmmo(BYTE weapon, WORD ammo);
    // 0x000ad190
    void* GetWeaponSlot(DWORD weapon);
    // 0x000abe10
    BOOL IsDead();
    // 0x000ac750
    void SetSpawnInfo(const CVector* position, float rotation);
    // 0x000ae300
    void LoadShoppingDataSubsection(const char* name);
    // 0x000abcb0
    void DeleteArrow();
    // 0x000ae070
    void EnableCellphone(BOOL enabled);
    // 0x000aeeb0
    BOOL GetAccessoryState(int index);
    // 0x000ae0d0
    void SetFightingStyle(int style);
    // 0x000ad880
    void ResetDamageEntity();
    // 0x00013680
    void* GetAccessory(int index);
    // 0x00071b30
    PedAccessoryState* GetAccessoryInfo(int index);
    // 0x000aea80
    void GetBoneMatrixById(CMatrix* out, int bone);
    // Accesses the backend bone table without bounds/null checks before forwarding.
    // R5 address to verify
    void GetBoneMatrix(CMatrix* out, int bone);
    // Raw animation-frame quaternion; the bone index is unchecked.
    // 0x000aebb0
    void* GetBoneQuaternion(int bone);
    // 0x000aecb0
    void ApplySecondaryPose();
    // 0x000b0760
    void ApplyDancePose();
    // 0x000b1270
    void ProcessAnimationPose();
    // 0x000b0580
    void ProcessAccessories();
    // 0x000aef60
    void* GetAimedPed();
    // 0x000ac110
    void RemoveWeaponWhenEnteringVehicle();
    // 0x000ae2e0
    const char* GetLoadedShoppingDataSubsection();
    // 0x000abbd0
    void SetInitialState();
    // 0x000abc30
    void SetAim(const CameraAim* aim);
    // 0x000ac140
    void* GetCurrentWeaponSlot();
    // 0x000ad860
    BOOL IsOnGround();
    // 0x000abe50
    void SetState(BYTE state);
    // 0x000abd90
    float GetArmour();
    // 0x000abd50
    float GetHealth();
    // 0x000abc90
    DWORD GetVehicleRef();
    // 0x000abdf0
    void SetFlags(DWORD flags);
    // 0x000abdb0
    void SetArmour(float armour);
    // 0x000abd70
    void SetHealth(float health);
    // 0x000abbb0
    void ResetPointers();
    // 0x000abdd0
    DWORD GetFlags();
    // 0x000adb60
    BOOL DoesDancing();
    // 0x000adfb0
    void SetDrunkLevel(DWORD level);
    // 0x000ac000
    void* GetVehicle();
    // 0x000abe40
    BYTE GetState();
    // 0x000ae260
    BOOL DoesUrinating();
    // 0x000adc90
    DWORD GetHeldObjectType();
    // 0x000ae0a0
    BOOL UsingCellphone();
    // R5 address to verify
    virtual ~CPed();
    // 0x000b0fa0
    void Destroy();
    // 0x000aee30
    BOOL HasAccessory();
    // 0x000b0ab0
    void DeleteAllAccessories();
    // 0x000adc00
    void DropStuff();
    // 0x000ac570
    void WarpFromVehicle(float x, float y, float z);
    // 0x000abe70
    BOOL IsInVehicle();
    // 0x000abfc0
    BOOL IsPassenger();
    // 0x000adfa0
    DWORD GetDrunkLevel();
    // Observed task-type test; does not establish a complete GTA task interface.
    // 0x000accd0
    BOOL HasTaskType268();
    // 0x000acef0
    BOOL HasDanceState();
    // 0x000ace40
    BOOL HasDanceStyle();
    // 0x000abc50
    DWORD GetCurrentWeapon();
    // 0x000ac060
    void SetArmedWeapon(DWORD weapon, BYTE direct);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CPed) == 0x32d);
STATIC_ASSERT(offsetof(CPed, m_pGamePed) == 0x2a4);
STATIC_ASSERT(offsetof(CPed, m_nPlayerNumber) == 0x2b0);
// 0x0026bf10
extern void* g_gamePlayerPeds[210];
// R5 address to verify
extern BYTE g_gamePlayerControllerState[210][16];
// 0x000b3c60
void* __stdcall GameFindLocalPed();
// 0x000b4320
void __stdcall GameAssignPlayerPed(BYTE player, void* ped);
// 0x000b3b70
void* __stdcall GamePoolPedGetAt(int handle);
// R5 address to verify
extern void* g_pedCommandTaskSnapshot;
// R5 address to verify
extern const float g_pedFloorRayLength;
// R5 address to verify
extern DWORD g_pedVehicleEntryFlag;
// R5 address to verify
extern DWORD g_pedDrunkVehicleTick;
// 0x000e6130
extern const float g_pedDrunkShakeScale;
// 0x000e55f8
extern const float g_pedDrunkSpeedThreshold;
#endif

// 0x000b41c0
BOOL __stdcall GameIsBackendOnScreen(void* backend);

// Opaque player flag table; factory-selected slots are2..209.
// Byte indices210..255 reach the verified adjacent gap/cache/ped storage.
// R5 address to verify
extern DWORD g_gamePlayerFlags[210];
// 0x000b44c0
void __stdcall GameSetPlayerFlag(BYTE player, DWORD value);
// 0x000ecda8
extern const Scripting::OpcodeInfo g_pedCreatePlayerCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedGetPlayerHandleCommand;
// 0x000ecfd8
extern const Scripting::OpcodeInfo g_pedCommand03fe;
// 0x000ed278
extern const float g_pedSpawnLift;

// R5 address to verify
extern const Scripting::OpcodeInfo g_pedBehaviorWalkStyleCommand;

// 0x000ecf24
extern const Scripting::OpcodeInfo g_pedBehaviorDrivebyCommand;

// 0x000ece98
extern const Scripting::OpcodeInfo g_pedBehaviorCreateObjectCommand;

// 0x000ed014
extern const Scripting::OpcodeInfo g_pedBehaviorAttachCommand;

// 0x000ed028
extern const Scripting::OpcodeInfo g_pedBehaviorObjectFlagCommand;

// R5 address to verify
extern const Scripting::OpcodeInfo g_pedBehaviorObjectScalarCommand;

// 0x000ed000
extern const Scripting::OpcodeInfo g_pedBehaviorObjectAnimationCommand;

// 0x000eceac
extern const Scripting::OpcodeInfo g_pedBehaviorEffectCommand;

// 0x000ed204
extern const char g_pedParachuteOpen[10];

// 0x000ed1f8
extern const char g_pedParachuteDecel[11];

// 0x000ed1dc
extern const char g_pedParachuteFloat[11];

// 0x000ed1c0
extern const char g_pedParachuteSteerRight[12];

// 0x000ed1a4
extern const char g_pedParachuteSteerLeft[12];

// 0x000ed184
extern const char g_pedParachuteLandWater[16];

// 0x000ed158
extern const char g_pedParachuteOpenObject[12];

// 0x000ed1e8
extern const char g_pedParachuteDecelObject[13];

// 0x000ed1cc
extern const char g_pedParachuteFloatObject[13];

// 0x000ed1b0
extern const char g_pedParachuteSteerRightObject[14];

// 0x000ed194
extern const char g_pedParachuteSteerLeftObject[14];

// 0x000ed170
extern const char g_pedParachuteLandWaterObject[18];

// 0x000e8898
extern const double g_pedParachuteOne;

// 0x000ed168
extern const double g_pedParachuteOpeningSpeed;

// 0x000ed150
extern const double g_pedDrivebyDistance;

// 0x000ed298
extern const double g_pedStuffOffset1X;

// 0x000ed2a0
extern const double g_pedStuffOffset1Y;

// 0x000ed2a8
extern const double g_pedStuffOffset1Z;

// 0x000ed290
extern const double g_pedStuffOffset2Z;

// 0x000ed280
extern const double g_pedStuffOffset3X;

// 0x000ea960
extern const double g_pedStuffOffset3Y;

// 0x000ed288
extern const double g_pedStuffOffset3Z;

// R5 address to verify
extern char g_pedShoppingName[32];

// R5 address to verify
extern CPed* g_pedFiringContext;
// R5 address to verify
extern void* g_pedFiringData;
// 0x000ea6ac
extern const char g_pedBoneIdFormat[3];

// R5 address to verify
BYTE* __stdcall GameGetPlayerControlFlags(BYTE player);
// 0x000b3e20
int __stdcall GameGetWeaponModel(int weapon);

// R5 address to verify
void GameInitializePlayerPedTables();
// Native has no bound check;210..255 reaches adjacent controller state.
// R5 address to verify
void* __stdcall GameGetPlayerPedUnchecked(BYTE player);
// R5 address to verify
void __stdcall GameSetPlayerControllerWords(BYTE player, DWORD first, DWORD second, DWORD third, DWORD fourth);
// R5 address to verify
BYTE __stdcall GameFindPlayerIndexByClump(void* clump);
// R5 address to verify
DWORD __stdcall GameGetPlayerFlagsUnchecked(BYTE player);

// Shared raw raster snapshot words (+14/+0C), retaining native table adjacency.
// 0x0026bf00
extern DWORD g_gamePlayerTableUnknown2A9D98[2];
