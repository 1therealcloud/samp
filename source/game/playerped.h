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
typedef char PedBulletDataExtent[(sizeof(PedBulletData) == 44) ? 1 : -1];
struct PedAccessoryState
{
    int model, bone;
    CVector offset, rotation, scale;
    DWORD materialColor1, materialColor2;
};
typedef char PedAccessoryExtent[(sizeof(PedAccessoryState) == 52) ? 1 : -1];
// samp.dll vtable RVA: 0x0011f24c
class CPed : public CEntity
{
public:
    // 0x000ab670
    PedBulletData* GetBulletData();

    // 0x000acd70
    WORD GetCurrentWeaponTotalAmmo();
    // 0x000b0ef0
    void ProcessCameraAimMarker();

    // 0x000ade70
    void ProcessVehicleHorn();
    // 0x000ae050
    void SetWeaponSkill(int skill, WORD value);
    // 0x000ae130
    void UpdateClumpAnimations(float timeStep, DWORD onScreen);
    // 0x000ae1d0
    BYTE* GetControlFlags();
    // Descriptive distance between mapped bone slots5 and6; result remains in ST0.
    // 0x000ae960
    float GetArmBoneDistance();
    // 0x000aee80
    void SetBulletData(PedBulletData* data);
    // 0x000af390
    void GiveWeapon(int weapon, int ammo);
    // 0x000afa60
    void EnableGoggles();
    // Returns the raw GTA hit entity; the output stays untouched on a miss.
    // 0x000af490
    void* GetAimHit(CVector* position);
    // 0x000af870
    void GetCameraAimRay(CVector* origin, CVector* target);
    // 0x000b0ce0
    void* GetCameraAimHit(CVector* position);
    // Preserves native publication/clearing of shared firing context; always returns TRUE.
    // 0x000af670
    BOOL FireWeapon();
    // 0x000aeb90
    void DrawBoneIds();
    // 0x000aec20
    void DrawMappedBoneIds();
    // 0x000aecf0
    void DrawAnimationFrameIds();
    // 0x000aedf0
    void UpdateAndDrawBoneIds();
    // 0x000b0d90
    void ProcessAimMarker();
    // 0x000afb10
    void ClearAimTarget();
    // Opaque task fields, not an established complete GTA animation-task class.
    // 0x000ae1f0
    DWORD PackAnimationTask(const void* task);
    // 0x000ae2a0
    DWORD PackQueuedAnimation();
    // 0x000ae2e0
    void* FindAnimationTask();
    // 0x000ae390
    DWORD GetPackedAnimation();
    // 0x000ae3d0
    void ApplyPackedAnimation(DWORD packed);
    // 0x000ae560
    int GetBoneId(int bone);
    // Raw hierarchy field +0; higher-level flag meaning is not assumed.
    // 0x000ae590
    DWORD GetBoneHierarchyFlags();
    // 0x000ae890
    void UpdateBoneMatrices();
    // Descriptive backend-name accessor, distinct from the global shopping subsection.
    // 0x000adf30
    const char* GetBackendShoppingName();
    // 0x000adf80
    void SetBackendShoppingName(const char* name);
    // 0x000aeae0
    void GetAimOffsetMatrix(CMatrix* matrix);
    // Native always returns TRUE; invalid output/backend paths are not repaired.
    // 0x000af5c0
    BOOL GetWeaponAimOffsets(BOOL alternate, CVector* position, CVector* weaponOffsetOut);
    // Descriptive task-state name; original numeric return categories retained.
    // 0x000ac1f0
    int GetVehicleTransitionTaskState();
    // 0x000ac260
    BOOL HasTaskType704();
    // 0x000ac2b0
    BOOL IsDucking();
    // 0x000ac2e0
    void ClearTasksIfOnFoot();
    // 0x000ac340
    void ClearDuckingFlag();
    // 0x000ac830
    BOOL HasTaskType211();
    // 0x000ac870
    BOOL HasTaskType1016();
    // 0x000ac8b0
    BOOL HasTaskType208();
    // Raw backend +500 or wrapper +2e6 gate; existing field names retained.
    // 0x000aca20
    BOOL HasAimTarget();
    // Descriptive pose-state name; does not rename the historical layout fields.
    // 0x000aca70
    void SetSecondaryPoseState(DWORD state);
    // 0x000acab0
    void ProcessSecondaryPoseWeapon();
    // Preserves the full state DWORD, not only a normalized BOOL.
    // 0x000acb20
    void SetDancePoseState(DWORD state);
    // 0x000acb40
    void ProcessDancePoseWeapon();
    // 0x000acc50
    BOOL HasTaskType1022();
    // 0x000acca0
    void StopTaskType1022();
    // 0x000ace20
    void SetMoney(int amount);
    // 0x000ad060
    void CheckParachuteWeapon();
    // 0x000ad640
    BOOL HasTaskType401();
    // Backend flag semantics not established; descriptive offset name.
    // 0x000adcb0
    DWORD GetBackendByte72D();
    // Argument/return words established; higher-level semantic name remains unknown.
    // 0x000ae0d0
    DWORD CallBackend5E01C0(DWORD first, DWORD second);
    // 0x000ae120
    void ResetLocalControlWord();
    // 0x000ae160
    void ApplyClumpAnimation(int mode, DWORD first, DWORD second, DWORD third);
    // 0x000ae5e0
    int GetBoneCount();
    // 0x000ae630
    CMatrix* GetBoneMatrices();
    // 0x000ae710
    void SetBoneMatrixById(const CMatrix* matrix, int bone);
    // 0x000ae850
    void SetBoneMatrix(const CMatrix* matrix, int bone);
    // 0x000aee20
    float GetRootPoseFactor();
    // 0x000afcb0
    void ProcessDancing();
    // 0x000afed0
    void GiveStuff(int type);
    // 0x000ac470
    BYTE GetDeathInfo(WORD* killer);
    // True native entry verified from opcode0713 references; API ACF90 is interior code.
    // 0x000acbb0
    BOOL EnablePassengerDrivebyMode();
    // 0x000acdf0
    void SetWalkStyle(const char* name);
    // 0x000ad0f0
    BOOL OpenParachute();
    // 0x000ad240
    void ProcessParachuteEvent(const char* name);
    // 0x000ab900
    BOOL IsOnScreen();
    // 0x000ad5a0
    void* GetContactEntity();
    // 0x000ad5b0
    void* GetContactVehicle();
    // 0x000141e0
    void SetCameraMode(BYTE mode);
    // 0x00014200
    void SetCameraExtZoomAndAspectRatio(float zoom, float aspect);
    // 0x00002d00
    BYTE GetCameraMode();

