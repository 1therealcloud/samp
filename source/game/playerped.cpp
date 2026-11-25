#include "game/playerped.h"

#include "exceptions.h"
#include "game/aimstuff.h"
#include "game/animation.h"
#include "game/building_removal.h"
#include "game/custommodels.h"
#include "game/debug.h"
#include "game/entity.h"
#include "game/game.h"
#include "game/hooks.h"
#include "game/keystuff.h"
#include "game/object.h"
#include "game/scripting.h"
#include "game/task.h"
#include "game/textdraw.h"
#include "game/util.h"
#include "game/vehicle.h"
#include "gui/gui.h"
#include "gui/label.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

DWORD g_gamePlayerFlags[210];

DWORD g_sceneRasterDepthAndWidth[2];

DWORD g_gameLocalPedMatrix;

DWORD g_gameVehicleMatrix;

void* g_gamePlayerPeds[210];

BYTE g_gamePlayerControllerState[210][16];

void* g_pedCommandTaskSnapshot;

const float g_pedFloorRayLength = 1.75f;

DWORD g_pedVehicleEntryFlag;

DWORD g_pedDrunkVehicleTick;

const float g_pedDrunkShakeScale = 0.02f;

const float g_pedDrunkSpeedThreshold = 0.05f;

const Scripting::OpcodeInfo g_pedDetachParachute = {0x682, "ifffi"};

const Scripting::OpcodeInfo g_pedDestroyParachute = {0x9a2, "i"};

const Scripting::OpcodeInfo g_pedDropStuff = {0x70a, "iifffiissi"};

const Scripting::OpcodeInfo g_pedWarpFromVehicle = {0x362, "ifff"};

const char g_pedNullAnimation[5] = "NULL";

const Scripting::OpcodeInfo g_pedGetCurrentWeaponCommand = {0x470, "iv"};

const Scripting::OpcodeInfo g_pedSetArmedWeaponCommand = {0x1b9, "ii"};

