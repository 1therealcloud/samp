#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

#pragma pack(push, 1)
struct ChatBubbleEntry
{
    BOOL active;
    char text[256];
    DWORD created, lifespan, color;
    float drawDistance;
    int rows;
};
class CChatBubble
{
public:
    ChatBubbleEntry entries[1004];
    // 0x00066de0
    CChatBubble();
    // Clearing precedes length validation; color input is RGBA.
    // 0x00066e10
    void Add(WORD player, const char* text, DWORD color, float drawDistance, int lifespan);
    // 0x00066ed0
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(ChatBubbleEntry) == 280);
STATIC_ASSERT(offsetof(ChatBubbleEntry, created) == 0x104);
STATIC_ASSERT(sizeof(CChatBubble) == 0x44a20);
// Mutates the caller buffer; valid length<=512 does not bound resulting length.
// 0x00017690
int FormatChatBubbleText(char* text, int width, int tolerance);
// R5 address to verify
extern const float g_chatBubbleLineDistanceScale;
// R5 address to verify
extern const float g_chatBubbleDistanceBase;
// 0x000e9c90
extern const float g_chatBubbleHeightLift;

#endif // CHATBUBBLE_H
