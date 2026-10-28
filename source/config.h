#ifndef SAMP_RECOVERED_CONFIG_H
#define SAMP_RECOVERED_CONFIG_H
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
    // samp.dll RVA: 0x00065e60
    CConfig(const char*);
    // samp.dll RVA: 0x00065680
    ~CConfig();
    // samp.dll RVA: 0x000656c0
    void FindFirstFree();
    // samp.dll RVA: 0x00065710
    int GetIndex(const char*);
    // samp.dll RVA: 0x000657b0
    bool DoesExist(const char*);
    // samp.dll RVA: 0x000657d0
    int CreateEntry(const char*);
    // samp.dll RVA: 0x00065890
    int GetIntValue(const char*);
    // samp.dll RVA: 0x000658c0
    const char* GetStringValue(const char*);
    // samp.dll RVA: 0x000658f0
    float GetFloatValue(const char*);
    // samp.dll RVA: 0x00065920
    BOOL Free(const char*);
    // samp.dll RVA: 0x00065980
    int GetValueType(const char*);
    // samp.dll RVA: 0x000659b0
    ConfigEntry* GetEntry(int);
    // samp.dll RVA: 0x000659e0
    int GetType(const char*);
    // samp.dll RVA: 0x00065a50
    BOOL Save();
    // samp.dll RVA: 0x00065b00
    BOOL WriteIntValue(const char*, int, BOOL = FALSE);
    // samp.dll RVA: 0x00065b60
    BOOL WriteStringValue(const char*, const char*, BOOL = FALSE);
    // samp.dll RVA: 0x00065c00
    BOOL WriteFloatValue(const char*, float, BOOL = FALSE);
    // samp.dll RVA: 0x00065c60
    void Write(const char*, char*);
    // samp.dll RVA: 0x00065cf0
    BOOL Load();
};
#pragma pack(pop)
typedef char ConfigEntrySize[(sizeof(CConfig::ConfigEntry) == 0x3d) ? 1 : -1];
typedef char ConfigSize[(sizeof(CConfig) == 0x8309) ? 1 : -1];
typedef char ConfigMapOffset[(offsetof(CConfig, occupied) == 0x7a00) ? 1 : -1];
typedef char ConfigFilenameOffset[(offsetof(CConfig, filename) == 0x8200) ? 1 : -1];
typedef char ConfigBoundOffset[(offsetof(CConfig, upperBound) == 0x8305) ? 1 : -1];
// samp.dll RVA: 0x002aca0c
extern CConfig* g_pConfig;
#endif
