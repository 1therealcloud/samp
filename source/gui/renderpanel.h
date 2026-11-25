#ifndef RENDERPANEL_H
#define RENDERPANEL_H

#include <windows.h>

struct IDirect3DDevice9;
class CDXUTDialog;
class CRenderPanel
{
public:
    BYTE nativeState[0x28];

    CRenderPanel(IDirect3DDevice9* device);

    void ResetDialogControls(CDXUTDialog* dialog);

    void UpdateLayout();

    void GetRect(RECT* out);

    void Show();

    void Hide();

    void ProcessControls();

    void DrawLabels();

    void Draw();
};
static_assert(sizeof(CRenderPanel) == 0x28);

extern const char g_renderPanelTitle[10];

#endif // RENDERPANEL_H