const Scripting::OpcodeInfo g_pedCommand087e = {0x87e, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedCommand0446 = {0x446, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_entityTeleportCommand = {0x7c7, {0x69, 0x66, 0x66, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedFightingStyleCommand = {0x7fe, {0x69, 0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedCellphoneCommand = {0x729, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedDeleteArrowCommand = {0x164, {0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedSpawnInfoCommand = {0x16c, {0x66, 0x66, 0x66, 0x66, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedHandsUpCommand = {0x5c4, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedImmunitiesCommand = {0x2ab, {0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedRotationCommand = {0x173, {0x69, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedParachuteCleanupCommand = {0x108, {0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedCreateArrowCommand = {0x187, {0x69, 0x76, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedArrowColorCommand = {0x165, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedArrowModeCommand = {0x18b, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const double g_pedParachuteZero = 0.0;

const Scripting::OpcodeInfo g_pedStopUrinationEffectCommand = {0x650, {0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedExitVehicleCommand = {0x5cd, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedApplyAnimationCommand = {0x812, {0x69, 0x73, 0x73, 0x66, 0x69, 0x69, 0x69, 0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedIsPerformingAnimationCommand = {0x611, {0x69, 0x73, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const char g_pedBlockedAnimationLibrary[4] = {0x53, 0x45, 0x58, 0x0};

char g_pedDanceLibraries[4][16] = {0x57, 0x4f, 0x50, 0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x47, 0x46, 0x55, 0x4e, 0x4b, 0x0,
                                   0x0,  0x0,  0x0,  0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x52, 0x55, 0x4e, 0x4e, 0x49, 0x4e, 0x47, 0x4d, 0x41, 0x4e, 0x0,  0x0,
                                   0x0,  0x0,  0x0,  0x0, 0x53, 0x54, 0x52, 0x49, 0x50, 0x0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0};

char g_pedDanceNames[4][16] = {0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x4c, 0x4f, 0x4f, 0x50, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f,
                               0x4c, 0x4f, 0x4f, 0x50, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x4c, 0x4f, 0x4f, 0x50, 0x0,  0x0,
                               0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x4c, 0x6f, 0x6f, 0x70, 0x5f, 0x42, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0};

char g_pedDanceMoveTables[3][16][16] = {
    0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x32, 0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x33, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x34, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x35, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47,
    0x36, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x37, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43,
    0x45, 0x5f, 0x47, 0x38, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x39, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44,
    0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31, 0x30, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31, 0x31, 0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31, 0x32, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31, 0x33, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31, 0x34, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31,
    0x35, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x47, 0x31, 0x36, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45,
    0x5f, 0x42, 0x31, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x32, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41,
    0x4e, 0x43, 0x45, 0x5f, 0x42, 0x33, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x34, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x35, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x36, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x37, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x38, 0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x39, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f,
    0x42, 0x31, 0x30, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x31, 0x31, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e,
    0x43, 0x45, 0x5f, 0x42, 0x31, 0x32, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x31, 0x33, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x31, 0x34, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x31, 0x35, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x44, 0x41, 0x4e, 0x43, 0x45, 0x5f, 0x42, 0x31, 0x36, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x73, 0x74, 0x72, 0x69, 0x70, 0x5f, 0x41, 0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x73, 0x74, 0x72, 0x69, 0x70, 0x5f, 0x42, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x73, 0x74, 0x72, 0x69, 0x70, 0x5f, 0x43,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x73, 0x74, 0x72, 0x69, 0x70, 0x5f, 0x44, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x73, 0x74, 0x72, 0x69,
    0x70, 0x5f, 0x45, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x73, 0x74, 0x72, 0x69, 0x70, 0x5f, 0x46, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x73,
    0x74, 0x72, 0x69, 0x70, 0x5f, 0x47, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x41, 0x32, 0x42, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x42, 0x32, 0x41, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x42, 0x32, 0x43, 0x0,  0x0,  0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x43, 0x31, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x43, 0x32, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x43, 0x32, 0x42, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x41,
    0x32, 0x42, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54, 0x52, 0x5f, 0x42, 0x32, 0x43, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x53, 0x54,
    0x52, 0x5f, 0x43, 0x32, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0};

const char g_pedEmptyDanceAnimation[1] = {0x0};

const Scripting::OpcodeInfo g_pedSelectInteriorCommand = {0x4bb, {0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedLinkInteriorCommand = {0x860, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedRefreshInteriorCommand = {0x4e4, {0x66, 0x66, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedControllableCommand = {0x1b4, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedFreezeCommand = {0x4d7, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedKillCommand = {0x321, {0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedWarpPassengerCommand = {0x430, {0x69, 0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedEnterPassengerCommand = {0x5ca, {0x69, 0x69, 0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedEnterDriverCommand = {0x5cb, {0x69, 0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedCreateUrinationEffectCommand = {0x66a, {0x73, 0x69, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x69, 0x76, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedStartUrinationEffectCommand = {0x64c, {0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const char g_pedUrinationLibrary[9] = {0x50, 0x41, 0x55, 0x4c, 0x4e, 0x4d, 0x41, 0x43, 0x0};

const char g_pedUrinationAnimation[10] = {0x50, 0x49, 0x53, 0x53, 0x5f, 0x4c, 0x4f, 0x4f, 0x50, 0x0};

const char g_pedUrinationEffect[10] = {0x50, 0x45, 0x54, 0x52, 0x4f, 0x4c, 0x43, 0x41, 0x4e, 0x0};

const double g_pedUrinationOffsetY = 0.57999998331069946;

const double g_pedUrinationOffsetZ = -0.079999998211860657;

const double g_pedUrinationDirectionY = 0.0099999997764825821;

const Scripting::OpcodeInfo g_pedWarpDriverCommand = {0x36a, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedTrainCameraCommand = {0x158, {0x69, 0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const char g_pedSmokeLibrary[6] = {0x47, 0x41, 0x4e, 0x47, 0x53, 0x0};

const char g_pedSmokeAnimation[12] = {0x73, 0x6d, 0x6b, 0x63, 0x69, 0x67, 0x5f, 0x70, 0x72, 0x74, 0x6c, 0x0};

const char g_pedDrinkLibrary[4] = {0x42, 0x41, 0x52, 0x0};

const char g_pedDrinkAnimationM[15] = {0x64, 0x6e, 0x6b, 0x5f, 0x73, 0x74, 0x6e, 0x64, 0x4d, 0x5f, 0x6c, 0x6f, 0x6f, 0x70, 0x0};

const char g_pedDrinkAnimationF[15] = {0x64, 0x6e, 0x6b, 0x5f, 0x73, 0x74, 0x6e, 0x64, 0x46, 0x5f, 0x6c, 0x6f, 0x6f, 0x70, 0x0};

const Scripting::OpcodeInfo g_pedDrunkShakeCommand = {0x52c, {0x69, 0x69, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const Scripting::OpcodeInfo g_pedCreatePlayerCommand = {0x53, "vfffv"};

const Scripting::OpcodeInfo g_pedGetPlayerHandleCommand = {0x1f5, "vv"};

const Scripting::OpcodeInfo g_pedCommand03fe = {0x3fe, "ii"};

const float g_pedSpawnLift = 0.15f;

const Scripting::OpcodeInfo g_pedBehaviorWalkStyleCommand = {0x245, "is"};

const Scripting::OpcodeInfo g_pedBehaviorDrivebyCommand = {0x713, "iiiffffiii"};

const Scripting::OpcodeInfo g_pedBehaviorCreateObjectCommand = {0x107, "ifffv"};

const Scripting::OpcodeInfo g_pedBehaviorAttachCommand = {0x69b, "iiffffff"};

const Scripting::OpcodeInfo g_pedBehaviorObjectFlagCommand = {0x750, "ii"};

const Scripting::OpcodeInfo g_pedBehaviorObjectScalarCommand = {0x8d2, "if"};

const Scripting::OpcodeInfo g_pedBehaviorObjectAnimationCommand = {0x75a, "issfii"};

const Scripting::OpcodeInfo g_pedBehaviorEffectCommand = {0x18c, "fffi"};

const char g_pedParachuteOpen[10] = "PARA_OPEN";

const char g_pedParachuteDecel[11] = "PARA_DECEL";

const char g_pedParachuteFloat[11] = "PARA_FLOAT";

const char g_pedParachuteSteerRight[12] = "PARA_STEERR";

const char g_pedParachuteSteerLeft[12] = "PARA_STEERL";

const char g_pedParachuteLandWater[16] = "PARA_LAND_WATER";

const char g_pedParachuteOpenObject[12] = "PARA_OPEN_O";

const char g_pedParachuteDecelObject[13] = "PARA_DECEL_O";

const char g_pedParachuteFloatObject[13] = "PARA_FLOAT_O";

const char g_pedParachuteSteerRightObject[14] = "PARA_STEERR_O";

const char g_pedParachuteSteerLeftObject[14] = "PARA_STEERL_O";

const char g_pedParachuteLandWaterObject[18] = "PARA_LAND_WATER_O";

const double g_pedParachuteOne = 1.0;

const double g_pedParachuteOpeningSpeed = 1000.0;

const double g_pedDrivebyDistance = 300.0;

const double g_pedStuffOffset1X = 0.05000000074505806;

const double g_pedStuffOffset1Y = 0.029999999329447746;

const double g_pedStuffOffset1Z = -0.30000001192092896;

const double g_pedStuffOffset2Z = -0.05000000074505806;

const double g_pedStuffOffset3X = 0.03999999910593033;

const double g_pedStuffOffset3Y = 0.10000000149011612;

const double g_pedStuffOffset3Z = -0.019999999552965164;

static const DWORD g_pedDancePose[9][5] = {
    {0x00000009, 0xbf2b14fd, 0x3e38cda7, 0xbf38b8e1, 0xbc4c0bdd}, {0x0000000a, 0x3d2bd2b7, 0x3f07455a, 0x3d9ce0fc, 0x3f583090},
    {0x0000000b, 0xbdcc5543, 0x3f0e6245, 0x3e959429, 0x3f4586fd}, {0x0000000c, 0xbf05aa93, 0xbec5dfa8, 0xbee184a1, 0x3f1ea37b},
    {0x0000000d, 0xb97fda40, 0x00000000, 0x3e9b074a, 0x3f73fb8b}, {0x0000000f, 0x3f23e3c1, 0xbe08e326, 0xbf41388b, 0xbd510d39},
    {0x00000010, 0x3c69ff0d, 0xbf01e00d, 0x3de86163, 0x3f5aa876}, {0x00000011, 0x3e027aea, 0xbf0cee39, 0x3ea91eeb, 0x3f418bc6},
    {0x00000012, 0x3f1470da, 0x3eede311, 0xbe784a0e, 0x3f1fb0a3},
};

static const DWORD g_pedSecondaryPose[12][5] = {
    {0x00000007, 0x3d440e17, 0xbd80895d, 0x3f147b03, 0x3f4f8f69}, {0x00000009, 0xbf24ab2e, 0xbd978a2b, 0xbf3f8cfc, 0xbe140a28},
    {0x0000000e, 0x00000000, 0x00000000, 0x3dee03b4, 0x3f7e43ee}, {0x00000013, 0x397fda40, 0x00000000, 0x3e2d883c, 0x3f7c4c16},
    {0x0000000a, 0xbe05c422, 0x3f0c8440, 0xbde2070c, 0x3f517657}, {0x0000000b, 0x00000000, 0x00000000, 0xbf1f0175, 0x3f48a1cf},
    {0x0000000c, 0xbf7ca3d7, 0xbd98023a, 0x3cdc0336, 0xbe104253}, {0x0000000d, 0xb97fda40, 0x00000000, 0x3e2d883c, 0x3f7c4c16},
    {0x0000000f, 0x3f25b888, 0x3db5097d, 0xbf3d89be, 0xbe21c865}, {0x00000010, 0x3e368944, 0xbf13e76d, 0xbd8b06c4, 0x3f4b2a39},
    {0x00000011, 0x00000000, 0x00000000, 0xbf1228dd, 0x3f522cba}, {0x00000012, 0xbf768134, 0xbd170a3d, 0xbc68101f, 0x3e88a99b},
};

char g_pedShoppingName[32];

CPed* g_pedFiringContext;

void* g_pedFiringData;

const char g_pedBoneIdFormat[3] = "%d";

void* CPed::GetGtaActor()
{
    return m_pGamePed;
}

CPed::CPed()
{
    m_nHandle = 1;
    m_pGamePed = GameFindLocalPed();
    m_pGameEntity = GameFindLocalPed();
    m_nPlayerNumber = 0;
    GameAssignPlayerPed(0, m_pGamePed);
    Scripting::ProcessCommand(&g_pedCommand087e, m_nHandle, 1);
    Scripting::ProcessCommand(&g_pedCommand0446, m_nHandle, 0);
    unknown2a8 = 1;
    unknown2ac = 1;
    m_arrow = 0;
    m_parachuteObject = 0;
    m_bUrinating = 0;
    m_dancing = 0;
    m_bDancing = 0;
    m_danceStyle = 0;
    m_bUsingCellphone = 0;
    m_lastDanceMove = 0;
    m_stuffType = 0;
    m_stuffObject = 0;
    m_drunkLevel = 0;
    m_blockPedInputAt1C = 0;
    m_blockPedInputAt24 = 0;
    m_blockPedInputAt20 = 0;
    for (int i = 0; i < 10; i++)
    {
        m_accessoryOccupied[i] = FALSE;
        m_accessoryObjects[i] = 0;
        memset(&m_accessoryInfo[i], 0, 52);
    }
    m_aimMarker = 0;
}

CPed::~CPed()
{
    Destroy();
}

BOOL CPed::IsInVehicle()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return (BOOL)ped;
    return (*(DWORD*)(ped + 0x46c) >> 8) & 1;
}

BOOL CPed::IsPassenger()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    BYTE* vehicle = *(BYTE**)(ped + 0x58c);
    if (vehicle && (*(DWORD*)(ped + 0x46c) & 0x100) && (*(void**)(vehicle + 0x460) != ped || *(short*)(vehicle + 0x22) == 0x23a || *(short*)(vehicle + 0x22) == 0x239))
        return TRUE;
    return FALSE;
}

CPed* CGame::GetPlayerPed()
{
    if (!m_pPlayerPed)
        m_pPlayerPed = new CPed;
    return m_pPlayerPed;
}

BOOL CPed::UsingCellphone()
{
    return m_bUsingCellphone;
}

DWORD CPed::GetHeldObjectType()
{
    return m_stuffType;
}

BOOL CPed::DoesUrinating()
{
    return m_bUrinating;
}

BYTE CPed::GetState()
{
    return *((BYTE*)m_pGamePed + 0x530);
}

void* CPed::GetVehicle()
{
    return *(void**)((BYTE*)m_pGamePed + 0x58c);
}

void CPed::SetDrunkLevel(DWORD level)
{
    m_drunkLevel = level;
}

BOOL CPed::DoesDancing()
{
    return m_dancing != 0;
}

DWORD CPed::GetFlags()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return (DWORD)ped;
    return *(DWORD*)(ped + 0x46c);
}

void CPed::ResetPointers()
{
    m_pGamePed = GamePoolPedGetAt(m_nHandle);
    m_pGameEntity = m_pGamePed;
}

void CPed::SetHealth(float health)
{
    if (m_pGamePed)
        *(DWORD*)((BYTE*)m_pGamePed + 0x540) = *(DWORD*)&health;
}

void CPed::SetArmour(float armour)
{
    if (m_pGamePed)
        *(DWORD*)((BYTE*)m_pGamePed + 0x548) = *(DWORD*)&armour;
}

void CPed::SetFlags(DWORD flags)
{
    if (m_pGamePed)
        *(DWORD*)((BYTE*)m_pGamePed + 0x46c) = flags;
}

DWORD CPed::GetVehicleRef()
{
    if (!m_pGamePed)
        return 0;
    return GamePoolVehicleGetIndex(*(void**)((BYTE*)m_pGamePed + 0x58c));
}

float CPed::GetHealth()
{
    if (!m_pGamePed)
        return g_objectScaleLowerBound;
    return *(float*)((BYTE*)m_pGamePed + 0x540);
}

float CPed::GetArmour()
{
    if (!m_pGamePed)
        return g_objectScaleLowerBound;
    return *(float*)((BYTE*)m_pGamePed + 0x548);
}

void CPed::SetState(BYTE state)
{
    if (m_pGamePed)
        *(DWORD*)((BYTE*)m_pGamePed + 0x530) = state;
}

BOOL CPed::IsOnGround()
{
    return m_pGamePed && (*((BYTE*)m_pGamePed + 0x46c) & 3) ? TRUE : FALSE;
}

void* CPed::GetCurrentWeaponSlot()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    return ped ? ped + 0x5a0 + ped[0x718] * 0x1c : 0;
}

void CPed::SetAim(const CameraAim* aim)
{
    AimStuff::SetAim(m_nPlayerNumber, aim);
}

void CPed::SetInitialState()
{
    // GTA SA virtual address: 0x0060cd20, backend ECX, one callee-popped argument.
    void* ped = m_pGamePed;
    typedef void(__fastcall * BackendFn)(void*, int, DWORD);
    reinterpret_cast<BackendFn>(0x60cd20)((void*)ped, 0, 0);
}

const char* CPed::GetLoadedShoppingDataSubsection()
{
    if (!m_pGamePed)
        return 0;
    // GTA SA virtual address: 0x00a9a7d8; returns the address, not a pointer loaded there.
    return GamePoolPedGetAt(m_nHandle) ? (const char*)0xa9a7d8 : 0;
}

void CPed::RemoveWeaponWhenEnteringVehicle()
{
    void* ped = m_pGamePed;
    if (ped)
    {
        // GTA SA virtual address: 0x005e6370, backend ECX, argument0.
        typedef void(__fastcall * BackendFn)(void*, int, DWORD);
        reinterpret_cast<BackendFn>(0x5e6370)((void*)ped, 0, 0);
    }
}

void* CPed::GetAimedPed()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded vtable0x00863c40.
    if (!ped)
        return 0;
    if (*(DWORD*)ped == 0x863c40)
        return 0;
    if (!*(DWORD*)(ped + 0x18))
        return 0;
    return *(void**)(ped + 0x79c);
}

void* CPed::GetAccessory(int index)
{
    if (index < 0 || index >= 10 || !m_accessoryOccupied[index])
        return 0;
    return m_accessoryObjects[index];
}

void CPed::ResetDamageEntity()
{
    if (m_pGamePed)
    {
        *(DWORD*)((BYTE*)m_pGamePed + 0x764) = 0;
        *(DWORD*)((BYTE*)m_pGamePed + 0x760) = 255;
    }
}

void CPed::SetFightingStyle(int style)
{
    if (m_pGamePed)
        Scripting::ProcessCommand(&g_pedFightingStyleCommand, m_nHandle, style, 6);
}

BOOL CPed::GetAccessoryState(int index)
{
    if (index < 0 || index >= 10)
        return FALSE;
    return m_accessoryOccupied[index] == 1;
}

void CPed::EnableCellphone(BOOL enabled)
{
    if (m_pGamePed)
    {
        m_bUsingCellphone = enabled;
        Scripting::ProcessCommand(&g_pedCellphoneCommand, m_nHandle, enabled);
    }
}

void CPed::DeleteArrow()
{
    if (m_arrow)
    {
        Scripting::ProcessCommand(&g_pedDeleteArrowCommand, m_arrow);
    }
    m_arrow = 0;
}

void CPed::LoadShoppingDataSubsection(const char* name)
{
    const char* end = name;
    while (*end)
        ++end;
    if ((DWORD)(end - name) <= 8)
    {
        // GTA SA virtual address: 0x0049bbe0, cdecl.
        ((void(__cdecl*)(const char*))0x49bbe0)(name);
    }
}

void CPed::SetSpawnInfo(const CVector* position, float rotation)
{
    Scripting::ProcessCommand(&g_pedSpawnInfoCommand, (double)position->x, (double)position->y, (double)position->z, (double)rotation, 0);
}

BOOL CPed::IsDead()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return TRUE;
    return !(*(const float*)(ped + 0x540) > g_objectScaleLowerBound);
}

void* CPed::GetWeaponSlot(DWORD weapon)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (ped)
        for (BYTE index = 0; index < 13; ++index)
        {
            BYTE* slot = ped + 0x5a0 + index * 0x1c;
            if (*(DWORD*)slot == weapon)
                return slot;
        }
    return 0;
}

void CPed::SetWeaponAmmo(BYTE weapon, WORD ammo)
{
    if (m_pGamePed)
    {
        BYTE* slot = (BYTE*)GetWeaponSlot(weapon);
        if (slot)
            *(DWORD*)(slot + 0x0c) = ammo;
    }
}

BYTE CPed::GetAim()
{
    KeyPad* keys = KeyStuff::GetInternalKeys();
    if (keys->keys1[11])
        return 1;
    if (keys->keys1[10])
        return 2;
    BYTE mask = (keys->keys1[9] == 0) - 1;
    return mask & 3;
}

BOOL CPed::PerformingCustomAnimation()
{
    if (!m_pGamePed || !CEntity::DoesExist())
        return FALSE;
    BYTE* tasks = *(BYTE**)((BYTE*)m_pGamePed + 0x47c);
    return *(DWORD*)(tasks + 0x10) != 0;
}

BOOL CPed::HasJetpack()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || (*(DWORD*)(ped + 0x46c) & 0x100))
        return FALSE;
    BYTE* tasks = *(BYTE**)(ped + 0x47c);
    BYTE* task = *(BYTE**)(tasks + 0x10);
    // GTA SA virtual address: 0x008705c4, native task vtable comparison.
    if (!task)
        return FALSE;
    return *(DWORD*)task == 0x8705c4;
}

void CPed::StopDancing()
{
    CMatrix matrix;
    m_dancing = 0;
    GetMatrix(&matrix);
    Teleport(matrix.position.x, matrix.position.y, matrix.position.z);
}

float CPed::GetAimZ()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return g_objectScaleLowerBound;
    BYTE* state = *(BYTE**)(ped + 0x480);
    return *(float*)(state + 0x54);
}

BOOL CPed::CurrentWeaponHasAmmo()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return TRUE;
    BYTE* slot = (BYTE*)((DWORD)ped + 0x5a0 + *(ped + 0x718) * 0x1c);
    if (!slot)
        return TRUE;
    DWORD weapon = *(DWORD*)slot;
    if (weapon <= 15 || weapon == 46 || *(DWORD*)(slot + 0x0c))
        return TRUE;
    return FALSE;
}

void CPed::RemoveWeaponModel(int weapon)
{
    void* ped = m_pGamePed;
    if (ped)
    {
        BYTE* info = (BYTE*)g_pGame->GetWeaponInfo(weapon, 1);
        DWORD model = *(DWORD*)(info + 0x0c);
        // GTA SA virtual address: 0x005e3990, ECX original backend, argument model.
        typedef void(__fastcall * BackendFn)(void*, int, DWORD);
        reinterpret_cast<BackendFn>(0x5e3990)((void*)ped, 0, model);
    }
}

DWORD CPed::GetBonePosition(int bone, CVector* position)
{
    void* ped = m_pGamePed;
    // GTA SA virtual addresses: excluded vtable0x00863c40, callee0x005e4280.
    if (!ped || *(DWORD*)ped == 0x863c40)
        return 0;
    DWORD result;
    typedef DWORD(__fastcall * BackendFn)(void*, int, void*, DWORD, DWORD);
    result = (DWORD) reinterpret_cast<BackendFn>(0x5e4280)((void*)ped, 0, (void*)position, (DWORD)bone, 0);
    return result;
}

void CPed::ClearWeapons()
{
    void* ped = m_pGamePed;
    *g_clientCrashPedContext = m_nPlayerNumber;
    if (ped)
    {
        // GTA SA virtual address: 0x005e6320, unchecked backend ECX, no stack arguments.
        typedef void(__fastcall * BackendFn)(void*, int);
        reinterpret_cast<BackendFn>(0x5e6320)((void*)ped, 0);
    }
    *g_clientCrashPedContext = 0;
}

void CPed::EnableJetpack()
{
    if (m_pGamePed)
    {
        *g_clientCrashPedContext = m_nPlayerNumber;
        CMatrix* matrix = *(CMatrix**)((BYTE*)m_pGamePed + 0x14);
        Teleport(matrix->position);
        // GTA SA virtual address: 0x00439600, no stack arguments.
        ((void(__cdecl*)())0x439600)();
        *g_clientCrashPedContext = 0;
    }
}

void CPed::HandsUp()
{
    if (m_pGamePed && !(*(DWORD*)((BYTE*)m_pGamePed + 0x46c) & 0x100) && CEntity::DoesExist() && GamePoolPedGetAt(m_nHandle))
        Scripting::ProcessCommand(&g_pedHandsUpCommand, m_nHandle, -2);
}

void CPed::SetImmunities(DWORD bullet, DWORD fire, DWORD explosion, DWORD collision, DWORD melee)
{
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle))
        Scripting::ProcessCommand(&g_pedImmunitiesCommand, m_nHandle, bullet, fire, explosion, collision, melee);
}

void CPed::HoldObject(DWORD object)
{
    if (m_pGamePed && CEntity::DoesExist() && GamePoolPedGetAt(m_nHandle))
    {
        void* ped = m_pGamePed;
        // GTA SA virtual address: 0x005e4390, ECX backend, arguments object/1.
        typedef void(__fastcall * BackendFn)(void*, int, DWORD, DWORD);
        reinterpret_cast<BackendFn>(0x5e4390)((void*)ped, 0, object, 1);
    }
}

void CPed::SetAimZ(float value)
{
    if (!(value > g_vehicleTrainSpeedMaximum || value < g_vehicleTrainSpeedMinimum) && m_pGamePed)
    {
        BYTE* state = *(BYTE**)((BYTE*)m_pGamePed + 0x480);
        *(DWORD*)(state + 0x54) = *(DWORD*)&value;
    }
}

void CPed::ApplyCommandTask(const char* name, int p1, int p2, int p3, const CVector* p4, int p5, float p6, int p7, int p8, int p9)
{
    void* ped = m_pGamePed;
    if (ped)
    {
        // GTA SA virtual addresses: receiver0x00c15448, callee0x00618970.
        typedef void(__fastcall * BackendFn)(void*, int, void*, void*, DWORD, DWORD, DWORD, void*, DWORD, float, DWORD, DWORD, DWORD);
        reinterpret_cast<BackendFn>(0x618970)((void*)0xc15448, 0, (void*)name, ped, (DWORD)p1, (DWORD)p2, (DWORD)p3, (void*)p4, (DWORD)p5, p6, (DWORD)p7, (DWORD)p8, (DWORD)p9);
    }
}

void CPed::DestroyCommandTask()
{
    if (m_pGamePed)
    {
        BYTE* manager = *(BYTE**)((BYTE*)m_pGamePed + 0x47c);
        void* task = *(void**)(manager + 0x2c);
        g_pedCommandTaskSnapshot = task;
        if (task)
        {
            // clang-format off
            // GTA SA virtual address: 0x00639330, ECX published task, argument1.
            __asm { mov ecx,g_pedCommandTaskSnapshot
                mov edx,0x639330
                push 1
                call edx
            }
            // clang-format on
            manager = *(BYTE**)((BYTE*)m_pGamePed + 0x47c);
            *(DWORD*)(manager + 0x2c) = 0;
        }
    }
}

BOOL CPed::DoesHandsUp()
{
    if (m_pGamePed && !(*(DWORD*)((BYTE*)m_pGamePed + 0x46c) & 0x100) && CEntity::DoesExist() && GamePoolPedGetAt(m_nHandle))
    {
        BYTE* manager = *(BYTE**)((BYTE*)m_pGamePed + 0x47c);
        BYTE* task = *(BYTE**)(manager + 0x10);
        // GTA SA virtual address: task vtable0x0085a29c.
        return task && *(DWORD*)task == 0x85a29c ? TRUE : FALSE;
    }
    return FALSE;
}

void CPed::DisableJetpack()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (ped && !(*(DWORD*)(ped + 0x46c) & 0x100))
    {
        BYTE* manager = *(BYTE**)(ped + 0x47c);
        void* task = *(void**)(manager + 0x10);
        // GTA SA virtual addresses: vtable0x008705c4 and callee0x006801d0.
        if (task && *(DWORD*)task == 0x8705c4)
        {
            void* backendTask = task;
            typedef void(__fastcall * BackendFn)(void*, int, DWORD);
            reinterpret_cast<BackendFn>(0x6801d0)((void*)backendTask, 0, 1);
            manager = *(BYTE**)((BYTE*)m_pGamePed + 0x47c);
            *(DWORD*)(manager + 0x10) = 0;
        }
    }
}

void CPed::ForceRotation(float rotation)
{
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle))
    {
        *(float*)((BYTE*)m_pGamePed + 0x55c) = GameHeadingToRadians(rotation);
        *(float*)((BYTE*)m_pGamePed + 0x558) = GameHeadingToRadians(rotation);
    }
}

void CPed::SetRotation(float rotation)
{
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle))
    {
        *(float*)((BYTE*)m_pGamePed + 0x55c) = GameHeadingToRadians(rotation);
        *(float*)((BYTE*)m_pGamePed + 0x558) = GameHeadingToRadians(rotation);
        Scripting::ProcessCommand(&g_pedRotationCommand, m_nHandle, (double)rotation);
    }
}

void CPed::DeleteAccessory(int index)
{
    if (index < 0 || index >= 10 || m_accessoryOccupied[index] != 1)
        return;
    if (m_accessoryObjects[index])
    {
        delete (CEntity*)m_accessoryObjects[index];
        m_accessoryObjects[index] = 0;
    }
    memset(&m_accessoryInfo[index], 0, sizeof(m_accessoryInfo[index]));
    m_accessoryOccupied[index] = 0;
}

float CPed::GetDistanceToEntity(CEntity* entity)
{
    CMatrix first, second;
    GetMatrix(&first);
    entity->GetMatrix(&second);
    const double z = (double)first.position.z - second.position.z;
    const double y = (double)first.position.y - second.position.y;
    const double x = (double)first.position.x - second.position.x;
    return (float)sqrt(x * x + y * y + z * z);
}

void CPed::DestroyParachute()
{
    if (m_parachuteObject)
    {
        Scripting::ProcessCommand(&g_pedDetachParachute, m_parachuteObject, g_pedParachuteZero, g_pedParachuteZero, g_pedParachuteZero, 0);
        Scripting::ProcessCommand(&g_pedParachuteCleanupCommand, m_parachuteObject);
        m_parachuteObject = 0;
    }
}

void CPed::CreateArrow(DWORD color)
{
    if (m_arrow)
    {
        Scripting::ProcessCommand(&g_pedDeleteArrowCommand, m_arrow);
        m_arrow = 0;
    }
    Scripting::ProcessCommand(&g_pedCreateArrowCommand, m_nHandle, &m_arrow);
    Scripting::ProcessCommand(&g_pedArrowColorCommand, m_arrow, color);
    Scripting::ProcessCommand(&g_pedArrowModeCommand, m_arrow, 2);
}

int CPed::GetVehicleSeatIndex()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (*(ped + 0x530) != 50)
        return -1;
    BYTE* vehicle = *(BYTE**)(ped + 0x58c);
    if (!vehicle)
        return -1;
    for (int seat = 0; seat < 8; ++seat)
        if (*(void**)(vehicle + 0x460 + seat * 4) == ped)
            return seat;
    return -1;
}

void CPed::StopUrinating()
{
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle) && m_bUrinating)
    {
        DWORD* effect = (DWORD*)m_urinationEffectHandle;
        if (*effect)
        {
            Scripting::ProcessCommand(&g_pedStopUrinationEffectCommand, *effect);
            *effect = 0;
        }
        CMatrix matrix;
        GetMatrix(&matrix);
        Teleport(matrix.position.x, matrix.position.y, matrix.position.z);
        m_bUrinating = 0;
    }
}

void CPed::ExitVehicle()
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!(*(DWORD*)(ped + 0x46c) & 0x100))
        return;
    if (!GamePoolVehicleGetIndex(*(void**)(ped + 0x58c)))
        return;
    DWORD reference = 0;
    if (m_pGamePed)
        reference = GamePoolVehicleGetIndex(*(void**)((BYTE*)m_pGamePed + 0x58c));
    BYTE* vehicle = (BYTE*)GamePoolVehicleGetAt(reference);
    if (!vehicle)
        return;
    WORD model = *(WORD*)(vehicle + 0x22);
    if (model == 570 || model == 538)
        return;
    Scripting::ProcessCommand(&g_pedExitVehicleCommand, m_nHandle, GetVehicleRef());
}

void CPed::SetModelIndex(int model)
{
    void* ped = m_pGamePed;
    if (GamePoolPedGetAt(m_nHandle))
    {
        if (!GameIsPedModel(model))
            model = 0;
        if (ped)
        {
            // GTA SA virtual address: persistent one-byte patch0x005a82c0.
            UnFuck((void*)0x5a82c0, 1);
            *(BYTE*)0x5a82c0 = 0xc3;
            DestroyCommandTask();
            CEntity::SetModelIndex(model);
            // clang-format off
            // GTA SA virtual address: 0x004e68d0, ECX original backend+294, argument original backend.
            __asm { mov eax,ped
                lea ecx,[eax+0x294]
                push eax
                mov ebx,0x4e68d0
                call ebx
            }
            // clang-format on
        }
    }
}

void CPed::ApplyAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time)
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    if (!GameIsAnimationNameValid(library, name))
        return;
    if (_stricmp(library, g_pedBlockedAnimationLibrary) == 0)
        return;
    if (!g_pGame->IsAnimationLoaded(library))
    {
        g_pGame->RequestAnimation(library);
        if (!g_pGame->IsAnimationLoaded(library))
        {
            void(WINAPI* volatile sleep)(DWORD) = ::Sleep;
            int attempts = 0;
            do
            {
                sleep(1);
                if (++attempts == 15)
                    return;
            }
            while (!g_pGame->IsAnimationLoaded(library));
        }
    }
    Scripting::ProcessCommand(&g_pedApplyAnimationCommand, m_nHandle, name, library, (double)speed, loop, lockX, lockY, freeze, time);
}

BOOL CPed::IsPerformingAnimation(const char* name)
{
    if (m_pGamePed)
        return Scripting::ProcessCommand(&g_pedIsPerformingAnimationCommand, m_nHandle, name) != 0;
    return FALSE;
}

void CPed::StartDancing(int style)
{
    if (style < 0 || style > 3)
        return;
    m_dancing = 1;
    *(DWORD*)m_danceMoveState = style;
    if (m_nPlayerNumber == 0)
        ApplyAnimation(g_pedDanceNames[style], g_pedDanceLibraries[style], 16.0f, 1, 0, 0, 0, -1);
}

float CPed::GetRotation()
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return g_objectScaleLowerBound;
    CMatrix matrix;
    GetMatrix(&matrix);
    double degrees = atan2(-(double)matrix.up.x, (double)matrix.up.y) * g_entityRadiansToDegrees;
    if (degrees < g_objectScaleLowerBound)
        degrees += g_anglePeriod;
    else if (degrees >= g_anglePeriod)
        degrees -= g_anglePeriod;
    return (float)degrees;
}

__declspec(naked) void* CPed::GetCurrentWeaponInfo()
{
    // clang-format off
    // Preserve the native uninitialized output slot on a null-backend path.
    __asm { mov eax,[ecx+0x2a4]
        sub esp,8
        test eax,eax
        jz weapon_info_return
        test eax,eax
        jz no_weapon_slot
        movzx ecx,byte ptr [eax+0x718]
        imul ecx,ecx,0x1c
        lea eax,[ecx+eax+0x5a0]
        jmp query_weapon_info
    no_weapon_slot:
        xor eax,eax
    query_weapon_info:
        mov edx,[eax]
        mov ecx,g_pGame
        push ebx
        push 1
        push edx
        call CGame::GetWeaponInfo
        mov [esp+8],eax
        mov ebx,[esp+8]
        lea eax,[ebx+0x24]
        mov [esp+4],eax
        pop ebx
    weapon_info_return:
        mov eax,[esp]
        add esp,8
        ret
    }
    // clang-format on
}

DWORD CPed::GetModelTable28()
{
    if (!m_pGamePed)
        return 0xffffffff;
    return GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed + 0x22));
}

DWORD CPed::GetModelTable24()
{
    if (!m_pGamePed)
        return 0xffffffff;
    return GameReadPedModelTable24(*(short*)((BYTE*)m_pGamePed + 0x22));
}

const char* CPed::GetAnimationForDance(int move)
{
    if (m_dancing)
    {
        int style = *(int*)m_danceMoveState;
        DWORD offset = (DWORD)move << 4;
        if (style >= 0 && style <= 2)
        {
            if ((m_pGamePed && GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed + 0x22)) == 5) || GetModelTable28() == 22)
                return (const char*)((DWORD)g_pedDanceMoveTables + offset);
            return (const char*)((DWORD)g_pedDanceMoveTables + 0x100 + offset);
        }
        if (style == 3)
            return (const char*)((DWORD)g_pedDanceMoveTables + 0x200 + offset);
    }
    return g_pedEmptyDanceAnimation;
}

void CPed::LinkToInterior(BYTE interior, BOOL refresh)
{
    if (!m_pGamePed)
        return;
    if (m_nPlayerNumber == 0)
    {
        Scripting::ProcessCommand(&g_pedSelectInteriorCommand, (DWORD)interior);
        Scripting::ProcessCommand(&g_pedLinkInteriorCommand, m_nHandle, (DWORD)interior);
        if (refresh)
        {
            CMatrix matrix;
            GetMatrix(&matrix);
            Scripting::ProcessCommand(&g_pedRefreshInteriorCommand, (double)matrix.position.x, (double)matrix.position.y);
        }
    }
    else
        Scripting::ProcessCommand(&g_pedLinkInteriorCommand, m_nHandle, (DWORD)interior);
}

void CPed::SetControllable(BOOL enabled)
{
    if (!GamePoolPedGetAt(m_nHandle))
        return;
    if (!enabled)
    {
        Scripting::ProcessCommand(&g_pedControllableCommand, (DWORD)m_nPlayerNumber, 0);
        Scripting::ProcessCommand(&g_pedFreezeCommand, m_nHandle, 1);
    }
    else
    {
        Scripting::ProcessCommand(&g_pedControllableCommand, (DWORD)m_nPlayerNumber, 1);
        Scripting::ProcessCommand(&g_pedFreezeCommand, m_nHandle, 0);
        if (m_pGamePed && (*(DWORD*)((BYTE*)m_pGamePed + 0x46c) & 0x100))
            return;
        CMatrix matrix;
        GetMatrix(&matrix);
        Teleport(matrix.position.x, matrix.position.y, matrix.position.z);
    }
}

void CPed::ExtinguishPedAndVehicleFire()
{
    if (m_pGamePed)
    {
        void* state = *(void**)((BYTE*)m_pGamePed + 0x730);
        if (state)
        {
            // GTA SA virtual address: 0x005393f0, ECX raw state/no arguments.
            typedef void(__fastcall * BackendFn)(void*, int);
            reinterpret_cast<BackendFn>(0x5393f0)((void*)state, 0);
        }
        BYTE* ped = (BYTE*)m_pGamePed;
        if (*(DWORD*)(ped + 0x46c) & 0x100)
        {
            BYTE* vehicle = *(BYTE**)(ped + 0x58c);
            if (vehicle)
            {
                state = *(void**)(vehicle + 0x490);
                if (state)
                {
                    typedef void(__fastcall * BackendFn2)(void*, int);
                    reinterpret_cast<BackendFn2>(0x5393f0)((void*)state, 0);
                }
            }
        }
    }
}

void CPed::Kill()
{
    if (!m_nHandle || !m_pGamePed)
        return;
    if (HasJetpack())
        DisableJetpack();
    ExtinguishPedAndVehicleFire();
    CMatrix matrix;
    GetMatrix(&matrix);
    Teleport(matrix.position);
    *(DWORD*)((BYTE*)m_pGamePed + 0x540) = 0;
    *g_clientCrashPedContext = m_nPlayerNumber;
    Scripting::ProcessCommand(&g_pedKillCommand, m_nHandle);
    *g_clientCrashPedContext = 0;
}

void* CPed::GetFloor()
{
    const BYTE* ped = (const BYTE*)m_pGamePed;
    if (!ped || (*(const DWORD*)(ped + 0x46c) & 0x100) || !GamePoolPedGetAt(m_nHandle))
        return 0;

    const CMatrix* matrix = *(const CMatrix* const*)((const BYTE*)m_pGamePed + 0x14);
    CVector start, end;
    end.x = start.x = matrix->position.x;
    end.y = start.y = matrix->position.y;
    const volatile float startZ = matrix->position.z - g_vehicleCreationLift;
    start.z = startZ;
    end.z = startZ - g_pedFloorRayLength;
    BYTE collision[0x2c];
    void* entity;
    GameProcessLineOfSightRaw(&start, &end, collision, &entity, 0, 1, 0, 1, 0, 0, 0, 0);
    return entity;
}

void CPed::EnterVehicle(int vehicle, BOOL passenger)
{
    if (!m_pGamePed)
        return;
    BYTE* backend = (BYTE*)GamePoolVehicleGetAt(vehicle);
    if (!backend || !GamePoolPedGetAt(m_nHandle))
        return;
    g_pedVehicleEntryFlag = 1;
    if (GetCurrentWeapon() == 46)
        SetArmedWeapon(0, 0);
    if (passenger)
    {
        if (*(WORD*)(backend + 0x22) == 570 && m_pGamePed == GameFindLocalPed())
            Scripting::ProcessCommand(&g_pedWarpPassengerCommand, m_nHandle, vehicle, -1);
        else
            Scripting::ProcessCommand(&g_pedEnterPassengerCommand, m_nHandle, vehicle, 3000, -1);
    }
    else
        Scripting::ProcessCommand(&g_pedEnterDriverCommand, m_nHandle, vehicle, 3000);
}

void CPed::StartUrinating()
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle) || m_bUrinating)
        return;
    if (m_nPlayerNumber == 0)
        ApplyAnimation(g_pedUrinationAnimation, g_pedUrinationLibrary, 4.0f, 1, 0, 0, 0, -1);
    Scripting::ProcessCommand(&g_pedCreateUrinationEffectCommand, g_pedUrinationEffect, m_nHandle, g_pedParachuteZero, g_pedUrinationOffsetY, g_pedUrinationOffsetZ,
                              g_pedParachuteZero, g_pedUrinationDirectionY, g_pedParachuteZero, 1, (DWORD*)m_urinationEffectHandle);
    Scripting::ProcessCommand(&g_pedStartUrinationEffectCommand, *(DWORD*)m_urinationEffectHandle);
    m_bUrinating = 1;
}

void CPed::PutIntoVehicle(int vehicle, int seat)
{
    if (!m_pGamePed || !GamePoolVehicleGetAt(vehicle) || !GamePoolPedGetAt(m_nHandle))
        return;
    if (GetCurrentWeapon() == 46)
        SetArmedWeapon(0, 0);
    BYTE* backend = (BYTE*)GamePoolVehicleGetAt(vehicle);
    const bool reject = *(const float*)(backend + 0x4c0) == g_objectScaleLowerBound;
    // GTA SA virtual address: excluded vehicle vtable0x00863c40.
    if (reject || *(DWORD*)backend == 0x863c40)
        return;
    if ((GameGetVehicleSubtype(backend) == 1 || GameGetVehicleSubtype(backend) == 2) && seat > (int)*(backend + 0x488))
        return;
    if (seat == 0)
    {
        BYTE* driver = *(BYTE**)(backend + 0x460);
        if (driver && (*(DWORD*)(driver + 0x46c) & 0x100))
            return;
        Scripting::ProcessCommand(&g_pedWarpDriverCommand, m_nHandle, vehicle);
    }
    else
        Scripting::ProcessCommand(&g_pedWarpPassengerCommand, m_nHandle, vehicle, (DWORD)seat - 1u);
    void* ped = m_pGamePed;
    if (ped == GameFindLocalPed() && (*(DWORD*)((BYTE*)ped + 0x46c) & 0x100))
        g_pGame->m_pCamera->Restore();
    if (g_pNetGame)
    {
        BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
        CVehiclePool* pool = *(CVehiclePool**)(pools + 0xc);
        WORD id = pool->Find(backend);
        if (id == 0xffff || id > 2000)
            return;
        CVehicle* wrapper = pool->Get(id);
        if (wrapper && wrapper->IsTrainPart() && m_pGamePed == GameFindLocalPed())
            Scripting::ProcessCommand(&g_pedTrainCameraCommand, wrapper->m_nHandle, 3, 2);
    }
}

BOOL CPed::ApplyStuff()
{
    if (!m_stuffType || !m_pGamePed || !CEntity::DoesExist())
        return FALSE;
    KeyPad* keys = m_nPlayerNumber ? KeyStuff::GetPlayerKeys(m_nPlayerNumber) : KeyStuff::GetInternalKeys();
    SetArmedWeapon(0, 0);
    if (m_nPlayerNumber || PerformingCustomAnimation() || !keys->keys1[17])
        return FALSE;
    DWORD type = m_stuffType;
    if (type == 4)
    {
        ApplyAnimation(g_pedSmokeAnimation, g_pedSmokeLibrary, 700.0f, 0, 0, 0, 0, 2750);
        return TRUE;
    }
    if (type == 1 || type == 2 || type == 3)
    {
        if (GetModelTable28() == 5 || GetModelTable28() == 22)
            ApplyAnimation(g_pedDrinkAnimationF, g_pedDrinkLibrary, 4.0f, 0, 0, 0, 0, -1);
        else
            ApplyAnimation(g_pedDrinkAnimationM, g_pedDrinkLibrary, 4.0f, 0, 0, 0, 0, -1);
        // Native reload after ApplyAnimation, modular add followed by signed upper bound.
        if (m_stuffType != 3)
        {
            DWORD level = m_drunkLevel + 1250u;
            m_drunkLevel = level;
            if ((int)level > 50000)
                m_drunkLevel = 50000;
        }
        return TRUE;
    }
    return FALSE;
}

WORD CPed::GetKeys(WORD* leftRight, WORD* upDown)
{
    KeyPad* keys = KeyStuff::GetInternalKeys();
    *leftRight = keys->keys1[0];
    *upDown = keys->keys1[1];
    short horizontal = (short)keys->keys1[2];
    short vertical = (short)keys->keys1[3];
    DWORD bits = 0;
    if (horizontal > 0)
        bits |= 0x4000;
    if (horizontal < 0)
        bits |= 0x2000;
    if (vertical > 0)
        bits |= 0x1000;
    if (vertical < 0)
        bits |= 0x0800;
    if (keys->keys1[21])
        bits |= 0x0400;
    if (keys->keys1[19])
        bits |= 0x0200;
    BYTE* backend = (BYTE*)m_pGamePed;
    if (backend && (*(volatile DWORD*)(backend + 0x46c) & 0x100) && keys->keys1[5])
        bits |= 0x0100;
    if (keys->keys1[6])
        bits |= 0x0080;
    if (backend && (*(volatile DWORD*)(backend + 0x46c) & 0x100) && keys->keys1[7])
        bits |= 0x0040;
    if (keys->keys1[14])
        bits |= 0x0020;
    BOOL suppress = FALSE;
    if (backend && !(*(volatile DWORD*)(backend + 0x46c) & 0x100))
    {
        BYTE* manager = *(BYTE**)(backend + 0x47c);
        BYTE* task = *(BYTE**)(manager + 0x10);
        // GTA SA virtual address: native jetpack task vtable0x008705c4.
        suppress = task && *(DWORD*)task == 0x8705c4;
    }
    if (!suppress && keys->keys1[15])
        bits |= 0x0010;
    if (keys->keys1[16])
        bits |= 0x0008;
    BOOL fire = TRUE;
    if (backend)
    {
        // Native reload is separate from the earlier cached nonnull gate.
        BYTE* current = (BYTE*)*(void* volatile*)&m_pGamePed;
        BYTE* slot = (BYTE*)((DWORD)current + 0x5a0 + *(current + 0x718) * 0x1c);
        if (slot)
        {
            DWORD weapon = *(DWORD*)slot;
            if (weapon > 15 && weapon != 46 && !*(DWORD*)(slot + 0x0c))
                fire = FALSE;
        }
    }
    if (fire && keys->keys1[17])
        bits |= 0x0004;
    if (keys->keys1[18])
        bits |= 0x0002;
    if (keys->keys1[4])
        bits |= 0x0001;
    return (WORD)bits;
}

void CPed::SetKeys(WORD mask, WORD leftRight, WORD upDown)
{
    KeyPad* keys = KeyStuff::GetPlayerKeys(m_nPlayerNumber);
    BOOL held = keys->keys1[0] != 0;
    keys->keys1[0] = leftRight;
    keys->keys2[0] = held && leftRight ? leftRight : 0;
    held = keys->keys1[1] != 0;
    keys->keys1[1] = upDown;
    keys->keys2[1] = held && upDown ? upDown : 0;
    if ((mask & 0x0001))
    {
        WORD previous = keys->keys1[4];
        keys->keys1[4] = 255;
        keys->keys2[4] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[4] = 0;
        keys->keys2[4] = 0;
    }
    BYTE* backend = (BYTE*)*(void* volatile*)&m_pGamePed;
    if (backend && (*(volatile DWORD*)(backend + 0x46c) & 0x100) && (mask & 0x0002))
    {
        WORD previous = keys->keys1[18];
        keys->keys1[18] = 255;
        keys->keys2[18] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[18] = 0;
        keys->keys2[18] = 0;
    }
    if ((mask & 0x0004))
    {
        WORD previous = keys->keys1[17];
        keys->keys1[17] = 255;
        keys->keys2[17] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[17] = 0;
        keys->keys2[17] = 0;
    }
    if ((mask & 0x0008))
    {
        WORD previous = keys->keys1[16];
        keys->keys1[16] = 255;
        keys->keys2[16] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[16] = 0;
        keys->keys2[16] = 0;
    }
    BOOL down = (mask & 0x0010) != 0;
    if (down)
    {
        BYTE* current = (BYTE*)*(void* volatile*)&m_pGamePed;
        if (current && !(*(volatile DWORD*)(current + 0x46c) & 0x100))
        {
            BYTE* manager = *(BYTE**)(current + 0x47c);
            BYTE* task = *(BYTE**)(manager + 0x10);
            // GTA SA virtual address: jetpack task vtable0x008705c4.
            if (task && *(DWORD*)task == 0x8705c4)
                down = FALSE;
        }
    }
    if (down)
    {
        WORD previous = keys->keys1[15];
        keys->keys1[15] = 255;
        keys->keys2[15] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[15] = 0;
        keys->keys2[15] = 0;
    }
    if ((mask & 0x0020))
    {
        WORD previous = keys->keys1[14];
        keys->keys1[14] = 255;
        keys->keys2[14] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[14] = 0;
        keys->keys2[14] = 0;
    }
    if ((mask & 0x0040))
    {
        WORD previous = keys->keys1[7];
        keys->keys1[7] = 255;
        keys->keys2[7] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[7] = 0;
        keys->keys2[7] = 0;
    }
    if ((mask & 0x0080))
    {
        WORD previous = keys->keys1[6];
        keys->keys1[6] = 255;
        keys->keys2[6] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[6] = 0;
        keys->keys2[6] = 0;
    }
    if ((mask & 0x0100))
    {
        WORD previous = keys->keys1[5];
        keys->keys1[5] = 255;
        keys->keys2[5] = previous != 0 ? 255 : 0;
    }
    else
    {
        keys->keys1[5] = 0;
        keys->keys2[5] = 0;
    }
    if ((mask & 0x0200))
    {
        WORD previous = keys->keys1[19];
        keys->keys1[19] = 255;
        keys->keys2[19] = previous == 255 ? 255 : 0;
    }
    else
    {
        keys->keys1[19] = 0;
        keys->keys2[19] = 0;
    }
    if ((mask & 0x0400))
    {
        WORD previous = keys->keys1[21];
        keys->keys1[21] = 255;
        keys->keys2[21] = previous == 255 ? 255 : 0;
    }
    else
    {
        keys->keys1[21] = 0;
        keys->keys2[21] = 0;
    }
    WORD direction = (mask >> 11) & 3;
    WORD axis = direction == 2 ? 0x0080 : direction == 1 ? 0xff80 : 0;
    keys->keys1[3] = axis;
    keys->keys2[3] = axis;
    direction = (mask >> 13) & 3;
    axis = direction == 2 ? 0x0080 : direction == 1 ? 0xff80 : 0;
    keys->keys1[2] = axis;
    keys->keys2[2] = axis;
    KeyStuff::SetKeys(m_nPlayerNumber, keys);
}

void CPed::ProcessDrunk()
{
    int level = (int)m_drunkLevel;
    DWORD deltaBits = 0;
    if (level > 2000)
    {
        if (m_nPlayerNumber)
            return;
        int shake = (int)(__int64)((double)level * g_pedDrunkShakeScale);
        if (shake > 250)
            shake = 250;
        else if (shake < 5)
            shake = 0;
        Scripting::ProcessCommand(&g_pedDrunkShakeCommand, 0, shake);
        if ((int)m_drunkLevel > 2000)
        {
            BYTE* ped = (BYTE*)m_pGamePed;
            if (ped && (*(DWORD*)(ped + 0x46c) & 0x100) && !IsPassenger())
            {
                BYTE* vehicle = *(BYTE**)(ped + 0x58c);
                if (vehicle)
                {
                    DWORD previous = g_pedDrunkVehicleTick;
                    DWORD(WINAPI* volatile tick)() = ::GetTickCount;
                    BOOL process = TRUE;
                    if (previous)
                    {
                        DWORD now = tick();
                        process = now - g_pedDrunkVehicleTick > 200u;
                    }
                    if (process)
                    {
                        int outcome = rand() % 40;
                        if (outcome >= 20)
                        {
                            if (outcome > 30)
                                deltaBits = (int)m_drunkLevel < 5000 ? 0x3c449ba6 : 0x3c75c28f;
                            else
                                deltaBits = (int)m_drunkLevel < 5000 ? 0xbc449ba6 : 0xbc75c28f;
                        }
                        if (FloatOffset(*(const float*)(vehicle + 0x44), 0.0f) > g_pedDrunkSpeedThreshold ||
                            FloatOffset(*(const float*)(vehicle + 0x48), 0.0f) > g_pedDrunkSpeedThreshold)
                        {
                            float delta;
                            memcpy(&delta, &deltaBits, sizeof(delta));
                            *(float*)(vehicle + 0x58) = delta + *(float*)(vehicle + 0x58);
                        }
                        g_pedDrunkVehicleTick = tick();
                    }
                }
            }
        }
        --m_drunkLevel;
        return;
    }
    if (!m_nPlayerNumber)
    {
        if (level > 0)
            m_drunkLevel = (DWORD)(level - 1);
        Scripting::ProcessCommand(&g_pedDrunkShakeCommand, 0, 0);
    }
}

void* __stdcall GamePoolPedGetAt(int handle)
{
    void* result;
    // GTA SA thiscall backend. The pool goes in ECX and the handle on the stack.
    typedef void*(__fastcall * BackendFn)(void*, int, DWORD);
    result = (void*)reinterpret_cast<BackendFn>(0x404910)(*(void**)0xb74490, 0, (DWORD)handle);
    return result;
}

BOOL CPed::HasAccessory()
{
    for (int i = 0; i < 10; ++i)
        if (m_accessoryOccupied[i] == 1)
            return TRUE;
    return FALSE;
}

void CPed::DeleteAllAccessories()
{
    for (int i = 0; i < 10; ++i)
        if (m_accessoryOccupied[i] == 1)
        {
            void* object = m_accessoryObjects[i];
            if (object)
            {
                // Observed scalar-deleting virtual entry; no complete CObject ABI assumed.
                ((void(__fastcall*)(void*, void*, int))(*(DWORD**)object)[0])(object, 0, 1);
                m_accessoryObjects[i] = 0;
            }
            memset(&m_accessoryInfo[i], 0, 52);
            m_accessoryOccupied[i] = FALSE;
        }
}

void CPed::DropStuff()
{
    if (m_stuffObject)
    {
        Scripting::ProcessCommand(&g_pedDropStuff, m_nHandle, m_stuffObject, 0.0, 0.0, 0.0, 6, 16, g_pedNullAnimation, g_pedNullAnimation, 0);
        m_stuffObject = 0;
    }
    CMatrix matrix;
    GetMatrix(&matrix);
    Teleport(matrix.position.x, matrix.position.y, matrix.position.z);
    m_stuffType = 0;
}

void CPed::WarpFromVehicle(float x, float y, float z)
{
    if (GamePoolPedGetAt(m_nHandle) && m_pGamePed && (*(DWORD*)((BYTE*)m_pGamePed + 0x46c) & 0x100))
        Scripting::ProcessCommand(&g_pedWarpFromVehicle, m_nHandle, (double)x, (double)y, (double)z);
}

void CPed::Destroy()
{
    BYTE* original = (BYTE*)m_pGamePed;
    KeyStuff::ResetKeys(m_nPlayerNumber);
    GameAssignPlayerPed(m_nPlayerNumber, 0);
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle) && *(DWORD*)m_pGamePed != 0x863c40)
    {
        if (m_parachuteObject)
        {
            Scripting::ProcessCommand(&g_pedDetachParachute, m_parachuteObject, 0.0, 0.0, 0.0, 0);
            Scripting::ProcessCommand(&g_pedDestroyParachute, m_parachuteObject);
            m_parachuteObject = 0;
        }
        if (HasAccessory())
            DeleteAllAccessories();
        if (m_stuffType)
            DropStuff();
        if (*(DWORD*)((BYTE*)m_pGamePed + 0x46c) & 0x100)
            WarpFromVehicle(100.0f, 100.0f, 10.0f);
        *(DWORD*)(*(BYTE**)(original + 0x480) + 0x4c) = 0;
        ((void(__fastcall*)(void*, void*, int))(*(DWORD**)original)[0])(original, 0, 1);
        m_pGamePed = 0;
        m_pGameEntity = 0;
        // Native successful deletion deliberately retains m_nHandle.
        return;
    }
    m_pGamePed = 0;
    m_pGameEntity = 0;
    m_nHandle = 0;
}

BOOL CPed::HasDanceState()
{
    return m_bDancing != 0;
}

BOOL CPed::HasDanceStyle()
{
    return m_danceStyle != 0;
}

DWORD CPed::GetCurrentWeapon()
{
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle))
    {
        DWORD weapon = (DWORD)this;
        Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand, m_nHandle, &weapon);
        return weapon & 0xff;
    }
    return 0;
}

void CPed::SetArmedWeapon(DWORD weapon, BYTE direct)
{
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle))
    {
        *g_clientCrashPedContext = m_nPlayerNumber;
        AimStuff::UpdateLocalSkills();
        AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
        void* ped = m_pGamePed;
        if ((!ped || !(*(DWORD*)((BYTE*)ped + 0x46c) & 0x100)) && !direct)
        {
            Scripting::ProcessCommand(&g_pedSetArmedWeaponCommand, m_nHandle, weapon);
            AimStuff::ApplyLocalSkills();
            *g_clientCrashPedContext = 0;
            return;
        }
        if (ped)
            ((void(__fastcall*)(void*, void*, DWORD))0x5e6280)(ped, 0, weapon);
        AimStuff::ApplyLocalSkills();
        *g_clientCrashPedContext = 0;
    }
}

DWORD CPed::GetDrunkLevel()
{
    return m_drunkLevel;
}

PedAccessoryState* CPed::GetAccessoryInfo(int index)
{
    if (index < 0 || index >= 10 || !m_accessoryOccupied[index])
        return 0;
    return &m_accessoryInfo[index];
}

void CPed::GetBoneMatrixById(CMatrix* out, int bone)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded vtable 0x00863c40.
    if (!ped || *(DWORD*)ped == 0x863c40)
        return;
    void* clump = *(void**)(ped + 0x18);
    if (!clump)
        return;
    typedef BYTE*(__cdecl * GetHierarchy)(void*);
    typedef int(__cdecl * GetBoneIndex)(void*, int);
    // GTA SA virtual addresses: hierarchy lookup and US/EU bone-index lookup.
    GetBoneIndex getIndex = (GetBoneIndex)(g_gameVersion == 1 ? 0x7c51a0 : 0x7c51e0);
    BYTE* hierarchy = ((GetHierarchy)0x734a40)(clump);
    int index = getIndex(hierarchy, bone);
    const DWORD* source = (const DWORD*)(*(BYTE**)(hierarchy + 8) + (DWORD)index * 64);
    // Native REP MOVSD copies forward, including overlapping output.
    volatile DWORD* destination = (volatile DWORD*)out;
    for (int i = 0; i < 16; i++)
        destination[i] = source[i];
}

void CPed::GetBoneMatrix(CMatrix* out, int bone)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    BYTE* entry = *(BYTE**)(ped + 0x488 + (DWORD)bone * 4);
    GetBoneMatrixById(out, *(int*)(entry + 0x14));
}

CameraAim* CPed::GetCameraAim()
{
    return AimStuff::GetInternalAim();
}

BOOL __stdcall GameIsBackendOnScreen(void* backend)
{
    // GTA SA virtual address: thiscall backend, raw EAX normalized to BOOL.
    int result = 0;
    typedef DWORD(__fastcall * BackendFn)(void*, int);
    result = (int)reinterpret_cast<BackendFn>(0x534540)((void*)backend, 0);
    return result != 0;
}

BOOL CPed::IsOnScreen()
{
    return m_pGamePed ? GameIsBackendOnScreen(m_pGamePed) : FALSE;
}

void* CPed::GetContactEntity()
{
    return *(void**)((BYTE*)m_pGamePed + 0x584);
}

void* CPed::GetContactVehicle()
{
    return *(void**)((BYTE*)m_pGamePed + 0x568);
}

void CPed::SetCameraMode(BYTE mode)
{
    AimStuff::SetPlayerCameraMode(mode, m_nPlayerNumber);
}

void CPed::SetCameraExtZoomAndAspectRatio(float zoom, float aspect)
{
    AimStuff::SetPlayerCamera(m_nPlayerNumber, zoom, aspect);
}

BYTE CPed::GetCameraMode()
{
    return m_nPlayerNumber ? AimStuff::GetPlayerCameraMode(m_nPlayerNumber) : AimStuff::GetCameraMode();
}

void __stdcall GameSetPlayerFlag(BYTE player, DWORD value)
{
    g_gamePlayerFlags[player] = value;
}

CPed::CPed(BYTE player, int model, float x, float y, float z, float rotation)
{
    DWORD handle = 0, scriptPlayer = (BYTE)player;
    m_pGamePed = 0;
    m_nHandle = 0;
    Scripting::ProcessCommand(&g_pedCreatePlayerCommand, &scriptPlayer, (double)x, (double)y, (double)z, &handle);
    Scripting::ProcessCommand(&g_pedGetPlayerHandleCommand, &scriptPlayer, &handle);
    m_nHandle = handle;
    m_pGamePed = GamePoolPedGetAt(handle);
    m_pGameEntity = m_pGamePed;
    m_nPlayerNumber = (BYTE)player;
    GameAssignPlayerPed((BYTE)player, m_pGamePed);
    Scripting::ProcessCommand(&g_pedCommand087e, m_nHandle, 1);
    Scripting::ProcessCommand(&g_pedImmunitiesCommand, m_nHandle, 0, 0, 1, 0, 0);
    Scripting::ProcessCommand(&g_pedCommand0446, m_nHandle, 0);
    if (g_pNetGame)
    {
        DWORD setting = *(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame + 0x3d5) + 1);
        Scripting::ProcessCommand(&g_pedCommand03fe, m_nHandle, 0);
        Scripting::ProcessCommand(&g_pedCommand03fe, m_nHandle, setting);
    }
    unknown2a8 = 1;
    unknown2ac = 1;
    m_arrow = 0;
    m_parachuteObject = 0;
    m_bUsingCellphone = 0;
    m_danceStyle = 0;
    m_dancing = 0;
    m_bUrinating = 0;
    m_bDancing = 0;
    m_lastDanceMove = 0;
    SetModelIndex(model);
    ForceRotation(rotation);
    KeyStuff::ResetKeys(m_nPlayerNumber);
    m_stuffType = 0;
    m_stuffObject = 0;
    m_drunkLevel = 0;
    m_blockPedInputAt1C = 0;
    m_blockPedInputAt24 = 0;
    m_blockPedInputAt20 = 0;
    *(DWORD*)((BYTE*)this + 0x329) = 0;
    memset(m_bulletData, 0, 44);
    CMatrix matrix;
    GetMatrix(&matrix);
    memcpy(&matrix.position.x, &x, 4);
    memcpy(&matrix.position.y, &y, 4);
    matrix.position.z = z + g_pedSpawnLift;
    SetMatrix(matrix);
    for (int i = 0; i < 10; i++)
    {
        m_accessoryOccupied[i] = 0;
        m_accessoryObjects[i] = 0;
        memset(&m_accessoryInfo[i], 0, 52);
    }
    m_aimMarker = 0;
    *(DWORD*)((BYTE*)this + 0x329) = 0;
    memset(m_bulletData, 0, 44);
}

void CPed::AddAccessory(int index, const PedAccessoryState* info)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded backend vtable0x00863c40.
    if (!ped || *(DWORD*)ped == 0x863c40 || !*(void**)(ped + 0x18))
        return;
    if (index < 0 || index >= 10 || info->bone <= 0 || info->bone > 18)
        return;
    if (GetAccessoryState(index))
        DeleteAccessory(index);
    CMatrix matrix;
    GetMatrix(&matrix);
    PedAccessoryState* stored = &m_accessoryInfo[index];
    // Native REP MOVSD runs after deletion and forwards overlapping input/output.
    for (int i = 0; i < 13; i++)
        ((volatile DWORD*)stored)[i] = ((const DWORD*)info)[i];
    if (stored->model < 0 && g_customModels && g_customModels->IsObjectEntry(stored->model) && !g_customModels->GetLoadedFlag(stored->model) &&
        !g_customModels->LoadObjectEntry(stored->model))
        stored->model = 18631;
    CObject* object = new CObject(stored->model, matrix.position.x, matrix.position.y, matrix.position.z, stored->rotation, 200.0f, 1, 0, 0);
    m_accessoryObjects[index] = object;
    if (info->materialColor1)
        object->SetMaterial(-1, 0, 0, 0, info->materialColor1);
    if (info->materialColor2)
        ((CObject*)m_accessoryObjects[index])->SetMaterial(-1, 1, 0, 0, info->materialColor2);
    ((CObject*)m_accessoryObjects[index])->SetCollisionFlag(FALSE);
    m_accessoryOccupied[index] = 1;
}

BYTE CPed::GetDeathInfo(WORD* killer)
{
    if (g_pNetGame)
    {
        BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
        CVehiclePool* vehicles = *(CVehiclePool**)(pools + 0xc);
        CPlayerPool* players = *(CPlayerPool**)(pools + 8);
        BYTE* ped = (BYTE*)m_pGamePed;
        if (ped)
        {
            BYTE reason = ped[0x760];
            if (reason == 53)
            {
                *killer = 0xffff;
                return reason;
            }
            bool direct = reason < 43 || reason == 50 || reason == 51;
            if (direct || reason == 49 || reason == 54)
            {
                void* damager = *(void**)(ped + 0x764);
                if (damager)
                {
                    if (direct)
                    {
                        WORD id = players->Find(damager);
                        if (id != 0xffff)
                        {
                            *killer = id;
                            return reason;
                        }
                        damager = *(void**)((BYTE*)m_pGamePed + 0x764);
                    }
                    if (vehicles->Find(damager) != 0xffff)
                    {
                        BYTE* vehicle = *(BYTE**)((BYTE*)m_pGamePed + 0x764);
                        WORD id = players->Find(*(void**)(vehicle + 0x460));
                        if (id != 0xffff)
                        {
                            *killer = id;
                            return reason;
                        }
                    }
                    else if (reason == 54)
                    {
                        *killer = 0xffff;
                        return reason;
                    }
                }
            }
        }
    }
    *killer = 0xffff;
    return 0xff;
}

BOOL CPed::EnablePassengerDrivebyMode()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || !(*(DWORD*)(ped + 0x46c) & 0x100) || !*(void**)(ped + 0x58c))
        return FALSE;
    DWORD weapon = GetCurrentWeapon() & 0xff;
    if (weapon == 46)
    {
        SetArmedWeapon(0, 0);
        return FALSE;
    }
    SetArmedWeapon(weapon, 0);
    Scripting::ProcessCommand(&g_pedBehaviorDrivebyCommand, m_nHandle, -1, -1, g_pedParachuteZero, g_pedParachuteZero, g_pedParachuteZero, g_pedDrivebyDistance, 8, 1, 20);
    return TRUE;
}

void CPed::SetWalkStyle(const char* name)
{
    if (m_pGamePed)
        Scripting::ProcessCommand(&g_pedBehaviorWalkStyleCommand, m_nHandle, name);
}

BOOL CPed::OpenParachute()
{
    if (m_parachuteObject)
        return TRUE;
    CMatrix matrix;
    GetMatrix(&matrix);
    Scripting::ProcessCommand(&g_pedBehaviorCreateObjectCommand, 3131, (double)matrix.position.x, (double)matrix.position.y, (double)matrix.position.z, &m_parachuteObject);
    if (!GamePoolObjectGetAt(m_parachuteObject))
    {
        m_parachuteObject = 0;
        return FALSE;
    }
    Scripting::ProcessCommand(&g_pedBehaviorAttachCommand, m_parachuteObject, m_nHandle, g_pedParachuteZero, g_pedParachuteZero, g_pedParachuteZero, g_pedParachuteZero,
                              g_pedParachuteZero, g_pedParachuteZero);
    Scripting::ProcessCommand(&g_pedBehaviorObjectFlagCommand, m_parachuteObject, 0);
    Scripting::ProcessCommand(&g_pedBehaviorObjectScalarCommand, m_parachuteObject, g_pedParachuteZero);
    Scripting::ProcessCommand(&g_pedBehaviorObjectAnimationCommand, m_parachuteObject, g_pedParachuteOpenObject, g_gameParachuteLibrary, g_pedParachuteOpeningSpeed, 0, 1);
    Scripting::ProcessCommand(&g_pedBehaviorObjectFlagCommand, m_parachuteObject, 1);
    Scripting::ProcessCommand(&g_pedBehaviorObjectScalarCommand, m_parachuteObject, g_pedParachuteOne);
    return TRUE;
}

void CPed::ProcessParachuteEvent(const char* name)
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    DWORD weapon;
    Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand, m_nHandle, &weapon);
    if ((BYTE)weapon != 46)
        return;
    if (!strcmp(name, g_pedParachuteOpen))
    {
        if (!OpenParachute())
            return;
        CMatrix matrix;
        GetMatrix(&matrix);
        if (!GameFindLocalPed())
            return;
        const float* local = (const float*)(*(BYTE**)((BYTE*)GameFindLocalPed() + 0x14) + 0x30);
        const float* position = (const float*)&matrix.position;
        const double effectX = ((double)position[0] - local[0]) * g_objectMutationStep + local[0];
        const double effectY = ((double)position[1] - local[1]) * g_objectMutationStep + local[1];
        const double effectZ = ((double)position[2] - local[2]) * g_objectMutationStep + local[2];
        Scripting::ProcessCommand(&g_pedBehaviorEffectCommand, effectX, effectY, effectZ, 1039);
        return;
    }
    const char* animation = 0;
    int loop = 1;
    if (!strcmp(name, g_pedParachuteDecel))
        animation = g_pedParachuteDecelObject;
    else if (!strcmp(name, g_pedParachuteFloat))
        animation = g_pedParachuteFloatObject;
    else if (!strcmp(name, g_pedParachuteSteerRight))
        animation = g_pedParachuteSteerRightObject;
    else if (!strcmp(name, g_pedParachuteSteerLeft))
        animation = g_pedParachuteSteerLeftObject;
    else if (!strcmp(name, g_pedParachuteLandWater))
    {
        animation = g_pedParachuteLandWaterObject;
        loop = 0;
    }
    if (animation && OpenParachute())
        Scripting::ProcessCommand(&g_pedBehaviorObjectAnimationCommand, m_parachuteObject, animation, g_gameParachuteLibrary, g_pedParachuteOne, loop, 1);
}

void CPed::ProcessDancing()
{
    if (!m_dancing || !m_pGamePed || !CEntity::DoesExist() || m_nPlayerNumber)
        return;
    KeyPad* keys = KeyStuff::GetInternalKeys();
    short vertical = (short)keys->keys2[1], horizontal = (short)keys->keys2[0];
    if (!PerformingCustomAnimation())
    {
        DWORD style = *(DWORD*)m_danceMoveState;
        ApplyAnimation(g_pedDanceNames[style], g_pedDanceLibraries[style], 4.0f, 1, 0, 0, 0, 0);
    }
    bool alternate = keys->keys1[16] != 0;
    int move;
    if (vertical > 0)
        move = horizontal < 0 ? 4 : horizontal > 0 ? 5 : 0;
    else if (vertical < 0)
        move = horizontal < 0 ? 6 : horizontal > 0 ? 7 : 1;
    else if (horizontal < 0)
        move = 2;
    else if (horizontal > 0)
        move = 3;
    else
        return;
    if (alternate)
        move += 8;
    DWORD* last = (DWORD*)(m_danceMoveState + 4);
    if (*last == (DWORD)move)
        return;
    *last = move;
    const char* animation = GetAnimationForDance(move);
    if (animation)
    {
        DWORD style = *(DWORD*)m_danceMoveState;
        ApplyAnimation(animation, g_pedDanceLibraries[style], 4.0f, 0, 0, 0, 0, 0);
    }
}

void CPed::GiveStuff(int type)
{
    if (m_stuffObject)
        DropStuff();
    CMatrix matrix;
    GetMatrix(&matrix);
    int model;
    double x, y, z;
    if (type == 4)
    {
        model = 1485;
        x = g_pedParachuteZero;
        y = g_pedParachuteZero;
        z = g_pedParachuteZero;
    }
    else if (type == 1)
    {
        model = 1543;
        x = g_pedStuffOffset1X;
        y = g_pedStuffOffset1Y;
        z = g_pedStuffOffset1Z;
    }
    else if (type == 2)
    {
        model = 1486;
        x = g_pedStuffOffset1X;
        y = g_pedStuffOffset1Y;
        z = g_pedStuffOffset2Z;
    }
    else if (type == 3)
    {
        model = 1546;
        x = g_pedStuffOffset3X;
        y = g_pedStuffOffset3Y;
        z = g_pedStuffOffset3Z;
    }
    else
        return;
    Scripting::ProcessCommand(&g_pedBehaviorCreateObjectCommand, model, (double)matrix.position.x, (double)matrix.position.y, (double)matrix.position.z, &m_stuffObject);
    if (!GamePoolObjectGetAt(m_stuffObject))
    {
        m_stuffObject = 0;
        return;
    }
    Scripting::ProcessCommand(&g_pedDropStuff, m_nHandle, m_stuffObject, x, y, z, 6, 16, g_pedNullAnimation, g_pedNullAnimation, -1);
    m_stuffType = type;
}

// Raw quaternion DWORDs from the native animation-frame tables.

void* CPed::GetBoneQuaternion(int bone)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded vtable 0x00863c40.
    if (!ped || *(DWORD*)ped == 0x863c40)
        return 0;
    void* clump = *(void**)(ped + 0x18);
    if (!clump)
        return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE* hierarchy = ((BYTE * (__cdecl*)(void*))0x734a40)(clump);
    BYTE* animation = *(BYTE**)(hierarchy + 0x20);
    return (void*)((DWORD)animation + 0x4c + *(DWORD*)(animation + 0x24) * (DWORD)bone);
}

void CPed::ApplySecondaryPose()
{
    for (int i = 0; i < 12; i++)
    {
        volatile DWORD* destination = (volatile DWORD*)GetBoneQuaternion(g_pedSecondaryPose[i][0]);
        if (destination)
            for (int j = 0; j < 4; j++)
                destination[j] = g_pedSecondaryPose[i][j + 1];
    }
    void* backend = m_pGamePed;
    if (backend)
    {
        // GTA SA virtual address: update ped bone matrices, thiscall.
        typedef void(__fastcall * BackendFn)(void*, int);
        reinterpret_cast<BackendFn>(0x532b20)((void*)backend, 0);
    }
}

void CPed::ApplyDancePose()
{
    for (int i = 0; i < 9; i++)
    {
        volatile DWORD* destination = (volatile DWORD*)GetBoneQuaternion(g_pedDancePose[i][0]);
        if (destination)
            for (int j = 0; j < 4; j++)
                destination[j] = g_pedDancePose[i][j + 1];
    }
    float factor = 0.0f;
    void* root = GetBoneQuaternion(0);
    if (root)
    {
        CVector axis = {0, 0, 0};
        float angle = 0.0f;
        GameQueryRwQuaternionAngle(root, &axis, &angle);
        factor = 1.0f - axis.z;
    }
    // Keep repeated backend/model lookups: each native helper may change state.
    if ((m_pGamePed && GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed + 0x22)) == 5) || (m_pGamePed && GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed + 0x22)) == 22))
    {
        void* quaternion = GetBoneQuaternion(9);
        if (quaternion)
        {
            GameRotateRwQuaternion(quaternion, 1, -6.25f, 1);
            GameRotateRwQuaternion(quaternion, 0, 12.5f, 1);
        }
        quaternion = GetBoneQuaternion(15);
        if (quaternion)
        {
            GameRotateRwQuaternion(quaternion, 1, 6.25f, 1);
            GameRotateRwQuaternion(quaternion, 0, -12.5f, 1);
        }
    }
    else if (m_pGamePed && GameReadPedModelTable24(*(short*)((BYTE*)m_pGamePed + 0x22)) == 124)
    {
        void* quaternion = GetBoneQuaternion(9);
        if (quaternion)
        {
            GameRotateRwQuaternion(quaternion, 1, 10.0f, 1);
            GameRotateRwQuaternion(quaternion, 0, 25.0f, 1);
        }
        quaternion = GetBoneQuaternion(15);
        if (quaternion)
        {
            GameRotateRwQuaternion(quaternion, 1, -10.0f, 1);
            GameRotateRwQuaternion(quaternion, 0, -25.0f, 1);
        }
    }
    else
    {
        // Native float constants: RVAs 0011e33c/0011f3c8/0011f3c4/
        // 0011c194/0011f3c0. Every product is spilled to a float argument.
        factor = factor + 0.25f;
        void* quaternion = GetBoneQuaternion(9);
        if (quaternion)
        {
            GameRotateRwQuaternion(quaternion, 1, factor * -5.0f, 1);
            GameRotateRwQuaternion(quaternion, 0, factor * 25.0f, 1);
        }
        quaternion = GetBoneQuaternion(15);
        if (quaternion)
        {
            GameRotateRwQuaternion(quaternion, 1, factor * 5.0f, 1);
            GameRotateRwQuaternion(quaternion, 0, factor * -25.0f, 1);
        }
    }
    void* backend = m_pGamePed;
    if (backend)
    {
        // GTA SA virtual address: update ped bone matrices, thiscall.
        typedef void(__fastcall * BackendFn)(void*, int);
        reinterpret_cast<BackendFn>(0x532b20)((void*)backend, 0);
    }
}

