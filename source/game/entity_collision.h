#ifndef SAMP_ENTITY_COLLISION_H
#define SAMP_ENTITY_COLLISION_H
#include "game/entity.h"
#include "game/native_scratch.h"
typedef bool (__cdecl *GameEntityCollisionBackend)(const GameEntityCollisionShape*,const GameEntityCollisionShape*,void*);
// Opaque native scratch span ends at saved-context RVA 002ABE5C.
// No typed collision-object layout or broader initialization completeness is claimed.
// samp.dll RVA: 0x0018de60
extern GameEntityCollisionShape g_entityCollisionShapeA;
// samp.dll RVA: 0x0018de4c
extern GameEntityCollisionShape g_entityCollisionShapeB;
// samp.dll RVA: 0x0018de7c
extern void *g_entityCollisionEntity;
// samp.dll RVA: 0x00155404
extern GameEntityCollisionBackend g_entityCollisionBackend;
// samp.dll RVA: 0x00155408
extern void **g_entityCollisionContextSlot;
// samp.dll RVA: 0x002abe5c
extern void *g_entityCollisionSavedContext;
// samp.dll RVA: 0x002abe60
extern BYTE g_entityCollisionResult;
// samp.dll RVA: 0x000b5ea0
float __cdecl GameGetModelCollisionRadius(int model);
// samp.dll RVA: 0x000b6070
bool __stdcall GameEntityCollisionQueryDirect(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,void *entity);
// samp.dll RVA: 0x000b6090
bool __stdcall GameEntityCollisionQueryScoped(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,void *entity);
// Only the four-float sphere prefix is read by this GTA query.
struct GameCollisionSphere { CVector centre; float radius; };
// samp.dll RVA: 0x000b60e0
BOOL GameTestCollisionSpheres(const GameCollisionSphere *first,const GameCollisionSphere *second);
#endif
