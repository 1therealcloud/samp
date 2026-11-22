#include "scrcore.h"
#include "console.h"
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

static const AMX_NATIVE_INFO scriptNatives[] = {
    {"print", n_print},
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

bool CallScriptPublic(AMX* amx, const char* name, cell& result)
{
    int index;
    if (amx_FindPublic(amx, name, &index) != AMX_ERR_NONE)
        return false;

    result = 0;
    ReportScriptError(name, amx_Exec(amx, &result, index));
    return true;
}

void ReportScriptError(const char* name, int error)
{
    if (error != AMX_ERR_NONE)
        logprintf("Script[%s]: Run time error %d", name, error);
}