    BOOL m_bUsingCellphone;
    BOOL m_accessoryOccupied[10];
    PedAccessoryState m_accessoryInfo[10];
    void* m_accessoryObjects[10];
    // Raw backend accessor; name follows0.2x; class binding inferred from verified2A4 layout and adjacent accessory entry.
    // 0x00013570
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
    // 0x000ab690
    CPed();
    // 0x000b08e0
    CPed(int player, int model, CVector position, float rotation);
    // 0x000ad9f0
    void ProcessDrunk();
    // 0x000aef40
    void SetKeys(WORD mask, WORD leftRight, WORD upDown);
    // 0x000af1d0
    WORD GetKeys(WORD* leftRight, WORD* upDown);
    // 0x000ad8a0
    BOOL ApplyStuff();
    // 0x000abeb0
    void PutIntoVehicle(int vehicle, int seat);
    // 0x000add00
    void StartUrinating();
    // 0x000ac030
    void EnterVehicle(int vehicle, BOOL passenger);
    // 0x000ac630
    void* GetFloor();
    // 0x000afbd0
    void Kill();
    // 0x000acd10
    void CallBackend730AndVehicle490();
    // 0x000ac3b0
    void SetControllable(BOOL enabled);
    // 0x000acf60
    void LinkToInterior(BYTE interior, BOOL refresh);
    // 0x000ad770
    const char* GetAnimationForDance(int move);
    // 0x000ad5e0
    DWORD GetModelTable24();
    // 0x000ad5c0
    DWORD GetModelTable28();
    // 0x000ac6e0
    void* GetCurrentWeaponInfo();
    // 0x000abab0
    float GetRotation();
    // 0x000ad6d0
    void StartDancing(int style);
    // 0x000acf20
    BOOL IsPerformingAnimation(const char* name);
    // 0x000ace50
    void ApplyAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time);
    // 0x000afb50
    void SetModelIndex(int model);
    // 0x000ac100
    void ExitVehicle();
    // 0x000adde0
    void StopUrinating();
    // 0x000abe20
    int GetVehicleSeatIndex();
    // 0x000af330
    void CreateArrow(DWORD color);
    // 0x000ad000
    void DestroyParachute();
    // 0x000abdc0
    float GetDistanceToEntity(CEntity* entity);
    // 0x000b0710
    void AddAccessory(int index, const PedAccessoryState* info);
    // 0x000aea50
    void DeleteAccessory(int index);
    // 0x000abb80
    void SetRotation(float rotation);
    // 0x000abb30
    void ForceRotation(float rotation);
    // 0x000ac980
    void DisableJetpack();
    // 0x000ac780
    BOOL DoesHandsUp();
    // 0x000adc20
    void DestroyCommandTask();
    // 0x000adbd0
    void ApplyCommandTask(const char* name, int p1, int p2, int p3, const CVector* p4, int p5, float p6, int p7, int p8, int p9);
    // 0x000ad550
    void SetAimZ(float value);
    // 0x000ac7e0
    void HoldObject(DWORD object);
    // 0x000ab920
    void SetImmunities(DWORD bullet, DWORD fire, DWORD explosion, DWORD collision, DWORD melee);
    // 0x000ac730
    void HandsUp();
    // 0x000ac930
    void EnableJetpack();
    // 0x000abc30
    void ClearWeapons();
    // 0x000ae080
    DWORD GetBonePosition(int bone, CVector* position);
    // 0x000ad4d0
    void RemoveWeaponModel(int weapon);
    // 0x000abd80
    BOOL CurrentWeaponHasAmmo();
    // 0x000ad510
    float GetAimZ();
    // 0x000ad720
    void StopDancing();
    // 0x000ac9e0
    BOOL HasJetpack();
    // 0x000ad600
    BOOL PerformingCustomAnimation();
    // 0x000ab810
    BYTE GetAim();
    // Native forwarding entry to the existing internal camera-aim accessor.
    // 0x000ab840
    CameraAim* GetCameraAim();
    // 0x000afc80
    void SetWeaponAmmo(BYTE weapon, WORD ammo);
    // 0x000acdb0
    void* GetWeaponSlot(DWORD weapon);
    // 0x000aba30
    BOOL IsDead();
    // 0x000ac370
    void SetSpawnInfo(const CVector* position, float rotation);
    // 0x000adf00
    void LoadShoppingDataSubsection(const char* name);
    // 0x000ab8d0
    void DeleteArrow();
    // 0x000adc70
    void EnableCellphone(BOOL enabled);
    // 0x000aeab0
    BOOL GetAccessoryState(int index);
    // 0x000adcd0
    void SetFightingStyle(int style);
    // 0x000ad4a0
    void ResetDamageEntity();
    // 0x00013580
    void* GetAccessory(int index);
    // 0x000715d0
    PedAccessoryState* GetAccessoryInfo(int index);
    // Descriptive names; native output remains untouched when the backend is invalid.
    // 0x000ae680
    void GetBoneMatrixById(CMatrix* out, int bone);
    // Accesses the backend bone table without bounds/null checks before forwarding.
    // 0x000ae810
    void GetBoneMatrix(CMatrix* out, int bone);
    // Raw animation-frame quaternion; the bone index is unchecked.
    // 0x000ae7b0
    void* GetBoneQuaternion(int bone);
    // Descriptive names for the observed pose/frame bodies.
    // 0x000ae8b0
    void ApplySecondaryPose();
    // 0x000b0360
    void ApplyDancePose();
    // 0x000b0e70
    void ProcessAnimationPose();
    // 0x000b0180
    void ProcessAccessories();
    // 0x000aeb60
    void* GetAimedPed();
    // 0x000abd30
    void RemoveWeaponWhenEnteringVehicle();
    // 0x000adee0
    const char* GetLoadedShoppingDataSubsection();
    // 0x000ab7f0
    void SetInitialState();
    // 0x000ab850
    void SetAim(const CameraAim* aim);
    // 0x000abd60
    void* GetCurrentWeaponSlot();
    // 0x000ad480
    BOOL IsOnGround();
    // 0x000aba70
    void SetState(BYTE state);
    // 0x000ab9b0
    float GetArmour();
    // 0x000ab970
    float GetHealth();
    // 0x000ab8b0
    DWORD GetVehicleRef();
    // 0x000aba10
    void SetFlags(DWORD flags);
    // 0x000ab9d0
    void SetArmour(float armour);
    // 0x000ab990
    void SetHealth(float health);
    // 0x000ab7d0
    void ResetPointers();
    // 0x000ab9f0
    DWORD GetFlags();
    // 0x000ad760
    BOOL DoesDancing();
    // 0x000adbb0
    void SetDrunkLevel(DWORD level);
    // 0x000abc20
    void* GetVehicle();
    // 0x000aba60
    BYTE GetState();
    // 0x000ade60
    BOOL DoesUrinating();
    // 0x000ad890
    DWORD GetStuff();
    // 0x000adca0
    BOOL UsingCellphone();
    // 0x000b0ea0
    virtual ~CPed();
    // 0x000b0ba0
    void Destroy();
    // 0x000aea30
    BOOL HasAccessory();
    // 0x000b06b0
    void DeleteAllAccessories();
    // 0x000ad800
    void DropStuff();
    // 0x000ac190
    void WarpFromVehicle(float x, float y, float z);
    // 0x000aba90
    BOOL IsInVehicle();
    // 0x000abbe0
    BOOL IsPassenger();
    // 0x000adba0
    DWORD GetDrunkLevel();
    // Observed task-type test; does not establish a complete GTA task interface.
    // 0x000ac8f0
    BOOL HasTaskType268();
    // Descriptive predicates for native dance state/style fields.
    // 0x000acb10
    BOOL HasDanceState();
    // 0x000aca60
    BOOL HasDanceStyle();
    // 0x000ab870
    DWORD GetCurrentWeapon();
    // 0x000abc80
    void SetArmedWeapon(DWORD weapon, BYTE direct);
};
#pragma pack(pop)
typedef char PedExtent[(sizeof(CPed) == 0x32d) ? 1 : -1];
typedef char PedBackendOffset[(offsetof(CPed, m_pGamePed) == 0x2a4) ? 1 : -1];
typedef char PedPlayerOffset[(offsetof(CPed, m_nPlayerNumber) == 0x2b0) ? 1 : -1];
// 0x002a9da8
extern void* g_gamePlayerPeds[210];
// 0x002aa0f0
extern BYTE g_gamePlayerControllerState[210][16];
// 0x000b3860
void* __stdcall GameFindLocalPed();
// 0x000b3f20
void __stdcall GameAssignPlayerPed(BYTE player, void* ped);
// 0x000b3770
void* __stdcall GamePoolPedGetAt(int handle);
// 0x002a92a0
extern void* g_pedCommandTaskSnapshot;
// 0x0011f258
extern const float g_pedFloorRayLength;
// 0x0018f4a4
extern DWORD g_pedVehicleEntryFlag;
// 0x002a92a4
extern DWORD g_pedDrunkVehicleTick;
// 0x001181c0
extern const float g_pedDrunkShakeScale;
// 0x00117668
extern const float g_pedDrunkSpeedThreshold;
#endif

