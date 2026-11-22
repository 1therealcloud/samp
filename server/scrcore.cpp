#include "scrcore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
