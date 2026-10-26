#ifndef SAMP_GAME_HOOKS_H
#define SAMP_GAME_HOOKS_H
#include <windows.h>
class CPlayerPool;
class CMatrix;
struct CVector;
struct PedBulletData;
// samp.dll RVA: 0x0018f624
extern CPlayerPool *g_hookDamagePlayerPool;
// samp.dll RVA: 0x00151afc
extern WORD g_hookDamagePlayerId;
// samp.dll RVA: 0x0018f628
extern DWORD g_hookVehicleDamageAttacker;
// samp.dll RVA: 0x0018f62c
extern DWORD g_hookVehicleDamageReceiver;
// samp.dll RVA: 0x0018f630
extern DWORD g_hookVehicleDamageWord;
// samp.dll RVA: 0x0018f634
extern DWORD g_hookVehicleDamageBlocked;
// samp.dll RVA: 0x0018f638
extern DWORD g_hookVehicleDamageReturn;
// samp.dll RVA: 0x0018f644
extern void *g_hookBulletHitEntity;
// samp.dll RVA: 0x0018e590
extern PedBulletData g_hookBulletHitData;
// samp.dll RVA: 0x0018e828
extern CMatrix g_hookBulletInverseMatrix;
// samp.dll RVA: 0x0018ea60
extern CMatrix g_hookBulletOrthoMatrix;
// Shared native DWORD slots at18F66C..18F688; the three-argument hook reuses slot0.
// samp.dll RVA: 0x0018f66c
extern DWORD g_hookShotArguments[8];
// samp.dll RVA: 0x0018eb7c
extern CVector g_hookAdjustedShotPoint;
// samp.dll RVA: 0x0018f648
extern DWORD g_hookShotLineResult;
// samp.dll RVA: 0x000a4fa0
BOOL GameShouldBlockVehicleDamage(void *vehicle,void *attacker,DWORD unused);
// Second pointer is deliberately unused; native RET16.
// samp.dll RVA: 0x000a5170
void __stdcall GameRecordBulletHit(const CVector *origin,const CVector *target,const CVector *collision,void **entity);
// Raw cdecl GTA adapter retains the full backend EAX result.
// samp.dll RVA: 0x000a5360
DWORD GameProcessShotLineOfSight(const CVector *origin,CVector *target,void *collision,void **entity,
    DWORD buildings,DWORD vehicles,DWORD peds,DWORD objects,DWORD dummies,DWORD seeThrough,DWORD cameraIgnore,DWORD shootThrough);
