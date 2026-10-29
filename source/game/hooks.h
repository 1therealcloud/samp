#ifndef SAMP_GAME_HOOKS_H
#define SAMP_GAME_HOOKS_H
#include <windows.h>

class CPlayerPool;
class CMatrix;
struct CVector;
struct PedBulletData;
// 0x0018f624
extern CPlayerPool* g_hookDamagePlayerPool;
// 0x00151afc
extern WORD g_hookDamagePlayerId;
// 0x0018f628
extern DWORD g_hookVehicleDamageAttacker;
// 0x0018f62c
extern DWORD g_hookVehicleDamageReceiver;
// 0x0018f630
extern DWORD g_hookVehicleDamageWord;
// 0x0018f634
extern DWORD g_hookVehicleDamageBlocked;
// 0x0018f638
extern DWORD g_hookVehicleDamageReturn;
// 0x0018f644
extern void* g_hookBulletHitEntity;
// 0x0018e590
extern PedBulletData g_hookBulletHitData;
// 0x0018e828
extern CMatrix g_hookBulletInverseMatrix;
// 0x0018ea60
extern CMatrix g_hookBulletOrthoMatrix;
// Shared native DWORD slots at18F66C..18F688; the three-argument hook reuses slot0.
// 0x0018f66c
extern DWORD g_hookShotArguments[8];
// 0x0018eb7c
extern CVector g_hookAdjustedShotPoint;
// 0x0018f648
extern DWORD g_hookShotLineResult;
// 0x000a4fa0
BOOL GameShouldBlockVehicleDamage(void* vehicle, void* attacker, DWORD unused);
// Second pointer is deliberately unused; native RET16.
// 0x000a5170
void __stdcall GameRecordBulletHit(const CVector* origin, const CVector* target, const CVector* collision, void** entity);
// Raw cdecl GTA adapter retains the full backend EAX result.
// 0x000a5360
DWORD GameProcessShotLineOfSight(const CVector* origin, CVector* target, void* collision, void** entity, DWORD buildings, DWORD vehicles, DWORD peds, DWORD objects, DWORD dummies,
                                 DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough);
