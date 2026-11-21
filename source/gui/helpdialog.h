#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <windows.h>
struct IDirect3DDevice9;
class CHelpDialog
{
public:
    BYTE nativeState[0x4];
    // R5 address to verify
    CHelpDialog(IDirect3DDevice9* device);
    // 0x0006bb30
    void Show();
};
// 0x000ea460
extern const char g_helpDialogTitle[12];
// 0x000ea470
extern const char g_helpDialogText[420];
// 0x000ea614
extern const char g_helpDialogButton[3];

#endif // HELPDIALOG_H
