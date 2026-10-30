#ifndef SAMP_OBJECT_MATH_H
#define SAMP_OBJECT_MATH_H
#include "game/entity.h"
#include <d3d9.h>
#include <windows.h>
// GTA SA VA: 0x00c97c28; returns the current global, including an installed proxy.
// 0x000c5110
IDirect3DDevice9* GameGetCurrentD3DDevice();
// Raw RenderWare engine callback at slot24; backend observes EBX=C97B24.
// 0x000c5510
DWORD GameRwInvokeEngineSlot24(DWORD first, DWORD second);
// 0x0011ee7c
extern const float g_objectMotionRotationSentinel;
// 0x001181ac
extern const float g_objectMutationStep;
// 0x001181c4
extern const float g_objectRotationFactor;
// 0x0011f578
extern const float g_angleUpperBound;
// 0x0011f574
extern const float g_angleLowerBound;
// 0x001181b4
extern const float g_anglePeriod;
// 0x001179b0
extern const float g_objectScaleLowerBound;
// Native ST0 retains extended precision until the caller chooses its store.
// 0x000b44e0
float __cdecl GameNormalizeAngle(float angle);
// Output may alias the matrix or input; native stores each component before reading the next.
// 0x000b4850
void GameTransformPoint(CVector* out, const CMatrix* matrix, const CVector* delta);
// 0x000b44b0
float FloatOffset(float first, float second);
// Descriptive native helper; preserves repeated input reads when output aliases it.
// 0x000b6440
void __stdcall GameBuildAimUpVector(const CVector* direction, CVector* up);
// 0x0011e3a0
extern const double g_aimVectorHalfPi;

// Native axis table is indexed without a bounds check.
// 0x001553d8
extern const CVector g_rwRotationAxes[3];
// 0x002aae14
extern float g_rwQueryAngle;
// Quaternion layout here belongs to the GTA/RenderWare backend; it is opaque.
// Do not substitute the maintained w,x,y,z GameQuaternion layout implicitly.
// 0x000b5320
void __cdecl GameRotateRwQuaternion(void* quaternion, int axis, float angle, int combine);
// 0x000b5370
void __cdecl GameQueryRwMatrixAngle(const CMatrix* matrix, int axis, float* angle);
// 0x000b53e0
void __cdecl GameQueryRwQuaternionAngle(const void* quaternion, CVector* axis, float* angle);
// 0x000b5410
void __cdecl GameRotateRwMatrix(CMatrix* matrix, const CVector* axis, float angle);
// 0x000b5450
void __cdecl GameOrthoNormalizeRwMatrix(CMatrix* out, const CMatrix* matrix);
// 0x000b5480
void __cdecl GameScaleRwMatrix(CMatrix* matrix, const CVector* scale);
// Raw backend layout; unlike GameQuaternionFromMatrix, no XYZW/WXYZ reorder.
// 0x000b4d20
void GameRwQuaternionFromMatrix(void* quaternion, const CMatrix* matrix);

// 0x000b4630
void GameNormalizeVector(CVector* vector);
// 0x000b4750
CVector* GameVectorCrossProduct(CVector* out, const CVector* left, const CVector* right);
// Transposed basis only; no translation.
// 0x000b4790
CVector* GameTransformVectorByTranspose(CVector* out, const CVector* vector, const CMatrix* matrix);

typedef CVector*(__cdecl* GameRwPointTransformBackend)(CVector*, const CVector*, int, const CMatrix*);
typedef void(__cdecl* GameRwAxisRotateBackend)(CMatrix*, const CVector*, float, int);
// 0x001553d4
extern GameRwPointTransformBackend g_rwPointTransformBackend;
// 0x001553fc
extern GameRwAxisRotateBackend g_rwAxisRotateBackend;
// 0x0011f58c
extern const float g_headingInverseHalfTurn;
// 0x0011f590
extern const float g_headingPi;
// 0x000b5210
CVector* GameTransformRwVector(CVector* out, const CVector* vector, const CMatrix* matrix);
// 0x000b5230
CVector* GameTransformRwPoint(CVector* out, const CVector* point, const CMatrix* matrix);
// 0x000b5260
void GameRepeatRwMatrixInvert();
// 0x000b5280
void GameInvertRwMatrix(CMatrix* out, const CMatrix* matrix);
// 0x000b52d0
void GameRotateRwMatrixOnAxis(CMatrix* matrix, int axis, float angle);
// 0x000b54b0
float GameHeadingToRadians(float heading);

