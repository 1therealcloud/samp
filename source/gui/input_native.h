#ifndef SAMP_RECOVERED_INPUT_NATIVE_H
#define SAMP_RECOVERED_INPUT_NATIVE_H
#include "chatwindow.h"
#include "cmdwindow.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <DXUTmisc.h>
#include <assert.h>
#include <new>
#include <string.h>

#include "config.h"
#include "main.h"
// samp.dll RVA: 0x00085940
void InputEnableIme(bool);
// samp.dll RVA: 0x00085460
void InputResetIme();

#endif
