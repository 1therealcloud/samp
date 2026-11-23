#include "gui/scoreboard.h"

#include "game/game.h"
#include "game/util.h"
#include "gui/gui.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "render/fontrender.h"

#include <BitStream.h>
#include <RakClientInterface.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 0x0011c6d0
int g_scoreboardNotificationRpc = 23;
// 0x0011c6d4
const char g_scoreboardSignedFormat[] = "%d";
// 0x0011b3d4
const char g_scoreboardUnsignedFormat[] = "%u";
// 0x0011c6d8
const char g_scoreboardPingTitle[] = "ping";
// 0x0011c6e0
const char g_scoreboardScoreTitle[] = "score";
// 0x0011c6e8
const char g_scoreboardNameTitle[] = "name";
// 0x0011c6f0
const char g_scoreboardIdTitle[] = "id";
// 0x0011c6f4
const float g_scoreboardIdRight = 56.0f;
// 0x0011c6f8
const char g_scoreboardCountProbe[] = "Players: 10000";
// 0x0011c708
const char g_scoreboardCountFormat[] = "Players: %d";
// 0x0011c194
const float g_scoreboardHeaderInset = 5.0f;

// 0x0006e310
void ScoreboardSwapRows(ScoreboardRow* first, ScoreboardRow* second)
{
    ScoreboardRow temporary;
    memcpy(&temporary, first, 45);
    memcpy(first, second, 45);
    memcpy(second, &temporary, 45);
}

// 0x0006e4e0
void CScoreboard::ResetDialogControls(CDXUTDialog* parent)
{
    dialog = parent;
    if (!parent)
        return;
    parent->SetSize((int)width, (int)height);
    list = new CDXUTListBoxDL(dialog);
    dialog->AddControl(list);
    list->SetLocation(0, (int)headerHeight);
    list->SetSize((int)width, (int)(height - headerHeight));
    list->OnInit();
    list->GetElement(0)->TextureColor.Init(0xc8ffffff, 0xc8808080, 0);
    list->columnCount = 3;
    list->columnWidths[0] = (int)(idWidthRatio * width);
    list->columnWidths[2] = (int)(scoreWidthRatio * width);
    list->columnWidths[1] = (int)(nameWidthRatio * width);
    list->SetEnabled(false);
    list->SetVisible(false);
    Recalc();
}

