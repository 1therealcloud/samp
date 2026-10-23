#ifndef SAMP_RECOVERED_PED_H
#define SAMP_RECOVERED_PED_H
#include "game/entity.h"
#include "game/scripting.h"
struct CameraAim;
#pragma pack(push,1)
struct PedAccessoryState {
    int model,bone;
    CVector offset,rotation,scale;
    DWORD materialColor1,materialColor2;
};
typedef char PedAccessoryExtent[(sizeof(PedAccessoryState)==52)?1:-1];
// samp.dll vtable RVA: 0x0011f24c
class CPed : public CEntity {
public:
    // Descriptive task-state name; original numeric return categories retained.
    // samp.dll RVA: 0x000ac1f0
    int GetVehicleTransitionTaskState();
    // samp.dll RVA: 0x000ac260
    BOOL HasTaskType704();
    // samp.dll RVA: 0x000ac2b0
    BOOL IsDucking();
    // samp.dll RVA: 0x000ac2e0
    void ClearTasksIfOnFoot();
    // samp.dll RVA: 0x000ac340
    void ClearDuckingFlag();
    // samp.dll RVA: 0x000ac830
    BOOL HasTaskType211();
    // samp.dll RVA: 0x000ac870
    BOOL HasTaskType1016();
    // samp.dll RVA: 0x000ac8b0
    BOOL HasTaskType208();
    // Raw backend +500 or wrapper +2e6 gate; existing field names retained.
    // samp.dll RVA: 0x000aca20
    BOOL HasAimTarget();
    // Descriptive pose-state name; does not rename the historical layout fields.
    // samp.dll RVA: 0x000aca70
    void SetSecondaryPoseState(DWORD state);
    // samp.dll RVA: 0x000acab0
    void ProcessSecondaryPoseWeapon();
    // Preserves the full state DWORD, not only a normalized BOOL.
    // samp.dll RVA: 0x000acb20
    void SetDancePoseState(DWORD state);
    // samp.dll RVA: 0x000acb40
    void ProcessDancePoseWeapon();
    // samp.dll RVA: 0x000acc50
    BOOL HasTaskType1022();
    // samp.dll RVA: 0x000acca0
    void StopTaskType1022();
    // samp.dll RVA: 0x000ace20
    void SetMoney(int amount);
    // samp.dll RVA: 0x000ad060
    void CheckParachuteWeapon();
    // samp.dll RVA: 0x000ad640
    BOOL HasTaskType401();
    // Backend flag semantics not established; descriptive offset name.
    // samp.dll RVA: 0x000adcb0
    DWORD GetBackendByte72D();
    // Argument/return words established; higher-level semantic name remains unknown.
    // samp.dll RVA: 0x000ae0d0
    DWORD CallBackend5E01C0(DWORD first,DWORD second);
    // samp.dll RVA: 0x000ae120
    void ResetLocalControlWord();
    // samp.dll RVA: 0x000ae160
    void ApplyClumpAnimation(int mode,DWORD first,DWORD second,DWORD third);
    // samp.dll RVA: 0x000ae5e0
    int GetBoneCount();
    // samp.dll RVA: 0x000ae630
    CMatrix * GetBoneMatrices();
    // samp.dll RVA: 0x000ae710
    void SetBoneMatrixById(const CMatrix *matrix,int bone);
    // samp.dll RVA: 0x000ae850
    void SetBoneMatrix(const CMatrix *matrix,int bone);
    // samp.dll RVA: 0x000aee20
    float GetRootPoseFactor();
    // samp.dll RVA: 0x000afcb0
    void ProcessDancing();
    // samp.dll RVA: 0x000afed0
    void GiveStuff(int type);
    // samp.dll RVA: 0x000ac470
    BYTE GetDeathInfo(WORD *killer);
    // True native entry verified from opcode0713 references; API ACF90 is interior code.
    // samp.dll RVA: 0x000acbb0
    BOOL EnablePassengerDrivebyMode();
    // samp.dll RVA: 0x000acdf0
    void SetWalkStyle(const char *name);
    // samp.dll RVA: 0x000ad0f0
    BOOL OpenParachute();
    // samp.dll RVA: 0x000ad240
    void ProcessParachuteEvent(const char *name);
    // samp.dll RVA: 0x000ab900
    BOOL IsOnScreen();
    // samp.dll RVA: 0x000ad5a0
    void *GetContactEntity();
    // samp.dll RVA: 0x000ad5b0
    void *GetContactVehicle();
    // samp.dll RVA: 0x000141e0
    void SetCameraMode(BYTE mode);
    // samp.dll RVA: 0x00014200
    void SetCameraExtZoomAndAspectRatio(float zoom,float aspect);
    // samp.dll RVA: 0x00002d00
    BYTE GetCameraMode();