// Raw GTA callback ABIs; address-only declarations, never ordinary void() calls.
// 0x000a5090
void GameHook_000a5090();
// 0x000a5540
void GameHook_000a5540();
// 0x000a5600
void GameHook_000a5600();
// 0x000a5650
void GameHook_000a5650();
// 0x002ac9cc
extern DWORD g_gameLoopAddress;
// 0x0018f56c
extern DWORD g_gameHookFarClipReturn;
// 0x00151a10
extern BYTE g_gameHookJump00151a10[10];
// 0x00151a1c
extern BYTE g_gameHookJump00151a1c[7];
// 0x0015197c
extern BYTE g_gameHookJump0015197c[7];
// 0x00151984
extern BYTE g_gameHookJump00151984[7];
// 0x00151994
extern BYTE g_gameHookJump00151994[7];
// 0x0015198c
extern BYTE g_gameHookJump0015198c[7];
// 0x001519b0
extern BYTE g_gameHookJump001519b0[7];
// 0x001519b8
extern BYTE g_gameHookJump001519b8[6];
// 0x001519e8
extern BYTE g_gameHookJump001519e8[6];
// 0x001519f0
extern BYTE g_gameHookJump001519f0[6];
// 0x001519d4
extern BYTE g_gameHookJump001519d4[9];
// 0x00151a34
extern BYTE g_gameHookJump00151a34[6];
// 0x00151a3c
extern BYTE g_gameHookJump00151a3c[7];
// 0x00151a44
extern BYTE g_gameHookJump00151a44[9];
// 0x00151a50
extern BYTE g_gameHookJump00151a50[6];
// 0x00151a68
extern BYTE g_gameHookJump00151a68[6];
// Raw callback declarations for installing addresses. Recovered bodies and exact
// entry ABIs are tracked in the callback review; remaining bodies stay open.
// Do not call these through the provisional void() types from ordinary C++.
// 0x000a2200
void GameHook_000a2200();
// 0x000a22c0
void GameHook_000a22c0();
// 0x000a22f0
void GameHook_000a22f0();
// 0x000a2370
void GameHook_000a2370();
// 0x000a23a0
void GameHook_000a23a0();
// 0x000a23d0
void GameHook_000a23d0();
// 0x000a26d0
void GameHook_000a26d0();
// 0x000a29d0
void GameHook_000a29d0();
// 0x000a2ab0
void GameHook_000a2ab0();
// 0x000a2b50
void GameHook_000a2b50();
// 0x000a2e70
void GameHook_000a2e70();
// 0x000a33c0
void GameHook_000a33c0();
// 0x000a3580
void GameHook_000a3580();
// 0x000a35e0
void GameHook_000a35e0();
// 0x000a37a0
void GameHook_000a37a0();
// 0x000a37c0
void GameHook_000a37c0();
// 0x000a3800
void GameHook_000a3800();
// 0x000a3a70
void GameHook_000a3a70();
// 0x000a3ac0
void GameHook_000a3ac0();
// 0x000a3ad0
void GameHook_000a3ad0();
// 0x000a3b00
void GameHook_000a3b00();
// 0x000a3b20
void GameHook_000a3b20();
// 0x000a3b90
void GameHook_000a3b90();
// 0x000a3c60
void GameHook_000a3c60();
// 0x000a3c80
void GameHook_000a3c80();
// 0x000a3ca0
void GameHook_000a3ca0();
// 0x000a3ce0
void GameHook_000a3ce0();
// 0x000a3d20
void GameHook_000a3d20();
// 0x000a3d90
void GameHook_000a3d90();
// 0x000a3e50
void GameHook_000a3e50();
// Raw installed ABI: no arguments, returns the preview backing pointer in EAX.
// The void() declaration is an installation-address type, not a calling prototype.
// 0x000a3ec0
void GameHook_000a3ec0();
// 0x000a3f30
void GameHook_000a3f30();
// 0x000a3fe0
void GameHook_000a3fe0();
// 0x000a4190
void GameHook_000a4190();
// 0x000a4280
void GameHook_000a4280();
// 0x000a42c0
void GameHook_000a42c0();
// 0x000a42d0
void GameHook_000a42d0();
// 0x000a46b0
void GameHook_000a46b0();
// 0x000a4770
void GameHook_000a4770();
// 0x000a47d0
void GameHook_000a47d0();
// 0x000a4850
void GameHook_000a4850();
// 0x000a48c0
void GameHook_000a48c0();
// 0x000a4960
void GameHook_000a4960();
// 0x000a49d0
void GameHook_000a49d0();
// 0x000a4c30
void GameHook_000a4c30();
// 0x000a4df0
void GameHook_000a4df0();
// 0x000a4e30
void GameHook_000a4e30();
// 0x000a4e70
void GameHook_000a4e70();
// 0x000a5100
void GameHook_000a5100();
// 0x000a5150
void GameHook_000a5150();
// 0x000a56b0
void GameHook_000a56b0();
// 0x000a5820
void GameHook_000a5820();
// 0x000a5850
void GameHook_000a5850();
// 0x000a5870
void GameHook_000a5870();
// 0x000a5910
void GameHook_000a5910();
// 0x000a59b0
void GameHook_000a59b0();
// 0x000a5a50
void GameHook_000a5a50();
// 0x000a5af0
void GameHook_000a5af0();
// 0x000a5b90
void GameHook_000a5b90();
// 0x000a5d20
void GameHook_000a5d20();
// 0x000a5d80
void GameInstallHook(DWORD address, DWORD function, DWORD storage, const BYTE* jumpCode, int size);
// 0x000a5e30
void GamePrepareHooks();
// 0x000a6020
void GameInstallHooks();
// Raw GTA callback; not an ordinary void() call interface.
// 0x000a5bf0
void GameHook_000a5bf0();
// 0x000a6d30
void GameInstallModelInfoHook();
// 0x00151a60
extern BYTE g_gameCustomModelHookJump[6];

