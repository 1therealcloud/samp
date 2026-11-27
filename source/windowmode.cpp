#include "windowmode.h"

#include "MinHook.h"
#include "cmdprocs.h"
#include "game/game.h"
#include "gui/chatwindow.h"
#include "gui/cmdwindow.h"
#include "gui/gui.h"
#include "main.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

struct WindowModeVideoMode
{
    D3DDISPLAYMODE mode;
    DWORD flags; // bit 0 = exclusive (fullscreen)
};

static const UINT_PTR g_timerId = 0x57D1;
static const DWORD g_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

static bool g_enabled;
static bool g_armed;                    // window is ours (windowed mode)
static bool g_busy;                     // inside our own mode change, ignore WM_SIZE
static bool g_resetting;                // inside IDirect3DDevice9::Reset (ours or rw's)
static bool g_deviceCursorHidden;       // d3d cursor hidden by us while the mouse is over the frame / title bar
static BOOL g_deviceCursorWas;
static bool g_freeCursor;               // after a resize: no grab until the next click in the client area
static bool g_inDefProc;                // our own DefWindowProc for frame hit-tests, SetCursor is allowed there
static bool g_sizeMove;                 // user is dragging / resizing
static bool g_clipped;
static bool g_hasPlacement;
static bool g_autoChecked;              // auto windowed mode is tried once
static int g_pendingWidth, g_pendingHeight;
static D3DDISPLAYMODE g_savedMode;       // fullscreen mode to go back to on alt+enter
static D3DPRESENT_PARAMETERS g_savedPresent;
static bool g_hasSavedPresent, g_restorePresent;
static WINDOWPLACEMENT g_placement;

static HWND Window()
{
    return g_pGame ? (HWND)g_pGame->GetWindowHandle() : 0;
}
static int& CurrentMode()
{
    return *(int*)0xc97c18;
}
static WindowModeVideoMode* Modes()
{
    return *(WindowModeVideoMode**)0xc97c48;
}

static bool ModeValid()
{
    int mode = CurrentMode();
    return mode >= 0 && mode < *(int*)0xc97c40 && Modes();
}

// RsGlobal.ps (0xc17054), diMouse at +28
static IDirectInputDevice8A* MouseDevice()
{
    BYTE* ps = *(BYTE**)0xc17054;
    return ps ? *(IDirectInputDevice8A**)(ps + 28) : nullptr;
}

