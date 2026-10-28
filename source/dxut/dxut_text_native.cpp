// Compiled only by the isolated Unicode DXUT candidate build until its full ABI is closed.
#include "dxstdafx.h"
#include "gui/input_native.h"
#include "gui/input_text.h"
#include <wchar.h>

// samp.dll RVA: 0x0009b700
void InputBufferAnsiToWide(const char* text, wchar_t* output, int capacity)
{
    memset(output, 0, capacity * sizeof(wchar_t));
    int length = MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), 0, 0);
    if (length < capacity)
        MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), output, length);
}

// samp.dll RVA: 0x00090b30
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