    BOOL m_bUsingCellphone;
    BOOL m_accessoryOccupied[10];
    PedAccessoryState m_accessoryInfo[10];
    void *m_accessoryObjects[10];
    void *m_pGamePed;
    DWORD unknown2a8,unknown2ac;
    BYTE m_nPlayerNumber;
    BYTE unknown2b1[8];
    DWORD m_parachuteObject;
    BYTE unknown2bd[4];
    DWORD m_stuffType,m_stuffObject,m_drunkLevel,m_arrow;
    BYTE unknown2d1;
    DWORD unknown2d2;
    BYTE unknown2d6[8];
    BOOL m_bDancing;
    DWORD m_danceStyle,m_lastDanceMove;
    BYTE unknown2ea[8];
    BOOL m_bUrinating;
    BYTE unknown2f6,unknown2f7,unknown2f8;
    DWORD unknown2f9;
    BYTE unknown2fd[48];
    // samp.dll RVA: 0x000ab690
    CPed();
    // samp.dll RVA: 0x000b08e0
    CPed(int player,int model,CVector position,float rotation);
    // samp.dll RVA: 0x000ad9f0
    void ProcessDrunk();
    // samp.dll RVA: 0x000aef40
    void SetKeys(WORD mask,WORD leftRight,WORD upDown);
    // samp.dll RVA: 0x000af1d0
    WORD GetKeys(WORD *leftRight,WORD *upDown);
    // samp.dll RVA: 0x000ad8a0
    BOOL ApplyStuff();
    // samp.dll RVA: 0x000abeb0
    void PutIntoVehicle(int vehicle,int seat);
    // samp.dll RVA: 0x000add00
    void StartUrinating();
    // samp.dll RVA: 0x000ac030
    void EnterVehicle(int vehicle,BOOL passenger);
    // samp.dll RVA: 0x000ac630
    void *GetFloor();
    // samp.dll RVA: 0x000afbd0
    void Kill();
    // samp.dll RVA: 0x000acd10
    void CallBackend730AndVehicle490();
    // samp.dll RVA: 0x000ac3b0
    void SetControllable(BOOL enabled);
    // samp.dll RVA: 0x000acf60
    void LinkToInterior(BYTE interior,BOOL refresh);
    // samp.dll RVA: 0x000ad770
    const char *GetAnimationForDance(int move);
    // samp.dll RVA: 0x000ad5e0
    DWORD GetModelTable24();
    // samp.dll RVA: 0x000ad5c0
    DWORD GetModelTable28();
    // samp.dll RVA: 0x000ac6e0
    void * GetCurrentWeaponInfo();
    // samp.dll RVA: 0x000abab0
    float GetRotation();
    // samp.dll RVA: 0x000ad6d0
    void StartDancing(int style);
    // samp.dll RVA: 0x000acf20
    BOOL IsPerformingAnimation(const char *name);
    // samp.dll RVA: 0x000ace50
    void ApplyAnimation(const char *name,const char *library,float speed,DWORD loop,DWORD lockX,DWORD lockY,DWORD freeze,int time);
    // samp.dll RVA: 0x000afb50
    void SetModelIndex(int model);
    // samp.dll RVA: 0x000ac100
    void ExitVehicle();
    // samp.dll RVA: 0x000adde0
    void StopUrinating();
    // samp.dll RVA: 0x000abe20
    int GetVehicleSeatIndex();
    // samp.dll RVA: 0x000af330
    void CreateArrow(DWORD color);
    // samp.dll RVA: 0x000ad000
    void DestroyParachute();
    // samp.dll RVA: 0x000abdc0
    float GetDistanceToEntity(CEntity *entity);
    // samp.dll RVA: 0x000b0710
    void AddAccessory(int index,const PedAccessoryState *info);
    // samp.dll RVA: 0x000aea50
    void DeleteAccessory(int index);
    // samp.dll RVA: 0x000abb80
    void SetRotation(float rotation);
    // samp.dll RVA: 0x000abb30
    void ForceRotation(float rotation);
    // samp.dll RVA: 0x000ac980
    void DisableJetpack();
    // samp.dll RVA: 0x000ac780
    BOOL DoesHandsUp();
    // samp.dll RVA: 0x000adc20
    void DestroyCommandTask();
    // samp.dll RVA: 0x000adbd0
    void ApplyCommandTask(const char *name,int p1,int p2,int p3,const CVector *p4,int p5,float p6,int p7,int p8,int p9);
    // samp.dll RVA: 0x000ad550
    void SetAimZ(float value);
    // samp.dll RVA: 0x000ac7e0
    void HoldObject(DWORD object);
    // samp.dll RVA: 0x000ab920
    void SetImmunities(DWORD bullet,DWORD fire,DWORD explosion,DWORD collision,DWORD melee);
    // samp.dll RVA: 0x000ac730
    void HandsUp();
    // samp.dll RVA: 0x000ac930
    void EnableJetpack();
    // samp.dll RVA: 0x000abc30
    void ClearWeapons();
    // samp.dll RVA: 0x000ae080
    DWORD GetBonePosition(int bone,CVector *position);
    // samp.dll RVA: 0x000ad4d0
    void RemoveWeaponModel(int weapon);
    // samp.dll RVA: 0x000abd80
    BOOL CurrentWeaponHasAmmo();
    // samp.dll RVA: 0x000ad510
    float GetAimZ();
    // samp.dll RVA: 0x000ad720
    void StopDancing();
    // samp.dll RVA: 0x000ac9e0
    BOOL HasJetpack();
    // samp.dll RVA: 0x000ad600
    BOOL PerformingCustomAnimation();
    // samp.dll RVA: 0x000ab810
    BYTE GetAim();
    // Native forwarding entry to the existing internal camera-aim accessor.
    // samp.dll RVA: 0x000ab840
    CameraAim *GetCameraAim();
    // samp.dll RVA: 0x000afc80
    void SetWeaponAmmo(BYTE weapon,WORD ammo);
    // samp.dll RVA: 0x000acdb0
    void * GetWeaponSlot(DWORD weapon);
    // samp.dll RVA: 0x000aba30
    BOOL IsDead();
    // samp.dll RVA: 0x000ac370
    void SetSpawnInfo(const CVector *position,float rotation);
    // samp.dll RVA: 0x000adf00
    void LoadShoppingDataSubsection(const char *name);
    // samp.dll RVA: 0x000ab8d0
    void DeleteArrow();
    // samp.dll RVA: 0x000adc70
    void EnableCellphone(BOOL enabled);
    // samp.dll RVA: 0x000aeab0
    BOOL GetAccessoryState(int index);
    // samp.dll RVA: 0x000adcd0
    void SetFightingStyle(int style);
    // samp.dll RVA: 0x000ad4a0
    void ResetDamageEntity();
    // samp.dll RVA: 0x00013580
    void * GetAccessory(int index);
    // samp.dll RVA: 0x000715d0
    PedAccessoryState *GetAccessoryInfo(int index);
    // Descriptive names; native output remains untouched when the backend is invalid.
    // samp.dll RVA: 0x000ae680
    void GetBoneMatrixById(CMatrix *out,int bone);
    // Accesses the backend bone table without bounds/null checks before forwarding.
    // samp.dll RVA: 0x000ae810
    void GetBoneMatrix(CMatrix *out,int bone);
    // Raw animation-frame quaternion; the bone index is unchecked.
    // samp.dll RVA: 0x000ae7b0
    void *GetBoneQuaternion(int bone);
    // Descriptive names for the observed pose/frame bodies.
    // samp.dll RVA: 0x000ae8b0
    void ApplySecondaryPose();
    // samp.dll RVA: 0x000b0360
    void ApplyDancePose();
    // samp.dll RVA: 0x000b0e70
    void ProcessAnimationPose();
    // samp.dll RVA: 0x000b0180
    void ProcessAccessories();
    // samp.dll RVA: 0x000aeb60
    void * GetAimedPed();
    // samp.dll RVA: 0x000abd30
    void RemoveWeaponWhenEnteringVehicle();
    // samp.dll RVA: 0x000adee0
    const char * GetLoadedShoppingDataSubsection();
    // samp.dll RVA: 0x000ab7f0
    void SetInitialState();
    // samp.dll RVA: 0x000ab850
    void SetAim(const CameraAim *aim);
    // samp.dll RVA: 0x000abd60
    void * GetCurrentWeaponSlot();
    // samp.dll RVA: 0x000ad480
    BOOL IsOnGround();
    // samp.dll RVA: 0x000aba70
    void SetState(BYTE state);
    // samp.dll RVA: 0x000ab9b0
    float GetArmour();
    // samp.dll RVA: 0x000ab970
    float GetHealth();
    // samp.dll RVA: 0x000ab8b0
    DWORD GetVehicleRef();
    // samp.dll RVA: 0x000aba10
    void SetFlags(DWORD flags);
    // samp.dll RVA: 0x000ab9d0
    void SetArmour(float armour);
    // samp.dll RVA: 0x000ab990
    void SetHealth(float health);
    // samp.dll RVA: 0x000ab7d0
    void ResetPointers();
    // samp.dll RVA: 0x000ab9f0
    DWORD GetFlags();
    // samp.dll RVA: 0x000ad760
    BOOL DoesDancing();
    // samp.dll RVA: 0x000adbb0
    void SetDrunkLevel(DWORD level);
    // samp.dll RVA: 0x000abc20
    void * GetVehicle();
    // samp.dll RVA: 0x000aba60
    BYTE GetState();
    // samp.dll RVA: 0x000ade60
    BOOL DoesUrinating();
    // samp.dll RVA: 0x000ad890
    DWORD GetStuff();
    // samp.dll RVA: 0x000adca0
    BOOL UsingCellphone();
    // samp.dll RVA: 0x000b0ea0
    virtual ~CPed();
    // samp.dll RVA: 0x000b0ba0
    void Destroy();
    // samp.dll RVA: 0x000aea30
    BOOL HasAccessory();
    // samp.dll RVA: 0x000b06b0
    void DeleteAllAccessories();
    // samp.dll RVA: 0x000ad800
    void DropStuff();
    // samp.dll RVA: 0x000ac190
    void WarpFromVehicle(float x,float y,float z);
    // samp.dll RVA: 0x000aba90
    BOOL IsInVehicle();
    // samp.dll RVA: 0x000abbe0
    BOOL IsPassenger();
    // samp.dll RVA: 0x000adba0
    DWORD GetDrunkLevel();
    // Observed task-type test; does not establish a complete GTA task interface.
    // samp.dll RVA: 0x000ac8f0
    BOOL HasTaskType268();
    // Descriptive predicates for native dance state/style fields.
    // samp.dll RVA: 0x000acb10
    BOOL HasDanceState();
    // samp.dll RVA: 0x000aca60
    BOOL HasDanceStyle();
    // samp.dll RVA: 0x000ab870
    DWORD GetCurrentWeapon();
    // samp.dll RVA: 0x000abc80
    void SetArmedWeapon(DWORD weapon,BYTE direct);
};
#pragma pack(pop)
typedef char PedExtent[(sizeof(CPed)==0x32d)?1:-1];
typedef char PedBackendOffset[(offsetof(CPed,m_pGamePed)==0x2a4)?1:-1];
typedef char PedPlayerOffset[(offsetof(CPed,m_nPlayerNumber)==0x2b0)?1:-1];
// samp.dll RVA: 0x002a9da8
extern void *g_gamePlayerPeds[210];
// samp.dll RVA: 0x002aa0f0
extern BYTE g_gamePlayerControllerState[210][16];
// samp.dll RVA: 0x000b3860
void *__stdcall GameFindLocalPed();
// samp.dll RVA: 0x000b3f20
void __stdcall GameAssignPlayerPed(BYTE player,void *ped);
// samp.dll RVA: 0x000b3770
void *__stdcall GamePoolPedGetAt(int handle);
// samp.dll RVA: 0x002a92a0
extern void *g_pedCommandTaskSnapshot;
// samp.dll RVA: 0x0011f258
extern const float g_pedFloorRayLength;
// samp.dll RVA: 0x0018f4a4
extern DWORD g_pedVehicleEntryFlag;
// samp.dll RVA: 0x002a92a4
extern DWORD g_pedDrunkVehicleTick;
// samp.dll RVA: 0x001181c0
extern const float g_pedDrunkShakeScale;
// samp.dll RVA: 0x00117668
extern const float g_pedDrunkSpeedThreshold;
#endif

