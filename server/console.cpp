#include "console.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

static char* LowerCase(char* value)
{
#ifdef _WIN32
    return _strlwr(value);
#else
    for (char* ch = value; *ch; ++ch)
        *ch = (char)tolower((unsigned char)*ch);
    return value;
#endif
}

static char* CopyString(const char* value)
{
#ifdef _WIN32
    return _strdup(value);
#else
    return strdup(value);
#endif
}

// 0x00490260
char* strrtrim(char* text)
{
    size_t length = strlen(text);
    while (length && (text[length - 1] == ' ' || text[length - 1] == '\t'))
        text[--length] = '\0';
    return text;
}

// 0x00491d70
CConsole::CConsole()
{
}

// 0x00491ce0
CConsole::~CConsole()
{
    for (StringConvarMap::iterator it = ConsoleVariables.begin(); it != ConsoleVariables.end(); ++it)
    {
        if (it->second->VarType == CON_VARTYPE_STRING)
            free(it->second->VarPtr);
        delete it->second;
    }
}

// 0x00490aa0
ConsoleVariable_s* CConsole::FindVariable(const char* pVarName)
{
    char name[256];
    strncpy(name, pVarName, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    StringConvarMap::iterator it = ConsoleVariables.find(LowerCase(name));
    return it == ConsoleVariables.end() ? NULL : it->second;
}

// 0x00491ab0
void CConsole::AddVariable(const char* pVarName, CON_VARTYPE VarType, unsigned long VarFlags,
                           void* VarPtr, VARCHANGEFUNC VarChangeFunc)
{
    char name[256];
    strncpy(name, pVarName, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    LowerCase(name);
    if (FindVariable(name))
    {
        logprintf("Unable to add console variable '%s'. Already exists.", name);
        return;
    }
    ConsoleVariable_s* variable = new ConsoleVariable_s;
    variable->VarType = VarType;
    variable->VarFlags = VarFlags;
    variable->VarPtr = VarPtr;
    variable->VarChangeFunc = VarChangeFunc;
    if (VarChangeFunc)
        VarChangeFunc();
    ConsoleVariables.insert(StringConvarMap::value_type(name, variable));
}

// 0x004905e0
void CConsole::PrintVariableList()
{
    logprintf("Console Variables:");
    for (StringConvarMap::const_iterator it = ConsoleVariables.begin(); it != ConsoleVariables.end(); ++it)
    {
        const ConsoleVariable_s& variable = *it->second;
        char value[1040];
        switch (variable.VarType)
        {
            case CON_VARTYPE_FLOAT: sprintf(value, "%f  (float)", *(float*)variable.VarPtr); break;
            case CON_VARTYPE_INT: sprintf(value, "%d  (int)", *(int*)variable.VarPtr); break;
            case CON_VARTYPE_BOOL: sprintf(value, "%d  (bool)", *(bool*)variable.VarPtr); break;
            case CON_VARTYPE_STRING: sprintf(value, "\"%.1023s\"  (string)", (char*)variable.VarPtr); break;
            default: continue;
        }
        logprintf("  %s\t%s= %s%s%s%s", it->first.c_str(), it->first.size() < 6 ? "\t" : "", value,
                  variable.VarFlags & CON_VARFLAG_READONLY ? " (read-only)" : "",
                  variable.VarFlags & CON_VARFLAG_DEBUG ? " (debug)" : "",
                  variable.VarFlags & CON_VARFLAG_RULE ? " (rule)" : "");
    }
    logprintf("");
}

// 0x00491c40
char* CConsole::AddStringVariable(const char* pVarName, unsigned long VarFlags, const char* pInitStr,
                                 VARCHANGEFUNC VarChangeFunc)
{
    char* value = CopyString(pInitStr ? pInitStr : "");
    if (!value)
        return NULL;
    if (FindVariable(pVarName))
    {
        logprintf("Unable to add console variable '%s'. Already exists.", pVarName);
        free(value);
        return NULL;
    }
    AddVariable(pVarName, CON_VARTYPE_STRING, VarFlags, value, VarChangeFunc);
    return value;
}

// Original address to verify.
void CConsole::RemoveVariable(const char* pVarName)
{
    char name[256];
    strncpy(name, pVarName, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    StringConvarMap::iterator it = ConsoleVariables.find(LowerCase(name));
    if (it == ConsoleVariables.end())
        return;
    if (it->second->VarType == CON_VARTYPE_STRING)
        free(it->second->VarPtr);
    delete it->second;
    ConsoleVariables.erase(it);
}

// 0x00490b80
void CConsole::ModifyVariableFlags(const char* pVarName, unsigned long VarFlags)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    if (variable)
        variable->VarFlags = VarFlags;
}

// 0x00490ba0
char* CConsole::GetStringVariable(const char* pVarName)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    return variable && variable->VarType == CON_VARTYPE_STRING ? (char*)variable->VarPtr : NULL;
}

// 0x00490bc0
void CConsole::SetStringVariable(const char* pVarName, const char* value)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    if (!variable || variable->VarType != CON_VARTYPE_STRING)
        return;
    char* replacement = CopyString(value);
    if (!replacement)
        return;
    free(variable->VarPtr);
    variable->VarPtr = replacement;
}

