#ifndef SAMP_DOWNLOAD_WINDOW_H
#define SAMP_DOWNLOAD_WINDOW_H
#include <windows.h>
struct IDirect3DDevice9;
class CDXUTDialog;
#pragma pack(push, 1)
// Packed native per-download record; semantic names are descriptive.
struct DownloadRecord
{
    DWORD row;
    BYTE type;
    DWORD id;
    BYTE status;
    DWORD downloaded, total;
};
typedef char NativeDownloadRecordExtent[(sizeof(DownloadRecord) == 18) ? 1 : -1];
class CDownloadWindow
{
public:
    // Initializes only the pointer/count prefix; does not free previous storage.
    // samp.dll RVA: 0x0006a410
    CDownloadWindow* InitializeRecordStorage();
    // Separate prefix-release entry; preserves ResizeRecords' result in AL.
    // samp.dll RVA: 0x0006aa00
    bool ReleaseRecordStorage();
    BYTE nativeState[0x260];
    // First two window DWORDs are a specialized pointer-array view.
    // Descriptive helpers retain its observed allocation behavior.
    // samp.dll RVA: 0x0006a420
    DownloadRecord* GetRecord(DWORD index);
    // samp.dll RVA: 0x0006a440
    int GetFilesCount();
    // samp.dll RVA: 0x0006a450
    bool ResizeRecords(DWORD count);
    // samp.dll RVA: 0x0006a4c0
    void TrimRecords();
    // samp.dll RVA: 0x0006a610
    int FindRecord(DWORD id);
    // samp.dll RVA: 0x0006a640
    void ResetRecordStorage();
    // samp.dll RVA: 0x0006a650
    int AddRecord(DownloadRecord* record);
    // samp.dll RVA: 0x0006a6b0
    bool RemoveRecord(DWORD index);
    // samp.dll RVA: 0x0006a6e0
    void ClearRecords();
    // samp.dll RVA: 0x0006a720
    void UpdateRecord(BYTE type, DWORD id, BYTE status, DWORD downloaded, DWORD total, int error, const char* detail);
    // samp.dll RVA: 0x0006ab10
    void Hide();
    // samp.dll RVA: 0x0006aa10
    CDownloadWindow(IDirect3DDevice9* device);
    // Native name retained; also lays out the dialog and list control.
    // samp.dll RVA: 0x0006a1a0
    void ResetFonts();
    // samp.dll RVA: 0x0006a280
    void GetRect(RECT* out);
    // Descriptive name; parameter is copied verbatim to native state +250.
    // samp.dll RVA: 0x0006a2b0
    void Show(DWORD mode);
    // samp.dll RVA: 0x0006a350
    void Draw();
    // samp.dll RVA: 0x0006a500
    void ResetDialogControls(CDXUTDialog* dialog);
};
#pragma pack(pop)
typedef char NativeDownloadWindowExtent[(sizeof(CDownloadWindow) == 0x260) ? 1 : -1];
// samp.dll RVA: 0x0011c37c
extern const char g_downloadWindowTitle[18];
// samp.dll RVA: 0x0011c414
extern const char g_downloadModel[6];
// samp.dll RVA: 0x0011c40c
extern const char g_downloadTexture[8];
// samp.dll RVA: 0x0011c404
extern const char g_downloadIdFormat[5];
// samp.dll RVA: 0x0011c400
extern const char g_downloadZeroFormat[4];
// samp.dll RVA: 0x0011c3f8
extern const char g_downloadPercentFormat[7];
// samp.dll RVA: 0x0011c3e0
extern const char g_downloadDetailFormat[21];
// samp.dll RVA: 0x0011c3c8
extern const char g_downloadErrorFormat[24];
// samp.dll RVA: 0x0011c3b8
extern const char g_downloadCompleteFormat[15];
// samp.dll RVA: 0x0011c390
extern const char g_downloadCaptionFormat[38];
// samp.dll RVA: 0x0011b918
extern const float g_downloadKilobytesPerByte;
#endif
