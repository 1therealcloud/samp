#ifndef SAMP_DOWNLOAD_WINDOW_H
#define SAMP_DOWNLOAD_WINDOW_H
#include <windows.h>
struct IDirect3DDevice9;
class CDXUTDialog;
#pragma pack(push,1)
class CDownloadWindow {public:
    BYTE nativeState[0x260];
    // samp.dll RVA: 0x0006aa10
    CDownloadWindow(IDirect3DDevice9 *device);
    // Native name retained; also lays out the dialog and list control.
    // samp.dll RVA: 0x0006a1a0
    void ResetFonts();
    // samp.dll RVA: 0x0006a280
    void GetRect(RECT *out);
    // Descriptive name; parameter is copied verbatim to native state +250.
    // samp.dll RVA: 0x0006a2b0
    void Show(DWORD mode);
    // samp.dll RVA: 0x0006a350
    void Draw();
    // samp.dll RVA: 0x0006a500
    void ResetDialogControls(CDXUTDialog *dialog);
};
#pragma pack(pop)
typedef char NativeDownloadWindowExtent[(sizeof(CDownloadWindow)==0x260)?1:-1];
// samp.dll RVA: 0x0011c37c
extern const char g_downloadWindowTitle[18];
#endif