// 0x000a4200
void GameReleaseCachedRasterBuffer();

// 0x000a2690
void GameDisablePedRemoteCall();
// 0x000a26b0
void GameRestorePedRemoteCall();

// 0x000a5d40
void GameWriteProtectedDword(DWORD address, DWORD value);
// 0x000a5df0
void GameWriteProtectedRelativeBranch(DWORD address, DWORD target, BYTE opcode);

// Raw GameHook_* declarations below are installation addresses, not ordinary
// C++ call interfaces: native register inputs, stack prologues and tail jumps.
// 0x000a22a0
void GameRestoreHudDraw();
// 0x000a2630
void GameDisablePedControlCalls();
// 0x000a2650
void GameRestorePedControlCalls();
// 0x000a3780
void GameHook_000a3780();
// 0x000a37e0
void GameHook_000a37e0();
// 0x000a3b40
void GameHook_000a3b40();
// 0x000a3d50
void GameHook_000a3d50();
// 0x000a3d70
void GameHook_000a3d70();
// 0x000a3e40
void GameHook_000a3e40();
// 0x000a4240
void* GameAllocateHookBuffer();
// 0x000a4c10
void GameHook_000a4c10();
// 0x000a4c90
void GameHook_000a4c90();
// 0x000a4cb0
void GameHook_000a4cb0();
// 0x000a4170
void GameHook_000a4170();
// 0x000a48a0
void GameHook_000a48a0();
// 0x0018f4d0
extern DWORD g_hookGate18F4D0;
// 0x0018eb2c
extern DWORD g_hookRandomReturn;
// 0x0018e8b8
extern DWORD g_hookRecordReturn18E8B8;
// 0x0018f5c8
extern DWORD g_hookRecordEcx18F5C8;

// Additional raw hook entries: address-only declarations, not C++ call interfaces.
// 0x0011ebe4
extern const char g_hookPedSayFormat[];
// 0x0011ec04
extern const char g_hookGamepadAimFormat[];
// 0x00151aac
extern DWORD g_hookCapture00151AAC;
// 0x00151ab0
extern DWORD g_hookCapture00151AB0;
// 0x00151ab4
extern WORD g_hookCapture00151AB4;
// 0x00151ac4
extern WORD g_hookCapture00151AC4;
// 0x0018e540
extern DWORD g_hookCapture0018E540;
// 0x0018e544
extern DWORD g_hookCapture0018E544;
// 0x0018e564
extern DWORD g_hookCapture0018E564;
// 0x0018e588
extern DWORD g_hookCapture0018E588;
// 0x0018e58c
extern DWORD g_hookCapture0018E58C;
// 0x0018e5c4
extern DWORD g_hookCapture0018E5C4;
// 0x0018e5d8
extern DWORD g_hookCapture0018E5D8;
// 0x0018e800
extern CVector g_hookAimRayTarget;
// 0x0018e814
extern DWORD g_hookCapture0018E814;
// 0x0018e818
extern DWORD g_hookCapture0018E818;
// 0x0018e86c
extern DWORD g_hookCapture0018E86C;
// 0x0018e874
extern DWORD g_hookCapture0018E874;
// 0x0018e8c8
extern DWORD g_hookCapture0018E8C8;
// 0x0018e8d4
extern DWORD g_hookCapture0018E8D4;
// 0x0018e924
extern DWORD g_hookCapture0018E924;
// 0x0018e928
extern DWORD g_hookCapture0018E928;
// 0x0018ea40
extern CVector g_hookAimRayOrigin;
// 0x0018eaa0
extern DWORD g_hookCapture0018EAA0;
// 0x0018eaa4
extern DWORD g_hookCapture0018EAA4;
// 0x0018eaa8
extern DWORD g_hookCapture0018EAA8;
// 0x0018eab0
extern DWORD g_hookCapture0018EAB0;
// 0x0018eb1c
extern DWORD g_hookCapture0018EB1C;
// 0x0018eb70
extern DWORD g_hookCapture0018EB70;
// 0x0018eb78
extern DWORD g_hookCapture0018EB78;
// 0x0018eb8c
extern DWORD g_hookCapture0018EB8C;
// 0x0018f33c
extern DWORD g_hookCapture0018F33C;
// 0x0018f340
extern DWORD g_hookCapture0018F340;
// 0x0018f34c
extern DWORD g_hookCapture0018F34C;
// 0x0018f350
extern DWORD g_hookCapture0018F350;
// 0x0018f354
extern DWORD g_hookCapture0018F354;
// 0x0018f360
extern DWORD g_hookCapture0018F360;
// 0x0018f368
extern DWORD g_hookCapture0018F368;
// 0x0018f37c
extern DWORD g_hookCapture0018F37C;
// 0x0018f558
extern DWORD g_hookCapture0018F558;
// 0x0018f55c
extern DWORD g_hookCapture0018F55C;
// 0x0018f594
extern DWORD g_hookCapture0018F594;
// 0x0018f598
extern DWORD g_hookCapture0018F598;
// 0x0018f59c
extern DWORD g_hookCapture0018F59C;
// 0x0018f5a0
extern DWORD g_hookCapture0018F5A0;
// 0x0018f5e4
extern DWORD g_hookCapture0018F5E4;
// 0x0018f658
extern DWORD g_hookCapture0018F658;
// 0x0018f65c
extern DWORD g_hookCapture0018F65C;
// 0x0018f660
extern DWORD g_hookCapture0018F660;
// 0x000a2cd0
void GameHook_000a2cd0();
// 0x000a3b60
void GameHook_000a3b60();
// 0x000a3bc0
void GameHook_000a3bc0();
// 0x000a3e70
void GameHook_000a3e70();
// 0x000a40a0
void GameHook_000a40a0();
// 0x000a4110
void GameHook_000a4110();
// 0x000a43d0
void GameHook_000a43d0();
// 0x000a4450
void GameHook_000a4450();
// 0x000a4aa0
void GameHook_000a4aa0();
// 0x000a4b20
void GameHook_000a4b20();
// 0x000a4b80
void GameHook_000a4b80();
// 0x000a5290
void GameHook_000a5290();
// 0x000a52d0
void GameHook_000a52d0();

