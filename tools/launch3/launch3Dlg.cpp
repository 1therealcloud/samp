//----------------------------------------------------------
//
// launch3Dlg.cpp
//
//----------------------------------------------------------

#include "stdafx.h"
#include "launch3Dlg.h"
#include "launch3.h"

#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------

typedef BOOL(WINAPI* PFN_CREATE_PROCESS_A)(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL, DWORD, LPVOID, LPCSTR, LPSTARTUPINFOA, LPPROCESS_INFORMATION);

CString GetAppPath();
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

// 0x12B0
CString GetAppPath()
{
    char module[MAX_PATH];
    module[0] = '\0';
    GetModuleFileNameA(AfxGetInstanceHandle(), module, MAX_PATH);
    CString path(module);
    return path.Left(path.ReverseFind('\\') + 1);
}

//----------------------------------------------------

// 0x1130
void LaunchGame(LPCSTR directory, LPSTR arguments, DWORD unused)
{
    char application[256];
    char dll[256];

    sprintf(application, "%s\\%s", directory, "gta_sa.exe");
    sprintf(dll, "%s\\%s", directory, "samp.dll");

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

// 0x1540
static FARPROC ResolveLoadLibraryA()
{
    HMODULE kernel32 = GetModuleHandleA("Kernel32");
    return GetProcAddress(kernel32, "LoadLibraryA");
}

//----------------------------------------------------

// 0x1560
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

// 0x1630
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

// 0x1890
BYTE* EmitMovEAX(BYTE* code, DWORD value)
{
    *code++ = 0xB8;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x18C0
BYTE* EmitMovEBX(BYTE* code, DWORD value)
{
    *code++ = 0xBB;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x18F0
BYTE* EmitMovECX(BYTE* code, DWORD value)
{
    *code++ = 0xB9;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x1920
BYTE* EmitMovEDX(BYTE* code, DWORD value)
{
    *code++ = 0xBA;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x1950
BYTE* EmitMovESI(BYTE* code, DWORD value)
{
    *code++ = 0xBE;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x1980
BYTE* EmitMovEDI(BYTE* code, DWORD value)
{
    *code++ = 0xBF;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x19B0
BYTE* EmitMovEBP(BYTE* code, DWORD value)
{
    *code++ = 0xBD;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x19E0
BYTE* EmitMovESP(BYTE* code, DWORD value)
{
    *code++ = 0xBC;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x1A10
BYTE* EmitPushImm32(BYTE* code, DWORD value)
{
    *code++ = 0x68;
    *(DWORD*)code = value;
    return code + 4;
}

//----------------------------------------------------

// 0x1A40
BYTE* EmitJmpRel32(BYTE* code, DWORD target, DWORD instructionVA)
{
    if (instructionVA == 0)
        instructionVA = (DWORD)code;
    *code++ = 0xE9;
    *(DWORD*)code = target - (instructionVA + 5);
    return code + 4;
}

//----------------------------------------------------

// 0x1A80
BYTE* EmitCallRel32(BYTE* code, DWORD target, DWORD instructionVA)
{
    if (instructionVA == 0)
        instructionVA = (DWORD)code;
    *code++ = 0xE8;
    *(DWORD*)code = target - (instructionVA + 5);
    return code + 4;
}

//----------------------------------------------------
// CLaunch3Dlg dialog

// 0x1200
CLaunch3Dlg::CLaunch3Dlg(CWnd* pParent /*=NULL*/) : CDialog(CLaunch3Dlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//----------------------------------------------------

// 0x1290
void CLaunch3Dlg::DoDataExchange(CDataExchange* pDX)
{
    (void)pDX;
}

//----------------------------------------------------

BEGIN_MESSAGE_MAP(CLaunch3Dlg, CDialog)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_EN_CHANGE(IDC_NICK, OnChangeNick)
ON_BN_CLICKED(IDC_LAUNCH, OnLaunch)
ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
END_MESSAGE_MAP()

//----------------------------------------------------
// CLaunch3Dlg message handlers

// 0x1390
BOOL CLaunch3Dlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return TRUE;
}

//----------------------------------------------------

// 0x13D0
void CLaunch3Dlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

//----------------------------------------------------

// 0x1490
HCURSOR CLaunch3Dlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

//----------------------------------------------------

// 0x14A0
void CLaunch3Dlg::OnChangeNick() {}

//----------------------------------------------------

// 0x14B0
void CLaunch3Dlg::OnLaunch()
{
    CString strAppPath = GetAppPath();
    LaunchGame((LPCSTR)strAppPath, "-d", 0);
}

//----------------------------------------------------

// 0x14E0
void CLaunch3Dlg::OnButton2()
{
    OnCancel();
}

//----------------------------------------------------

// 0x14F0
void CLaunch3Dlg::OnButton3()
{
    CString strAppPath = GetAppPath();
    LaunchGame((LPCSTR)strAppPath, "-c -h 127.0.0.1 -p 7777 -n Player", 0);
}

//----------------------------------------------------
// EOF
