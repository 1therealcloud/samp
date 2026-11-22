#ifndef SAMPSRV_CONSOLE_H
#define SAMPSRV_CONSOLE_H

#include "system.h"

#include <map>
#include <string>

enum CON_VARTYPE { CON_VARTYPE_FLOAT, CON_VARTYPE_INT, CON_VARTYPE_BOOL, CON_VARTYPE_STRING };

#define CON_VARFLAG_DEBUG 1
#define CON_VARFLAG_READONLY 2
#define CON_VARFLAG_RULE 4

typedef void (*VARCHANGEFUNC)();

struct ConsoleVariable_s
{
    CON_VARTYPE VarType;
    unsigned long VarFlags;
    void* VarPtr;
    VARCHANGEFUNC VarChangeFunc;
};

typedef std::map<std::string, ConsoleVariable_s*> StringConvarMap;

class CConsole
{
public:
    StringConvarMap ConsoleVariables;

    CConsole();
    ~CConsole();
    ConsoleVariable_s* FindVariable(const char* pVarName);
    void PrintVariableList();
    void AddVariable(const char* pVarName, CON_VARTYPE VarType, unsigned long VarFlags, void* VarPtr,
                     VARCHANGEFUNC VarChangeFunc = 0);
    char* AddStringVariable(const char* pVarName, unsigned long VarFlags, const char* pInitStr,
                            VARCHANGEFUNC VarChangeFunc = 0);
    void RemoveVariable(const char* pVarName);
    void ModifyVariableFlags(const char* pVarName, unsigned long VarFlags);
    char* GetStringVariable(const char* pVarName);
    void SetStringVariable(const char* pVarName, const char* value);
    float GetFloatVariable(const char* pVarName);
    void SetFloatVariable(const char* pVarName, float value);
    int GetIntVariable(const char* pVarName);
    void SetIntVariable(const char* pVarName, int value);
    bool GetBoolVariable(const char* pVarName);
    void SetBoolVariable(const char* pVarName, bool value);
    void SendRules(SOCKET socket, char* data, const sockaddr_in* to, int tolen);
    void Execute(const char* pExecLine);
};

extern CConsole* pConsole;
void logprintf(const char* format, ...);
void RequestServerExit();

#endif
