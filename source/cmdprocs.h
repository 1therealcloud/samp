#ifndef CMDPROCS_H
#define CMDPROCS_H

#include "game/scripting.h"
namespace Commands
{
// 0x000684e0
void Default(char* params);
// 0x00068500
void TestDeathWindow(char* params);
// 0x000685e0
void ToggleCameraTargetLabels(char* params);
// 0x000685f0
void SetChatPageSize(char* params);
// 0x00068670
void SetChatFontSize(char* params);
// 0x00068720
void DrawNameTagStatus(char* params);
// 0x00068730
void DrawChatTimestamps(char* params);
// 0x00068790
void ToggleAudioStreamMessages(char* params);
// 0x00068800
void ToggleURLMessages(char* params);
// R5 address to verify
void ToggleHudScaleFix(char* params);
// 0x000688b0
void PrintMemory(char* params);
// 0x000688d0
void SetFrameLimiter(char* params);
// 0x00068960
void ToggleHeadMoves(char* params);
// 0x000689e0
void Quit(char* params);
// 0x000689f0
void CmpStat(char* params);
// 0x00068a00
void SavePosition(char* params);
// 0x00068b80
void SavePositionOnly(char* params);
// 0x00068d00
void DebugSetPlayerSkin(char* params);
// 0x00068d70
void DebugCreateVehicle(char* params);
// 0x00068eb0
void DebugEnableVehicleSelection(char* params);
// 0x00068ed0
void DebugSetWorldWeather(char* params);
// 0x00068f20
void DebugSetWorldTime(char* params);
// 0x00068fd0
void PrintCurrentInterior(char* params);
// 0x00069000
void ToggleObjectsLight(char* params);
// 0x00069020
void ToggleDebugLabels(char* params);
// 0x00069030
void SendRconCommand(char* params);
// 0x00069110
void Setup();
} // namespace Commands
// 0x000c3e80
void ClientRequestQuit();
// R5 address to verify
extern BYTE g_commandCameraTargetLabels;
// R5 address to verify
extern DWORD g_commandObjectLighting;
// R5 address to verify
extern const Scripting::OpcodeInfo g_commandGetInterior;
// R5 address to verify
extern const Scripting::OpcodeInfo g_commandGetVehicleAngle;
// R5 address to verify
extern const Scripting::OpcodeInfo g_commandGetActorAngle;

#endif // CMDPROCS_H
