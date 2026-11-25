#ifndef SCRIPTRPC_H
#define SCRIPTRPC_H

class RakClientInterface;
struct RPCParameters;
namespace Scripting
{
struct OpcodeInfo;
}
namespace RakNet
{
class BitStream;
}
class CObject;
// Reads count material records from an existing borrowed stream.

void ObjectReadMaterials(unsigned char count, CObject* object, RakNet::BitStream* stream);

void ScrCreateObject(RPCParameters* parameters);

void ScrSetObjectMaterial(RPCParameters* parameters);

void ScrAttachObjectToPlayer(RPCParameters* parameters);

void ScrSetPlayerAttachedObject(RPCParameters* parameters);
// Native updates custom-model state; higher-level name remains unestablished.

void ScrSetCustomModelVirtualWorld(RPCParameters* parameters);

extern const Scripting::OpcodeInfo g_rpcAttachObjectToPlayerCommand;

void ScrSetPlayerName(RPCParameters* parameters);

void ScrSetPlayerDrunkLevel(RPCParameters* parameters);

void ScrSetPlayerArmedWeapon(RPCParameters* parameters);

void ScrAttachCameraToObject(RPCParameters* parameters);

void ScrInterpolateCamera(RPCParameters* parameters);

void ClickTextDraw(RPCParameters* parameters);

void ScrStopObjectCameraCollision(RPCParameters* parameters);

void ScrRemoveBuildingForPlayer(RPCParameters* parameters);

void ScrPlayCrimeReport(RPCParameters* parameters);

void ScrPlayerOpcode052C(RPCParameters* parameters);

void ScrPlayerOpcode03FD(RPCParameters* parameters);
// The original returns immediately.

void ScrIgnoreRpc125(RPCParameters* parameters);
// Native consumes and discards a WORD, with stream construction/destruction.

void ScrReadAndDiscardRpc64(RPCParameters* parameters);

extern const Scripting::OpcodeInfo g_rpcPlayerOpcode052cCommand;

extern const Scripting::OpcodeInfo g_rpcPlayerOpcode03fdCommand;

void ScrSetSpawnInfo(RPCParameters* parameters);

void ScrSetPlayerFightingStyle(RPCParameters* parameters);

void ScrSetPlayerAmmo(RPCParameters* parameters);

void ScrSetPlayerTeam(RPCParameters* parameters);

void ScrSetPlayerSkin(RPCParameters* parameters);

void ScrApplyAnimation(RPCParameters* parameters);

void ScrClearAnimations(RPCParameters* parameters);

void ScrMoveVehicleComponent(RPCParameters* parameters);

void ScrSetPlayerWidescreen(RPCParameters* parameters);

void ScrSetVehicleTires(RPCParameters* parameters);

extern const Scripting::OpcodeInfo g_rpcWidescreenCommand;

extern const char g_rpcWidescreenMessage[];

extern const char g_rpcInvalidSkinWarning[];

extern const Scripting::OpcodeInfo g_rpcExplosionCommand;

void ScrSetObjectPosition(RPCParameters* parameters);

void ScrSetObjectRotation(RPCParameters* parameters);

void ScrDestroyObject(RPCParameters* parameters);

void ScrMoveObject(RPCParameters* parameters);

void ScrStopObject(RPCParameters* parameters);

void ScrSetActorAnimation(RPCParameters* parameters);

void ScrClearActorAnimations(RPCParameters* parameters);

void ScrSetActorRotation(RPCParameters* parameters);

void ScrSetActorPosition(RPCParameters* parameters);

void ScrSetActorHealth(RPCParameters* parameters);

void ScrSetPlayerMapIcon(RPCParameters* parameters);

void ScrRemovePlayerMapIcon(RPCParameters* parameters);

void ScrCreateExplosion(RPCParameters* parameters);

void ScrShowPlayerNametagForPlayer(RPCParameters* parameters);

void ScrSetVehiclePosition(RPCParameters* parameters);

void ScrSetVehicleZAngle(RPCParameters* parameters);

void ScrSetVehicleParamsForPlayer(RPCParameters* parameters);

void ScrLinkVehicleToInterior(RPCParameters* parameters);

void ScrSetVehicleHealth(RPCParameters* parameters);

void ScrAttachTrailerToVehicle(RPCParameters* parameters);

