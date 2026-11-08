#ifndef SAMP_RECOVERED_CONFIG_H
#define SAMP_RECOVERED_CONFIG_H
#include <static_assert.h>
#include <stddef.h>
#include <windows.h>
#pragma pack(push, 1)
class CConfig
{
public:
    enum
    {
        MAX_ENTRIES = 512,
        MAX_ENTRY_NAME = 40
    };
    enum ValueType
    {
        VALUE_TYPE_NONE,
        VALUE_TYPE_INT,
        VALUE_TYPE_STRING,
        VALUE_TYPE_FLOAT
    };
    struct ConfigEntry
    {
        char name[41];
        BOOL readOnly;
        int type;
        int intValue;
        float floatValue;
        char* stringValue;
    };
    ConfigEntry entries[MAX_ENTRIES];
    BOOL occupied[MAX_ENTRIES];
    char filename[261];
    int upperBound; // Native FindFirstFree computes highest occupied index + 1.
    // 0x00065e60
    CConfig(const char*);
    // 0x00065680
    ~CConfig();
    // 0x000656c0
    void FindFirstFree();
    // 0x00065710
    int GetIndex(const char*);
    // 0x000657b0
    bool DoesExist(const char*);
    // 0x000657d0
    int CreateEntry(const char*);
    // 0x00065890
    int GetIntValue(const char*);
    // 0x000658c0
    const char* GetStringValue(const char*);
    // 0x000658f0
    float GetFloatValue(const char*);
    // 0x00065920
    BOOL Free(const char*);
    // 0x00065980
    int GetValueType(const char*);
    // 0x000659b0
    ConfigEntry* GetEntry(int);
    // 0x000659e0
    int GetType(const char*);
    // 0x00065a50
    BOOL Save();
    // 0x00065b00
    BOOL WriteIntValue(const char*, int, BOOL = FALSE);
    // 0x00065b60
    BOOL WriteStringValue(const char*, const char*, BOOL = FALSE);
    // 0x00065c00
    BOOL WriteFloatValue(const char*, float, BOOL = FALSE);
    // 0x00065c60
    void Write(const char*, char*);
    // 0x00065cf0
    BOOL Load();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CConfig::ConfigEntry) == 0x3d);
STATIC_ASSERT(sizeof(CConfig) == 0x8309);
STATIC_ASSERT(offsetof(CConfig, occupied) == 0x7a00);
STATIC_ASSERT(offsetof(CConfig, filename) == 0x8200);
STATIC_ASSERT(offsetof(CConfig, upperBound) == 0x8305);
// 0x002aca0c
extern CConfig* g_pConfig;

#pragma pack(push, 1)
struct ClientSettings
{
    BOOL debug, online;
    DWORD unknown08;
    char password[257], host[257], port[257], nickname[257], debugScript[257];
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(ClientSettings) == 1297);
STATIC_ASSERT(offsetof(ClientSettings, host) == 0x10d);
STATIC_ASSERT(offsetof(ClientSettings, debugScript) == 0x410);
// 0x002abe78
extern ClientSettings g_clientSettings;

namespace Settings
{
// 0x000c5c90
void Initialize();
// 0x000c5590
void GetFromCommandLine(const char* line, char* output);
// 0x000c55e0
void GetFromQuotes(const char* line, char* output);
} // namespace Settings

// Native start addresses; buffer backing windows are bounded by the next known native buffer.
// Overflow adjacency is not established by these declarations.
// 0x002ac498
extern char g_clientConfigFilename[520];

// 0x002ac6a0
extern char g_clientChatFilename[264];

// Observed native buffers include the 261st byte cleared by startup.
// 0x002ac7a8
extern char g_clientDataDirectory[261];

// 0x002ac8b0
extern char g_clientCacheDirectory[261];

// 0x000c51a0
const char* ClientGetDataDirectory();

// 0x000c51b0
const char* ClientGetCacheDirectory();

// Native _stat success test accepts any existing file type.
// 0x000b5900
BOOL ClientPathExists(const char* path);

// 0x000c51c0
void ClientInitializeCacheDirectory();

// 0x000c5340
void ClientInitializePaths();

#endif
