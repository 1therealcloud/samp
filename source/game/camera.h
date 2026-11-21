#ifndef CAMERA_H
#define CAMERA_H

#include "game/entity.h"

#include <static_assert.h>

#pragma pack(push, 1)
class CCamera
{
public:
    CEntity* m_pAttachedTo;
    CMatrix* m_pMatrix;
    // 0x0009ff60
    CCamera()
    {
        m_pMatrix = (CMatrix*)0xb6f99c;
        m_pAttachedTo = 0;
    }
    // Provisional external binding; standalone native destructor identity unverified.
    // 0x0009ff70
    ~CCamera();
    // 0x0009d440
    void Fade(BOOL in);
    // 0x0009d460
    void GetMatrix(CMatrix* matrix);
    // 0x0009d4e0
    void SetMatrix(CMatrix matrix);
    // Target is a GTA entity pointer, not a recovered CEntity wrapper.
    // 0x0009d570
    void TakeControl(void* target, int mode, int switchType);
    // 0x0009d590
    void SetMoveVector(CVector* from, CVector* to, int time, BYTE mode);
    // 0x0009d600
    void SetTrackVector(CVector* from, CVector* to, int time, BYTE mode);
    // 0x0009d660
    void Attach(CEntity* owner);
    // 0x0009d6b0
    void SetToOwner();
    // 0x0009d700
    float GetDistanceToPoint(CVector* point);
    // 0x0009d740
    void Restore();
    // 0x0009d780
    void Set(CVector position, CVector rotation);
    // 0x0009d7e0
    void PointAt(CVector point, int switchStyle);
    // 0x0009d830
    void Detach();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CCamera) == 8);
// 0x000b4b50
BYTE __cdecl GameIsCameraPointValid(const CVector* point);

#endif // CAMERA_H