// 0x000b3dc0
BOOL __stdcall GameIsBackendOnScreen(void* backend);

// Opaque player flag table; factory-selected slots are2..209.
// Byte indices210..255 reach the verified adjacent gap/cache/ped storage.
// 0x002a9a50
extern DWORD g_gamePlayerFlags[210];
// 0x000b40c0
void __stdcall GameSetPlayerFlag(BYTE player, DWORD value);
// 0x0011eea0
extern const Scripting::OpcodeInfo g_pedCreatePlayerCommand;
// 0x0011eeb4
extern const Scripting::OpcodeInfo g_pedGetPlayerHandleCommand;
// 0x0011f0e4
extern const Scripting::OpcodeInfo g_pedCommand03fe;
// 0x0011f388
extern const float g_pedSpawnLift;

// 0x0011f094
extern const Scripting::OpcodeInfo g_pedBehaviorWalkStyleCommand;

// 0x0011f030
extern const Scripting::OpcodeInfo g_pedBehaviorDrivebyCommand;

// 0x0011efa4
extern const Scripting::OpcodeInfo g_pedBehaviorCreateObjectCommand;

// 0x0011f120
extern const Scripting::OpcodeInfo g_pedBehaviorAttachCommand;

// 0x0011f134
extern const Scripting::OpcodeInfo g_pedBehaviorObjectFlagCommand;