void CPed::ProcessAnimationPose()
{
    if (m_bDancing)
        ApplyDancePose();
    if (m_danceStyle)
        ApplySecondaryPose();
}

void CPed::ProcessAccessories()
{
    bool bonesUpdated = false;
    for (int i = 0; i < 10; i++)
    {
        if (m_accessoryOccupied[i] != 1)
            continue;
        CEntity* object = (CEntity*)m_accessoryObjects[i];
        if (!object || !object->m_pGameEntity)
            continue;
        // The native ECX is this ped, not the accessory wrapper.
        if (!DoesExist())
        {
            CVector zero = {0, 0, 0};
            ((CEntity*)m_accessoryObjects[i])->Teleport(zero);
            continue;
        }
        if (!bonesUpdated)
        {
            void* ped = m_pGamePed;
            if (ped)
            {
                // GTA SA virtual address: update ped bone matrices, thiscall.
                typedef void(__fastcall * BackendFn)(void*, int);
                reinterpret_cast<BackendFn>(0x532b20)((void*)ped, 0);
            }
            bonesUpdated = true;
        }
        void* backend = ((CEntity*)m_accessoryObjects[i])->m_pGameEntity;
        DWORD* vtable = *(DWORD**)backend;
        // clang-format off
        // Cache the original backend and vtable across all following calls.
        __asm {
            mov ecx,backend
            mov eax,vtable
            call dword ptr [eax+0ch]
        }
        // clang-format on
        CMatrix matrix;
        GetBoneMatrix(&matrix, m_accessoryInfo[i].bone);
        CVector position;
        GameTransformPoint(&position, &matrix, &m_accessoryInfo[i].offset);
        memcpy(&matrix.position, &position, sizeof(position));
        for (int axis = 0; axis < 3; axis++)
        {
            float* rotation = (float*)&m_accessoryInfo[i].rotation + axis;
            if (*rotation != g_objectScaleLowerBound)
                GameRotateRwMatrixOnAxis(&matrix, axis, *rotation);
        }
        GameScaleRwMatrix(&matrix, &m_accessoryInfo[i].scale);
        ((CEntity*)m_accessoryObjects[i])->SetMatrix(matrix);
        ((CEntity*)m_accessoryObjects[i])->UpdateRwFrame();
        // clang-format off
        __asm {
            mov ecx,backend
            mov eax,vtable
            call dword ptr [eax+8]
        }
        // clang-format on
    }
}