// Opaque GTA/RenderWare objects, not SA-MP CEntity/CCamera wrapper pointers.
typedef void*(__cdecl* GameRwObjectCallback)(void* object, void* data);
// 0x000b1010
int GameRwCountClumpAtomics(void* clump);
// 0x000b1090
void* GameRwForAllAtomics(void* clump, GameRwObjectCallback callback, void* data);
// 0x000b10c0
void* GameRwForAllFrameObjects(void* frame, GameRwObjectCallback callback, void* data);
// 0x000b1360
void GameRwDestroyTexture(void* texture);
// 0x000b13d0
void GameRwDestroyFrame(void* frame);
// 0x000b1400
void GameRwDestroyAtomic(void* atomic);
// 0x000b1470
void GameRwDestroyRaster(void* raster);
// 0x000b14f0
void GameRwClearCamera(void* camera, const void* color, DWORD flags);
// 0x000b1520
void GameRwBeginCameraUpdate(void* camera);
// 0x000b1550
void GameRwEndCameraUpdate(void* camera);
// 0x000b15d0
void GameRwRenderClump(void* clump);
// 0x000b1740
void GameRwAddClumpLight(void* clump, void* light);
// 0x000b17a0
void GameRwSetLightRadius(void* light, float radius);
// 0x000b1880
BOOL GameRwDestroyClump(void* clump);
// 0x000b1030
void* GameRwQueryAtomic48(const void* object);
// 0x000b1130
void* __stdcall GameRwReadField34(const void* object);
// 0x000b14b0
void GameRwWorldAddCamera(void* camera);
// 0x000b1600
void GameRwWorldAddClump(void* clump);
// 0x000b1630
void GameRwWorldRemoveClump(void* clump);
// 0x000b1660
void GameRwWorldRender();
// 0x000b17c0
void GameRwWorldAddLight(void* light);
// 0x000b1800
void GameRwWorldRemoveLight(void* light);
// 0x000b1850
void* GameRwCloneClump(void* clump);
// 0x000b0ff0
DWORD GameRwInvokeEngineSlot20(DWORD first, DWORD second);
// 0x000b1100
void* GameRwUnlockGeometry(void* geometry);
// 0x000b1110
DWORD GameRwLockGeometry(void* geometry);
// 0x000b1840
void GameRwSetVisibilityCamera(void* camera);
// 0x000b1980
void GameRwUpdateClumpHierarchy(void* clump);
// 0x000b19a0
void GameRwUpdateClumpAnimations(void* clump, float timeStep, DWORD onScreen);
// 0x000b1050
void* GameRwQueryAtomic7c(void* object);
// 0x000b5df0
void GameRwDestroyObject(void* object);
// 0x000b5e30
BOOL __stdcall GameRwRenderAtomic(void* object);
// 0x000b5e70
void __stdcall GameRwRenderObject(void* object);

// Opaque GTA backend pointers; these are not SA-MP pool/wrapper objects.
// 0x000b37a0
int __stdcall GamePoolPedGetIndex(void* ped);
// 0x000b3800
DWORD __stdcall GamePoolVehicleGetIndex(void* vehicle);
// 0x000b3870
void* GamePoolBuildingGetArray();
// 0x000b3890
void* GamePoolDummyGetArray();
// 0x000b38b0
void* GamePoolObjectGetArray();
// 0x000b3930
bool GamePoolHasObjectModel(int model);
// Fixed physical GTA table span, independent of recovered custom-model bank.
// 0x000b3a00
int GameCountEmptyBackendModelSlots();

// 0x002a9d98
extern int g_rwImageDepth;
// 0x002a9d9c
extern int g_rwImageWidth;
// 0x002a9a40
extern int g_rwImageHeight;
// 0x002aae10
extern void* g_rwImageScratch;
// 0x00155400
extern void** g_rwSceneCameraSlot;
// 0x000b5690
void GameWriteRasterBmp(void* raster, const char* filename);
// 0x000b57d0
void GameCreateSceneRasters();
// Raw GPR-preserving entry, not a RenderWare camera method.
// 0x000b5860
void GameRenderWithSceneRasters();

