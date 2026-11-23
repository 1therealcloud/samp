#include "gui/objectedit.h"

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "filehooks.h"
#include "game/entity.h"
#include "game/object.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "gui/cmdwindow.h"
#include "gui/gui.h"
#include "main.h"

#include <BitStream.h>
#include <RakClientInterface.h>

#include <math.h>
#include <string.h>
// 0x0011a7ac
const float g_cursorRayUnsignedBias = 4294967296.0f;
// 0x00118388
const char g_objectEditGlyphMax[4] = "C";
// 0x0011c340
const char g_objectEditGlyphXPosition[4] = "6";
// 0x0011c33c
const char g_objectEditGlyphXRotation[4] = "3";
// 0x00118394
const char g_objectEditGlyphXScale[4] = "9";
// 0x00118304
const char g_objectEditGlyphYPosition[4] = "7";
// 0x00117880
const char g_objectEditGlyphYRotation[4] = "4";
// 0x00118308
const char g_objectEditGlyphYScale[4] = "A";
// 0x001183e0
const char g_objectEditGlyphZPosition[4] = "8";
// 0x00117950
const char g_objectEditGlyphZRotation[4] = "5";
// 0x001184c0
const char g_objectEditGlyphZScale[4] = "B";
// 0x001182d8
const char g_objectEditGlyphDefault[4] = "0";
// 0x0011c310
extern int g_inputCommandRpcId;
// 0x0011c344
const char g_objectEditGlyphScale[4] = "1";
// 0x00118350
const char g_objectEditGlyphSave[4] = "H";

// 0x0011c768
const float g_objectEditDistanceScale = 0.125f;
// 0x00117f2c
int g_objectEditRpc = 117;
// 0x00117f30
int g_objectEditAttachedRpc = 116;

// 0x00017850
WORD CObjectEdit::GetObjectId()
{
    return objectId;
}

// 0x00072ff0
BOOL CObjectEdit::MsgProc(UINT message, WPARAM, LPARAM)
{
    BYTE* state = (BYTE*)this;
    if (!editing)
        return FALSE;
    if (message == WM_LBUTTONDOWN)
    {
        DWORD type = hoveredControl;
        if (!state[0xa3] && (type == 0 || type == 1 || type == 2))
        {
            state[0xa3] = 1;
            GetCursorPos((POINT*)(state + 0xa7));
            GetCursorPos((POINT*)(state + 0xaf));
            ScreenToClient((*(HWND*)0x00c97c1c), (POINT*)(state + 0xaf));
            SetCapture((*(HWND*)0x00c97c1c));
            ((int(WINAPI*)(BOOL))g_archiveRealShowCursor)(FALSE);
            ClipCursor(0);
            CObjectPool* pool = *(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
            CEntity* object = pool->Get(objectId);
            if (object)
                object->GetMatrix((CMatrix*)(state + 0xc3));
            return TRUE;
        }
    }
    else if (message == WM_LBUTTONUP)
    {
        if (state[0xa3])
        {
            state[0xa3] = 0;
            ReleaseCapture();
            ((int(WINAPI*)(BOOL))g_archiveRealShowCursor)(TRUE);
            POINT point;
            if (hoveredControl == 0)
                point = *(POINT*)(state + 8);
            else if (hoveredControl == 1)
                point = *(POINT*)(state + 0x18);
            else if (hoveredControl == 2)
                point = *(POINT*)(state + 0x28);
            else
                point = *(POINT*)(state + 0xaf);
            ClientToScreen((*(HWND*)0x00c97c1c), &point);
            SetCursorPos(point.x, point.y);
            TryChangeProcessType();
            return TRUE;
        }
        if (hoveredControl == 3)
        {
            SetEditMode(0);
            return TRUE;
        }
        if (hoveredControl == 4)
        {
            SetEditMode(1);
            return TRUE;
        }
        if (hoveredControl == 5)
        {
            SetEditMode(2);
            return TRUE;
        }
        if (hoveredControl == 10)
        {
            Disable(TRUE);
            return TRUE;
        }
    }
    else if (message == WM_MOUSEMOVE && state[0xa3])
    {
        ProcessMouseMove();
        return TRUE;
    }
    return FALSE;
}

// 0x00071b40
void CObjectEdit::TryChangeProcessType()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient((*(HWND*)0x00c97c1c), &point);
    if (!controlsVisible)
    {
        hoveredControl = 0xffffffff;
        return;
    }
    if (!g_pGame->m_nCursorMode)
    {
        hoveredControl = 0xffffffff;
        return;
    }
    BYTE* state = (BYTE*)this;
    if (PtInRect((RECT*)(state + 0x8), point))
        hoveredControl = 0;
    else if (PtInRect((RECT*)(state + 0x18), point))
        hoveredControl = 1;
    else if (PtInRect((RECT*)(state + 0x28), point))
        hoveredControl = 2;
    else if (PtInRect((RECT*)(state + 0x38), point))
        hoveredControl = 3;
    else if (PtInRect((RECT*)(state + 0x48), point))
        hoveredControl = 4;
    else if (PtInRect((RECT*)(state + 0x58), point))
        hoveredControl = 5;
    else if (PtInRect((RECT*)(state + 0x68), point))
        hoveredControl = 10;
    else
        hoveredControl = 0xffffffff;
}

