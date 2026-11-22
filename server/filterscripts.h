#ifndef SAMPSRV_FILTERSCRIPTS_H
#define SAMPSRV_FILTERSCRIPTS_H

#include "amx.h"

#include <string>

class CFilterScripts
{
public:
    CFilterScripts();
    ~CFilterScripts();
    void LoadFilterScripts(const char* list);
    bool LoadFilterScript(const char* name);
    bool UnloadOneFilterScript(const char* name);
    void UnloadFilterScripts();
    int CallPublic(const char* name);
    void OnGameModeInit();
    void OnGameModeExit();

private:
    CFilterScripts(const CFilterScripts&);
    CFilterScripts& operator=(const CFilterScripts&);
    void RemoveFilterScript(int index);

    enum { MaxFilterScripts = 16 };
    struct FilterScript
    {
        AMX amx;
        std::string name;
        bool active;
    };
    FilterScript m_scripts[MaxFilterScripts];
};

extern CFilterScripts* pFilterScripts;

#endif
