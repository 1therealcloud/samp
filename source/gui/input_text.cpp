#include "gui/input_text.h"
#include <string.h>
#include <wchar.h>
// 0x0017fbb8
char g_inputAnsiText[2049]; // Includes the observed terminator write at index 2048.
// 0x00085060
const char* InputWideToAnsi(const wchar_t* text)
{
    memset(g_inputAnsiText, 0, 2048);
    int length = WideCharToMultiByte(CP_ACP, 0, text, (int)wcslen(text), 0, 0, 0, 0);
    if (length < 2049)
    {
        WideCharToMultiByte(CP_ACP, 0, text, (int)wcslen(text), g_inputAnsiText, length, 0, 0);
        g_inputAnsiText[length] = 0;
    }
    return g_inputAnsiText;
}
