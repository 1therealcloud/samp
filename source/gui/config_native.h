#ifndef SAMP_RECOVERED_CONFIG_NATIVE_H
#define SAMP_RECOVERED_CONFIG_NATIVE_H
#include "config.h"
// Historical recovery aliases: both modules address the same native object.
typedef CConfig CInputConfig;
#define g_pInputConfig g_pConfig
#endif
