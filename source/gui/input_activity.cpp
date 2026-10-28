// Isolated Unicode/hybrid candidate only: uses the actual DXUT static IME state.
#include "gui/input_activity.h"
#include "dxstdafx.h"
// samp.dll RVA: 0x0017f698
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
// samp.dll RVA: 0x00085e60
bool ChatImeActive()
{
    if (InputImeActivityAccess::Visible())
        return true;
    return InputImeRecent(GetTickCount(), g_inputImeActivityTick);
}
