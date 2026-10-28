#include "gui/input_password.h"
#include <string.h>
#include <wchar.h>
// samp.dll RVA: 0x00180400
wchar_t g_inputMaskedText[256];
// samp.dll RVA: 0x000853a0
const wchar_t* InputMaskWideText(const wchar_t* text)
{
    memset(g_inputMaskedText, 0, 511); // Native clears exactly 511 bytes.
    size_t length = wcslen(text);
    if (length < 256)
    {
        for (size_t i = 0; i < length; ++i)
            g_inputMaskedText[i] = 0x25cf;
        g_inputMaskedText[length] = 0;
    }
    return g_inputMaskedText;
}