// 0x00490c30
float CConsole::GetFloatVariable(const char* pVarName)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    return variable && variable->VarType == CON_VARTYPE_FLOAT ? *(float*)variable->VarPtr : 0.0f;
}

// 0x00490c60
void CConsole::SetFloatVariable(const char* pVarName, float value)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    if (variable && variable->VarType == CON_VARTYPE_FLOAT)
        *(float*)variable->VarPtr = value;
}

// 0x00490c80
int CConsole::GetIntVariable(const char* pVarName)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    return variable && variable->VarType == CON_VARTYPE_INT ? *(int*)variable->VarPtr : 0;
}

// 0x00490ca0
void CConsole::SetIntVariable(const char* pVarName, int value)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    if (variable && variable->VarType == CON_VARTYPE_INT)
        *(int*)variable->VarPtr = value;
}

// 0x00490cc0
bool CConsole::GetBoolVariable(const char* pVarName)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    return variable && variable->VarType == CON_VARTYPE_BOOL && *(bool*)variable->VarPtr;
}

// Original address to verify.
void CConsole::SetBoolVariable(const char* pVarName, bool value)
{
    ConsoleVariable_s* variable = FindVariable(pVarName);
    if (variable && variable->VarType == CON_VARTYPE_BOOL)
        *(bool*)variable->VarPtr = value;
}

// 0x00490770
void CConsole::SendRules(SOCKET socket, char* data, const sockaddr_in* to, int tolen)
{
    std::vector<char> response(data, data + 11);
    response.resize(13);
    unsigned short count = 0;
    for (StringConvarMap::const_iterator it = ConsoleVariables.begin(); it != ConsoleVariables.end(); ++it)
    {
        const ConsoleVariable_s& variable = *it->second;
        if (!(variable.VarFlags & CON_VARFLAG_RULE))
            continue;
        char buffer[64];
        const char* value = buffer;
        switch (variable.VarType)
        {
            case CON_VARTYPE_FLOAT: sprintf(buffer, "%f", *(float*)variable.VarPtr); break;
            case CON_VARTYPE_INT: sprintf(buffer, "%d", *(int*)variable.VarPtr); break;
            case CON_VARTYPE_BOOL: sprintf(buffer, "%d", *(bool*)variable.VarPtr); break;
            case CON_VARTYPE_STRING: value = (const char*)variable.VarPtr; break;
            default: continue;
        }
        const unsigned char nameLength = (unsigned char)it->first.size();
        const unsigned char valueLength = (unsigned char)strlen(value);
        if (response.size() + nameLength + valueLength + 2 > 65507)
            break;
        response.push_back((char)nameLength);
        response.insert(response.end(), it->first.c_str(), it->first.c_str() + nameLength);
        response.push_back((char)valueLength);
        response.insert(response.end(), value, value + valueLength);
        ++count;
    }
    response[11] = (char)count;
    response[12] = (char)(count >> 8);
    sendto(socket, &response[0], (int)response.size(), 0, (const sockaddr*)to, tolen);
}

// 0x00491150
void con_exec()
{
    const char* arg = strtok(NULL, " ");
    if (!arg)
    {
        logprintf("Usage:");
        logprintf("  exec <filename>");
        return;
    }

    char line[1025];
    sprintf(line, "%s.cfg", arg);
    FILE* file = fopen(line, "r");
    if (!file)
    {
        logprintf("Unable to exec file '%s'.", line);
        return;
    }

    while (fgets(line, 1024, file))
    {
        size_t length = strlen(line);
        if (length && line[length - 1] == '\n')
            line[--length] = '\0';
        if (length && line[length - 1] == '\r')
            line[--length] = '\0';
        for (size_t i = 0; i + 1 < length; ++i)
        {
            if ((line[i] == '/' && line[i + 1] == '/') || line[i] == '#')
            {
                line[i] = '\0';
                length = i;
                break;
            }
        }
        if (length <= 2 || line[0] == '/' || line[1] == '/')
            continue;

        char name[1025];
        const size_t nameLength = strcspn(line, " ");
        memcpy(name, line, nameLength);
        name[nameLength] = '\0';
        if (pConsole->FindVariable(name))
            pConsole->Execute(line);
    }
    fclose(file);
}

// 0x0048fbd0
void con_echo()
{
}

// 0x00490200
void con_cmdlist()
{
}

// 0x004909c0
void con_varlist()
{
}

// 0x0048fbc0
void con_exit()
{
}

// 0x0048fc00
void con_kick()
{
}

// 0x0048fca0
void con_ban()
{
}

