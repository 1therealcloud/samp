#ifndef HOOKS_H
#define HOOKS_H

#include <windows.h>

class CPlayerPool;
class CMatrix;
struct CVector;
struct PedBulletData;
// R5 address to verify
extern CPlayerPool* g_hookDamagePlayerPool;
// R5 address to verify
extern WORD g_hookDamagePlayerId;
// R5 address to verify
extern DWORD g_hookVehicleDamageAttacker;
// R5 address to verify
extern DWORD g_hookVehicleDamageReceiver;
// R5 address to verify
extern DWORD g_hookVehicleDamageWord;
// R5 address to verify
extern DWORD g_hookVehicleDamageBlocked;
// R5 address to verify
extern DWORD g_hookVehicleDamageReturn;
// R5 address to verify
extern void* g_hookBulletHitEntity;
// R5 address to verify
extern PedBulletData g_hookBulletHitData;
// R5 address to verify
extern CMatrix g_hookBulletInverseMatrix;
// R5 address to verify
extern CMatrix g_hookBulletOrthoMatrix;
// Shared native DWORD slots at18F66C..18F688; the three-argument hook reuses slot0.
// R5 address to verify
extern DWORD g_hookShotArguments[8];
// R5 address to verify
extern CVector g_hookAdjustedShotPoint;
// R5 address to verify
extern DWORD g_hookShotLineResult;
// 0x000a5240
BOOL GameShouldBlockVehicleDamage(void* vehicle, void* attacker, DWORD unused);
// Second pointer is deliberately unused; native RET16.
// 0x000a5410
void __stdcall GameRecordBulletHit(const CVector* origin, const CVector* target, const CVector* collision, void** entity);
// Raw cdecl GTA adapter retains the full backend EAX result.
// R5 address to verify
DWORD GameProcessShotLineOfSight(const CVector* origin, CVector* target, void* collision, void** entity, DWORD buildings, DWORD vehicles, DWORD peds, DWORD objects, DWORD dummies,
                                 DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough);
