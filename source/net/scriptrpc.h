#ifndef SAMP_SCRIPTRPC_H
#define SAMP_SCRIPTRPC_H
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
// 0x0001aba0
void ObjectReadMaterials(unsigned char count, CObject* object, RakNet::BitStream* stream);
// 0x0001ae70
void ScrCreateObject(RPCParameters* parameters);
// 0x0001b1d0
void ScrSetObjectMaterial(RPCParameters* parameters);
// 0x0001c1d0
void ScrAttachObjectToPlayer(RPCParameters* parameters);
// 0x00018a00
void ScrSetPlayerAttachedObject(RPCParameters* parameters);
// Native updates custom-model state; higher-level name remains unestablished.
// 0x0001d780
void ScrRpc48(RPCParameters* parameters);
// 0x00118288
extern const Scripting::OpcodeInfo g_rpcAttachObjectToPlayerCommand;
// 0x0001da90
void ScrSetPlayerName(RPCParameters* parameters);
// 0x000188b0
void ScrSetPlayerDrunkLevel(RPCParameters* parameters);
// 0x00018950
void ScrSetPlayerArmedWeapon(RPCParameters* parameters);
// 0x00019b20
void ScrAttachCameraToObject(RPCParameters* parameters);
// 0x00019c20
void ScrInterpolateCamera(RPCParameters* parameters);
// 0x0001d110
void ClickTextDraw(RPCParameters* parameters);
// 0x0001bca0
void ScrStopObjectCameraCollision(RPCParameters* parameters);
// 0x0001cff0
void ScrRemoveBuildingForPlayer(RPCParameters* parameters);
// 0x00018b40
void ScrPlayCrimeReport(RPCParameters* parameters);
// Opcode-specific names: higher-level native semantics not established.
// 0x00018750
void ScrPlayerOpcode052C(RPCParameters* parameters);
// 0x00018800
void ScrPlayerOpcode03FD(RPCParameters* parameters);
// Native is a bare RET, not a reconstruction stub.
// 0x00017bd0
void ScrRpc125(RPCParameters* parameters);
// Native consumes and discards a WORD, with stream construction/destruction.
// 0x0001a230
void ScrRpc64(RPCParameters* parameters);
// 0x001182b0
extern const Scripting::OpcodeInfo g_rpcPlayerOpcode052cCommand;
// 0x001182c4
extern const Scripting::OpcodeInfo g_rpcPlayerOpcode03fdCommand;
// 0x00017a60
void ScrSetSpawnInfo(RPCParameters* parameters);
// 0x00018250
void ScrSetPlayerFightingStyle(RPCParameters* parameters);
// 0x0001a740
void ScrSetPlayerAmmo(RPCParameters* parameters);
// 0x00019240
void ScrSetPlayerTeam(RPCParameters* parameters);
// 0x00018c80
void ScrSetPlayerSkin(RPCParameters* parameters);
// 0x0001a480
void ScrApplyAnimation(RPCParameters* parameters);
// 0x00018090
void ScrClearAnimations(RPCParameters* parameters);
// 0x0001c0f0
void ScrMoveVehicleComponent(RPCParameters* parameters);
// Descriptive name established by native message/opcode, RPC111.
// 0x000185c0
void ScrSetPlayerWidescreen(RPCParameters* parameters);
// Descriptive name established by the native SetTires call, RPC98.
// 0x00018670
void ScrSetVehicleTires(RPCParameters* parameters);
// 0x00118274
extern const Scripting::OpcodeInfo g_rpcWidescreenCommand;
// 0x0011844c
extern const char g_rpcWidescreenMessage[];
// 0x0011845c
extern const char g_rpcInvalidSkinWarning[];
// 0x0011829c
extern const Scripting::OpcodeInfo g_rpcExplosionCommand;
// 0x0001b540
void ScrSetObjectPosition(RPCParameters* parameters);
// 0x0001b650
void ScrSetObjectRotation(RPCParameters* parameters);
// 0x0001b750
void ScrDestroyObject(RPCParameters* parameters);
// 0x0001ba30
void ScrMoveObject(RPCParameters* parameters);
// 0x0001bbe0
void ScrStopObject(RPCParameters* parameters);
// 0x0001d210
void ScrSetActorAnimation(RPCParameters* parameters);
// Descriptive name established by RPC174 and the actor ClearTasks call.
// 0x0001d3f0
void ScrClearActorAnimations(RPCParameters* parameters);
// 0x0001d4b0
void ScrSetActorRotation(RPCParameters* parameters);
// 0x0001d590
void ScrSetActorPosition(RPCParameters* parameters);
// 0x0001d6a0
void ScrSetActorHealth(RPCParameters* parameters);
// 0x0001a2c0
void ScrSetPlayerMapIcon(RPCParameters* parameters);
// 0x0001a3e0
void ScrRemovePlayerMapIcon(RPCParameters* parameters);
// 0x0001b840
void ScrCreateExplosion(RPCParameters* parameters);
// 0x0001b950
void ScrShowPlayerNametagForPlayer(RPCParameters* parameters);
// 0x000197a0
void ScrSetVehiclePosition(RPCParameters* parameters);
// 0x000198b0
void ScrSetVehicleZAngle(RPCParameters* parameters);
// 0x00019990
void ScrSetVehicleParamsForPlayer(RPCParameters* parameters);
// 0x00019a60
void ScrLinkVehicleToInterior(RPCParameters* parameters);
// 0x0001a8a0
void ScrSetVehicleHealth(RPCParameters* parameters);
// 0x0001a980
void ScrAttachTrailerToVehicle(RPCParameters* parameters);
// 0x0001aac0
void ScrDetachTrailerFromVehicle(RPCParameters* parameters);
// 0x00018450
void ScrSetVehicleVelocity(RPCParameters* parameters);
// 0x0001bd60
void ScrSetNumberPlate(RPCParameters* parameters);
// 0x0001be80
void ScrTogglePlayerSpectating(RPCParameters* parameters);
// 0x0001bf30
void ScrPlayerSpectatePlayer(RPCParameters* parameters);
// 0x0001c010
void ScrPlayerSpectateVehicle(RPCParameters* parameters);
// 0x00019130
void ScrPutPlayerInVehicle(RPCParameters* parameters);
// 0x00017b00
void ScrRemovePlayerFromVehicle(RPCParameters* parameters);
// 0x00019420
void ScrDisplayGametext(RPCParameters* parameters);
// 0x00017960
void ScrSetPlayerShopName(RPCParameters* parameters);
// 0x00019330
void ScrSetPlayerColor(RPCParameters* parameters);
// 0x0001d830
void ScrDeathMessage(RPCParameters* parameters);
// 0x0001c390
void ScrInitMenu(RPCParameters* parameters);
// 0x0001c650
void ScrShowMenu(RPCParameters* parameters);
// 0x0001c700
void ScrHideMenu(RPCParameters* parameters);
// 0x0001c850
void ScrShowTextDraw(RPCParameters* parameters);
// 0x0001c980
void ScrHideTextDrawForPlayer(RPCParameters* parameters);
// Verified entry; external SAMP-API1CF70 points inside the audio-stream handler.
// 0x0001ca30
void ScrTextDrawSetString(RPCParameters* parameters);
// 0x00019e60
void ScrPlaySound(RPCParameters* parameters);
// 0x0001ce80
void ScrPlayAudioStream(RPCParameters* parameters);
// 0x00017c00
void ScrStopAudioStream(RPCParameters* parameters);
// 0x00118494
extern const char g_rpcLargeTextDrawWarning[];
// 0x000195d0
void ScrSetPlayerCameraPosition(RPCParameters* parameters);
// 0x000196a0
void ScrSetPlayerCameraLookAt(RPCParameters* parameters);
// 0x00017b90
void ScrSetCameraBehindPlayer(RPCParameters* parameters);
// 0x00018e50
void ScrSetPlayerPosition(RPCParameters* parameters);
// 0x00018f70
void ScrSetPlayerPositionFindZ(RPCParameters* parameters);
// 0x00018350
void ScrSetPlayerVelocity(RPCParameters* parameters);
// 0x00019f40
void ScrSetPlayerWorldBounds(RPCParameters* parameters);
// 0x0001a0d0
void ScrSetPlayerFacingAngle(RPCParameters* parameters);
// 0x00017860
void ScrEnableStuntBonusForPlayer(RPCParameters* parameters);
// 0x00017be0
void ScrForceClassSelection(RPCParameters* parameters);
// 0x00017bb0
void ScrResetPlayerWeapons(RPCParameters* parameters);
// 0x0001a170
void ScrGivePlayerWeapon(RPCParameters* parameters);
// 0x0001cb40
void ScrGangZoneCreate(RPCParameters* parameters);
// 0x0001cc60
void ScrGangZoneDestroy(RPCParameters* parameters);
// 0x0001cd10
void ScrGangZoneFlash(RPCParameters* parameters);
// 0x0001cdd0
void ScrGangZoneStopFlash(RPCParameters* parameters);
// 0x000178f0
void ScrDisableVehicleCollisions(RPCParameters* parameters);
// 0x0001a800
void ScrSetGravity(RPCParameters* parameters);
// 0x0001c7b0
void ScrSetPlayerWantedLevel(RPCParameters* parameters);
// 0x00019080
void ScrSetPlayerHealth(RPCParameters* parameters);
// 0x0001a690
void ScrSetPlayerArmor(RPCParameters* parameters);
// 0x00019530
void ScrSetPlayerInterior(RPCParameters* parameters);
// 0x0001a030
void ScrGivePlayerMoney(RPCParameters* parameters);
// 0x00017ba0
void ScrResetPlayerMoney(RPCParameters* parameters);
// 0x000181a0
void ScrSetPlayerSpecialAction(RPCParameters* parameters);
// 0x00019dc0
void ScrTogglePlayerControllable(RPCParameters* parameters);
// 0x001184c0
extern const int g_rpcScrSetPlayerArmor;
// 0x0001dbf0
void RegisterScriptRPCs(RakClientInterface* client);
// 0x00017c20
void UnRegisterScriptRPCs(RakClientInterface* client);
// 0x00118448
extern const int g_rpcScrDisableVehicleCollisions;
// 0x00118444
extern const int g_rpcScrForceClassSelection;
// 0x00118440
extern const int g_rpcScrAttachObjectToPlayer;
// 0x0011843c
extern const int g_rpcScrInitMenu;
// 0x00118438
extern const int g_rpcScrShowMenu;
// 0x00118434
extern const int g_rpcScrHideMenu;
// 0x00118430
extern const int g_rpcScrSetPlayerWantedLevel;
// 0x0011842c
extern const int g_rpcScrShowTextDraw;
// 0x00118428
extern const int g_rpcScrHideTextDrawForPlayer;
// 0x00118424
extern const int g_rpcScrTextDrawSetString;
// 0x00118420
extern const int g_rpcScrGangZoneCreate;
// 0x0011841c
extern const int g_rpcScrGangZoneDestroy;
// 0x00118418
extern const int g_rpcScrGangZoneFlash;
// 0x00118414
extern const int g_rpcScrGangZoneStopFlash;
// 0x00118410
extern const int g_rpcScrApplyAnimation;
// 0x0011840c
extern const int g_rpcScrClearAnimations;
// 0x00118408
extern const int g_rpcScrSetPlayerSpecialAction;
// 0x00118404
extern const int g_rpcScrEnableStuntBonusForPlayer;
// 0x00118400
extern const int g_rpcScrSetPlayerFightingStyle;
// 0x001183fc
extern const int g_rpcScrSetPlayerVelocity;
// 0x001183f8
extern const int g_rpcScrSetVehicleVelocity;
// 0x001183f4
extern const int g_rpcScrSetPlayerWidescreen;
// 0x001183f0
extern const int g_rpcScrSetVehicleTires;
// 0x001183ec
extern const int g_rpcUnknown001183EC;
// 0x001183e8
extern const int g_rpcUnknown001183E8;
// 0x001183e4
extern const int g_rpcScrPlayCrimeReport;
// 0x001183e0
extern const int g_rpcScrSetPlayerMapIcon;
// 0x001183dc
extern const int g_rpcScrRemovePlayerMapIcon;
// 0x001183d8
extern const int g_rpcScrSetPlayerAmmo;
// 0x001183d4
extern const int g_rpcScrSetGravity;
// 0x001183d0
extern const int g_rpcScrSetVehicleHealth;
// 0x001183cc
extern const int g_rpcScrAttachTrailerToVehicle;
// 0x001183c8
extern const int g_rpcScrDetachTrailerFromVehicle;
// 0x001183c4
extern const int g_rpcScrCreateObject;
// 0x001183c0
extern const int g_rpcScrSetObjectPosition;
// 0x001183bc
extern const int g_rpcScrSetObjectRotation;
// 0x00117694
extern const int g_rpcScrDestroyObject;
// 0x001183b8
extern const int g_rpcScrCreateExplosion;
// 0x001183b4
extern const int g_rpcScrShowPlayerNametagForPlayer;
// 0x001183b0
extern const int g_rpcScrMoveObject;
// 0x001183ac
extern const int g_rpcScrStopObject;
// 0x001183a8
extern const int g_rpcScrSetNumberPlate;
// 0x001183a4
extern const int g_rpcScrTogglePlayerSpectating;
// 0x001183a0
extern const int g_rpcUnknown001183A0;
// 0x0011839c
extern const int g_rpcScrPlayerSpectatePlayer;
// 0x00118398
extern const int g_rpcScrPlayerSpectateVehicle;
// 0x00118394
extern const int g_rpcScrMoveVehicleComponent;
// 0x00118390
extern const int g_rpcScrSetPlayerShopName;
// 0x0011838c
extern const int g_rpcScrSetPlayerDrunkLevel;
// 0x00118388
extern const int g_rpcScrSetPlayerArmedWeapon;
// 0x00118384
extern const int g_rpcScrSetPlayerAttachedObject;
// 0x00118380
extern const int g_rpcScrPlayAudioStream;
// 0x0011837c
extern const int g_rpcScrStopAudioStream;
// 0x00118378
extern const int g_rpcScrRemoveBuildingForPlayer;
// 0x00118374
extern const int g_rpcScrSetSpawnInfo;
// 0x00118370
extern const int g_rpcScrSetPlayerTeam;
// 0x0011836c
extern const int g_rpcScrSetPlayerName;
// 0x00118368
extern const int g_rpcScrSetPlayerSkin;
// 0x00118364
extern const int g_rpcScrSetPlayerPosition;
// 0x00118360
extern const int g_rpcScrSetPlayerPositionFindZ;
// 0x0011835c
extern const int g_rpcScrSetPlayerHealth;
// 0x00118358
extern const int g_rpcScrPutPlayerInVehicle;
// 0x00118354
extern const int g_rpcScrRemovePlayerFromVehicle;
// 0x00118350
extern const int g_rpcScrSetPlayerColor;
// 0x0011834c
extern const int g_rpcScrDisplayGametext;
// 0x00118348
extern const int g_rpcScrSetPlayerInterior;
// 0x00118344
extern const int g_rpcScrSetPlayerCameraPosition;
// 0x00118340
extern const int g_rpcScrSetPlayerCameraLookAt;
// 0x0011833c
extern const int g_rpcScrSetVehiclePosition;
// 0x00118338
extern const int g_rpcScrSetVehicleZAngle;
// 0x00118334
extern const int g_rpcScrSetVehicleParamsForPlayer;
// 0x00118330
extern const int g_rpcScrSetCameraBehindPlayer;
// 0x0011832c
extern const int g_rpcScrTogglePlayerControllable;
// 0x00118328
extern const int g_rpcScrPlaySound;
// 0x00118324
extern const int g_rpcScrSetPlayerWorldBounds;
// 0x00118320
extern const int g_rpcScrGivePlayerMoney;
// 0x0011831c
extern const int g_rpcScrSetPlayerFacingAngle;
// 0x00118318
extern const int g_rpcScrResetPlayerMoney;
// 0x00118314
extern const int g_rpcScrResetPlayerWeapons;
// 0x00118310
extern const int g_rpcScrGivePlayerWeapon;
// 0x0011830c
extern const int g_rpcUnknown0011830C;
// 0x00118308
extern const int g_rpcScrLinkVehicleToInterior;
// 0x00118304
extern const int g_rpcScrDeathMessage;
// 0x00118300
extern const int g_rpcScrAttachCameraToObject;
// 0x001182fc
extern const int g_rpcScrInterpolateCamera;
// 0x001182f4
extern const int g_rpcScrSetObjectMaterial;
// 0x001182f0
extern const int g_rpcScrStopObjectCameraCollision;
// 0x001182ec
extern const int g_rpcScrSetActorAnimation;
// 0x001182e8
extern const int g_rpcScrClearActorAnimations;
// 0x001182e4
extern const int g_rpcScrSetActorRotation;
// 0x001182e0
extern const int g_rpcScrSetActorPosition;
// 0x001182dc
extern const int g_rpcScrSetActorHealth;
// 0x001182d8
extern const int g_rpcUnknown001182D8;
#endif
