#ifndef SAMP_OBJECT_MATH_H
#define SAMP_OBJECT_MATH_H
#include "game/entity.h"
#include <d3d9.h>
// GTA SA VA: 0x00c97c28; returns the current global, including an installed proxy.
// samp.dll RVA: 0x000c5110
IDirect3DDevice9 *GameGetCurrentD3DDevice();
// GTA SA global C17048; no device or viewport query.
// samp.dll RVA: 0x00065ee0
DWORD GameGetScreenHeight();
// Raw RenderWare engine callback at slot24; backend observes EBX=C97B24.
// samp.dll RVA: 0x000c5510
DWORD GameRwInvokeEngineSlot24(DWORD first,DWORD second);
// samp.dll RVA: 0x0011ee7c
extern const float g_objectMotionRotationSentinel;
// samp.dll RVA: 0x001181ac
extern const float g_objectMutationStep;
// samp.dll RVA: 0x001181c4
extern const float g_objectRotationFactor;
// samp.dll RVA: 0x0011f578
extern const float g_angleUpperBound;
// samp.dll RVA: 0x0011f574
extern const float g_angleLowerBound;
// samp.dll RVA: 0x001181b4
extern const float g_anglePeriod;
// samp.dll RVA: 0x001179b0
extern const float g_objectScaleLowerBound;
// Native ST0 retains extended precision until the caller chooses its store.
// samp.dll RVA: 0x000b44e0
float __cdecl GameNormalizeAngle(float angle);
// Output may alias the matrix or input; native stores each component before reading the next.
// samp.dll RVA: 0x000b4850
void GameTransformPoint(CVector *out,const CMatrix *matrix,const CVector *delta);
// samp.dll RVA: 0x000b44b0
float FloatOffset(float first,float second);
// Descriptive native helper; preserves repeated input reads when output aliases it.
// samp.dll RVA: 0x000b6440
void __stdcall GameBuildAimUpVector(const CVector *direction,CVector *up);
// samp.dll RVA: 0x0011e3a0
extern const double g_aimVectorHalfPi;
#endif

#ifndef SAMP_RW_TRANSFORM_H
#define SAMP_RW_TRANSFORM_H
// Native axis table is indexed without a bounds check.
// samp.dll RVA: 0x001553d8
extern const CVector g_rwRotationAxes[3];
// samp.dll RVA: 0x002aae14
extern float g_rwQueryAngle;
// Quaternion layout here belongs to the GTA/RenderWare backend; it is opaque.
// Do not substitute the maintained w,x,y,z GameQuaternion layout implicitly.
// samp.dll RVA: 0x000b5320
void __cdecl GameRotateRwQuaternion(void *quaternion,int axis,float angle,int combine);
// samp.dll RVA: 0x000b5370
void __cdecl GameQueryRwMatrixAngle(const CMatrix *matrix,int axis,float *angle);
// samp.dll RVA: 0x000b53e0
void __cdecl GameQueryRwQuaternionAngle(const void *quaternion,CVector *axis,float *angle);
// samp.dll RVA: 0x000b5410
void __cdecl GameRotateRwMatrix(CMatrix *matrix,const CVector *axis,float angle);
// samp.dll RVA: 0x000b5450
void __cdecl GameOrthoNormalizeRwMatrix(CMatrix *out,const CMatrix *matrix);
// samp.dll RVA: 0x000b5480
void __cdecl GameScaleRwMatrix(CMatrix *matrix,const CVector *scale);
// Raw backend layout; unlike GameQuaternionFromMatrix, no XYZW/WXYZ reorder.
// samp.dll RVA: 0x000b4d20
void GameRwQuaternionFromMatrix(void *quaternion,const CMatrix *matrix);
#endif

#ifndef SAMP_VECTOR_ADAPTERS_H
#define SAMP_VECTOR_ADAPTERS_H
// samp.dll RVA: 0x000b4630
void GameNormalizeVector(CVector *vector);
// samp.dll RVA: 0x000b4750
CVector *GameVectorCrossProduct(CVector *out,const CVector *left,const CVector *right);
// Transposed basis only; no translation.
// samp.dll RVA: 0x000b4790
CVector *GameTransformVectorByTranspose(CVector *out,const CVector *vector,const CMatrix *matrix);
#endif

