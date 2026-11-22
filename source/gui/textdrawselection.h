#ifndef TEXTDRAWSELECTION_H
#define TEXTDRAWSELECTION_H

#include <windows.h>

#include <static_assert.h>

#pragma pack(push, 1)
class CTextDrawSelection
{
public:
    // R5 address to verify
    CTextDrawSelection();
    DWORD enabled, hoverColor;
    WORD hoveredTextDrawId;
    // 0x00071440
    void Enable(DWORD color);
    // 0x00071310
    void UpdateHovered();
    // 0x00071480
    void SendNotification();
    // R5 address to verify
    void ResetTextDraws();
    // 0x00071570
    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x00071520
    void Disable();
    // 0x00071410
    void Process();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CTextDrawSelection) == 10);
// 0x000e6268
extern int g_textDrawClickRpc;

#endif // TEXTDRAWSELECTION_H