// Raw GTA callback ABIs; address-only declarations, never ordinary void() calls.
// samp.dll RVA: 0x000a5090
void GameHook_000a5090();
// samp.dll RVA: 0x000a5540
void GameHook_000a5540();
// samp.dll RVA: 0x000a5600
void GameHook_000a5600();
// samp.dll RVA: 0x000a5650
void GameHook_000a5650();
// samp.dll RVA: 0x002ac9cc
extern DWORD g_gameLoopAddress;
// samp.dll RVA: 0x0018f56c
extern DWORD g_gameHookFarClipReturn;
// samp.dll RVA: 0x00151a10
extern BYTE g_gameHookJump00151a10[10];
// samp.dll RVA: 0x00151a1c
extern BYTE g_gameHookJump00151a1c[7];
// samp.dll RVA: 0x0015197c
extern BYTE g_gameHookJump0015197c[7];
// samp.dll RVA: 0x00151984
extern BYTE g_gameHookJump00151984[7];
// samp.dll RVA: 0x00151994
extern BYTE g_gameHookJump00151994[7];
// samp.dll RVA: 0x0015198c
extern BYTE g_gameHookJump0015198c[7];
// samp.dll RVA: 0x001519b0
extern BYTE g_gameHookJump001519b0[7];
// samp.dll RVA: 0x001519b8
extern BYTE g_gameHookJump001519b8[6];
// samp.dll RVA: 0x001519e8
extern BYTE g_gameHookJump001519e8[6];
// samp.dll RVA: 0x001519f0
extern BYTE g_gameHookJump001519f0[6];
// samp.dll RVA: 0x001519d4
extern BYTE g_gameHookJump001519d4[9];
// samp.dll RVA: 0x00151a34
extern BYTE g_gameHookJump00151a34[6];
// samp.dll RVA: 0x00151a3c
extern BYTE g_gameHookJump00151a3c[7];
// samp.dll RVA: 0x00151a44
extern BYTE g_gameHookJump00151a44[9];
// samp.dll RVA: 0x00151a50
extern BYTE g_gameHookJump00151a50[6];
// samp.dll RVA: 0x00151a68
extern BYTE g_gameHookJump00151a68[6];
// Raw callback declarations for installing addresses. Recovered bodies and exact
// entry ABIs are tracked in the callback review; remaining bodies stay open.
// Do not call these through the provisional void() types from ordinary C++.
// samp.dll RVA: 0x000a2200
void GameHook_000a2200();
// samp.dll RVA: 0x000a22c0
void GameHook_000a22c0();
// samp.dll RVA: 0x000a22f0
void GameHook_000a22f0();
// samp.dll RVA: 0x000a2370
void GameHook_000a2370();
// samp.dll RVA: 0x000a23a0
void GameHook_000a23a0();
// samp.dll RVA: 0x000a23d0
void GameHook_000a23d0();
// samp.dll RVA: 0x000a26d0
void GameHook_000a26d0();
// samp.dll RVA: 0x000a29d0
void GameHook_000a29d0();
// samp.dll RVA: 0x000a2ab0
void GameHook_000a2ab0();
// samp.dll RVA: 0x000a2b50
void GameHook_000a2b50();
// samp.dll RVA: 0x000a2e70
void GameHook_000a2e70();
// samp.dll RVA: 0x000a33c0
void GameHook_000a33c0();
// samp.dll RVA: 0x000a3580
void GameHook_000a3580();
// samp.dll RVA: 0x000a35e0
void GameHook_000a35e0();
// samp.dll RVA: 0x000a37a0
void GameHook_000a37a0();
// samp.dll RVA: 0x000a37c0
void GameHook_000a37c0();
// samp.dll RVA: 0x000a3800
void GameHook_000a3800();
// samp.dll RVA: 0x000a3a70
void GameHook_000a3a70();
// samp.dll RVA: 0x000a3ac0
void GameHook_000a3ac0();
// samp.dll RVA: 0x000a3ad0
void GameHook_000a3ad0();
// samp.dll RVA: 0x000a3b00
void GameHook_000a3b00();
// samp.dll RVA: 0x000a3b20
void GameHook_000a3b20();
// samp.dll RVA: 0x000a3b90
void GameHook_000a3b90();
// samp.dll RVA: 0x000a3c60
void GameHook_000a3c60();
// samp.dll RVA: 0x000a3c80
void GameHook_000a3c80();
// samp.dll RVA: 0x000a3ca0
void GameHook_000a3ca0();
// samp.dll RVA: 0x000a3ce0
void GameHook_000a3ce0();
// samp.dll RVA: 0x000a3d20
void GameHook_000a3d20();
// samp.dll RVA: 0x000a3d90
void GameHook_000a3d90();
// samp.dll RVA: 0x000a3e50
void GameHook_000a3e50();
// Raw installed ABI: no arguments, returns the preview backing pointer in EAX.
// The void() declaration is an installation-address type, not a calling prototype.
// samp.dll RVA: 0x000a3ec0
void GameHook_000a3ec0();
// samp.dll RVA: 0x000a3f30
void GameHook_000a3f30();
// samp.dll RVA: 0x000a3fe0
void GameHook_000a3fe0();
// samp.dll RVA: 0x000a4190
void GameHook_000a4190();
// samp.dll RVA: 0x000a4280
void GameHook_000a4280();
// samp.dll RVA: 0x000a42c0
void GameHook_000a42c0();
// samp.dll RVA: 0x000a42d0
void GameHook_000a42d0();
// samp.dll RVA: 0x000a46b0
void GameHook_000a46b0();
// samp.dll RVA: 0x000a4770
void GameHook_000a4770();
// samp.dll RVA: 0x000a47d0
void GameHook_000a47d0();
// samp.dll RVA: 0x000a4850
void GameHook_000a4850();
// samp.dll RVA: 0x000a48c0
void GameHook_000a48c0();
// samp.dll RVA: 0x000a4960
void GameHook_000a4960();
// samp.dll RVA: 0x000a49d0
void GameHook_000a49d0();
// samp.dll RVA: 0x000a4c30
void GameHook_000a4c30();
// samp.dll RVA: 0x000a4df0
void GameHook_000a4df0();
// samp.dll RVA: 0x000a4e30
void GameHook_000a4e30();
// samp.dll RVA: 0x000a4e70
void GameHook_000a4e70();
// samp.dll RVA: 0x000a5100
void GameHook_000a5100();
// samp.dll RVA: 0x000a5150
void GameHook_000a5150();
// samp.dll RVA: 0x000a56b0
void GameHook_000a56b0();
// samp.dll RVA: 0x000a5820
void GameHook_000a5820();
// samp.dll RVA: 0x000a5850
void GameHook_000a5850();
// samp.dll RVA: 0x000a5870
void GameHook_000a5870();
// samp.dll RVA: 0x000a5910
void GameHook_000a5910();
// samp.dll RVA: 0x000a59b0
void GameHook_000a59b0();
// samp.dll RVA: 0x000a5a50
void GameHook_000a5a50();
// samp.dll RVA: 0x000a5af0
void GameHook_000a5af0();
// samp.dll RVA: 0x000a5b90
void GameHook_000a5b90();
// samp.dll RVA: 0x000a5d20
void GameHook_000a5d20();
// samp.dll RVA: 0x000a5d80
void GameInstallHook(DWORD address,DWORD function,DWORD storage,const BYTE *jumpCode,int size);
// samp.dll RVA: 0x000a5e30
void GamePrepareHooks();
// samp.dll RVA: 0x000a6020
void GameInstallHooks();
// Raw GTA callback; not an ordinary void() call interface.
// samp.dll RVA: 0x000a5bf0
void GameHook_000a5bf0();
// samp.dll RVA: 0x000a6d30
void GameInstallModelInfoHook();
// samp.dll RVA: 0x00151a60
extern BYTE g_gameCustomModelHookJump[6];
#endif

