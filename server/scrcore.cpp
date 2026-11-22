#include "scrcore.h"
#include "console.h"
#include "netgame.h"
#include "plugins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
    int AMXEXPORT amx_CoreInit(AMX* amx);
    int AMXEXPORT amx_FloatInit(AMX* amx);
    int AMXEXPORT amx_StringInit(AMX* amx);
    int AMXEXPORT amx_FileInit(AMX* amx);
    int AMXEXPORT amx_TimeInit(AMX* amx);
    int AMXEXPORT amx_CoreCleanup(AMX* amx);
    int AMXEXPORT amx_FloatCleanup(AMX* amx);
    int AMXEXPORT amx_StringCleanup(AMX* amx);
    int AMXEXPORT amx_FileCleanup(AMX* amx);
    int AMXEXPORT amx_TimeCleanup(AMX* amx);
}

// 0x00473090 (Windows)
static cell AMX_NATIVE_CALL n_print(AMX* amx, const cell* params)
{
    cell* address;
    char text[2048];
    if (amx_GetAddr(amx, params[1], &address) == AMX_ERR_NONE &&
        amx_GetString(text, address, 0, sizeof(text)) == AMX_ERR_NONE)
        logprintf("%s", text);
    return 0;
}

// 0x0046ff70 (Windows)
static cell AMX_NATIVE_CALL n_AddPlayerClass(AMX*, const cell* params)
{
    if (!pNetGame || params[0] != 11 * sizeof(cell))
        return -1;

    PlayerSpawnInfo info = {0};
    info.team = 0xff;
    info.skin = params[1];
    memcpy(&info.x, params + 2, 4 * sizeof(cell));
    for (int i = 0; i < 3; ++i)
    {
        info.weapons[i] = params[6 + i * 2];
        info.ammunition[i] = params[7 + i * 2];
    }
    return pNetGame->AddPlayerClass(info);
}

// 0x00470ee0 (Windows)
static cell AMX_NATIVE_CALL n_SetPlayerVirtualWorld(AMX*, const cell* params)
{
    return pNetGame && params[0] == 2 * sizeof(cell) &&
           pNetGame->SetPlayerVirtualWorld(params[1], params[2]);
}

// 0x00470f30 (Windows)
static cell AMX_NATIVE_CALL n_GetPlayerVirtualWorld(AMX*, const cell* params)
{
    if (!pNetGame || params[0] != sizeof(cell))
        return 0;
    const CPlayerPool::Slot* player = pNetGame->GetPlayerPool().GetAt(params[1]);
    return player ? player->virtualWorld : 0;
}

// 0x00472dd0 (Windows)
static cell AMX_NATIVE_CALL n_IsPlayerConnected(AMX*, const cell* params)
{
    return pNetGame && params[0] == sizeof(cell) &&
           pNetGame->GetPlayerPool().GetAt(static_cast<unsigned short>(params[1]));
}

// 0x00470ff0 (Windows)
static cell AMX_NATIVE_CALL n_GetPlayerPos(AMX* amx, const cell* params)
{
    if (!pNetGame || params[0] != 4 * sizeof(cell))
        return 0;

    const CPlayerPool::Slot* player = pNetGame->GetPlayerPool().GetAt(static_cast<unsigned short>(params[1]));
    if (!player)
        return 0;

    cell* positions[3];
    for (int i = 0; i < 3; ++i)
    {
        const cell address = params[i + 2];
        if (address % sizeof(cell) || amx_GetAddr(amx, address, &positions[i]) != AMX_ERR_NONE)
            return 0;
    }
    for (int i = 0; i < 3; ++i)
        memcpy(positions[i], &player->position[i], sizeof(cell));
    return 1;
}

static cell SetPlayerPosition(const cell* params, bool findGround)
{
    if (!pNetGame || params[0] != 4 * sizeof(cell))
        return 0;

    float position[3];
    memcpy(position, params + 2, sizeof(position));
    return pNetGame->SetPlayerPos(static_cast<unsigned short>(params[1]), position, findGround);
}