// Shared physical native BSS span. Collision and rotation adapters use views
// inside it; this is not one established typed collision object.
// 0x002aae18
extern BYTE g_entityCollisionScratch[0x1044];
// 0x0015535c
extern const CVector g_rwPreviewAxes[3];
// 0x000b1700
void* GameRwCreateLight(int type);
// 0x000b1770
void GameRwSetLightColor(void* light, const float* rgba);
// 0x000b12d0
void* GameRwCreateRaster(int width, int height, int depth, int flags);
// 0x000b1430
void* GameRwCreateCamera();
// 0x000b1390
void* GameRwCreateFrame();
// 0x000b18f0
void GameRwTranslateFrame(void* frame, const CVector* position, int combine);
// 0x000b1920
void GameRwRotateFrame(void* frame, int axis, float angle);
// 0x000b15b0
void GameRwSetCameraRasters(void* camera, void* color, void* depth);
// 0x000b1580
void GameRwSetObjectFrame(void* object, void* frame);
// 0x000b1680
void GameRwSetClipDistances(void* camera, float farClip, float nearClip);
// 0x000b16d0
void GameRwSetViewWindow(void* camera, const float* window);
// 0x000b18c0
void GameRwSetProjection(void* camera, int projection);
// 0x000b1320
void* GameRwCreateTexture(void* raster);
// Raw native argument words forwarded to GTA CPad::Clear.
// 0x000b58e0
void GameClearLocalPad(DWORD first, DWORD second);
// Opaque GTA backend objects, not maintained CEntity/CPed wrapper instances.
// Flag arguments retain full native DWORD words while the backend consumes bools.
// 0x000b6020
bool __stdcall GameWeaponFireInstantHit(void* weapon, void* shooter, const CVector* origin, const CVector* muzzle, void* victim, const CVector* target, const CVector* driveBy,
                                        DWORD crossHair, DWORD effects);
// 0x000b6050
bool __stdcall GameWeaponFireSniper(void* weapon, void* shooter);
// 0x00151b20
extern DWORD g_playerDefaultColors[1000];
// 0x0018f6c0
extern DWORD g_playerColors[1000];
namespace PlayerColors
{
// 0x000a6fc0
void Initialize();
// 0x000a6f50
void __stdcall Set(DWORD index, DWORD color);
// 0x000a6f70
DWORD __stdcall Get(DWORD index);
} // namespace PlayerColors
// The effects parser uses the same23-byte native stream as DFF/TXD parsing.
// Its single implementation,including borrowed-buffer lifecycle,is in modelfile.
// 0x000b2790
BOOL GameLoadTextureDictionary(const char* name, const char* filename, BYTE fromSamp);
// 0x000b2860
void* GameLoadDictionaryTexture(const char* dictionary, const char* name);
// 0x000b3480
int GameTxdFind(const char* name);
// 0x000b3510
void GameTxdPush();
// 0x000b3520
void GameTxdPop();
// 0x000b3530
void GameTxdSetCurrent(int index);
// 0x000b3540
void* GameTxdReadTexture(const char* name);
// 0x000b34f0
void GameTxdAddRef(int index);
// 0x000b3500
void GameTxdRemoveRef(int index);
// 0x000b35b0
void GameTxdRemove(int index);
// 0x000b35d0
void GameTxdRemoveSlot(int index);
// 0x000b35f0
DWORD GameTxdGetReferenceCount(int index);
// 0x000b3580
void GameTxdRemoveSlotByName(const char* name);
// 0x000b1ce0
void* GameRwGetGeometrySkin(void* geometry);
// 0x000b1da0
int GameRwRegisterClumpPlugin(DWORD pluginId);
// 0x000b1e00
int GameRwRegisterClumpPluginStream(DWORD pluginId, DWORD readCallback);
// 0x000b1e60
void GameRwEnableAutoMipmapping();
// 0x000b1ca0
void* GameRwGetFirstAtomic(void* clump);
// 0x000b1d40
void* GameRwGetClumpAnimHierarchy(void* clump);
// 0x000b1d70
void* GameRwCreateHierarchyAnimation(void* hierarchy);

// 0x000b1d20
void* GameRwGetClumpSkin(void* clump);

// 0x000b3e40
DWORD __stdcall GameTestCameraSphere(const CVector* centre, float radius);

// 0x000b1170
void __stdcall GameRwReplaceField34(void* object, void* replacement);
// 0x000b11c0
void* GameRwDumpAtomicMaterials(void* object, void* data);
// 0x000b1260
void* __stdcall GameRwDumpObjectFrameMaterials(void* object);
// 0x000b1280
void* GameRwKeepType1Object(void* object, void* data);
// 0x000b12a0
void* __stdcall GameRwEnumerateObjectFrameType1(void* object);
// 0x000b1970
void* GameRwGetObjectFrame(const void* object);
// 0x000b1e90
void* GameRwGetClumpWord8Minus64(void* clump, int index);