// Raw GTA callback ABIs; address-only declarations, never ordinary void() calls.
// R5 address to verify
void GameHookFilterVehicleDamage();
// R5 address to verify
void GameHookProcessShotLineOfSight();
// R5 address to verify
void GameHookProcessShotLineOfSightSimple();
// 0x000a58f0
void GameHookAdjustShotRayPoint();
// 0x0026eb3c
extern DWORD g_gameLoopAddress;
// R5 address to verify
extern DWORD g_gameHookFarClipReturn;
// 0x00113a28
extern BYTE g_gameHookJump00151a10[10];
// 0x00113a34
extern BYTE g_gameHookJump00151a1c[7];
// 0x00113994
extern BYTE g_gameHookJump0015197c[7];
// 0x0011399c
extern BYTE g_gameHookJump00151984[7];
// 0x001139ac
extern BYTE g_gameHookJump00151994[7];
// 0x001139a4
extern BYTE g_gameHookJump0015198c[7];
// 0x001139c8
extern BYTE g_gameHookJump001519b0[7];
// 0x001139d0
extern BYTE g_gameHookJump001519b8[6];
// 0x00113a00
extern BYTE g_gameHookJump001519e8[6];
// 0x00113a08
extern BYTE g_gameHookJump001519f0[6];
// 0x001139ec
extern BYTE g_gameHookJump001519d4[9];
// 0x00113a4c
extern BYTE g_gameHookJump00151a34[6];
// 0x00113a54
extern BYTE g_gameHookJump00151a3c[7];
// 0x00113a5c
extern BYTE g_gameHookJump00151a44[9];
// 0x00113a68
extern BYTE g_gameHookJump00151a50[6];
// R5 address to verify
extern BYTE g_gameHookJump00151a68[6];
// Raw callback declarations for installing addresses. Recovered bodies and exact
// entry ABIs are tracked in the callback review; remaining bodies stay open.
// Do not call these through the provisional void() types from ordinary C++.
// R5 address to verify
void GameHookYieldBeforeWorldUpdate();
// R5 address to verify
void GameHookDrawScaledRadar();
// R5 address to verify
void GameHookDrawScaledCrosshair();
// R5 address to verify
void GameHookPostProcessRemotePeds();
// R5 address to verify
void GameHookUpdateCameraOwner();
// 0x000a2660
void GameHookProcessMultiplayerFrame();
// R5 address to verify
void GameHookProcessPedControl();
// R5 address to verify
void GameHookPreservePedTransform();
// R5 address to verify
void GameHookApplyRemoteWeaponSkills();
// R5 address to verify
void GameHookProcessRemoteGunTask();
// R5 address to verify
void GameHookProcessRemoteVehicleControl();
// R5 address to verify
void GameHookHandleVehicleHorn();
// R5 address to verify
void GameHookNotifyVehicleEnter();
// R5 address to verify
void GameHookNotifyVehicleExit();
// R5 address to verify
void GameHookTranslateRadarColor();
// R5 address to verify
void GameHookDisableCheatProcessing();
// R5 address to verify
void GameHookDeferGameShutdown();
// R5 address to verify
void GameHookNotifyPedDamage();
// R5 address to verify
void GameHookDisableSniperCameraShake();
// R5 address to verify
void GameHookSetLocalCameraFlags();
// R5 address to verify
void GameHookPreserveTrainSpeedOnDerailment();
// 0x000a3dc0
void GameHookGuardAnimationDecompression();
// R5 address to verify
void GameHookOverrideFogDuringRender();
// R5 address to verify
void GameHookDrawGangZones();
// R5 address to verify
void GameHookMarkRemotePedState();
// R5 address to verify
void GameHookNotifyPickupCollected();
// R5 address to verify
void GameHookGuardMissingRenderMaterial();
// R5 address to verify
void GameHookFilterRenderStateType();
// 0x000a4030
void GameHookResolveMultiplayerMenuText();
// R5 address to verify
void GameHookGuardPhysicalProcessCollision();
// Raw installed ABI: no arguments, returns the preview backing pointer in EAX.
// The void() declaration is an installation-address type, not a calling prototype.
// R5 address to verify
void GameHookGetPreviewPedBuffer();
// R5 address to verify
void GameHookAddPedAnimation();
// R5 address to verify
void GameHookBlendPedAnimation();
// R5 address to verify
void GameHookPreserveCachedRenderBuffer();
// R5 address to verify
void GameHookAllocateCachedRenderBuffer();
// R5 address to verify
void GameHookCaptureRenderEsi();
// R5 address to verify
void GameHookApplyCustomObjectMaterials();
// R5 address to verify
void GameHookUpdateSurfingAttachment();
// R5 address to verify
void GameHookGetContextPlayerSpeed();
// R5 address to verify
void GameHookSuppressWorldMarker();
// R5 address to verify
void GameHookPatchRenderCallback();
// R5 address to verify
void GameHookRenderCustomObjects();
// R5 address to verify
void GameHookAdjustFontScale();
// R5 address to verify
void GameHookHandleVehicleTowLink();
// R5 address to verify
void GameHookUpdateAttachedObjectRotation();
// R5 address to verify
void GameHookRenderMultiplayerObjects();
// R5 address to verify
void GameHookRenderLateMultiplayerObjects();
// 0x000a5110
void GameHookFilterObjectRenderList();
// R5 address to verify
void GameHookResolvePedPlayerContext();
// R5 address to verify
void GameHookGuardNullVehicleArgument();
// 0x000a5950
void GameHookTestMultiplayerCollisionShapes();
// R5 address to verify
void GameHookFilterCollisionTest();
// R5 address to verify
void GameHookFilterCameraIgnoredObject();
// R5 address to verify
void GameHookFilterAutomobileCollision();
// R5 address to verify
void GameHookFilterBikeCollision();
// R5 address to verify
void GameHookFilterBoatCollision();
// R5 address to verify
void GameHookFilterPhysicalEntityCollision();
// R5 address to verify
void GameHookOverrideVehicleSiren();
// R5 address to verify
void GameHookOverrideVehicleAudioState();
// R5 address to verify
void GameHookOverrideVerticalLineCollisionFlags();
// 0x000a6040
void GameInstallHook(DWORD address, DWORD function, DWORD storage, const BYTE* jumpCode, int size);
// 0x000a60f0
void GamePrepareHooks();
// 0x000a62e0
void GameInstallHooks();
// Raw GTA callback; not an ordinary void() call interface.
// R5 address to verify
void GameHookValidateCustomModelIndex();
// 0x000a6ff0
void GameInstallModelInfoHook();
// R5 address to verify
extern BYTE g_gameCustomModelHookJump[6];

