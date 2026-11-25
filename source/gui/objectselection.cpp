#include "gui/objectselection.h"

#include "game/playerped.h"
#include "gui/cmdwindow.h"
#include "gui/gui.h"
#include "gui/objectedit.h"
#include "main.h"
#include "net/objectpool.h"

#include <BitStream.h>
#include <RakClientInterface.h>

#include <stdio.h>
#include <string.h>

const float g_objectSelectionMaxDistance = 150.0f;

const char g_objectSelectionLabelFormat[36] = "[id: %d, type: %d]\nDistance: %.2fm\n";

int g_objectSelectionRpc = 27;

CObjectSelection::CObjectSelection()
{
    enabled = 0;
    objectId = 0xffff;
}

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

void CObjectSelection::DrawLabels()
{
    if (!g_pNetGame)
        return;
    BYTE* pool = *(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
    WORD id = objectId;
    // The original also accepts object ID 2100.
    if (id > 2100 || !*(DWORD*)(pool + 4 + id * 4))
        return;
    CObject* object = *(DWORD*)(pool + 4 + id * 4) ? *(CObject**)(pool + 0x20d4 + id * 4) : 0;
    if (!object)
        return;
    if (!(object->GetDistanceToCamera() <= g_objectSelectionMaxDistance))
        return;
    CMatrix matrix;
    object->GetMatrix(&matrix);
    CVector position;
    memcpy(&position, &matrix.position, 12);
    ((CObjectEdit*)g_deviceObjectEdit)->RenderAxes(&matrix, 1.0f);
    const double distance = object->GetDistanceToCamera();
    int model = object->GetModelIndex();
    char text[256];
    snprintf(text, sizeof(text), g_objectSelectionLabelFormat, (unsigned)objectId, model, distance);
    g_deviceLabel->Begin();
    g_deviceLabel->Draw(&position, text, 0xffd47b75, TRUE, FALSE);
    g_deviceLabel->End();
}

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
    if (object->GetDistanceToCamera() <= g_objectSelectionMaxDistance)
    {
        objectId = id;
        DrawLabels();
    }
}

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

BOOL CObjectSelection::MsgProc(UINT message, WPARAM, LPARAM)
{
    if (enabled && message == WM_LBUTTONUP)
    {
        SendNotification();
        return TRUE;
    }
    return FALSE;
}
