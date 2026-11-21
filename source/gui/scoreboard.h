#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <DXUTgui.h>
#include <d3dx9.h>

#include <static_assert.h>

#pragma pack(push, 1)
struct ScoreboardRow
{
    DWORD color;
    char name[29];
    DWORD id;
    int score;
    DWORD ping;
};
class CScoreboard
{
public:
    union
    {
        BYTE nativeState[0x44];
        struct
        {
            BOOL enabled;
            int playerCount;
            float x, y, scale, width, height, headerHeight;
            DWORD unknown20;
            float idWidthRatio, scoreWidthRatio, nameWidthRatio;
            IDirect3DDevice9* device;
            CDXUTDialog* dialog;
            CDXUTListBoxDL* list;
            DWORD unknown3c;
            int sortMode;
        };
    };
    // 0x0006ea30
    CScoreboard(IDirect3DDevice9* device);
    // 0x0006e930
    void Recalc();
    // 0x0006e990
    void GetRect(RECT* rect);
    // 0x0006e9e0
    void Close(bool restoreCursor);
    // 0x0006f3d0
    void Enable();
    // 0x0006eab0
    void ResetDialogControls(CDXUTDialog* parent);
    // 0x0006ec10
    void SendNotification();
    // 0x0006ed30
    void UpdateList();
    // 0x0006f0b0
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CScoreboard) == 0x44);
STATIC_ASSERT(sizeof(ScoreboardRow) == 45);
// R5 address to verify
void ScoreboardSwapRows(ScoreboardRow* first, ScoreboardRow* second);
// R5 address to verify
extern int g_scoreboardNotificationRpc;
// 0x000ea6ac
extern const char g_scoreboardSignedFormat[];
// 0x000e9344
extern const char g_scoreboardUnsignedFormat[];
// 0x000ea6b0
extern const char g_scoreboardPingTitle[];
// 0x000ea6b8
extern const char g_scoreboardScoreTitle[];
// 0x000ea6c0
extern const char g_scoreboardNameTitle[];
// R5 address to verify
extern const char g_scoreboardIdTitle[];
// R5 address to verify
extern const float g_scoreboardIdRight;
// 0x000ea6d0
extern const char g_scoreboardCountProbe[];
// 0x000ea6e0
extern const char g_scoreboardCountFormat[];
// 0x000ea16c
extern const float g_scoreboardHeaderInset;

#endif // SCOREBOARD_H
