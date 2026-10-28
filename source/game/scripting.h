#ifndef SAMP_RECOVERED_SCRIPTING_H
#define SAMP_RECOVERED_SCRIPTING_H
#include <windows.h>
// Layout fields are not established yet; native Initialize allocates and clears 224 bytes.
struct CRunningScript
{
    BYTE opaque[224];
};
// samp.dll RVA: 0x002a93f0
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
// samp.dll RVA: 0x000b2150
void Initialize();
// samp.dll RVA: 0x000b1ed0
DWORD ExecBuffer();
// samp.dll RVA: 0x000b1f10
DWORD __cdecl ProcessCommand(const OpcodeInfo* command, ...);
} // namespace Scripting
// samp.dll RVA: 0x002a92f0
extern BYTE g_scriptBuffer[256];
// samp.dll RVA: 0x002a92a8
extern DWORD* g_scriptVariables[18];
// samp.dll RVA: 0x002a93f4
extern DWORD g_scriptLastOpcode;
// samp.dll RVA: 0x002a93f8
extern BOOL g_scriptLocalDebug;
// samp.dll RVA: 0x00155390
extern FARPROC g_scriptProcessOneCommand;
// samp.dll RVA: 0x00163a70
extern int g_gameExceptionCount;
// samp.dll RVA: 0x00060470
int __stdcall GameExceptionFilter(DWORD code, EXCEPTION_POINTERS* pointers, const char* warning);
#endif