// 0x000a44a0
void GameReleaseCachedRasterBuffer();

// 0x000a2920
void GameDisablePedRemoteCall();
// R5 address to verify
void GameRestorePedRemoteCall();

// R5 address to verify
void GameWriteProtectedDword(DWORD address, DWORD value);
// R5 address to verify
void GameWriteProtectedRelativeBranch(DWORD address, DWORD target, BYTE opcode);

// Raw GameHook_* declarations below are installation addresses, not ordinary
// C++ call interfaces: native register inputs, stack prologues and tail jumps.
// R5 address to verify
void GameRestoreHudDraw();
// R5 address to verify
void GameDisablePedControlCalls();
// R5 address to verify
void GameRestorePedControlCalls();
// R5 address to verify
void GameHookFilterVehicleCreation();
// R5 address to verify
void GameHookOverrideFarClipDistance();
// R5 address to verify
void GameHookGuardGameWindowState();
// R5 address to verify
void GameHookGuardNullPedTask();
// R5 address to verify
void GameHookGuardVehicleColorAccess();
// R5 address to verify
void GameHookCaptureRandomResult();
// R5 address to verify
void* GameAllocateHookBuffer();
// R5 address to verify
void GameHookApplyFrameLimiter();
// R5 address to verify
void GameHookGuardMenuState();
// R5 address to verify
void GameHookSetGunMoveAnimation();
// R5 address to verify
void GameHookCaptureVehicleSetClumpCaller();
// R5 address to verify
void GameHookCaptureObjectCall();
// R5 address to verify
extern DWORD g_hookGate18F4D0;
// R5 address to verify
extern DWORD g_hookRandomReturn;
// R5 address to verify
extern DWORD g_hookVehicleSetClumpCaller;
// R5 address to verify
extern DWORD g_hookCapturedObject;

