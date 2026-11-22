#include "gui/chatbubble.h"

#include "game/game.h"
#include "game/playerped.h"
#include "gui/gui.h"
#include "gui/label.h"
#include "main.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"

#include <string.h>
// 0x0011bcd0
const float g_chatBubbleLineDistanceScale = 0.0125f;
// 0x0011bccc
const float g_chatBubbleDistanceBase = 0.065f;
// 0x0011bcc8
const float g_chatBubbleHeightLift = 0.2f;

// 0x000171a0
int FormatChatBubbleText(char* text, int width, int tolerance)
{
    int remaining = (int)strlen(text);
    int base = 0, cursor = width, rows = 1;
    char temporary[513];
    memset(temporary, 0, sizeof(temporary));
    if (strlen(text) > 512)
        return -1;
    while (remaining > width)
    {
        int boundary = width;
        while (text[base + boundary] != ' ' && boundary)
            --boundary;
        if (cursor - base - boundary > tolerance)
        {
            strcpy(temporary, text + cursor);
            text[cursor + 1] = 0;
            text[cursor] = '\n';
            strcat(text, temporary);
            remaining -= width;
            base = cursor + 1;
            cursor = base + width;
        }
        else
        {
            base += boundary;
            text[base] = '\n';
            remaining -= boundary;
            cursor = base + width;
        }
        ++rows;
    }
    return rows;
}

// 0x00066860
CChatBubble::CChatBubble()
{
    memset(entries, 0, sizeof(entries));
}

// 0x00066890
void CChatBubble::Add(WORD player, const char* text, DWORD color, float drawDistance, int lifespan)
{
    if (player >= 1004)
        return;
    ChatBubbleEntry* entry = &entries[player];
    memset(entry, 0, sizeof(*entry));
    if (strlen(text) > 144)
        return;
    DWORD converted = (color >> 8) | 0xff000000;
    // Native copies only after clearing and validating the source length.
    strcpy(entries[player].text, text);
    entry->rows = FormatChatBubbleText(entry->text, 36, 12);
    entry->color = converted;
    memcpy(&entry->drawDistance, &drawDistance, 4);
    entry->lifespan = (DWORD)lifespan;
    entry->created = GetTickCount();
    entry->active = 1;
}

// 0x00066950
void CChatBubble::Draw()
{
    if (!g_pGame->GetPlayerPed())
        return;
    if (!g_pNetGame)
        return;
    CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    DWORD now = GetTickCount();
    if (g_deviceLabel)
        g_deviceLabel->Begin();
    for (int player = 0; player < 1004; player++)
    {
        ChatBubbleEntry* entry = &entries[player];
        if (!entry->active)
            continue;
        // Native compares unsigned absolute sums, including tick overflow.
        if (now > entry->created + entry->lifespan)
        {
            entry->active = 0;
            continue;
        }
        if (!players->IsConnected((WORD)player))
            continue;
        CRemotePlayer* remote = players->GetPlayer((WORD)player);
        // Preserve inconsistent-record/null remote dereference before validation.
        CPed* ped = *(CPed**)((BYTE*)remote + 4);
        if (!ped || !((BYTE*)remote)[0x1a])
            continue;
        remote = players->GetPlayer((WORD)player);
        ped = *(CPed**)((BYTE*)remote + 4);
        if (!ped)
            continue;

        // fcomp + test ah,41h + setnp  =>  nearby = (distance <= drawDistance), NaN => false
        float distance = ped->GetDistanceToLocalPlayer();
        if (!(distance <= entry->drawDistance))
            continue;

        CVector bone;
        ped->GetBonePosition(8, &bone);
        CVector position;
        memcpy(&position, &bone, 12);

        int adjustment = (int)((DWORD)entry->rows - 1u);

        // padding і factor в asm зберігаються як float (fstp), далі розрахунок іде на стеку FPU
        float padding = (float)((double)g_pedDrunkShakeScale * (double)adjustment);
        float factor  = (float)((double)adjustment * (double)g_chatBubbleLineDistanceScale);

        double cameraDistance = (double)ped->GetDistanceToCamera();
        double result = cameraDistance * ((double)factor + (double)g_chatBubbleDistanceBase);
        result += (double)padding;
        result += (double)position.z;
        result += (double)g_chatBubbleHeightLift;
        position.z = (float)result;

        // Original per-entry draw is unconditional even if Begin was skipped.
        g_deviceLabel->Draw(&position, entry->text, entry->color, TRUE, FALSE);
    }
    if (g_deviceLabel)
        g_deviceLabel->End();
}
