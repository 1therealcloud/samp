#include "gui/downloadwindow.h"

#include "game/vehicle.h"
#include "gui/gui.h"
#include "gui/objectedit.h"
#include "render/fontrender.h"

#include <DXUTgui.h>
#include <GetTime.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 0x0011c37c
const char g_downloadWindowTitle[18] = "Updating models..";
// 0x0011c414
const char g_downloadModel[6] = "Model";
// 0x0011c40c
const char g_downloadTexture[8] = "Texture";
// 0x0011c404
const char g_downloadIdFormat[5] = "0x%X";
// 0x0011c400
const char g_downloadZeroFormat[4] = "0%%";
// 0x0011c3f8
const char g_downloadPercentFormat[7] = "%.0f%%";
// 0x0011c3e0
const char g_downloadDetailFormat[21] = "{CC8888}(%d) %.16s..";
// 0x0011c3c8
const char g_downloadErrorFormat[24] = "{CC8888}Error(%d) %d/%d";
// 0x0011c3b8
const char g_downloadCompleteFormat[15] = "{77AA77}%.0f%%";
// 0x0011c390
const char g_downloadCaptionFormat[38] = "Updating models.. [%d/%d] (%.2f KB/s)";
// 0x0011b918
const float g_downloadKilobytesPerByte = 0.0009765625f;

// 0x0006a410
CDownloadWindow* CDownloadWindow::InitializeRecordStorage()
{
    *(DWORD*)nativeState = 0;
    *(DWORD*)(nativeState + 4) = 0;
    return this;
}

// 0x0006aa00
DownloadRecordStorage::~DownloadRecordStorage()
{
    ResizeRecords(0);
}

// 0x0006a440
int CDownloadWindow::GetFilesCount()
{
    return *(int*)(nativeState + 4);
}

// 0x0006aa10
CDownloadWindow::CDownloadWindow(IDirect3DDevice9* device)
{
    DWORD* state = (DWORD*)nativeState;
    state[2] = (DWORD)device;
    state[10] = 0;
    state[12] = 0;
    state[5] = 640;
    state[6] = 300;
    state[7] = 210;
    state[8] = 30;
    state[13] = 3;
    memcpy(nativeState + 0x38, "Type", 5);
    memcpy(nativeState + 0xb9, "ID", 3);
    memcpy(nativeState + 0x13a, "Progress", 9);
    state[0x8f] = 180;
    state[0x90] = 220;
    state[0x93] = 0;
    state[0x94] = 0xffffffff;
    DWORD started = RakNet::GetTime();
    state[0x97] = 0;
    state[0x96] = 0;
    state[0x95] = started;
}

// 0x0006a1a0
void CDownloadWindow::ResetFonts()
{
    DWORD* state = (DWORD*)nativeState;
    DWORD height = state[6];
    BYTE* dialog = (BYTE*)state[10];
    DWORD width = state[5];
    *(DWORD*)(dialog + 0x11e) = width;
    *(DWORD*)(dialog + 0x122) = height;
    RECT client;
    GetClientRect((*(HWND*)0x00c97c1c), &client);
    DWORD left = (DWORD)(client.right / 2) - (DWORD)((LONG)state[5] / 2);
    DWORD top = (DWORD)(*(const volatile LONG*)&client.bottom / 2) - (DWORD)((LONG)state[6] / 2);
    state[3] = left;
    state[4] = top;
    dialog = (BYTE*)state[10];
    *(DWORD*)(dialog + 0x116) = left;
    *(DWORD*)(dialog + 0x11a) = top;
    DWORD captionHeight = *(DWORD*)((BYTE*)state[10] + 0x126);
    state[9] = state[6] - captionHeight;
    FontSize measured = g_deviceFonts->GetLittleTextScreenSize(g_fontMeasureProbe, 0);
    CDXUTControl* list = (CDXUTControl*)state[11];
    list->SetLocation(2, (LONG)((DWORD)measured.cy + 2));
    DWORD listHeight = state[9] - (DWORD)measured.cy - 4;
    DWORD listWidth = state[5] - 4;
    list = (CDXUTControl*)state[11];
    list->SetSize((LONG)listWidth, (LONG)listHeight);
}

// 0x0006a280
void CDownloadWindow::GetRect(RECT* out)
{
    DWORD* state = (DWORD*)nativeState;
    out->left = state[3];
    out->right = (LONG)(state[5] + (DWORD)out->left);
    out->top = state[4];
    out->bottom = (LONG)(state[6] + (DWORD)out->top);
}