// 0x0047ae30 (Windows)
static cell AMX_NATIVE_CALL n_SetPlayerPos(AMX*, const cell* params)
{
    return SetPlayerPosition(params, false);
}

// 0x0047af60 (Windows)
static cell AMX_NATIVE_CALL n_SetPlayerPosFindZ(AMX*, const cell* params)
{
    return SetPlayerPosition(params, true);
}

// 0x00471370 (Windows)
static cell AMX_NATIVE_CALL n_GetPlayerName(AMX* amx, const cell* params)
{
    if (!pNetGame || params[0] != 3 * sizeof(cell) || params[3] <= 0)
        return 0;

    const CPlayerPool::Slot* player = pNetGame->GetPlayerPool().GetAt(static_cast<unsigned short>(params[1]));
    if (!player)
        return 0;

    cell* destination;
    const cell address = params[2];
    if (address % sizeof(cell) || amx_GetAddr(amx, address, &destination) != AMX_ERR_NONE)
        return 0;

    cell length = static_cast<cell>(strlen(player->name));
    if (length >= params[3])
        length = params[3] - 1;

    const cell end = address < amx->hea ? amx->hea : amx->stp;
    if ((end - address) / static_cast<cell>(sizeof(cell)) <= length)
        return 0;

    amx_SetString(destination, player->name, 0, 0, static_cast<size_t>(length + 1));
    return length;
}

// 0x00473540 (Windows)
static cell AMX_NATIVE_CALL n_GetPlayerPing(AMX*, const cell* params)
{
    if (!pNetGame || params[0] != sizeof(cell))
        return -1;
    const CPlayerPool::Slot* player = pNetGame->GetPlayerPool().GetAt(static_cast<unsigned short>(params[1]));
    return player ? pNetGame->GetPlayerPing(player->address) : -1;
}

static cell GetPlayerVitals(AMX* amx, const cell* params, bool armour)
{
    if (!pNetGame || params[0] != 2 * sizeof(cell))
        return 0;

    const CPlayerPool::Slot* player = pNetGame->GetPlayerPool().GetAt(static_cast<unsigned short>(params[1]));
    cell* destination;
    const cell address = params[2];
    if (!player || address % sizeof(cell) || amx_GetAddr(amx, address, &destination) != AMX_ERR_NONE)
        return 0;

    const float value = armour ? player->armour : player->health;
    memcpy(destination, &value, sizeof(value));
    return 1;
}

static cell SetPlayerVitals(const cell* params, bool armour)
{
    if (!pNetGame || params[0] != 2 * sizeof(cell))
        return 0;
    float value;
    memcpy(&value, params + 2, sizeof(value));
    return pNetGame->SetPlayerVitals(static_cast<unsigned short>(params[1]), value, armour);
}

// 0x004711b0 (Windows)
static cell AMX_NATIVE_CALL n_GetPlayerHealth(AMX* amx, const cell* params)
{
    return GetPlayerVitals(amx, params, false);
}

// 0x00473dd0 (Windows)
static cell AMX_NATIVE_CALL n_GetPlayerArmour(AMX* amx, const cell* params)
{
    return GetPlayerVitals(amx, params, true);
}

// 0x0047b090 (Windows)
static cell AMX_NATIVE_CALL n_SetPlayerHealth(AMX*, const cell* params)
{
    return SetPlayerVitals(params, false);
}

// 0x0047cf30 (Windows)
static cell AMX_NATIVE_CALL n_SetPlayerArmour(AMX*, const cell* params)
{
    return SetPlayerVitals(params, true);
}

