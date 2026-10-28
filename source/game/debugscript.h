#ifndef SAMP_GAME_DEBUGSCRIPT_H
#define SAMP_GAME_DEBUGSCRIPT_H
#include "game/entity.h"
class CObjectPool;
// 0x0018de38
extern CVector g_debugScriptCameraPosition;
// 0x0018de44
extern CObjectPool* g_debugScriptObjects;
// 0x0018de48
extern WORD g_debugScriptObjectCount;
namespace DebugScript
{
// Mutates the caller's line; native leaves one old byte before its terminator.
// 0x0009e2f0
char* GetCommandParams(char* line);
// 0x0009e340
void CreateObject(const char* params);
// 0x0009e450
void CreateVehicle(const char* params);
// 0x0009e560
void SetPlayerCameraPos(const char* params);
// 0x0009e5d0
void RemoveBuildingForPlayer(const char* params);
// Confirmed native RET; ProcessLine still extracts/mutates params first.
// 0x0009e680
void SetPlayerCameraLookAt(const char* params);
// 0x0009e690
void SetPlayerInterior(const char* params);
// Input must be writable despite the historical const API spelling.
// 0x0009e6e0
void ProcessLine(const char* line);
// 0x0009e7f0
void Initialize(const char* filename);
} // namespace DebugScript
#endif
