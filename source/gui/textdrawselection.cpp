#include "gui/textdrawselection.h"

#include "gui/cmdwindow.h"
#include "main.h"

#include <BitStream.h>
#include <RakClientInterface.h>

int g_textDrawClickRpc = 83;

CTextDrawSelection::CTextDrawSelection()
{
    enabled = 0;
    hoveredTextDrawId = 0xffff;
    hoverColor = 0xffffffff;
}

BOOL CTextDrawSelection::MsgProc(UINT message, WPARAM, LPARAM)
{
    if (enabled && message == WM_LBUTTONUP)
    {
        if (hoveredTextDrawId != 0xffff)
            SendNotification();
        return TRUE;
    }
    return FALSE;
}

void CTextDrawSelection::Disable()
{
    enabled = 0;
    g_pGame->SetCursorMode(0, FALSE);
    if (g_pInput->m_bEnabled)
    {
        g_pInput->Close();
        g_pInput->Open();
    }
    hoveredTextDrawId = 0xffff;
    SendNotification();
    ResetTextDraws();
}

void CTextDrawSelection::Enable(DWORD color)
{
    enabled = 1;
    hoverColor = ((color & 0xff) << 24) | ((color & 0xff00) << 8) | ((color & 0xff0000) >> 8) | ((color & 0xff000000) >> 24);
    hoveredTextDrawId = 0xffff;
}

void CTextDrawSelection::ResetTextDraws()
{
    if (!g_pNetGame)
        return;
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    BYTE* pool = *(BYTE**)(pools + 0x20);
    if (!pool)
        return;
    for (int i = 0; i != 2304; ++i)
    {
        // The native loop narrows the index before the pool lookup.
        WORD id = (WORD)i;
        if (id < 2304 && *(DWORD*)(pool + id * 4))
        {
            BYTE* draw = *(BYTE**)(pool + 0x2400 + id * 4);
            if (!draw)
                continue;
            draw[0x9d1] = 0;
            *(DWORD*)(draw + 0x9d2) = 0;
        }
    }
}

void CTextDrawSelection::UpdateHovered()
{
    // GTA SA VA: 0x00c97c1c (game window handle, outside samp.dll).
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(*(HWND*)0xc97c1c, &point);
    if (!g_pNetGame)
        return;
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    BYTE* pool = *(BYTE**)(pools + 0x20);
    if (!pool)
        return;
    int i = 0;
    hoveredTextDrawId = 0xffff;
    for (; i != 2304; ++i)
    {
        WORD id = (WORD)i;
        if (id < 2304 && *(DWORD*)(pool + id * 4))
        {
            BYTE* draw = *(BYTE**)(pool + 0x2400 + id * 4);
            if (!draw)
                continue;
            draw[0x9d1] = 0;
            *(DWORD*)(draw + 0x9d2) = 0;
            draw = *(DWORD*)(pool + id * 4) == 0 ? 0 : *(BYTE**)(pool + 0x2400 + id * 4);
            if (!draw[0x9a7] || !draw[0x9bf])
                continue;
            RECT* rect = (RECT*)(draw + 0x9c1);
            if (!rect)
                continue;
            if (!PtInRect(rect, point))
                continue;
            hoveredTextDrawId = i;
            draw = *(DWORD*)(pool + id * 4) == 0 ? 0 : *(BYTE**)(pool + 0x2400 + id * 4);
            DWORD color = hoverColor;
            draw[0x9d1] = 1;
            *(DWORD*)(draw + 0x9d2) = color;
        }
    }
}

void CTextDrawSelection::Process()
{
    if (!enabled)
        return;
    g_pGame->SetCursorMode(2, FALSE);
    g_pGame->EnableHUD(FALSE);
    UpdateHovered();
}

void CTextDrawSelection::SendNotification()
{
    RakNet::BitStream stream;
    stream.Write(hoveredTextDrawId);
    (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_textDrawClickRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}
