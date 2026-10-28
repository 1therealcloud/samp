#ifndef SAMP_CLIENT_COMMANDS_H
#define SAMP_CLIENT_COMMANDS_H
#include "game/scripting.h"
namespace Commands
{
// 0x00067f50
void Default(char* params);
// 0x00067f70
void TestDeathWindow(char* params);
// 0x00068050
void ToggleCameraTargetLabels(char* params);
// 0x00068060
void SetChatPageSize(char* params);
// 0x000680e0
void SetChatFontSize(char* params);
// 0x00068190
void DrawNameTagStatus(char* params);
// 0x000681a0
void DrawChatTimestamps(char* params);
// 0x00068200
void ToggleAudioStreamMessages(char* params);
// 0x00068270
void ToggleURLMessages(char* params);
// 0x000682e0
void ToggleHudScaleFix(char* params);
// 0x000682f0
void PrintMemory(char* params);
// 0x00068310
void SetFrameLimiter(char* params);
// 0x000683a0
void ToggleHeadMoves(char* params);
// 0x00068420
void Quit(char* params);
// 0x00068430
void CmpStat(char* params);
// 0x00068440
void SavePosition(char* params);
// 0x000685c0
void SavePositionOnly(char* params);
// 0x00068740
void DebugSetPlayerSkin(char* params);
// 0x000687b0
void DebugCreateVehicle(char* params);
// 0x000688f0
void DebugEnableVehicleSelection(char* params);
// 0x00068910
void DebugSetWorldWeather(char* params);
// 0x00068960
void DebugSetWorldTime(char* params);
// 0x00068a10
void PrintCurrentInterior(char* params);
// 0x00068a40
void ToggleObjectsLight(char* params);
// 0x00068a60
void ToggleDebugLabels(char* params);
// 0x00068a70
void SendRconCommand(char* params);
// 0x00068b50
void Setup();
} // namespace Commands
// 0x000c5550
void ClientRequestQuit();
// 0x00156ac8
extern BYTE g_commandCameraTargetLabels;
// 0x00154698
extern DWORD g_commandObjectLighting;
// 0x0011bde4
extern const Scripting::OpcodeInfo g_commandGetInterior;
// 0x0011bdd0
extern const Scripting::OpcodeInfo g_commandGetVehicleAngle;
// 0x0011bdf8
extern const Scripting::OpcodeInfo g_commandGetActorAngle;
#endif