void ScrDetachTrailerFromVehicle(RPCParameters* parameters);

void ScrSetVehicleVelocity(RPCParameters* parameters);

void ScrSetNumberPlate(RPCParameters* parameters);

void ScrTogglePlayerSpectating(RPCParameters* parameters);

void ScrPlayerSpectatePlayer(RPCParameters* parameters);

void ScrPlayerSpectateVehicle(RPCParameters* parameters);

void ScrPutPlayerInVehicle(RPCParameters* parameters);

void ScrRemovePlayerFromVehicle(RPCParameters* parameters);

void ScrDisplayGametext(RPCParameters* parameters);

void ScrSetPlayerShopName(RPCParameters* parameters);

void ScrSetPlayerColor(RPCParameters* parameters);

void ScrDeathMessage(RPCParameters* parameters);

void ScrInitMenu(RPCParameters* parameters);

void ScrShowMenu(RPCParameters* parameters);

void ScrHideMenu(RPCParameters* parameters);

void ScrShowTextDraw(RPCParameters* parameters);

void ScrHideTextDrawForPlayer(RPCParameters* parameters);

void ScrTextDrawSetString(RPCParameters* parameters);

void ScrPlaySound(RPCParameters* parameters);

void ScrPlayAudioStream(RPCParameters* parameters);

void ScrStopAudioStream(RPCParameters* parameters);

extern const char g_rpcLargeTextDrawWarning[];

void ScrSetPlayerCameraPosition(RPCParameters* parameters);

void ScrSetPlayerCameraLookAt(RPCParameters* parameters);

void ScrSetCameraBehindPlayer(RPCParameters* parameters);

void ScrSetPlayerPosition(RPCParameters* parameters);

void ScrSetPlayerPositionFindZ(RPCParameters* parameters);

void ScrSetPlayerVelocity(RPCParameters* parameters);

void ScrSetPlayerWorldBounds(RPCParameters* parameters);

void ScrSetPlayerFacingAngle(RPCParameters* parameters);

void ScrEnableStuntBonusForPlayer(RPCParameters* parameters);

void ScrForceClassSelection(RPCParameters* parameters);

void ScrResetPlayerWeapons(RPCParameters* parameters);

void ScrGivePlayerWeapon(RPCParameters* parameters);

void ScrGangZoneCreate(RPCParameters* parameters);

void ScrGangZoneDestroy(RPCParameters* parameters);

void ScrGangZoneFlash(RPCParameters* parameters);

void ScrGangZoneStopFlash(RPCParameters* parameters);

void ScrDisableVehicleCollisions(RPCParameters* parameters);

void ScrSetGravity(RPCParameters* parameters);

void ScrSetPlayerWantedLevel(RPCParameters* parameters);

void ScrSetPlayerHealth(RPCParameters* parameters);

void ScrSetPlayerArmor(RPCParameters* parameters);

void ScrSetPlayerInterior(RPCParameters* parameters);

void ScrGivePlayerMoney(RPCParameters* parameters);

void ScrResetPlayerMoney(RPCParameters* parameters);

void ScrSetPlayerSpecialAction(RPCParameters* parameters);

void ScrTogglePlayerControllable(RPCParameters* parameters);

extern const int g_rpcScrSetPlayerArmor;

void RegisterScriptRPCs(RakClientInterface* client);

void UnRegisterScriptRPCs(RakClientInterface* client);

extern const int g_rpcScrDisableVehicleCollisions;

extern const int g_rpcScrForceClassSelection;

extern const int g_rpcScrAttachObjectToPlayer;

extern const int g_rpcScrInitMenu;

extern const int g_rpcScrShowMenu;

extern const int g_rpcScrHideMenu;

extern const int g_rpcScrSetPlayerWantedLevel;

extern const int g_rpcScrShowTextDraw;

extern const int g_rpcScrHideTextDrawForPlayer;

extern const int g_rpcScrTextDrawSetString;

extern const int g_rpcScrGangZoneCreate;

extern const int g_rpcScrGangZoneDestroy;

extern const int g_rpcScrGangZoneFlash;

extern const int g_rpcScrGangZoneStopFlash;

extern const int g_rpcScrApplyAnimation;

extern const int g_rpcScrClearAnimations;

extern const int g_rpcScrSetPlayerSpecialAction;

extern const int g_rpcScrEnableStuntBonusForPlayer;