static void ChangeMode(int width, int height, bool exclusive, bool keepWindow)
{
    if (g_busy || width <= 0 || height <= 0 || !ModeValid())
        return;
    int mode = CurrentMode();
    WindowModeVideoMode& entry = Modes()[mode];

    g_pendingWidth = width;
    g_pendingHeight = height;
    g_busy = true; // reset sends WM_SIZE back to us

    // rw / d3d restyle and resize the window during reset, put it back afterwards
    HWND window = Window();
    LONG style = GetWindowLongA(window, GWL_STYLE);
    RECT rect;
    GetWindowRect(window, &rect);

    if (exclusive)
    {
        entry.mode = g_savedMode; // original format and refresh rate
        entry.flags |= 1;
    }
    else
    {
        entry.flags &= ~1u;
        entry.mode.Format = D3DFMT_X8R8G8B8; // A8R8G8B8 is not valid for CheckDeviceFormat (rw checks texture formats against it)
        entry.mode.RefreshRate = 0;
    }
    entry.mode.Width = width;
    entry.mode.Height = height; // camera raster takes its size from here
    CurrentMode() = -1;         // otherwise engine skips the change
    GameSetVideoMode(mode);
    CurrentMode() = mode;

    if (keepWindow)
    {
        SetWindowLongA(window, GWL_STYLE, style);
        SetWindowPos(window, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
    g_busy = false;
}

static void ApplySize(int width, int height)
{
    if (!ModeValid())
        return;
    WindowModeVideoMode& entry = Modes()[CurrentMode()];
    if (!(entry.flags & 1) && entry.mode.Width == (UINT)width && entry.mode.Height == (UINT)height)
        return;
    ChangeMode(width, height, false, true);
}

static void SetMouseExclusive(HWND window, bool exclusive)
{
    IDirectInputDevice8A* mouse = MouseDevice();
    if (!mouse)
        return;
    mouse->Unacquire();
    mouse->SetCooperativeLevel(window, DISCL_FOREGROUND | (exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE));
    mouse->Acquire();
}

// WM_ACTIVATE (0x747FA3) and WM_KILLFOCUS (0x748054) write 0 to ForegroundApp (0x8D621C), which stops
// frames and network; the immediate is patched to 1 so the game keeps running unfocused
static void SetKeepRunning(bool keep)
{
    static const DWORD immediates[] = {
        g_gameVersion == 1 ? 0x747FB6 : 0x748006, // WM_ACTIVATE
        g_gameVersion == 1 ? 0x74805A : 0x7480AA  // WM_KILLFOCUS
    };

    for (DWORD address : immediates)
    {
        if (*(BYTE*)address == !keep)
        {
            UnFuck((void*)address, 1);
            *(BYTE*)address = keep;
        }
    }
}

// engine started windowed on its own, just give the window a resizable frame
static void Adopt(HWND window)
{
    WindowModeVideoMode& entry = Modes()[CurrentMode()];
    g_savedMode = entry.mode;
    g_hasSavedPresent = false; // nothing fullscreen to restore, rw builds it
    g_armed = true;
    SetKeepRunning(true);

    RECT client;
    GetClientRect(window, &client);
    POINT origin = {0, 0};
    ClientToScreen(window, &origin);
    RECT outer = {origin.x, origin.y, origin.x + client.right, origin.y + client.bottom};
    AdjustWindowRect(&outer, WS_OVERLAPPEDWINDOW, FALSE); // keeps the client area as is

    SetWindowLongA(window, GWL_STYLE, g_style);
    SetWindowPos(window, HWND_NOTOPMOST, outer.left, outer.top, outer.right - outer.left, outer.bottom - outer.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    SetMouseExclusive(window, false);
}

static void EnterWindowed(HWND window)
{
    WindowModeVideoMode& entry = Modes()[CurrentMode()];
    g_savedMode = entry.mode;
    g_savedPresent = *(D3DPRESENT_PARAMETERS*)0xc9c040; // rw's present params, still the live fullscreen ones
    g_hasSavedPresent = true;

    RECT frame = {0, 0, 0, 0};
    AdjustWindowRect(&frame, WS_OVERLAPPEDWINDOW, FALSE);
    int frameWidth = frame.right - frame.left, frameHeight = frame.bottom - frame.top;

    int clientWidth, clientHeight;
    RECT outer = {0, 0, 0, 0};
    if (g_hasPlacement)
    {
        outer = g_placement.rcNormalPosition;
        clientWidth = outer.right - outer.left - frameWidth;
        clientHeight = outer.bottom - outer.top - frameHeight;
    }
    else
    {
        RECT work;
        SystemParametersInfoA(SPI_GETWORKAREA, 0, &work, 0);
        int workWidth = work.right - work.left, workHeight = work.bottom - work.top;
        clientWidth = g_savedMode.Width;
        clientHeight = g_savedMode.Height;
        if (clientWidth > workWidth - frameWidth)
            clientWidth = workWidth - frameWidth;
        if (clientHeight > workHeight - frameHeight)
            clientHeight = workHeight - frameHeight;
        int x = work.left + (workWidth - (clientWidth + frameWidth)) / 2;
        int y = work.top + (workHeight - (clientHeight + frameHeight)) / 2;
        SetRect(&outer, x, y, x + clientWidth + frameWidth, y + clientHeight + frameHeight);
    }
    if (clientWidth < 320)
        clientWidth = 320;
    if (clientHeight < 240)
        clientHeight = 240;

    g_armed = true;
    SetKeepRunning(true);
    // reset first, d3d fiddles with the window when leaving exclusive mode
    ChangeMode(clientWidth, clientHeight, false, false);

    SetWindowLongA(window, GWL_STYLE, g_style);
    if (g_hasPlacement)
    {
        WINDOWPLACEMENT placement = g_placement;
        placement.length = sizeof(placement);
        if (placement.showCmd != SW_SHOWMAXIMIZED)
            placement.showCmd = SW_SHOWNORMAL;
        SetWindowPlacement(window, &placement);
        SetWindowPos(window, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
    }
    else
        SetWindowPos(window, HWND_NOTOPMOST, outer.left, outer.top, outer.right - outer.left, outer.bottom - outer.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    SetMouseExclusive(window, false);
}

static void LeaveWindowed(HWND window)
{
    ZeroMemory(&g_placement, sizeof(g_placement));
    g_placement.length = sizeof(g_placement);
    g_hasPlacement = GetWindowPlacement(window, &g_placement) != 0;

    g_armed = false;
    SetKeepRunning(false); // exclusive fullscreen has to pause when the focus is lost
    ClipCursor(0);
    g_clipped = false;

    SetWindowLongA(window, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    g_restorePresent = g_hasSavedPresent;
    ChangeMode(g_savedMode.Width, g_savedMode.Height, true, false);
    g_restorePresent = false;
    SetWindowPos(window, 0, 0, 0, g_savedMode.Width, g_savedMode.Height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
    SetMouseExclusive(window, true);
}

static void ToggleWindowedMode()
{
    HWND window = Window();
    if (!window || !ModeValid())
        return;
    if (Modes()[CurrentMode()].flags & 1)
        EnterWindowed(window);
    else
    {
        if (!g_armed)
            Adopt(window);
        LeaveWindowed(window);
    }
}

// win32 counter must be >= 0, otherwise the arrow is hidden whatever SetCursor says
static void RaiseCursorCount()
{
    int count = ShowCursor(TRUE);
    while (count < 0)
        count = ShowCursor(TRUE);
    if (count > 0)
        ShowCursor(FALSE);
}

// the system cursor is ours: over the frame, or in the client area after a resize while the game draws no cursor of its own
static bool OwnsCursor()
{
    if (g_deviceCursorHidden)
        return true;
    return g_freeCursor && g_pGame && !g_pGame->IsMenuVisible() && !g_pGame->m_nCursorMode;
}

// true when the mouse belongs to the game (playing, cursor inside the client area)
static bool GameOwnsMouse(HWND window)
{
    if (GetForegroundWindow() != window || g_sizeMove || IsIconic(window))
        return false;
    if (g_freeCursor || g_deviceCursorHidden || (GetKeyState(VK_MENU) & 0x8000))
        return false;
    POINT point;
    RECT client;
    GetCursorPos(&point);
    GetClientRect(window, &client);
    ScreenToClient(window, &point);
    return PtInRect(&client, point) != 0; // over the frame / outside -> not ours
}

// over the frame only the system cursor may live, d3d / game SetCursor calls are dropped
static HCURSOR(WINAPI* g_realSetCursor)(HCURSOR);

static HCURSOR WINAPI SetCursorHook(HCURSOR cursor)
{
    if (OwnsCursor() && !g_inDefProc)
        return GetCursor();
    return g_realSetCursor(cursor);
}

// the game re-centers the mouse every frame for mouse look, which must not happen over the frame or without focus
static BOOL(WINAPI* g_realSetCursorPos)(int, int);

static BOOL WINAPI SetCursorPosHook(int x, int y)
{
    if (OwnsCursor() || (g_armed && !GameOwnsMouse(Window())))
        return TRUE;
    return g_realSetCursorPos(x, y);
}

static void InstallCursorHooks()
{
    LPVOID target = nullptr;
    if (MH_CreateHookApiEx(L"user32", "SetCursor", (LPVOID)SetCursorHook, (LPVOID*)&g_realSetCursor, &target) != MH_OK || MH_EnableHook(target) != MH_OK)
        OutputDebugString("WindowMode: SetCursor hook failed\n");
    if (MH_CreateHookApiEx(L"user32", "SetCursorPos", (LPVOID)SetCursorPosHook, (LPVOID*)&g_realSetCursorPos, &target) != MH_OK || MH_EnableHook(target) != MH_OK)
        OutputDebugString("WindowMode: SetCursorPos hook failed\n");
}

// DirectInput still reports mouse movement while the cursor is free, which would turn the camera;
// the game gets an empty state instead, the real call is still made so no delta piles up
static HRESULT(__stdcall* g_realGetDeviceState)(IDirectInputDevice8A*, DWORD, LPVOID);
static HRESULT __stdcall GetDeviceStateHook(IDirectInputDevice8A* self, DWORD size, LPVOID data)
{
    HRESULT result = g_realGetDeviceState(self, size, data);
    if (SUCCEEDED(result) && data && g_armed && self == MouseDevice() && !GameOwnsMouse(Window()))
        ZeroMemory(data, size);
    return result;
}

// the mouse device only exists after the game initialised input, so this is retried from the timer
static void InstallMouseHook()
{
    static bool installed;
    IDirectInputDevice8A* mouse = MouseDevice();
    if (installed || !mouse)
        return;
    installed = true;
    LPVOID target = (*(LPVOID**)mouse)[9]; // IDirectInputDevice8::GetDeviceState (shared by all devices, filtered by this)
    if (MH_CreateHook(target, (LPVOID)GetDeviceStateHook, (LPVOID*)&g_realGetDeviceState) != MH_OK || MH_EnableHook(target) != MH_OK)
        OutputDebugString("WindowMode: GetDeviceState hook failed\n");
}

static void Tick(HWND window)
{
    if (!g_gameD3DDevice || !g_pGame || !ModeValid())
        return;
    InstallMouseHook();

    bool exclusive = (Modes()[CurrentMode()].flags & 1) != 0;
    if (!g_autoChecked)
    {
        g_autoChecked = true;
        if (exclusive && IsDebuggerPresent())
        {
            ToggleWindowedMode();
            return;
        }
    }
    if (!g_armed && !exclusive)
        Adopt(window);

    // game hides the cursor with ShowCursor(FALSE) all the time, counter goes negative
    // and the arrow never comes back over the title bar
    if (g_armed)
        RaiseCursorCount();

    bool grab = g_armed && GetForegroundWindow() == window && !g_sizeMove && !IsIconic(window) && !g_pGame->IsMenuVisible() && !g_pGame->m_nCursorMode &&
                !(GetKeyState(VK_MENU) & 0x8000) && !g_freeCursor;
    if (grab)
    {
        RECT rect;
        GetClientRect(window, &rect);
        MapWindowPoints(window, 0, (POINT*)&rect, 2);
        ClipCursor(&rect);
        g_clipped = true;
    }
    else if (g_clipped)
    {
        ClipCursor(0);
        g_clipped = false;
    }
}

void WindowMode::Install(HWND window)
{
    g_enabled = true;
    InstallCursorHooks();
    SetTimer(window, g_timerId, 15, 0); // short, so the camera does not turn for long when the cursor leaves the client area
}

bool WindowMode::DeviceCursorHeld()
{
    return g_deviceCursorHidden;
}

// the game asked for the d3d cursor while we hold it hidden: remember the request, return the previous one
BOOL WindowMode::RecordDeviceCursor(BOOL show)
{
    BOOL previous = g_deviceCursorWas;
    g_deviceCursorWas = show;
    return previous;
}

// rw builds its own present parameters from the video mode entry, which the windowed mode already switches to windowed.
// So only force what must not stay fullscreen, and leave format, swap effect and presentation interval as rw built them.
void WindowMode::BeforeReset(D3DPRESENT_PARAMETERS* params)
{
    if (!g_enabled)
        return;
    g_resetting = true; // d3d sends window messages synchronously from inside Reset
    if (!params)
        return;
    if (g_armed)
    {
        HWND window = Window();
        if (g_busy)
        {
            params->BackBufferWidth = g_pendingWidth;
            params->BackBufferHeight = g_pendingHeight;
        }
        else
        {
            RECT client;
            if (!IsIconic(window) && GetClientRect(window, &client) && client.right > 0 && client.bottom > 0)
            {
                params->BackBufferWidth = client.right;
                params->BackBufferHeight = client.bottom;
            }
        }
        params->Windowed = TRUE;
        params->hDeviceWindow = window;
        params->FullScreen_RefreshRateInHz = 0; // must be 0 in windowed mode
    }
    else if (g_restorePresent)
        *params = g_savedPresent;
}

void WindowMode::AfterReset()
{
    g_resetting = false;
}

// returns true if the message is fully handled
bool WindowMode::HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LRESULT* result)
{
    if (!g_enabled)
        return false;
    if (message == WM_TIMER)
    {
        if (wparam != g_timerId)
            return false;
        Tick(window);
        *result = 0;
        return true;
    }
    // reset dispatches activation / position messages synchronously, never let them into game code
    if ((g_busy || (g_resetting && g_armed)) && message != WM_STYLECHANGING)
    {
        *result = DefWindowProcA(window, message, wparam, lparam);
        return true;
    }
    if (message == WM_SYSKEYUP && wparam == VK_RETURN)
    {
        ToggleWindowedMode();
        *result = 0;
        return true;
    }
    if (!g_armed)
        return false;

    switch (message)
    {
        case WM_STYLECHANGING: // keep rw / d3d from restyling the window behind our back
            if ((int)wparam == GWL_STYLE)
            {
                STYLESTRUCT* style = (STYLESTRUCT*)lparam;
                style->styleNew = (style->styleNew & (WS_MINIMIZE | WS_MAXIMIZE)) | g_style;
            }
            *result = 0;
            return true;
        case WM_SYSCOMMAND:
            if ((wparam & 0xFFF0) == SC_KEYMENU) // alt alone would freeze the game in menu mode
            {
                *result = 0;
                return true;
            }
            break;
        case WM_SETCURSOR:
            if (LOWORD(lparam) == HTCLIENT)
            {
                // game / d3d own the cursor in the client area
                if (g_deviceCursorHidden)
                {
                    g_deviceCursorHidden = false;
                    if (g_gameD3DDevice)
                        g_gameD3DDevice->ShowCursor(g_deviceCursorWas);
                    // game calls SetCursor(0) only once, the frame arrow would stay in the client area
                    if (!g_deviceCursorWas)
                        SetCursor(0);
                }
                if (OwnsCursor())
                {
                    g_inDefProc = true;
                    SetCursor(LoadCursorA(0, IDC_ARROW));
                    g_inDefProc = false;
                    *result = TRUE;
                    return true;
                }
                return false;
            }
            // frame and title bar: only the system cursor, otherwise d3d draws its own on top and they alternate
            if (!g_deviceCursorHidden && g_gameD3DDevice)
            {
                g_deviceCursorWas = g_gameD3DDevice->ShowCursor(FALSE);
                g_deviceCursorHidden = true;
            }
            RaiseCursorCount();
            g_inDefProc = true;
            *result = DefWindowProcA(window, message, wparam, lparam); // resize arrows etc
            g_inDefProc = false;
            return true;
        case WM_LBUTTONDOWN:
            if (g_freeCursor)
            {
                g_freeCursor = false;
                g_inDefProc = true;
                SetCursor(0); // game only hides it once, arrow would stay
                g_inDefProc = false;
                *result = 0; // this click is only for grabbing, not for the game
                return true;
            }
            break;
        case WM_SIZING:
            // game's handler resets the device on every step and assumes the right/bottom edge was dragged; resize once on WM_EXITSIZEMOVE instead
            *result = TRUE;
            return true;
        case WM_GETMINMAXINFO:
        {
            RECT rect = {0, 0, 320, 240};
            AdjustWindowRect(&rect, GetWindowLongA(window, GWL_STYLE), FALSE);
            MINMAXINFO* info = (MINMAXINFO*)lparam;
            info->ptMinTrackSize.x = rect.right - rect.left;
            info->ptMinTrackSize.y = rect.bottom - rect.top;
            *result = 0;
            return true;
        }
        case WM_ENTERSIZEMOVE:
            g_sizeMove = true;
            g_freeCursor = true; // stays free after the resize, click in the window to grab again
            ClipCursor(0);
            g_clipped = false;
            *result = 0;
            return true;
        case WM_EXITSIZEMOVE:
        {
            g_sizeMove = false;
            RECT client;
            GetClientRect(window, &client);
            ApplySize(client.right, client.bottom); // one reset at the end, not per pixel
            *result = 0;
            return true;
        }
        case WM_SIZE:
            if (!g_busy && !g_sizeMove && wparam != SIZE_MINIMIZED) // maximize / restore
                ApplySize(LOWORD(lparam), HIWORD(lparam));
            *result = DefWindowProcA(window, message, wparam, lparam); // maximize breaks otherwise on newer windows
            return true;
        case WM_KILLFOCUS:
            ClipCursor(0);
            g_clipped = false;
            return false; // game still wants this one
    }
    return false;
}