#ifndef SAMP_RW_HELPERS_H
#define SAMP_RW_HELPERS_H
typedef CVector *(__cdecl *GameRwPointTransformBackend)(CVector*,const CVector*,int,const CMatrix*);
typedef void (__cdecl *GameRwAxisRotateBackend)(CMatrix*,const CVector*,float,int);
// samp.dll RVA: 0x001553d4
extern GameRwPointTransformBackend g_rwPointTransformBackend;
// samp.dll RVA: 0x001553fc
extern GameRwAxisRotateBackend g_rwAxisRotateBackend;
// samp.dll RVA: 0x0011f58c
extern const float g_headingInverseHalfTurn;
// samp.dll RVA: 0x0011f590
extern const float g_headingPi;
// samp.dll RVA: 0x000b5210
CVector *GameTransformRwVector(CVector *out,const CVector *vector,const CMatrix *matrix);
// samp.dll RVA: 0x000b5230
CVector *GameTransformRwPoint(CVector *out,const CVector *point,const CMatrix *matrix);
// samp.dll RVA: 0x000b5260
void GameRepeatRwMatrixInvert();
// samp.dll RVA: 0x000b5280
void GameInvertRwMatrix(CMatrix *out,const CMatrix *matrix);
// samp.dll RVA: 0x000b52d0
void GameRotateRwMatrixOnAxis(CMatrix *matrix,int axis,float angle);
// samp.dll RVA: 0x000b54b0
float GameHeadingToRadians(float heading);
#endif

#ifndef SAMP_RW_OBJECTS_H
#define SAMP_RW_OBJECTS_H
#include <windows.h>
// Opaque GTA/RenderWare objects, not SA-MP CEntity/CCamera wrapper pointers.
typedef void *(__cdecl *GameRwObjectCallback)(void *object,void *data);
// samp.dll RVA: 0x000b1010
int GameRwCountClumpAtomics(void *clump);
// samp.dll RVA: 0x000b1090
void *GameRwForAllAtomics(void *clump,GameRwObjectCallback callback,void *data);
// samp.dll RVA: 0x000b10c0
void *GameRwForAllFrameObjects(void *frame,GameRwObjectCallback callback,void *data);
// samp.dll RVA: 0x000b1360
void GameRwDestroyTexture(void *texture);
// samp.dll RVA: 0x000b13d0
void GameRwDestroyFrame(void *frame);
// samp.dll RVA: 0x000b1400
void GameRwDestroyAtomic(void *atomic);
// samp.dll RVA: 0x000b1470
void GameRwDestroyRaster(void *raster);
// samp.dll RVA: 0x000b14f0
void GameRwClearCamera(void *camera,const void *color,DWORD flags);
// samp.dll RVA: 0x000b1520
void GameRwBeginCameraUpdate(void *camera);
// samp.dll RVA: 0x000b1550
void GameRwEndCameraUpdate(void *camera);
// samp.dll RVA: 0x000b15d0
void GameRwRenderClump(void *clump);
// samp.dll RVA: 0x000b1740
void GameRwAddClumpLight(void *clump,void *light);
// samp.dll RVA: 0x000b17a0
void GameRwSetLightRadius(void *light,float radius);
// samp.dll RVA: 0x000b1880
BOOL GameRwDestroyClump(void *clump);
// samp.dll RVA: 0x000b1030
void *GameRwQueryAtomic48(const void *object);
// samp.dll RVA: 0x000b1130
void *__stdcall GameRwReadField34(const void *object);
// samp.dll RVA: 0x000b14b0
void GameRwWorldAddCamera(void *camera);
// samp.dll RVA: 0x000b1600
void GameRwWorldAddClump(void *clump);
// samp.dll RVA: 0x000b1630
void GameRwWorldRemoveClump(void *clump);
// samp.dll RVA: 0x000b1660
void GameRwWorldRender();
// samp.dll RVA: 0x000b17c0
void GameRwWorldAddLight(void *light);
// samp.dll RVA: 0x000b1800
void GameRwWorldRemoveLight(void *light);
// samp.dll RVA: 0x000b1850
void *GameRwCloneClump(void *clump);
// samp.dll RVA: 0x000b0ff0
DWORD GameRwInvokeEngineSlot20(DWORD first,DWORD second);
// samp.dll RVA: 0x000b1100
void *GameRwUnlockGeometry(void *geometry);
// samp.dll RVA: 0x000b1110
DWORD GameRwLockGeometry(void *geometry);
// samp.dll RVA: 0x000b1840
void GameRwSetVisibilityCamera(void *camera);
// samp.dll RVA: 0x000b1980
void GameRwUpdateClumpHierarchy(void *clump);
// samp.dll RVA: 0x000b19a0
void GameRwUpdateClumpAnimations(void *clump,float timeStep,DWORD onScreen);
// samp.dll RVA: 0x000b1050
void *GameRwQueryAtomic7c(void *object);
// samp.dll RVA: 0x000b5df0
void GameRwDestroyObject(void *object);
// samp.dll RVA: 0x000b5e30
BOOL __stdcall GameRwRenderAtomic(void *object);
// samp.dll RVA: 0x000b5e70
void __stdcall GameRwRenderObject(void *object);
#endif

