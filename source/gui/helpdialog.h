#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <windows.h>
struct IDirect3DDevice9;
class CHelpDialog
{
public:
    BYTE nativeState[0x4];

    CHelpDialog(IDirect3DDevice9* device);

    void Show();
};

extern const char g_helpDialogTitle[12];

extern const char g_helpDialogText[420];

extern const char g_helpDialogButton[3];

#endif // HELPDIALOG_H
