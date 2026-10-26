#ifndef SAMP_OBJECT_MATH_H
#define SAMP_OBJECT_MATH_H
#include "game/entity.h"
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