// Shared original float storage, already defined by textdraw/dialog units.
// 0x00117884
extern const float g_textDrawHalf;
// 0x00118490
extern const float g_dialogLineScale;
// Native Newton approximation, not CRT sqrt; preserves signed bit shift and x87.
// 0x000b55a0
float GameApproximateSqrt(float value);
// 0x0011f414
extern const char g_rwMaterialCountText[];
// 0x0011f3f0
extern const char g_rwMaterialNameText[];
// 0x0011f3cc
extern const char g_rwMaterialNullText[];

// Opaque GTA entity/clump/hierarchy layout, not SA-MP wrapper pointers.
// 0x000b62f0
BOOL GameCopyBackendBoneMatrix(void* entity, CMatrix* out, int bone);

// Opaque DWORD arguments/results; backend semantics are not established.
// 0x000b14a0
DWORD GameCall705B60(DWORD argument);
// 0x000b4770
DWORD GameCall59C790(DWORD first, DWORD second, DWORD third);
// Native scans14000 records of56 bytes starting at the building pool's +4 slot.
// 0x000b4360
void GameReplaceBuildingPoolModels(int model, int replacement);

// Returns the unspilled extended difference.
// 0x000b44a0
float GameSubtractFloats(float first, float second);

// dy squared plus dx squared, no intermediate float spill.
// 0x000b4510
float __stdcall GameSquaredDistance2D(float x, float y, float otherX, float otherY);

// Square root of dy squared plus dx squared.
// 0x000b4540
float __stdcall GameDistance2D(float x, float y, float otherX, float otherY);

// dz squared plus dy squared then dx squared.
// 0x000b45b0
float __stdcall GameSquaredDistance3D(float x, float y, float z, float otherX, float otherY, float otherZ);

// dx squared plus dy squared then dz squared; RET8.
// 0x000b45f0
float __stdcall GameSquaredDistanceVectors(const CVector* first, const CVector* second);

// x squared plus y squared then z squared.
// April 2005 D3DX inline reproduces this native body; wrapper name is descriptive.
// 0x000b4640
float GameVectorLengthSquared(const CVector* vector);

// Inclusive signed unit bounds for all components; unordered values rejected.
// 0x000b4670
bool GameIsVectorWithinUnitBounds(const CVector* vector);

// Sequential stores and input reloads preserve native alias behavior.
// 0x000b47b0
void GameVectorCrossProductLocal(CVector* out, const CVector* left, const CVector* right);

// Square root of x squared plus y squared then z squared.
// April 2005 D3DX inline reproduces this native body; wrapper name is descriptive.
// 0x000b47f0
float GameVectorLength(const CVector* vector);

// Separate native entry, same arithmetic as RVA000B4640; original name unknown.
// April 2005 D3DX inline reproduces this native body; wrapper name is descriptive.
// 0x000b4820
float GameVectorLengthSquared4820(const CVector* vector);

// x squared plus y squared then z squared; cdecl.
// 0x000b4b20
float GameLengthSquared3D(float x, float y, float z);

// z products plus y products then x products.
// April 2005 D3DX inline reproduces this native body; wrapper name is descriptive.
// 0x000b4b40
float GameVectorDotProduct(const CVector* first, const CVector* second);

// Three sequential component stores; native input/output alias behavior.
// 0x000b4cf0
void GameScaleVector(CVector* out, const CVector* vector, float scale);

// Native inclusive0..360 gate accepts NaN; preserves branch arithmetic.
// 0x000b5520
float GameHeadingToRadiansBounded(float heading);

// Raw inline pool bytes at +4; signed capacity at +8; no null check.
// 0x000b38d0
DWORD GameCountNonzeroPoolBytesB7449C();
// 0x000b3900
DWORD GameCountNonzeroPoolBytesC8800C();
// 0x000b4130
void __stdcall GameReplaceBackendVtable85BBF0(void* entity);
// 0x000b42c0
void GameWriteGlobal9689E0(DWORD value);
// 0x000b3620
float GameAsin(float value);

// Position12 bytes and float heading; null backend is not checked natively.
// 0x000b6260
void GameGetBackendPositionAndHeading(void* entity, CVector* position, float* heading);
#endif