// Additional raw hook entries: address-only declarations, not C++ call interfaces.
// 0x000ecaec
extern const char g_hookPedSayFormat[];
// 0x000ecb0c
extern const char g_hookGamepadAimFormat[];
// R5 address to verify
extern DWORD g_hookPedAnimationRequestContinue;
// R5 address to verify
extern DWORD g_hookResolveAnimationContinue;
// R5 address to verify
extern WORD g_hookResolvedAnimationIndex;
// R5 address to verify
extern WORD g_hookVehicleFlagId;
// R5 address to verify
extern DWORD g_hookCameraTargetThis;
// R5 address to verify
extern DWORD g_hookSpeechForceAudible;
// R5 address to verify
extern DWORD g_hookCapture0018E564;
// R5 address to verify
extern DWORD g_hookSavedGunTaskParameter;
// R5 address to verify
extern DWORD g_hookCapture0018E58C;
// R5 address to verify
extern DWORD g_hookCapture0018E5C4;
// R5 address to verify
extern DWORD g_hookCapture0018E5D8;
// R5 address to verify
extern CVector g_hookAimRayTarget;
// R5 address to verify
extern DWORD g_hookCapture0018E814;
// R5 address to verify
extern DWORD g_hookCameraTargetSourceOut;
// R5 address to verify
extern DWORD g_hookCapture0018E86C;
// R5 address to verify
extern DWORD g_hookCameraTargetOriginY;
// R5 address to verify
extern DWORD g_hookSpeechContext;
// R5 address to verify
extern DWORD g_hookCapture0018E8D4;
// R5 address to verify
extern DWORD g_hookCameraTargetDestinationOut;
// R5 address to verify
extern DWORD g_hookSpeechFrontEnd;
// R5 address to verify
extern CVector g_hookAimRayOrigin;
// R5 address to verify
extern DWORD g_hookCameraTargetOriginX;
// R5 address to verify
extern DWORD g_hookSpeechProbability;
// R5 address to verify
extern DWORD g_hookDoorAnimationCaller;
// R5 address to verify
extern DWORD g_hookCapture0018EAB0;
// R5 address to verify
extern DWORD g_hookCapture0018EB1C;
// R5 address to verify
extern DWORD g_hookCameraTargetOriginZ;
// R5 address to verify
extern DWORD g_hookCapture0018EB78;
// R5 address to verify
extern DWORD g_hookCapture0018EB8C;
// R5 address to verify
extern DWORD g_hookSpeechOverrideSilence;
// R5 address to verify
extern DWORD g_hookSpeechStartDelay;
// R5 address to verify
extern DWORD g_hookCapture0018F34C;
// R5 address to verify
extern DWORD g_hookCapture0018F350;
// R5 address to verify
extern DWORD g_hookCapture0018F354;
// R5 address to verify
extern DWORD g_hookCameraTargetRange;
// R5 address to verify
extern DWORD g_hookCapture0018F368;
// R5 address to verify
extern DWORD g_hookGunTaskResult;
// R5 address to verify
extern DWORD g_hookCapture0018F558;
// R5 address to verify
extern DWORD g_hookResolvedAnimationKey;
// R5 address to verify
extern DWORD g_hookDoorAnimationPed;
// R5 address to verify
extern DWORD g_hookVehicleFlagGameVehicle;
// R5 address to verify
extern DWORD g_hookVehicleFlagPool;
// R5 address to verify
extern DWORD g_hookVehicleFlagWrapper;
// R5 address to verify
extern DWORD g_hookCapture0018F5E4;
// R5 address to verify
extern DWORD g_hookGamepadInputThis;
// R5 address to verify
extern DWORD g_hookGamepadAimVector;
// R5 address to verify
extern DWORD g_hookGamepadAimThis;
// R5 address to verify
void GameHookOverrideGunTaskParameter();
// R5 address to verify
void GameHookAllocatePedTask();
// R5 address to verify
void GameHookLogPedSpeech();
// R5 address to verify
void GameHookTemporarilyBypassRenderInit();
// R5 address to verify
void GameHookCapturePedAnimationRequest();
// R5 address to verify
void GameHookResolveAnimationByKey();
// R5 address to verify
void GameHookSkipVehicleDoorWhenDancing();
// R5 address to verify
void GameHookUpdateVehicleStateFlags();
// R5 address to verify
void GameHookCaptureThirdPersonCameraTarget();
// R5 address to verify
void GameHookCaptureWeaponAimRay();
// R5 address to verify
void GameHookCaptureCameraAimRay();
// R5 address to verify
void GameHookSkipGamepadInputPath();
// R5 address to verify
void GameHookLogGamepadAimUpdate();

// Raw installation-only entries; register/stack ABI is documented in analysis.
// R5 address to verify
extern CMatrix g_hookAimOffsetMatrix;
// R5 address to verify
extern CMatrix g_hookMountedMatrix;
// R5 address to verify
extern DWORD g_hookVisibilityCameraPosition[3];
// R5 address to verify
extern BYTE g_hookVisibilityCheckOriginalCode[22];
// R5 address to verify
extern WORD g_hookVisibilityBranchOriginalCode;
// R5 address to verify
extern DWORD g_hookCapture0018E548;
// R5 address to verify
extern DWORD g_hookCapture0018E54C;
// R5 address to verify
extern DWORD g_hookSavedRendererCameraX;
// R5 address to verify
extern DWORD g_hookSavedRendererCameraY;
// R5 address to verify
extern DWORD g_hookSavedRendererCameraZ;
// R5 address to verify
extern DWORD g_hookVehicleAimCamera;
// R5 address to verify
extern DWORD g_hookCapture0018E92C;
// R5 address to verify
extern DWORD g_hookStreamingFileBuffer;
// R5 address to verify
extern DWORD g_hookVehicleAimInternalAim;
// R5 address to verify
extern DWORD g_hookCapture0018F338;
// R5 address to verify
extern DWORD g_hookStreamingThirdStackWord;
// R5 address to verify
extern DWORD g_hookVisibilityDistanceOut;
// R5 address to verify
extern DWORD g_hookCapture0018F358;
// R5 address to verify
extern DWORD g_hookVehicleAimCaller;
// R5 address to verify
extern DWORD g_hookCapture0018F364;
// R5 address to verify
extern DWORD g_hookVehicleAimVehicle;
// R5 address to verify
extern DWORD g_hookRemoteSkillTask;
// R5 address to verify
extern DWORD g_hookStreamingModelId;
// R5 address to verify
extern DWORD g_hookVisibilityOverrideEntity;
// R5 address to verify
extern DWORD g_hookVisibilityEntity;
// R5 address to verify
extern DWORD g_hookVisibilityResult;
// R5 address to verify
void GameHookApplyRemoteSkillContextA();
// R5 address to verify
void GameHookApplyRemoteSkillContextB();
// R5 address to verify
void GameHookAdjustVehicleAimMatrix();
// R5 address to verify
void GameHookCaptureStreamingConversion();
// R5 address to verify
void GameHookPatchCustomObjectRenderPath();

