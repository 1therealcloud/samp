#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <windows.h>
// Initialize allocates and clears 224 bytes.
struct CRunningScript
{
    BYTE opaque[224];
};
// 0x0026b558
extern CRunningScript* g_scriptThread;
namespace Scripting
{
#pragma pack(push, 1)
struct OpcodeInfo
{
    WORD opcode;
    char parameters[16];
};
#pragma pack(pop)
// 0x000b2550
void Initialize();
// 0x000b22d0
DWORD ExecBuffer();
// 0x000b2310
DWORD __cdecl ProcessCommand(const OpcodeInfo* command, ...);
} // namespace Scripting
// 0x0026b458
extern BYTE g_scriptBuffer[256];
// R5 address to verify
extern DWORD* g_scriptVariables[18];
// 0x0026b55c
extern DWORD g_scriptLastOpcode;
// 0x0026b560
extern BOOL g_scriptLocalDebug;
// 0x001173b8
extern FARPROC g_scriptProcessOneCommand;
// R5 address to verify
extern int g_gameExceptionCount;
// 0x000609b0
extern WORD g_scriptVehicleComponent;

int __stdcall GameExceptionFilter(DWORD code, EXCEPTION_POINTERS* pointers, const char* warning);
#endif

#ifndef SAMP_PED_COMMANDS_H
#define SAMP_PED_COMMANDS_H
// 0x000ecfc4
extern const Scripting::OpcodeInfo g_pedCommand087e;
// 0x000ece84
extern const Scripting::OpcodeInfo g_pedCommand0446;
// 0x000ec50c
extern const Scripting::OpcodeInfo g_entityTeleportCommand;
// 0x000ed050
extern const Scripting::OpcodeInfo g_pedDetachParachute;
// 0x000ed03c
extern const Scripting::OpcodeInfo g_pedDestroyParachute;
// 0x000ed12c
extern const Scripting::OpcodeInfo g_pedDropStuff;
// 0x000ecec0
extern const Scripting::OpcodeInfo g_pedWarpFromVehicle;
// 0x000ed210
extern const char g_pedNullAnimation[5];
// 0x000ece48
extern const Scripting::OpcodeInfo g_pedGetCurrentWeaponCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedSetArmedWeaponCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedFightingStyleCommand;
// 0x000ed08c
extern const Scripting::OpcodeInfo g_pedCellphoneCommand;
// 0x000ecdf8
extern const Scripting::OpcodeInfo g_pedDeleteArrowCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedSpawnInfoCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedHandsUpCommand;
// 0x000ece70
extern const Scripting::OpcodeInfo g_pedImmunitiesCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedRotationCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedParachuteCleanupCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedCreateArrowCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedArrowColorCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedArrowModeCommand;
// 0x000e8dd8
extern const double g_pedParachuteZero;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedStopUrinationEffectCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedExitVehicleCommand;
// 0x000ecf9c
extern const Scripting::OpcodeInfo g_pedApplyAnimationCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedIsPerformingAnimationCommand;
// 0x000ec2fc
extern const char g_pedBlockedAnimationLibrary[4];
// 0x00116dd8
extern char g_pedDanceLibraries[4][16];
// 0x00116e18
extern char g_pedDanceNames[4][16];
// 0x00116e58
extern char g_pedDanceMoveTables[3][16][16];
// 0x000e59c9
extern const char g_pedEmptyDanceAnimation[1];
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedSelectInteriorCommand;
// 0x000ecfec
extern const Scripting::OpcodeInfo g_pedLinkInteriorCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedRefreshInteriorCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedControllableCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedFreezeCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedKillCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedWarpPassengerCommand;
// 0x000ecee8
extern const Scripting::OpcodeInfo g_pedEnterPassengerCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedEnterDriverCommand;
// 0x000ed0f0
extern const Scripting::OpcodeInfo g_pedCreateUrinationEffectCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedStartUrinationEffectCommand;
// 0x000ec9d4
extern const char g_pedUrinationLibrary[9];
// 0x000ed268
extern const char g_pedUrinationAnimation[10];
// 0x000ed244
extern const char g_pedUrinationEffect[10];
// 0x000ed250
extern const double g_pedUrinationOffsetY;
// 0x000ed258
extern const double g_pedUrinationOffsetZ;
// 0x000ed260
extern const double g_pedUrinationDirectionY;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedWarpDriverCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedTrainCameraCommand;
// 0x000ec9e0
extern const char g_pedSmokeLibrary[6];
// 0x000ed238
extern const char g_pedSmokeAnimation[12];
// 0x000ec9e8
extern const char g_pedDrinkLibrary[4];
// 0x000ed228
extern const char g_pedDrinkAnimationM[15];
// 0x000ed218
extern const char g_pedDrinkAnimationF[15];
// R5 address to verify
extern const Scripting::OpcodeInfo g_pedDrunkShakeCommand;
#endif

