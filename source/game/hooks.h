#ifndef HOOKS_H
#define HOOKS_H

#include <windows.h>

class CPlayerPool;
class CMatrix;
struct CVector;
struct PedBulletData;

extern CPlayerPool* g_hookDamagePlayerPool;

extern WORD g_hookDamagePlayerId;

extern DWORD g_hookVehicleDamageAttacker;

extern DWORD g_hookVehicleDamageReceiver;

extern DWORD g_hookVehicleDamageWord;

extern DWORD g_hookVehicleDamageBlocked;

extern DWORD g_hookVehicleDamageReturn;

extern void* g_hookBulletHitEntity;

extern PedBulletData g_hookBulletHitData;

extern CMatrix g_hookBulletInverseMatrix;

extern CMatrix g_hookBulletOrthoMatrix;
// Shared native DWORD slots at18F66C..18F688; the three-argument hook reuses slot0.

extern DWORD g_hookShotArguments[8];

extern CVector g_hookAdjustedShotPoint;

extern DWORD g_hookShotLineResult;

BOOL GameShouldBlockVehicleDamage(void* vehicle, void* attacker, DWORD unused);
// Second pointer is deliberately unused; native RET16.

void __stdcall GameRecordBulletHit(const CVector* origin, const CVector* target, const CVector* collision, void** entity);
// Raw cdecl GTA adapter retains the full backend EAX result.

DWORD GameProcessShotLineOfSight(const CVector* origin, CVector* target, void* collision, void** entity, DWORD buildings, DWORD vehicles, DWORD peds, DWORD objects, DWORD dummies,
                                 DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough);
// Raw GTA callback ABIs; address-only declarations, never ordinary void() calls.

void GameHookFilterVehicleDamage();

void GameHookProcessShotLineOfSight();

void GameHookProcessShotLineOfSightSimple();

void GameHookAdjustShotRayPoint();

extern DWORD g_gameLoopAddress;

extern DWORD g_gameHookFarClipReturn;

extern BYTE g_gameHookJump00151a10[10];

extern BYTE g_gameHookJump00151a1c[7];

extern BYTE g_gameHookJump0015197c[7];

extern BYTE g_gameHookJump00151984[7];

extern BYTE g_gameHookJump00151994[7];

extern BYTE g_gameHookJump0015198c[7];

extern BYTE g_gameHookJump001519b0[7];

extern BYTE g_gameHookJump001519b8[6];

extern BYTE g_gameHookJump001519e8[6];

extern BYTE g_gameHookJump001519f0[6];

extern BYTE g_gameHookJump001519d4[9];

extern BYTE g_gameHookJump00151a34[6];

extern BYTE g_gameHookJump00151a3c[7];

extern BYTE g_gameHookJump00151a44[9];

extern BYTE g_gameHookJump00151a50[6];

extern BYTE g_gameHookJump00151a68[6];
// Raw callback declarations for installing addresses. Recovered bodies and exact
// entry ABIs are tracked in the callback review; remaining bodies stay open.
// Do not call these through the provisional void() types from ordinary C++.

void GameHookYieldBeforeWorldUpdate();

void GameHookDrawScaledRadar();

void GameHookDrawScaledCrosshair();

void GameHookPostProcessRemotePeds();

void GameHookUpdateCameraOwner();

void GameHookProcessMultiplayerFrame();

void GameHookProcessPedControl();

void GameHookPreservePedTransform();

void GameHookApplyRemoteWeaponSkills();

void GameHookProcessRemoteGunTask();

void GameHookProcessRemoteVehicleControl();

void GameHookHandleVehicleHorn();

void GameHookNotifyVehicleEnter();

void GameHookNotifyVehicleExit();

void GameHookTranslateRadarColor();

void GameHookDisableCheatProcessing();

void GameHookDeferGameShutdown();

void GameHookNotifyPedDamage();

void GameHookDisableSniperCameraShake();

void GameHookSetLocalCameraFlags();

void GameHookPreserveTrainSpeedOnDerailment();

void GameHookGuardAnimationDecompression();

void GameHookOverrideFogDuringRender();

void GameHookDrawGangZones();