// Raw ABI entries are declared in hooks.h for address installation only.
// Recovered bodies are naked because callers supply registers or an in-flight
// GTA stack frame, and some paths tail-jump into the original game function.
// R5 address to verify
extern DWORD g_hookRadarIndex;
// R5 address to verify
extern DWORD g_hookCheatFunction;
// R5 address to verify
extern DWORD g_hookCameraPed;
// R5 address to verify
extern DWORD g_hookRenderEsi;
// R5 address to verify
extern DWORD g_hookRenderEbx;
// R5 address to verify
extern DWORD g_hookFreedBuffer;
// R5 address to verify
extern DWORD g_hookKeepBuffer;
// 0x001516c8
extern DWORD g_hookCachedBuffer;
// R5 address to verify
extern DWORD g_hookAllocateFunction;
// R5 address to verify
extern DWORD g_hookSavedEsi;
// R5 address to verify
extern DWORD g_hookRenderReturn;
// R5 address to verify
extern BYTE g_hookRenderRestore[3];
// R5 address to verify
extern DWORD g_hookSavedFontA;
// R5 address to verify
extern DWORD g_hookSavedFontB;

// R5 address to verify
extern DWORD g_hookHudScaleX;
// R5 address to verify
extern DWORD g_hookHudScaleY;
// R5 address to verify
extern float g_hookHudWidth;
// R5 address to verify
extern float g_hookHudHeight;
// R5 address to verify
extern BYTE g_hookHudAspectFix;
// 0x000ea700
extern const float g_hookHudAspectThreshold;
// R5 address to verify
extern const float g_hookHudScaleNumerator;
// R5 address to verify
extern DWORD g_hookFogEnabled;
// 0x0026ebe0
extern DWORD g_hookDisableFog;
// R5 address to verify
extern DWORD g_hookCollisionPed;
// R5 address to verify
extern DWORD g_hookSavedMatrix[16];
// R5 address to verify
extern DWORD g_hookSavedSpeed[3];
// R5 address to verify
extern DWORD g_hookHornArgument;
// R5 address to verify
extern DWORD g_hookHornThis;
// R5 address to verify
extern DWORD g_hookHornVehicle;
// R5 address to verify
extern BYTE g_hookHornStatus;
// R5 address to verify
extern DWORD g_hookAnimationReturn;
// R5 address to verify
extern DWORD g_hookAnimationClump;
// R5 address to verify
extern DWORD g_hookAnimationGroup;
// R5 address to verify
extern DWORD g_hookAnimationId;
// R5 address to verify
extern DWORD g_hookAnimationBlend;
// R5 address to verify
extern DWORD g_hookAnimationPed;
// R5 address to verify
extern DWORD g_hookAnimationLocalClump;
// R5 address to verify
extern DWORD g_hookAnimationType;
// R5 address to verify
extern DWORD g_hookQueuedAnimationGroup;
// R5 address to verify
extern DWORD g_hookQueuedAnimationId;
// R5 address to verify
extern DWORD g_hookQueuedAnimationBlend;
// R5 address to verify
extern DWORD g_hookAnimationContinueA;
// R5 address to verify
extern DWORD g_hookAnimationContinueB;
// R5 address to verify
extern DWORD g_hookRenderObject;
// R5 address to verify
extern BYTE g_hookObjectRestore[5];
// R5 address to verify
extern DWORD g_hookMarker;
// R5 address to verify
extern DWORD g_hookMarkerArgument;
// R5 address to verify
extern DWORD g_hookSuppressAllMarkers;
// R5 address to verify
void GamePrepareHudDraw();
// 0x000c3d30
void GameSetupD3DFog(DWORD enabled);

