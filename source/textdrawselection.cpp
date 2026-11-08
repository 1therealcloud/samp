#include "textdrawselection.h"
#include "cmdwindow.h"
#include "main.h"
// 0x000c5120
CTextDrawSelection::CTextDrawSelection()
{
    enabled = 0;
    unknown08 = 0xffff;
    unknown04 = 0xffffffff;
}
// 0x00071010
BOOL CTextDrawSelection::MsgProc(UINT message, WPARAM, LPARAM)
{
    if (enabled && message == WM_LBUTTONUP)
    {
        if (unknown08 != 0xffff)
            SendNotification();
        return TRUE;
    }
    return FALSE;
}
// 0x00070fc0
void CTextDrawSelection::Disable()
{
    enabled = 0;
    g_pGame->SetCursorMode(0, FALSE);
    if (g_pInput->m_bEnabled)
    {
        g_pInput->Close();
        g_pInput->Open();
    }
    unknown08 = 0xffff;
    SendNotification();
    ResetTextDraws();
}

// 0x00070ee0
void CTextDrawSelection::Enable(DWORD color)
{
    enabled = 1;
    unknown04 = ((color & 0xff) << 24) | ((color & 0xff00) << 8) | ((color & 0xff0000) >> 8) | ((color & 0xff000000) >> 24);
    unknown08 = 0xffff;
}
// 0x00070d50
void CTextDrawSelection::ResetTextDraws()
{
    if (!g_pNetGame)
        return;
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    BYTE* pool = *(BYTE**)(pools + 0x20);
    if (!pool)
        return;
    for (WORD i = 0; i < 2304; ++i)
        if (*(DWORD*)(pool + i * 4))
        {
            BYTE* draw = *(BYTE**)(pool + 0x2400 + i * 4);
            if (!draw)
                continue;
            draw[0x9d1] = 0;
            *(DWORD*)(draw + 0x9d2) = 0;
        }
}
// 0x00070db0
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
    unknown08 = 0xffff;
    for (WORD i = 0; i < 2304; ++i)
        if (*(DWORD*)(pool + i * 4))
        {
            BYTE* draw = *(BYTE**)(pool + 0x2400 + i * 4);
            if (!draw)
                continue;
            draw[0x9d1] = 0;
            *(DWORD*)(draw + 0x9d2) = 0;
            draw = *(DWORD*)(pool + i * 4) ? *(BYTE**)(pool + 0x2400 + i * 4) : 0;
            if (!draw[0x9a7] || !draw[0x9bf])
                continue;
            RECT* rect = (RECT*)(draw + 0x9c1);
            if (!rect)
                continue;
            if (!PtInRect(rect, point))
                continue;
            unknown08 = i;
            draw = *(DWORD*)(pool + i * 4) ? *(BYTE**)(pool + 0x2400 + i * 4) : 0;
            draw[0x9d1] = 1;
            *(DWORD*)(draw + 0x9d2) = unknown04;
        }
}
// 0x00070eb0
void CTextDrawSelection::Process()
{
    if (!enabled)
        return;
    g_pGame->SetCursorMode(2, FALSE);
    g_pGame->EnableHUD(FALSE);
    UpdateHovered();
}

#include "BitStream.h"
#include "RakClientInterface.h"
// 0x001182f8
int g_textDrawClickRpc = 83;
// 0x00070f20
void CTextDrawSelection::SendNotification()
{
    RakNet::BitStream stream;
    DWORD id = unknown08;
    stream.WriteBits((const BYTE*)&id, 16, true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_textDrawClickRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}
