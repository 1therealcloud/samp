// Compiled only by the isolated Unicode DXUT candidate build until its full ABI is closed.
#include "chatwindow.h"
#include "cmdwindow.h"
#include "config.h"
#include "dxstdafx.h"
#include "main.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <DXUTmisc.h>
#include <assert.h>
#include <new>
#include <string.h>
#include <wchar.h>

// 0x0009b700
void InputBufferAnsiToWide(const char* text, wchar_t* output, int capacity)
{
    memset(output, 0, capacity * sizeof(wchar_t));
    int length = MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), 0, 0);
    if (length < capacity)
        MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), output, length);
}

// 0x00090b30
HRESULT CDXUTDialog::AddIMEEditBox(int id, const char* text, int x, int y, int width, int height, bool isDefault, CDXUTIMEEditBox** created)
{
    CDXUTIMEEditBox* edit = new CDXUTIMEEditBox(this);
    if (created)
        *created = edit;
    if (!edit)
        return E_OUTOFMEMORY;
    HRESULT result = AddControl(edit);
    if (FAILED(result))
        return result;
    edit->SetID(id);
    edit->SetLocation(x, y);
    edit->SetSize(width, height);
    edit->m_bIsDefault = isDefault;
    if (text)
        edit->SetText(text);
    return S_OK;
}

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

// 0x00180400
wchar_t g_inputMaskedText[256];
// 0x000853a0
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

// Accesses the linked DXUT's actual IMM state; no duplicate context or flags.
class InputImeAccess : public CDXUTIMEEditBox
{
public:
    // samp.dll RVA: unknown (recovery access helper for RVA 0x00085460)
    static void ResetDefaultContext(HWND window)
    {
        _ImmDisableTextFrameService((DWORD)-1);
        s_hImcDef = _ImmGetContext(window);
        _ImmReleaseContext(window, s_hImcDef);
    }
};
// 0x00085460
void InputResetIme()
{
    InputImeAccess::ResetDefaultContext(g_inputGameWindow);
}
// 0x00085940
void InputEnableIme(bool enable)
{
    CDXUTIMEEditBox::EnableImeSystem(enable);
}

// Isolated Unicode/hybrid candidate only: uses the actual DXUT static IME state.
// 0x0017f698
DWORD g_inputImeActivityTick;
class InputImeActivityAccess : public CDXUTIMEEditBox
{
public:
    // samp.dll RVA: unknown (protected DXUT state accessor; no standalone native entry)
    static bool Visible()
    {
        return s_CandList.bShowWindow || s_bShowReadingWindow;
    }
};
// 0x00085e60
bool ChatImeActive()
{
    if (InputImeActivityAccess::Visible())
        return true;
    return InputImeRecent(GetTickCount(), g_inputImeActivityTick);
}