BOOL CPed::HasInWaterTask()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (ped && !(*(DWORD*)(ped + 0x46c) & 0x100))
    {
        void* task = *(void**)(*(BYTE**)(ped + 0x47c) + 0xc);
        if (task)
            return GameGetTaskType(task) == 0x10c;
    }
    return FALSE;
}

int CPed::GetVehicleTransitionTaskState()
{
    if (!GamePoolPedGetAt(m_nHandle) || !m_pGamePed)
        return 0;
    BYTE* tasks = *(BYTE**)((BYTE*)m_pGamePed + 0x47c);
    if (!tasks)
        return 0;
    void* task = *(void**)(tasks + 0x10);
    if (!task)
        return 0;
    DWORD type = GameGetTaskType(task);
    if (type == 700 || type == 712)
        return 2;
    if (type == 701 || type == 713)
        return 1;
    return 0;
}

BOOL CPed::HasLeaveCarTask()
{
    if (!GamePoolPedGetAt(m_nHandle) || !m_pGamePed)
        return FALSE;
    BYTE* tasks = *(BYTE**)((BYTE*)m_pGamePed + 0x47c);
    if (!tasks)
        return FALSE;
    void* task = *(void**)(tasks + 0x10);
    return task ? GameGetTaskType(task) == 704 : FALSE;
}

