// Names are descriptive recovery names, not claims about original source spelling.
#ifndef SAMP_RECOVERED_CHAT_DEPENDENCIES_H
#define SAMP_RECOVERED_CHAT_DEPENDENCIES_H
#include "cmdwindow.h"
#include <stddef.h>
#include <windows.h>
typedef CInput ChatInputState; // Historical recovery alias.
// 0x00085e60
bool ChatImeActive(); // External zero-argument native dependency; body not supplied here.
// 0x000b5950
int ChatIsHexDigit(char value);
// 0x000b59c0
unsigned long ChatParseColorTag(char* text);
// 0x000b5c00
void ChatStripColorTags(char* text);
// 0x000b5c40
int ChatPrepareMessage(char* text, int maximumLength);
#endif
