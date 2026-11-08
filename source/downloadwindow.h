#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include <windows.h>

#include <static_assert.h>
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
STATIC_ASSERT(sizeof(DownloadRecord) == 18);
class CDownloadWindow
{
public:
    // Initializes only the pointer/count prefix; does not free previous storage.
    // 0x0006a410
    CDownloadWindow* InitializeRecordStorage();
    // Separate prefix-release entry; preserves ResizeRecords' result in AL.
    // 0x0006aa00
    bool ReleaseRecordStorage();
    BYTE nativeState[0x260];
    // First two window DWORDs are a specialized pointer-array view.
    // Descriptive helpers retain its observed allocation behavior.
    // 0x0006a420
    DownloadRecord* GetRecord(DWORD index);
    // 0x0006a440
    int GetFilesCount();
    // 0x0006a450
    bool ResizeRecords(DWORD count);
    // 0x0006a4c0
    void TrimRecords();
    // 0x0006a610
    int FindRecord(DWORD id);
    // 0x0006a640
    void ResetRecordStorage();
    // 0x0006a650
    int AddRecord(DownloadRecord* record);
    // 0x0006a6b0
    bool RemoveRecord(DWORD index);
    // 0x0006a6e0
    void ClearRecords();
    // 0x0006a720
    void UpdateRecord(BYTE type, DWORD id, BYTE status, DWORD downloaded, DWORD total, int error, const char* detail);
    // 0x0006ab10
    void Hide();
    // 0x0006aa10
    CDownloadWindow(IDirect3DDevice9* device);
    // Native name retained; also lays out the dialog and list control.
    // 0x0006a1a0
    void ResetFonts();
    // 0x0006a280
    void GetRect(RECT* out);
    // Descriptive name; parameter is copied verbatim to native state +250.
    // 0x0006a2b0
    void Show(DWORD mode);
    // 0x0006a350
    void Draw();
    // 0x0006a500
    void ResetDialogControls(CDXUTDialog* dialog);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CDownloadWindow) == 0x260);
// 0x0011c37c
extern const char g_downloadWindowTitle[18];
// 0x0011c414
extern const char g_downloadModel[6];
// 0x0011c40c
extern const char g_downloadTexture[8];
// 0x0011c404
extern const char g_downloadIdFormat[5];
// 0x0011c400
extern const char g_downloadZeroFormat[4];
// 0x0011c3f8
extern const char g_downloadPercentFormat[7];
// 0x0011c3e0
extern const char g_downloadDetailFormat[21];
// 0x0011c3c8
extern const char g_downloadErrorFormat[24];
// 0x0011c3b8
extern const char g_downloadCompleteFormat[15];
// 0x0011c390
extern const char g_downloadCaptionFormat[38];
// 0x0011b918
extern const float g_downloadKilobytesPerByte;

#endif // DOWNLOADWINDOW_H
