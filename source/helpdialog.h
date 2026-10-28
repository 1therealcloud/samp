#ifndef SAMP_HELPDIALOG_H
#define SAMP_HELPDIALOG_H
#include <windows.h>
struct IDirect3DDevice9;
class CHelpDialog
{
public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x4];
    // samp.dll RVA: 0x0006b560
    CHelpDialog(IDirect3DDevice9* device);
    // samp.dll RVA: 0x0006b570
    void Show();
};
// samp.dll RVA: 0x0011c488
extern const char g_helpDialogTitle[12];
// samp.dll RVA: 0x0011c498
extern const char g_helpDialogText[420];
// samp.dll RVA: 0x0011c63c
extern const char g_helpDialogButton[3];
#endif
