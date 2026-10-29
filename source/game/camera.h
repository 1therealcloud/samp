#ifndef SAMP_RECOVERED_CAMERA_H
#define SAMP_RECOVERED_CAMERA_H
#include "game/entity.h"
#pragma pack(push, 1)
class CCamera
{
public:
    CEntity* m_pAttachedTo;
    CMatrix* m_pMatrix;
    // 0x0009fda0
    CCamera();
    // Provisional external binding; standalone native destructor identity unverified.
    // 0x0009fdb0
    ~CCamera();
    // 0x0009d280
    void Fade(BOOL in);
    // 0x0009d2a0
    void GetMatrix(CMatrix* matrix);
    // 0x0009d320
    void SetMatrix(CMatrix matrix);
    // Target is a GTA entity pointer, not a recovered CEntity wrapper.
    // 0x0009d3b0
    void TakeControl(void* target, int mode, int switchType);
    // 0x0009d3d0
    void SetMoveVector(CVector* from, CVector* to, int time, BYTE mode);
    // 0x0009d440
    void SetTrackVector(CVector* from, CVector* to, int time, BYTE mode);
    // 0x0009d4a0
    void Attach(CEntity* owner);
    // 0x0009d4f0
    void SetToOwner();
    // 0x0009d540
    float GetDistanceToPoint(CVector* point);
    // 0x0009d580
    void Restore();
    // 0x0009d5c0
    void Set(CVector position, CVector rotation);
    // 0x0009d620
    void PointAt(CVector point, int switchStyle);
    // 0x0009d670
    void Detach();
};
#pragma pack(pop)
typedef char CameraExtent[(sizeof(CCamera) == 8) ? 1 : -1];
// 0x000b46e0
BYTE __cdecl GameIsCameraPointValid(const CVector* point);
#endif
