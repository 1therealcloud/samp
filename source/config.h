#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <windows.h>

namespace Settings
{
void Initialize();

void GetFromCommandLine(const char* line, char* output);

void GetFromQuotes(const char* line, char* output);
} // namespace Settings

extern char g_clientConfigFilename[520];
extern char g_clientChatFilename[264];

extern char g_clientDataDirectory[261];
extern char g_clientCacheDirectory[261];

const char* ClientGetDataDirectory();
const char* ClientGetCacheDirectory();

BOOL ClientPathExists(const char* path);
void ClientInitializeCacheDirectory();
void ClientInitializePaths();

#pragma pack(push, 1)
struct ClientSettings
{
    BOOL debug, online;
    DWORD unknown08;
    char password[257], host[257], port[257], nickname[257], debugScript[257];
};
#pragma pack(pop)
static_assert(sizeof(ClientSettings) == 1297);
static_assert(offsetof(ClientSettings, host) == 0x10d);
static_assert(offsetof(ClientSettings, debugScript) == 0x410);

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

    CConfig(const char*);
    ~CConfig();

    void FindFirstFree();
    int GetIndex(const char*);
    bool DoesExist(const char*);
    int CreateEntry(const char*);

    int GetIntValue(const char*);
    const char* GetStringValue(const char*);
    float GetFloatValue(const char*);

    int GetValueType(const char*);
    ConfigEntry* GetEntry(int);
    int GetType(const char*);

    BOOL Write(const char*, int, BOOL = FALSE);
    BOOL Write(const char*, const char*, BOOL = FALSE);
    BOOL Write(const char*, float, BOOL = FALSE);
    void WriteRaw(const char*, char*);

    BOOL Free(const char*);
    BOOL Save();
    BOOL Load();
};
#pragma pack(pop)
static_assert(sizeof(CConfig::ConfigEntry) == 0x3d);
static_assert(sizeof(CConfig) == 0x8309);
static_assert(offsetof(CConfig, occupied) == 0x7a00);
static_assert(offsetof(CConfig, filename) == 0x8200);
static_assert(offsetof(CConfig, upperBound) == 0x8305);

extern CConfig* g_pConfig;
extern ClientSettings g_clientSettings;

#endif // CONFIG_H