// 0x0011f170
extern const Scripting::OpcodeInfo g_pedBehaviorObjectScalarCommand;

// 0x0011f10c
extern const Scripting::OpcodeInfo g_pedBehaviorObjectAnimationCommand;

// 0x0011efb8
extern const Scripting::OpcodeInfo g_pedBehaviorEffectCommand;

// 0x0011f314
extern const char g_pedParachuteOpen[10];

// 0x0011f308
extern const char g_pedParachuteDecel[11];

// 0x0011f2ec
extern const char g_pedParachuteFloat[11];

// 0x0011f2d0
extern const char g_pedParachuteSteerRight[12];

// 0x0011f2b4
extern const char g_pedParachuteSteerLeft[12];

// 0x0011f294
extern const char g_pedParachuteLandWater[16];

// 0x0011f268
extern const char g_pedParachuteOpenObject[12];

// 0x0011f2f8
extern const char g_pedParachuteDecelObject[13];

// 0x0011f2dc
extern const char g_pedParachuteFloatObject[13];

// 0x0011f2c0
extern const char g_pedParachuteSteerRightObject[14];

// 0x0011f2a4
extern const char g_pedParachuteSteerLeftObject[14];

// 0x0011f280
extern const char g_pedParachuteLandWaterObject[18];

// 0x0011a928
extern const double g_pedParachuteOne;

