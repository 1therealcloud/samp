#include "chatwindow.h"
#include "cmdprocs.h"
#include "cmdwindow.h"
#include "game/game.h"
#include "gui.h"
#include "main.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <d3dx9.h>
#include <stdio.h>

#ifdef GetSystemMetrics
#undef GetSystemMetrics
#endif
// 0x0016be84
DWORD g_clientInputOutsideClick;
// 0x0016be88
DWORD g_clientScoreboardOutsideClick;
// 0x000610d0
LRESULT CALLBACK ClientWindowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (g_clientIntegrityFailures > 10)
        return 0;
    if (message == WM_ACTIVATEAPP && wparam == 1 && g_pGame && g_gameD3DDevice && !g_pGame->IsMenuVisible() && !g_pGame->m_nCursorMode)
        g_gameD3DDevice->ShowCursor(FALSE);
    if (g_pInput)
        g_pInput->MsgProc(message, wparam, lparam);
    if (g_renderDialog)
        g_renderDialog->MsgProc(message, wparam, lparam);
    if (g_renderFirstDialog && g_pInput && g_pInput->m_bEnabled && !g_pGame->IsMenuVisible())
    {
        if (g_renderFirstDialog->MsgProc(window, message, wparam, lparam))
            return 0;
        if ((message == WM_KEYUP || message == WM_KEYDOWN) && wparam == VK_RETURN)
            return 0;
        if (message == WM_CHAR && wparam == 10)
            return 0;
    }
    else if (g_deviceScoreboard && g_guiScoreboardDialog)
    {
        if (!g_pGame->IsMenuVisible() && message == WM_KEYUP && wparam == VK_TAB)
        {
            if (!*(DWORD*)g_deviceScoreboard)
            {
                if (!*(DWORD*)(g_renderPanel->nativeState + 0x24))
                    g_deviceScoreboard->Enable();
            }
            else
                g_deviceScoreboard->Close(true);
        }
        if (*(DWORD*)g_deviceScoreboard && !g_pGame->IsMenuVisible())
            g_guiScoreboardDialog->MsgProc(window, message, wparam, lparam);
    }
    if (g_renderSecondDialog && *((BYTE*)g_renderSecondDialog + 0x13) && !g_pGame->IsMenuVisible())
        g_renderSecondDialog->MsgProc(window, message, wparam, lparam);
    if (g_renderDialog && g_guiServerDialog && *(DWORD*)(g_renderDialog->nativeState + 0x28) && !*(DWORD*)g_deviceScoreboard && !g_pInput->m_bEnabled &&
        !g_pGame->IsMenuVisible() && g_guiServerDialog->MsgProc(window, message, wparam, lparam))
    {
        if (message == WM_CHAR && wparam == VK_ESCAPE && !g_renderDialog->IsInputHidden())
            g_renderDialog->Close(0);
        return 0;
    }
    if (g_deviceObjectEdit)
    {
        if (((CObjectEdit*)g_deviceObjectEdit)->MsgProc(message, wparam, lparam))
            return 0;
        if (*(DWORD*)(g_deviceObjectEdit + 0x80) && message == WM_CHAR && wparam == VK_ESCAPE)
        {
            ((CObjectEdit*)g_deviceObjectEdit)->Disable(FALSE);
            return 0;
        }
    }
    if (g_renderObjectSelection)
    {
        if (g_renderObjectSelection->MsgProc(message, wparam, lparam))
            return 0;
        if (g_renderObjectSelection->enabled && message == WM_CHAR && wparam == VK_ESCAPE)
        {
            g_renderObjectSelection->Enable(FALSE);
            return 0;
        }
    }
    if (g_renderTextDrawSelection)
    {
        if (g_renderTextDrawSelection->MsgProc(message, wparam, lparam))
            return 0;
        if (g_renderTextDrawSelection->enabled && message == WM_CHAR && wparam == VK_ESCAPE)
        {
            g_renderTextDrawSelection->Disable();
            return 0;
        }
    }
    if (g_renderPanel && g_guiPanelDialog && *(DWORD*)(g_renderPanel->nativeState + 0x24) && !*(DWORD*)g_deviceScoreboard && !g_pInput->m_bEnabled && !g_pGame->IsMenuVisible() &&
        g_guiPanelDialog->MsgProc(window, message, wparam, lparam))
        return 0;
    // Native snapshots these five pointers before its message switch.
    CInput* input = g_pInput;
    CScoreboard* scoreboard = g_deviceScoreboard;
    BYTE* edit = g_deviceObjectEdit;
    CObjectSelection* selection = g_renderObjectSelection;
    CTextDrawSelection* text = g_renderTextDrawSelection;
    if (message <= WM_CHAR)
    {
        switch (message)
        {
            case WM_KILLFOCUS:
                if (scoreboard && *(DWORD*)scoreboard)
                    scoreboard->Close(true);
                if (g_pInput && g_pInput->m_bEnabled)
                    g_pInput->Close();
                if (g_renderSecondDialog && *((BYTE*)g_renderSecondDialog + 0x13))
                    g_pGame->SetCursorMode(0, TRUE);
                g_pGame->ProcessInputEnabling();
                break;
            case WM_SETCURSOR:
                if (!g_deviceResetMarker)
                {
                    SetCursor(0);
                    g_deviceResetMarker = 1;
                }
                return 1;
            case WM_KEYDOWN:
                if (wparam == VK_ESCAPE &&
                    ((input && input->m_bEnabled) || (scoreboard && *(DWORD*)scoreboard) || (g_renderDialog && *(DWORD*)(g_renderDialog->nativeState + 0x28)) ||
                     (g_renderPanel && *(DWORD*)(g_renderPanel->nativeState + 0x24)) || (g_renderSecondDialog && *((BYTE*)g_renderSecondDialog + 0x13)) ||
                     (edit && *(DWORD*)(edit + 0x80)) || (selection && selection->enabled) || (text && text->enabled)))
                    return 0;
                break;
            // 0x00060de0
            case WM_KEYUP:
                ClientHandleKeyRelease(wparam);
                break;
            // 0x00061010
            case WM_CHAR:
                ClientHandleCharacter(wparam);
                break;
        }
    }
    else if (message <= WM_LBUTTONUP)
    {
        if (message == WM_LBUTTONUP)
        {
            if (g_clientInputOutsideClick)
            {
                if (input && input->m_bEnabled)
                    input->Close();
                g_clientInputOutsideClick = 0;
                return 0;
            }
            if (g_clientScoreboardOutsideClick)
            {
                if (scoreboard && *(DWORD*)scoreboard)
                    scoreboard->Close(true);
                g_clientScoreboardOutsideClick = 0;
                return 0;
            }
        }
        else if (message == WM_SYSKEYUP)
        {
            if (wparam == VK_RETURN)
                ClientToggleWindowedMode();
        }
        else if (message == WM_LBUTTONDOWN)
        {
            int x = (short)LOWORD(lparam), y = (short)HIWORD(lparam);
            RECT rect;
            if (scoreboard && *(DWORD*)scoreboard)
            {
                scoreboard->GetRect(&rect);
                if (x < rect.left || x > rect.right || y < rect.top || y > rect.bottom)
                {
                    g_clientScoreboardOutsideClick = 1;
                    return 0;
                }
            }
            else if (input && input->m_bEnabled && !*(DWORD*)(edit + 0x80) && !selection->enabled)
            {
                InputRect inputRect;
                input->GetRect(&inputRect);
                rect = inputRect;
                if (x < rect.left || x > rect.right || y < rect.top || y > rect.bottom)
                {
                    g_clientInputOutsideClick = 1;
                    return 0;
                }
            }
        }
    }
    else if (message == WM_MOUSEWHEEL && g_pInput->m_bEnabled && g_pChat->m_nMode)
    {
        g_pChat->Scroll((short)HIWORD(wparam) / 120);
        return 0;
    }
    return CallWindowProcA(g_clientPreviousWindowProcedure, window, message, wparam, lparam);
}