void GameHookMarkRemotePedState();

void GameHookNotifyPickupCollected();

void GameHookGuardMissingRenderMaterial();

void GameHookFilterRenderStateType();

void GameHookResolveMultiplayerMenuText();

void GameHookGuardPhysicalProcessCollision();
// Raw installed ABI: no arguments, returns the preview backing pointer in EAX.
// The void() declaration is an installation-address type, not a calling prototype.

void GameHookGetPreviewPedBuffer();

void GameHookAddPedAnimation();

void GameHookBlendPedAnimation();

void GameHookPreserveCachedRenderBuffer();

void GameHookAllocateCachedRenderBuffer();

void GameHookCaptureRenderEsi();

void GameHookApplyCustomObjectMaterials();

void GameHookUpdateSurfingAttachment();

void GameHookGetContextPlayerSpeed();

void GameHookSuppressWorldMarker();

void GameHookPatchRenderCallback();

void GameHookRenderCustomObjects();

void GameHookAdjustFontScale();

void GameHookHandleVehicleTowLink();

void GameHookUpdateAttachedObjectRotation();

void GameHookRenderMultiplayerObjects();

void GameHookRenderLateMultiplayerObjects();

void GameHookFilterObjectRenderList();

void GameHookResolvePedPlayerContext();

void GameHookGuardNullVehicleArgument();

void GameHookTestMultiplayerCollisionShapes();

void GameHookFilterCollisionTest();

void GameHookFilterCameraIgnoredObject();

void GameHookFilterAutomobileCollision();

void GameHookFilterBikeCollision();

void GameHookFilterBoatCollision();

void GameHookFilterPhysicalEntityCollision();

void GameHookOverrideVehicleSiren();

void GameHookOverrideVehicleAudioState();

void GameHookOverrideVerticalLineCollisionFlags();

void GameInstallHook(DWORD address, DWORD function, DWORD storage, const BYTE* jumpCode, int size);

void GamePrepareHooks();

void GameInstallHooks();

void GameInstallFastloadHooks();
// Raw GTA callback; not an ordinary void() call interface.

void GameHookValidateCustomModelIndex();

void GameInstallModelInfoHook();

extern BYTE g_gameCustomModelHookJump[6];

void GameReleaseCachedRasterBuffer();

void GameDisablePedRemoteCall();

void GameRestorePedRemoteCall();

void GameWriteProtectedDword(DWORD address, DWORD value);

void GameWriteProtectedRelativeBranch(DWORD address, DWORD target, BYTE opcode);

// Raw GameHook_* declarations below are installation addresses, not ordinary
// C++ call interfaces: native register inputs, stack prologues and tail jumps.

void GameRestoreHudDraw();

void GameDisablePedControlCalls();

void GameRestorePedControlCalls();

void GameHookFilterVehicleCreation();

void GameHookOverrideFarClipDistance();

void GameHookGuardGameWindowState();

void GameHookGuardNullPedTask();

void GameHookGuardVehicleColorAccess();

void GameHookCaptureRandomResult();

void* GameAllocateHookBuffer();

void GameHookApplyFrameLimiter();

void GameHookGuardMenuState();

void GameHookSetGunMoveAnimation();

void GameHookCaptureVehicleSetClumpCaller();

void GameHookCaptureObjectCall();

extern DWORD g_hookGate18F4D0;

extern DWORD g_hookRandomReturn;

extern DWORD g_hookVehicleSetClumpCaller;

extern DWORD g_hookCapturedObject;

// Additional raw hook entries: address-only declarations, not C++ call interfaces.

extern const char g_hookPedSayFormat[];

extern const char g_hookGamepadAimFormat[];

extern DWORD g_hookPedAnimationRequestContinue;

extern DWORD g_hookResolveAnimationContinue;

extern WORD g_hookResolvedAnimationIndex;

extern WORD g_hookVehicleFlagId;

extern DWORD g_hookCameraTargetThis;

extern DWORD g_hookSpeechForceAudible;

extern DWORD g_hookCapture0018E564;

extern DWORD g_hookSavedGunTaskParameter;