// Raw installation-only entries; register/stack ABI is documented in analysis.
// 0x0018e878
extern CMatrix g_hookAimOffsetMatrix;
// 0x0018ead8
extern CMatrix g_hookMountedMatrix;
// 0x00151ad4
extern DWORD g_hookPatchOverride151AD4[3];
// 0x00151ae0
extern BYTE g_hookRestore5543C0[22];
// 0x00151af8
extern WORD g_hookRestore5543DC;
// 0x0018e548
extern DWORD g_hookCapture0018E548;
// 0x0018e54c
extern DWORD g_hookCapture0018E54C;
// 0x0018e7e4
extern DWORD g_hookCapture0018E7E4;
// 0x0018e7e8
extern DWORD g_hookCapture0018E7E8;
// 0x0018e7ec
extern DWORD g_hookCapture0018E7EC;
// 0x0018e80c
extern DWORD g_hookCapture0018E80C;
// 0x0018e92c
extern DWORD g_hookCapture0018E92C;
// 0x0018eaac
extern DWORD g_hookCapture0018EAAC;
// 0x0018eb74
extern DWORD g_hookCapture0018EB74;
// 0x0018f338
extern DWORD g_hookCapture0018F338;
// 0x0018f344
extern DWORD g_hookCapture0018F344;
// 0x0018f348
extern DWORD g_hookCapture0018F348;
// 0x0018f358
extern DWORD g_hookCapture0018F358;
// 0x0018f35c
extern DWORD g_hookCapture0018F35C;
// 0x0018f364
extern DWORD g_hookCapture0018F364;
// 0x0018f380
extern DWORD g_hookCapture0018F380;
// 0x0018f48c
extern DWORD g_hookCapture0018F48C;
// 0x0018f490
extern DWORD g_hookCapture0018F490;
// 0x0018f608
extern DWORD g_hookCapture0018F608;
// 0x0018f60c
extern DWORD g_hookCapture0018F60C;
// 0x0018f610
extern DWORD g_hookCapture0018F610;
// 0x000a2d30
void GameHook_000a2d30();
// 0x000a2de0
void GameHook_000a2de0();
// 0x000a4510
void GameHook_000a4510();
// 0x000a4680
void GameHook_000a4680();
// 0x000a4cc0
void GameHook_000a4cc0();

