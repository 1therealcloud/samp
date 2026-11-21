#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include <windows.h>

#include <static_assert.h>

struct IDirect3DDevice9;
class CDXUTDialog;
#pragma pack(push, 1)
class CRenderPanel
{
public:
    BYTE nativeState[0x28];
    // 0x00071660
    CRenderPanel(IDirect3DDevice9* device);
    // 0x000716a0
    void ResetDialogControls(CDXUTDialog* dialog);
    // 0x00071820
    void UpdateLayout();
    // R5 address to verify
    void GetRect(RECT* out);
    // 0x000718b0
    void Show();
    // R5 address to verify
    void Hide();
    // 0x00071930
    void ProcessControls();
    // 0x000719e0
    void DrawLabels();
    // 0x00071ac0
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CRenderPanel) == 0x28);
// 0x000ea734
extern const char g_renderPanelTitle[10];

#endif // RENDERPANEL_H