// R5 address to verify
extern BYTE g_hookCameraHudPrepared;
// R5 address to verify
extern DWORD g_hookAudioSavedState;
// R5 address to verify
extern DWORD g_hookAudioArgument0;
// R5 address to verify
extern DWORD g_hookAudioArgument1;

// 0x0011397c
extern WORD* g_hookCameraModeWord;
// R5 address to verify
extern BYTE g_hookSavedPlayer;
// R5 address to verify
extern BYTE g_hookPlayerIndex;
// R5 address to verify
extern DWORD g_hookPedControlThis;
// R5 address to verify
extern DWORD g_hookWeaponResult;
// R5 address to verify
extern DWORD g_hookRestoreSkills;
// R5 address to verify
extern DWORD g_hookLocalPedControl;
// R5 address to verify
extern DWORD g_hookRestoreKeys;
// R5 address to verify
extern BYTE g_hookSavedCameraMode;
// R5 address to verify
extern WORD g_hookSavedCameraModeWord;
// R5 address to verify
extern DWORD g_hookVehicleProcess;
// R5 address to verify
extern DWORD g_hookControlVehicle;
// R5 address to verify
extern DWORD g_hookVehicleVtable;
// R5 address to verify
extern DWORD g_hookVehicleKeys;
// R5 address to verify
extern BYTE g_hookVehicleStatus;
// R5 address to verify
extern DWORD g_hookVehicleLean;
// R5 address to verify
extern DWORD g_hookVehicleLeanTarget;
// R5 address to verify
extern int g_gameVehicleModel;
// R5 address to verify
extern DWORD g_gameLinkedVehicle;
// R5 address to verify
extern float g_gameVehicleDistanceSquared;
// R5 address to verify
extern DWORD g_gameLocalPedMatrix;
// R5 address to verify
extern DWORD g_gameVehicleMatrix;
// 0x000ed4d8
extern const float g_gameStreamingDistanceSquared;
// 0x000b4360
BYTE __stdcall GameFindPlayerPedIndex(void* ped);
// R5 address to verify
BOOL __stdcall GameIsTrainVehicle(void* vehicle);
// 0x000b4860
BOOL __stdcall GameHasOccupiedLinkedVehicle(void* vehicle);
// 0x000b6950
BOOL __stdcall GameIsVehicleInStreamingArea(void* vehicle);

class CObjectPool;
// 0x0014ffac
extern CObjectPool* g_debugObjectPool;
class CVehicle;
class CVehiclePool;
class CObject;
struct CVector;
// R5 address to verify
extern DWORD g_hookPickupBackend;
// R5 address to verify
extern DWORD g_hookMenuTextResult;
// R5 address to verify
extern CVector g_hookPassengerCameraPosition;
// R5 address to verify
extern DWORD g_hookTrailerBackend;
// R5 address to verify
extern CVehiclePool* g_hookTrailerPool;
// R5 address to verify
extern WORD g_hookTrailerId;
// R5 address to verify
extern CVehicle* g_hookTrailerVehicle;
// R5 address to verify
extern DWORD g_hookObjectSavedStack;
// R5 address to verify
extern DWORD g_hookObjectBackend;
// R5 address to verify
extern DWORD g_hookObjectResult;
// R5 address to verify
extern CObject* g_hookObjectRotationWrapper;
// R5 address to verify
extern CObjectPool* g_hookObjectDrawPool;
// R5 address to verify
extern DWORD g_hookObjectDrawBackend;
// R5 address to verify
extern CObject* g_hookObjectDrawWrapper;
class CPlayerPool;
class CRemotePlayer;
struct GameEntityCollisionShape;
// R5 address to verify
extern DWORD g_hookMaterialReturn;
// R5 address to verify
extern DWORD g_hookMaterialBackend;
// R5 address to verify
extern CPlayerPool* g_hookMaterialPlayerPool;
// R5 address to verify
extern DWORD g_hookSurfingIndex;
// R5 address to verify
extern CRemotePlayer* g_hookSurfingRemote;
// R5 address to verify
extern GameEntityCollisionShape g_hookCollisionFirst;
// R5 address to verify
extern GameEntityCollisionShape g_hookCollisionSecond;
// R5 address to verify
extern BYTE g_hookCollisionResult;
// R5 address to verify
extern const float g_hookCollisionBoundHigh;
// R5 address to verify
extern const float g_hookCollisionBoundLow;