// 0x0048fea0
void con_gmx()
{
}

// 0x0048fed0
void con_changemode()
{
}

// 0x0048ff00
void con_say()
{
}

// 0x0048ff50
void con_reloadbans()
{
}

// 0x0048ff60
void con_reloadlog()
{
}

// 0x0048ff70
void con_players()
{
}

// 0x0048fe10
void con_banip()
{
}

// 0x0048fe60
void con_unbanip()
{
}

// 0x00490020
void con_gravity()
{
}

// 0x00490050
void con_weather()
{
}

// 0x00490080
void con_loadfs()
{
}

// 0x00490190
void con_unloadfs()
{
}

// 0x004900f0
void con_reloadfs()
{
}

#define CON_CMDFLAG_DEBUG 1

#pragma pack(push, 1)
struct ConsoleCommand_s
{
    char CmdName[255];
    unsigned long CmdFlags;
    void (*CmdFunc)();
};
#pragma pack(pop)

// 0x004ea848
static const ConsoleCommand_s ConsoleCommands[] =
{
    {"echo", 0, con_echo},
    {"exec", 0, con_exec},
    {"cmdlist", 0, con_cmdlist},
    {"varlist", 0, con_varlist},
    {"exit", 0, con_exit},
    {"kick", 0, con_kick},
    {"ban", 0, con_ban},
    {"gmx", 0, con_gmx},
    {"changemode", 0, con_changemode},
    {"say", 0, con_say},
    {"reloadbans", 0, con_reloadbans},
    {"reloadlog", 0, con_reloadlog},
    {"players", 0, con_players},
    {"banip", 0, con_banip},
    {"unbanip", 0, con_unbanip},
    {"gravity", 0, con_gravity},
    {"weather", 0, con_weather},
    {"loadfs", 0, con_loadfs},
    {"unloadfs", 0, con_unloadfs},
    {"reloadfs", 0, con_reloadfs},
};

// 0x00490ce0
void CConsole::Execute(const char* pExecLine)
{
    if (!pExecLine)
        return;
    char line[256];
    strncpy(line, pExecLine, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';
    char* cmd = strtok(line, " ");
    if (!cmd)
        return;
    LowerCase(cmd);
    for (size_t i = 0; i < sizeof(ConsoleCommands) / sizeof(ConsoleCommands[0]); ++i)
    {
        const ConsoleCommand_s& command = ConsoleCommands[i];
        if (strcmp(cmd, command.CmdName))
            continue;
#ifndef _DEBUG
        if (command.CmdFlags & CON_CMDFLAG_DEBUG)
        {
            logprintf("Unable to execute command '%s'. Debug mode required.", command.CmdName);
            return;
        }
#endif
        command.CmdFunc();
        return;
    }
    ConsoleVariable_s* variable = FindVariable(cmd);
    if (!variable)
    {
        logprintf("Unknown command or variable:\n  %s", cmd);
        return;
    }
#ifndef _DEBUG
    if (variable->VarFlags & CON_VARFLAG_DEBUG)
    {
        logprintf("Unable to access variable '%s'. Debug mode required.", cmd);
        return;
    }
#endif
    const bool readOnly = (variable->VarFlags & CON_VARFLAG_READONLY) != 0;
    char* arg = strtok(NULL, " ");
    if (!arg || readOnly)
    {
        const char* suffix = readOnly ? ", read-only" : "";
        switch (variable->VarType)
        {
            case CON_VARTYPE_FLOAT: logprintf("%s = %f  (float%s)", cmd, *(float*)variable->VarPtr, suffix); break;
            case CON_VARTYPE_INT: logprintf("%s = %d  (int%s)", cmd, *(int*)variable->VarPtr, suffix); break;
            case CON_VARTYPE_BOOL: logprintf("%s = %d  (bool%s)", cmd, *(bool*)variable->VarPtr, suffix); break;
            case CON_VARTYPE_STRING: logprintf("%s = \"%s\"  (string%s)", cmd, (char*)variable->VarPtr, suffix); break;
        }
        return;
    }
    switch (variable->VarType)
    {
        case CON_VARTYPE_FLOAT: *(float*)variable->VarPtr = (float)atof(arg); break;
        case CON_VARTYPE_INT: *(int*)variable->VarPtr = atoi(arg); break;
        case CON_VARTYPE_BOOL: *(bool*)variable->VarPtr = atoi(arg) > 0; break;
        case CON_VARTYPE_STRING:
        {
            char* ext = strtok(NULL, "");
            char* value = (char*)malloc(strlen(arg) + (ext ? strlen(ext) + 2 : 1));
            if (!value)
                return;
            strcpy(value, arg);
            if (ext)
            {
                strcat(value, " ");
                strcat(value, strrtrim(ext));
            }
            free(variable->VarPtr);
            variable->VarPtr = value;
            break;
        }
    }
    if (variable->VarChangeFunc)
        variable->VarChangeFunc();
}
