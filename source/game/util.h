#ifndef UTIL_H
#define UTIL_H

#include "game/entity.h"

#include <d3d9.h>

#include <windows.h>
// GTA SA VA: 0x00c97c28; returns the current global, including an installed proxy.
// R5 address to verify
IDirect3DDevice9* GameGetCurrentD3DDevice();
// Raw RenderWare engine callback at slot24; backend observes EBX=C97B24.
// 0x000c3e40
DWORD GameRwInvokeEngineSlot24(DWORD first, DWORD second);
// R5 address to verify
extern const float g_objectMotionRotationSentinel;
// 0x000e611c
extern const float g_objectMutationStep;
// 0x000e6134
extern const float g_objectRotationFactor;
// 0x000ed468
extern const float g_angleUpperBound;
// 0x000ed464
extern const float g_angleLowerBound;
// 0x000e6124
extern const float g_anglePeriod;
// 0x000e5940
extern const float g_objectScaleLowerBound;
// Native ST0 retains extended precision until the caller chooses its store.
// 0x000b48e0
float __cdecl GameNormalizeAngle(float angle);
// Output may alias the matrix or input; native stores each component before reading the next.
// 0x000b4d10
void GameTransformPoint(CVector* out, const CMatrix* matrix, const CVector* delta);
// 0x000b48b0
float FloatOffset(float first, float second);
// Descriptive native helper; preserves repeated input reads when output aliases it.
// 0x000b6900
void __stdcall GameBuildAimUpVector(const CVector* direction, CVector* up);
// R5 address to verify
extern const double g_aimVectorHalfPi;

// Native axis table is indexed without a bounds check.
// 0x00117400
extern const CVector g_rwRotationAxes[3];
// R5 address to verify
extern float g_rwQueryAngle;
// Quaternion layout here belongs to the GTA/RenderWare backend; it is opaque.
// Do not substitute the maintained w,x,y,z GameQuaternion layout implicitly.
// 0x000b57e0
void __cdecl GameRotateRwQuaternion(void* quaternion, int axis, float angle, int combine);
// 0x000b5830
void __cdecl GameQueryRwMatrixAngle(const CMatrix* matrix, int axis, float* angle);
// 0x000b58a0
void __cdecl GameQueryRwQuaternionAngle(const void* quaternion, CVector* axis, float* angle);
// 0x000b58d0
void __cdecl GameRotateRwMatrix(CMatrix* matrix, const CVector* axis, float angle);
// 0x000b5910
void __cdecl GameOrthoNormalizeRwMatrix(CMatrix* out, const CMatrix* matrix);
// 0x000b5940
void __cdecl GameScaleRwMatrix(CMatrix* matrix, const CVector* scale);
// Raw backend layout; unlike GameQuaternionFromMatrix, no XYZW/WXYZ reorder.
// 0x000b51e0
void GameRwQuaternionFromMatrix(void* quaternion, const CMatrix* matrix);

// 0x000b4a30
void GameNormalizeVector(CVector* vector);
// 0x000b4bc0
CVector* GameVectorCrossProduct(CVector* out, const CVector* left, const CVector* right);
// Transposed basis only; no translation.
// 0x000b4c00
CVector* GameTransformVectorByTranspose(CVector* out, const CVector* vector, const CMatrix* matrix);

typedef CVector*(__cdecl* GameRwPointTransformBackend)(CVector*, const CVector*, int, const CMatrix*);
typedef void(__cdecl* GameRwAxisRotateBackend)(CMatrix*, const CVector*, float, int);
// R5 address to verify
extern GameRwPointTransformBackend g_rwPointTransformBackend;
// 0x00117424
extern GameRwAxisRotateBackend g_rwAxisRotateBackend;
// 0x000ed47c
extern const float g_headingInverseHalfTurn;
// 0x000ed480
extern const float g_headingPi;
// 0x000b56d0
CVector* GameTransformRwVector(CVector* out, const CVector* vector, const CMatrix* matrix);
// 0x000b56f0
CVector* GameTransformRwPoint(CVector* out, const CVector* point, const CMatrix* matrix);
// 0x000b5720
void GameRepeatRwMatrixInvert();
// 0x000b5740
void GameInvertRwMatrix(CMatrix* out, const CMatrix* matrix);
// 0x000b5790
void GameRotateRwMatrixOnAxis(CMatrix* matrix, int axis, float angle);
// 0x000b5970
float GameHeadingToRadians(float heading);