// 0x0011f278
extern const double g_pedParachuteOpeningSpeed;

// 0x0011f260
extern const double g_pedDrivebyDistance;

// 0x0011f3a8
extern const double g_pedStuffOffset1X;

// 0x0011f3b0
extern const double g_pedStuffOffset1Y;

// 0x0011f3b8
extern const double g_pedStuffOffset1Z;

// 0x0011f3a0
extern const double g_pedStuffOffset2Z;

// 0x0011f390
extern const double g_pedStuffOffset3X;

// 0x0011c988
extern const double g_pedStuffOffset3Y;

// 0x0011f398
extern const double g_pedStuffOffset3Z;

// 0x002a9280
extern char g_pedShoppingName[32];

// 0x0018f664
extern CPed* g_pedFiringContext;
// 0x0018f668
extern void* g_pedFiringData;
// 0x0011c6d4
extern const char g_pedBoneIdFormat[3];

// 0x000b3fd0
BYTE* __stdcall GameGetPlayerControlFlags(BYTE player);
// 0x000b3a20
int __stdcall GameGetWeaponModel(int weapon);

// 0x000b3f00
void GameInitializePlayerPedTables();
// Native has no bound check;210..255 reaches adjacent controller state.
// 0x000b3f50
void* __stdcall GameGetPlayerPedUnchecked(BYTE player);
// 0x000b3f90
void __stdcall GameSetPlayerControllerWords(BYTE player, DWORD first, DWORD second, DWORD third, DWORD fourth);
// 0x000b3ff0
BYTE __stdcall GameFindPlayerIndexByClump(void* clump);
// 0x000b40b0
DWORD __stdcall GameGetPlayerFlagsUnchecked(BYTE player);

// Shared raw raster snapshot words (+14/+0C), retaining native table adjacency.
// 0x002a9d98
extern DWORD g_gamePlayerTableUnknown2A9D98[2];
