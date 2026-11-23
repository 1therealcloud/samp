#include "stdafx.h"

#include "launch3Dlg.h"
#include "resource.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    return static_cast<int>(DialogBoxParamA(instance, MAKEINTRESOURCEA(IDD_LAUNCH3_DIALOG), nullptr, LaunchDialogProc, reinterpret_cast<LPARAM>(instance)));
}
