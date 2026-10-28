#ifndef SAMP_RECOVERED_SCRIPTING_H
#define SAMP_RECOVERED_SCRIPTING_H
#include <windows.h>
// Layout fields are not established yet; native Initialize allocates and clears 224 bytes.
struct CRunningScript
{
    BYTE opaque[224];
};
// 0x002a93f0
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
// 0x000b2150
void Initialize();
// 0x000b1ed0
DWORD ExecBuffer();
// 0x000b1f10
DWORD __cdecl ProcessCommand(const OpcodeInfo* command, ...);
} // namespace Scripting
// 0x002a92f0
extern BYTE g_scriptBuffer[256];
// 0x002a92a8
extern DWORD* g_scriptVariables[18];
// 0x002a93f4
extern DWORD g_scriptLastOpcode;
// 0x002a93f8
extern BOOL g_scriptLocalDebug;
// 0x00155390
extern FARPROC g_scriptProcessOneCommand;
// 0x00163a70
extern int g_gameExceptionCount;
// 0x00060470
int __stdcall GameExceptionFilter(DWORD code, EXCEPTION_POINTERS* pointers, const char* warning);
#endif

#ifndef SAMP_PED_COMMANDS_H
#define SAMP_PED_COMMANDS_H
// 0x0011f0d0
extern const Scripting::OpcodeInfo g_pedCommand087e;
// 0x0011ef90
extern const Scripting::OpcodeInfo g_pedCommand0446;
// 0x0011e604
extern const Scripting::OpcodeInfo g_entityTeleportCommand;
// 0x0011f15c
extern const Scripting::OpcodeInfo g_pedDetachParachute;
// 0x0011f148
extern const Scripting::OpcodeInfo g_pedDestroyParachute;
// 0x0011f238
extern const Scripting::OpcodeInfo g_pedDropStuff;
// 0x0011efcc
extern const Scripting::OpcodeInfo g_pedWarpFromVehicle;
// 0x0011f320
extern const char g_pedNullAnimation[5];
// 0x0011ef54
extern const Scripting::OpcodeInfo g_pedGetCurrentWeaponCommand;
// 0x0011ef40
extern const Scripting::OpcodeInfo g_pedSetArmedWeaponCommand;
// 0x0011f1c0
extern const Scripting::OpcodeInfo g_pedFightingStyleCommand;
// 0x0011f198
extern const Scripting::OpcodeInfo g_pedCellphoneCommand;
// 0x0011eef0
extern const Scripting::OpcodeInfo g_pedDeleteArrowCommand;
// 0x0011ef18
extern const Scripting::OpcodeInfo g_pedSpawnInfoCommand;
// 0x0011f1ac
extern const Scripting::OpcodeInfo g_pedHandsUpCommand;
// 0x0011ef7c
extern const Scripting::OpcodeInfo g_pedImmunitiesCommand;
// 0x0011f01c
extern const Scripting::OpcodeInfo g_pedRotationCommand;
// 0x0011f1d4
extern const Scripting::OpcodeInfo g_pedParachuteCleanupCommand;
// 0x0011ee8c
extern const Scripting::OpcodeInfo g_pedCreateArrowCommand;
// 0x0011ef04
extern const Scripting::OpcodeInfo g_pedArrowColorCommand;
// 0x0011f184
extern const Scripting::OpcodeInfo g_pedArrowModeCommand;
// 0x0011ae68
extern const double g_pedParachuteZero;
// 0x0011f1e8
extern const Scripting::OpcodeInfo g_pedStopUrinationEffectCommand;
// 0x0011f008
extern const Scripting::OpcodeInfo g_pedExitVehicleCommand;
// 0x0011f0a8
extern const Scripting::OpcodeInfo g_pedApplyAnimationCommand;
// 0x0011f0bc
extern const Scripting::OpcodeInfo g_pedIsPerformingAnimationCommand;
// 0x0011e324
extern const char g_pedBlockedAnimationLibrary[4];
// 0x00154db0
extern char g_pedDanceLibraries[4][16];
// 0x00154df0
extern char g_pedDanceNames[4][16];
// 0x00154e30
extern char g_pedDanceMoveTables[3][16][16];
// 0x00117a39
extern const char g_pedEmptyDanceAnimation[1];
// 0x0011eec8
extern const Scripting::OpcodeInfo g_pedSelectInteriorCommand;
// 0x0011f0f8
extern const Scripting::OpcodeInfo g_pedLinkInteriorCommand;
// 0x0011f058
extern const Scripting::OpcodeInfo g_pedRefreshInteriorCommand;
// 0x0011ef2c
extern const Scripting::OpcodeInfo g_pedControllableCommand;
// 0x0011f044
extern const Scripting::OpcodeInfo g_pedFreezeCommand;
// 0x0011f080
extern const Scripting::OpcodeInfo g_pedKillCommand;
// 0x0011f06c
extern const Scripting::OpcodeInfo g_pedWarpPassengerCommand;
// 0x0011eff4
extern const Scripting::OpcodeInfo g_pedEnterPassengerCommand;
// 0x0011efe0
extern const Scripting::OpcodeInfo g_pedEnterDriverCommand;
// 0x0011f1fc
extern const Scripting::OpcodeInfo g_pedCreateUrinationEffectCommand;
// 0x0011f210
extern const Scripting::OpcodeInfo g_pedStartUrinationEffectCommand;
// 0x0011eacc
extern const char g_pedUrinationLibrary[9];
// 0x0011f378
extern const char g_pedUrinationAnimation[10];
// 0x0011f354
extern const char g_pedUrinationEffect[10];
// 0x0011f360
extern const double g_pedUrinationOffsetY;
// 0x0011f368
extern const double g_pedUrinationOffsetZ;
// 0x0011f370
extern const double g_pedUrinationDirectionY;
// 0x0011ef68
extern const Scripting::OpcodeInfo g_pedWarpDriverCommand;
// 0x0011eedc
extern const Scripting::OpcodeInfo g_pedTrainCameraCommand;
// 0x0011ead8
extern const char g_pedSmokeLibrary[6];
// 0x0011f348
extern const char g_pedSmokeAnimation[12];
// 0x0011eae0
extern const char g_pedDrinkLibrary[4];
// 0x0011f338
extern const char g_pedDrinkAnimationM[15];
// 0x0011f328
extern const char g_pedDrinkAnimationF[15];
// 0x0011f224
extern const Scripting::OpcodeInfo g_pedDrunkShakeCommand;
#endif

