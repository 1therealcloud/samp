#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <stddef.h>
#include <windows.h>

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

    CChatBubble();
    // Clearing precedes length validation; color input is RGBA.

    void Add(WORD player, const char* text, DWORD color, float drawDistance, int lifespan);

    void Draw();
};
static_assert(sizeof(ChatBubbleEntry) == 280);
static_assert(offsetof(ChatBubbleEntry, created) == 0x104);
static_assert(sizeof(CChatBubble) == 0x44a20);
// Mutates the caller buffer; valid length<=512 does not bound resulting length.

extern const float g_chatBubbleLineDistanceScale;

extern const float g_chatBubbleDistanceBase;

extern const float g_chatBubbleHeightLift;

#endif // CHATBUBBLE_H
