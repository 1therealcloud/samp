#ifndef SAMP_RENDER_PANEL_H
#define SAMP_RENDER_PANEL_H
#include <windows.h>
struct IDirect3DDevice9;
class CDXUTDialog;
#pragma pack(push, 1)
class CRenderPanel
{
public:
    BYTE nativeState[0x28];
    // 0x00071100
    CRenderPanel(IDirect3DDevice9* device);
    // 0x00071140
    void ResetDialogControls(CDXUTDialog* dialog);
    // Descriptive helper names established from native behavior.
    // 0x000712c0
    void UpdateLayout();
    // 0x00071320
    void GetRect(RECT* out);
    // 0x00071350
    void Show();
    // 0x000713c0
    void Hide();
    // 0x000713d0
    void ProcessControls();
    // 0x00071480
    void DrawLabels();
    // 0x00071560
    void Draw();
};
#pragma pack(pop)
typedef char NativeRenderPanelExtent[(sizeof(CRenderPanel) == 0x28) ? 1 : -1];
// 0x0011c75c
extern const char g_renderPanelTitle[10];
#endif
