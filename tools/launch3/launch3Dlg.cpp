//----------------------------------------------------------
//
// launch3Dlg.cpp
//
//----------------------------------------------------------

#include "stdafx.h"

#include "launch3Dlg.h"

#include "resource.h"

#include <stdio.h>
#include <string.h>
#include <string>

//----------------------------------------------------

typedef BOOL(WINAPI* PFN_CREATE_PROCESS_A)(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);

void LaunchGame(LPCSTR szDirectory, LPSTR szArguments, DWORD dwUnused);
BOOL WINAPI CreateProcessWithDll(LPCSTR application, LPSTR arguments, LPSECURITY_ATTRIBUTES processAttributes, LPSECURITY_ATTRIBUTES threadAttributes, BOOL inheritHandles,
                                 DWORD flags, LPVOID environment, LPCSTR currentDirectory, LPSTARTUPINFOA startupInfo, LPPROCESS_INFORMATION processInfo, LPCSTR dllPath,
                                 PFN_CREATE_PROCESS_A createProcessFunction);
BOOL InjectDllViaThreadContext(HANDLE process, HANDLE thread, FARPROC loadLibraryA, LPCSTR dllPath, DWORD pathLength);
BYTE* EmitMovEAX(BYTE* code, DWORD value);
BYTE* EmitMovEBX(BYTE* code, DWORD value);
BYTE* EmitMovECX(BYTE* code, DWORD value);
BYTE* EmitMovEDX(BYTE* code, DWORD value);
BYTE* EmitMovESI(BYTE* code, DWORD value);
BYTE* EmitMovEDI(BYTE* code, DWORD value);
BYTE* EmitMovEBP(BYTE* code, DWORD value);
BYTE* EmitMovESP(BYTE* code, DWORD value);
BYTE* EmitPushImm32(BYTE* code, DWORD value);
BYTE* EmitJmpRel32(BYTE* code, DWORD target, DWORD instructionVA);
BYTE* EmitCallRel32(BYTE* code, DWORD target, DWORD instructionVA);

static PROCESS_INFORMATION g_processInformation; // 0x1F780
static STARTUPINFOA g_startupInformation;        // 0x1F790

//----------------------------------------------------

static std::string GetAppPath()
{
    char module[MAX_PATH];
    const DWORD length = GetModuleFileNameA(nullptr, module, MAX_PATH);
    if (length == 0 || length >= MAX_PATH)
        return {};
    const std::string path(module, length);
    const size_t separator = path.rfind('\\');
    return separator == std::string::npos ? std::string() : path.substr(0, separator + 1);
}

//----------------------------------------------------

void LaunchGame(LPCSTR directory, LPSTR arguments, DWORD unused)
{
    char application[256];
    char dll[256];

    snprintf(application, sizeof(application), "%s\\%s", directory, "gta_sa.exe");
    snprintf(dll, sizeof(dll), "%s\\%s", directory, "samp.dll");

    ZeroMemory(&g_processInformation, sizeof(g_processInformation));
    ZeroMemory(&g_startupInformation, sizeof(g_startupInformation));
    g_startupInformation.cb = sizeof(g_startupInformation);

    if (!CreateProcessWithDll(application, arguments, NULL, NULL, FALSE, 0x04000000, NULL, directory, &g_startupInformation, &g_processInformation, dll, NULL))
    {
        MessageBoxA(NULL, "Initialization failed.\r\nPlease reinstall.", "SA:MP", 0x4010);
    }
    (void)unused;
}

//----------------------------------------------------

static FARPROC ResolveLoadLibraryA()
{
    HMODULE kernel32 = GetModuleHandleA("Kernel32");
    return GetProcAddress(kernel32, "LoadLibraryA");
}

//----------------------------------------------------

BOOL WINAPI CreateProcessWithDll(LPCSTR application, LPSTR arguments, LPSECURITY_ATTRIBUTES processAttributes, LPSECURITY_ATTRIBUTES threadAttributes, BOOL inheritHandles,
                                 DWORD flags, LPVOID environment, LPCSTR currentDirectory, LPSTARTUPINFOA startupInfo, LPPROCESS_INFORMATION processInfo, LPCSTR dllPath,
                                 PFN_CREATE_PROCESS_A createProcessFunction)
{
    PROCESS_INFORMATION info;
    DWORD forcedFlags = flags | CREATE_SUSPENDED;

    if (createProcessFunction == NULL)
        createProcessFunction = &CreateProcessA;

    if (!createProcessFunction(application, arguments, processAttributes, threadAttributes, inheritHandles, forcedFlags, environment, currentDirectory, startupInfo, &info))
        return FALSE;

    DWORD length = dllPath ? ((DWORD)strlen(dllPath) + 1) : 0;
    if (!InjectDllViaThreadContext(info.hProcess, info.hThread, ResolveLoadLibraryA(), dllPath, length))
        return FALSE;

    if (processInfo != NULL)
        memcpy(processInfo, &info, sizeof(info));

    if ((flags & CREATE_SUSPENDED) == 0)
        ResumeThread(info.hThread);

    return TRUE;
}

//----------------------------------------------------

