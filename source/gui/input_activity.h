#ifndef SAMP_RECOVERED_INPUT_ACTIVITY_H
#define SAMP_RECOVERED_INPUT_ACTIVITY_H
#include <windows.h>
// samp.dll RVA: 0x0017f698
extern DWORD g_inputImeActivityTick;
// samp.dll RVA: 0x00085e60
bool ChatImeActive();
// samp.dll RVA: unknown (extracted signed tick arithmetic; no standalone native entry)
inline bool InputImeRecent(DWORD now, DWORD start)
{
    return (LONG)(now - start) < 300;
}
#endif