// samp.dll RVA: 0x000b3dc0
BOOL __stdcall GameIsBackendOnScreen(void *backend);

// Opaque player flag table; factory-selected slots are2..209.
// The byte-index setter has no native bounds check; adjacency beyond210 is unproven.
// samp.dll RVA: 0x002a9a50
extern DWORD g_gamePlayerFlags[210];
// samp.dll RVA: 0x000b40c0
void __stdcall GameSetPlayerFlag(BYTE player,DWORD value);
// samp.dll RVA: 0x0011eea0
extern const Scripting::OpcodeInfo g_pedCreatePlayerCommand;
// samp.dll RVA: 0x0011eeb4
extern const Scripting::OpcodeInfo g_pedGetPlayerHandleCommand;
// samp.dll RVA: 0x0011f0e4
extern const Scripting::OpcodeInfo g_pedCommand03fe;
// samp.dll RVA: 0x0011f388
extern const float g_pedSpawnLift;

// samp.dll RVA: 0x0011f094
extern const Scripting::OpcodeInfo g_pedBehaviorWalkStyleCommand;

// samp.dll RVA: 0x0011f030
extern const Scripting::OpcodeInfo g_pedBehaviorDrivebyCommand;

// samp.dll RVA: 0x0011efa4
extern const Scripting::OpcodeInfo g_pedBehaviorCreateObjectCommand;