extern DWORD g_hookCapture0018E58C;

extern DWORD g_hookCapture0018E5C4;

extern DWORD g_hookCapture0018E5D8;

extern CVector g_hookAimRayTarget;

extern DWORD g_hookCapture0018E814;

extern DWORD g_hookCameraTargetSourceOut;

extern DWORD g_hookCapture0018E86C;

extern DWORD g_hookCameraTargetOriginY;

extern DWORD g_hookSpeechContext;

extern DWORD g_hookCapture0018E8D4;

extern DWORD g_hookCameraTargetDestinationOut;

extern DWORD g_hookSpeechFrontEnd;

extern CVector g_hookAimRayOrigin;

extern DWORD g_hookCameraTargetOriginX;

extern DWORD g_hookSpeechProbability;

extern DWORD g_hookDoorAnimationCaller;

extern DWORD g_hookCapture0018EAB0;

extern DWORD g_hookCapture0018EB1C;

extern DWORD g_hookCameraTargetOriginZ;

extern DWORD g_hookCapture0018EB78;

extern DWORD g_hookCapture0018EB8C;

extern DWORD g_hookSpeechOverrideSilence;

extern DWORD g_hookSpeechStartDelay;

extern DWORD g_hookCapture0018F34C;

extern DWORD g_hookCapture0018F350;

extern DWORD g_hookCapture0018F354;

extern DWORD g_hookCameraTargetRange;

extern DWORD g_hookCapture0018F368;

extern DWORD g_hookGunTaskResult;

extern DWORD g_hookCapture0018F558;

extern DWORD g_hookResolvedAnimationKey;

extern DWORD g_hookDoorAnimationPed;

extern DWORD g_hookVehicleFlagGameVehicle;

extern DWORD g_hookVehicleFlagPool;

extern DWORD g_hookVehicleFlagWrapper;

extern DWORD g_hookCapture0018F5E4;

extern DWORD g_hookGamepadInputThis;

extern DWORD g_hookGamepadAimVector;

extern DWORD g_hookGamepadAimThis;

void GameHookOverrideGunTaskParameter();

void GameHookAllocatePedTask();

void GameHookLogPedSpeech();

void GameHookTemporarilyBypassRenderInit();

void GameHookCapturePedAnimationRequest();

void GameHookResolveAnimationByKey();

void GameHookSkipVehicleDoorWhenDancing();

void GameHookUpdateVehicleStateFlags();

void GameHookCaptureThirdPersonCameraTarget();

void GameHookCaptureWeaponAimRay();

void GameHookCaptureCameraAimRay();

void GameHookSkipGamepadInputPath();

void GameHookLogGamepadAimUpdate();

// Raw installation-only entries; register/stack ABI is documented in analysis.

extern CMatrix g_hookAimOffsetMatrix;

extern CMatrix g_hookMountedMatrix;

extern DWORD g_hookVisibilityCameraPosition[3];

extern BYTE g_hookVisibilityCheckOriginalCode[22];

extern WORD g_hookVisibilityBranchOriginalCode;

extern DWORD g_hookCapture0018E548;

extern DWORD g_hookCapture0018E54C;

extern DWORD g_hookSavedRendererCameraX;

extern DWORD g_hookSavedRendererCameraY;

extern DWORD g_hookSavedRendererCameraZ;

extern DWORD g_hookVehicleAimCamera;

extern DWORD g_hookCapture0018E92C;

extern DWORD g_hookStreamingFileBuffer;

extern DWORD g_hookVehicleAimInternalAim;

extern DWORD g_hookCapture0018F338;

extern DWORD g_hookStreamingThirdStackWord;

extern DWORD g_hookVisibilityDistanceOut;

extern DWORD g_hookCapture0018F358;

extern DWORD g_hookVehicleAimCaller;

extern DWORD g_hookCapture0018F364;

extern DWORD g_hookVehicleAimVehicle;

extern DWORD g_hookRemoteSkillTask;

extern DWORD g_hookStreamingModelId;

extern DWORD g_hookVisibilityOverrideEntity;

extern DWORD g_hookVisibilityEntity;

extern DWORD g_hookVisibilityResult;