#ifndef SAMP_GAME_POOL_HELPERS_H
#define SAMP_GAME_POOL_HELPERS_H
// Opaque GTA backend pointers; these are not SA-MP pool/wrapper objects.
// samp.dll RVA: 0x000b37a0
int __stdcall GamePoolPedGetIndex(void *ped);
// samp.dll RVA: 0x000b3800
DWORD __stdcall GamePoolVehicleGetIndex(void *vehicle);
// samp.dll RVA: 0x000b3870
void *GamePoolBuildingGetArray();
// samp.dll RVA: 0x000b3890
void *GamePoolDummyGetArray();
// samp.dll RVA: 0x000b38b0
void *GamePoolObjectGetArray();
// samp.dll RVA: 0x000b3930
bool GamePoolHasObjectModel(int model);
// Fixed physical GTA table span, independent of recovered custom-model bank.
// samp.dll RVA: 0x000b3a00
int GameCountEmptyBackendModelSlots();
#endif

#ifndef SAMP_RASTER_HELPERS_H
#define SAMP_RASTER_HELPERS_H
// samp.dll RVA: 0x002a9d98
extern int g_rwImageDepth;
// samp.dll RVA: 0x002a9d9c
extern int g_rwImageWidth;
// samp.dll RVA: 0x002a9a40
extern int g_rwImageHeight;
// samp.dll RVA: 0x002aae10
extern void * g_rwImageScratch;
// samp.dll RVA: 0x00155400
extern void ** g_rwSceneCameraSlot;
// samp.dll RVA: 0x000b5690
void GameWriteRasterBmp(void *raster,const char *filename);
// samp.dll RVA: 0x000b57d0
void GameCreateSceneRasters();
// Raw GPR-preserving entry, not a RenderWare camera method.
// samp.dll RVA: 0x000b5860
void GameRenderWithSceneRasters();
#endif

#ifndef SAMP_NATIVE_SCRATCH_H
#define SAMP_NATIVE_SCRATCH_H
// Shared physical native BSS span. Collision and rotation adapters use views
// inside it; this is not one established typed collision object.
// samp.dll RVA: 0x002aae18
extern BYTE g_entityCollisionScratch[0x1044];
#endif

// samp.dll RVA: 0x000b3e40
DWORD __stdcall GameTestCameraSphere(const CVector *centre,float radius);

// samp.dll RVA: 0x000b1170
void __stdcall GameRwReplaceField34(void *object,void *replacement);
// samp.dll RVA: 0x000b11c0
void * GameRwDumpAtomicMaterials(void *object,void *data);
// samp.dll RVA: 0x000b1260
void * __stdcall GameRwDumpObjectFrameMaterials(void *object);
// samp.dll RVA: 0x000b1280
void * GameRwKeepType1Object(void *object,void *data);
// samp.dll RVA: 0x000b12a0
void * __stdcall GameRwEnumerateObjectFrameType1(void *object);
// samp.dll RVA: 0x000b1970
void * GameRwGetObjectFrame(const void *object);
// samp.dll RVA: 0x000b1e90
void * GameRwGetClumpWord8Minus64(void *clump,int index);

// Shared original float storage, already defined by textdraw/dialog units.
// samp.dll RVA: 0x00117884
extern const float g_textDrawHalf;
// samp.dll RVA: 0x00118490
extern const float g_dialogLineScale;
// Native Newton approximation, not CRT sqrt; preserves signed bit shift and x87.
// samp.dll RVA: 0x000b55a0
float GameApproximateSqrt(float value);
// samp.dll RVA: 0x0011f414
extern const char g_rwMaterialCountText[];
// samp.dll RVA: 0x0011f3f0
extern const char g_rwMaterialNameText[];
// samp.dll RVA: 0x0011f3cc
extern const char g_rwMaterialNullText[];