BOOL InjectDllViaThreadContext(HANDLE process, HANDLE thread, FARPROC loadLibraryA, LPCSTR dllPath, DWORD pathLength)
{
    DWORD bytesWritten = 0;
    DWORD oldProtection = 0;
    DWORD remoteCodeAddress;
    BYTE remoteBuffer[0x480];
    BYTE* out = remoteBuffer;
    CONTEXT context;
    BOOL result = FALSE;

    SuspendThread(thread);
    ZeroMemory(&context, sizeof(context));
    context.ContextFlags = CONTEXT_FULL;

    if (!GetThreadContext(thread, &context))
        goto done;

    remoteCodeAddress = (context.Esp - 0x480) & ~((DWORD)0x1F);

    if (dllPath != NULL)
    {
        // offset 0x80 inside the code block is reserved for the DLL path
        memcpy(remoteBuffer + 0x80, dllPath, pathLength);
        out = EmitPushImm32(out, remoteCodeAddress + 0x80);
        out = EmitCallRel32(out, (DWORD)loadLibraryA, remoteCodeAddress + (DWORD)(out - remoteBuffer));
    }

    // restore the original general-purpose registers in the stub
    out = EmitMovEAX(out, context.Eax);
    out = EmitMovEBX(out, context.Ebx);
    out = EmitMovECX(out, context.Ecx);
    out = EmitMovEDX(out, context.Edx);
    out = EmitMovESI(out, context.Esi);
    out = EmitMovEDI(out, context.Edi);
    out = EmitMovEBP(out, context.Ebp);
    out = EmitMovESP(out, context.Esp);
    out = EmitJmpRel32(out, context.Eip, remoteCodeAddress + (DWORD)(out - remoteBuffer));

    context.Esp = remoteCodeAddress - 4;
    context.Eip = remoteCodeAddress;

    if (!VirtualProtectEx(process, (LPVOID)remoteCodeAddress, 0x480, PAGE_EXECUTE_READWRITE, &oldProtection))
        goto done;

    if (!WriteProcessMemory(process, (LPVOID)remoteCodeAddress, remoteBuffer, 0x480, &bytesWritten))
        goto done;

    if (!FlushInstructionCache(process, (LPCVOID)remoteCodeAddress, 0x480))
        goto done;

    if (!SetThreadContext(thread, &context))
        goto done;

    result = TRUE;

done:
    ResumeThread(thread);
    return result;
}

// these are deliberately separate routines, matching all eleven
// project-owned emitters at 00401890..00401ABB
//----------------------------------------------------

BYTE* EmitMovEAX(BYTE* code, DWORD value)
{
    *code++ = 0xB8;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitMovEBX(BYTE* code, DWORD value)
{
    *code++ = 0xBB;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitMovECX(BYTE* code, DWORD value)
{
    *code++ = 0xB9;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitMovEDX(BYTE* code, DWORD value)
{
    *code++ = 0xBA;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitMovESI(BYTE* code, DWORD value)
{
    *code++ = 0xBE;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitMovEDI(BYTE* code, DWORD value)
{
    *code++ = 0xBF;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitMovEBP(BYTE* code, DWORD value)
{
    *code++ = 0xBD;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitMovESP(BYTE* code, DWORD value)
{
    *code++ = 0xBC;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitPushImm32(BYTE* code, DWORD value)
{
    *code++ = 0x68;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitJmpRel32(BYTE* code, DWORD target, DWORD instructionVA)
{
    if (instructionVA == 0)
        instructionVA = (DWORD)code;
    *code++ = 0xE9;
    *(DWORD*)code = target - (instructionVA + 5);
    return code + 4;
}

//----------------------------------------------------

BYTE* EmitCallRel32(BYTE* code, DWORD target, DWORD instructionVA)
{
    if (instructionVA == 0)
        instructionVA = (DWORD)code;
    *code++ = 0xE8;
    *(DWORD*)code = target - (instructionVA + 5);
    return code + 4;
}

//----------------------------------------------------
INT_PTR CALLBACK LaunchDialogProc(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            const HICON icon = LoadIconA(reinterpret_cast<HINSTANCE>(lParam), MAKEINTRESOURCEA(IDR_MAINFRAME));
            SendMessageA(dialog, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
            SendMessageA(dialog, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
            return TRUE;
        }
        case WM_PAINT:
            if (IsIconic(dialog))
            {
                PAINTSTRUCT paint;
                HDC dc = BeginPaint(dialog, &paint);
                SendMessageA(dialog, WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc), 0);
                RECT rect;
                GetClientRect(dialog, &rect);
                const int x = (rect.right - rect.left - GetSystemMetrics(SM_CXICON) + 1) / 2;
                const int y = (rect.bottom - rect.top - GetSystemMetrics(SM_CYICON) + 1) / 2;
                DrawIcon(dc, x, y, reinterpret_cast<HICON>(SendMessageA(dialog, WM_GETICON, ICON_BIG, 0)));
                EndPaint(dialog, &paint);
                return TRUE;
            }
            break;
        case WM_QUERYDRAGICON:
            return SendMessageA(dialog, WM_GETICON, ICON_BIG, 0);
        case WM_COMMAND:
            if (HIWORD(wParam) != BN_CLICKED)
                break;
            switch (LOWORD(wParam))
            {
                case IDC_LAUNCH:
                {
                    char arguments[] = "-d";
                    LaunchGame(GetAppPath().c_str(), arguments, 0);
                    return TRUE;
                }
                case IDC_BUTTON3:
                {
                    char arguments[] = "-c -h 127.0.0.1 -p 7777 -n Player";
                    LaunchGame(GetAppPath().c_str(), arguments, 0);
                    return TRUE;
                }
                case IDC_BUTTON2:
                case IDCANCEL:
                    EndDialog(dialog, 0);
                    return TRUE;
            }
            break;
        case WM_CLOSE:
            EndDialog(dialog, 0);
            return TRUE;
    }
    return FALSE;
}