// Raw ABI entries are declared in hooks.h for address installation only.
// Recovered bodies are naked because callers supply registers or an in-flight
// GTA stack frame, and some paths tail-jump into the original game function.
// 0x0018f4c8
extern DWORD g_hookRadarIndex;
// 0x0018f4cc
extern DWORD g_hookCheatFunction;
// 0x0018f4b0
extern DWORD g_hookCameraPed;
// 0x0018f540
extern DWORD g_hookRenderEsi;
// 0x0018f544
extern DWORD g_hookRenderEbx;
// 0x0018f568
extern DWORD g_hookFreedBuffer;
// 0x00151ab8
extern DWORD g_hookKeepBuffer;
// 0x0018f560
extern DWORD g_hookCachedBuffer;
// 0x00151abc
extern DWORD g_hookAllocateFunction;
// 0x0018f570
extern DWORD g_hookSavedEsi;
// 0x0018f5c4
extern DWORD g_hookRenderReturn;
// 0x00151ac8
extern BYTE g_hookRenderRestore[3];
// 0x0018f5d4
extern DWORD g_hookSavedFontA;
// 0x0018f5d8
extern DWORD g_hookSavedFontB;

// 0x0018f504
extern DWORD g_hookHudScaleX;
// 0x0018f508
extern DWORD g_hookHudScaleY;
// 0x0018f50c
extern float g_hookHudWidth;
// 0x0018f510
extern float g_hookHudHeight;
// 0x00151a6e
extern BYTE g_hookHudAspectFix;
// 0x0011c728
extern const float g_hookHudAspectThreshold;
// 0x0011eb68
extern const float g_hookHudScaleNumerator;
// 0x002aca6c
extern DWORD g_hookFogEnabled;
// 0x002aca70
extern DWORD g_hookDisableFog;
// 0x0018e560
extern DWORD g_hookCollisionPed;
// 0x0018e8e0
extern DWORD g_hookSavedMatrix[16];
// 0x0018eab4
extern DWORD g_hookSavedSpeed[3];
// 0x0018f52c
extern DWORD g_hookHornArgument;
// 0x0018f530
extern DWORD g_hookHornThis;
// 0x0018eb24
extern DWORD g_hookHornVehicle;
// 0x0018f534
extern BYTE g_hookHornStatus;
// 0x0018f4c4
extern DWORD g_hookAnimationReturn;
// 0x0018e584
extern DWORD g_hookAnimationClump;
// 0x0018f548
extern DWORD g_hookAnimationGroup;
// 0x0018f54c
extern DWORD g_hookAnimationId;
// 0x0018f550
extern DWORD g_hookAnimationBlend;
// 0x0018eb90
extern DWORD g_hookAnimationPed;
// 0x0018eb88
extern DWORD g_hookAnimationLocalClump;
// 0x0018f4ec
extern DWORD g_hookAnimationType;
// 0x0018f4f0
extern DWORD g_hookQueuedAnimationGroup;
// 0x0018f4f4
extern DWORD g_hookQueuedAnimationId;
// 0x0018f4f8
extern DWORD g_hookQueuedAnimationBlend;
// 0x00151aa4
extern DWORD g_hookAnimationContinueA;
// 0x00151aa8
extern DWORD g_hookAnimationContinueB;
// 0x0018f5cc
extern DWORD g_hookRenderObject;
// 0x00151acc
extern BYTE g_hookObjectRestore[5];
// 0x0018f5b4
extern DWORD g_hookMarker;
// 0x0018f5b8
extern DWORD g_hookMarkerArgument;
// 0x0018f5bc
extern DWORD g_hookSuppressAllMarkers;
// 0x000a2220
void GamePrepareHudDraw();
// 0x000c5400
void GameSetupD3DFog(DWORD enabled);