// samp.dll RVA: 0x000a4200
void GameReleaseCachedRasterBuffer();

// samp.dll RVA: 0x000a2690
void GameDisablePedRemoteCall();
// samp.dll RVA: 0x000a26b0
void GameRestorePedRemoteCall();

// samp.dll RVA: 0x000a5d40
void GameWriteProtectedDword(DWORD address,DWORD value);
// samp.dll RVA: 0x000a5df0
void GameWriteProtectedRelativeBranch(DWORD address,DWORD target,BYTE opcode);

// Raw GameHook_* declarations below are installation addresses, not ordinary
// C++ call interfaces: native register inputs, stack prologues and tail jumps.
// samp.dll RVA: 0x000a22a0
void GameRestoreHudDraw();
// samp.dll RVA: 0x000a2630
void GameDisablePedControlCalls();
// samp.dll RVA: 0x000a2650
void GameRestorePedControlCalls();
// samp.dll RVA: 0x000a3780
void GameHook_000a3780();
// samp.dll RVA: 0x000a37e0
void GameHook_000a37e0();
// samp.dll RVA: 0x000a3b40
void GameHook_000a3b40();
// samp.dll RVA: 0x000a3d50
void GameHook_000a3d50();
// samp.dll RVA: 0x000a3d70
void GameHook_000a3d70();
// samp.dll RVA: 0x000a3e40
void GameHook_000a3e40();
// samp.dll RVA: 0x000a4240
void *GameAllocateHookBuffer();
// samp.dll RVA: 0x000a4c10
void GameHook_000a4c10();
// samp.dll RVA: 0x000a4c90
void GameHook_000a4c90();
// samp.dll RVA: 0x000a4cb0
void GameHook_000a4cb0();
// samp.dll RVA: 0x000a4170
void GameHook_000a4170();
// samp.dll RVA: 0x000a48a0
void GameHook_000a48a0();
// samp.dll RVA: 0x0018f4d0
extern DWORD g_hookGate18F4D0;
// samp.dll RVA: 0x0018eb2c
extern DWORD g_hookRandomReturn;
// samp.dll RVA: 0x0018e8b8
extern DWORD g_hookRecordReturn18E8B8;
// samp.dll RVA: 0x0018f5c8
extern DWORD g_hookRecordEcx18F5C8;

