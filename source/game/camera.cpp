#include "game/camera.h"

#include "game/scripting.h"

#include <math.h>
#include <string.h>
// Provisional SAMP-API attribution: this entry also fits CAudio initialization.
// No native references establish its class/lifecycle role; keep it unverified.

CCamera::~CCamera()
{
    m_pAttachedTo = 0;
    *(BYTE*)&m_pMatrix = 0;
}

void CCamera::Fade(BOOL in)
{
    Scripting::ProcessCommand(&g_cameraFade, 500, in);
}

void CCamera::GetMatrix(CMatrix* matrix)
{
    matrix->right.x = m_pMatrix->right.x;
    matrix->right.y = m_pMatrix->right.y;
    matrix->right.z = m_pMatrix->right.z;
    matrix->up.x = m_pMatrix->up.x;
    matrix->up.y = m_pMatrix->up.y;
    matrix->up.z = m_pMatrix->up.z;
    matrix->at.x = m_pMatrix->at.x;
    matrix->at.y = m_pMatrix->at.y;
    matrix->at.z = m_pMatrix->at.z;
    matrix->position.x = m_pMatrix->position.x;
    matrix->position.y = m_pMatrix->position.y;
    matrix->position.z = m_pMatrix->position.z;
}

void CCamera::SetMatrix(CMatrix matrix)
{
    m_pMatrix->right.x = matrix.right.x;
    m_pMatrix->right.y = matrix.right.y;
    m_pMatrix->right.z = matrix.right.z;
    m_pMatrix->up.x = matrix.up.x;
    m_pMatrix->up.y = matrix.up.y;
    m_pMatrix->up.z = matrix.up.z;
    m_pMatrix->at.x = matrix.at.x;
    m_pMatrix->at.y = matrix.at.y;
    m_pMatrix->at.z = matrix.at.z;
    m_pMatrix->position.x = matrix.position.x;
    m_pMatrix->position.y = matrix.position.y;
    m_pMatrix->position.z = matrix.position.z;
}

void CCamera::TakeControl(void* target, int mode, int switchType)
{
    // The native adapter forwards complete stack DWORDs to GTA's thiscall.
    typedef void(__fastcall * BackendFn)(void*, int, void*, DWORD, DWORD, DWORD);
    reinterpret_cast<BackendFn>(0x50c7c0)((void*)0xb6f028, 0, target, (DWORD)mode, (DWORD)switchType, 1);
}

void CCamera::SetMoveVector(CVector* from, CVector* to, int time, BYTE mode)
{
    Scripting::ProcessCommand(&g_cameraStop);
    Scripting::ProcessCommand(&g_cameraPersistPosition, 1);
    Scripting::ProcessCommand(&g_cameraMove, (double)from->x, (double)from->y, (double)from->z, (double)to->x, (double)to->y, (double)to->z, time, (int)mode);
}

void CCamera::SetTrackVector(CVector* from, CVector* to, int time, BYTE mode)
{
    Scripting::ProcessCommand(&g_cameraPersistTrack, 1);
    Scripting::ProcessCommand(&g_cameraTrack, (double)from->x, (double)from->y, (double)from->z, (double)to->x, (double)to->y, (double)to->z, time, (int)mode);
}

// Strict bounds, ordered comparisons. Native returns AL, not a WORD or BOOL.

BYTE __cdecl GameIsCameraPointValid(const CVector* point)
{
    return point->x < 20000.0f && point->x > -20000.0f && point->y < 20000.0f && point->y > -20000.0f && point->z < 100000.0f && point->z > -10000.0f;
}

void CCamera::Attach(CEntity* owner)
{
    m_pAttachedTo = owner;
    if (owner)
    {
        CMatrix matrix;
        owner->GetMatrix(&matrix);
        if (GameIsCameraPointValid(&matrix.position))
            SetMoveVector(&matrix.position, &matrix.position, 100, true);
    }
}

void CCamera::SetToOwner()
{
    if (m_pAttachedTo && m_pAttachedTo->m_pGameEntity)
    {
        CMatrix matrix;
        m_pAttachedTo->GetMatrix(&matrix);
        if (GameIsCameraPointValid(&matrix.position))
            SetMoveVector(&matrix.position, &matrix.position, 100, true);
    }
}

// VC7 /O2 keeps these intermediates in x87 and returns the native unspilled result.

float CCamera::GetDistanceToPoint(CVector* point)
{
    // GTA SA camera position VA: 0x00b6f9cc; native read order is Z, Y, X.
    float z = point->z - *(const float*)0xb6f9d4;
    float y = point->y - *(const float*)0xb6f9d0;
    float x = point->x - *(const float*)0xb6f9cc;
    return (float)sqrt(x * x + y * y + z * z);
}

void CCamera::Restore()
{
    Scripting::ProcessCommand(&g_cameraPersistPosition, 0);
    Scripting::ProcessCommand(&g_cameraStop);
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraRestoreJumpcut);
    Scripting::ProcessCommand(&g_cameraRestore);
}

void CCamera::Set(CVector position, CVector rotation)
{
    Scripting::ProcessCommand(&g_cameraStop);
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraSet, (double)position.x, (double)position.y, (double)position.z, (double)rotation.x, (double)rotation.y, (double)rotation.z);
}

void CCamera::PointAt(CVector point, int switchStyle)
{
    Scripting::ProcessCommand(&g_cameraStop);
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraPointAt, (double)point.x, (double)point.y, (double)point.z, switchStyle);
}

void CCamera::Detach()
{
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraRestore);
}
