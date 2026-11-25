#ifndef UTIL_H
#define UTIL_H

#include "game/entity.h"

#include <d3d9.h>

#include <windows.h>
// GTA SA VA: 0x00c97c28; returns the current global, including an installed proxy.

IDirect3DDevice9* GameGetCurrentD3DDevice();
// Raw RenderWare engine callback at slot24; backend observes EBX=C97B24.

DWORD GameRwInvokeEngineSlot24(DWORD first, DWORD second);

extern const float g_objectMotionRotationSentinel;

extern const float g_objectMutationStep;

extern const float g_objectRotationFactor;

extern const float g_angleUpperBound;

extern const float g_angleLowerBound;

extern const float g_anglePeriod;

extern const float g_objectScaleLowerBound;
// Native ST0 retains extended precision until the caller chooses its store.

float __cdecl GameNormalizeAngle(float angle);
// Output may alias the matrix or input; native stores each component before reading the next.

void GameTransformPoint(CVector* out, const CMatrix* matrix, const CVector* delta);

float FloatOffset(float first, float second);
// Descriptive native helper; preserves repeated input reads when output aliases it.

void __stdcall GameBuildAimUpVector(const CVector* direction, CVector* up);

extern const double g_aimVectorHalfPi;

// Native axis table is indexed without a bounds check.

extern const CVector g_rwRotationAxes[3];

extern float g_rwQueryAngle;
// Quaternion layout here belongs to the GTA/RenderWare backend; it is opaque.
// Do not substitute the maintained w,x,y,z GameQuaternion layout implicitly.

void __cdecl GameRotateRwQuaternion(void* quaternion, int axis, float angle, int combine);

void __cdecl GameQueryRwMatrixAngle(const CMatrix* matrix, int axis, float* angle);

void __cdecl GameQueryRwQuaternionAngle(const void* quaternion, CVector* axis, float* angle);

void __cdecl GameRotateRwMatrix(CMatrix* matrix, const CVector* axis, float angle);

void __cdecl GameOrthoNormalizeRwMatrix(CMatrix* out, const CMatrix* matrix);

void __cdecl GameScaleRwMatrix(CMatrix* matrix, const CVector* scale);
// Raw backend layout; unlike GameQuaternionFromMatrix, no XYZW/WXYZ reorder.

void GameRwQuaternionFromMatrix(void* quaternion, const CMatrix* matrix);

void GameNormalizeVector(CVector* vector);

CVector* GameVectorCrossProduct(CVector* out, const CVector* left, const CVector* right);
// Transposed basis only; no translation.

CVector* GameTransformVectorByTranspose(CVector* out, const CVector* vector, const CMatrix* matrix);

typedef CVector*(__cdecl* GameRwPointTransformBackend)(CVector*, const CVector*, int, const CMatrix*);
typedef void(__cdecl* GameRwAxisRotateBackend)(CMatrix*, const CVector*, float, int);

extern GameRwPointTransformBackend g_rwPointTransformBackend;

extern GameRwAxisRotateBackend g_rwAxisRotateBackend;

extern const float g_headingInverseHalfTurn;

extern const float g_headingPi;

CVector* GameTransformRwVector(CVector* out, const CVector* vector, const CMatrix* matrix);

CVector* GameTransformRwPoint(CVector* out, const CVector* point, const CMatrix* matrix);

void GameRepeatRwMatrixInvert();

void GameInvertRwMatrix(CMatrix* out, const CMatrix* matrix);

void GameRotateRwMatrixOnAxis(CMatrix* matrix, int axis, float angle);

float GameHeadingToRadians(float heading);

// Opaque GTA/RenderWare objects, not SA-MP CEntity/CCamera wrapper pointers.
typedef void*(__cdecl* GameRwObjectCallback)(void* object, void* data);

int GameRwCountClumpAtomics(void* clump);

void* GameRwForAllAtomics(void* clump, GameRwObjectCallback callback, void* data);

void* GameRwForAllFrameObjects(void* frame, GameRwObjectCallback callback, void* data);

void GameRwDestroyTexture(void* texture);

void GameRwDestroyFrame(void* frame);

void GameRwDestroyAtomic(void* atomic);

void GameRwDestroyRaster(void* raster);

void GameRwClearCamera(void* camera, const void* color, DWORD flags);

void GameRwBeginCameraUpdate(void* camera);

void GameRwEndCameraUpdate(void* camera);

void GameRwRenderClump(void* clump);

