#ifndef DEBUGSCRIPT_H
#define DEBUGSCRIPT_H

#include "game/entity.h"
class CObjectPool;

extern CVector g_debugScriptCameraPosition;

extern CObjectPool* g_debugScriptObjects;

extern WORD g_debugScriptObjectCount;
namespace DebugScript
{
// Mutates the caller's line; native leaves one old byte before its terminator.

char* GetCommandParams(char* line);

void CreateObject(const char* params);

void CreateVehicle(const char* params);

void SetPlayerCameraPos(const char* params);

void RemoveBuildingForPlayer(const char* params);
// Confirmed native RET; ProcessLine still extracts/mutates params first.

void SetPlayerCameraLookAt(const char* params);

void SetPlayerInterior(const char* params);
// Input must be writable despite the historical const API spelling.

void ProcessLine(const char* line);

void Initialize(const char* filename);
} // namespace DebugScript

#endif // DEBUGSCRIPT_H