// 0x0006a2b0
void CDownloadWindow::Show(DWORD mode)
{
    DWORD* state = (DWORD*)nativeState;
    state[0x94] = mode;
    FontSize measured = g_deviceFonts->GetTextScreenSize(g_fontMeasureProbe, 0);
    strncpy_s((char*)state[10] + 0x16, 256, g_downloadWindowTitle, _TRUNCATE);
    ((BYTE*)state[10])[0x14] = 1;
    *(DWORD*)((BYTE*)state[10] + 0x126) = (DWORD)measured.cy + 4;
    ((BYTE*)state[10])[1] = 0;
    ((BYTE*)state[10])[2] = 0;
    ((BYTE*)state[10])[0x13] = 1;
    if (state[11])
    {
        ((CDXUTControl*)state[11])->SetVisible(true);
        ((CDXUTControl*)state[11])->SetEnabled(true);
    }
    ResetFonts();
    state[12] = 1;
}

// 0x0006a350
void CDownloadWindow::Draw()
{
    DWORD* state = (DWORD*)nativeState;
    if (!state[12])
        return;
    CDXUTDialog* dialog = (CDXUTDialog*)state[10];
    if (dialog)
        dialog->OnRender(10.0f);
    RECT rect;
    GetRect(&rect);
    rect.top = (LONG)((DWORD)rect.top + *(DWORD*)((BYTE*)state[10] + 0x126) + 3);
    rect.left = (LONG)((DWORD)rect.left + 16);
    int column = 0;
    if ((LONG)state[13] > 0)
    {
        const DWORD* width = (const DWORD*)(nativeState + 0x23c);
        const char* title = (const char*)nativeState + 0x38;
        do
        {
            g_deviceFonts->DrawLittleText(0, title, rect, 0x120, 0xffa9c4e4, TRUE);
            rect.left = (LONG)((DWORD)rect.left + *width);
            ++column;
            ++width;
            title += 129;
        }
        while (column < (LONG)state[13]);
    }
}

// 0x0006a500
void CDownloadWindow::ResetDialogControls(CDXUTDialog* dialog)
{
    DWORD* state = (DWORD*)nativeState;
    state[10] = (DWORD)dialog;
    if (!dialog)
        return;
    CDXUTListBoxDL* created = new CDXUTListBoxDL((CDXUTDialog*)state[10]);
    state[11] = (DWORD)created;
    ((CDXUTDialog*)state[10])->AddControl(created);
    ((CDXUTControl*)state[11])->SetLocation(2, 2);
    ((CDXUTControl*)state[11])->SetSize((LONG)(state[5] - 4), (LONG)(state[6] - 35));
    ((CDXUTControl*)state[11])->OnInit();
    ((CDXUTControl*)state[11])->GetElement(0)->TextureColor.Init(0xc8ffffff, 0xc8808080, 0);
    ((CDXUTControl*)state[11])->SetEnabled(false);
    ((CDXUTControl*)state[11])->SetVisible(false);
    created = (CDXUTListBoxDL*)state[11];
    created->columnCount = 2;
    created = (CDXUTListBoxDL*)state[11];
    created->columnWidths[0] = state[0x8f];
    created = (CDXUTListBoxDL*)state[11];
    created->columnWidths[1] = state[0x90];
}

// 0x0006a420
DownloadRecord* CDownloadWindow::GetRecord(DWORD index)
{
    DWORD* state = (DWORD*)nativeState;
    if (index >= state[1])
        return 0;
    return ((DownloadRecord**)state[0])[index];
}

// 0x0006a450
bool DownloadRecordStorage::ResizeRecords(DWORD count)
{
    DWORD* state = (DWORD*)nativeState;
    if (!count)
    {
        if (state[0])
        {
            free((void*)state[0]);
            state[0] = 0;
        }
        state[1] = 0;
        return true;
    }
    DownloadRecord** records = (DownloadRecord**)realloc((void*)state[0], count * 4);
    if (!records)
        return false;
    state[0] = (DWORD)records;
    DWORD i = state[1];
    if (i < count)
    {
        do
        {
            ((DownloadRecord**)state[0])[i] = 0;
        }
        while (++i != count);
    }
    state[1] = count;
    return true;
}

// 0x0006a4c0
void CDownloadWindow::TrimRecords()
{
    DWORD* state = (DWORD*)nativeState;
    DWORD count = state[1];
    DWORD i = 0;
    DWORD last = 0xffffffff;
    if (count)
    {
        do
        {
            if (((DownloadRecord**)state[0])[i])
                last = i;
        }
        while (++i != count);
    }
    if (last == 0xffffffff)
        ResizeRecords(0);
    else
        ResizeRecords(++last);
}

// 0x0006a610
int CDownloadWindow::FindRecord(DWORD id)
{
    DWORD* state = (DWORD*)nativeState;
    DWORD count = state[1];
    DWORD i = 0;
    if (count)
    {
        do
        {
            if (GetRecord(i)->id == id)
                return (int)i;
        }
        while (++i != count);
    }
    return -1;
}

// 0x0006a640
void CDownloadWindow::ResetRecordStorage()
{
    ResizeRecords(0);
}