// Additional raw hook entries: address-only declarations, not C++ call interfaces.
// samp.dll RVA: 0x0011ebe4
extern const char g_hookPedSayFormat[];
// samp.dll RVA: 0x0011ec04
extern const char g_hookGamepadAimFormat[];
// samp.dll RVA: 0x00151aac
extern DWORD g_hookCapture00151AAC;
// samp.dll RVA: 0x00151ab0
extern DWORD g_hookCapture00151AB0;
// samp.dll RVA: 0x00151ab4
extern WORD g_hookCapture00151AB4;
// samp.dll RVA: 0x00151ac4
extern WORD g_hookCapture00151AC4;
// samp.dll RVA: 0x0018e540
extern DWORD g_hookCapture0018E540;
// samp.dll RVA: 0x0018e544
extern DWORD g_hookCapture0018E544;
// samp.dll RVA: 0x0018e564
extern DWORD g_hookCapture0018E564;
// samp.dll RVA: 0x0018e588
extern DWORD g_hookCapture0018E588;
// samp.dll RVA: 0x0018e58c
extern DWORD g_hookCapture0018E58C;
// samp.dll RVA: 0x0018e5c4
extern DWORD g_hookCapture0018E5C4;
// samp.dll RVA: 0x0018e5d8
extern DWORD g_hookCapture0018E5D8;
// samp.dll RVA: 0x0018e800
extern CVector g_hookAimRayTarget;
// samp.dll RVA: 0x0018e814
extern DWORD g_hookCapture0018E814;
// samp.dll RVA: 0x0018e818
extern DWORD g_hookCapture0018E818;
// samp.dll RVA: 0x0018e86c
extern DWORD g_hookCapture0018E86C;
// samp.dll RVA: 0x0018e874
extern DWORD g_hookCapture0018E874;
// samp.dll RVA: 0x0018e8c8
extern DWORD g_hookCapture0018E8C8;
// samp.dll RVA: 0x0018e8d4
extern DWORD g_hookCapture0018E8D4;
// samp.dll RVA: 0x0018e924
extern DWORD g_hookCapture0018E924;
// samp.dll RVA: 0x0018e928
extern DWORD g_hookCapture0018E928;
// samp.dll RVA: 0x0018ea40
extern CVector g_hookAimRayOrigin;
// samp.dll RVA: 0x0018eaa0
extern DWORD g_hookCapture0018EAA0;
// samp.dll RVA: 0x0018eaa4
extern DWORD g_hookCapture0018EAA4;
// samp.dll RVA: 0x0018eaa8
extern DWORD g_hookCapture0018EAA8;
// samp.dll RVA: 0x0018eab0
extern DWORD g_hookCapture0018EAB0;
// samp.dll RVA: 0x0018eb1c
extern DWORD g_hookCapture0018EB1C;
// samp.dll RVA: 0x0018eb70
extern DWORD g_hookCapture0018EB70;
// samp.dll RVA: 0x0018eb78
extern DWORD g_hookCapture0018EB78;
// samp.dll RVA: 0x0018eb8c
extern DWORD g_hookCapture0018EB8C;
// samp.dll RVA: 0x0018f33c
extern DWORD g_hookCapture0018F33C;
// samp.dll RVA: 0x0018f340
extern DWORD g_hookCapture0018F340;
// samp.dll RVA: 0x0018f34c
extern DWORD g_hookCapture0018F34C;
// samp.dll RVA: 0x0018f350
extern DWORD g_hookCapture0018F350;
// samp.dll RVA: 0x0018f354
extern DWORD g_hookCapture0018F354;
// samp.dll RVA: 0x0018f360
extern DWORD g_hookCapture0018F360;
// samp.dll RVA: 0x0018f368
extern DWORD g_hookCapture0018F368;
// samp.dll RVA: 0x0018f37c
extern DWORD g_hookCapture0018F37C;
// samp.dll RVA: 0x0018f558
extern DWORD g_hookCapture0018F558;
// samp.dll RVA: 0x0018f55c
extern DWORD g_hookCapture0018F55C;
// samp.dll RVA: 0x0018f594
extern DWORD g_hookCapture0018F594;
// samp.dll RVA: 0x0018f598
extern DWORD g_hookCapture0018F598;
// samp.dll RVA: 0x0018f59c
extern DWORD g_hookCapture0018F59C;
// samp.dll RVA: 0x0018f5a0
extern DWORD g_hookCapture0018F5A0;
// samp.dll RVA: 0x0018f5e4
extern DWORD g_hookCapture0018F5E4;
// samp.dll RVA: 0x0018f658
extern DWORD g_hookCapture0018F658;
// samp.dll RVA: 0x0018f65c
extern DWORD g_hookCapture0018F65C;
// samp.dll RVA: 0x0018f660
extern DWORD g_hookCapture0018F660;
// samp.dll RVA: 0x000a2cd0
void GameHook_000a2cd0();
// samp.dll RVA: 0x000a3b60
void GameHook_000a3b60();
// samp.dll RVA: 0x000a3bc0
void GameHook_000a3bc0();
// samp.dll RVA: 0x000a3e70
void GameHook_000a3e70();
// samp.dll RVA: 0x000a40a0
void GameHook_000a40a0();
// samp.dll RVA: 0x000a4110
void GameHook_000a4110();
// samp.dll RVA: 0x000a43d0
void GameHook_000a43d0();
// samp.dll RVA: 0x000a4450
void GameHook_000a4450();
// samp.dll RVA: 0x000a4aa0
void GameHook_000a4aa0();
// samp.dll RVA: 0x000a4b20
void GameHook_000a4b20();
// samp.dll RVA: 0x000a4b80
void GameHook_000a4b80();
// samp.dll RVA: 0x000a5290
void GameHook_000a5290();
// samp.dll RVA: 0x000a52d0
void GameHook_000a52d0();
