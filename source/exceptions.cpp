#include "exceptions.h"

#include "game/game.h"
#include "game/scripting.h"
#include "main.h"

#include <tlhelp32.h>
// Native crash reports use the ANSI 548-byte Toolhelp record.
#undef MODULEENTRY32
#undef Module32First
#undef Module32Next

/*
#include "httpclient.h"
#include "runutil.h"
*/

#include <stdio.h>
#include <string.h>
// 0x0015fa68
CONTEXT* g_clientExceptionContext;
// 0x0015fa70
char g_clientCrashText[16384];
// 0x00151960
BYTE* g_clientCrashPedContext = (BYTE*)0xb7cd74; // GTA SA virtual address.
// 0x0018f584
WORD g_clientCrashLastRenderedObject;

// 0x00060260
/*
void ClientSendCrashReport()
{
    CHttpClient http;
    char url[256], encoded[16384];
    ClientConstructCrashText(TRUE);
    strcpy_s(encoded, "data=");
    Util_Base64Encode(g_clientCrashText, encoded + 5);
    snprintf(url, sizeof(url), "team.sa-mp.com/report_037.php?addr=0x%X&gta=sa", g_clientExceptionContext->Eip);
    http.ProcessURL(HTTP_POST, url, encoded, "www.sa-mp.com");
}
*/

// 0x0005fd50
void ClientDumpMemory(const BYTE* data, int count, char (&text)[16384], BOOL asDwords)
{
    char line[16384];
    for (int offset = 0; offset < count; offset += 16)
    {
        const BYTE* p = data + offset;
        if (asDwords)
        {
            const DWORD* words = (const DWORD*)p;
            snprintf(line, sizeof(line), "+%04X: 0x%08X   0x%08X   0x%08X   0x%08X\r\n", offset, words[0], words[1], words[2], words[3]);
        }
        else
        {
            snprintf(line, sizeof(line), "+%04X: %02X %02X %02X %02X   %02X %02X %02X %02X   %02X %02X %02X %02X   %02X %02X %02X %02X\r\n", offset, p[0], p[1], p[2], p[3], p[4],
                     p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
        }
        strncat_s(text, line, _TRUNCATE);
    }
}

// 0x0005fc20
void ClientDumpNetworkState(char (&text)[16384])
{
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    BYTE* players = *(BYTE**)(pools + 8);
    char line[16384];
    text[0] = 0;
    snprintf(text, sizeof(text), "\r\nState Information: Ped Context: %u\r\n", (unsigned)*g_clientCrashPedContext);
    for (WORD index = 0; index < 1004; ++index)
    {
        if (*(BOOL*)(players + 0xfd6 + index * 4))
        {
            BYTE* remote = *(BYTE**)(players + 0x26 + index * 4);
            BYTE* player = remote ? *(BYTE**)(remote + 8) : 0;
            snprintf(line, sizeof(line), "P%u (%u,%u) ", (unsigned)index, (unsigned)player[0x1a], (unsigned)*(WORD*)(player + 2));
            strncat_s(text, line, _TRUNCATE);
            if (index && ((index + 1) % 4) == 0)
                strncat_s(text, "\r\n", _TRUNCATE);
        }
    }
}

// 0x0005fee0
void ClientDumpLoadedModules(char (&text)[16384])
{
    MODULEENTRY32 entry;
    char line[16384];
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    strcpy_s(text, "\r\nLoaded Modules:\r\n");
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        strncat_s(text, "-FailedCreate-\r\n", _TRUNCATE);
        return;
    }
    entry.dwSize = sizeof(entry);
    if (!Module32First(snapshot, &entry))
    {
        strncat_s(text, "-FailedFirst-", _TRUNCATE);
        CloseHandle(snapshot);
        return;
    }
    do
    {
        if (entry.szModule[0] != 'f' && entry.szModule[1] != 'l' && entry.szModule[2] != 'a')
        {
            snprintf(line, sizeof(line), "%s\tB: 0x%08X\tS: 0x%08X\t(%s)\r\n", entry.szModule, reinterpret_cast<UINT_PTR>(entry.modBaseAddr), entry.modBaseSize, entry.szExePath);
            strncat_s(text, line, _TRUNCATE);
        }
    }
    while (Module32Next(snapshot, &entry));
    CloseHandle(snapshot);
}

