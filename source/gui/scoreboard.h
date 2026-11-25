#ifndef SCOREBOARD_H
#define SCOREBOARD_H

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

    CScoreboard(IDirect3DDevice9* device);

    void Recalc();

    void GetRect(RECT* rect);

    void Close(bool restoreCursor);

    void Enable();

    void ResetDialogControls(CDXUTDialog* parent);

    void SendNotification();

    void UpdateList();

    void Draw();
};
#pragma pack(pop)
static_assert(sizeof(CScoreboard) == 0x44);
static_assert(sizeof(ScoreboardRow) == 45);

void ScoreboardSwapRows(ScoreboardRow* first, ScoreboardRow* second);

extern int g_scoreboardNotificationRpc;

extern const char g_scoreboardSignedFormat[];

extern const char g_scoreboardUnsignedFormat[];

extern const char g_scoreboardPingTitle[];

extern const char g_scoreboardScoreTitle[];

extern const char g_scoreboardNameTitle[];

extern const char g_scoreboardIdTitle[];

extern const float g_scoreboardIdRight;

extern const char g_scoreboardCountProbe[];

extern const char g_scoreboardCountFormat[];

extern const float g_scoreboardHeaderInset;

#endif // SCOREBOARD_H