// 0x0018f514
extern BYTE g_hookCameraHudPrepared;
// 0x0018f6a8
extern DWORD g_hookAudioSavedState;
// 0x0018f6ac
extern DWORD g_hookAudioArgument0;
// 0x0018f6b0
extern DWORD g_hookAudioArgument1;

// 0x00151964
extern WORD* g_hookCameraModeWord;
// 0x0018f4ac
extern BYTE g_hookSavedPlayer;
// 0x0018f4b4
extern BYTE g_hookPlayerIndex;
// 0x0018f4c0
extern DWORD g_hookPedControlThis;
// 0x0018f51c
extern DWORD g_hookWeaponResult;
// 0x0018f520
extern DWORD g_hookRestoreSkills;
// 0x0018f524
extern DWORD g_hookLocalPedControl;
// 0x0018f528
extern DWORD g_hookRestoreKeys;
// 0x0018ea58
extern BYTE g_hookSavedCameraMode;
// 0x0018e5c8
extern WORD g_hookSavedCameraModeWord;
// 0x0018e810
extern DWORD g_hookVehicleProcess;
// 0x0018e8d0
extern DWORD g_hookControlVehicle;
// 0x0018ea54
extern DWORD g_hookVehicleVtable;
// 0x0018e934
extern DWORD g_hookVehicleKeys;
// 0x0018e938
extern BYTE g_hookVehicleStatus;
// 0x0018eb20
extern DWORD g_hookVehicleLean;
// 0x0018e8cc
extern DWORD g_hookVehicleLeanTarget;
// 0x002a9a44
extern int g_gameVehicleModel;
// 0x002a9a48
extern DWORD g_gameLinkedVehicle;
// 0x002a9a4c
extern float g_gameVehicleDistanceSquared;
// 0x002a9da0
extern DWORD g_gameLocalPedMatrix;
// 0x002a9da4
extern DWORD g_gameVehicleMatrix;
// 0x0011f5dc
extern const float g_gameStreamingDistanceSquared;
// 0x000b3f60
BYTE __stdcall GameFindPlayerPedIndex(void* ped);
// 0x000b4410
BOOL __stdcall GameIsTrainVehicle(void* vehicle);
// 0x000b4460
BOOL __stdcall GameHasOccupiedLinkedVehicle(void* vehicle);
// 0x000b6490
BOOL __stdcall GameIsVehicleInStreamingArea(void* vehicle);

class CObjectPool;
// 0x0018de44
extern CObjectPool* g_debugObjectPool;
class CVehicle;
class CVehiclePool;
class CObject;
struct CVector;
// 0x0018e7f8
extern DWORD g_hookPickupBackend;
// 0x0018e5cc
extern DWORD g_hookMenuTextResult;
// 0x0018f5a8
extern CVector g_hookPassengerCameraPosition;
// 0x0018e580
extern DWORD g_hookTrailerBackend;
// 0x0018e7fc
extern CVehiclePool* g_hookTrailerPool;
// 0x0018e7f0
extern WORD g_hookTrailerId;
// 0x0018f5e0
extern CVehicle* g_hookTrailerVehicle;
// 0x0018f600
extern DWORD g_hookObjectSavedStack;
// 0x0018f5f8
extern DWORD g_hookObjectBackend;
// 0x0018f604
extern DWORD g_hookObjectResult;
// 0x0018f5fc
extern CObject* g_hookObjectRotationWrapper;
// 0x0018f614
extern CObjectPool* g_hookObjectDrawPool;
// 0x0018f618
extern DWORD g_hookObjectDrawBackend;
// 0x0018f578
extern CObject* g_hookObjectDrawWrapper;
class CPlayerPool;
class CRemotePlayer;
struct GameEntityCollisionShape;
// 0x0018f580
extern DWORD g_hookMaterialReturn;
// 0x0018f574
extern DWORD g_hookMaterialBackend;
// 0x0018f57c
extern CPlayerPool* g_hookMaterialPlayerPool;
// 0x0018f5a4
extern DWORD g_hookSurfingIndex;
// 0x0018e7f4
extern CRemotePlayer* g_hookSurfingRemote;
// 0x0018eac0
extern GameEntityCollisionShape g_hookCollisionFirst;
// 0x0018e56c
extern GameEntityCollisionShape g_hookCollisionSecond;
// 0x0018f68c
extern BYTE g_hookCollisionResult;
// 0x0011e508
extern const float g_hookCollisionBoundHigh;
// 0x0011ec3c
extern const float g_hookCollisionBoundLow;