extern const int g_rpcScrSetPlayerFightingStyle;

extern const int g_rpcScrSetPlayerVelocity;

extern const int g_rpcScrSetVehicleVelocity;

extern const int g_rpcScrSetPlayerWidescreen;

extern const int g_rpcScrSetVehicleTires;

extern const int g_rpcPlayerOpcode052C;

extern const int g_rpcPlayerOpcode03FD;

extern const int g_rpcScrPlayCrimeReport;

extern const int g_rpcScrSetPlayerMapIcon;

extern const int g_rpcScrRemovePlayerMapIcon;

extern const int g_rpcScrSetPlayerAmmo;

extern const int g_rpcScrSetGravity;

extern const int g_rpcScrSetVehicleHealth;

extern const int g_rpcScrAttachTrailerToVehicle;

extern const int g_rpcScrDetachTrailerFromVehicle;

extern const int g_rpcScrCreateObject;

extern const int g_rpcScrSetObjectPosition;

extern const int g_rpcScrSetObjectRotation;

extern const int g_rpcScrDestroyObject;

extern const int g_rpcScrCreateExplosion;

extern const int g_rpcScrShowPlayerNametagForPlayer;

extern const int g_rpcScrMoveObject;

extern const int g_rpcScrStopObject;

extern const int g_rpcScrSetNumberPlate;

extern const int g_rpcScrTogglePlayerSpectating;

extern const int g_rpcUnknown001183A0;

extern const int g_rpcScrPlayerSpectatePlayer;

extern const int g_rpcScrPlayerSpectateVehicle;

extern const int g_rpcScrMoveVehicleComponent;

extern const int g_rpcScrSetPlayerShopName;

extern const int g_rpcScrSetPlayerDrunkLevel;

extern const int g_rpcScrSetPlayerArmedWeapon;

extern const int g_rpcScrSetPlayerAttachedObject;

extern const int g_rpcScrPlayAudioStream;

extern const int g_rpcScrStopAudioStream;

extern const int g_rpcScrRemoveBuildingForPlayer;

extern const int g_rpcScrSetSpawnInfo;

extern const int g_rpcScrSetPlayerTeam;

extern const int g_rpcScrSetPlayerName;

extern const int g_rpcScrSetPlayerSkin;

extern const int g_rpcScrSetPlayerPosition;

extern const int g_rpcScrSetPlayerPositionFindZ;

extern const int g_rpcScrSetPlayerHealth;

extern const int g_rpcScrPutPlayerInVehicle;

extern const int g_rpcScrRemovePlayerFromVehicle;

extern const int g_rpcScrSetPlayerColor;

extern const int g_rpcScrDisplayGametext;

extern const int g_rpcScrSetPlayerInterior;

extern const int g_rpcScrSetPlayerCameraPosition;

extern const int g_rpcScrSetPlayerCameraLookAt;

extern const int g_rpcScrSetVehiclePosition;

extern const int g_rpcScrSetVehicleZAngle;

extern const int g_rpcScrSetVehicleParamsForPlayer;

extern const int g_rpcScrSetCameraBehindPlayer;

extern const int g_rpcScrTogglePlayerControllable;

extern const int g_rpcScrPlaySound;

extern const int g_rpcScrSetPlayerWorldBounds;

extern const int g_rpcScrGivePlayerMoney;

extern const int g_rpcScrSetPlayerFacingAngle;

extern const int g_rpcScrResetPlayerMoney;

extern const int g_rpcScrResetPlayerWeapons;

extern const int g_rpcScrGivePlayerWeapon;

extern const int g_rpcUnknown0011830C;

extern const int g_rpcScrLinkVehicleToInterior;

extern const int g_rpcScrDeathMessage;

extern const int g_rpcScrAttachCameraToObject;

extern const int g_rpcScrInterpolateCamera;

extern const int g_rpcScrSetObjectMaterial;

extern const int g_rpcScrStopObjectCameraCollision;

extern const int g_rpcScrSetActorAnimation;

extern const int g_rpcScrClearActorAnimations;

extern const int g_rpcScrSetActorRotation;

extern const int g_rpcScrSetActorPosition;

extern const int g_rpcScrSetActorHealth;

extern const int g_rpcSetCustomModelVirtualWorld;

#endif // SCRIPTRPC_H
