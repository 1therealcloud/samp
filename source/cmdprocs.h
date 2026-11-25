#ifndef CMDPROCS_H
#define CMDPROCS_H

#include "game/scripting.h"
namespace Commands
{

void Default(char* params);

void TestDeathWindow(char* params);

void ToggleCameraTargetLabels(char* params);

void SetChatPageSize(char* params);

void SetChatFontSize(char* params);

void DrawNameTagStatus(char* params);

void DrawChatTimestamps(char* params);

void ToggleAudioStreamMessages(char* params);

void ToggleURLMessages(char* params);

void ToggleHudScaleFix(char* params);

void PrintMemory(char* params);

void SetFrameLimiter(char* params);

void ToggleHeadMoves(char* params);

void Quit(char* params);

void CmpStat(char* params);

void SavePosition(char* params);

void SavePositionOnly(char* params);

void DebugSetPlayerSkin(char* params);

void DebugCreateVehicle(char* params);

void DebugEnableVehicleSelection(char* params);

void DebugSetWorldWeather(char* params);

void DebugSetWorldTime(char* params);

void PrintCurrentInterior(char* params);

void ToggleObjectsLight(char* params);

void ToggleDebugLabels(char* params);

void SendRconCommand(char* params);

void Setup();
} // namespace Commands

void ClientRequestQuit();

extern BYTE g_commandCameraTargetLabels;

extern DWORD g_commandObjectLighting;

extern const Scripting::OpcodeInfo g_commandGetInterior;

extern const Scripting::OpcodeInfo g_commandGetVehicleAngle;

extern const Scripting::OpcodeInfo g_commandGetActorAngle;

#endif // CMDPROCS_H
