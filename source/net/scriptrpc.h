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
// 0x0001b070
void ObjectReadMaterials(unsigned char count, CObject* object, RakNet::BitStream* stream);
// 0x0001b340
void ScrCreateObject(RPCParameters* parameters);
// 0x0001b6a0
void ScrSetObjectMaterial(RPCParameters* parameters);
// 0x0001c6a0
void ScrAttachObjectToPlayer(RPCParameters* parameters);
// 0x00018f00
void ScrSetPlayerAttachedObject(RPCParameters* parameters);
// Native updates custom-model state; higher-level name remains unestablished.
// 0x0001dcc0
void ScrSetCustomModelVirtualWorld(RPCParameters* parameters);
// R5 address to verify
extern const Scripting::OpcodeInfo g_rpcAttachObjectToPlayerCommand;
// 0x0001dfd0
void ScrSetPlayerName(RPCParameters* parameters);
// 0x00018db0
void ScrSetPlayerDrunkLevel(RPCParameters* parameters);
// 0x00018e50
void ScrSetPlayerArmedWeapon(RPCParameters* parameters);
// 0x00019ff0
void ScrAttachCameraToObject(RPCParameters* parameters);
// 0x0001a0f0
void ScrInterpolateCamera(RPCParameters* parameters);
// 0x0001d650
void ClickTextDraw(RPCParameters* parameters);
// 0x0001c170
void ScrStopObjectCameraCollision(RPCParameters* parameters);
// 0x0001d530
void ScrRemoveBuildingForPlayer(RPCParameters* parameters);
// 0x00019050
void ScrPlayCrimeReport(RPCParameters* parameters);
// R5 address to verify
void ScrPlayerOpcode052C(RPCParameters* parameters);
// R5 address to verify
void ScrPlayerOpcode03FD(RPCParameters* parameters);
// The original returns immediately.
// R5 address to verify
void ScrIgnoreRpc125(RPCParameters* parameters);
// Native consumes and discards a WORD, with stream construction/destruction.
// 0x0001a700
void ScrReadAndDiscardRpc64(RPCParameters* parameters);
// R5 address to verify
extern const Scripting::OpcodeInfo g_rpcPlayerOpcode052cCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_rpcPlayerOpcode03fdCommand;
// 0x00017f50
void ScrSetSpawnInfo(RPCParameters* parameters);
// 0x00018740
void ScrSetPlayerFightingStyle(RPCParameters* parameters);
// 0x0001ac10
void ScrSetPlayerAmmo(RPCParameters* parameters);
// 0x00019710
void ScrSetPlayerTeam(RPCParameters* parameters);
// 0x00019190
void ScrSetPlayerSkin(RPCParameters* parameters);
// 0x0001a950
void ScrApplyAnimation(RPCParameters* parameters);
// 0x00018580
void ScrClearAnimations(RPCParameters* parameters);
// 0x0001c5c0
void ScrMoveVehicleComponent(RPCParameters* parameters);
// 0x00018ac0
void ScrSetPlayerWidescreen(RPCParameters* parameters);
// R5 address to verify
void ScrSetVehicleTires(RPCParameters* parameters);
// R5 address to verify
extern const Scripting::OpcodeInfo g_rpcWidescreenCommand;
// 0x000e63bc
extern const char g_rpcWidescreenMessage[];
// 0x000e63cc
extern const char g_rpcInvalidSkinWarning[];
// 0x000e620c
extern const Scripting::OpcodeInfo g_rpcExplosionCommand;
// 0x0001ba10
void ScrSetObjectPosition(RPCParameters* parameters);
// 0x0001bb20
void ScrSetObjectRotation(RPCParameters* parameters);
// 0x0001bc20
void ScrDestroyObject(RPCParameters* parameters);
// 0x0001bf00
void ScrMoveObject(RPCParameters* parameters);
// 0x0001c0b0
void ScrStopObject(RPCParameters* parameters);
// 0x0001d750
void ScrSetActorAnimation(RPCParameters* parameters);
// R5 address to verify
void ScrClearActorAnimations(RPCParameters* parameters);
// 0x0001d9f0
void ScrSetActorRotation(RPCParameters* parameters);
// 0x0001dad0
void ScrSetActorPosition(RPCParameters* parameters);
// 0x0001dbe0
void ScrSetActorHealth(RPCParameters* parameters);
// 0x0001a790
void ScrSetPlayerMapIcon(RPCParameters* parameters);
// 0x0001a8b0
void ScrRemovePlayerMapIcon(RPCParameters* parameters);
// 0x0001bd10
void ScrCreateExplosion(RPCParameters* parameters);
// 0x0001be20
void ScrShowPlayerNametagForPlayer(RPCParameters* parameters);
// 0x00019c70
void ScrSetVehiclePosition(RPCParameters* parameters);
// 0x00019d80
void ScrSetVehicleZAngle(RPCParameters* parameters);
// 0x00019e60
void ScrSetVehicleParamsForPlayer(RPCParameters* parameters);
// 0x00019f30
void ScrLinkVehicleToInterior(RPCParameters* parameters);
// 0x0001ad70
void ScrSetVehicleHealth(RPCParameters* parameters);
// 0x0001ae50
void ScrAttachTrailerToVehicle(RPCParameters* parameters);
// 0x0001af90
void ScrDetachTrailerFromVehicle(RPCParameters* parameters);
// 0x00018950
void ScrSetVehicleVelocity(RPCParameters* parameters);
// 0x0001c230
void ScrSetNumberPlate(RPCParameters* parameters);
// 0x0001c350
void ScrTogglePlayerSpectating(RPCParameters* parameters);
// 0x0001c400
void ScrPlayerSpectatePlayer(RPCParameters* parameters);
// 0x0001c4e0
void ScrPlayerSpectateVehicle(RPCParameters* parameters);
// 0x00019600
void ScrPutPlayerInVehicle(RPCParameters* parameters);
// 0x00017ff0
void ScrRemovePlayerFromVehicle(RPCParameters* parameters);
// 0x000198f0
void ScrDisplayGametext(RPCParameters* parameters);
// 0x00017e50
void ScrSetPlayerShopName(RPCParameters* parameters);
// 0x00019800
void ScrSetPlayerColor(RPCParameters* parameters);
// 0x0001dd70
void ScrDeathMessage(RPCParameters* parameters);
// 0x0001c870
void ScrInitMenu(RPCParameters* parameters);
// 0x0001cb90
void ScrShowMenu(RPCParameters* parameters);
// 0x0001cc40
void ScrHideMenu(RPCParameters* parameters);
// 0x0001cd90
void ScrShowTextDraw(RPCParameters* parameters);
// 0x0001cec0
void ScrHideTextDrawForPlayer(RPCParameters* parameters);
// 0x0001cf70
void ScrTextDrawSetString(RPCParameters* parameters);
// 0x0001a330
void ScrPlaySound(RPCParameters* parameters);
// 0x0001d3c0
void ScrPlayAudioStream(RPCParameters* parameters);
// 0x000180f0
void ScrStopAudioStream(RPCParameters* parameters);
// 0x000e6404
extern const char g_rpcLargeTextDrawWarning[];
// 0x00019aa0
void ScrSetPlayerCameraPosition(RPCParameters* parameters);
// 0x00019b70
void ScrSetPlayerCameraLookAt(RPCParameters* parameters);
// 0x00018080
void ScrSetCameraBehindPlayer(RPCParameters* parameters);
// 0x00019320
void ScrSetPlayerPosition(RPCParameters* parameters);
// 0x00019440
void ScrSetPlayerPositionFindZ(RPCParameters* parameters);
// 0x00018850
void ScrSetPlayerVelocity(RPCParameters* parameters);
// 0x0001a410
void ScrSetPlayerWorldBounds(RPCParameters* parameters);
// 0x0001a5a0
void ScrSetPlayerFacingAngle(RPCParameters* parameters);
// 0x00017d50
void ScrEnableStuntBonusForPlayer(RPCParameters* parameters);
// 0x000180d0
void ScrForceClassSelection(RPCParameters* parameters);
// 0x000180a0
void ScrResetPlayerWeapons(RPCParameters* parameters);
// 0x0001a640
void ScrGivePlayerWeapon(RPCParameters* parameters);
// 0x0001d080
void ScrGangZoneCreate(RPCParameters* parameters);
// 0x0001d1a0
void ScrGangZoneDestroy(RPCParameters* parameters);
// 0x0001d250
void ScrGangZoneFlash(RPCParameters* parameters);
// 0x0001d310
void ScrGangZoneStopFlash(RPCParameters* parameters);
// 0x00017de0
void ScrDisableVehicleCollisions(RPCParameters* parameters);
// 0x0001acd0
void ScrSetGravity(RPCParameters* parameters);
// 0x0001ccf0
void ScrSetPlayerWantedLevel(RPCParameters* parameters);
// 0x00019550
void ScrSetPlayerHealth(RPCParameters* parameters);
// 0x0001ab60
void ScrSetPlayerArmor(RPCParameters* parameters);
// 0x00019a00
void ScrSetPlayerInterior(RPCParameters* parameters);
// 0x0001a500
void ScrGivePlayerMoney(RPCParameters* parameters);
// 0x00018090
void ScrResetPlayerMoney(RPCParameters* parameters);
// 0x00018690
void ScrSetPlayerSpecialAction(RPCParameters* parameters);
// 0x0001a290
void ScrTogglePlayerControllable(RPCParameters* parameters);
// R5 address to verify
extern const int g_rpcScrSetPlayerArmor;
// 0x0001e130
void RegisterScriptRPCs(RakClientInterface* client);
// 0x00018110
void UnRegisterScriptRPCs(RakClientInterface* client);
// 0x000e63b8
extern const int g_rpcScrDisableVehicleCollisions;
// 0x000e63b4
extern const int g_rpcScrForceClassSelection;
// 0x000e63b0
extern const int g_rpcScrAttachObjectToPlayer;
// R5 address to verify
extern const int g_rpcScrInitMenu;
// 0x000e63a8
extern const int g_rpcScrShowMenu;
// R5 address to verify
extern const int g_rpcScrHideMenu;
// 0x000e63a0
extern const int g_rpcScrSetPlayerWantedLevel;
// 0x000e639c
extern const int g_rpcScrShowTextDraw;
// 0x000e6398
extern const int g_rpcScrHideTextDrawForPlayer;
// 0x000e6394
extern const int g_rpcScrTextDrawSetString;
// 0x000e6390
extern const int g_rpcScrGangZoneCreate;
// 0x000e638c
extern const int g_rpcScrGangZoneDestroy;
// 0x000e6388
extern const int g_rpcScrGangZoneFlash;
// 0x000e6384
extern const int g_rpcScrGangZoneStopFlash;
// 0x000e6380
extern const int g_rpcScrApplyAnimation;
// 0x000e637c
extern const int g_rpcScrClearAnimations;
// 0x000e6378
extern const int g_rpcScrSetPlayerSpecialAction;
// 0x000e6374
extern const int g_rpcScrEnableStuntBonusForPlayer;
// 0x000e637c
extern const int g_rpcScrSetPlayerFightingStyle;
// 0x000e636c
extern const int g_rpcScrSetPlayerVelocity;
// 0x000e6368
extern const int g_rpcScrSetVehicleVelocity;
// 0x000e6364
extern const int g_rpcScrSetPlayerWidescreen;
// 0x000e6360
extern const int g_rpcScrSetVehicleTires;
// R5 address to verify
extern const int g_rpcPlayerOpcode052C;
// R5 address to verify
extern const int g_rpcPlayerOpcode03FD;
// R5 address to verify
extern const int g_rpcScrPlayCrimeReport;
// 0x000e635c
extern const int g_rpcScrSetPlayerMapIcon;
// R5 address to verify
extern const int g_rpcScrRemovePlayerMapIcon;
// R5 address to verify
extern const int g_rpcScrSetPlayerAmmo;
// R5 address to verify
extern const int g_rpcScrSetGravity;
// R5 address to verify
extern const int g_rpcScrSetVehicleHealth;
// R5 address to verify
extern const int g_rpcScrAttachTrailerToVehicle;
// R5 address to verify
extern const int g_rpcScrDetachTrailerFromVehicle;
// R5 address to verify
extern const int g_rpcScrCreateObject;
// R5 address to verify
extern const int g_rpcScrSetObjectPosition;
// R5 address to verify
extern const int g_rpcScrSetObjectRotation;
// R5 address to verify
extern const int g_rpcScrDestroyObject;
// R5 address to verify
extern const int g_rpcScrCreateExplosion;
// R5 address to verify
extern const int g_rpcScrShowPlayerNametagForPlayer;
// R5 address to verify
extern const int g_rpcScrMoveObject;
// R5 address to verify
extern const int g_rpcScrStopObject;
// R5 address to verify
extern const int g_rpcScrSetNumberPlate;
// R5 address to verify
extern const int g_rpcScrTogglePlayerSpectating;
// R5 address to verify
extern const int g_rpcUnknown001183A0;
// R5 address to verify
extern const int g_rpcScrPlayerSpectatePlayer;
// R5 address to verify
extern const int g_rpcScrPlayerSpectateVehicle;
// 0x000e6310
extern const int g_rpcScrMoveVehicleComponent;
// 0x000e630c
extern const int g_rpcScrSetPlayerShopName;
// 0x000e6308
extern const int g_rpcScrSetPlayerDrunkLevel;
// 0x000e6304
extern const int g_rpcScrSetPlayerArmedWeapon;
// R5 address to verify
extern const int g_rpcScrSetPlayerAttachedObject;
// R5 address to verify
extern const int g_rpcScrPlayAudioStream;
// R5 address to verify
extern const int g_rpcScrStopAudioStream;
// 0x000e62f4
extern const int g_rpcScrRemoveBuildingForPlayer;
// 0x000e62f0
extern const int g_rpcScrSetSpawnInfo;
// 0x000e62ec
extern const int g_rpcScrSetPlayerTeam;
// R5 address to verify
extern const int g_rpcScrSetPlayerName;
// R5 address to verify
extern const int g_rpcScrSetPlayerSkin;
// 0x000e62d4
extern const int g_rpcScrSetPlayerPosition;
// 0x000e62d0
extern const int g_rpcScrSetPlayerPositionFindZ;
// 0x000e62cc
extern const int g_rpcScrSetPlayerHealth;
// R5 address to verify
extern const int g_rpcScrPutPlayerInVehicle;
// 0x000e62d0
extern const int g_rpcScrRemovePlayerFromVehicle;
// R5 address to verify
extern const int g_rpcScrSetPlayerColor;
// 0x000e62c8
extern const int g_rpcScrDisplayGametext;
// 0x000e62b8
extern const int g_rpcScrSetPlayerInterior;
// 0x000e62b4
extern const int g_rpcScrSetPlayerCameraPosition;
// 0x000e62b0
extern const int g_rpcScrSetPlayerCameraLookAt;
// 0x000e62ac
extern const int g_rpcScrSetVehiclePosition;
// 0x000e62a8
extern const int g_rpcScrSetVehicleZAngle;
// 0x000e62a4
extern const int g_rpcScrSetVehicleParamsForPlayer;
// 0x000e62a0
extern const int g_rpcScrSetCameraBehindPlayer;
// 0x000e629c
extern const int g_rpcScrTogglePlayerControllable;
// 0x000e6298
extern const int g_rpcScrPlaySound;
// 0x000e6294
extern const int g_rpcScrSetPlayerWorldBounds;
// 0x000e6290
extern const int g_rpcScrGivePlayerMoney;
// 0x000e628c
extern const int g_rpcScrSetPlayerFacingAngle;
// 0x000e6288
extern const int g_rpcScrResetPlayerMoney;
// 0x000e6284
extern const int g_rpcScrResetPlayerWeapons;
// R5 address to verify
extern const int g_rpcScrGivePlayerWeapon;
// R5 address to verify
extern const int g_rpcUnknown0011830C;
// 0x000e6284
extern const int g_rpcScrLinkVehicleToInterior;
// 0x000e6280
extern const int g_rpcScrDeathMessage;
// R5 address to verify
extern const int g_rpcScrAttachCameraToObject;
// R5 address to verify
extern const int g_rpcScrInterpolateCamera;
// 0x000e6264
extern const int g_rpcScrSetObjectMaterial;
// 0x000e6260
extern const int g_rpcScrStopObjectCameraCollision;
// 0x000e625c
extern const int g_rpcScrSetActorAnimation;
// 0x000e6258
extern const int g_rpcScrClearActorAnimations;
// 0x000e6254
extern const int g_rpcScrSetActorRotation;
// 0x000e6250
extern const int g_rpcScrSetActorPosition;
// 0x000e624c
extern const int g_rpcScrSetActorHealth;
// R5 address to verify
extern const int g_rpcSetCustomModelVirtualWorld;

#endif // SCRIPTRPC_H
