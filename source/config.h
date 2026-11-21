#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

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
    // 0x000663e0
    CConfig(const char*);
    // 0x00065c00
    ~CConfig();
    // 0x00065c40
    void FindFirstFree();
    // 0x00065c90
    int GetIndex(const char*);
    // 0x00065d30
    bool DoesExist(const char*);
    // 0x00065d50
    int CreateEntry(const char*);
    // 0x00065e10
    int GetIntValue(const char*);
    // 0x00065e40
    const char* GetStringValue(const char*);
    // 0x00065e70
    float GetFloatValue(const char*);
    // 0x00065ea0
    BOOL Free(const char*);
    // 0x00065f00
    int GetValueType(const char*);
    // 0x00065f30
    ConfigEntry* GetEntry(int);
    // 0x00065f60
    int GetType(const char*);
    // 0x00065fd0
    BOOL Save();
    // 0x00066080
    BOOL WriteIntValue(const char*, int, BOOL = FALSE);
    // 0x000660e0
    BOOL WriteStringValue(const char*, const char*, BOOL = FALSE);
    // 0x00066180
    BOOL WriteFloatValue(const char*, float, BOOL = FALSE);
    // 0x000661e0
    void Write(const char*, char*);
    // 0x00066270
    BOOL Load();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CConfig::ConfigEntry) == 0x3d);
STATIC_ASSERT(sizeof(CConfig) == 0x8309);
STATIC_ASSERT(offsetof(CConfig, occupied) == 0x7a00);
STATIC_ASSERT(offsetof(CConfig, filename) == 0x8200);
STATIC_ASSERT(offsetof(CConfig, upperBound) == 0x8305);
// 0x0026eb7c
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
// 0x0026dfe8
extern ClientSettings g_clientSettings;

namespace Settings
{
// 0x000c45b0
void Initialize();
// 0x000c3ec0
void GetFromCommandLine(const char* line, char* output);
// 0x000c3f10
void GetFromQuotes(const char* line, char* output);
} // namespace Settings

// Native start addresses; buffer backing windows are bounded by the next known native buffer.
// Overflow adjacency is not established by these declarations.
// R5 address to verify
extern char g_clientConfigFilename[520];

// R5 address to verify
extern char g_clientChatFilename[264];

// Observed native buffers include the 261st byte cleared by startup.
// 0x0026e918
extern char g_clientDataDirectory[261];

// R5 address to verify
extern char g_clientCacheDirectory[261];

// R5 address to verify
const char* ClientGetDataDirectory();

// R5 address to verify
const char* ClientGetCacheDirectory();

// Native _stat success test accepts any existing file type.
// 0x000b5dc0
BOOL ClientPathExists(const char* path);

// 0x000c3af0
void ClientInitializeCacheDirectory();

// 0x000c3c70
void ClientInitializePaths();

#endif // CONFIG_H
