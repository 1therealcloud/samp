#include "client/device_setup.h"
#include "client/commands.h"
#include "main.h"
#include "cmdwindow.h"
#include "chatwindow.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
// samp.dll RVA: 0x00060de0
BOOL ClientHandleKeyRelease(unsigned int key) {
    switch(key) {
    case VK_RETURN:if(g_renderDialog&&*(DWORD*)(g_renderDialog->nativeState+0x28)){g_renderDialog->Close(1);return FALSE;}break;
    case VK_ESCAPE:if(g_pInput->m_bEnabled&&!g_pInput->IsActive()){g_pInput->Close();return FALSE;}break;
    case VK_PRIOR:if(g_pNetGame&&g_pChat&&!g_pInput->IsActive()){g_pChat->PageUp();return FALSE;}break;
    case VK_NEXT:if(g_pNetGame&&g_pChat&&!g_pInput->IsActive()){g_pChat->PageDown();return FALSE;}break;
    case VK_UP:if(g_pInput->m_bEnabled&&!g_pInput->IsActive()){g_pInput->RecallUp();return FALSE;}break;
    case VK_DOWN:if(g_pInput->m_bEnabled&&!g_pInput->IsActive())g_pInput->RecallDown();break;
    case VK_SUBTRACT:if(g_clientSettings.debug){Commands::SavePositionOnly("");return FALSE;}break;
    case VK_F6:if(!g_pInput->m_bEnabled)g_pInput->Open();else g_pInput->Close();return FALSE;
    case VK_F7:g_pChat->SwitchMode();return FALSE;
    // samp.dll RVA: 0x0016be7c
    case VK_F8:g_renderScreenshotRequested=1;return FALSE;
    case VK_F9:*(DWORD*)g_deviceDeathWindow=*(DWORD*)g_deviceDeathWindow==0;return FALSE;
    }
    return FALSE;
}