void GameHookApplyRemoteSkillContextA();

void GameHookApplyRemoteSkillContextB();

void GameHookAdjustVehicleAimMatrix();

void GameHookCaptureStreamingConversion();

void GameHookPatchCustomObjectRenderPath();

// Raw ABI entries are declared in hooks.h for address installation only.
// Recovered bodies are naked because callers supply registers or an in-flight
// GTA stack frame, and some paths tail-jump into the original game function.

extern DWORD g_hookRadarIndex;

extern DWORD g_hookCheatFunction;

extern DWORD g_hookCameraPed;

extern DWORD g_hookRenderEsi;

extern DWORD g_hookRenderEbx;

extern DWORD g_hookFreedBuffer;

extern DWORD g_hookKeepBuffer;

extern DWORD g_hookCachedBuffer;

extern DWORD g_hookAllocateFunction;

extern DWORD g_hookSavedEsi;

extern DWORD g_hookRenderReturn;

extern BYTE g_hookRenderRestore[3];

extern DWORD g_hookSavedFontA;

extern DWORD g_hookSavedFontB;

extern DWORD g_hookHudScaleX;

extern DWORD g_hookHudScaleY;

extern float g_hookHudWidth;

extern float g_hookHudHeight;

extern BYTE g_hookHudAspectFix;

extern const float g_hookHudAspectThreshold;

extern const float g_hookHudScaleNumerator;

extern DWORD g_hookFogEnabled;

extern DWORD g_hookDisableFog;

extern DWORD g_hookCollisionPed;

extern DWORD g_hookSavedMatrix[16];

extern DWORD g_hookSavedSpeed[3];

extern DWORD g_hookHornArgument;

extern DWORD g_hookHornThis;

extern DWORD g_hookHornVehicle;

extern BYTE g_hookHornStatus;

extern DWORD g_hookAnimationReturn;

extern DWORD g_hookAnimationClump;

extern DWORD g_hookAnimationGroup;

extern DWORD g_hookAnimationId;

extern DWORD g_hookAnimationBlend;

extern DWORD g_hookAnimationPed;

extern DWORD g_hookAnimationLocalClump;

extern DWORD g_hookAnimationType;

extern DWORD g_hookQueuedAnimationGroup;

extern DWORD g_hookQueuedAnimationId;

extern DWORD g_hookQueuedAnimationBlend;

extern DWORD g_hookAnimationContinueA;

extern DWORD g_hookAnimationContinueB;

extern DWORD g_hookRenderObject;

extern BYTE g_hookObjectRestore[5];

extern DWORD g_hookMarker;

extern DWORD g_hookMarkerArgument;

extern DWORD g_hookSuppressAllMarkers;

void GamePrepareHudDraw();

void GameSetupD3DFog(DWORD enabled);

extern BYTE g_hookCameraHudPrepared;

extern DWORD g_hookAudioSavedState;

extern DWORD g_hookAudioArgument0;

extern DWORD g_hookAudioArgument1;

extern WORD* g_hookCameraModeWord;

extern BYTE g_hookSavedPlayer;

extern BYTE g_hookPlayerIndex;

extern DWORD g_hookPedControlThis;

extern DWORD g_hookWeaponResult;

extern DWORD g_hookRestoreSkills;

extern DWORD g_hookLocalPedControl;

extern DWORD g_hookRestoreKeys;

extern BYTE g_hookSavedCameraMode;

extern WORD g_hookSavedCameraModeWord;

extern DWORD g_hookVehicleProcess;

extern DWORD g_hookControlVehicle;

extern DWORD g_hookVehicleVtable;

extern DWORD g_hookVehicleKeys;

extern BYTE g_hookVehicleStatus;

extern DWORD g_hookVehicleLean;

extern DWORD g_hookVehicleLeanTarget;

extern int g_gameVehicleModel;

extern DWORD g_gameLinkedVehicle;

extern float g_gameVehicleDistanceSquared;

extern DWORD g_gameLocalPedMatrix;

extern DWORD g_gameVehicleMatrix;

extern const float g_gameStreamingDistanceSquared;

