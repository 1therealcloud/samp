#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <windows.h>
// Initialize allocates and clears 224 bytes.
struct CRunningScript
{
    BYTE opaque[224];
};

extern CRunningScript* g_scriptThread;
namespace Scripting
{
struct OpcodeInfo
{
    WORD opcode;
    char parameters[16];
};

static_assert(sizeof(OpcodeInfo) == 18);

void Initialize();

DWORD ExecBuffer();

DWORD __cdecl ProcessCommand(const OpcodeInfo* command, ...);
} // namespace Scripting

extern BYTE g_scriptBuffer[256];

extern DWORD* g_scriptVariables[18];

extern DWORD g_scriptLastOpcode;

extern BOOL g_scriptLocalDebug;

extern FARPROC g_scriptProcessOneCommand;

extern int g_gameExceptionCount;

int __stdcall GameExceptionFilter(DWORD code, EXCEPTION_POINTERS* pointers, const char* warning);
#endif

#ifndef SAMP_PED_COMMANDS_H
#define SAMP_PED_COMMANDS_H

extern const Scripting::OpcodeInfo g_pedCommand087e;

extern const Scripting::OpcodeInfo g_pedCommand0446;

extern const Scripting::OpcodeInfo g_entityTeleportCommand;

extern const Scripting::OpcodeInfo g_pedDetachParachute;

extern const Scripting::OpcodeInfo g_pedDestroyParachute;

extern const Scripting::OpcodeInfo g_pedDropStuff;

extern const Scripting::OpcodeInfo g_pedWarpFromVehicle;

extern const char g_pedNullAnimation[5];

extern const Scripting::OpcodeInfo g_pedGetCurrentWeaponCommand;

extern const Scripting::OpcodeInfo g_pedSetArmedWeaponCommand;

extern const Scripting::OpcodeInfo g_pedFightingStyleCommand;

extern const Scripting::OpcodeInfo g_pedCellphoneCommand;

extern const Scripting::OpcodeInfo g_pedDeleteArrowCommand;

extern const Scripting::OpcodeInfo g_pedSpawnInfoCommand;

extern const Scripting::OpcodeInfo g_pedHandsUpCommand;

extern const Scripting::OpcodeInfo g_pedImmunitiesCommand;

extern const Scripting::OpcodeInfo g_pedRotationCommand;

extern const Scripting::OpcodeInfo g_pedParachuteCleanupCommand;

extern const Scripting::OpcodeInfo g_pedCreateArrowCommand;

extern const Scripting::OpcodeInfo g_pedArrowColorCommand;

extern const Scripting::OpcodeInfo g_pedArrowModeCommand;

extern const double g_pedParachuteZero;

extern const Scripting::OpcodeInfo g_pedStopUrinationEffectCommand;

extern const Scripting::OpcodeInfo g_pedExitVehicleCommand;

extern const Scripting::OpcodeInfo g_pedApplyAnimationCommand;

extern const Scripting::OpcodeInfo g_pedIsPerformingAnimationCommand;

extern const char g_pedBlockedAnimationLibrary[4];

extern char g_pedDanceLibraries[4][16];

extern char g_pedDanceNames[4][16];

extern char g_pedDanceMoveTables[3][16][16];

extern const char g_pedEmptyDanceAnimation[1];

extern const Scripting::OpcodeInfo g_pedSelectInteriorCommand;

extern const Scripting::OpcodeInfo g_pedLinkInteriorCommand;

extern const Scripting::OpcodeInfo g_pedRefreshInteriorCommand;

extern const Scripting::OpcodeInfo g_pedControllableCommand;

extern const Scripting::OpcodeInfo g_pedFreezeCommand;

extern const Scripting::OpcodeInfo g_pedKillCommand;

extern const Scripting::OpcodeInfo g_pedWarpPassengerCommand;

extern const Scripting::OpcodeInfo g_pedEnterPassengerCommand;

extern const Scripting::OpcodeInfo g_pedEnterDriverCommand;

extern const Scripting::OpcodeInfo g_pedCreateUrinationEffectCommand;

extern const Scripting::OpcodeInfo g_pedStartUrinationEffectCommand;

extern const char g_pedUrinationLibrary[9];

extern const char g_pedUrinationAnimation[10];

extern const char g_pedUrinationEffect[10];

extern const double g_pedUrinationOffsetY;

extern const double g_pedUrinationOffsetZ;

extern const double g_pedUrinationDirectionY;

extern const Scripting::OpcodeInfo g_pedWarpDriverCommand;

extern const Scripting::OpcodeInfo g_pedTrainCameraCommand;

extern const char g_pedSmokeLibrary[6];

extern const char g_pedSmokeAnimation[12];

extern const char g_pedDrinkLibrary[4];

extern const char g_pedDrinkAnimationM[15];

extern const char g_pedDrinkAnimationF[15];

extern const Scripting::OpcodeInfo g_pedDrunkShakeCommand;
#endif

#ifndef SAMP_WRAPPER_COMMANDS_H
#define SAMP_WRAPPER_COMMANDS_H

