#ifndef SAMP_HELPDIALOG_H
#define SAMP_HELPDIALOG_H
#include <windows.h>
struct IDirect3DDevice9;
class CHelpDialog
{
public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x4];
    // 0x0006b560
    CHelpDialog(IDirect3DDevice9* device);
    // 0x0006b570
    void Show();
};
// 0x0011c488
extern const char g_helpDialogTitle[12];
// 0x0011c498
extern const char g_helpDialogText[420];
// 0x0011c63c
extern const char g_helpDialogButton[3];
#endif