// Opaque GTA/RenderWare objects, not SA-MP CEntity/CCamera wrapper pointers.
typedef void*(__cdecl* GameRwObjectCallback)(void* object, void* data);
// 0x000b1410
int GameRwCountClumpAtomics(void* clump);
// 0x000b1490
void* GameRwForAllAtomics(void* clump, GameRwObjectCallback callback, void* data);
// 0x000b14c0
void* GameRwForAllFrameObjects(void* frame, GameRwObjectCallback callback, void* data);
// 0x000b1760
void GameRwDestroyTexture(void* texture);
// 0x000b17d0
void GameRwDestroyFrame(void* frame);
// 0x000b1800
void GameRwDestroyAtomic(void* atomic);
// 0x000b1870
void GameRwDestroyRaster(void* raster);
// 0x000b18f0
void GameRwClearCamera(void* camera, const void* color, DWORD flags);
// 0x000b1920
void GameRwBeginCameraUpdate(void* camera);
// 0x000b1950
void GameRwEndCameraUpdate(void* camera);
// 0x000b19d0
void GameRwRenderClump(void* clump);
// 0x000b1b40
void GameRwAddClumpLight(void* clump, void* light);
// 0x000b1ba0
void GameRwSetLightRadius(void* light, float radius);
// 0x000b1c80
BOOL GameRwDestroyClump(void* clump);
// 0x000b1430
void* GameRwQueryAtomic48(const void* object);
// 0x000b1530
void* __stdcall GameRwReadField34(const void* object);
// 0x000b18b0
void GameRwWorldAddCamera(void* camera);
// 0x000b1a00
void GameRwWorldAddClump(void* clump);
// 0x000b1a30
void GameRwWorldRemoveClump(void* clump);
// 0x000b1a60
void GameRwWorldRender();
// 0x000b1bc0
void GameRwWorldAddLight(void* light);
// 0x000b1c00
void GameRwWorldRemoveLight(void* light);
// 0x000b1c50
void* GameRwCloneClump(void* clump);
// 0x000b13f0
DWORD GameRwInvokeEngineSlot20(DWORD first, DWORD second);
// 0x000b1500
void* GameRwUnlockGeometry(void* geometry);
// 0x000b1510
DWORD GameRwLockGeometry(void* geometry);
// 0x000b1c40
void GameRwSetVisibilityCamera(void* camera);
// 0x000b1d80
void GameRwUpdateClumpHierarchy(void* clump);
// 0x000b1da0
void GameRwUpdateClumpAnimations(void* clump, float timeStep, DWORD onScreen);
// R5 address to verify
void* GameRwQueryAtomic7c(void* object);
// 0x000b62b0
void GameRwDestroyObject(void* object);
// 0x000b62f0
BOOL __stdcall GameRwRenderAtomic(void* object);
// 0x000b6330
void __stdcall GameRwRenderObject(void* object);

// Opaque GTA backend pointers; these are not SA-MP pool/wrapper objects.
// 0x000b3ba0
int __stdcall GamePoolPedGetIndex(void* ped);
// 0x000b3c00
DWORD __stdcall GamePoolVehicleGetIndex(void* vehicle);
// 0x000b3c70
void* GamePoolBuildingGetArray();
// 0x000b3c90
void* GamePoolDummyGetArray();
// 0x000b3cb0
void* GamePoolObjectGetArray();
// 0x000b3d30
bool GamePoolHasObjectModel(int model);
// Fixed physical GTA table span, independent of recovered custom-model bank.
// 0x000b3e00
int GameCountEmptyBackendModelSlots();

// 0x0026bba8
extern int g_rwImageHeight;
// R5 address to verify
extern void* g_rwImageScratch;
// 0x00117428
extern void** g_rwSceneCameraSlot;
// Compatibility name for GameDebugWriteRasterImage.
void GameWriteRasterBmp(void* raster, const char* filename);
// 0x000b5c90
void GameCreateSceneRasters();
// Raw GPR-preserving entry, not a RenderWare camera method.
// 0x000b5d20
void GameRenderWithSceneRasters();