BOOL CPed::IsDucking()
{
    if (!GamePoolPedGetAt(m_nHandle) || !m_pGamePed)
        return FALSE;
    return (*(DWORD*)((BYTE*)m_pGamePed + 0x46c) >> 26) & 1;
}

void CPed::ClearTasksIfOnFoot()
{
    if (!GamePoolPedGetAt(m_nHandle) || !m_pGamePed)
        return;
    if (*(DWORD*)((BYTE*)m_pGamePed + 0x46c) & 0x100)
        return;
    if (IsDucking())
        return;
    void* tasks = *(void**)((BYTE*)m_pGamePed + 0x47c);
    if (tasks)
    {
        // GTA SA virtual address: task-manager thiscall, argument 0.
        typedef void(__fastcall * BackendFn)(void*, int, DWORD);
        reinterpret_cast<BackendFn>(0x601230)((void*)tasks, 0, 0);
    }
}

void CPed::ClearDuckingFlag()
{
    if (!GamePoolPedGetAt(m_nHandle) || !m_pGamePed)
        return;
    *(DWORD*)((BYTE*)m_pGamePed + 0x46c) &= 0xfbffffff;
}

BOOL CPed::HasJumpTask()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || (*(DWORD*)(ped + 0x46c) & 0x100))
        return FALSE;
    // Native task-manager pointer is unchecked here.
    void* task = *(void**)(*(BYTE**)(ped + 0x47c) + 0x10);
    return task ? GameGetTaskType(task) == 211 : FALSE;
}

