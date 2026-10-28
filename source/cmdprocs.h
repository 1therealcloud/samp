#ifndef SAMP_CLIENT_COMMANDS_H
#define SAMP_CLIENT_COMMANDS_H
#include "game/scripting.h"
namespace Commands
{
// samp.dll RVA: 0x00067f50
void Default(char* params);
// samp.dll RVA: 0x00067f70
void TestDeathWindow(char* params);
// samp.dll RVA: 0x00068050
void ToggleCameraTargetLabels(char* params);
// samp.dll RVA: 0x00068060
void SetChatPageSize(char* params);
// samp.dll RVA: 0x000680e0
void SetChatFontSize(char* params);
// samp.dll RVA: 0x00068190
void DrawNameTagStatus(char* params);
// samp.dll RVA: 0x000681a0
void DrawChatTimestamps(char* params);
// samp.dll RVA: 0x00068200
void ToggleAudioStreamMessages(char* params);
// samp.dll RVA: 0x00068270
void ToggleURLMessages(char* params);
// samp.dll RVA: 0x000682e0
void ToggleHudScaleFix(char* params);
// samp.dll RVA: 0x000682f0
void PrintMemory(char* params);
// samp.dll RVA: 0x00068310
void SetFrameLimiter(char* params);
// samp.dll RVA: 0x000683a0
void ToggleHeadMoves(char* params);
// samp.dll RVA: 0x00068420
void Quit(char* params);
// samp.dll RVA: 0x00068430
void CmpStat(char* params);
// samp.dll RVA: 0x00068440
void SavePosition(char* params);
// samp.dll RVA: 0x000685c0
void SavePositionOnly(char* params);
// samp.dll RVA: 0x00068740
void DebugSetPlayerSkin(char* params);
// samp.dll RVA: 0x000687b0
void DebugCreateVehicle(char* params);
// samp.dll RVA: 0x000688f0
void DebugEnableVehicleSelection(char* params);
// samp.dll RVA: 0x00068910
void DebugSetWorldWeather(char* params);
// samp.dll RVA: 0x00068960
void DebugSetWorldTime(char* params);
// samp.dll RVA: 0x00068a10
void PrintCurrentInterior(char* params);
// samp.dll RVA: 0x00068a40
void ToggleObjectsLight(char* params);
// samp.dll RVA: 0x00068a60
void ToggleDebugLabels(char* params);
// samp.dll RVA: 0x00068a70
void SendRconCommand(char* params);
// samp.dll RVA: 0x00068b50
void Setup();
} // namespace Commands
// samp.dll RVA: 0x000c5550
void ClientRequestQuit();
// samp.dll RVA: 0x00156ac8
extern BYTE g_commandCameraTargetLabels;
// samp.dll RVA: 0x00154698
extern DWORD g_commandObjectLighting;
// samp.dll RVA: 0x0011bde4
extern const Scripting::OpcodeInfo g_commandGetInterior;
// samp.dll RVA: 0x0011bdd0
extern const Scripting::OpcodeInfo g_commandGetVehicleAngle;
// samp.dll RVA: 0x0011bdf8
extern const Scripting::OpcodeInfo g_commandGetActorAngle;
#endif
