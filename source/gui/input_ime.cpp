#include "gui/input_native.h"
// Accesses the linked DXUT's actual IMM state; no duplicate context or flags.
class InputImeAccess : public CDXUTIMEEditBox {
public:
    // samp.dll RVA: unknown (recovery access helper for RVA 0x00085460)
    static void ResetDefaultContext(HWND window) {
        _ImmDisableTextFrameService((DWORD)-1);
        s_hImcDef = _ImmGetContext(window);
        _ImmReleaseContext(window, s_hImcDef);
    }
};
// samp.dll RVA: 0x00085460
void InputResetIme() { InputImeAccess::ResetDefaultContext(g_inputGameWindow); }
// samp.dll RVA: 0x00085940
void InputEnableIme(bool enable) { CDXUTIMEEditBox::EnableImeSystem(enable); }
