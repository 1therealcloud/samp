#include "gamemodes.h"
#include "filterscripts.h"
#include "plugins.h"
#include "scrcore.h"

#include <string.h>

CGameMode* pGameMode;

CGameMode::CGameMode() : m_active(false), m_sleeping(false), m_sleepTime(0.0f)
{
    memset(&m_amx, 0, sizeof(m_amx));
}

CGameMode::~CGameMode()
{
    Unload();
}

// 0x0046f910 (Windows)
bool CGameMode::Load(const char* filename)
{
    Unload();
    if (!LoadScript(&m_amx, filename))
        return false;

    m_active = true;
    CallPublic("OnGameModeInit");
    pFilterScripts->OnGameModeInit();
    Run(AMX_EXEC_MAIN);
    return true;
}

// 0x0046da90 (Windows)
void CGameMode::Unload()
{
    if (!m_active)
        return;

    CallPublic("OnGameModeExit");
    pFilterScripts->OnGameModeExit();
    m_active = false;
    m_sleeping = false;
    UnloadScript(&m_amx);
}

void CGameMode::Run(int index)
{
    cell result = 0;
    const int error = amx_Exec(&m_amx, &result, index);
    m_sleeping = error == AMX_ERR_SLEEP;
    if (m_sleeping)
        m_sleepTime = static_cast<float>(result) / 1000.0f;
    else if (error != AMX_ERR_INDEX)
        ReportScriptError("main", error);
}

// 0x0046db30 (Windows)
void CGameMode::Frame(float elapsed)
{
    if (!m_sleeping)
        return;

    if (m_sleepTime > 0.0f)
        m_sleepTime -= elapsed;
    else
        Run(AMX_EXEC_CONT);
}

// 0x0046dba0 (Windows)
int CGameMode::CallPublic(const char* name)
{
    return CallPublic(name, 0, 0, 0);
}

int CGameMode::CallPublic(const char* name, const cell* arguments, int count, cell defaultResult)
{
    cell result = defaultResult;
    if (!m_active)
        return result;

    if (!m_sleeping)
    {
        CallScriptPublic(&m_amx, name, result, arguments, count);
        return result;
    }

    // amx_Exec updates the registers needed by AMX_EXEC_CONT.
    const AMX suspended = m_amx;
    CallScriptPublic(&m_amx, name, result, arguments, count);
    m_amx.cip = suspended.cip;
    m_amx.frm = suspended.frm;
    m_amx.hea = suspended.hea;
    m_amx.stk = suspended.stk;
    m_amx.pri = suspended.pri;
    m_amx.alt = suspended.alt;
    m_amx.reset_stk = suspended.reset_stk;
    m_amx.reset_hea = suspended.reset_hea;
    return result;
}