// 0x00071c90
void CObjectEdit::SetEditMode(int mode)
{
    if (editTarget == 1 && mode == 2)
        mode = 0;
    editMode = mode;
}

// 0x00071e60
void CObjectEdit::ResetMousePos()
{
    RECT rect;
    GetClientRect((*(HWND*)0x00c97c1c), &rect);
    int x = (LONG)((DWORD)rect.right - (DWORD)rect.left) / 2, y = (LONG)((DWORD)rect.bottom - (DWORD)rect.top) / 2;
    BYTE* state = (BYTE*)this;
    *(int*)(state + 0xa7) = x;
    *(int*)(state + 0xab) = y;
    SetCursorPos(x, y);
}

// 0x00072f00
void CObjectEdit::ApplyChanges(int axis, float difference)
{
    if (editTarget == 1)
        ApplyObjectChanges(axis, difference);
    else if (editTarget == 2)
        ApplyAttachedChanges(axis, difference);
}

// 0x00072f20
void CObjectEdit::ProcessMouseMove()
{
    POINT point;
    DWORD vertical = 0;
    GetCursorPos(&point);
    BYTE* state = (BYTE*)this;
    int axis = (int)hoveredControl;
    BYTE reverse = 0;
    if (axis == 0)
    {
        reverse = state[0xa4];
        vertical = *(DWORD*)(state + 0xb7);
    }
    else if (axis == 1)
    {
        reverse = state[0xa5];
        vertical = *(DWORD*)(state + 0xbb);
    }
    else if (axis == 2)
    {
        reverse = state[0xa6];
        vertical = *(DWORD*)(state + 0xbf);
    }
    int delta;
    if (editMode == 1)
    {
        if (vertical)
            goto horizontal;
    }
    else if (!vertical)
        goto horizontal;
    if (reverse)
        delta = *(int*)(state + 0xab) - point.y;
    else
        delta = point.y - *(int*)(state + 0xab);
    goto apply;
horizontal:
    if (reverse)
        delta = *(int*)(state + 0xa7) - point.x;
    else
        delta = point.x - *(int*)(state + 0xa7);
apply:
    if (delta)
    {
        ApplyChanges(axis, (float)delta * 0.01f);
        ResetMousePos();
    }
}

