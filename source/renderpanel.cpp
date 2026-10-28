#include "renderpanel.h"
#include "fontrender.h"
#include "game/gta_globals.h"
#include "gui/device_lifecycle.h"
#include "net/pool_frame.h"
#include "objectedit.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <string.h>
// samp.dll RVA: 0x0011c75c
const char g_renderPanelTitle[10] = "Transform";
// samp.dll RVA: 0x00071100
CRenderPanel::CRenderPanel(IDirect3DDevice9* device)
{
    DWORD* state = (DWORD*)nativeState;
    state[0] = (DWORD)device;
    state[8] = 0;
    state[9] = 0;
    state[3] = 280;
    state[4] = 150;
    state[5] = 210;
    state[6] = 30;
    state[7] = 38;
}

// samp.dll RVA: 0x00071140
void CRenderPanel::ResetDialogControls(CDXUTDialog* dialog)
{
    DWORD* state = (DWORD*)nativeState;
    state[8] = (DWORD)dialog;
    if (!dialog)
        return;
    dialog->AddSlider(30, 50, 10, state[5], state[6], -100, 100, 0, false, 0);
    BYTE* slider = (BYTE*)((CDXUTDialog*)state[8])->GetControl(30, 5);
    DWORD color[4] = {0x3f19999a, 0x3f19999a, 0x3f4cccce, 0x3f800000};
    slider[0x76] = 1;
    memcpy(slider + 0x77, color, 16);
    int secondY = (LONG)(state[7] + 10);
    ((CDXUTDialog*)state[8])->AddSlider(32, 50, secondY, state[5], state[6], -100, 100, 0, false, 0);
    slider = (BYTE*)((CDXUTDialog*)state[8])->GetControl(32, 5);
    color[0] = 0x3f4cccce;
    color[1] = 0x3f19999a;
    color[2] = 0x3f19999a;
    slider[0x76] = 1;
    memcpy(slider + 0x77, color, 16);
    int thirdY = (LONG)(state[7] + (DWORD)secondY);
    ((CDXUTDialog*)state[8])->AddSlider(34, 50, thirdY, state[5], state[6], -100, 100, 0, false, 0);
    slider = (BYTE*)((CDXUTDialog*)state[8])->GetControl(34, 5);
    color[0] = 0x3f19999a;
    color[1] = 0x3f4cccce;
    slider[0x76] = 1;
    memcpy(slider + 0x77, color, 16);
}
// samp.dll RVA: 0x000712c0
void CRenderPanel::UpdateLayout()
{
    DWORD* state = (DWORD*)nativeState;
    BYTE* dialog = (BYTE*)state[8];
    DWORD height = state[4], width = state[3];
    *(DWORD*)(dialog + 0x11e) = width;
    *(DWORD*)(dialog + 0x122) = height;
    RECT rect;
    GetClientRect(g_inputGameWindow, &rect);
    state[1] = (DWORD)rect.right - state[3] - 5;
    state[2] = (DWORD)rect.bottom - state[4] - 10;
    dialog = (BYTE*)state[8];
    *(DWORD*)(dialog + 0x116) = state[1];
    *(DWORD*)(dialog + 0x11a) = state[2];
}
// samp.dll RVA: 0x00071320
void CRenderPanel::GetRect(RECT* out)
{
    DWORD* state = (DWORD*)nativeState;
    out->left = state[1];
    out->right = (LONG)(state[3] + (DWORD)out->left);
    out->top = state[2];
    out->bottom = (LONG)(state[4] + (DWORD)out->top);
}
// samp.dll RVA: 0x00071350
void CRenderPanel::Show()
{
    SIZE measured;
    g_deviceFonts->GetTextScreenSize((FontSize*)&measured, g_fontMeasureProbe, 0);
    DWORD* state = (DWORD*)nativeState;
    BYTE* dialog = (BYTE*)state[8];
    strcpy((char*)dialog + 0x16, g_renderPanelTitle);
    ((BYTE*)state[8])[0x14] = 1;
    *(DWORD*)((BYTE*)state[8] + 0x126) = (DWORD)measured.cy + 4;
    ((BYTE*)state[8])[0x13] = 1;
    UpdateLayout();
    state[9] = 1;
}
// samp.dll RVA: 0x000713c0
void CRenderPanel::Hide()
{
    DWORD* state = (DWORD*)nativeState;
    ((BYTE*)state[8])[0x13] = 0;
    state[9] = 0;
}
// samp.dll RVA: 0x000713d0
void CRenderPanel::ProcessControls()
{
    DWORD* state = (DWORD*)nativeState;
    if (!state[8])
        return;
    for (int axis = 0; axis < 3; axis++)
    {
        BYTE* slider = (BYTE*)((CDXUTDialog*)state[8])->GetControl(30 + axis * 2, 5);
        int value = *(int*)(slider + 0x4d);
        if (value)
        {
            float difference;
            // clang-format off
            __asm {
                fild value
                fmul g_objectPoolSecondsPerTick
                fstp difference
            }
            // clang-format on
            ((CObjectEdit*)g_deviceObjectEdit)->ApplyChanges(axis, difference);
        }
    }
}
// samp.dll RVA: 0x00071480
void CRenderPanel::DrawLabels()
{
    CObjectEdit* editor = (CObjectEdit*)g_deviceObjectEdit;
    ID3DXFont* font = editor->smallFont;
    if (!font)
        return;
    DWORD* state = (DWORD*)nativeState;
    LONG x = (LONG)(state[1] + 25), y = (LONG)(state[2] + *(DWORD*)((BYTE*)state[8] + 0x126) + 30);
    RECT rect = {x, y, (LONG)((DWORD)x + 1), (LONG)((DWORD)y + 1)};
    font->DrawTextA(0, editor->GetRenderChar(0), 1, &rect, 0x105, 0xff9999cc);
    rect.top = (LONG)((DWORD)rect.top + state[7]);
    rect.bottom = (LONG)((DWORD)rect.top + 1);
    font->DrawTextA(0, ((CObjectEdit*)g_deviceObjectEdit)->GetRenderChar(1), 1, &rect, 0x105, 0xffcc9999);
    rect.top = (LONG)((DWORD)rect.top + state[7]);
    rect.bottom = (LONG)((DWORD)rect.top + 1);
    font->DrawTextA(0, ((CObjectEdit*)g_deviceObjectEdit)->GetRenderChar(2), 1, &rect, 0x105, 0xff99cc99);
}
// samp.dll RVA: 0x00071560
void CRenderPanel::Draw()
{
    DWORD* state = (DWORD*)nativeState;
    if (!state[9])
        return;
    ProcessControls();
    CDXUTDialog* dialog = (CDXUTDialog*)state[8];
    if (dialog)
        dialog->OnRender(10.0f);
    DrawLabels();
}