// 0x00061010
BOOL ClientHandleCharacter(unsigned int character)
{
    if (g_pInput && g_pInput->m_bEnabled)
    {
        if (character == 27)
        {
            g_pInput->Close();
            return TRUE;
        }
    }
    else if (g_deviceScoreboard && *(DWORD*)g_deviceScoreboard)
    {
        if (character == 27)
        {
            g_deviceScoreboard->Close(true);
            return TRUE;
        }
    }
    else if (g_renderDialog && *(DWORD*)(g_renderDialog->nativeState + 0x28))
    {
        if (character == 27)
        {
            g_renderDialog->Close(0);
            return TRUE;
        }
    }
    else if ((character == 'T' || character == '`' || character == 't') && g_pInput && (!g_renderDialog || !*(DWORD*)(g_renderDialog->nativeState + 0x28)) &&
             !g_pGame->IsMenuVisible())
    {
        g_pInput->Open();
        return TRUE;
    }
    return FALSE;
}
// 0x00060d90
void ClientToggleWindowedMode()
{
    // GTA SA virtual addresses: current mode C97C18, mode table C97C48, change-mode 745C70.
    int mode = *(int*)0xc97c18;
    DWORD* windowed = (DWORD*)(*(DWORD*)0xc97c48 + mode * 20 + 16);
    *windowed = *windowed == 0;
    *(int*)0xc97c18 = -1;
    ((void(__cdecl*)(int))0x745c70)(mode);
    *(int*)0xc97c18 = mode;
}