// 0x00072cb0
void CObjectEdit::ApplyObjectChanges(int axis, float difference)
{
    CObjectPool* pool = *(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    CObject* object = (CObject*)pool->Get(objectId);
    if (!object)
        return;
    float x, y, z;
    object->GetEulerInverted(&x, &y, &z);
    object->ClearBackendFlag8();
    CMatrix matrix;
    memcpy(&matrix, (BYTE*)this + 0xc3, 64);
    CVector position = matrix.position, delta = {0, 0, 0};
    if (axis >= 0 && axis < 3)
    {
        if (editMode == 0)
        {
            ((float*)&delta)[axis] = difference;
            GameTransformPoint(&position, &matrix, &delta);
            matrix.position = position;
            CVector noRotation = {-1000.0f, -1000.0f, -1000.0f};
            object->Move(position, 1.0f, noRotation);
        }
        else if (editMode == 1)
        {
            float* angle = (float*)(objectRotation + axis * 4);
            *angle = GameNormalizeAngle(difference * g_objectRotationFactor + *angle);
            object->SetRotation((CVector*)(objectRotation));
        }
    }
    SendEditEndNotification(2);
    memcpy((BYTE*)this + 0xc3, &matrix, 64);
}
// Preserve the native x87 load order for the caller's difference argument.
template <int axis>
static __forceinline void ApplyAttachedAxis(BYTE* record, DWORD mode, const volatile float& difference)
{
    if (mode == 0)
    {
        float* value = (float*)(record + 8 + axis * 4);
        *value = difference * g_objectMutationStep + *value;
    }
    else if (mode == 1)
    {
        float* value = (float*)(record + 0x14 + axis * 4);
        *value = GameNormalizeAngle(difference * g_objectRotationFactor + *value);
    }
    else if (mode == 2)
    {
        float* value = (float*)(record + 0x20 + axis * 4);
        *value = difference * g_objectMutationStep + *value;
        if (*value < g_objectScaleLowerBound)
            *value = 0;
    }
}

// 0x00071cb0
void CObjectEdit::ApplyAttachedChanges(int axis, float difference)
{
    int slot = attachedObjectSlot;
    CPed* ped = g_pGame->GetPlayerPed();
    if (slot < 0 || slot >= 10 || !ped->m_accessoryOccupied[slot])
        return;
    BYTE* record = (BYTE*)&ped->m_accessoryInfo[slot];
    if (!record)
        return;
    if (axis == 0)
        ApplyAttachedAxis<0>(record, editMode, difference);
    else if (axis == 1)
        ApplyAttachedAxis<1>(record, editMode, difference);
    else if (axis == 2)
        ApplyAttachedAxis<2>(record, editMode, difference);
}

// 0x00071600
CObjectEdit::CObjectEdit(IDirect3DDevice9* value)
{
    device = value;
    editMode = 0;
    hoveredControl = 0xffffffff;
    editing = 0;
    controlsVisible = 0;
    editTarget = 0;
    flags[1] = 0;
    flags[2] = 0;
    flags[3] = 0;
    flags[4] = 0;
    flags[0] = 0;
    objectId = 0xffff;
    lastTick = GetTickCount();
    D3DXCreateLine(value, &line);
    D3DXCreateFontA(value, 22, 0, 400, 1, FALSE, SYMBOL_CHARSET, 0, 0, 0, "SAMPAUX3", &smallFont);
    D3DXCreateFontA(value, 28, 0, 400, 1, FALSE, SYMBOL_CHARSET, 0, 0, 0, "SAMPAUX3", &largeFont);
}

// 0x000716c0
float CObjectEdit::WorldToScreen(const CVector* position, float* screen)
{
    CVector point;
    memcpy(&point, position, 12);
    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);
    D3DXMATRIX identity;
    D3DXMatrixIdentity(&identity);
    D3DXVECTOR3 projected;
    D3DXVec3Project(&projected, (const D3DXVECTOR3*)&point, &viewport, (const D3DXMATRIX*)&g_deviceProjectionMatrix, (const D3DXMATRIX*)&g_deviceViewMatrix, &identity);
    screen[0] = projected.x;
    memcpy(&screen[1], &projected.y, sizeof(float));
    return projected.z;
}

