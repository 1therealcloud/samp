#include "game/camera.h"
#include "game/scripting.h"
#include <string.h>
// Native destructor clears only the low BYTE of the matrix pointer.
// 0x0009fdb0
CCamera::~CCamera()
{
    m_pAttachedTo = 0;
    *(BYTE*)&m_pMatrix = 0;
}
// 0x0009d280
void CCamera::Fade(BOOL in)
{
    Scripting::ProcessCommand(&g_cameraFade, 500, in);
}
// 0x0009d2a0
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
// 0x0009d320
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
// 0x0009d3b0
void CCamera::TakeControl(void* target, int mode, int switchType)
{
    // The native adapter forwards complete stack DWORDs to GTA's thiscall.
    // clang-format off
    __asm {
        push 1
        push switchType
        push mode
        push target
        mov ecx,0xb6f028
        mov edx,0x50c7c0
        call edx
    }
    // clang-format on
}
// 0x0009d3d0
void CCamera::SetMoveVector(CVector* from, CVector* to, int time, BYTE mode)
{
    Scripting::ProcessCommand(&g_cameraStop);
    Scripting::ProcessCommand(&g_cameraPersistPosition, 1);
    Scripting::ProcessCommand(&g_cameraMove, (double)from->x, (double)from->y, (double)from->z, (double)to->x, (double)to->y, (double)to->z, time, (int)mode);
}
// 0x0009d440
void CCamera::SetTrackVector(CVector* from, CVector* to, int time, BYTE mode)
{
    Scripting::ProcessCommand(&g_cameraPersistTrack, 1);
    Scripting::ProcessCommand(&g_cameraTrack, (double)from->x, (double)from->y, (double)from->z, (double)to->x, (double)to->y, (double)to->z, time, (int)mode);
}
// Strict bounds, ordered comparisons. Native returns AL, not a WORD or BOOL.
// 0x000b46e0
bool __cdecl GameIsCameraPointValid(const CVector* point)
{
    return point->x < 20000.0f && point->x > -20000.0f && point->y < 20000.0f && point->y > -20000.0f && point->z < 100000.0f && point->z > -10000.0f;
}
// 0x0009d4a0
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
// 0x0009d4f0
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
// Keep native extended intermediates and return the unspilled x87 result.
// 0x0009d540
__declspec(naked) float CCamera::GetDistanceToPoint(CVector* point)
{
    // clang-format off
    __asm {
        mov eax,[esp+4]
        fld dword ptr [eax+8]
        fsub dword ptr ds:[0xb6f9d4]
        fld dword ptr [eax+4]
        fsub dword ptr ds:[0xb6f9d0]
        fld dword ptr [eax]
        fsub dword ptr ds:[0xb6f9cc]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        ret 4
    }
    // clang-format on
}
// 0x0009d580
void CCamera::Restore()
{
    Scripting::ProcessCommand(&g_cameraPersistPosition, 0);
    Scripting::ProcessCommand(&g_cameraStop);
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraRestoreJumpcut);
    Scripting::ProcessCommand(&g_cameraRestore);
}
// 0x0009d5c0
void CCamera::Set(CVector position, CVector rotation)
{
    Scripting::ProcessCommand(&g_cameraStop);
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraSet, (double)position.x, (double)position.y, (double)position.z, (double)rotation.x, (double)rotation.y, (double)rotation.z);
}
// 0x0009d620
void CCamera::PointAt(CVector point, int switchStyle)
{
    Scripting::ProcessCommand(&g_cameraStop);
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraPointAt, (double)point.x, (double)point.y, (double)point.z, switchStyle);
}
// 0x0009d670
void CCamera::Detach()
{
    m_pAttachedTo = 0;
    Scripting::ProcessCommand(&g_cameraRestore);
}

// 0x0009fda0
CCamera::CCamera()
{
    m_pMatrix = (CMatrix*)0xb6f99c;
    m_pAttachedTo = 0;
}