#ifndef SAMP_WRAPPER_COMMANDS_H
#define SAMP_WRAPPER_COMMANDS_H
// 0x000ecd04
extern const Scripting::OpcodeInfo g_objectSetRotationCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_cameraFade;
// R5 address to verify
extern const Scripting::OpcodeInfo g_cameraRestoreJumpcut;
// 0x000ec340
extern const Scripting::OpcodeInfo g_cameraPointAt;
// R5 address to verify
extern const Scripting::OpcodeInfo g_cameraRestore;
// 0x000ec368
extern const Scripting::OpcodeInfo g_cameraSet;
// 0x000ec37c
extern const Scripting::OpcodeInfo g_cameraStop;
// 0x000ec390
extern const Scripting::OpcodeInfo g_cameraMove;
// 0x000ec3a4
extern const Scripting::OpcodeInfo g_cameraTrack;
// 0x000ec3b8
extern const Scripting::OpcodeInfo g_cameraPersistTrack;
// 0x000ec3cc
extern const Scripting::OpcodeInfo g_cameraPersistPosition;
// 0x000ec9f8
extern const Scripting::OpcodeInfo g_audioPlay;
// R5 address to verify
extern const Scripting::OpcodeInfo g_audioLoad;
// R5 address to verify
extern const Scripting::OpcodeInfo g_audioUnload;
// 0x000eca5c
extern const Scripting::OpcodeInfo g_audioSetPosition;
// R5 address to verify
extern const Scripting::OpcodeInfo g_audioIsLoaded;
// R5 address to verify
extern const Scripting::OpcodeInfo g_audioStart;
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameSetWorldTime;
// 0x000ed6a0
extern const Scripting::OpcodeInfo g_vehicleChangeInteriorCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleIsSunkCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleAttachTrailerCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleDetachTrailerCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleRemoveComponentCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehiclePaintjobCommand;
// 0x000ed510
extern const Scripting::OpcodeInfo g_vehicleRotationCommand;
// 0x000ed59c
extern const Scripting::OpcodeInfo g_vehicleProofsCommand;
// 0x000ed68c
extern const Scripting::OpcodeInfo g_vehicleDamageableCommand;
// 0x000ed5b0
extern const Scripting::OpcodeInfo g_vehicleWarpPedCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleAddComponentCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleComponentLoadedCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleRequestComponentCommand;
// 0x000ed524
extern const Scripting::OpcodeInfo g_vehicleCreateMarkerCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleDestroyMarkerCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleMarkerColorCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleMarkerModeCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleDestroyCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleDestroyTrainCommand;
// 0x000ed4e8
extern const Scripting::OpcodeInfo g_vehicleCreateCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleGasTankExplosionCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleHydraulicsCommand;
// 0x000ed5c4
extern const Scripting::OpcodeInfo g_vehicleCreateTrainCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameDisableTrainTrafficCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameRequestAnimationCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameIsAnimationLoadedCommand;
// 0x000ec5bc
extern const Scripting::OpcodeInfo g_gameDeleteMarkerCommand;
// 0x000ec648
extern const Scripting::OpcodeInfo g_gameIncreaseMoneyCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameZoneDisplayCommand;
// 0x000ec65c
extern const Scripting::OpcodeInfo g_gameFindGroundZCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameRefreshRendererCommand;
// 0x000ec684
extern const Scripting::OpcodeInfo g_gameDeleteRacingCheckpointCommand;
// 0x000ec5e4
extern const Scripting::OpcodeInfo g_gameCreateMarkerCommand;
// 0x000ec620
extern const Scripting::OpcodeInfo g_gameCreateMarkerType1Command;
// 0x000ec760
extern const Scripting::OpcodeInfo g_gameCreateMarkerType2Command;
// 0x000ec60c
extern const Scripting::OpcodeInfo g_gameCreateMarkerType3Command;
// 0x000ec5d0
extern const Scripting::OpcodeInfo g_gameMarkerColorCommand;
// 0x000ec5f8
extern const Scripting::OpcodeInfo g_gameMarkerModeCommand;
// 0x000ec670
extern const Scripting::OpcodeInfo g_gameCreateRacingCheckpointCommand;
// 0x000ec634
extern const Scripting::OpcodeInfo g_gameLocateCheckpointCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_gameClearTextCommand;
// 0x000ec738
extern const Scripting::OpcodeInfo g_gameCreatePickupCommand;
// 0x000ec6ac
extern const Scripting::OpcodeInfo g_gameReleaseModelCommand;
// 0x000ec9ec
extern const char g_gameParachuteLibrary[10];
// 0x000e6194
extern const char g_gameVendingLibrary[8];
// 0x000ec9cc
extern const char g_gameFightBLibrary[8];
// 0x000ec9c4
extern const char g_gameFightCLibrary[8];
// 0x000ec9bc
extern const char g_gameFightDLibrary[8];
// 0x000ec9b4
extern const char g_gameFightELibrary[8];

#endif // SCRIPTING_H
