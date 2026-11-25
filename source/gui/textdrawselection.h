#ifndef TEXTDRAWSELECTION_H
#define TEXTDRAWSELECTION_H

#include <windows.h>

#pragma pack(push, 1)
class CTextDrawSelection
{
public:

    CTextDrawSelection();
    DWORD enabled, hoverColor;
    WORD hoveredTextDrawId;

    void Enable(DWORD color);

    void UpdateHovered();

    void SendNotification();

    void ResetTextDraws();

    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);

    void Disable();

    void Process();
};
#pragma pack(pop)
static_assert(sizeof(CTextDrawSelection) == 10);

extern int g_textDrawClickRpc;

#endif // TEXTDRAWSELECTION_H
