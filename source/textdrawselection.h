#ifndef SAMP_TEXTDRAW_SELECTION_H
#define SAMP_TEXTDRAW_SELECTION_H
#include <windows.h>
#pragma pack(push, 1)
class CTextDrawSelection
{
public:
    // samp.dll RVA: 0x000c5120
    CTextDrawSelection();
    DWORD enabled, unknown04;
    WORD unknown08;
    // samp.dll RVA: 0x00070ee0
    void Enable(DWORD color);
    // samp.dll RVA: 0x00070db0
    void UpdateHovered();
    // samp.dll RVA: 0x00070f20
    void SendNotification();
    // samp.dll RVA: 0x00070d50
    void ResetTextDraws();
    // samp.dll RVA: 0x00071010
    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // samp.dll RVA: 0x00070fc0
    void Disable();
    // samp.dll RVA: 0x00070eb0
    void Process();
};
#pragma pack(pop)
typedef char NativeTextDrawSelectionSize[(sizeof(CTextDrawSelection) == 10) ? 1 : -1];
// samp.dll RVA: 0x001182f8
extern int g_textDrawClickRpc;
#endif