BOOL CPed::HasFightTask()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || (*(DWORD*)(ped + 0x46c) & 0x100))
        return FALSE;
    // Native task-manager pointer is unchecked here.
    void* task = *(void**)(*(BYTE**)(ped + 0x47c) + 0x18);
    return task ? GameGetTaskType(task) == 1016 : FALSE;
}

BOOL CPed::HasFallAndGetUpTask()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || (*(DWORD*)(ped + 0x46c) & 0x100))
        return FALSE;
    // Native task-manager pointer is unchecked here.
    void* task = *(void**)(*(BYTE**)(ped + 0x47c) + 0x4);
    return task ? GameGetTaskType(task) == 208 : FALSE;
}

BOOL CPed::HasAimTarget()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return FALSE;
    if (*(DWORD*)(ped + 0x500) || m_lastDanceMove)
        return TRUE;
    return FALSE;
}

void CPed::SetSecondaryPoseState(DWORD state)
{
    m_danceStyle = state;
    m_blockPedInputAt24 = m_blockPedInputAt1C = m_blockPedInputAt20 = state != 0;
}

void CPed::ProcessSecondaryPoseWeapon()
{
    if (!m_danceStyle || !m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    DWORD weapon;
    Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand, m_nHandle, &weapon);
    if ((BYTE)weapon)
        SetArmedWeapon(0, 0);
}

void CPed::SetDancePoseState(DWORD state)
{
    m_bDancing = state;
    m_blockPedInputAt20 = state != 0;
}

void CPed::ProcessDancePoseWeapon()
{
    if (!m_bDancing)
        return;
    if (g_deviceObjectEdit && *(DWORD*)(g_deviceObjectEdit + 0x80))
        m_blockPedInputAt20 = 0;
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    DWORD weapon;
    Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand, m_nHandle, &weapon);
    if ((BYTE)weapon)
        SetArmedWeapon(0, 0);
}

BOOL CPed::HasGangDrivebyTask()
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return FALSE;
    void* task = *(void**)(*(BYTE**)((BYTE*)m_pGamePed + 0x47c) + 0x10);
    return task ? GameGetTaskType(task) == 1022 : FALSE;
}