void GameRwAddClumpLight(void* clump, void* light);

void GameRwSetLightRadius(void* light, float radius);

BOOL GameRwDestroyClump(void* clump);

void* GameRwQueryAtomic48(const void* object);

void* __stdcall GameRwReadField34(const void* object);

void GameRwWorldAddCamera(void* camera);

void GameRwWorldAddClump(void* clump);

void GameRwWorldRemoveClump(void* clump);

void GameRwWorldRender();

void GameRwWorldAddLight(void* light);

void GameRwWorldRemoveLight(void* light);

void* GameRwCloneClump(void* clump);

DWORD GameRwInvokeEngineSlot20(DWORD first, DWORD second);

void* GameRwUnlockGeometry(void* geometry);

DWORD GameRwLockGeometry(void* geometry);

void GameRwSetVisibilityCamera(void* camera);

void GameRwUpdateClumpHierarchy(void* clump);

void GameRwUpdateClumpAnimations(void* clump, float timeStep, DWORD onScreen);

void* GameRwQueryAtomic7c(void* object);

void GameRwDestroyObject(void* object);

BOOL __stdcall GameRwRenderAtomic(void* object);

void __stdcall GameRwRenderObject(void* object);

// Opaque GTA backend pointers; these are not SA-MP pool/wrapper objects.

int __stdcall GamePoolPedGetIndex(void* ped);

DWORD __stdcall GamePoolVehicleGetIndex(void* vehicle);

void* GamePoolBuildingGetArray();

void* GamePoolDummyGetArray();

void* GamePoolObjectGetArray();

bool GamePoolHasObjectModel(int model);
// Fixed physical GTA table span, independent of recovered custom-model bank.

int GameCountEmptyBackendModelSlots();

extern int g_rwImageHeight;

extern void* g_rwImageScratch;

extern void** g_rwSceneCameraSlot;

void GameCreateSceneRasters();
// Raw GPR-preserving entry, not a RenderWare camera method.

void GameRenderWithSceneRasters();

// Shared physical native BSS span. Collision and rotation adapters use views
// inside it; this is not one established typed collision object.

extern BYTE g_entityCollisionScratch[0x1044];

extern const CVector g_rwPreviewAxes[3];

void* GameRwCreateLight(int type);

void GameRwSetLightColor(void* light, const float* rgba);

void* GameRwCreateRaster(int width, int height, int depth, int flags);

void* GameRwCreateCamera();

void* GameRwCreateFrame();

void GameRwTranslateFrame(void* frame, const CVector* position, int combine);

void GameRwRotateFrame(void* frame, int axis, float angle);

void GameRwSetCameraRasters(void* camera, void* color, void* depth);

void GameRwSetObjectFrame(void* object, void* frame);

void GameRwSetClipDistances(void* camera, float farClip, float nearClip);

void GameRwSetViewWindow(void* camera, const float* window);

void GameRwSetProjection(void* camera, int projection);

void* GameRwCreateTexture(void* raster);
// Raw native argument words forwarded to GTA CPad::Clear.

void GameClearLocalPad(DWORD first, DWORD second);
// Opaque GTA backend objects, not maintained CEntity/CPed wrapper instances.
// Flag arguments retain full native DWORD words while the backend consumes bools.

bool __stdcall GameWeaponFireInstantHit(void* weapon, void* shooter, const CVector* origin, const CVector* muzzle, void* victim, const CVector* target, const CVector* driveBy,
                                        DWORD crossHair, DWORD effects);

bool __stdcall GameWeaponFireSniper(void* weapon, void* shooter);

extern DWORD g_playerDefaultColors[1000];

extern DWORD g_playerColors[1000];
namespace PlayerColors
{

void Initialize();

void __stdcall Set(DWORD index, DWORD color);

DWORD __stdcall Get(DWORD index);
} // namespace PlayerColors
// The effects parser uses the same23-byte native stream as DFF/TXD parsing.
// Its single implementation,including borrowed-buffer lifecycle,is in modelfile.

BOOL GameLoadTextureDictionary(const char* name, const char* filename, BYTE fromSamp);

void* GameLoadDictionaryTexture(const char* dictionary, const char* name);

int GameTxdFind(const char* name);

void GameTxdPush();

void GameTxdPop();

void GameTxdSetCurrent(int index);

void* GameTxdReadTexture(const char* name);

void GameTxdAddRef(int index);

void GameTxdRemoveRef(int index);