// 0x00060070
void ClientConstructCrashText(BOOL includeModules)
{
    char text[16384];
    CONTEXT* c = g_clientExceptionContext;
    snprintf(g_clientCrashText, sizeof(g_clientCrashText),
             "SA-MP %s\r\nException At Address: 0x%08X\r\nBase: 0x%08X\r\n\r\nRegisters:\r\nEAX: 0x%08X\tEBX: 0x%08X\tECX: 0x%08X\tEDX: 0x%08X\r\nESI: 0x%08X\tEDI: 0x%08X\tEBP: "
             "0x%08X\tESP: 0x%08X\r\nEFLAGS: 0x%08X\r\n\r\nStack:\r\n",
             g_ClientVersionString.c_str(), c->Eip, reinterpret_cast<UINT_PTR>(g_clientModule),
             c->Eax, c->Ebx, c->Ecx, c->Edx, c->Esi, c->Edi, c->Ebp, c->Esp, c->EFlags);

    ClientDumpMemory((const BYTE*)c->Esp, 640, g_clientCrashText, TRUE);
    snprintf(text, sizeof(text), "\r\nSCM Op: 0x%X, lDbg: %d LastRendObj: %u\r\n", g_scriptLastOpcode, g_scriptLocalDebug, (unsigned)g_clientCrashLastRenderedObject);
    strncat_s(g_clientCrashText, text, _TRUNCATE);
    const char* version = g_gameVersion == 1 ? "US 1.0" : g_gameVersion == 2 ? "EU 1.0" : "UNKNOWN";
    snprintf(text, sizeof(text), "\r\nGame Version: %s\r\n", version);
    strncat_s(g_clientCrashText, text, _TRUNCATE);
    if (g_pNetGame)
    {
        ClientDumpNetworkState(text);
        strncat_s(g_clientCrashText, text, _TRUNCATE);
    }
    if (includeModules)
    {
        ClientDumpLoadedModules(text);
        strncat_s(g_clientCrashText, text, _TRUNCATE);
    }
}

// 0x00060330
INT_PTR CALLBACK ClientExceptionDialog(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            EndDialog(dialog, TRUE);
            ShowWindow((*(HWND*)0x00c97c1c), SW_HIDE);
            DestroyWindow((*(HWND*)0x00c97c1c));
            break;
        case WM_INITDIALOG:
            ClientConstructCrashText(FALSE);
            SetDlgItemTextA(dialog, 1000, g_clientCrashText);
            SetForegroundWindow(GetDlgItem(dialog, 101));
            SetFocus(GetDlgItem(dialog, 1001));
            SetCursor(LoadCursorA(0, MAKEINTRESOURCEA(32512)));
            ShowCursor(TRUE);
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case 1001:
                    EndDialog(dialog, TRUE);
                    break;
                case 1002:
                    //ClientSendCrashReport();
                    EnableWindow(GetDlgItem(dialog, 1002), FALSE);
                    SetDlgItemTextA(dialog, 1000, "Thanks for reporting this problem.");
                    break;
            }
            break;
    }
    return FALSE;
}

// 0x00060430
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception)
{
    g_clientExceptionContext = exception->ContextRecord;
    ShowWindow((*(HWND*)0x00c97c1c), SW_MINIMIZE);
    DialogBoxParamA(g_clientModule, MAKEINTRESOURCEA(101), (*(HWND*)0x00c97c1c), ClientExceptionDialog, 0);
    return EXCEPTION_EXECUTE_HANDLER;
}
