#ifndef CAMERA_H
#define CAMERA_H

#include "game/entity.h"

class CCamera
{
public:
    CEntity* m_pAttachedTo;
    CMatrix* m_pMatrix;

    CCamera()
    {
        m_pMatrix = (CMatrix*)0xb6f99c;
        m_pAttachedTo = 0;
    }
    // Provisional external binding; standalone native destructor identity unverified.

    ~CCamera();

    void Fade(BOOL in);

    void GetMatrix(CMatrix* matrix);

    void SetMatrix(CMatrix matrix);
    // Target is a GTA entity pointer, not a recovered CEntity wrapper.

    void TakeControl(void* target, int mode, int switchType);

    void SetMoveVector(CVector* from, CVector* to, int time, BYTE mode);

    void SetTrackVector(CVector* from, CVector* to, int time, BYTE mode);

    void Attach(CEntity* owner);

    void SetToOwner();

    float GetDistanceToPoint(CVector* point);

    void Restore();

    void Set(CVector position, CVector rotation);

    void PointAt(CVector point, int switchStyle);

    void Detach();
};
static_assert(sizeof(CCamera) == 8);

BYTE __cdecl GameIsCameraPointValid(const CVector* point);

#endif // CAMERA_H