// 0x0011b928
const char g_screenshotFilenamePattern[] = "\\screens\\sa-mp-%03i.png";

// 0x0011c7dc
const char g_screenshotTakenMessage[] = "Screenshot Taken - sa-mp-%03i.png";

// 0x0011c7c0
const char g_screenshotFailureMessage[] = "Unable to save screenshot.";

// 0x0016be78
WNDPROC g_clientPreviousWindowProcedure;

// 0x00061cf0
int ClientFindScreenshotFilename(std::string* path)
{
    path->assign(ClientGetDataDirectory());
    char candidate[260] = {0};
    int index;
    for (index = 0; index < 1000; index++)
    {
        sprintf(candidate, (*path + g_screenshotFilenamePattern).c_str(), index);
        WIN32_FIND_DATAA data;
        HANDLE found = FindFirstFileA(candidate, &data);
        if (found == INVALID_HANDLE_VALUE)
            break;
        FindClose(found);
    }
    path->assign(candidate);
    // If all names exist, native reports1000 but retains the final999 filename.
    return index;
}

// 0x00073b80
int ClientSaveRenderTarget(IDirect3DDevice9* device, const char* filename)
{
    IDirect3DSurface9 *target = 0, *surface = 0;
    POINT origin = {0, 0};
    RECT rect;
    ClientToScreen(g_inputGameWindow, &origin);
    GetClientRect(g_inputGameWindow, &rect);
    rect.left += origin.x;
    rect.right += origin.x;
    rect.top += origin.y;
    rect.bottom += origin.y;
    if (FAILED(device->GetRenderTarget(0, &target)))
        return 1;
    D3DSURFACE_DESC desc;
    if (FAILED(target->GetDesc(&desc)))
        return 2;
    if (FAILED(device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &surface, 0)))
        return 3;
    if (FAILED(device->GetRenderTargetData(target, surface)))
        return 4;
    D3DXSaveSurfaceToFileA(filename, D3DXIFF_PNG, surface, 0, &rect);
    target->Release();
    surface->Release();
    return 0;
}

