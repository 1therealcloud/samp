#include "helpdialog.h"
#include "game/scripting.h"
#include "gui/render_dependencies.h"

// 0x0011c488
const char g_helpDialogTitle[12] = "SA-MP Keys:";
// 0x0011c498
const char g_helpDialogText[420] =
    "{BB9999}F1\t{BBBBBB}Display this help dialog\n{BB9999}Tab\t{BBBBBB}Display the scoreboard\n{BB9999}F4\t{BBBBBB}Allows you to change class next time you "
    "respawn\n{BB9999}F5\t{BBBBBB}Show bandwidth statistics\n{BB9999}F7\t{BBBBBB}Toggle the chat box\n{BB9999}F8\t{BBBBBB}Take a screenshot\n{BB9999}F9\t{BBBBBB}Toggle the "
    "deathwindow\n{BB9999}T/F6\t{BBBBBB}Allows you to enter a chat message\n{BB9999}G\t{BBBBBB}Enter vehicle as passenger\n";
// 0x0011c63c
const char g_helpDialogButton[3] = "OK";

// 0x0006b560
CHelpDialog::CHelpDialog(IDirect3DDevice9* device)
{
    *(IDirect3DDevice9**)nativeState = device;
}

// 0x0006b570
void CHelpDialog::Show()
{
    if (!*(DWORD*)(g_renderDialog->nativeState + 0x28))
        g_renderDialog->Show(1, 0, g_helpDialogTitle, g_helpDialogText, g_helpDialogButton, g_pedEmptyDanceAnimation, FALSE);
}