// Shared physical native BSS span. Collision and rotation adapters use views
// inside it; this is not one established typed collision object.
// R5 address to verify
extern BYTE g_entityCollisionScratch[0x1044];
// R5 address to verify
extern const CVector g_rwPreviewAxes[3];
// 0x000b1b00
void* GameRwCreateLight(int type);
// 0x000b1b70
void GameRwSetLightColor(void* light, const float* rgba);
// 0x000b16d0
void* GameRwCreateRaster(int width, int height, int depth, int flags);
// 0x000b1830
void* GameRwCreateCamera();
// 0x000b1790
void* GameRwCreateFrame();
// 0x000b1cf0
void GameRwTranslateFrame(void* frame, const CVector* position, int combine);
// 0x000b1d20
void GameRwRotateFrame(void* frame, int axis, float angle);
// 0x000b19b0
void GameRwSetCameraRasters(void* camera, void* color, void* depth);
// 0x000b1980
void GameRwSetObjectFrame(void* object, void* frame);
// 0x000b1a80
void GameRwSetClipDistances(void* camera, float farClip, float nearClip);
// 0x000b1ad0
void GameRwSetViewWindow(void* camera, const float* window);
// 0x000b1cc0
void GameRwSetProjection(void* camera, int projection);
// 0x000b1720
void* GameRwCreateTexture(void* raster);
// Raw native argument words forwarded to GTA CPad::Clear.
// 0x000b5da0
void GameClearLocalPad(DWORD first, DWORD second);
// Opaque GTA backend objects, not maintained CEntity/CPed wrapper instances.
// Flag arguments retain full native DWORD words while the backend consumes bools.
// 0x000b64e0
bool __stdcall GameWeaponFireInstantHit(void* weapon, void* shooter, const CVector* origin, const CVector* muzzle, void* victim, const CVector* target, const CVector* driveBy,
                                        DWORD crossHair, DWORD effects);
// 0x000b6510
bool __stdcall GameWeaponFireSniper(void* weapon, void* shooter);
// 0x00113b48
extern DWORD g_playerDefaultColors[1000];
// 0x00151828
extern DWORD g_playerColors[1000];
namespace PlayerColors
{
// 0x000a7280
void Initialize();
// 0x000a7210
void __stdcall Set(DWORD index, DWORD color);
// 0x000a7230
DWORD __stdcall Get(DWORD index);
} // namespace PlayerColors
// The effects parser uses the same23-byte native stream as DFF/TXD parsing.
// Its single implementation,including borrowed-buffer lifecycle,is in modelfile.
// 0x000b2b90
BOOL GameLoadTextureDictionary(const char* name, const char* filename, BYTE fromSamp);
// 0x000b2c60
void* GameLoadDictionaryTexture(const char* dictionary, const char* name);
// 0x000b3880
int GameTxdFind(const char* name);
// 0x000b3910
void GameTxdPush();
// R5 address to verify
void GameTxdPop();
// 0x000b3930
void GameTxdSetCurrent(int index);
// 0x000b3940
void* GameTxdReadTexture(const char* name);
// 0x000b38f0
void GameTxdAddRef(int index);
// 0x000b3900
void GameTxdRemoveRef(int index);
// 0x000b39b0
void GameTxdRemove(int index);
// 0x000b39d0
void GameTxdRemoveSlot(int index);
// 0x000b39f0
DWORD GameTxdGetReferenceCount(int index);
// 0x000b3980
void GameTxdRemoveSlotByName(const char* name);
// 0x000b20e0
void* GameRwGetGeometrySkin(void* geometry);
// 0x000b21a0
int GameRwRegisterClumpPlugin(DWORD pluginId);
// 0x000b2200
int GameRwRegisterClumpPluginStream(DWORD pluginId, DWORD readCallback);
// 0x000b2260
void GameRwEnableAutoMipmapping();
// 0x000b20a0
void* GameRwGetFirstAtomic(void* clump);
// 0x000b2140
void* GameRwGetClumpAnimHierarchy(void* clump);
// 0x000b2170
void* GameRwCreateHierarchyAnimation(void* hierarchy);

// R5 address to verify
void* GameRwGetClumpSkin(void* clump);

// 0x000b4240
DWORD __stdcall GameTestCameraSphere(const CVector* centre, float radius);

