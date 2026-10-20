#ifndef SAMP_RECOVERED_PED_H
#define SAMP_RECOVERED_PED_H
#include "game/entity.h"
struct CameraAim;
#pragma pack(push,1)
struct PedAccessoryState { BYTE opaque[52]; };
// samp.dll vtable RVA: 0x0011f24c
class CPed : public CEntity {
public:
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
#endif
