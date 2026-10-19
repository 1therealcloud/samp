#include "client/device_setup.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x002ac7a8
char g_clientDataDirectory[261];
// samp.dll RVA: 0x002ac8b0
char g_clientCacheDirectory[261];
// samp.dll RVA: 0x000b5900
BOOL ClientPathExists(const char *path) {
    struct _stat info;return _stat(path,&info)==0;
}
// samp.dll RVA: 0x000c51c0
void ClientInitializeCacheDirectory() {
    memset(g_clientCacheDirectory,0,261);
    BYTE configured[261];memset(configured,0,sizeof(configured));
    DWORD length=260,type;HKEY key;
    sprintf(g_clientCacheDirectory,"%s\\cache",g_clientDataDirectory);
    if(RegOpenKeyExA(HKEY_CURRENT_USER,"Software\\SAMP",0,KEY_READ,&key)==ERROR_SUCCESS) {
        if(RegQueryValueExA(key,"model_cache",0,&type,configured,&length)==ERROR_SUCCESS)
            strncpy(g_clientCacheDirectory,(const char*)configured,261);
    }
    // Native does not close key and does not validate value type or copied termination.
    if(!ClientPathExists(g_clientCacheDirectory))CreateDirectoryA(g_clientCacheDirectory,0);
    char local[268];sprintf(local,"%s\\local",g_clientCacheDirectory);
    if(!ClientPathExists(local))CreateDirectoryA(local,0);
}
// samp.dll RVA: 0x000c5340
void ClientInitializePaths() {
    memset(g_clientDataDirectory,0,261);
    const char *documents=(const char*)0xc92368; // GTA SA virtual address.
    if(!strlen(documents)){GetCurrentDirectoryA(261,g_clientDataDirectory);return;}
    sprintf(g_clientDataDirectory,"%s\\SAMP",documents);
    if(!ClientPathExists(g_clientDataDirectory))CreateDirectoryA(g_clientDataDirectory,0);
    char screens[264];sprintf(screens,"%s\\screens",g_clientDataDirectory);
    if(!ClientPathExists(screens))CreateDirectoryA(screens,0);
    ClientInitializeCacheDirectory();
}
