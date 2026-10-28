#ifndef SAMP_TEXTDRAW_SELECTION_H
#define SAMP_TEXTDRAW_SELECTION_H
#include <windows.h>
#pragma pack(push, 1)
class CTextDrawSelection
{
public:
    // 0x000c5120
    CTextDrawSelection();
    DWORD enabled, unknown04;
    WORD unknown08;
    // 0x00070ee0
    void Enable(DWORD color);
    // 0x00070db0
    void UpdateHovered();
    // 0x00070f20
    void SendNotification();
    // 0x00070d50
    void ResetTextDraws();
    // 0x00071010
    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x00070fc0
    void Disable();
    // 0x00070eb0
    void Process();
};
#pragma pack(pop)
typedef char NativeTextDrawSelectionSize[(sizeof(CTextDrawSelection) == 10) ? 1 : -1];
// 0x001182f8
extern int g_textDrawClickRpc;
#endif
