#ifndef SAMPSRV_SCRCORE_H
#define SAMPSRV_SCRCORE_H

#include "amx.h"

int AMXAPI aux_LoadProgram(AMX* amx, const char* filename);
int AMXAPI aux_FreeProgram(AMX* amx);
bool LoadScript(AMX* amx, const char* filename);
void UnloadScript(AMX* amx);
bool CallScriptPublic(AMX* amx, const char* name, cell& result);
void ReportScriptError(const char* name, int error);

#endif