extern const Scripting::OpcodeInfo g_objectSetRotationCommand;

extern const Scripting::OpcodeInfo g_cameraFade;

extern const Scripting::OpcodeInfo g_cameraRestoreJumpcut;

extern const Scripting::OpcodeInfo g_cameraPointAt;

extern const Scripting::OpcodeInfo g_cameraRestore;

extern const Scripting::OpcodeInfo g_cameraSet;

extern const Scripting::OpcodeInfo g_cameraStop;

extern const Scripting::OpcodeInfo g_cameraMove;

extern const Scripting::OpcodeInfo g_cameraTrack;

extern const Scripting::OpcodeInfo g_cameraPersistTrack;

extern const Scripting::OpcodeInfo g_cameraPersistPosition;

extern const Scripting::OpcodeInfo g_audioPlay;

extern const Scripting::OpcodeInfo g_audioLoad;

extern const Scripting::OpcodeInfo g_audioUnload;

extern const Scripting::OpcodeInfo g_audioSetPosition;

extern const Scripting::OpcodeInfo g_audioIsLoaded;

extern const Scripting::OpcodeInfo g_audioStart;

extern const Scripting::OpcodeInfo g_gameSetWorldTime;

extern const Scripting::OpcodeInfo g_vehicleChangeInteriorCommand;

extern const Scripting::OpcodeInfo g_vehicleIsSunkCommand;

extern const Scripting::OpcodeInfo g_vehicleAttachTrailerCommand;

extern const Scripting::OpcodeInfo g_vehicleDetachTrailerCommand;

extern const Scripting::OpcodeInfo g_vehicleRemoveComponentCommand;

extern const Scripting::OpcodeInfo g_vehiclePaintjobCommand;

extern const Scripting::OpcodeInfo g_vehicleRotationCommand;

extern const Scripting::OpcodeInfo g_vehicleProofsCommand;

extern const Scripting::OpcodeInfo g_vehicleDamageableCommand;

extern const Scripting::OpcodeInfo g_vehicleWarpPedCommand;

extern const Scripting::OpcodeInfo g_vehicleAddComponentCommand;

extern const Scripting::OpcodeInfo g_vehicleComponentLoadedCommand;

extern const Scripting::OpcodeInfo g_vehicleRequestComponentCommand;

extern const Scripting::OpcodeInfo g_vehicleCreateMarkerCommand;

extern const Scripting::OpcodeInfo g_vehicleDestroyMarkerCommand;

extern const Scripting::OpcodeInfo g_vehicleMarkerColorCommand;

extern const Scripting::OpcodeInfo g_vehicleMarkerModeCommand;

extern const Scripting::OpcodeInfo g_vehicleDestroyCommand;

extern const Scripting::OpcodeInfo g_vehicleDestroyTrainCommand;

extern const Scripting::OpcodeInfo g_vehicleCreateCommand;

extern const Scripting::OpcodeInfo g_vehicleGasTankExplosionCommand;

extern const Scripting::OpcodeInfo g_vehicleHydraulicsCommand;

extern const Scripting::OpcodeInfo g_vehicleCreateTrainCommand;

extern const Scripting::OpcodeInfo g_gameDisableTrainTrafficCommand;

extern const Scripting::OpcodeInfo g_gameRequestAnimationCommand;

extern const Scripting::OpcodeInfo g_gameIsAnimationLoadedCommand;

extern const Scripting::OpcodeInfo g_gameDeleteMarkerCommand;

extern const Scripting::OpcodeInfo g_gameIncreaseMoneyCommand;

extern const Scripting::OpcodeInfo g_gameZoneDisplayCommand;

extern const Scripting::OpcodeInfo g_gameFindGroundZCommand;

extern const Scripting::OpcodeInfo g_gameRefreshRendererCommand;

extern const Scripting::OpcodeInfo g_gameDeleteRacingCheckpointCommand;

extern const Scripting::OpcodeInfo g_gameCreateMarkerCommand;

extern const Scripting::OpcodeInfo g_gameCreateMarkerType1Command;

extern const Scripting::OpcodeInfo g_gameCreateMarkerType2Command;

extern const Scripting::OpcodeInfo g_gameCreateMarkerType3Command;

extern const Scripting::OpcodeInfo g_gameMarkerColorCommand;

extern const Scripting::OpcodeInfo g_gameMarkerModeCommand;

extern const Scripting::OpcodeInfo g_gameCreateRacingCheckpointCommand;

extern const Scripting::OpcodeInfo g_gameLocateCheckpointCommand;

extern const Scripting::OpcodeInfo g_gameClearTextCommand;

extern const Scripting::OpcodeInfo g_gameCreatePickupCommand;

extern const Scripting::OpcodeInfo g_gameReleaseModelCommand;

extern const char g_gameParachuteLibrary[10];

extern const char g_gameVendingLibrary[8];

extern const char g_gameFightBLibrary[8];

extern const char g_gameFightCLibrary[8];

extern const char g_gameFightDLibrary[8];

extern const char g_gameFightELibrary[8];

#endif // SCRIPTING_H