// R5 address to verify
void __stdcall GameRwReplaceField34(void* object, void* replacement);
// R5 address to verify
void* GameRwDumpAtomicMaterials(void* object, void* data);
// R5 address to verify
void* __stdcall GameRwDumpObjectFrameMaterials(void* object);
// R5 address to verify
void* GameRwKeepType1Object(void* object, void* data);
// R5 address to verify
void* __stdcall GameRwEnumerateObjectFrameType1(void* object);
// R5 address to verify
void* GameRwGetObjectFrame(const void* object);
// R5 address to verify
void* GameRwGetClumpWord8Minus64(void* clump, int index);

// Shared original float storage, already defined by textdraw/dialog units.
// 0x000e5814
extern const float g_textDrawHalf;
// 0x000e6400
extern const float g_dialogLineScale;
// Native Newton approximation, not CRT sqrt; preserves signed bit shift and x87.
// R5 address to verify
float GameApproximateSqrt(float value);
// 0x000ed304
extern const char g_rwMaterialCountText[];
// 0x000ed2e0
extern const char g_rwMaterialNameText[];
// 0x000ed2bc
extern const char g_rwMaterialNullText[];

// Opaque GTA entity/clump/hierarchy layout, not SA-MP wrapper pointers.
// 0x000b67b0
BOOL GameCopyBackendBoneMatrix(void* entity, CMatrix* out, int bone);

// Opaque DWORD arguments/results; backend semantics are not established.
// 0x000b18a0
DWORD GameCall705B60(DWORD argument);
// 0x000b4be0
DWORD GameCall59C790(DWORD first, DWORD second, DWORD third);
// Native scans14000 records of56 bytes starting at the building pool's +4 slot.
// 0x000b4760
void GameReplaceBuildingPoolModels(int model, int replacement);

// Returns the unspilled extended difference.
// R5 address to verify
float GameSubtractFloats(float first, float second);

// dy squared plus dx squared, no intermediate float spill.
// R5 address to verify
float __stdcall GameSquaredDistance2D(float x, float y, float otherX, float otherY);

// Square root of dy squared plus dx squared.
// R5 address to verify
float __stdcall GameDistance2D(float x, float y, float otherX, float otherY);

// dz squared plus dy squared then dx squared.
// R5 address to verify
float __stdcall GameSquaredDistance3D(float x, float y, float z, float otherX, float otherY, float otherZ);

// dx squared plus dy squared then dz squared; RET8.
// R5 address to verify
float __stdcall GameSquaredDistanceVectors(const CVector* first, const CVector* second);

// x squared plus y squared then z squared.
// R5 address to verify
float GameVectorLengthSquared(const CVector* vector);

// Inclusive signed unit bounds for all components; unordered values rejected.
// R5 address to verify
bool GameIsVectorWithinUnitBounds(const CVector* vector);

// Sequential stores and input reloads preserve native alias behavior.
// R5 address to verify
void GameVectorCrossProductLocal(CVector* out, const CVector* left, const CVector* right);

// Square root of x squared plus y squared then z squared.
// R5 address to verify
float GameVectorLength(const CVector* vector);

// Separate native entry, same arithmetic as RVA000B4640; original name unknown.
// R5 address to verify
float GameVectorLengthSquared4820(const CVector* vector);

// x squared plus y squared then z squared; cdecl.
// R5 address to verify
float GameLengthSquared3D(float x, float y, float z);

// z products plus y products then x products.
// R5 address to verify
float GameVectorDotProduct(const CVector* first, const CVector* second);

// Three sequential component stores; native input/output alias behavior.
// R5 address to verify
void GameScaleVector(CVector* out, const CVector* vector, float scale);

// Native inclusive0..360 gate accepts NaN; preserves branch arithmetic.
// R5 address to verify
float GameHeadingToRadiansBounded(float heading);

// Raw inline pool bytes at +4; signed capacity at +8; no null check.
// R5 address to verify
DWORD GameCountNonzeroPoolBytesB7449C();
// R5 address to verify
DWORD GameCountNonzeroPoolBytesC8800C();
// R5 address to verify
void __stdcall GameReplaceAtomicModelInfoVtableWithClump(void* entity);
// R5 address to verify
void GameSetCollisionPluginCurrentModel(DWORD value);
// R5 address to verify
float GameAsin(float value);

// Position12 bytes and float heading; null backend is not checked natively.
// R5 address to verify
void GameGetBackendPositionAndHeading(void* entity, CVector* position, float* heading);

#endif // UTIL_H