// samp.dll RVA: 0x0011f120
extern const Scripting::OpcodeInfo g_pedBehaviorAttachCommand;

// samp.dll RVA: 0x0011f134
extern const Scripting::OpcodeInfo g_pedBehaviorObjectFlagCommand;

// samp.dll RVA: 0x0011f170
extern const Scripting::OpcodeInfo g_pedBehaviorObjectScalarCommand;

// samp.dll RVA: 0x0011f10c
extern const Scripting::OpcodeInfo g_pedBehaviorObjectAnimationCommand;

// samp.dll RVA: 0x0011efb8
extern const Scripting::OpcodeInfo g_pedBehaviorEffectCommand;

// samp.dll RVA: 0x0011f314
extern const char g_pedParachuteOpen[10];

// samp.dll RVA: 0x0011f308
extern const char g_pedParachuteDecel[11];

// samp.dll RVA: 0x0011f2ec
extern const char g_pedParachuteFloat[11];

// samp.dll RVA: 0x0011f2d0
extern const char g_pedParachuteSteerRight[12];

// samp.dll RVA: 0x0011f2b4
extern const char g_pedParachuteSteerLeft[12];

// samp.dll RVA: 0x0011f294
extern const char g_pedParachuteLandWater[16];

// samp.dll RVA: 0x0011f268
extern const char g_pedParachuteOpenObject[12];

