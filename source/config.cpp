#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// samp.dll RVA: 0x002aca0c
CConfig *g_pConfig;

// samp.dll RVA: 0x00065e60
CConfig::CConfig(const char *file) {
    for (int i = 0; i < MAX_ENTRIES; ++i) {
        memset(&entries[i], 0, sizeof(ConfigEntry)); occupied[i] = FALSE;
    }
    upperBound = 0;
    // Native leaves filename untouched when no nonempty filename is supplied.
    if (file && strlen(file)) { strcpy(filename, file); Load(); }
}
// samp.dll RVA: 0x00065680
CConfig::~CConfig() {
    for (int i = 0; i < MAX_ENTRIES; ++i) {
        if (occupied[i] && entries[i].stringValue) free(entries[i].stringValue);
        occupied[i] = FALSE;
    }
}
// samp.dll RVA: 0x000656c0
void CConfig::FindFirstFree() {
    int last = 0;
    for (int i = 0; i < MAX_ENTRIES; ++i) if (occupied[i]) last = i;
    upperBound = last + 1;
}
// samp.dll RVA: 0x00065710
int CConfig::GetIndex(const char *name) {
    // Native mutates the supplied key despite the const signature in references.
    _strlwr((char *)name);
    for (int i = 0; i < upperBound; ++i)
        if (occupied[i] && !strcmp(name, entries[i].name)) return i;
    return -1;
}
// samp.dll RVA: 0x000657b0
bool CConfig::DoesExist(const char *name) { return GetIndex(name) >= 0; }
// samp.dll RVA: 0x000657d0
int CConfig::CreateEntry(const char *name) {
    int index = 0;
    while (index < MAX_ENTRIES && occupied[index]) ++index;
    if (index == MAX_ENTRIES || strlen(name) >= 41) return -1;
    _strlwr((char *)name);
    strcpy(entries[index].name, name);
    entries[index].readOnly = FALSE;
    entries[index].type = VALUE_TYPE_NONE;
    occupied[index] = TRUE;
    FindFirstFree();
    return index;
}
// samp.dll RVA: 0x00065890
int CConfig::GetIntValue(const char *name) {
    int i = GetIndex(name);
    return i >= 0 && entries[i].type == VALUE_TYPE_INT ? entries[i].intValue : 0;
}
// samp.dll RVA: 0x000658c0
const char *CConfig::GetStringValue(const char *name) {
    int i = GetIndex(name);
    return i >= 0 && entries[i].type == VALUE_TYPE_STRING ? entries[i].stringValue : 0;
}
// samp.dll RVA: 0x000658f0
float CConfig::GetFloatValue(const char *name) {
    int i = GetIndex(name);
    return i >= 0 && entries[i].type == VALUE_TYPE_FLOAT ? entries[i].floatValue : 0.0f;
}
// samp.dll RVA: 0x00065920
BOOL CConfig::Free(const char *name) {
    int i = GetIndex(name);
    if (i < 0) return FALSE;
    if (entries[i].stringValue) free(entries[i].stringValue);
    memset(&entries[i], 0, sizeof(ConfigEntry)); occupied[i] = FALSE;
    FindFirstFree(); return TRUE;
}
// samp.dll RVA: 0x00065980
int CConfig::GetValueType(const char *name) {
    int i = GetIndex(name); return i < 0 ? VALUE_TYPE_NONE : entries[i].type;
}
// samp.dll RVA: 0x000659b0
CConfig::ConfigEntry *CConfig::GetEntry(int index) {
    return index >= 0 && index < MAX_ENTRIES && occupied[index] ? &entries[index] : 0;
}
// samp.dll RVA: 0x000659e0
int CConfig::GetType(const char *text) {
    if (!text || !strlen(text)) return VALUE_TYPE_NONE;
    if (text[0] == '"' && text[strlen(text) - 1] == '"') return VALUE_TYPE_STRING;
    return strchr(text, '.') ? VALUE_TYPE_FLOAT : VALUE_TYPE_INT;
}
// samp.dll RVA: 0x00065a50
BOOL CConfig::Save() {
    FILE *file = fopen(filename, "w");
    if (!file) return FALSE;
    for (int i = 0; i < MAX_ENTRIES; ++i) if (occupied[i]) {
        ConfigEntry &entry = entries[i];
        if (entry.type == VALUE_TYPE_INT) fprintf(file, "%s=%d\n", entry.name, entry.intValue);
        else if (entry.type == VALUE_TYPE_STRING && entry.stringValue) fprintf(file, "%s=\"%s\"\n", entry.name, entry.stringValue);
        else if (entry.type == VALUE_TYPE_FLOAT) fprintf(file, "%s=%f\n", entry.name, (double)entry.floatValue);
    }
    fclose(file); return TRUE;
}
// samp.dll RVA: 0x00065b00
BOOL CConfig::WriteIntValue(const char *name, int value, BOOL readOnly) {
    int i = GetIndex(name); if (i < 0) i = CreateEntry(name);
    if (i < 0) return FALSE;
    if (readOnly) entries[i].readOnly = TRUE;
    else if (entries[i].readOnly) return FALSE;
    entries[i].intValue = value; entries[i].type = VALUE_TYPE_INT;
    Save(); return TRUE;
}
// samp.dll RVA: 0x00065b60
BOOL CConfig::WriteStringValue(const char *name, const char *value, BOOL readOnly) {
    int i = GetIndex(name); if (i < 0) i = CreateEntry(name);
    if (i < 0) return FALSE;
    if (readOnly) entries[i].readOnly = TRUE;
    else if (entries[i].readOnly) return FALSE;
    entries[i].type = VALUE_TYPE_STRING;
    if (entries[i].stringValue) free(entries[i].stringValue);
    entries[i].stringValue = (char *)calloc(1, strlen(value) + 1);
    strcpy(entries[i].stringValue, value);
    Save(); return TRUE;
}
// samp.dll RVA: 0x00065c00
BOOL CConfig::WriteFloatValue(const char *name, float value, BOOL readOnly) {
    int i = GetIndex(name); if (i < 0) i = CreateEntry(name);
    if (i < 0) return FALSE;
    if (readOnly) entries[i].readOnly = TRUE;
    else if (entries[i].readOnly) return FALSE;
    entries[i].floatValue = value; entries[i].type = VALUE_TYPE_FLOAT;
    Save(); return TRUE;
}
// samp.dll RVA: 0x00065c60
void CConfig::Write(const char *name, char *value) {
    int type = GetType(value);
    if (type == VALUE_TYPE_INT) WriteIntValue(name, atol(value), FALSE);
    else if (type == VALUE_TYPE_STRING) {
        int length = (int)strlen(value);
        strncpy(value, value + 1, length - 1); value[length - 2] = 0;
        WriteStringValue(name, value, FALSE);
    } else if (type == VALUE_TYPE_FLOAT) WriteFloatValue(name, (float)atof(value), FALSE);
}
// samp.dll RVA: 0x00065cf0
BOOL CConfig::Load() {
    FILE *file = fopen(filename, "r");
    if (!file) return FALSE;
    char line[260], name[260], value[260];
    while (!feof(file)) {
        fgets(line, 256, file); // Native ignores the return value, including at EOF.
        char *p = line; int nameLength = 0, valueLength = 0;
        while (*p == ' ' || *p == '\t') ++p;
        if (!*p || *p == ';' || *p == '\n' || *p == '[') continue;
        while (*p && *p != ' ' && *p != '=' && *p != '\n' && *p != '\t' && *p != ';') name[nameLength++] = (char)toupper(*p++);
        if (!nameLength) continue;
        name[nameLength] = 0;
        while (*p == ' ' || *p == '\t') ++p;
        if (*p != '=') continue;
        do { ++p; } while (*p == ' ' || *p == '\t');
        if (!*p) continue;
        while (*p && *p != '\n') value[valueLength++] = *p++;
        if (!valueLength) continue;
        value[valueLength] = 0;
        p = value + valueLength - 1;
        while (*p == ' ' || *p == '\t' || *p == '\r') *p-- = 0;
        Write(name, value);
    }
    fclose(file); return TRUE;
}
