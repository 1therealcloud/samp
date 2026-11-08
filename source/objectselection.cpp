#include <stdio.h>
#include <string.h>

#include "objectselection.h"

#include "BitStream.h"
#include "RakClientInterface.h"
#include "cmdwindow.h"
#include "game/playerped.h"
#include "gui.h"
#include "main.h"
#include "net/objectpool.h"
#include "objectedit.h"

// 0x0011c6b0
const float g_objectSelectionMaxDistance = 150.0f;
// 0x0011c68c
const char g_objectSelectionLabelFormat[36] = "[id: %d, type: %d]\nDistance: %.2fm\n";
// 0x00117f28
int g_objectSelectionRpc = 27;

// 0x0006d440
CObjectSelection::CObjectSelection()
{
    enabled = 0;
    objectId = 0xffff;
}

// 0x0006d450
WORD CObjectSelection::DefineObject()
{
    if (!g_pNetGame)
        return 0xffff;
    CVector origin, end;
    ObjectEditBuildCursorRay(150.0f, &origin, &end);
    // Reload the network/pool globals after the cursor-ray helper.
    CObjectPool* pool = *(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    return pool->FindInRay(150.0f, &origin, &end);
}

// 0x0006d4a0
void CObjectSelection::DrawLabels()
{
    if (!g_pNetGame)
        return;
    BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    WORD id = objectId;
    // Preserve the native inclusive2100 guard; adjacent layout is unproven.
    if (id > 2100 || !*(DWORD*)(pool + 4 + id * 4))
        return;
    CObject* object = *(DWORD*)(pool + 4 + id * 4) ? *(CObject**)(pool + 0x20d4 + id * 4) : 0;
    if (!object)
        return;
    BYTE visible;
    // clang-format off
    __asm {
        mov ecx,object
        call CEntity::GetDistanceToCamera
        fcomp g_objectSelectionMaxDistance
        fnstsw ax
        test ah,41h
        setnp visible
    }
    // clang-format on
    if (!visible) return;
    CMatrix matrix;
    object->GetMatrix(&matrix);
    CVector position;
    memcpy(&position, &matrix.position, 12);
    ((CObjectEdit*)g_deviceObjectEdit)->RenderAxes(&matrix, 1.0f);
    // The second distance is promoted directly from ST0 to a double for sprintf.
    double distance;
    // clang-format off
    __asm {
        mov ecx,object
        call CEntity::GetDistanceToCamera
        fstp distance
    }
    // clang-format on
    int model = object->GetModelIndex();
    char text[256];
    sprintf(text, g_objectSelectionLabelFormat, (unsigned)objectId, model, distance);
    g_deviceLabel->Begin();
    g_deviceLabel->Draw(&position, text, 0xffd47b75, TRUE, FALSE);
    g_deviceLabel->End();
}

// 0x0006d5c0
void CObjectSelection::Enable(BOOL on)
{
    if (enabled)
    {
        if (!on)
        {
            g_pGame->SetCursorMode(0, TRUE);
            if (g_pInput->m_bEnabled)
            {
                g_pInput->Close();
                g_pInput->Open();
            }
        }
        if (enabled)
        {
            enabled = on;
            return;
        }
    }
    if (on)
    {
        g_pGame->SetCursorMode(0, TRUE);
        g_pGame->m_nInputEnableWaitFrames = 0;
        g_pGame->ProcessInputEnabling();
    }
    enabled = on;
}

// 0x0006d640
void CObjectSelection::Draw()
{
    if (!enabled)
        return;
    WORD leftRight, upDown;
    WORD keys = g_pGame->GetPlayerPed()->GetKeys(&leftRight, &upDown);
    if (keys & 8)
        g_pGame->SetCursorMode(0, TRUE);
    else
    {
        g_gameD3DDevice->ShowCursor(TRUE);
        g_pGame->SetCursorMode(3, FALSE);
    }
    objectId = 0xffff;
    if (!g_pGame->m_nCursorMode)
        return;
    WORD id = DefineObject();
    if (id == 0xffff)
        return;
    CObjectPool* pool = *(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    if (id > 2100 || !*(DWORD*)((BYTE*)pool + 4 + id * 4))
        return;
    CEntity* object = pool->Get(id);
    if (!object)
        return;
    BYTE visible;
    // clang-format off
    __asm {
        mov ecx,object
        call CEntity::GetDistanceToCamera
        fcomp g_objectSelectionMaxDistance
        fnstsw ax
        test ah,41h
        setnp visible
    }
    // clang-format on
    if (visible)
    {
        objectId = id;
        DrawLabels();
    }
}

// 0x0006d710
void CObjectSelection::SendNotification()
{
    BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    WORD id = objectId;
    if (id > 2100 || !*(DWORD*)(pool + 4 + id * 4))
        return;
    CObject* object = *(CObject**)(pool + 0x20d4 + id * 4);
    if (!object)
        return;
    CMatrix matrix;
    object->GetMatrix(&matrix);
    RakNet::BitStream stream;
    DWORD value = 0;
    stream.WriteBits((const BYTE*)&value, 32, true);
    value = objectId;
    stream.WriteBits((const BYTE*)&value, 16, true);
    value = object->GetModelIndex();
    stream.WriteBits((const BYTE*)&value, 32, true);
    memcpy(&value, &matrix.position.x, 4);
    stream.WriteBits((const BYTE*)&value, 32, true);
    memcpy(&value, &matrix.position.y, 4);
    stream.WriteBits((const BYTE*)&value, 32, true);
    memcpy(&value, &matrix.position.z, 4);
    stream.WriteBits((const BYTE*)&value, 32, true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_objectSelectionRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

// 0x0006d880
BOOL CObjectSelection::MsgProc(UINT message, WPARAM, LPARAM)
{
    if (enabled && message == WM_LBUTTONUP)
    {
        SendNotification();
        return TRUE;
    }
    return FALSE;
}
