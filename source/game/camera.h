#ifndef SAMP_RECOVERED_CAMERA_H
#define SAMP_RECOVERED_CAMERA_H
#include "game/entity.h"
#pragma pack(push,1)
class CCamera {
public:
    CEntity *m_pAttachedTo;
    CMatrix *m_pMatrix;
    // samp.dll RVA: 0x0009fda0
    CCamera();
    // samp.dll RVA: 0x0009fdb0
    ~CCamera();
    // samp.dll RVA: 0x0009d280
    void Fade(BOOL in);
    // samp.dll RVA: 0x0009d2a0
    void GetMatrix(CMatrix *matrix);
    // samp.dll RVA: 0x0009d320
    void SetMatrix(CMatrix matrix);
    // Target is a GTA entity pointer, not a recovered CEntity wrapper.
    // samp.dll RVA: 0x0009d3b0
    void TakeControl(void *target,short mode,short switchType);
    // samp.dll RVA: 0x0009d3d0
    void SetMoveVector(CVector *from,CVector *to,int time,bool smooth);
    // samp.dll RVA: 0x0009d440
    void SetTrackVector(CVector *from,CVector *to,int time,bool smooth);
    // samp.dll RVA: 0x0009d4a0
    void Attach(CEntity *owner);
    // samp.dll RVA: 0x0009d4f0
    void SetToOwner();
    // samp.dll RVA: 0x0009d540
    float GetDistanceToPoint(CVector *point);
    // samp.dll RVA: 0x0009d580
    void Restore();
    // samp.dll RVA: 0x0009d5c0
    void Set(CVector position,CVector rotation);
    // samp.dll RVA: 0x0009d620
    void PointAt(CVector point,int switchStyle);
    // samp.dll RVA: 0x0009d670
    void Detach();
};
#pragma pack(pop)
typedef char CameraExtent[(sizeof(CCamera)==8)?1:-1];
// samp.dll RVA: 0x000b46e0
bool __cdecl GameIsCameraPointValid(const CVector *point);
#endif