// 0x00075040
void ClientCaptureScreenshot()
{
    g_renderScreenshotRequested = 0;
    std::string path;
    int index = ClientFindScreenshotFilename(&path);
    // Native captures this vtable slot before both metric calls, then reloads this.
    typedef HRESULT(WINAPI * CreateSurfaceFunction)(IDirect3DDevice9*, UINT, UINT, D3DFORMAT, D3DPOOL, IDirect3DSurface9**, HANDLE*);
    CreateSurfaceFunction create = (CreateSurfaceFunction)(*(void***)g_gameD3DDevice)[36];
    int height = GetSystemMetrics(SM_CYSCREEN), width = GetSystemMetrics(SM_CXSCREEN);
    IDirect3DSurface9* surface;
    create(g_gameD3DDevice, width, height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surface, 0);
    if (SUCCEEDED(g_gameD3DDevice->GetFrontBufferData(0, surface)))
    {
        POINT origin = {0, 0};
        ClientToScreen(g_inputGameWindow, &origin);
        RECT rect;
        GetClientRect(g_inputGameWindow, &rect);
        rect.left += origin.x;
        rect.right += origin.x;
        rect.top += origin.y;
        rect.bottom += origin.y;
        D3DXSaveSurfaceToFileA(path.c_str(), D3DXIFF_PNG, surface, 0, &rect);
        g_pChat->AddInfoMessage(g_screenshotTakenMessage, index);
    }
    else
        g_pChat->AddDebugMessage(g_screenshotFailureMessage);
    // Original does not release surface and does not check create/save HRESULTs.
}

// 0x00061940
void ClientRefreshWindowProcedure()
{
    HWND window = g_inputGameWindow;
    if (window)
    {
        g_clientPreviousWindowProcedure = (WNDPROC)GetWindowLongA(window, GWL_WNDPROC);
        if (g_clientPreviousWindowProcedure != ClientWindowProcedure)
            SetWindowLongA(window, GWL_WNDPROC, (LONG)ClientWindowProcedure);
    }
}

// 0x00061970
BOOL ClientInstallWindowProcedure()
{
    HWND window = g_inputGameWindow;
    if (!window)
        return FALSE;
    SetClassLongA(window, GCL_STYLE, GetClassLongA(window, GCL_STYLE) | CS_DBLCLKS);
    HWND current = g_inputGameWindow;
    if (current)
    {
        g_clientPreviousWindowProcedure = (WNDPROC)GetWindowLongA(current, GWL_WNDPROC);
        if (g_clientPreviousWindowProcedure != ClientWindowProcedure)
            SetWindowLongA(current, GWL_WNDPROC, (LONG)ClientWindowProcedure);
    }
    SetWindowTextA(window, "GTA:SA:MP");
    IsWindowUnicode(window);
    return TRUE;
}

// 0x00060de0
BOOL ClientHandleKeyRelease(unsigned int key)
{
    switch (key)
    {
        case VK_RETURN:
            if (g_renderDialog && *(DWORD*)(g_renderDialog->nativeState + 0x28))
            {
                g_renderDialog->Close(1);
                return FALSE;
            }
            break;
        case VK_ESCAPE:
            if (g_pInput->m_bEnabled && !g_pInput->IsActive())
            {
                g_pInput->Close();
                return FALSE;
            }
            break;
        case VK_PRIOR:
            if (g_pNetGame && g_pChat && !g_pInput->IsActive())
            {
                g_pChat->PageUp();
                return FALSE;
            }
            break;
        case VK_NEXT:
            if (g_pNetGame && g_pChat && !g_pInput->IsActive())
            {
                g_pChat->PageDown();
                return FALSE;
            }
            break;
        case VK_UP:
            if (g_pInput->m_bEnabled && !g_pInput->IsActive())
            {
                g_pInput->RecallUp();
                return FALSE;
            }
            break;
        case VK_DOWN:
            if (g_pInput->m_bEnabled && !g_pInput->IsActive())
                g_pInput->RecallDown();
            break;
        case VK_SUBTRACT:
            if (g_clientSettings.debug)
            {
                Commands::SavePositionOnly("");
                return FALSE;
            }
            break;
        case VK_F6:
            if (!g_pInput->m_bEnabled)
                g_pInput->Open();
            else
                g_pInput->Close();
            return FALSE;
        case VK_F7:
            g_pChat->SwitchMode();
            return FALSE;
        // 0x0016be7c
        case VK_F8:
            g_renderScreenshotRequested = 1;
            return FALSE;
        case VK_F9:
            *(DWORD*)g_deviceDeathWindow = *(DWORD*)g_deviceDeathWindow == 0;
            return FALSE;
    }
    return FALSE;
}