// 0x000717c0
int CObjectEdit::RenderAxes(const CMatrix* matrix, float axisSize)
{
    if (!line)
        return 0;
    line->SetAntialias(TRUE);
    CVector delta, point;
    D3DXVECTOR2 vertices[2];
    float negativeSize;
    for (int axis = 0; axis < 3; ++axis)
    {
        delta.x = 0;
        delta.y = 0;
        delta.z = 0;
        memcpy((float*)&delta + axis, &axisSize, 4);
        GameTransformPoint(&point, matrix, &delta);
        if (WorldToScreen(&point, (float*)&vertices[0]) > g_quaternionMatrixOne)
            return 0;
        if (axis == 0)
            negativeSize = -axisSize;
        memcpy((float*)&delta + axis, &negativeSize, 4);
        GameTransformPoint(&point, matrix, &delta);
        if (WorldToScreen(&point, (float*)&vertices[1]) > g_quaternionMatrixOne)
            return 0;
        line->Begin();
        line->Draw(vertices, 2, 0xffcc9999);
        line->End();
    }
    return 1;
}

// 0x000739e0
void __cdecl ObjectEditBuildCursorRay(float rayLength, CVector* origin, CVector* end)
{
    IDirect3DSurface9* surface = 0;
    D3DSURFACE_DESC description;
    g_gameD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);
    surface->GetDesc(&description);
    surface->Release();
    POINT point;
    GetCursorPos(&point);
    ScreenToClient((*(HWND*)0x00c97c1c), &point);
    const float* projection = (const float*)&g_deviceProjectionMatrix;
    const volatile float normalizedX = (float)((2.0 * point.x / description.Width - g_quaternionMatrixOne) / projection[0]);
    const volatile float normalizedY = (float)(-(2.0 * point.y / description.Height - g_quaternionMatrixOne) / projection[5]);
    D3DXMATRIX product, copy, inverse;
    D3DXMatrixMultiply(&product, (const D3DXMATRIX*)&g_deviceWorldMatrix, (const D3DXMATRIX*)&g_deviceViewMatrix);
    memcpy(&copy, &product, 64);
    // Native ignores singular-matrix/API failures and leaves untouched output bytes.
    D3DXMatrixInverse(&inverse, 0, &copy);
    end->x = (float)((double)inverse._21 * normalizedY + (double)inverse._11 * normalizedX + inverse._31);
    end->y = (float)((double)inverse._22 * normalizedY + (double)inverse._12 * normalizedX + inverse._32);
    end->z = (float)((double)inverse._23 * normalizedY + (double)inverse._13 * normalizedX + inverse._33);
    memcpy(&origin->y, &inverse._42, sizeof(float));
    origin->x = inverse._41;
    memcpy(&origin->z, &inverse._43, sizeof(float));
    end->x = (float)((double)rayLength * end->x + inverse._41);
    end->y = (float)((double)rayLength * end->y + origin->y);
    end->z = (float)((double)rayLength * end->z + origin->z);
}

// 0x00071a30
const char* CObjectEdit::GetMaxSizeChar()
{
    return g_objectEditGlyphMax;
}

// 0x00071a40
const char* CObjectEdit::GetRenderChar(int axis)
{
    if (axis == 0)
    {
        if (editMode == 0)
            return g_objectEditGlyphXPosition;
        if (editMode == 1)
            return g_objectEditGlyphXRotation;
        if (editMode == 2)
            return g_objectEditGlyphXScale;
    }
    else if (axis == 1)
    {
        if (editMode == 0)
            return g_objectEditGlyphYPosition;
        if (editMode == 1)
            return g_objectEditGlyphYRotation;
        if (editMode == 2)
            return g_objectEditGlyphYScale;
    }
    else if (axis == 2)
    {
        if (editMode == 0)
            return g_objectEditGlyphZPosition;
        if (editMode == 1)
            return g_objectEditGlyphZRotation;
        if (editMode == 2)
            return g_objectEditGlyphZScale;
    }
    return g_objectEditGlyphDefault;
}

// 0x00071ae0
void CObjectEdit::CenterButtonRect(RECT* out, const POINT* center)
{
    LONG width = *(LONG*)controlLayout;
    LONG half = width / 2;
    out->left = (LONG)((DWORD)center->x - (DWORD)half);
    out->right = (LONG)(*(DWORD*)controlLayout + (DWORD)out->left);
    LONG height = *(LONG*)(controlLayout + 4);
    half = height / 2;
    out->top = (LONG)((DWORD)center->y - (DWORD)half);
    out->bottom = (LONG)(*(DWORD*)(controlLayout + 4) + (DWORD)out->top);
}