void CPed::StopGangDrivebyTask()
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    void* task = *(void**)(*(BYTE**)((BYTE*)m_pGamePed + 0x47c) + 0x10);
    if (!task || GameGetTaskType(task) != 1022)
        return;
    // GTA SA virtual address: original task teardown, thiscall/1.
    typedef void(__fastcall * BackendFn)(void*, int, DWORD);
    reinterpret_cast<BackendFn>(0x6275e0)((void*)task, 0, 1);
    // Native reloads the backend and task manager after teardown.
    *(DWORD*)(*(BYTE**)((BYTE*)m_pGamePed + 0x47c) + 0x10) = 0;
}

void CPed::SetMoney(int amount)
{
    Scripting::ProcessCommand(&g_pedCommand03fe, m_nHandle, 0);
    Scripting::ProcessCommand(&g_pedCommand03fe, m_nHandle, amount);
}

void CPed::CheckParachuteWeapon()
{
    if (!GamePoolPedGetAt(m_nHandle) || !DoesExist() || !m_parachuteObject)
        return;
    if ((BYTE)GetCurrentWeapon() == 46)
        return;
    Scripting::ProcessCommand(&g_pedDetachParachute, m_parachuteObject, g_pedParachuteZero, g_pedParachuteZero, g_pedParachuteZero, 0);
    Scripting::ProcessCommand(&g_pedDestroyParachute, m_parachuteObject);
    m_parachuteObject = 0;
}

BOOL CPed::HasNamedAnimationTask()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded backend vtable.
    if (!ped || *(DWORD*)ped == 0x863c40 || !*(void**)(ped + 0x18))
        return FALSE;
    void* task = *(void**)(*(BYTE**)(ped + 0x47c) + 0x10);
    if (!task)
        return FALSE;
    if (GameGetTaskType(task) == 401)
        return TRUE;
    task = GameGetTaskChild(task);
    while (task)
    {
        if (GameGetTaskType(task) == 401)
        {
            // Native checks the matching child again, retaining both calls.
            return task && GameGetTaskType(task) == 401;
        }
        task = GameGetTaskChild(task);
    }
    return FALSE;
}

DWORD CPed::GetFightingStyle()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return (DWORD)ped;
    return ped[0x72d];
}

DWORD CPed::GetTransformedBonePosition(DWORD first, DWORD second)
{
    void* ped = m_pGamePed;
    if (!ped || *(DWORD*)ped == 0x863c40)
        return 0;
    DWORD result;
    // GTA SA virtual address: thiscall backend(second,first,0).
    typedef DWORD(__fastcall * BackendFn)(void*, int, DWORD, DWORD, DWORD);
    result = (DWORD) reinterpret_cast<BackendFn>(0x5e01c0)((void*)ped, 0, second, first, 0);
    return result;
}

void CPed::ResetLocalControlWord()
{
    // GTA SA virtual address: local control word.
    *(WORD*)0xb7cedc = 0;
}

void CPed::ApplyClumpAnimation(int mode, DWORD first, DWORD second, DWORD third)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || *(DWORD*)ped == 0x863c40)
        return;
    void* clump = *(void**)(ped + 0x18);
    if (!clump)
        return;
    // GTA SA virtual addresses: native clump animation backends.
    if (mode == 1)
        ((void(__cdecl*)(void*, DWORD, DWORD))0x4d3aa0)(clump, first, second);
    else if (mode == 2)
        ((void(__cdecl*)(void*, DWORD, DWORD, DWORD))0x4d4610)(clump, first, second, third);
}

int CPed::GetBoneCount()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || *(DWORD*)ped == 0x863c40)
        return 0;
    void* clump = *(void**)(ped + 0x18);
    if (!clump)
        return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE* hierarchy = ((BYTE * (__cdecl*)(void*))0x734a40)(clump);
    return *(int*)(hierarchy + 4);
}

CMatrix* CPed::GetBoneMatrices()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || *(DWORD*)ped == 0x863c40)
        return 0;
    void* clump = *(void**)(ped + 0x18);
    if (!clump)
        return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE* hierarchy = ((BYTE * (__cdecl*)(void*))0x734a40)(clump);
    return *(CMatrix**)(hierarchy + 8);
}

void CPed::SetBoneMatrixById(const CMatrix* matrix, int bone)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || *(DWORD*)ped == 0x863c40)
        return;
    void* clump = *(void**)(ped + 0x18);
    if (!clump)
        return;
    // GTA SA virtual addresses: US/EU index helper selected before hierarchy call.
    typedef int(__cdecl * GetIndex)(void*, int);
    GetIndex getIndex = (GetIndex)(g_gameVersion == 1 ? 0x7c51a0 : 0x7c51e0);
    BYTE* hierarchy = ((BYTE * (__cdecl*)(void*))0x734a40)(clump);
    int index = getIndex(hierarchy, bone);
    volatile DWORD* destination = (volatile DWORD*)(*(BYTE**)(hierarchy + 8) + (DWORD)index * 64);
    const volatile DWORD* source = (const volatile DWORD*)matrix;
    // Forward DWORD copy, including overlap, then clear native RW matrix flags.
    for (int i = 0; i < 16; i++)
        destination[i] = source[i];
    destination[3] &= 0xfffdfffc;
}

void CPed::SetBoneMatrix(const CMatrix* matrix, int bone)
{
    BYTE* entry = *(BYTE**)((BYTE*)m_pGamePed + 0x488 + (DWORD)bone * 4);
    SetBoneMatrixById(matrix, *(int*)(entry + 0x14));
}

float CPed::GetRootPoseFactor()
{
    void* quaternion = GetBoneQuaternion(0);
    if (!quaternion)
        return 0.0f;
    CVector axis = {0, 0, 0};
    float angle = 0.0f;
    GameQueryRwQuaternionAngle(quaternion, &axis, &angle);
    // Native returns the subtraction in extended ST0, without a float spill.
    return 1.0f - axis.z;
}

DWORD CPed::PackAnimationTask(const void* task)
{
    const BYTE* data = (const BYTE*)task;
    WORD index = GameFindAnimation((const char*)data + 0x28, (const char*)data + 0x10);
    BYTE speed = (BYTE)(int)*(const float*)(data + 0x38);
    DWORD flags = *(const DWORD*)(data + 0x5c);
    int time = *(const int*)(data + 0x40);
    DWORD direction = time > 0 ? 2 : (time < 0 ? 1 : 0);
    DWORD packedFlags = (direction << 4) | ((flags & 8) ? 0 : 8) | ((flags & 0x80) ? 4 : 0) | ((flags & 0x40) ? 2 : 0) | ((flags & 2) ? 1 : 0);
    // Native sign-extends the index before OR, even for a failed lookup.
    return (DWORD)(int)(short)index | ((DWORD)speed << 16) | (packedFlags << 24);
}

DWORD CPed::PackQueuedAnimation()
{
    DWORD key = GameGetAnimationGroupKey(g_hookQueuedAnimationGroup, g_hookQueuedAnimationId);
    short index = (short)GameFindAnimationByKey(key);
    BYTE speed = (BYTE)(int)*(float*)&g_hookQueuedAnimationBlend;
    return (DWORD)(int)index | ((0xffff8000u | (DWORD)speed) << 16);
}

void* CPed::FindAnimationTask()
{
    void* task = *(void**)(*(BYTE**)((BYTE*)m_pGamePed + 0x47c) + 0x10);
    if (!task)
        return 0;
    if (GameGetTaskType(task) == 401 || GameGetTaskType(task) == 934)
        return task;
    task = GameGetTaskChild(task);
    while (task)
    {
        if (GameGetTaskType(task) == 401 || GameGetTaskType(task) == 934)
        {
            // Native repeats the matching-child type tests before returning it.
            if (task && (GameGetTaskType(task) == 401 || GameGetTaskType(task) == 934))
                return task;
            return 0;
        }
        task = GameGetTaskChild(task);
    }
    return 0;
}

DWORD CPed::GetPackedAnimation()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return 0;
    if (*(DWORD*)ped == 0x863c40)
        return 0;
    if (!*(void**)(ped + 0x18))
        return 0;
    void* task = FindAnimationTask();
    return task ? PackAnimationTask(task) : PackQueuedAnimation();
}

void CPed::ApplyPackedAnimation(DWORD packed)
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || *(DWORD*)ped == 0x863c40 || !*(void**)(ped + 0x18))
        return;
    float speed = (float)((packed >> 16) & 0xff);
    DWORD flags = packed >> 24;
    DWORD direction = (flags >> 4) & 3;
    int time = direction == 2 ? 1 : (direction == 1 ? -1 : 0);
    char* combined = GameGetAnimationName((short)packed);
    char library[32] = {0}, name[32] = {0};
    if (combined)
    {
        char* separator = strchr(combined, ':');
        if (separator)
        {
            const size_t libraryLength = separator - combined;
            if (libraryLength >= sizeof(library) || strlen(separator + 1) >= sizeof(name))
                return;
            memcpy(library, combined, libraryLength);
            strcpy_s(name, separator + 1);
            ApplyAnimation(name, library, speed, flags & 1, (flags >> 1) & 1, (flags >> 2) & 1, (flags >> 3) & 1, time);
        }
    }
    if (!memcmp(library, g_gameParachuteLibrary, 10))
        ProcessParachuteEvent(name);
}

int CPed::GetBoneId(int bone)
{
    BYTE* entry = *(BYTE**)((BYTE*)m_pGamePed + 0x488 + (DWORD)bone * 4);
    return *(int*)(entry + 0x14);
}

DWORD CPed::GetBoneHierarchyFlags()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped || *(DWORD*)ped == 0x863c40)
        return 0;
    void* clump = *(void**)(ped + 0x18);
    if (!clump)
        return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE* hierarchy = ((BYTE * (__cdecl*)(void*))0x734a40)(clump);
    return *(DWORD*)hierarchy;
}

__forceinline void CPed::UpdateBoneMatrices()
{
    void* ped = m_pGamePed;
    if (ped)
    {
        // GTA SA virtual address: native thiscall backend.
        typedef void(__fastcall * BackendFn)(void*, int);
        reinterpret_cast<BackendFn>(0x532b20)((void*)ped, 0);
    }
}

const char* CPed::GetBackendShoppingName()
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return 0;
    return *(const char**)((BYTE*)m_pGamePed + 0x78c);
}

void CPed::SetBackendShoppingName(const char* name)
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    BYTE* ped = (BYTE*)m_pGamePed;
    if (name && strlen(name))
    {
        if (strlen(name) > 31)
            return;
        memset(g_pedShoppingName, 0, 32);
        strcpy_s(g_pedShoppingName, name);
        *(char**)(ped + 0x78c) = g_pedShoppingName;
    }
    else
    {
        // Native clears only the low byte of the field, not the whole pointer.
        *(BYTE*)(ped + 0x78c) = 0;
    }
}

void CPed::GetAimOffsetMatrix(CMatrix* matrix)
{
    GetBoneMatrix(matrix, 2);
    CameraAim* aim = AimStuff::GetInternalAim();
    // Native float constants at RVAs 0011b31c and 0011f384.
    matrix->position.x = aim->front.x * 0.04f + matrix->position.x;
    matrix->position.y = aim->front.y * 0.04f + matrix->position.y;
    matrix->position.z = matrix->position.z + 0.08f;
}

BOOL CPed::GetWeaponAimOffsets(BOOL alternate, CVector* position, CVector* weaponOffsetOut)
{
    int bone = alternate ? 34 : 24;
    CVector* weaponOffset = (CVector*)GetCurrentWeaponInfo();
    // Preserve read-before-backend-validation and raw three-word copy.
    memcpy(weaponOffsetOut, weaponOffset, 12);
    void* ped = m_pGamePed;
    if (ped && *(DWORD*)ped != 0x863c40)
    {
        // GTA SA virtual address: backend(position,bone,0), thiscall.
        typedef void(__fastcall * BackendFn)(void*, int, void*, DWORD, DWORD);
        reinterpret_cast<BackendFn>(0x5e4280)((void*)ped, 0, (void*)position, (DWORD)bone, 0);
    }
    position->z = (weaponOffset->z + position->z) + g_pedSpawnLift;
    ped = m_pGamePed;
    if (ped && *(DWORD*)ped != 0x863c40)
    {
        // GTA SA virtual address: backend(weaponOffsetOut,bone,0), thiscall.
        typedef void(__fastcall * BackendFn2)(void*, int, void*, DWORD, DWORD);
        reinterpret_cast<BackendFn2>(0x5e01c0)((void*)ped, 0, (void*)weaponOffsetOut, (DWORD)bone, 0);
    }
    return TRUE;
}

void* CPed::GetAimHit(CVector* position)
{
    if (!m_pGamePed || GetCameraMode() != 4)
        return 0;
    CMatrix unused;
    GetMatrix(&unused);
    CameraAim* aim = AimStuff::GetInternalAim();
    CVector bone;
    GetBonePosition(8, &bone);
    CVector origin, target;
    // Native float constants at RVAs 0011bcc8 and 0011c664.
    origin.x = aim->front.x * 0.2f + bone.x;
    origin.y = aim->front.y * 0.2f + bone.y;
    origin.z = aim->front.z * 0.2f + bone.z;
    target.x = aim->front.x * 50.0f + bone.x;
    target.y = aim->front.y * 50.0f + bone.y;
    target.z = aim->front.z * 50.0f + bone.z;
    BYTE collision[44];
    void* entity = 0;
    GameProcessLineOfSightRaw(&origin, &target, collision, &entity, 1, 1, 1, 1, 0, 1, 0, 1);
    // Native ignores AL and tests the returned entity pointer instead.
    if (entity)
    {
        float x;
        memcpy(&x, collision, sizeof(x));
        position->x = x;
        memcpy(&position->y, collision + 4, 8);
    }
    return entity;
}

void CPed::GetCameraAimRay(CVector* origin, CVector* target)
{
    CVector source = {0, 0, 0}, destination = {0, 0, 0};
    // Native saves these only on the nonlocal path. A later changed player byte
    // still controls restoration, so do not invent initialized fallback values.
    BYTE savedMode;
    WORD savedModeWord;
    if (m_nPlayerNumber)
    {
        savedMode = *g_aimInternalCameraMode;
        *g_aimInternalCameraMode = AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        savedModeWord = *g_hookCameraModeWord;
        *g_hookCameraModeWord = AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        if (*g_hookCameraModeWord == 4)
            *g_hookCameraModeWord = 0;
        AimStuff::UpdateCamera();
        AimStuff::ApplyPlayerCamera(m_nPlayerNumber);
        AimStuff::UpdateAim();
        AimStuff::ApplyPlayerAim(m_nPlayerNumber);
        AimStuff::UpdateLocalSkills();
        AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
    }
    // This camera-mode query is present even though its return is unused.
    GetCameraMode();
    CVector* weaponOffset = (CVector*)GetCurrentWeaponInfo();
    if (*g_aimInternalCameraMode == 55)
    {
        CMatrix matrix;
        GetBoneMatrix(&matrix, 6);
        GameTransformRwPoint(origin, weaponOffset, &matrix);
    }
    else
    {
        void* ped = m_pGamePed;
        if (ped && *(DWORD*)ped != 0x863c40)
        {
            // GTA SA virtual address: backend(origin,24,0), thiscall.
            typedef void(__fastcall * BackendFn)(void*, int, void*, DWORD, DWORD);
            reinterpret_cast<BackendFn>(0x5e4280)((void*)ped, 0, (void*)origin, 24, 0);
        }
        origin->z = (weaponOffset->z + origin->z) + g_pedSpawnLift;
    }
    CVector copied;
    memcpy(&copied, origin, 12);
    // clang-format off
    // GTA SA virtual addresses: camera 00b6f028, thiscall 00514970.
    __asm {
        lea eax,destination
        push eax
        lea eax,source
        push eax
        push copied.z
        push copied.y
        push copied.x
        push 43960000h
        mov ecx,0b6f028h
        mov eax,514970h
        call eax
    }
    // clang-format on
    target->x = destination.x;
    memcpy(&target->y, &destination.y, 8);
    if (m_nPlayerNumber)
    {
        *g_aimInternalCameraMode = savedMode;
        *g_hookCameraModeWord = savedModeWord;
        AimStuff::ApplyLocalSkills();
        AimStuff::ApplyLocalCamera();
        AimStuff::ApplyLocalAim();
    }
}

