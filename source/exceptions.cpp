#include "exceptions.h"
#include "main.h"
#include "game/game.h"
#include "game/scripting.h"
#include <tlhelp32.h>
// Native crash reports use the ANSI 548-byte Toolhelp record.
#undef MODULEENTRY32
#undef Module32First
#undef Module32Next
#include <stdio.h>
#include <string.h>
#include "httpclient.h"
#include "runutil.h"
// samp.dll RVA: 0x00060260
void ClientSendCrashReport() {
    CHttpClient http;
    char url[256],encoded[16384];
    ClientConstructCrashText(TRUE);
    strcpy(encoded,"data=");
    Util_Base64Encode(g_clientCrashText,encoded+5);
    sprintf(url,"team.sa-mp.com/report_037.php?addr=0x%X&gta=sa",g_clientExceptionContext->Eip);
    http.ProcessURL(HTTP_POST,url,encoded,"www.sa-mp.com");
}
// samp.dll RVA: 0x0015fa68
CONTEXT *g_clientExceptionContext;
// samp.dll RVA: 0x0015fa70
char g_clientCrashText[16384];
// samp.dll RVA: 0x00151960
BYTE *g_clientCrashPedContext=(BYTE*)0xb7cd74; // GTA SA virtual address.
// samp.dll RVA: 0x0018f584
WORD g_clientCrashLastRenderedObject;
// samp.dll RVA: 0x0005fd50
void ClientDumpMemory(const BYTE *data,int count,char *text,BOOL asDwords) {
    char line[16384];
    for(int offset=0;offset<count;offset+=16) {
        const BYTE *p=data+offset;
        if(asDwords) {
            const DWORD *words=(const DWORD*)p;
            sprintf(line,"+%04X: 0x%08X   0x%08X   0x%08X   0x%08X\r\n",offset,words[0],words[1],words[2],words[3]);
        } else {
            sprintf(line,"+%04X: %02X %02X %02X %02X   %02X %02X %02X %02X   %02X %02X %02X %02X   %02X %02X %02X %02X\r\n",offset,p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15]);
        }
        strcat(text,line);
    }
}
// samp.dll RVA: 0x0005fc20
void ClientDumpNetworkState(char *text) {
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    BYTE *players=*(BYTE**)(pools+8);
    char line[16384];
    text[0]=0;
    sprintf(text,"\r\nState Information: Ped Context: %u\r\n",(unsigned)*g_clientCrashPedContext);
    for(WORD index=0;index<1004;++index) {
        if(*(BOOL*)(players+0xfd6+index*4)) {
            BYTE *remote=*(BYTE**)(players+0x26+index*4);
            BYTE *player=remote?*(BYTE**)(remote+8):0;
            sprintf(line,"P%u (%u,%u) ",(unsigned)index,(unsigned)player[0x1a],(unsigned)*(WORD*)(player+2));
            strcat(text,line);
            if(index && ((index+1)%4)==0)strcat(text,"\r\n");
        }
    }
}
// samp.dll RVA: 0x0005fee0
void ClientDumpLoadedModules(char *text) {
    MODULEENTRY32 entry;
    char line[16384];
    HANDLE snapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
    strcpy(text,"\r\nLoaded Modules:\r\n");
    if(snapshot==INVALID_HANDLE_VALUE) { strcat(text,"-FailedCreate-\r\n");return; }
    entry.dwSize=sizeof(entry);
    if(!Module32First(snapshot,&entry)) { strcat(text,"-FailedFirst-");CloseHandle(snapshot);return; }
    do {
        if(entry.szModule[0]!='f' && entry.szModule[1]!='l' && entry.szModule[2]!='a') {
            sprintf(line,"%s\tB: 0x%08X\tS: 0x%08X\t(%s)\r\n",entry.szModule,entry.modBaseAddr,entry.modBaseSize,entry.szExePath);
            strcat(text,line);
        }
    } while(Module32Next(snapshot,&entry));
    CloseHandle(snapshot);
}
// samp.dll RVA: 0x00060070
void ClientConstructCrashText(BOOL includeModules) {
    char text[16384];
    CONTEXT *c=g_clientExceptionContext;
    sprintf(g_clientCrashText,"SA-MP %s\r\nException At Address: 0x%08X\r\nBase: 0x%08X\r\n\r\nRegisters:\r\nEAX: 0x%08X\tEBX: 0x%08X\tECX: 0x%08X\tEDX: 0x%08X\r\nESI: 0x%08X\tEDI: 0x%08X\tEBP: 0x%08X\tESP: 0x%08X\r\nEFLAGS: 0x%08X\r\n\r\nStack:\r\n","0.3.DL-R1",c->Eip,g_clientModule,c->Eax,c->Ebx,c->Ecx,c->Edx,c->Esi,c->Edi,c->Ebp,c->Esp,c->EFlags);
    ClientDumpMemory((const BYTE*)c->Esp,640,g_clientCrashText,TRUE);
    sprintf(text,"\r\nSCM Op: 0x%X, lDbg: %d LastRendObj: %u\r\n",g_scriptLastOpcode,g_scriptLocalDebug,(unsigned)g_clientCrashLastRenderedObject);
    strcat(g_clientCrashText,text);
    const char *version=g_gameVersion==1?"US 1.0":g_gameVersion==2?"EU 1.0":"UNKNOWN";
    sprintf(text,"\r\nGame Version: %s\r\n",version);strcat(g_clientCrashText,text);
    if(g_pNetGame) { ClientDumpNetworkState(text);strcat(g_clientCrashText,text); }
    if(includeModules) { ClientDumpLoadedModules(text);strcat(g_clientCrashText,text); }
}
// samp.dll RVA: 0x00060330
INT_PTR CALLBACK ClientExceptionDialog(HWND dialog,UINT message,WPARAM wParam,LPARAM lParam) {
    switch(message) {
    case WM_DESTROY:
        EndDialog(dialog,TRUE);ShowWindow(g_inputGameWindow,SW_HIDE);DestroyWindow(g_inputGameWindow);break;
    case WM_INITDIALOG:
        ClientConstructCrashText(FALSE);SetDlgItemTextA(dialog,1000,g_clientCrashText);
        SetForegroundWindow(GetDlgItem(dialog,101));SetFocus(GetDlgItem(dialog,1001));
        SetCursor(LoadCursorA(0,MAKEINTRESOURCEA(32512)));ShowCursor(TRUE);break;
    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case 1001: EndDialog(dialog,TRUE);break;
        case 1002:
            ClientSendCrashReport();EnableWindow(GetDlgItem(dialog,1002),FALSE);
            SetDlgItemTextA(dialog,1000,"Thanks for reporting this problem.");break;
        }
        break;
    }
    return FALSE;
}
// samp.dll RVA: 0x00060430
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS *exception) {
    g_clientExceptionContext=exception->ContextRecord;
    ShowWindow(g_inputGameWindow,SW_MINIMIZE);
    DialogBoxParamA(g_clientModule,MAKEINTRESOURCEA(101),g_inputGameWindow,ClientExceptionDialog,0);
    return EXCEPTION_EXECUTE_HANDLER;
}