// 0x0006a650
int CDownloadWindow::AddRecord(DownloadRecord* record)
{
    DWORD* state = (DWORD*)nativeState;
    DWORD count = state[1];
    DWORD i = 0;
    if (count)
    {
        do
        {
            if (!((DownloadRecord**)state[0])[i])
            {
                if ((LONG)i < 0)
                    break;
                ((DownloadRecord**)state[0])[i] = record;
                return (int)i;
            }
        }
        while (++i != count);
    }
    if (!ResizeRecords(count + 1))
        return -1;
    ((DownloadRecord**)state[0])[count] = record;
    return (int)count;
}

// 0x0006a6b0
bool CDownloadWindow::RemoveRecord(DWORD index)
{
    DWORD* state = (DWORD*)nativeState;
    if (index >= state[1])
        return false;
    ((DownloadRecord**)state[0])[index] = 0;
    TrimRecords();
    return true;
}

// 0x0006a6e0
void CDownloadWindow::ClearRecords()
{
    DWORD* state = (DWORD*)nativeState;
    int count = (int)state[1];
    for (int i = 0; i < count; i++)
        if ((DWORD)i < state[1])
        {
            DownloadRecord* record = ((DownloadRecord**)state[0])[i];
            if (record)
            {
                free(record);
                RemoveRecord(i);
            }
        }
    ((CDXUTListBoxDL*)state[11])->RemoveAllItems();
}

// 0x0006ab10
void CDownloadWindow::Hide()
{
    DWORD* state = (DWORD*)nativeState;
    ((BYTE*)state[10])[0x13] = 0;
    state[12] = 0;
    ClearRecords();
    state[0x93] = 0;
    state[0x94] = 0xffffffff;
}

// 0x0006a720
void CDownloadWindow::UpdateRecord(BYTE type, DWORD id, BYTE status, DWORD downloaded, DWORD total, int error, const char* detail)
{
    DWORD* state = (DWORD*)nativeState;
    int index = FindRecord(id);
    DownloadRecord* record;
    DWORD delta;
    BYTE currentStatus;
    char text[68];
    if (index == -1)
    {
        record = (DownloadRecord*)calloc(1, 18);
        record->type = type;
        record->id = id;
        record->status = status;
        record->downloaded = downloaded;
        delta = downloaded;
        record->total = total;
        if (type == 1)
            snprintf(text, 64, g_downloadModel);
        else
            snprintf(text, 64, g_downloadTexture);
        ((CDXUTListBoxDL*)state[11])->AddItem(text, (void*)0xffffffff, -1);
        record->row = state[1];
        AddRecord(record);
        snprintf(text, 64, g_downloadIdFormat, id);
        ((CDXUTListBoxDL*)state[11])->SetSubItemText(record->row, 0, text);
        ((CDXUTListBoxDL*)state[11])->SelectItem(record->row);
        currentStatus = status;
        ((CDXUTListBoxDL*)state[11])->GetItem(record->row)->flag = true;
    }
    else
    {
        record = GetRecord(index);
        DWORD previousDownloaded = record->downloaded;
        currentStatus = status;
        record->status = currentStatus;
        delta = downloaded - previousDownloaded;
        record->downloaded = downloaded;
    }
    float percentage = 0.0f;
    if (downloaded > 0 && record->total > 0)
        percentage = (float)downloaded * g_vehicleTrainSpeedMaximum / (float)record->total;
    state[0x96] += delta;
    if ((LONG)(RakNet::GetTime() - state[0x95]) > 1000)
    {
        state[0x95] = RakNet::GetTime();
        DWORD bytes = state[0x96];
        state[0x97] = 0;
        if (bytes > 0)
        {
            float speed = (float)bytes * g_downloadKilobytesPerByte;
            memcpy(&state[0x97], &speed, 4);
        }
        state[0x96] = 0;
    }
    if (currentStatus == 0)
        snprintf(text, 64, g_downloadZeroFormat);
    else if (currentStatus == 1)
        snprintf(text, 64, g_downloadPercentFormat, (double)percentage);
    else if (currentStatus == 3)
    {
        if (detail)
            snprintf(text, 64, g_downloadDetailFormat, error, detail);
        else
            snprintf(text, 64, g_downloadErrorFormat, error, record->downloaded, record->total);
    }
    else if (currentStatus == 4)
    {
        state[0x93]++;
        snprintf(text, 64, g_downloadCompleteFormat, (double)percentage);
    }
    // Other native statuses leave the previous or untouched local text in place.
    ((CDXUTListBoxDL*)state[11])->SetSubItemText(record->row, 1, text);
    if ((LONG)state[0x94] > 0)
    {
        char title[68];
        snprintf(title, 64, g_downloadCaptionFormat, state[0x93], state[0x94], (double)*(const float*)&state[0x97]);
        strncpy_s((char*)state[10] + 0x16, 256, title, _TRUNCATE);
    }
}