#ifndef SAMP_WRAPPER_COMMANDS_H
#define SAMP_WRAPPER_COMMANDS_H
// 0x0011edfc
extern const Scripting::OpcodeInfo g_objectSetRotationCommand;
// 0x0011e410
extern const Scripting::OpcodeInfo g_cameraFade;
// 0x0011e424
extern const Scripting::OpcodeInfo g_cameraRestoreJumpcut;
// 0x0011e438
extern const Scripting::OpcodeInfo g_cameraPointAt;
// 0x0011e44c
extern const Scripting::OpcodeInfo g_cameraRestore;
// 0x0011e460
extern const Scripting::OpcodeInfo g_cameraSet;
// 0x0011e474
extern const Scripting::OpcodeInfo g_cameraStop;
// 0x0011e488
extern const Scripting::OpcodeInfo g_cameraMove;
// 0x0011e49c
extern const Scripting::OpcodeInfo g_cameraTrack;
// 0x0011e4b0
extern const Scripting::OpcodeInfo g_cameraPersistTrack;
// 0x0011e4c4
extern const Scripting::OpcodeInfo g_cameraPersistPosition;
// 0x0011eaf0
extern const Scripting::OpcodeInfo g_audioPlay;
// 0x0011eb04
extern const Scripting::OpcodeInfo g_audioLoad;
// 0x0011eb40
extern const Scripting::OpcodeInfo g_audioUnload;
// 0x0011eb54
extern const Scripting::OpcodeInfo g_audioSetPosition;
// 0x0011eb18
extern const Scripting::OpcodeInfo g_audioIsLoaded;
// 0x0011eb2c
extern const Scripting::OpcodeInfo g_audioStart;
// 0x0011e68c
extern const Scripting::OpcodeInfo g_gameSetWorldTime;
// 0x0011f7a4
extern const Scripting::OpcodeInfo g_vehicleChangeInteriorCommand;
// 0x0011f678
extern const Scripting::OpcodeInfo g_vehicleIsSunkCommand;
// 0x0011f7b8
extern const Scripting::OpcodeInfo g_vehicleAttachTrailerCommand;
// 0x0011f7cc
extern const Scripting::OpcodeInfo g_vehicleDetachTrailerCommand;
// 0x0011f77c
extern const Scripting::OpcodeInfo g_vehicleRemoveComponentCommand;
// 0x0011f72c
extern const Scripting::OpcodeInfo g_vehiclePaintjobCommand;
// 0x0011f614
extern const Scripting::OpcodeInfo g_vehicleRotationCommand;
// 0x0011f6a0
extern const Scripting::OpcodeInfo g_vehicleProofsCommand;
// 0x0011f790
extern const Scripting::OpcodeInfo g_vehicleDamageableCommand;
// 0x0011f6b4
extern const Scripting::OpcodeInfo g_vehicleWarpPedCommand;
// 0x0011f740
extern const Scripting::OpcodeInfo g_vehicleAddComponentCommand;
// 0x0011f754
extern const Scripting::OpcodeInfo g_vehicleComponentLoadedCommand;
// 0x0011f768
extern const Scripting::OpcodeInfo g_vehicleRequestComponentCommand;
// 0x0011f628
extern const Scripting::OpcodeInfo g_vehicleCreateMarkerCommand;
// 0x0011f63c
extern const Scripting::OpcodeInfo g_vehicleDestroyMarkerCommand;
// 0x0011f650
extern const Scripting::OpcodeInfo g_vehicleMarkerColorCommand;
// 0x0011f664
extern const Scripting::OpcodeInfo g_vehicleMarkerModeCommand;
// 0x0011f600
extern const Scripting::OpcodeInfo g_vehicleDestroyCommand;
// 0x0011f6dc
extern const Scripting::OpcodeInfo g_vehicleDestroyTrainCommand;
// 0x0011f5ec
extern const Scripting::OpcodeInfo g_vehicleCreateCommand;
// 0x0011f6f0
extern const Scripting::OpcodeInfo g_vehicleGasTankExplosionCommand;
// 0x0011f718
extern const Scripting::OpcodeInfo g_vehicleHydraulicsCommand;
// 0x0011f6c8
extern const Scripting::OpcodeInfo g_vehicleCreateTrainCommand;
// 0x0011e790
extern const Scripting::OpcodeInfo g_gameDisableTrainTrafficCommand;
// 0x0011e7e0
extern const Scripting::OpcodeInfo g_gameRequestAnimationCommand;
// 0x0011e7f4
extern const Scripting::OpcodeInfo g_gameIsAnimationLoadedCommand;
// 0x0011e6b4
extern const Scripting::OpcodeInfo g_gameDeleteMarkerCommand;
// 0x0011e740
extern const Scripting::OpcodeInfo g_gameIncreaseMoneyCommand;
// 0x0011e86c
extern const Scripting::OpcodeInfo g_gameZoneDisplayCommand;
// 0x0011e754
extern const Scripting::OpcodeInfo g_gameFindGroundZCommand;
// 0x0011e7cc
extern const Scripting::OpcodeInfo g_gameRefreshRendererCommand;
// 0x0011e77c
extern const Scripting::OpcodeInfo g_gameDeleteRacingCheckpointCommand;
// 0x0011e6dc
extern const Scripting::OpcodeInfo g_gameCreateMarkerCommand;
// 0x0011e718
extern const Scripting::OpcodeInfo g_gameCreateMarkerType1Command;
// 0x0011e858
extern const Scripting::OpcodeInfo g_gameCreateMarkerType2Command;
// 0x0011e704
extern const Scripting::OpcodeInfo g_gameCreateMarkerType3Command;
// 0x0011e6c8
extern const Scripting::OpcodeInfo g_gameMarkerColorCommand;
// 0x0011e6f0
extern const Scripting::OpcodeInfo g_gameMarkerModeCommand;
// 0x0011e768
extern const Scripting::OpcodeInfo g_gameCreateRacingCheckpointCommand;
// 0x0011e72c
extern const Scripting::OpcodeInfo g_gameLocateCheckpointCommand;
// 0x0011e81c
extern const Scripting::OpcodeInfo g_gameClearTextCommand;
// 0x0011e830
extern const Scripting::OpcodeInfo g_gameCreatePickupCommand;
// 0x0011e7a4
extern const Scripting::OpcodeInfo g_gameReleaseModelCommand;
// 0x0011eae4
extern const char g_gameParachuteLibrary[10];
// 0x00118224
extern const char g_gameVendingLibrary[8];
// 0x0011eac4
extern const char g_gameFightBLibrary[8];
// 0x0011eabc
extern const char g_gameFightCLibrary[8];
// 0x0011eab4
extern const char g_gameFightDLibrary[8];
// 0x0011eaac
extern const char g_gameFightELibrary[8];
#endif