// 0x00071b20
void CObjectEdit::ClearButtonRect(RECT* out)
{
    out->left = 0;
    out->right = 0;
    out->top = 0;
    out->bottom = 0;
}

// 0x00071ec0
void CObjectEdit::EnterEditObject(WORD id, BOOL playerObject)
{
    editing = 1;
    editTarget = 1;
    SetEditMode(0);
    objectId = id;
    this->playerObject = playerObject;
    flags[0] = 0;
    g_pGame->SetCursorMode(0, TRUE);
    g_pGame->m_nInputEnableWaitFrames = 0;
    g_pGame->ProcessInputEnabling();
    if (!g_pNetGame)
        return;
    BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    // The original also accepts object ID 2100.
    if (id > 2100 || !*(DWORD*)(pool + 4 + id * 4))
        return;
    BYTE* object = *(BYTE**)(pool + 0x20d4 + id * 4);
    if (!object)
        return;
    *(DWORD*)(objectRotation) = *(DWORD*)(object + 0xad);
    *(DWORD*)(objectRotation + 4) = *(DWORD*)(object + 0xb1);
    *(DWORD*)(objectRotation + 8) = *(DWORD*)(object + 0xb5);
}

// 0x00071f80
void CObjectEdit::EnterEditAttachedObject(int index)
{
    editing = 1;
    editTarget = 2;
    SetEditMode(0);
    attachedObjectSlot = index;
    objectId = 0xffff;
    flags[0] = 1;
    g_pGame->SetCursorMode(0, TRUE);
    g_pGame->m_nInputEnableWaitFrames = 0;
    g_pGame->ProcessInputEnabling();
}

// Also used as the string "2" by the native editor; share the existing RPC storage.
// 0x00071fe0
void CObjectEdit::DrawButton(int processType, RECT rect)
{
    DWORD background = 0x99222222, foreground = 0xff999999;
    int slot;
    if (processType >= 0 && processType <= 2)
    {
        slot = processType;
        if (hoveredControl == (DWORD)processType)
            background = flags[1] ? 0xff774444 : 0xffaaaaaa;
        foreground = processType == 0 ? 0xff9999cc : processType == 1 ? 0xffcc9999 : 0xff99cc99;
    }
    else if (processType >= 3 && processType <= 5)
    {
        slot = processType;
        if (hoveredControl == (DWORD)processType)
            background = 0xffaaaaaa;
        if (editMode == (DWORD)(processType - 3))
            background = 0xff774444;
    }
    else if (processType == 10)
    {
        slot = 6;
        if (hoveredControl == 10)
            background = 0xffaaaaaa;
    }
    else
        return;
    largeFont->DrawTextA(0, g_objectEditGlyphMax, 1, &rect, 0x105, background);
    ID3DXFont* font = smallFont;
    const char* glyph;
    if (processType <= 2)
        glyph = GetRenderChar(processType);
    else if (processType == 3)
        glyph = (const char*)&g_inputCommandRpcId;
    else if (processType == 4)
        glyph = g_objectEditGlyphDefault;
    else if (processType == 5)
        glyph = g_objectEditGlyphScale;
    else
        glyph = g_objectEditGlyphSave;
    font->DrawTextA(0, glyph, 1, &rect, 0x105, foreground);
    CenterButtonRect((RECT*)((BYTE*)this + 8 + slot * 16), (const POINT*)&rect);
}

