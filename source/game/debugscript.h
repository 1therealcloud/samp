#ifndef DEBUGSCRIPT_H
#define DEBUGSCRIPT_H

#include "game/entity.h"
class CObjectPool;
// R5 address to verify
extern CVector g_debugScriptCameraPosition;
// 0x0014ffac
extern CObjectPool* g_debugScriptObjects;
// R5 address to verify
extern WORD g_debugScriptObjectCount;
namespace DebugScript
{
// Mutates the caller's line; native leaves one old byte before its terminator.
// 0x0009e4b0
char* GetCommandParams(char* line);
// 0x0009e500
void CreateObject(const char* params);
// 0x0009e610
void CreateVehicle(const char* params);
// 0x0009e720
void SetPlayerCameraPos(const char* params);
// 0x0009e790
void RemoveBuildingForPlayer(const char* params);
// Confirmed native RET; ProcessLine still extracts/mutates params first.
// R5 address to verify
void SetPlayerCameraLookAt(const char* params);
// 0x0009e850
void SetPlayerInterior(const char* params);
// Input must be writable despite the historical const API spelling.
// 0x0009e8a0
void ProcessLine(const char* line);
// 0x0009e9b0
void Initialize(const char* filename);
} // namespace DebugScript

#endif // DEBUGSCRIPT_H