// 0x0018f690
extern DWORD g_hookContactThis;
// 0x0018f694
extern DWORD g_hookContactOther;
// 0x0018f698
extern DWORD g_hookContactReturn;
// 0x0018f69c
extern DWORD g_hookSirenVehicle;
// 0x0018f6a0
extern BYTE g_hookSirenResult;
// 0x0018f6a4
extern DWORD g_hookSirenOnce;
// 0x00151b0c
extern WORD g_hookSirenVehicleId;
// 0x0018f63c
extern DWORD g_hookPedQueryThis;
// 0x0018f640
extern DWORD g_hookPedQueryPed;

// 0x002aca50
extern DWORD g_clientPreviousRenderState;

// 0x002aca54
extern DWORD g_clientRenderCaller;

// 0x002aca58
extern DWORD g_clientRenderState;

// 0x002aca5c
extern DWORD g_clientRenderValue;

// Installs the recovered callback into the current RenderWare engine table.
// 0x000c54c0
void ClientInstallRenderStateHook();

// Naked two-word GTA render-state callback; tail-jumps through original target.
// 0x000c5450
void ClientRenderStateHook();

class CLocalPlayer;
// 0x0018e8c0
extern DWORD g_hookPedControlField540;
// 0x0018f378
extern CPlayerPool* g_hookFaultPlayerPool;
// 0x00151970
extern WORD g_hookFaultPlayerId;
// 0x0018e824
extern CRemotePlayer* g_hookFaultRemotePlayer;
// 0x00151a70
extern const BYTE g_hookPedRestoreA[6];
// 0x00151a88
extern const BYTE g_hookPedRestoreB[6];
// 0x00151a90
extern const BYTE g_hookPedRemoteRestore[5];
// 0x0011ebcc
extern const char g_hookPedExceptionLabel[10];
// 0x0011ebac
extern const char g_hookPedRemovedMessage[32];
// 0x000a2530
void GameRunPedControlBackend();
// samp.dll RVA: unknown (reconstruction-only body helper extracted from A26D0; no standalone native entry)
void GameProcessPedControlBody();
// Native policy helpers; their protocol send methods remain separate dependencies.
// 0x000a3430
BOOL GameNotifyEnterVehicle();
// 0x000a3910
BOOL __stdcall GameNotifyPedDamage(void* response, void* ped);
// 0x0018f4a8
extern DWORD g_hookIgnoreNextExit;
// 0x0018e550
extern DWORD g_hookExitTask;
// 0x0018e5d4
extern CVehiclePool* g_hookEventVehiclePool;
// 0x0018e568
extern WORD g_hookEventVehicleId;
// 0x0018e868
extern CVehicle* g_hookExitVehicle;
// 0x0018eb18
extern CLocalPlayer* g_hookEventLocalPlayer;
// 0x0018eb30
extern CMatrix g_hookExitMatrix;
// 0x00151aa0
extern DWORD g_hookExitTaskArgument;
// 0x0011c65c
extern const float g_hookTrainExitOffset;
// 0x0018e5bc
extern DWORD g_hookQuitDeadline;
// 0x0018f538
extern DWORD g_hookDamageResponse;
// 0x0018f53c
extern DWORD g_hookDamagePed;
// 0x000a3840
WORD GameResolveDamagePlayer(void* entity);
// 0x000a38e0
WORD GameResolveDamageActor(void* entity);
// 0x0018e81c
extern CVehicle* g_hookEnterVehicle;
#endif