void* CPed::GetCameraAimHit(CVector* position)
{
    if (!m_pGamePed)
        return 0;
    BYTE mode = GetCameraMode();
    if (mode != 53 && mode != 55 && mode != 65)
        return 0;
    CVector origin, target;
    GetCameraAimRay(&origin, &target);
    BYTE collision[44];
    void* entity = 0;
    GameProcessLineOfSightRaw(&origin, &target, collision, &entity, 0, 1, 1, 1, 0, 0, 0, 0);
    if (entity)
    {
        float x;
        memcpy(&x, collision, sizeof(x));
        position->x = x;
        memcpy(&position->y, collision + 4, 8);
    }
    return entity;
}

BOOL CPed::FireWeapon()
{
    BYTE savedMode;
    WORD savedModeWord;
    if (m_nPlayerNumber)
    {
        savedMode = *g_aimInternalCameraMode;
        *g_aimInternalCameraMode = AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        savedModeWord = *g_hookCameraModeWord;
        *g_hookCameraModeWord = AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        if (*g_hookCameraModeWord == 4)
            *g_hookCameraModeWord = 0;
        AimStuff::UpdateCamera();
        AimStuff::ApplyPlayerCamera(m_nPlayerNumber);
        AimStuff::UpdateAim();
        AimStuff::ApplyPlayerAim(m_nPlayerNumber);
        AimStuff::UpdateLocalSkills();
        AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
    }
    g_pedFiringContext = this;
    g_pedFiringData = *(DWORD*)(m_bulletData + 44) ? m_bulletData : 0;
    CVector origin = {0, 0, 0}, muzzle = {0, 0, 0};
    DWORD weapon;
    bool sniper = false;
    if (m_pGamePed && GamePoolPedGetAt(m_nHandle))
    {
        Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand, m_nHandle, &weapon);
        sniper = (BYTE)weapon == 34;
    }
    if (sniper)
        GameWeaponFireSniper(GetCurrentWeaponSlot(), m_pGamePed);
    else
    {
        GetWeaponAimOffsets(0, &origin, &muzzle);
        GameWeaponFireInstantHit(GetCurrentWeaponSlot(), m_pGamePed, &origin, &muzzle, 0, 0, 0, 0, 1);
    }
    g_pedFiringContext = 0;
    g_pedFiringData = 0;
    if (m_nPlayerNumber)
    {
        *g_aimInternalCameraMode = savedMode;
        *g_hookCameraModeWord = savedModeWord;
        AimStuff::ApplyLocalCamera();
        AimStuff::ApplyLocalAim();
        AimStuff::ApplyLocalSkills();
    }
    return TRUE;
}

void CPed::DrawBoneIds()
{
    if (!g_deviceLabel)
        return;
    g_deviceLabel->Begin();
    CMatrix matrix;
    for (int bone = 0; bone < 64; bone++)
    {
        GetBoneMatrixById(&matrix, bone);
        CVector position;
        memcpy(&position, &matrix.position, 12);
        char text[256];
        snprintf(text, sizeof(text), g_pedBoneIdFormat, bone);
        g_deviceLabel->Draw(&position, text, 0xffffffff, 1, 0);
    }
    g_deviceLabel->End();
}

void CPed::DrawMappedBoneIds()
{
    if (!g_deviceLabel)
        return;
    g_deviceLabel->Begin();
    CMatrix matrix;
    for (int bone = 1; bone <= 18; bone++)
    {
        GetBoneMatrix(&matrix, bone);
        CVector position;
        memcpy(&position, &matrix.position, 12);
        char text[256];
        snprintf(text, sizeof(text), g_pedBoneIdFormat, bone);
        g_deviceLabel->Draw(&position, text, 0xffffffff, 1, 0);
    }
    g_deviceLabel->End();
}

void CPed::DrawAnimationFrameIds()
{
    if (!g_deviceLabel)
        return;
    CMatrix matrix;
    GetMatrix(&matrix);
    g_deviceLabel->Begin();
    for (int bone = 0; bone < 30; bone++)
    {
        BYTE* frame = (BYTE*)GetBoneQuaternion(bone);
        if (!frame)
            continue;
        CVector position;
        GameTransformPoint(&position, &matrix, (CVector*)(frame + 0x10));
        char text[256];
        snprintf(text, sizeof(text), g_pedBoneIdFormat, bone);
        g_deviceLabel->Draw(&position, text, 0xffffffff, 1, 0);
    }
    g_deviceLabel->End();
}

void CPed::UpdateAndDrawBoneIds()
{
    UpdateBoneMatrices();
    DrawBoneIds();
}

void CPed::ProcessAimMarker()
{
    CVector position;
    if (GetAimHit(&position))
    {
        if (!m_aimMarker)
        {
            CVector rotation = {0, 0, 0};
            srand((unsigned)time(0));
            CObject* object = g_pGame->NewObject(19282, 0, 0, 0, rotation, 200.0f, 0, 0);
            m_aimMarker = (DWORD)object;
            object->SetCollisionFlag(0);
            ((CEntity*)m_aimMarker)->Add();
        }
        ((CEntity*)m_aimMarker)->Teleport(position);
    }
    else if (m_aimMarker)
    {
        delete (CObject*)m_aimMarker;
        m_aimMarker = 0;
    }
}

void CPed::ClearAimTarget()
{
    void* ped = m_pGamePed;
    if (!ped)
        return;
    if (!*(DWORD*)((BYTE*)ped + 0x500) && !m_lastDanceMove)
        return;
    m_lastDanceMove = 0;
    // GTA SA virtual address: native thiscall backend, no arguments.
    typedef void(__fastcall * BackendFn)(void*, int);
    reinterpret_cast<BackendFn>(0x5e6010)((void*)ped, 0);
}

void CPed::ProcessVehicleHorn()
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    KeyPad* keys;
    BYTE player = m_nPlayerNumber;
    if (!player)
        keys = KeyStuff::GetInternalKeys();
    else
        keys = KeyStuff::GetPlayerKeys(player);
    BYTE* ped = (BYTE*)m_pGamePed;
    BYTE* vehicle = *(BYTE**)(ped + 0x58c);
    if (vehicle && (*(DWORD*)(ped + 0x46c) & 0x100) && *(void**)(vehicle + 0x460) == ped)
        *(BYTE*)(vehicle + 0x514) = *(WORD*)((BYTE*)keys + 0x24) != 0;
}

void CPed::SetWeaponSkill(int skill, WORD value)
{
    BYTE player = m_nPlayerNumber;
    if (!player)
        return AimStuff::SetLocalSkill(skill, value);
    AimStuff::SetPlayerSkill(player, skill, value);
}

void CPed::UpdateClumpAnimations(float timeStep, DWORD onScreen)
{
    if (!m_pGamePed)
        return;
    void* clump = GetRwObject();
    if (clump)
        GameRwUpdateClumpAnimations(clump, timeStep, onScreen);
}

BYTE* CPed::GetControlFlags()
{
    return m_nPlayerNumber ? GameGetPlayerControlFlags(m_nPlayerNumber) : 0;
}

float CPed::GetArmBoneDistance()
{
    CMatrix first, second;
    memset(&first, 0, 64);
    memset(&second, 0, 64);
    GetBoneMatrix(&first, 5);
    GetBoneMatrix(&second, 6);
    return GameDistanceBetweenPoints(first.position.x, first.position.y, first.position.z, second.position.x, second.position.y, second.position.z);
}

void CPed::SetBulletData(PedBulletData* data)
{
    volatile DWORD* destination = (volatile DWORD*)m_bulletData;
    if (!data)
    {
        destination[11] = 0;
        for (int i = 0; i < 11; i++)
            destination[i] = 0;
        return;
    }
    const volatile DWORD* source = (const volatile DWORD*)data;
    destination[11] = 1;
    destination[10] = source[10];
    // Preserve forward word reads/stores and the untouched leading DWORD.
    for (int i = 1; i < 10; i++)
        destination[i] = source[i];
    if (m_nPlayerNumber || !g_pNetGame)
        return;
    CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    if (players)
        (*(CLocalPlayer**)((BYTE*)players + 0x1e))->SendBullet(data);
}

void CPed::GiveWeapon(int weapon, int ammo)
{
    if (!m_pGamePed || !GamePoolPedGetAt(m_nHandle))
        return;
    int model = GameGetWeaponModel(weapon);
    if (model == -1)
        return;
    if (!g_pGame->IsModelAvailable(model))
    {
        g_pGame->RequestModel(model, 2);
        g_pGame->LoadRequestedModels();
        while (!g_pGame->IsModelAvailable(model))
            Sleep(1);
    }
    *g_clientCrashPedContext = m_nPlayerNumber;
    AimStuff::UpdateLocalSkills();
    AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
    void* ped = m_pGamePed;
    // GTA SA virtual address: backend GiveWeapon(weapon,ammo,1), thiscall.
    typedef void(__fastcall * BackendFn)(void*, int, DWORD, DWORD, DWORD);
    reinterpret_cast<BackendFn>(0x5e6080)((void*)ped, 0, (DWORD)weapon, (DWORD)ammo, 1);
    AimStuff::ApplyLocalSkills();
    SetArmedWeapon(weapon, 0);
    *g_clientCrashPedContext = 0;
}

void CPed::EnableGoggles()
{
    if (m_pGamePed && (*(DWORD*)((BYTE*)m_pGamePed + 0x500) || m_lastDanceMove))
        return;
    if (!GetWeaponSlot(44) && !GetWeaponSlot(45))
        GiveWeapon(44, 1);
    CTaskUseGoggles* task = new CTaskUseGoggles;
    // The native allocation/constructor path does not handle a null result here.
    task->ApplyToPed(this);
    m_lastDanceMove = 1;
}

BYTE* __stdcall GameGetPlayerControlFlags(BYTE player)
{
    return player < 210 ? g_gamePlayerControllerState[player] : 0;
}

int __stdcall GameGetWeaponModel(int weapon)
{
    switch (weapon)
    {
        case 1:
            return 331;
        case 2:
            return 333;
        case 3:
            return 334;
        case 4:
            return 335;
        case 5:
            return 336;
        case 6:
            return 337;
        case 7:
            return 338;
        case 8:
            return 339;
        case 9:
            return 341;
        case 10:
            return 321;
        case 11:
            return 322;
        case 12:
            return 323;
        case 13:
            return 324;
        case 14:
            return 325;
        case 15:
            return 326;
        case 16:
            return 342;
        case 17:
            return 343;
        case 18:
            return 344;
        case 22:
            return 346;
        case 23:
            return 347;
        case 24:
            return 348;
        case 25:
            return 349;
        case 26:
            return 350;
        case 27:
            return 351;
        case 28:
            return 352;
        case 29:
            return 353;
        case 30:
            return 355;
        case 31:
            return 356;
        case 32:
            return 372;
        case 33:
            return 357;
        case 34:
            return 358;
        case 35:
            return 359;
        case 36:
            return 360;
        case 37:
            return 361;
        case 38:
            return 362;
        case 39:
            return 363;
        case 40:
            return 364;
        case 41:
            return 365;
        case 42:
            return 366;
        case 43:
            return 367;
        case 44:
            return 368;
        case 45:
            return 369;
        case 46:
            return 371;
        default:
            return -1;
    }
}

WORD CPed::GetCurrentWeaponTotalAmmo()
{
    BYTE* ped = (BYTE*)m_pGamePed;
    if (!ped)
        return 0;
    DWORD slot = (DWORD)ped + 0x5a0 + (DWORD)ped[0x718] * 28;
    if (!slot)
        return 0xffff;
    DWORD weapon = *(DWORD*)slot;
    if (weapon <= 15 || weapon == 46)
        return 0xffff;
    return *(WORD*)(slot + 0xc);
}

void CPed::ProcessCameraAimMarker()
{
    CVector position;
    if (GetCameraAimHit(&position))
    {
        if (!m_aimMarker)
        {
            CVector rotation = {0, 0, 0};
            srand((unsigned)time(0));
            CObject* object = g_pGame->NewObject(3065, 0, 0, 0, rotation, 200.0f, 0, 0);
            m_aimMarker = (DWORD)object;
            object->SetCollisionFlag(0);
            ((CEntity*)m_aimMarker)->Add();
        }
        ((CEntity*)m_aimMarker)->Teleport(position);
    }
    else if (m_aimMarker)
    {
        delete (CObject*)m_aimMarker;
        m_aimMarker = 0;
    }
}

void GameInitializePlayerPedTables()
{
    memset(g_gamePlayerPeds, 0, sizeof(g_gamePlayerPeds));
    memset(g_gamePlayerControllerState, 0, sizeof(g_gamePlayerControllerState));
}

// Native byte index is unchecked;210..255 reaches the adjacent controller state.

void* __stdcall GameGetPlayerPedUnchecked(BYTE player)
{
    return g_gamePlayerPeds[player];
}

void __stdcall GameSetPlayerControllerWords(BYTE player, DWORD first, DWORD second, DWORD third, DWORD fourth)
{
    if (player < 210)
    {
        DWORD* words = (DWORD*)g_gamePlayerControllerState[player];
        words[0] = first;
        words[1] = second;
        words[2] = third;
        words[3] = fourth;
    }
}

BYTE __stdcall GameFindPlayerIndexByClump(void* clump)
{
    BYTE player = 0;
    do
    {
        void* ped = g_gamePlayerPeds[player];
        if (ped)
        {
            void* object = *(void**)((BYTE*)ped + 0x18);
            if (object && object == clump)
                return player;
        }
        ++player;
    }
    while (player != 210);
    return 0;
}

// Native byte index is unchecked, unlike the existing bounded flags getter.

DWORD __stdcall GameGetPlayerFlagsUnchecked(BYTE player)
{
    return g_gamePlayerFlags[player];
}

PedBulletData* CPed::GetBulletData()
{
    return *(DWORD*)(m_bulletData + 44) ? (PedBulletData*)m_bulletData : 0;
}