// 0x0006e640
void CScoreboard::SendNotification()
{
    if (!enabled || !dialog)
        return;
    CDXUTListBoxDL* selectedList = list;
    DXUTListBoxItemDL* item = selectedList->GetItem(selectedList->GetSelectedIndex(-1));
    if (item)
    {
        WORD player = (WORD)atoi(item->text);
        RakNet::BitStream stream;
        stream.Write(player);
        stream.Write((BYTE)0);
        (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_scoreboardNotificationRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
    }
    Close(true);
}

// 0x0006e760
void CScoreboard::UpdateList()
{
    if (!g_pNetGame)
        return;
    CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    if (!players || !enabled || !dialog)
        return;
    int selected = list->GetSelectedIndex(-1);
    CDXUTScrollBar* scroll = &list->scrollBar;
    int position = list->scrollBar.GetTrackPos();
    list->RemoveAllItems();
    players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    int count = players->GetCount(FALSE) + 1;
    DWORD localId = *(WORD*)players;
    ScoreboardRow* rows = (ScoreboardRow*)calloc(count, sizeof(ScoreboardRow));
    // Native has no allocation failure branch and copies names without a bound.
    const char* name = *(DWORD*)((BYTE*)players + 0x1a) >= 16 ? *(char**)((BYTE*)players + 6) : (char*)players + 6;
    char* out = rows[0].name;
    do
    {
        *out++ = *name;
    }
    while (*name++);
    rows[0].color = (*(CLocalPlayer**)((BYTE*)players + 0x1e))->GetColorAsARGB();
    rows[0].score = players->GetLocalScore();
    rows[0].ping = players->GetLocalPing();
    rows[0].id = *(WORD*)players;
    ScoreboardRow* row = rows + 1;
    for (int id = 0; id < 1004; id++)
    {
        if (*(DWORD*)((BYTE*)players + 0xfd6 + id * 4) != 1 || (DWORD)id == localId || players->GetPlayerInfoFlag((WORD)id))
            continue;
        row->ping = players->GetPing((WORD)id);
        name = players->GetName((WORD)id);
        out = row->name;
        do
        {
            *out++ = *name;
        }
        while (*name++);
        row->score = players->GetScore((WORD)id);
        row->color = players->GetPlayer((WORD)id)->GetColorAsARGB();
        row->id = id;
        row++;
    }
    if (sortMode == 1)
    {
        for (int last = count - 1; last > 0; last--)
            for (int i = 1; i <= last; i++)
                if (strcmp(rows[i].name, rows[i - 1].name) < 0)
                    ScoreboardSwapRows(rows + i - 1, rows + i);
    }
    else if (sortMode == 2)
    {
        for (int last = count - 1; last > 0; last--)
            for (int i = 1; i <= last; i++)
                if (rows[i].score > rows[i - 1].score)
                    ScoreboardSwapRows(rows + i - 1, rows + i);
    }
    char text[256];
    for (int i = 0; i < count; i++)
    {
        row = rows + i;
        sprintf(text, g_scoreboardUnsignedFormat, row->id);
        list->AddItem(text, (void*)row->id, row->color);
        if (list->GetItem(i) && *row->name)
        {
            list->SetSubItemText(i, 0, row->name);
            sprintf(text, g_scoreboardSignedFormat, row->score);
            list->SetSubItemText(i, 1, text);
            sprintf(text, g_scoreboardUnsignedFormat, row->ping);
            list->SetSubItemText(i, 2, text);
        }
    }
    list->SelectItem(selected >= 0 ? selected : -1);
    free(rows);
    scroll->SetTrackPos(position);
}

// 0x0006eaf0
void CScoreboard::Draw()
{
    if (!enabled)
        return;
    if (g_pNetGame)
    {
        CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
        int count = players->GetCount(FALSE) + 1;
        if (count != playerCount)
        {
            playerCount = count;
            UpdateList();
        }
        ((CNetGame*)g_pNetGame)->UpdatePlayersInfo();
    }
    if (dialog)
    {
        // GTA SA virtual addresses: screen width 0x00c17044, height 0x00c17048.
        int screenWidth = *(int*)0xc17044, screenHeight = *(int*)0xc17048;
        if (screenWidth > 0 && screenHeight > 0)
            dialog->SetLocation(screenWidth / 2 - (int)width / 2, screenHeight / 2 - (int)height / 2);
        dialog->OnRender(10.0f);
    }
    int count = 0;
    if (g_pNetGame)
    {
        CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
        if (players)
            count = players->GetCount(FALSE) + 1;
    }
    char counter[64];
    sprintf(counter, g_scoreboardCountFormat, count);
    LONG left = (LONG)(__int64)((double)x + g_scoreboardHeaderInset);
    LONG top = (LONG)(__int64)((double)y + g_scoreboardHeaderInset);
    LONG right = (LONG)(__int64)((double)x + width);
    LONG bottom = (LONG)(__int64)((double)headerHeight + y);
    RECT host = {left, top, right, bottom};
    // Native reaches this dereference even after the nullable earlier net gates.
    g_deviceFonts->DrawLittleText(0, (char*)g_pNetGame + 0x131, host, 0x120, 0xffbebebe, TRUE);
    FontSize size = g_deviceFonts->GetLittleTextScreenSize(g_scoreboardCountProbe, 0);
    left = (LONG)(__int64)(((double)width - size.cx) + x);
    right = (LONG)(__int64)((double)x + width);
    RECT players = {left, top, right, bottom};
    g_deviceFonts->DrawLittleText(0, counter, players, 0x120, 0xffbebebe, TRUE);
    left = (LONG)(__int64)((double)scale * g_objectRotationFactor + x);
    right = (LONG)(__int64)((double)scale * g_scoreboardIdRight + x);
    const LONG header = (LONG)(__int64)headerHeight;
    const LONG lineTop = (LONG)((DWORD)header - (DWORD)g_deviceFonts->m_nLittleFontHeight - 3u);
    top = (LONG)(__int64)((double)lineTop + y);
    bottom = (LONG)(__int64)((double)header + y);
    RECT column = {left, top, right, bottom};
    g_deviceFonts->DrawLittleText(0, g_scoreboardIdTitle, column, 0x120, 0xff95b0d0, TRUE);
    LONG advance = (LONG)(__int64)((double)idWidthRatio * width);
    left = (LONG)((DWORD)left + (DWORD)advance);
    right = (LONG)((DWORD)right + (DWORD)advance);
    column.left = left;
    column.right = right;
    g_deviceFonts->DrawLittleText(0, g_scoreboardNameTitle, column, 0x120, 0xff95b0d0, TRUE);
    advance = (LONG)(__int64)((double)nameWidthRatio * width);
    left = (LONG)((DWORD)left + (DWORD)advance);
    right = (LONG)((DWORD)right + (DWORD)advance);
    column.left = left;
    column.right = right;
    g_deviceFonts->DrawLittleText(0, g_scoreboardScoreTitle, column, 0x120, 0xff95b0d0, TRUE);
    advance = (LONG)(__int64)((double)scoreWidthRatio * width);
    left = (LONG)((DWORD)left + (DWORD)advance);
    right = (LONG)((DWORD)right + (DWORD)advance);
    column.left = left;
    column.right = right;
    g_deviceFonts->DrawLittleText(0, g_scoreboardPingTitle, column, 0x120, 0xff95b0d0, TRUE);
}

// 0x0006e460
CScoreboard::CScoreboard(IDirect3DDevice9* device)
{
    DWORD* state = (DWORD*)nativeState;
    state[12] = (DWORD)device;
    state[13] = 0;
    state[14] = 0;
    state[15] = 0;
    state[16] = 0;
    state[1] = 0;
    if (!g_pGame || (*(int*)0x00c17044) <= 800)
    {
        state[5] = 0x44200000;
        state[6] = 0x43f00000;
    }
    else
    {
        state[5] = 0x44480000;
        state[6] = 0x44160000;
    }
    state[7] = 0x42700000;
    state[8] = 0;
    state[9] = 0x3dae147b;
    state[10] = 0x3e880000;
    state[11] = 0x3ee00000;
    Recalc();
    state[0] = 0;
}

// 0x0006e360
void CScoreboard::Recalc()
{
    RECT rect;
    GetClientRect((*(HWND*)0x00c97c1c), &rect);
    *(float*)(nativeState + 0x10) = 1.0f;
    *(float*)(nativeState + 8) = (float)rect.right * 0.5f - *(float*)(nativeState + 0x14) * 0.5f;
    *(float*)(nativeState + 12) = (float)rect.bottom * 0.5f - *(float*)(nativeState + 0x18) * 0.5f;
}

// 0x0006e3c0
void CScoreboard::GetRect(RECT* rect)
{
    int left = (int)*(float*)(nativeState + 8);
    rect->left = left;
    rect->right = left + (int)*(float*)(nativeState + 0x14);
    int top = (int)*(float*)(nativeState + 12);
    rect->top = top;
    rect->bottom = top + (int)*(float*)(nativeState + 0x18);
}

// 0x0006e410
void CScoreboard::Close(bool restoreCursor)
{
    if (*(DWORD*)nativeState && *(CDXUTDialog**)(nativeState + 0x34))
    {
        (*(CDXUTDialog**)(nativeState + 0x34))->SetVisible(false);
        (*(CDXUTListBox**)(nativeState + 0x38))->SetEnabled(false);
        (*(CDXUTListBox**)(nativeState + 0x38))->SetVisible(false);
        if (restoreCursor)
            g_pGame->SetCursorMode(0, FALSE);
        *(DWORD*)nativeState = 0;
    }
}

// 0x0006ee10
void CScoreboard::Enable()
{
    if (!*(DWORD*)nativeState && *(CDXUTDialog**)(nativeState + 0x34))
    {
        (*(CDXUTDialog**)(nativeState + 0x34))->SetVisible(true);
        (*(CDXUTListBox**)(nativeState + 0x38))->SetEnabled(true);
        (*(CDXUTListBox**)(nativeState + 0x38))->UpdateRects();
        (*(CDXUTListBox**)(nativeState + 0x38))->SetVisible(true);
        UpdateList();
        g_pGame->SetCursorMode(3, FALSE);
        *(DWORD*)nativeState = 1;
    }
}