// R5 address to verify
extern DWORD g_hookContactThis;
// R5 address to verify
extern DWORD g_hookContactOther;
// R5 address to verify
extern DWORD g_hookContactReturn;
// R5 address to verify
extern DWORD g_hookSirenVehicle;
// R5 address to verify
extern BYTE g_hookSirenResult;
// R5 address to verify
extern DWORD g_hookSirenOnce;
// R5 address to verify
extern WORD g_hookSirenVehicleId;
// R5 address to verify
extern DWORD g_hookPedQueryThis;
// R5 address to verify
extern DWORD g_hookPedQueryPed;

// R5 address to verify
extern DWORD g_clientPreviousRenderState;

// R5 address to verify
extern DWORD g_clientRenderCaller;

// R5 address to verify
extern DWORD g_clientRenderState;

// R5 address to verify
extern DWORD g_clientRenderValue;

// Installs the recovered callback into the current RenderWare engine table.
// R5 address to verify
void ClientInstallRenderStateHook();

// Naked two-word GTA render-state callback; tail-jumps through original target.
// R5 address to verify
void ClientRenderStateHook();

class CLocalPlayer;
// R5 address to verify
extern DWORD g_hookPedControlField540;
// R5 address to verify
extern CPlayerPool* g_hookFaultPlayerPool;
// R5 address to verify
extern WORD g_hookFaultPlayerId;
// R5 address to verify
extern CRemotePlayer* g_hookFaultRemotePlayer;
// R5 address to verify
extern const BYTE g_hookPedRestoreA[6];
// R5 address to verify
extern const BYTE g_hookPedRestoreB[6];
// R5 address to verify
extern const BYTE g_hookPedRemoteRestore[5];
// 0x000ecad4
extern const char g_hookPedExceptionLabel[10];
// 0x000ecab4
extern const char g_hookPedRemovedMessage[32];
// 0x000a27c0
void GameRunPedControlBackend();
// inline
void GameProcessPedControlBody();
// Native policy helpers; their protocol send methods remain separate dependencies.
// R5 address to verify
BOOL GameNotifyEnterVehicle();
// R5 address to verify
BOOL __stdcall GameNotifyPedDamage(void* response, void* ped);
// R5 address to verify
extern DWORD g_hookIgnoreNextExit;
// R5 address to verify
extern DWORD g_hookExitTask;
// R5 address to verify
extern CVehiclePool* g_hookEventVehiclePool;
// R5 address to verify
extern WORD g_hookEventVehicleId;
// R5 address to verify
extern CVehicle* g_hookExitVehicle;
// R5 address to verify
extern CLocalPlayer* g_hookEventLocalPlayer;
// R5 address to verify
extern CMatrix g_hookExitMatrix;
// R5 address to verify
extern DWORD g_hookExitTaskArgument;
// 0x000ea634
extern const float g_hookTrainExitOffset;
// R5 address to verify
extern DWORD g_hookQuitDeadline;
// R5 address to verify
extern DWORD g_hookDamageResponse;
// R5 address to verify
extern DWORD g_hookDamagePed;
// R5 address to verify
WORD GameResolveDamagePlayer(void* entity);
// R5 address to verify
WORD GameResolveDamageActor(void* entity);
// R5 address to verify
extern CVehicle* g_hookEnterVehicle;

// R5 address to verify
void GameUpdateWeather();

#endif // HOOKS_H