// GTA SA virtual address: 0x00859524; raw float semantics unresolved.
// samp.dll RVA: 0x000b26d0
float GameReadFloat859524();
// GTA SA virtual address: 0x00859520; raw float semantics unresolved.
// samp.dll RVA: 0x000b26e0
float GameReadFloat859520();

// Opaque GTA entity/clump/hierarchy layout, not SA-MP wrapper pointers.
// samp.dll RVA: 0x000b62f0
BOOL GameCopyBackendBoneMatrix(void *entity,CMatrix *out,int bone);

// Opaque DWORD arguments/results; backend semantics are not established.
// samp.dll RVA: 0x000b14a0
DWORD GameCall705B60(DWORD argument);
// samp.dll RVA: 0x000b4770
DWORD GameCall59C790(DWORD first,DWORD second,DWORD third);
// Native scans14000 records of56 bytes starting at the building pool's +4 slot.
// samp.dll RVA: 0x000b4360
void GameReplaceBuildingPoolModels(int model,int replacement);

// Returns the unspilled extended difference.
// samp.dll RVA: 0x000b44a0
float GameSubtractFloats(float first,float second);

// dy squared plus dx squared, no intermediate float spill.
// samp.dll RVA: 0x000b4510
float __stdcall GameSquaredDistance2D(float x,float y,float otherX,float otherY);

// Square root of dy squared plus dx squared.
// samp.dll RVA: 0x000b4540
float __stdcall GameDistance2D(float x,float y,float otherX,float otherY);

// dz squared plus dy squared then dx squared.
// samp.dll RVA: 0x000b45b0
float __stdcall GameSquaredDistance3D(float x,float y,float z,float otherX,float otherY,float otherZ);

// dx squared plus dy squared then dz squared; RET8.
// samp.dll RVA: 0x000b45f0
float __stdcall GameSquaredDistanceVectors(const CVector *first,const CVector *second);

// x squared plus y squared then z squared.
// samp.dll RVA: 0x000b4640
float GameVectorLengthSquared(const CVector *vector);

// Inclusive signed unit bounds for all components; unordered values rejected.
// samp.dll RVA: 0x000b4670
bool GameIsVectorWithinUnitBounds(const CVector *vector);

// Sequential stores and input reloads preserve native alias behavior.
// samp.dll RVA: 0x000b47b0
void GameVectorCrossProductLocal(CVector *out,const CVector *left,const CVector *right);

// Square root of x squared plus y squared then z squared.
// samp.dll RVA: 0x000b47f0
float GameVectorLength(const CVector *vector);

// Separate native entry, same arithmetic as RVA000B4640; original name unknown.
// samp.dll RVA: 0x000b4820
float GameVectorLengthSquared4820(const CVector *vector);

// x squared plus y squared then z squared; cdecl.
// samp.dll RVA: 0x000b4b20
float GameLengthSquared3D(float x,float y,float z);

// z products plus y products then x products.
// samp.dll RVA: 0x000b4b40
float GameVectorDotProduct(const CVector *first,const CVector *second);

// Three sequential component stores; native input/output alias behavior.
// samp.dll RVA: 0x000b4cf0
void GameScaleVector(CVector *out,const CVector *vector,float scale);

// Native inclusive0..360 gate accepts NaN; preserves branch arithmetic.
// samp.dll RVA: 0x000b5520
float GameHeadingToRadiansBounded(float heading);

// Raw inline pool bytes at +4; signed capacity at +8; no null check.
// samp.dll RVA: 0x000b38d0
DWORD GameCountNonzeroPoolBytesB7449C();
// samp.dll RVA: 0x000b3900
DWORD GameCountNonzeroPoolBytesC8800C();
// samp.dll RVA: 0x000b4130
void __stdcall GameReplaceBackendVtable85BBF0(void *entity);
// samp.dll RVA: 0x000b42c0
void GameWriteGlobal9689E0(DWORD value);
// samp.dll RVA: 0x000b3620
float GameAsin(float value);

// Position12 bytes and float heading; null backend is not checked natively.
// samp.dll RVA: 0x000b6260
void GameGetBackendPositionAndHeading(void *entity,CVector *position,float *heading);
