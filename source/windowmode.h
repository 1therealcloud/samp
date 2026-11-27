#ifndef WINDOWMODE_H
#define WINDOWMODE_H

#include <d3d9.h>

#include <windows.h>

namespace WindowMode
{
void Install(HWND window);

bool HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam, LRESULT* result);

// IDirect3DDevice9Hook::Reset
void BeforeReset(D3DPRESENT_PARAMETERS* params);
void AfterReset();

// IDirect3DDevice9Hook::ShowCursor / SetCursorPosition
bool DeviceCursorHeld();
BOOL RecordDeviceCursor(BOOL show);

} // namespace WindowMode

#endif // WINDOWMODE_H
