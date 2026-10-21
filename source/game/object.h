#ifndef SAMP_OBJECT_WRAPPER_H
#define SAMP_OBJECT_WRAPPER_H
#include "game/entity.h"
// Address interface over the established CEntity prefix. Never allocate this partial class:
// complete CObject allocation/constructor/layout remain unrecovered.
class CVehicle;
class CObject : public CEntity {public:
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
// samp.dll RVA: 0x000b3830
void *__stdcall GamePoolObjectGetAt(int handle);
#endif
