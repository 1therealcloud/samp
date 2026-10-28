#ifndef SAMP_RECOVERED_INPUT_PASSWORD_H
#define SAMP_RECOVERED_INPUT_PASSWORD_H
#include <windows.h>
// 0x00180400
extern wchar_t g_inputMaskedText[256];
// 0x000853a0
const wchar_t* InputMaskWideText(const wchar_t*);
#endif