#ifndef SAMP_PED_COMMANDS_H
#define SAMP_PED_COMMANDS_H
// samp.dll RVA: 0x0011f0d0
extern const Scripting::OpcodeInfo g_pedCommand087e;
// samp.dll RVA: 0x0011ef90
extern const Scripting::OpcodeInfo g_pedCommand0446;
// samp.dll RVA: 0x0011e604
extern const Scripting::OpcodeInfo g_entityTeleportCommand;
// samp.dll RVA: 0x0011f15c
extern const Scripting::OpcodeInfo g_pedDetachParachute;
// samp.dll RVA: 0x0011f148
extern const Scripting::OpcodeInfo g_pedDestroyParachute;
// samp.dll RVA: 0x0011f238
extern const Scripting::OpcodeInfo g_pedDropStuff;
// samp.dll RVA: 0x0011efcc
extern const Scripting::OpcodeInfo g_pedWarpFromVehicle;
// samp.dll RVA: 0x0011f320
extern const char g_pedNullAnimation[5];
// samp.dll RVA: 0x0011ef54
extern const Scripting::OpcodeInfo g_pedGetCurrentWeaponCommand;
// samp.dll RVA: 0x0011ef40
extern const Scripting::OpcodeInfo g_pedSetArmedWeaponCommand;
// samp.dll RVA: 0x0011f1c0
extern const Scripting::OpcodeInfo g_pedFightingStyleCommand;
// samp.dll RVA: 0x0011f198
extern const Scripting::OpcodeInfo g_pedCellphoneCommand;
// samp.dll RVA: 0x0011eef0
extern const Scripting::OpcodeInfo g_pedDeleteArrowCommand;
// samp.dll RVA: 0x0011ef18
extern const Scripting::OpcodeInfo g_pedSpawnInfoCommand;
// samp.dll RVA: 0x0011f1ac
extern const Scripting::OpcodeInfo g_pedHandsUpCommand;
// samp.dll RVA: 0x0011ef7c
extern const Scripting::OpcodeInfo g_pedImmunitiesCommand;
// samp.dll RVA: 0x0011f01c
extern const Scripting::OpcodeInfo g_pedRotationCommand;
// samp.dll RVA: 0x0011f1d4
extern const Scripting::OpcodeInfo g_pedParachuteCleanupCommand;
// samp.dll RVA: 0x0011ee8c
extern const Scripting::OpcodeInfo g_pedCreateArrowCommand;
// samp.dll RVA: 0x0011ef04
extern const Scripting::OpcodeInfo g_pedArrowColorCommand;
// samp.dll RVA: 0x0011f184
extern const Scripting::OpcodeInfo g_pedArrowModeCommand;
// samp.dll RVA: 0x0011ae68
extern const double g_pedParachuteZero;
// samp.dll RVA: 0x0011f1e8
extern const Scripting::OpcodeInfo g_pedStopUrinationEffectCommand;
// samp.dll RVA: 0x0011f008
extern const Scripting::OpcodeInfo g_pedExitVehicleCommand;
// samp.dll RVA: 0x0011f0a8
extern const Scripting::OpcodeInfo g_pedApplyAnimationCommand;
// samp.dll RVA: 0x0011f0bc
extern const Scripting::OpcodeInfo g_pedIsPerformingAnimationCommand;
// samp.dll RVA: 0x0011e324
extern const char g_pedBlockedAnimationLibrary[4];
// samp.dll RVA: 0x00154db0
extern char g_pedDanceLibraries[4][16];
// samp.dll RVA: 0x00154df0
extern char g_pedDanceNames[4][16];
// samp.dll RVA: 0x00154e30
extern char g_pedDanceMoveTables[3][16][16];
// samp.dll RVA: 0x00117a39
extern const char g_pedEmptyDanceAnimation[1];
// samp.dll RVA: 0x0011eec8
extern const Scripting::OpcodeInfo g_pedSelectInteriorCommand;
// samp.dll RVA: 0x0011f0f8
extern const Scripting::OpcodeInfo g_pedLinkInteriorCommand;
// samp.dll RVA: 0x0011f058
extern const Scripting::OpcodeInfo g_pedRefreshInteriorCommand;
// samp.dll RVA: 0x0011ef2c
extern const Scripting::OpcodeInfo g_pedControllableCommand;
// samp.dll RVA: 0x0011f044
extern const Scripting::OpcodeInfo g_pedFreezeCommand;
// samp.dll RVA: 0x0011f080
extern const Scripting::OpcodeInfo g_pedKillCommand;
// samp.dll RVA: 0x0011f06c
extern const Scripting::OpcodeInfo g_pedWarpPassengerCommand;
// samp.dll RVA: 0x0011eff4
extern const Scripting::OpcodeInfo g_pedEnterPassengerCommand;
// samp.dll RVA: 0x0011efe0
extern const Scripting::OpcodeInfo g_pedEnterDriverCommand;
// samp.dll RVA: 0x0011f1fc
extern const Scripting::OpcodeInfo g_pedCreateUrinationEffectCommand;
// samp.dll RVA: 0x0011f210
extern const Scripting::OpcodeInfo g_pedStartUrinationEffectCommand;
// samp.dll RVA: 0x0011eacc
extern const char g_pedUrinationLibrary[9];
// samp.dll RVA: 0x0011f378
extern const char g_pedUrinationAnimation[10];
// samp.dll RVA: 0x0011f354
extern const char g_pedUrinationEffect[10];
// samp.dll RVA: 0x0011f360
extern const double g_pedUrinationOffsetY;
// samp.dll RVA: 0x0011f368
extern const double g_pedUrinationOffsetZ;
// samp.dll RVA: 0x0011f370
extern const double g_pedUrinationDirectionY;
// samp.dll RVA: 0x0011ef68
extern const Scripting::OpcodeInfo g_pedWarpDriverCommand;
// samp.dll RVA: 0x0011eedc
extern const Scripting::OpcodeInfo g_pedTrainCameraCommand;
// samp.dll RVA: 0x0011ead8
extern const char g_pedSmokeLibrary[6];
// samp.dll RVA: 0x0011f348
extern const char g_pedSmokeAnimation[12];
// samp.dll RVA: 0x0011eae0
extern const char g_pedDrinkLibrary[4];
// samp.dll RVA: 0x0011f338
extern const char g_pedDrinkAnimationM[15];
// samp.dll RVA: 0x0011f328
extern const char g_pedDrinkAnimationF[15];
// samp.dll RVA: 0x0011f224
extern const Scripting::OpcodeInfo g_pedDrunkShakeCommand;
#endif