void GameTxdRemove(int index);

void GameTxdRemoveSlot(int index);

DWORD GameTxdGetReferenceCount(int index);

void GameTxdRemoveSlotByName(const char* name);

void* GameRwGetGeometrySkin(void* geometry);

int GameRwRegisterClumpPlugin(DWORD pluginId);

int GameRwRegisterClumpPluginStream(DWORD pluginId, DWORD readCallback);

void GameRwEnableAutoMipmapping();

void* GameRwGetFirstAtomic(void* clump);

void* GameRwGetClumpAnimHierarchy(void* clump);

void* GameRwCreateHierarchyAnimation(void* hierarchy);

void* GameRwGetClumpSkin(void* clump);

DWORD __stdcall GameTestCameraSphere(const CVector* centre, float radius);

void __stdcall GameRwReplaceField34(void* object, void* replacement);

void* GameRwDumpAtomicMaterials(void* object, void* data);

void* __stdcall GameRwDumpObjectFrameMaterials(void* object);

void* GameRwKeepType1Object(void* object, void* data);

void* __stdcall GameRwEnumerateObjectFrameType1(void* object);

void* GameRwGetObjectFrame(const void* object);

void* GameRwGetClumpWord8Minus64(void* clump, int index);

// Shared original float storage, already defined by textdraw/dialog units.

extern const float g_textDrawHalf;

extern const float g_dialogLineScale;
// Native Newton approximation, not CRT sqrt; preserves signed bit shift and x87.

float GameApproximateSqrt(float value);

extern const char g_rwMaterialCountText[];

extern const char g_rwMaterialNameText[];

extern const char g_rwMaterialNullText[];

// Opaque GTA entity/clump/hierarchy layout, not SA-MP wrapper pointers.

BOOL GameCopyBackendBoneMatrix(void* entity, CMatrix* out, int bone);

// Opaque DWORD arguments/results; backend semantics are not established.

DWORD GameCall705B60(DWORD argument);

DWORD GameCall59C790(DWORD first, DWORD second, DWORD third);
// Native scans14000 records of56 bytes starting at the building pool's +4 slot.

void GameReplaceBuildingPoolModels(int model, int replacement);

// Returns the unspilled extended difference.

float GameSubtractFloats(float first, float second);

// dy squared plus dx squared, no intermediate float spill.

float __stdcall GameSquaredDistance2D(float x, float y, float otherX, float otherY);

// Square root of dy squared plus dx squared.

float __stdcall GameDistance2D(float x, float y, float otherX, float otherY);

// dz squared plus dy squared then dx squared.

float __stdcall GameSquaredDistance3D(float x, float y, float z, float otherX, float otherY, float otherZ);

// dx squared plus dy squared then dz squared; RET8.

float __stdcall GameSquaredDistanceVectors(const CVector* first, const CVector* second);

// x squared plus y squared then z squared.

float GameVectorLengthSquared(const CVector* vector);

// Inclusive signed unit bounds for all components; unordered values rejected.

bool GameIsVectorWithinUnitBounds(const CVector* vector);

// Sequential stores and input reloads preserve native alias behavior.

void GameVectorCrossProductLocal(CVector* out, const CVector* left, const CVector* right);

// Square root of x squared plus y squared then z squared.

float GameVectorLength(const CVector* vector);

// Separate native entry, same arithmetic as RVA000B4640; original name unknown.

float GameVectorLengthSquared4820(const CVector* vector);

// x squared plus y squared then z squared; cdecl.

float GameLengthSquared3D(float x, float y, float z);

// z products plus y products then x products.

float GameVectorDotProduct(const CVector* first, const CVector* second);

// Three sequential component stores; native input/output alias behavior.

void GameScaleVector(CVector* out, const CVector* vector, float scale);

// Native inclusive0..360 gate accepts NaN; preserves branch arithmetic.

float GameHeadingToRadiansBounded(float heading);

// Raw inline pool bytes at +4; signed capacity at +8; no null check.

DWORD GameCountNonzeroPoolBytesB7449C();

DWORD GameCountNonzeroPoolBytesC8800C();

void __stdcall GameReplaceAtomicModelInfoVtableWithClump(void* entity);

void GameSetCollisionPluginCurrentModel(DWORD value);

float GameAsin(float value);

// Position12 bytes and float heading; null backend is not checked natively.

void GameGetBackendPositionAndHeading(void* entity, CVector* position, float* heading);

#endif // UTIL_H
