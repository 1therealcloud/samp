#ifndef SAMP_RECOVERED_INPUT_TEXT_H
#define SAMP_RECOVERED_INPUT_TEXT_H
#include <windows.h>
// samp.dll RVA: 0x0017fbb8
extern char g_inputAnsiText[2049];
// samp.dll RVA: 0x00085060
const char *InputWideToAnsi(const wchar_t *);
// samp.dll RVA: 0x0009b700
void InputBufferAnsiToWide(const char *, wchar_t *, int);
#endif
