#ifndef SAMP_SCOREBOARD_H
#define SAMP_SCOREBOARD_H
#include <d3dx9.h>
#include "dxut/dxut_listbox_native.h"
#pragma pack(push,1)
struct ScoreboardRow {DWORD color;char name[29];DWORD id;int score;DWORD ping;};
class CScoreboard {
public:
    union {
        BYTE nativeState[0x44];
        struct {
            BOOL enabled;int playerCount;
            float x,y,scale,width,height,headerHeight;
            DWORD unknown20;
            float idWidthRatio,scoreWidthRatio,nameWidthRatio;
            IDirect3DDevice9 *device;
            CDXUTDialog *dialog;
            CDXUTListBoxDL *list;
            DWORD unknown3c;
            int sortMode;
        };
    };
    // samp.dll RVA: 0x0006e460
    CScoreboard(IDirect3DDevice9 *device);
    // samp.dll RVA: 0x0006e360
    void Recalc();
    // samp.dll RVA: 0x0006e3c0
    void GetRect(RECT *rect);
    // samp.dll RVA: 0x0006e410
    void Close(bool restoreCursor);
    // samp.dll RVA: 0x0006ee10
    void Enable();
    // samp.dll RVA: 0x0006e4e0
    void ResetDialogControls(CDXUTDialog *parent);
    // samp.dll RVA: 0x0006e640
    void SendNotification();
    // samp.dll RVA: 0x0006e760
    void UpdateList();
    // samp.dll RVA: 0x0006eaf0
    void Draw();
};
#pragma pack(pop)
typedef char ScoreboardExtent[(sizeof(CScoreboard)==0x44)?1:-1];
typedef char ScoreboardRowExtent[(sizeof(ScoreboardRow)==45)?1:-1];
// samp.dll RVA: 0x0006e310
void ScoreboardSwapRows(ScoreboardRow *first,ScoreboardRow *second);
// samp.dll RVA: 0x0011c6d0
extern int g_scoreboardNotificationRpc;
// samp.dll RVA: 0x0011c6d4
extern const char g_scoreboardSignedFormat[];
// samp.dll RVA: 0x0011b3d4
extern const char g_scoreboardUnsignedFormat[];
// samp.dll RVA: 0x0011c6d8
extern const char g_scoreboardPingTitle[];
// samp.dll RVA: 0x0011c6e0
extern const char g_scoreboardScoreTitle[];
// samp.dll RVA: 0x0011c6e8
extern const char g_scoreboardNameTitle[];
// samp.dll RVA: 0x0011c6f0
extern const char g_scoreboardIdTitle[];
// samp.dll RVA: 0x0011c6f4
extern const float g_scoreboardIdRight;
// samp.dll RVA: 0x0011c6f8
extern const char g_scoreboardCountProbe[];
// samp.dll RVA: 0x0011c708
extern const char g_scoreboardCountFormat[];
// samp.dll RVA: 0x0011c194
extern const float g_scoreboardHeaderInset;
#endif
