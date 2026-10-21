#ifndef SAMP_OBJECT_WRAPPER_H
#define SAMP_OBJECT_WRAPPER_H
#include "game/entity.h"
// Native allocation extent is established; the tail remains an opaque storage view.
// Constructor/lifecycle use the native opaque tail; field semantics remain partial.
class CVehicle;
#pragma pack(push,1)
class CObject : public CEntity {
    BYTE nativeState[0x1154];
public:
    // Native eleven-word entry; flag1193 retains its raw byte.
    // samp.dll RVA: 0x000a8d00
    CObject(int model,CVector position,CVector rotation,float drawDistance,
        BYTE flag1193,BYTE firstHour,BYTE lastHour);
    // samp.dll RVA: 0x000a9020
    virtual ~CObject();
    // samp.dll RVA: 0x000a79a0
    virtual void Add();
    // samp.dll RVA: 0x000a79f0
    virtual void Remove();
    // samp.dll RVA: 0x000a7d00
    void SetAttachedVehicle(WORD id,const CVector *offset,const CVector *rotation);
    // samp.dll RVA: 0x000a7d90
    void SetAttachedObject(WORD id,const CVector *offset,const CVector *rotation,BYTE flag);
    // samp.dll RVA: 0x000a80b0
    BOOL IsMoving();
    // samp.dll RVA: 0x000a79c0
    BOOL IsAdded();
    // samp.dll RVA: 0x000a8c30
    void NativeA8C30();
    // samp.dll RVA: 0x000a8c70
    void NativeA8C70();
    // samp.dll RVA: 0x000a8cc0
    bool HasRwFrameFlag8();
    // samp.dll RVA: 0x000a8bc0
    void UpdateCachedCentre();
    // samp.dll RVA: 0x000a7b40
    bool IsTimeVisible();
    // samp.dll RVA: 0x000a8b90
    void ClearModelFlag0();
    // samp.dll RVA: 0x000a9150
    void ProcessStreaming();
    // samp.dll RVA: 0x000a7bb0
    float GetDistanceToTarget(const CMatrix *matrix);
    // samp.dll RVA: 0x000a7e30
    void AttachToVehicle(CVehicle *vehicle);
    // samp.dll RVA: 0x000a7eb0
    void AttachToObject(CObject *object);
    // Downstream motion dependency of the recovered pool coordinator.
    // samp.dll RVA: 0x000a9240
    void Process(float elapsedSeconds);
    // samp.dll RVA: 0x000a8870
    bool GetMaterialSize(int code,int *width,int *height);
    // samp.dll RVA: 0x000a9ad0
    void ConstructMaterialText();
    // samp.dll RVA: 0x000a8ac0
    void ShutdownMaterialText();
    // samp.dll RVA: 0x000a7c90
    void SetRotation(const CVector *rotation);
    // samp.dll RVA: 0x000a7c20
    void Stop();
    // Native angle helper ignores this but consumes two float words.
    // samp.dll RVA: 0x000a7bf0
    float GetAngleDifference(float from,float to);
    // Rotation matrix builder dependency; not a production stand-in.
    // samp.dll RVA: 0x000a7fb0
    void Rotate(CVector rotation);
    // Original consumes seven float words.
    // samp.dll RVA: 0x000a97c0
    void Move(CVector position,float speed,CVector rotation);
};
#pragma pack(pop)
typedef char NativeObjectAllocationExtent[(sizeof(CObject)==0x119c)?1:-1];
// samp.dll RVA: 0x001e0498
extern void *g_objectSharedMaterialTexture;
// samp.dll RVA: 0x000b3830
void *__stdcall GamePoolObjectGetAt(int handle);
#endif
