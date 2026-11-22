#ifndef SAMPSRV_GAMEMODES_H
#define SAMPSRV_GAMEMODES_H

#include "amx.h"

class CGameMode
{
public:
    CGameMode();
    ~CGameMode();
    bool Load(const char* filename);
    void Unload();
    void Frame(float elapsed);
    int CallPublic(const char* name);
    int CallPublic(const char* name, const cell* arguments, int count, cell defaultResult);

private:
    CGameMode(const CGameMode&);
    CGameMode& operator=(const CGameMode&);
    void Run(int index);

    AMX m_amx;
    bool m_active;
    bool m_sleeping;
    float m_sleepTime;
};

extern CGameMode* pGameMode;

#endif