// 0x000726d0
BOOL CObjectEdit::RenderControlsForObject(const CMatrix* matrix, float axisSize)
{
    if (!line)
        return FALSE;
    RECT measured; // Native does not initialize this, including the null-font path.
    if (largeFont)
        largeFont->DrawTextA(0, GetMaxSizeChar(), -1, &measured, 0x505, 0xff000000);
    *(DWORD*)controlLayout = (DWORD)measured.right - (DWORD)measured.left;
    *(DWORD*)(controlLayout + 4) = (DWORD)measured.bottom - (DWORD)measured.top;
    line->SetAntialias(TRUE);
    D3DXVECTOR2 vertices[2];
    const DWORD colors[3] = {0xff9999cc, 0xffcc9999, 0xff99cc99};
    float negativeSize;
    for (int axis = 0; axis < 3; axis++)
    {
        CVector delta = {0, 0, 0}, point;
        memcpy((float*)&delta + axis, &axisSize, 4);
        GameTransformPoint(&point, matrix, &delta);
        if (WorldToScreen(&point, (float*)&vertices[0]) > g_quaternionMatrixOne)
            return FALSE;
        if (axis == 0)
            negativeSize = -axisSize;
        memcpy((float*)&delta + axis, &negativeSize, 4);
        GameTransformPoint(&point, matrix, &delta);
        if (WorldToScreen(&point, (float*)&vertices[1]) > g_quaternionMatrixOne)
            return FALSE;
        line->Begin();
        line->Draw(vertices, 2, colors[axis]);
        line->End();
        if (!flags[1])
        {
            const double dx = fabs((double)vertices[0].x - vertices[1].x);
            const double dy = fabs((double)vertices[0].y - vertices[1].y);
            const bool horizontal = dy < dx;
            const bool reverse = horizontal ? vertices[0].x < vertices[1].x : vertices[0].y < vertices[1].y;
            *((BYTE*)this + 0xa4 + axis) = reverse;
            *(DWORD*)((BYTE*)this + 0xb7 + axis * 4) = horizontal ? 0 : 1;
        }
        LONG x = (LONG)vertices[0].x, y = (LONG)vertices[0].y;
        RECT rect = {x, y, (LONG)((DWORD)x + 1), (LONG)((DWORD)y + 1)};
        DrawButton(axis, rect);
    }
    LONG x = (LONG)vertices[1].x, y = (LONG)vertices[1].y;
    RECT rect = {x, y, (LONG)((DWORD)x + 1), (LONG)((DWORD)y + 1)};
    DrawButton(10, rect);
    if (flags[0])
    {
        DWORD shift = 0xfffffffe - *(DWORD*)controlLayout;
        rect.left = (LONG)((DWORD)rect.left + shift);
        rect.right = (LONG)((DWORD)rect.right + shift);
        DrawButton(5, rect);
    }
    else
        ClearButtonRect((RECT*)((BYTE*)this + 0x58));
    DWORD shift = 0xfffffffe - *(DWORD*)controlLayout;
    rect.left = (LONG)((DWORD)rect.left + shift);
    rect.right = (LONG)((DWORD)rect.right + shift);
    DrawButton(4, rect);
    shift = 0xfffffffe - *(DWORD*)controlLayout;
    rect.left = (LONG)((DWORD)rect.left + shift);
    rect.right = (LONG)((DWORD)rect.right + shift);
    DrawButton(3, rect);
    return TRUE;
}

// 0x00072ca0
BOOL CObjectEdit::RenderControls(const CMatrix* matrix, float axisSize)
{
    return RenderControlsForObject(matrix, axisSize);
}

// 0x00073220
void CObjectEdit::Render()
{
    controlsVisible = 0;
    if (!editing)
        return;
    WORD leftRight, upDown;
    WORD keys = g_pGame->GetPlayerPed()->GetKeys(&leftRight, &upDown);
    if (flags[1])
        g_gameD3DDevice->ShowCursor(FALSE);
    else if (keys & 8)
        g_pGame->SetCursorMode(0, TRUE);
    else
    {
        g_gameD3DDevice->ShowCursor(TRUE);
        g_pGame->SetCursorMode(3, TRUE);
    }
    if (!g_pNetGame)
        return;
    if (editTarget == 1)
    {
        WORD id = objectId;
        BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
        if (id <= 2100 && *(DWORD*)(pool + 4 + id * 4))
        {
            CEntity* object = *(CEntity**)(pool + 0x20d4 + id * 4);
            if (object)
            {
                CMatrix matrix;
                object->GetMatrix(&matrix);
                const float distance = (float)((double)object->GetDistanceToCamera() * g_objectEditDistanceScale);
                if (RenderControls(&matrix, distance)) controlsVisible = 1;
            }
        }
    }
    else if (editTarget == 2)
    {
        int index = attachedObjectSlot;
        CEntity* object = (CEntity*)g_pGame->GetPlayerPed()->GetAccessory(index);
        // Reload the index and player after the first call, even when object is null.
        PedAccessoryState* info = g_pGame->GetPlayerPed()->GetAccessoryInfo(attachedObjectSlot);
        if (object)
        {
            CMatrix matrix;
            object->GetMatrix(&matrix);
            const volatile float distance = object->GetDistanceToCamera();
            if (!editMode)
            {
                CMatrix boneMatrix;
                g_pGame->GetPlayerPed()->GetBoneMatrix(&boneMatrix, *(int*)((BYTE*)info + 4));
                // Preserve matrix position and all three untouched padding DWORDs.
                memcpy(&matrix.up, &boneMatrix.up, 12);
                memcpy(&matrix.right, &boneMatrix.right, 12);
                memcpy(&matrix.at, &boneMatrix.at, 12);
            }
            if (RenderControls(&matrix, distance * g_objectEditDistanceScale)) controlsVisible = 1;
        }
    }
    if (!flags[1])
        TryChangeProcessType();
}

