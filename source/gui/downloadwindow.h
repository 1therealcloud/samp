#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include <windows.h>

#include <static_assert.h>

struct IDirect3DDevice9;
class CDXUTDialog;
#pragma pack(push, 1)
// Packed download record.
struct DownloadRecord
{
    DWORD row;
    BYTE type;
    DWORD id;
    BYTE status;
    DWORD downloaded, total;
};
STATIC_ASSERT(sizeof(DownloadRecord) == 18);
// The record storage is the first subobject of the window.
struct DownloadRecordStorage
{
    BYTE nativeState[0x260];

    DownloadRecordStorage()
    {
        *(DWORD*)nativeState = 0;
        *(DWORD*)(nativeState + 4) = 0;
    }
    ~DownloadRecordStorage();
    bool ResizeRecords(DWORD count);
};

class CDownloadWindow : public DownloadRecordStorage
{
public:
    // Initializes only the pointer/count prefix; does not free previous storage.
    // 0x0006a9d0
    CDownloadWindow* InitializeRecordStorage();
    // First two window DWORDs are a specialized pointer-array view.
    // R5 address to verify
    DownloadRecord* GetRecord(DWORD index);
    // R5 address to verify
    int GetFilesCount();
    // R5 address to verify
    // R5 address to verify
    void TrimRecords();
    // 0x0006abd0
    int FindRecord(DWORD id);
    // R5 address to verify
    void ResetRecordStorage();
    // 0x0006ac10
    int AddRecord(DownloadRecord* record);
    // 0x0006ac70
    bool RemoveRecord(DWORD index);
    // 0x0006aca0
    void ClearRecords();
    // 0x0006ace0
    void UpdateRecord(BYTE type, DWORD id, BYTE status, DWORD downloaded, DWORD total, int error, const char* detail);
    // 0x0006b0d0
    void Hide();
    // 0x0006afd0
    CDownloadWindow(IDirect3DDevice9* device);
    // Also lays out the dialog and list control.
    // 0x0006a760
    void ResetFonts();
    // 0x0006a840
    void GetRect(RECT* out);
    // 0x0006a870
    void Show(DWORD mode);
    // 0x0006a910
    void Draw();
    // 0x0006aac0
    void ResetDialogControls(CDXUTDialog* dialog);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CDownloadWindow) == 0x260);
// 0x000ea354
extern const char g_downloadWindowTitle[18];
// 0x000ea3ec
extern const char g_downloadModel[6];
// 0x000ea3e4
extern const char g_downloadTexture[8];
// 0x000ea3dc
extern const char g_downloadIdFormat[5];
// 0x000ea3d8
extern const char g_downloadZeroFormat[4];
// 0x000ea3d0
extern const char g_downloadPercentFormat[7];
// 0x000ea3b8
extern const char g_downloadDetailFormat[21];
// 0x000ea3a0
extern const char g_downloadErrorFormat[24];
// 0x000ea390
extern const char g_downloadCompleteFormat[15];
// 0x000ea368
extern const char g_downloadCaptionFormat[38];
// 0x000e98e0
extern const float g_downloadKilobytesPerByte;

#endif // DOWNLOADWINDOW_H