// samp.dll RVA: 0x0011f2f8
extern const char g_pedParachuteDecelObject[13];

// samp.dll RVA: 0x0011f2dc
extern const char g_pedParachuteFloatObject[13];

// samp.dll RVA: 0x0011f2c0
extern const char g_pedParachuteSteerRightObject[14];

// samp.dll RVA: 0x0011f2a4
extern const char g_pedParachuteSteerLeftObject[14];

// samp.dll RVA: 0x0011f280
extern const char g_pedParachuteLandWaterObject[18];

// samp.dll RVA: 0x0011a928
extern const double g_pedParachuteOne;

// samp.dll RVA: 0x0011f278
extern const double g_pedParachuteOpeningSpeed;

// samp.dll RVA: 0x0011f260
extern const double g_pedDrivebyDistance;

// samp.dll RVA: 0x0011f3a8
extern const double g_pedStuffOffset1X;

// samp.dll RVA: 0x0011f3b0
extern const double g_pedStuffOffset1Y;

// samp.dll RVA: 0x0011f3b8
extern const double g_pedStuffOffset1Z;

// samp.dll RVA: 0x0011f3a0
extern const double g_pedStuffOffset2Z;

// samp.dll RVA: 0x0011f390
extern const double g_pedStuffOffset3X;

// samp.dll RVA: 0x0011c988
extern const double g_pedStuffOffset3Y;

// samp.dll RVA: 0x0011f398
extern const double g_pedStuffOffset3Z;
