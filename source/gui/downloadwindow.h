#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include <windows.h>

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
static_assert(sizeof(DownloadRecord) == 18);
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

    CDownloadWindow* InitializeRecordStorage();
    // First two window DWORDs are a specialized pointer-array view.

    DownloadRecord* GetRecord(DWORD index);

    int GetFilesCount();


    void TrimRecords();

    int FindRecord(DWORD id);

    void ResetRecordStorage();

    int AddRecord(DownloadRecord* record);

    bool RemoveRecord(DWORD index);

    void ClearRecords();

    void UpdateRecord(BYTE type, DWORD id, BYTE status, DWORD downloaded, DWORD total, int error, const char* detail);

    void Hide();

    CDownloadWindow(IDirect3DDevice9* device);
    // Also lays out the dialog and list control.

    void ResetFonts();

    void GetRect(RECT* out);

    void Show(DWORD mode);

    void Draw();

    void ResetDialogControls(CDXUTDialog* dialog);
};
#pragma pack(pop)
static_assert(sizeof(CDownloadWindow) == 0x260);

extern const char g_downloadWindowTitle[18];

extern const char g_downloadModel[6];

extern const char g_downloadTexture[8];

extern const char g_downloadIdFormat[5];

extern const char g_downloadZeroFormat[4];

extern const char g_downloadPercentFormat[7];

extern const char g_downloadDetailFormat[21];

extern const char g_downloadErrorFormat[24];

extern const char g_downloadCompleteFormat[15];

extern const char g_downloadCaptionFormat[38];

extern const float g_downloadKilobytesPerByte;

#endif // DOWNLOADWINDOW_H