BYTE __stdcall GameFindPlayerPedIndex(void* ped);

BOOL __stdcall GameIsTrainVehicle(void* vehicle);

BOOL __stdcall GameHasOccupiedLinkedVehicle(void* vehicle);

BOOL __stdcall GameIsVehicleInStreamingArea(void* vehicle);

class CObjectPool;

extern CObjectPool* g_debugObjectPool;
class CVehicle;
class CVehiclePool;
class CObject;
struct CVector;

extern DWORD g_hookPickupBackend;

extern DWORD g_hookMenuTextResult;

extern CVector g_hookPassengerCameraPosition;

extern DWORD g_hookTrailerBackend;

extern CVehiclePool* g_hookTrailerPool;

extern WORD g_hookTrailerId;

extern CVehicle* g_hookTrailerVehicle;

extern DWORD g_hookObjectSavedStack;

extern DWORD g_hookObjectBackend;

extern DWORD g_hookObjectResult;

extern CObject* g_hookObjectRotationWrapper;

extern CObjectPool* g_hookObjectDrawPool;

extern DWORD g_hookObjectDrawBackend;

extern CObject* g_hookObjectDrawWrapper;
class CPlayerPool;
class CRemotePlayer;
struct GameEntityCollisionShape;

extern DWORD g_hookMaterialReturn;

extern DWORD g_hookMaterialBackend;

extern CPlayerPool* g_hookMaterialPlayerPool;

extern DWORD g_hookSurfingIndex;

extern CRemotePlayer* g_hookSurfingRemote;

extern GameEntityCollisionShape g_hookCollisionFirst;

extern GameEntityCollisionShape g_hookCollisionSecond;

extern BYTE g_hookCollisionResult;

extern const float g_hookCollisionBoundHigh;

extern const float g_hookCollisionBoundLow;

extern DWORD g_hookContactThis;

extern DWORD g_hookContactOther;

extern DWORD g_hookContactReturn;

extern DWORD g_hookSirenVehicle;

extern BYTE g_hookSirenResult;

extern DWORD g_hookSirenOnce;

extern WORD g_hookSirenVehicleId;

extern DWORD g_hookPedQueryThis;

extern DWORD g_hookPedQueryPed;

extern DWORD g_clientPreviousRenderState;

extern DWORD g_clientRenderCaller;

extern DWORD g_clientRenderState;

extern DWORD g_clientRenderValue;

// Installs the recovered callback into the current RenderWare engine table.

void ClientInstallRenderStateHook();

// Naked two-word GTA render-state callback; tail-jumps through original target.

void ClientRenderStateHook();

class CLocalPlayer;

extern DWORD g_hookPedControlField540;

extern CPlayerPool* g_hookFaultPlayerPool;

extern WORD g_hookFaultPlayerId;

extern CRemotePlayer* g_hookFaultRemotePlayer;

extern const BYTE g_hookPedRestoreA[6];

extern const BYTE g_hookPedRestoreB[6];

extern const BYTE g_hookPedRemoteRestore[5];

extern const char g_hookPedExceptionLabel[10];

extern const char g_hookPedRemovedMessage[32];

void GameRunPedControlBackend();

void GameProcessPedControlBody();

BOOL GameNotifyEnterVehicle();

BOOL __stdcall GameNotifyPedDamage(void* response, void* ped);

extern DWORD g_hookIgnoreNextExit;

extern DWORD g_hookExitTask;

extern CVehiclePool* g_hookEventVehiclePool;

extern WORD g_hookEventVehicleId;

extern CVehicle* g_hookExitVehicle;

extern CLocalPlayer* g_hookEventLocalPlayer;

extern CMatrix g_hookExitMatrix;

extern DWORD g_hookExitTaskArgument;

extern const float g_hookTrainExitOffset;

extern DWORD g_hookQuitDeadline;

extern DWORD g_hookDamageResponse;

extern DWORD g_hookDamagePed;

WORD GameResolveDamagePlayer(void* entity);

WORD GameResolveDamageActor(void* entity);

extern CVehicle* g_hookEnterVehicle;

void GameUpdateWeather();

#endif // HOOKS_H
