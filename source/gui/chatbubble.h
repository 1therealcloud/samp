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
    // 0x00066860
    CChatBubble();
    // Clearing precedes length validation; color input is RGBA.
    // 0x00066890
    void Add(WORD player, const char* text, DWORD color, float drawDistance, int lifespan);
    // 0x00066950
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(ChatBubbleEntry) == 280);
STATIC_ASSERT(offsetof(ChatBubbleEntry, created) == 0x104);
STATIC_ASSERT(sizeof(CChatBubble) == 0x44a20);
// Mutates the caller buffer; valid length<=512 does not bound resulting length.
// 0x000171a0
int FormatChatBubbleText(char* text, int width, int tolerance);
// 0x0011bcd0
extern const float g_chatBubbleLineDistanceScale;
// 0x0011bccc
extern const float g_chatBubbleDistanceBase;
// 0x0011bcc8
extern const float g_chatBubbleHeightLift;

#endif // CHATBUBBLE_H
