#include <string.h>

#include <DXUT.h>
#include <DXUTgui.h>

#include "gui/renderpanel.h"

#include "render/fontrender.h"
#include "game/gta_globals.h"
#include "gui/gui.h"
#include "net/objectpool.h"
#include "gui/objectedit.h"
// 0x0011c75c
const char g_renderPanelTitle[10] = "Transform";
// 0x00071100
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

// 0x000710d0
void CDXUTSlider::SetColorOverride(D3DXCOLOR color)
{
    BYTE* slider = (BYTE*)this;
    slider[0x76] = 1;
    memcpy(slider + 0x77, &color, sizeof(color));
}

// 0x00071140
void CRenderPanel::ResetDialogControls(CDXUTDialog* dialog)
{
    DWORD* state = (DWORD*)nativeState;
    state[8] = (DWORD)dialog;
    if (!dialog)
        return;
    const float low = 0.6f;
    const float high = 0.80000007f;
    dialog->AddSlider(30, 50, 10, state[5], state[6], -100, 100, 0, false, 0);
    ((CDXUTDialog*)state[8])->GetSlider(30)->SetColorOverride(D3DXCOLOR(low, low, high, 1.0f));
    int secondY = (LONG)(state[7] + 10);
    ((CDXUTDialog*)state[8])->AddSlider(32, 50, secondY, state[5], state[6], -100, 100, 0, false, 0);
    ((CDXUTDialog*)state[8])->GetSlider(32)->SetColorOverride(D3DXCOLOR(high, low, low, 1.0f));
    int thirdY = (LONG)(state[7] + (DWORD)secondY);
    ((CDXUTDialog*)state[8])->AddSlider(34, 50, thirdY, state[5], state[6], -100, 100, 0, false, 0);
    ((CDXUTDialog*)state[8])->GetSlider(34)->SetColorOverride(D3DXCOLOR(low, high, low, 1.0f));
}
// 0x000712c0
void CRenderPanel::UpdateLayout()
{
    DWORD* state = (DWORD*)nativeState;
    ((CDXUTDialog*)state[8])->SetSize(state[3], state[4]);
    RECT rect;
    GetClientRect(g_inputGameWindow, &rect);
    LONG x = state[1] = rect.right - (LONG)state[3] - 5;
    LONG y = state[2] = rect.bottom - (LONG)state[4] - 10;
    ((CDXUTDialog*)state[8])->SetLocation(x, y);
}
// 0x00071320
void CRenderPanel::GetRect(RECT* out)
{
    DWORD* state = (DWORD*)nativeState;
    out->left = state[1];
    out->right = (LONG)(state[3] + (DWORD)out->left);
    out->top = state[2];
    out->bottom = (LONG)(state[4] + (DWORD)out->top);
}
// 0x00071350
void CRenderPanel::Show()
{
    FontSize measured = g_deviceFonts->GetTextScreenSize(g_fontMeasureProbe, 0);
    DWORD* state = (DWORD*)nativeState;
    BYTE* dialog = (BYTE*)state[8];
    strcpy((char*)dialog + 0x16, g_renderPanelTitle);
    ((BYTE*)state[8])[0x14] = 1;
    *(DWORD*)((BYTE*)state[8] + 0x126) = (DWORD)measured.cy + 4;
    ((BYTE*)state[8])[0x13] = 1;
    UpdateLayout();
    state[9] = 1;
}
// 0x000713c0
void CRenderPanel::Hide()
{
    DWORD* state = (DWORD*)nativeState;
    ((BYTE*)state[8])[0x13] = 0;
    state[9] = 0;
}
// 0x000713d0
void CRenderPanel::ProcessControls()
{
    DWORD* state = (DWORD*)nativeState;
    if (!state[8])
        return;
    int value;
    value = *(int*)((BYTE*)((CDXUTDialog*)state[8])->GetControl(30, 5) + 0x4d);
    if (value)
        ((CObjectEdit*)g_deviceObjectEdit)->ApplyChanges(0, value * g_objectPoolSecondsPerTick);
    value = *(int*)((BYTE*)((CDXUTDialog*)state[8])->GetControl(32, 5) + 0x4d);
    if (value)
        ((CObjectEdit*)g_deviceObjectEdit)->ApplyChanges(1, value * g_objectPoolSecondsPerTick);
    value = *(int*)((BYTE*)((CDXUTDialog*)state[8])->GetControl(34, 5) + 0x4d);
    if (value)
        ((CObjectEdit*)g_deviceObjectEdit)->ApplyChanges(2, value * g_objectPoolSecondsPerTick);
}
// 0x00071480
void CRenderPanel::DrawLabels()
{
    CObjectEdit* editor = (CObjectEdit*)g_deviceObjectEdit;
    ID3DXFont* font = editor->smallFont;
    if (!font)
        return;
    DWORD* state = (DWORD*)nativeState;
    RECT rect;
    rect.left = (LONG)(state[1] + 25);
    rect.top = (LONG)(state[2] + *(DWORD*)((BYTE*)state[8] + 0x126) + 30);
    rect.right = (LONG)((DWORD)rect.left + 1);
    rect.bottom = (LONG)((DWORD)rect.top + 1);
    font->DrawTextA(0, editor->GetRenderChar(0), 1, &rect, 0x105, 0xff9999cc);
    rect.top = (LONG)((DWORD)rect.top + state[7]);
    rect.bottom = (LONG)((DWORD)rect.top + 1);
    font->DrawTextA(0, ((CObjectEdit*)g_deviceObjectEdit)->GetRenderChar(1), 1, &rect, 0x105, 0xffcc9999);
    rect.top = (LONG)((DWORD)rect.top + state[7]);
    rect.bottom = (LONG)((DWORD)rect.top + 1);
    font->DrawTextA(0, ((CObjectEdit*)g_deviceObjectEdit)->GetRenderChar(2), 1, &rect, 0x105, 0xff99cc99);
}
// 0x00071560
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
