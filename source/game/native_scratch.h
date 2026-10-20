#ifndef SAMP_NATIVE_SCRATCH_H
#define SAMP_NATIVE_SCRATCH_H
#include <windows.h>
// Shared physical native BSS span. Collision and rotation adapters use views
// inside it; this is not one established typed collision object.
// samp.dll RVA: 0x002aae18
extern BYTE g_entityCollisionScratch[0x1044];
#endif
