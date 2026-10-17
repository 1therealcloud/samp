#ifndef SAMP_RECOVERED_INPUT_NATIVE_H
#define SAMP_RECOVERED_INPUT_NATIVE_H
#include "cmdwindow.h"
#include "chatwindow.h"
#include <string.h>
#include <assert.h>
#include <new>
#include <DXUT.h>
#include <DXUTmisc.h>
#include <DXUTgui.h>
// samp.dll RVA: 0x000850d0
class InputEditBox : public CDXUTEditBox { public: const char *GetText(); };
#pragma pack(push,1)
struct InputEditColors { char unknown[0x12b]; D3DCOLOR selectedText,selectedBackground,caret; };
#pragma pack(pop)

#include "main.h"
#include "config.h"
// samp.dll RVA: 0x00085940
void InputEnableIme(bool);
// samp.dll RVA: 0x00085460
void InputResetIme();


#endif
