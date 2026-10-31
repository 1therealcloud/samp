#ifndef SAMP_SCOREBOARD_H
#define SAMP_SCOREBOARD_H
#include <DXUTgui.h>
#include <d3dx9.h>
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
    // 0x0006e460
    CScoreboard(IDirect3DDevice9* device);
    // 0x0006e360
    void Recalc();
    // 0x0006e3c0
    void GetRect(RECT* rect);
    // 0x0006e410
    void Close(bool restoreCursor);
    // 0x0006ee10
    void Enable();
    // 0x0006e4e0
    void ResetDialogControls(CDXUTDialog* parent);
    // 0x0006e640
    void SendNotification();
    // 0x0006e760
    void UpdateList();
    // 0x0006eaf0
    void Draw();
};
#pragma pack(pop)
typedef char ScoreboardExtent[(sizeof(CScoreboard) == 0x44) ? 1 : -1];
typedef char ScoreboardRowExtent[(sizeof(ScoreboardRow) == 45) ? 1 : -1];
// 0x0006e310
void ScoreboardSwapRows(ScoreboardRow* first, ScoreboardRow* second);
// 0x0011c6d0
extern int g_scoreboardNotificationRpc;
// 0x0011c6d4
extern const char g_scoreboardSignedFormat[];
// 0x0011b3d4
extern const char g_scoreboardUnsignedFormat[];
// 0x0011c6d8
extern const char g_scoreboardPingTitle[];
// 0x0011c6e0
extern const char g_scoreboardScoreTitle[];
// 0x0011c6e8
extern const char g_scoreboardNameTitle[];
// 0x0011c6f0
extern const char g_scoreboardIdTitle[];
// 0x0011c6f4
extern const float g_scoreboardIdRight;
// 0x0011c6f8
extern const char g_scoreboardCountProbe[];
// 0x0011c708
extern const char g_scoreboardCountFormat[];
// 0x0011c194
extern const float g_scoreboardHeaderInset;
#endif
