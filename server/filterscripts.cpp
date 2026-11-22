#include "filterscripts.h"
#include "scrcore.h"

#include <sstream>
#include <string.h>

CFilterScripts* pFilterScripts;

// 0x0046a990 (Windows)
CFilterScripts::CFilterScripts()
{
    for (int i = 0; i < MaxFilterScripts; ++i)
    {
        memset(&m_scripts[i].amx, 0, sizeof(AMX));
        m_scripts[i].active = false;
    }
}

CFilterScripts::~CFilterScripts()
{
    UnloadFilterScripts();
}

void CFilterScripts::LoadFilterScripts(const char* list)
{
    std::istringstream scripts(list ? list : "");
    std::string name;
    while (scripts >> name)
        LoadFilterScript(name.c_str());
}

// 0x0046a9d0 (Windows)
bool CFilterScripts::LoadFilterScript(const char* name)
{
    int slot = -1;
    for (int i = 0; i < MaxFilterScripts; ++i)
    {
        if (m_scripts[i].name == name)
            return false;
        if (slot == -1 && m_scripts[i].name.empty())
            slot = i;
    }
    if (slot == -1)
        return false;

    FilterScript& script = m_scripts[slot];
    const std::string path = std::string("filterscripts/") + name + ".amx";
    script.name = name;
    if (!LoadScript(&script.amx, path.c_str()))
    {
        script.name.clear();
        return false;
    }

    script.active = true;
    cell result = 0;
    CallScriptPublic(&script.amx, "OnFilterScriptInit", result);
    return true;
}

// 0x0046acc0 (Windows)
void CFilterScripts::RemoveFilterScript(int index)
{
    FilterScript& script = m_scripts[index];
    cell result = 0;
    CallScriptPublic(&script.amx, "OnFilterScriptExit", result);
    script.active = false;
    UnloadScript(&script.amx);
    script.name.clear();
}

// 0x0046d1c0 (Windows)
bool CFilterScripts::UnloadOneFilterScript(const char* name)
{
    for (int i = 0; i < MaxFilterScripts; ++i)
    {
        if (m_scripts[i].active && m_scripts[i].name == name)
        {
            RemoveFilterScript(i);
            return true;
        }
    }
    return false;
}

// 0x0046d190 (Windows)
void CFilterScripts::UnloadFilterScripts()
{
    for (int i = 0; i < MaxFilterScripts; ++i)
        if (m_scripts[i].active)
            RemoveFilterScript(i);
}

// 0x0046ada0 (Windows)
int CFilterScripts::CallPublic(const char* name)
{
    return CallPublic(name, 0, 0, 0);
}

int CFilterScripts::CallPublic(const char* name, const cell* arguments, int count, cell defaultResult)
{
    cell result = defaultResult;
    for (int i = 0; i < MaxFilterScripts; ++i)
    {
        if (m_scripts[i].active && CallScriptPublic(&m_scripts[i].amx, name, result, arguments, count) && !result)
            break;
    }
    return result;
}

// 0x0046af20 (Windows)
void CFilterScripts::OnGameModeInit()
{
    CallPublic("OnGameModeInit");
}

// 0x0046af90 (Windows)
void CFilterScripts::OnGameModeExit()
{
    CallPublic("OnGameModeExit");
}