static const AMX_NATIVE_INFO scriptNatives[] = {
    {"print", n_print},
    {"AddPlayerClass", n_AddPlayerClass},
    {"SetPlayerVirtualWorld", n_SetPlayerVirtualWorld},
    {"GetPlayerVirtualWorld", n_GetPlayerVirtualWorld},
    {"IsPlayerConnected", n_IsPlayerConnected},
    {"GetPlayerPos", n_GetPlayerPos},
    {"SetPlayerPos", n_SetPlayerPos},
    {"SetPlayerPosFindZ", n_SetPlayerPosFindZ},
    {"GetPlayerName", n_GetPlayerName},
    {"GetPlayerPing", n_GetPlayerPing},
    {"GetPlayerHealth", n_GetPlayerHealth},
    {"GetPlayerArmour", n_GetPlayerArmour},
    {"SetPlayerHealth", n_SetPlayerHealth},
    {"SetPlayerArmour", n_SetPlayerArmour},
    {NULL, NULL},
};

// 0x0046fa70 (Windows), 0x080e17c0 (Linux)
int AMXAPI aux_LoadProgram(AMX* amx, const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
        return AMX_ERR_NOTFOUND;

    AMX_HEADER header;
    if (fread(&header, sizeof(header), 1, file) != 1)
    {
        fclose(file);
        return AMX_ERR_FORMAT;
    }

    uint16_t magic = header.magic;
    uint32_t size = header.size;
    uint32_t stp = header.stp;
    amx_Align16(&magic);
    amx_Align32(&size);
    amx_Align32(&stp);
    if (magic != AMX_MAGIC || size < sizeof(header) || size > stp || stp > 0x7fffffffU)
    {
        fclose(file);
        return AMX_ERR_FORMAT;
    }

    void* program = malloc(stp);
    if (!program)
    {
        fclose(file);
        return AMX_ERR_MEMORY;
    }

    rewind(file);
    const size_t bytesRead = fread(program, 1, size, file);
    fclose(file);
    if (bytesRead != size)
    {
        free(program);
        return AMX_ERR_FORMAT;
    }

    memset(amx, 0, sizeof(*amx));
    const int result = amx_Init(amx, program);
    if (result != AMX_ERR_NONE)
    {
        free(program);
        amx->base = NULL;
    }
    return result;
}

// 0x0046fc20 (Windows), 0x080e19c0 (Linux)
int AMXAPI aux_FreeProgram(AMX* amx)
{
    if (amx->base)
    {
        amx_Cleanup(amx);
        free(amx->base);
        memset(amx, 0, sizeof(*amx));
    }
    return AMX_ERR_NONE;
}

bool LoadScript(AMX* amx, const char* filename)
{
    const int error = aux_LoadProgram(amx, filename);
    if (error != AMX_ERR_NONE)
    {
        ReportScriptError(filename, error);
        return false;
    }

    amx_CoreInit(amx);
    amx_FloatInit(amx);
    amx_StringInit(amx);
    amx_FileInit(amx);
    amx_TimeInit(amx);
    amx_Register(amx, scriptNatives, -1);
    pPlugins->DoAmxLoad(amx);
    return true;
}

void UnloadScript(AMX* amx)
{
    aux_FreeProgram(amx);
    pPlugins->DoAmxUnload(amx);
    amx_TimeCleanup(amx);
    amx_FileCleanup(amx);
    amx_StringCleanup(amx);
    amx_FloatCleanup(amx);
    amx_CoreCleanup(amx);
}

bool CallScriptPublic(AMX* amx, const char* name, cell& result, const cell* arguments, int count)
{
    int index;
    if (amx_FindPublic(amx, name, &index) != AMX_ERR_NONE)
        return false;

    const cell stack = amx->stk;
    const int parameterCount = amx->paramcount;
    while (count)
    {
        const int error = amx_Push(amx, arguments[--count]);
        if (error != AMX_ERR_NONE)
        {
            amx->stk = stack;
            amx->paramcount = parameterCount;
            ReportScriptError(name, error);
            return false;
        }
    }
    result = 0;
    const int error = amx_Exec(amx, &result, index);
    if (error != AMX_ERR_NONE)
    {
        amx->stk = stack;
        amx->paramcount = parameterCount;
        ReportScriptError(name, error);
    }
    return true;
}

void ReportScriptError(const char* name, int error)
{
    if (error != AMX_ERR_NONE)
        logprintf("Script[%s]: Run time error %d", name, error);
}
