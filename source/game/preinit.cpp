#include "game/preinit.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x002a9270
DWORD g_gamePhysicalMegabytes;
// samp.dll RVA: 0x002a9274
DWORD g_gameStreamingBytes;
// samp.dll RVA: 0x0011ee84
const char g_gameStartupTitle[6]="title";
// samp.dll RVA: 0x0018f388
char g_gameFrameNodeError[256];
// samp.dll RVA: 0x0018f6b4
bool g_gameCustomStreamRead;
// samp.dll RVA: 0x0018f6b8
void *g_gameStreamBuffer;
// samp.dll RVA: 0x000aa060
void GameWaitUnprotect(BYTE *address,SIZE_T size,BYTE expected) {
    DWORD oldProtect;
    VirtualProtect(address,size,PAGE_EXECUTE_READWRITE,&oldProtect);
    if(*address!=expected) {
        do { Sleep(1); } while(*address!=expected);
        VirtualProtect(address,size,PAGE_EXECUTE_READWRITE,&oldProtect);
    }
}
// samp.dll RVA: 0x000aa300
BOOL GameInitializeVersionPatches() {
    BYTE *version=(BYTE*)0x747483;
    int count=0;
    while(*version!=0x89 && *version!=0xc8) {
        if(*(BYTE*)0xba6831==1)return FALSE;
        Sleep(10);if(++count>6000)return FALSE;
    }
    if(*version==0x89)g_gameVersion=1;
    else if(*version==0xc8)g_gameVersion=2;
    DWORD oldProtect;
    BYTE *skip=0;
    if(g_gameVersion==1)skip=(BYTE*)0x747483;
    else if(g_gameVersion==2)skip=(BYTE*)0x7474d3;
    if(skip) {
        VirtualProtect(skip,6,PAGE_EXECUTE_READWRITE,&oldProtect);
        *(BYTE*)0xc8d4c0=5;memset(skip,0x90,6);
    }
    VirtualProtect((void*)0x866cd8,10,PAGE_EXECUTE_READWRITE,&oldProtect);
    VirtualProtect((void*)0x866ccc,10,PAGE_EXECUTE_READWRITE,&oldProtect);
    memcpy((void*)0x866cd8,g_gameStartupTitle,6);memcpy((void*)0x866ccc,g_gameStartupTitle,6);
    VirtualProtect((void*)0x745b87,0x44,PAGE_EXECUTE_READWRITE,&oldProtect);memset((void*)0x745b87,0x90,0x44);
    VirtualProtect((void*)0x7459e1,2,PAGE_EXECUTE_READWRITE,&oldProtect);*(WORD*)0x7459e1=0x9090;
    GameWaitUnprotect((BYTE*)0x561872,0x1e,0x85);
    *(BYTE*)0x561872=0x33;*(BYTE*)0x561873=0xc0;memset((void*)0x561874,0x90,27);
    MEMORYSTATUSEX memory;memory.dwLength=sizeof(memory);GlobalMemoryStatusEx(&memory);
    g_gamePhysicalMegabytes=(DWORD)(memory.ullTotalPhys>>20);
    if(g_gamePhysicalMegabytes>4000)g_gameStreamingBytes=0x40000000;
    else if(g_gamePhysicalMegabytes>2000)g_gameStreamingBytes=0x20000000;
    else if(g_gamePhysicalMegabytes>1000)g_gameStreamingBytes=0x10000000;
    else if(g_gamePhysicalMegabytes>500)g_gameStreamingBytes=0x8000000;
    else g_gameStreamingBytes=0x6000000;
    VirtualProtect((void*)0x5b8e6a,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x5b8e6a=g_gameStreamingBytes;
    GameWaitUnprotect((BYTE*)0x4083c0,1,0xb8);*(BYTE*)0x4083c0=0xc3;
    VirtualProtect((void*)0x590099,5,PAGE_EXECUTE_READWRITE,&oldProtect);memset((void*)0x590099,0x90,5);
    VirtualProtect((void*)0x53e94c,1,PAGE_EXECUTE_READWRITE,&oldProtect);*(BYTE*)0x53e94c=2;
    VirtualProtect((void*)0x731f60,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x731f60=20000;
    VirtualProtect((void*)0x72fad6,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x72fad6=(DWORD)GameFrameNodeNameHook;
    VirtualProtect((void*)0x6fa996,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x6fa996=(DWORD)GameDataReadHook;
    return TRUE;
}
// samp.dll RVA: 0x000a5c10
DWORD __cdecl GameFrameNodeNameHook(void *arg0,DWORD length,void *arg2) {
    if(length>23) {
        if(g_pGame) {
            _snprintf(g_gameFrameNodeError,256,"SA-MP: FrameNode name was too long (%u). SA-MP must exit.",length);
            ShowWindow(g_inputGameWindow,SW_MINIMIZE);
            MessageBoxA(0,g_gameFrameNodeError,"SA-MP",MB_ICONWARNING);
        }
        __asm mov esp,0
        __asm int 3
    }
    return ((DWORD(__cdecl*)(void*,DWORD,void*))0x72fa50)(arg0,length,arg2);
}
// samp.dll RVA: 0x000a5c90
DWORD __cdecl GameDataReadHook(void *arg0,DWORD length,void *arg2) {
    if(g_gameCustomStreamRead) {
        g_gameStreamBuffer=calloc(1,length);
        GameReadStream(arg0,g_gameStreamBuffer,length);
        GameParseStreamData(g_gameStreamBuffer,length,arg2);
        if(g_gameStreamBuffer) { free(g_gameStreamBuffer);g_gameStreamBuffer=0; }
        return (DWORD)arg0;
    }
    return ((DWORD(__cdecl*)(void*,DWORD,void*))0x6f9fd0)(arg0,length,arg2);
}
// samp.dll RVA: 0x000b1a40
DWORD GameReadStream(void *stream,void *buffer,DWORD length) {
    DWORD address=g_gameVersion==1?0x7ec9d0:0x7eca10;
    return ((DWORD(__cdecl*)(void*,void*,DWORD))address)(stream,buffer,length);
}