// 0x00072350
void CObjectEdit::SendEditEndNotification(int status)
{
    BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    WORD id = objectId;
    if (id > 2100 || !*(DWORD*)(pool + 4 + id * 4))
        return;
    CEntity* object = *(CEntity**)(pool + 0x20d4 + id * 4);
    if (!object)
        return;
    if (status == 2 && (LONG)(GetTickCount() - lastTick) < 250)
        return;
    DWORD tick = GetTickCount();
    DWORD rotation[3];
    rotation[0] = *(DWORD*)(objectRotation);
    rotation[2] = *(DWORD*)(objectRotation + 8);
    lastTick = tick;
    rotation[1] = *(DWORD*)(objectRotation + 4);
    CMatrix matrix;
    object->GetMatrix(&matrix);
    RakNet::BitStream stream;
    if (playerObject)
        stream.Write1();
    else
        stream.Write0();
    DWORD value = objectId;
    stream.WriteBits((const BYTE*)&value, 16, true);
    value = status;
    stream.WriteBits((const BYTE*)&value, 32, true);
    memcpy(&value, &matrix.position.x, 4);
    stream.WriteBits((const BYTE*)&value, 32, true);
    memcpy(&value, &matrix.position.y, 4);
    stream.WriteBits((const BYTE*)&value, 32, true);
    memcpy(&value, &matrix.position.z, 4);
    stream.WriteBits((const BYTE*)&value, 32, true);
    value = rotation[0];
    stream.WriteBits((const BYTE*)&value, 32, true);
    value = rotation[1];
    stream.WriteBits((const BYTE*)&value, 32, true);
    value = rotation[2];
    stream.WriteBits((const BYTE*)&value, 32, true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_objectEditRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

// 0x00072560
void CObjectEdit::SendAttachedEditEndNotification(int status)
{
    RakNet::BitStream stream;
    int index = attachedObjectSlot;
    CPed* ped = g_pGame->GetPlayerPed();
    // Inline native occupancy/bounds lookup; the 52-byte record remains live.
    PedAccessoryState* info = index >= 0 && index < 10 && ped->m_accessoryOccupied[index] ? &ped->m_accessoryInfo[index] : 0;
    DWORD value = status;
    stream.WriteBits((const BYTE*)&value, 32, true);
    value = (DWORD)attachedObjectSlot;
    stream.WriteBits((const BYTE*)&value, 32, true);
    stream.Write((const char*)info, 52);
    (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_objectEditAttachedRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

// 0x00072660
void CObjectEdit::Disable(BOOL send)
{
    BOOL notify = FALSE;
    if (send)
        notify = TRUE;
    editing = 0;
    g_pGame->SetCursorMode(0, TRUE);
    if (g_pInput->m_bEnabled)
    {
        g_pInput->Close();
        g_pInput->Open();
    }
    if (editTarget == 2)
        SendAttachedEditEndNotification(notify);
    if (editTarget == 1)
        SendEditEndNotification(notify);
}