#ifndef SAMP_WRAPPER_COMMANDS_H
#define SAMP_WRAPPER_COMMANDS_H
// samp.dll RVA: 0x0011edfc
extern const Scripting::OpcodeInfo g_objectSetRotationCommand;
// samp.dll RVA: 0x0011e410
extern const Scripting::OpcodeInfo g_cameraFade;
// samp.dll RVA: 0x0011e424
extern const Scripting::OpcodeInfo g_cameraRestoreJumpcut;
// samp.dll RVA: 0x0011e438
extern const Scripting::OpcodeInfo g_cameraPointAt;
// samp.dll RVA: 0x0011e44c
extern const Scripting::OpcodeInfo g_cameraRestore;
// samp.dll RVA: 0x0011e460
extern const Scripting::OpcodeInfo g_cameraSet;
// samp.dll RVA: 0x0011e474
extern const Scripting::OpcodeInfo g_cameraStop;
// samp.dll RVA: 0x0011e488
extern const Scripting::OpcodeInfo g_cameraMove;
// samp.dll RVA: 0x0011e49c
extern const Scripting::OpcodeInfo g_cameraTrack;
// samp.dll RVA: 0x0011e4b0
extern const Scripting::OpcodeInfo g_cameraPersistTrack;
// samp.dll RVA: 0x0011e4c4
extern const Scripting::OpcodeInfo g_cameraPersistPosition;
// samp.dll RVA: 0x0011eaf0
extern const Scripting::OpcodeInfo g_audioPlay;
// samp.dll RVA: 0x0011eb04
extern const Scripting::OpcodeInfo g_audioLoad;
// samp.dll RVA: 0x0011eb40
extern const Scripting::OpcodeInfo g_audioUnload;
// samp.dll RVA: 0x0011eb54
extern const Scripting::OpcodeInfo g_audioSetPosition;
// samp.dll RVA: 0x0011eb18
extern const Scripting::OpcodeInfo g_audioIsLoaded;
// samp.dll RVA: 0x0011eb2c
extern const Scripting::OpcodeInfo g_audioStart;
// samp.dll RVA: 0x0011e68c
extern const Scripting::OpcodeInfo g_gameSetWorldTime;
// samp.dll RVA: 0x0011f7a4
extern const Scripting::OpcodeInfo g_vehicleChangeInteriorCommand;
// samp.dll RVA: 0x0011f678
extern const Scripting::OpcodeInfo g_vehicleIsSunkCommand;
// samp.dll RVA: 0x0011f7b8
extern const Scripting::OpcodeInfo g_vehicleAttachTrailerCommand;
// samp.dll RVA: 0x0011f7cc
extern const Scripting::OpcodeInfo g_vehicleDetachTrailerCommand;
// samp.dll RVA: 0x0011f77c
extern const Scripting::OpcodeInfo g_vehicleRemoveComponentCommand;
// samp.dll RVA: 0x0011f72c
extern const Scripting::OpcodeInfo g_vehiclePaintjobCommand;
// samp.dll RVA: 0x0011f614
extern const Scripting::OpcodeInfo g_vehicleRotationCommand;
// samp.dll RVA: 0x0011f6a0
extern const Scripting::OpcodeInfo g_vehicleProofsCommand;
// samp.dll RVA: 0x0011f790
extern const Scripting::OpcodeInfo g_vehicleDamageableCommand;
// samp.dll RVA: 0x0011f6b4
extern const Scripting::OpcodeInfo g_vehicleWarpPedCommand;
// samp.dll RVA: 0x0011f740
extern const Scripting::OpcodeInfo g_vehicleAddComponentCommand;
// samp.dll RVA: 0x0011f754
extern const Scripting::OpcodeInfo g_vehicleComponentLoadedCommand;
// samp.dll RVA: 0x0011f768
extern const Scripting::OpcodeInfo g_vehicleRequestComponentCommand;
// samp.dll RVA: 0x0011f628
extern const Scripting::OpcodeInfo g_vehicleCreateMarkerCommand;
// samp.dll RVA: 0x0011f63c
extern const Scripting::OpcodeInfo g_vehicleDestroyMarkerCommand;
// samp.dll RVA: 0x0011f650
extern const Scripting::OpcodeInfo g_vehicleMarkerColorCommand;
// samp.dll RVA: 0x0011f664
extern const Scripting::OpcodeInfo g_vehicleMarkerModeCommand;
// samp.dll RVA: 0x0011f600
extern const Scripting::OpcodeInfo g_vehicleDestroyCommand;
// samp.dll RVA: 0x0011f6dc
extern const Scripting::OpcodeInfo g_vehicleDestroyTrainCommand;
// samp.dll RVA: 0x0011f5ec
extern const Scripting::OpcodeInfo g_vehicleCreateCommand;
// samp.dll RVA: 0x0011f6f0
extern const Scripting::OpcodeInfo g_vehicleGasTankExplosionCommand;
// samp.dll RVA: 0x0011f718
extern const Scripting::OpcodeInfo g_vehicleHydraulicsCommand;
// samp.dll RVA: 0x0011f6c8
extern const Scripting::OpcodeInfo g_vehicleCreateTrainCommand;
// samp.dll RVA: 0x0011e790
extern const Scripting::OpcodeInfo g_gameDisableTrainTrafficCommand;
// samp.dll RVA: 0x0011e7e0
extern const Scripting::OpcodeInfo g_gameRequestAnimationCommand;
// samp.dll RVA: 0x0011e7f4
extern const Scripting::OpcodeInfo g_gameIsAnimationLoadedCommand;
// samp.dll RVA: 0x0011e6b4
extern const Scripting::OpcodeInfo g_gameDeleteMarkerCommand;
// samp.dll RVA: 0x0011e740
extern const Scripting::OpcodeInfo g_gameIncreaseMoneyCommand;
// samp.dll RVA: 0x0011e86c
extern const Scripting::OpcodeInfo g_gameZoneDisplayCommand;
// samp.dll RVA: 0x0011e754
extern const Scripting::OpcodeInfo g_gameFindGroundZCommand;
// samp.dll RVA: 0x0011e7cc
extern const Scripting::OpcodeInfo g_gameRefreshRendererCommand;
// samp.dll RVA: 0x0011e77c
extern const Scripting::OpcodeInfo g_gameDeleteRacingCheckpointCommand;
// samp.dll RVA: 0x0011e6dc
extern const Scripting::OpcodeInfo g_gameCreateMarkerCommand;
// samp.dll RVA: 0x0011e718
extern const Scripting::OpcodeInfo g_gameCreateMarkerType1Command;
// samp.dll RVA: 0x0011e858
extern const Scripting::OpcodeInfo g_gameCreateMarkerType2Command;
// samp.dll RVA: 0x0011e704
extern const Scripting::OpcodeInfo g_gameCreateMarkerType3Command;
// samp.dll RVA: 0x0011e6c8
extern const Scripting::OpcodeInfo g_gameMarkerColorCommand;
// samp.dll RVA: 0x0011e6f0
extern const Scripting::OpcodeInfo g_gameMarkerModeCommand;
// samp.dll RVA: 0x0011e768
extern const Scripting::OpcodeInfo g_gameCreateRacingCheckpointCommand;
// samp.dll RVA: 0x0011e72c
extern const Scripting::OpcodeInfo g_gameLocateCheckpointCommand;
// samp.dll RVA: 0x0011e81c
extern const Scripting::OpcodeInfo g_gameClearTextCommand;
// samp.dll RVA: 0x0011e830
extern const Scripting::OpcodeInfo g_gameCreatePickupCommand;
// samp.dll RVA: 0x0011e7a4
extern const Scripting::OpcodeInfo g_gameReleaseModelCommand;
// samp.dll RVA: 0x0011eae4
extern const char g_gameParachuteLibrary[10];
// samp.dll RVA: 0x00118224
extern const char g_gameVendingLibrary[8];
// samp.dll RVA: 0x0011eac4
extern const char g_gameFightBLibrary[8];
// samp.dll RVA: 0x0011eabc
extern const char g_gameFightCLibrary[8];
// samp.dll RVA: 0x0011eab4
extern const char g_gameFightDLibrary[8];
// samp.dll RVA: 0x0011eaac
extern const char g_gameFightELibrary[8];
#endif
