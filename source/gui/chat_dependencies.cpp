// Recovered from native instructions. No callbacks into the reference samp.dll.
#include "gui/chat_dependencies.h"
#include <string.h>
#include <stdlib.h>


// samp.dll RVA: 0x000b5950
int ChatIsHexDigit(char value) {
    if((value>='0' && value<='9') ||
       (value>='A' && value<='F') ||
       (value>='a' && value<='f')) return 1;
    return 0;
}

// samp.dll RVA: 0x000b59c0
unsigned long ChatParseColorTag(char *text) {
    if(text[0] && text[0]=='{' &&
       text[1] && ChatIsHexDigit(text[1]) &&
       text[2] && ChatIsHexDigit(text[2]) &&
       text[3] && ChatIsHexDigit(text[3]) &&
       text[4] && ChatIsHexDigit(text[4]) &&
       text[5] && ChatIsHexDigit(text[5]) &&
       text[6] && ChatIsHexDigit(text[6]) &&
       text[7] && text[7]=='}') {
        char digits[17];
        memset(digits,0,sizeof(digits));
        strncpy(digits,text+1,6);
        return strtoul(digits,0,16);
    }
    return 0xffffffff;
}

// samp.dll RVA: 0x000b5c00
void ChatStripColorTags(char *text) {
    while(*text) {
        if(ChatParseColorTag(text)!=0xffffffff) strcpy(text,text+8);
        else ++text;
    }
    *text=0;
}

// samp.dll RVA: 0x000b5c40
int ChatPrepareMessage(char *text, int maximumLength) {
    char buffer[2049];
    memset(buffer,0,sizeof(buffer));
    if(text && strlen(text)<400 && strstr(text,"~k~")) {
        strcpy(buffer,text);
        // GTA SA's key-token expansion routine, outside samp.dll.
        // Native GTA bridge preserves the target's register/stack convention.
        __asm lea edx, buffer
        __asm push edx
        __asm mov edx, 0x69e160
        __asm call edx
        __asm pop edx
        if((int)strlen(buffer)<=maximumLength) {
            strcpy(text,buffer);
            return (int)strlen(buffer);
        }
    }
    return 0;
}
