#ifndef SAMP_SCRIPTRPC_H
#define SAMP_SCRIPTRPC_H
class RakClientInterface;
struct RPCParameters;
// samp.dll RVA: 0x00019420
void ScrDisplayGametext(RPCParameters *parameters);
// samp.dll RVA: 0x00017960
void ScrSetPlayerShopName(RPCParameters *parameters);
// samp.dll RVA: 0x00019330
void ScrSetPlayerColor(RPCParameters *parameters);
// samp.dll RVA: 0x0001d830
void ScrDeathMessage(RPCParameters *parameters);
// samp.dll RVA: 0x0001c390
void ScrInitMenu(RPCParameters *parameters);
// samp.dll RVA: 0x0001c650
void ScrShowMenu(RPCParameters *parameters);
// samp.dll RVA: 0x0001c700
void ScrHideMenu(RPCParameters *parameters);
// samp.dll RVA: 0x0001c850
void ScrShowTextDraw(RPCParameters *parameters);
// samp.dll RVA: 0x0001c980
void ScrHideTextDrawForPlayer(RPCParameters *parameters);
// Verified entry; external SAMP-API1CF70 points inside the audio-stream handler.
// samp.dll RVA: 0x0001ca30
void ScrTextDrawSetString(RPCParameters *parameters);
// samp.dll RVA: 0x00019e60
void ScrPlaySound(RPCParameters *parameters);
// samp.dll RVA: 0x0001ce80
void ScrPlayAudioStream(RPCParameters *parameters);
// samp.dll RVA: 0x00017c00
void ScrStopAudioStream(RPCParameters *parameters);
// samp.dll RVA: 0x00118494
extern const char g_rpcLargeTextDrawWarning[];
// samp.dll RVA: 0x000195d0
void ScrSetPlayerCameraPosition(RPCParameters *parameters);
// samp.dll RVA: 0x000196a0
void ScrSetPlayerCameraLookAt(RPCParameters *parameters);
// samp.dll RVA: 0x00017b90
void ScrSetCameraBehindPlayer(RPCParameters *parameters);
// samp.dll RVA: 0x00018e50
void ScrSetPlayerPosition(RPCParameters *parameters);
// samp.dll RVA: 0x00018f70
void ScrSetPlayerPositionFindZ(RPCParameters *parameters);
// samp.dll RVA: 0x00018350
void ScrSetPlayerVelocity(RPCParameters *parameters);
// samp.dll RVA: 0x00019f40
void ScrSetPlayerWorldBounds(RPCParameters *parameters);
// samp.dll RVA: 0x0001a0d0
void ScrSetPlayerFacingAngle(RPCParameters *parameters);
// samp.dll RVA: 0x00017860
void ScrEnableStuntBonusForPlayer(RPCParameters *parameters);
// samp.dll RVA: 0x00017be0
void ScrForceClassSelection(RPCParameters *parameters);
// samp.dll RVA: 0x00017bb0
void ScrResetPlayerWeapons(RPCParameters *parameters);
// samp.dll RVA: 0x0001a170
void ScrGivePlayerWeapon(RPCParameters *parameters);
// samp.dll RVA: 0x0001cb40
void ScrGangZoneCreate(RPCParameters *parameters);
// samp.dll RVA: 0x0001cc60
void ScrGangZoneDestroy(RPCParameters *parameters);
// samp.dll RVA: 0x0001cd10
void ScrGangZoneFlash(RPCParameters *parameters);
// samp.dll RVA: 0x0001cdd0
void ScrGangZoneStopFlash(RPCParameters *parameters);
// samp.dll RVA: 0x000178f0
void ScrDisableVehicleCollisions(RPCParameters *parameters);
// samp.dll RVA: 0x0001a800
void ScrSetGravity(RPCParameters *parameters);
// samp.dll RVA: 0x0001c7b0
void ScrSetPlayerWantedLevel(RPCParameters *parameters);
// samp.dll RVA: 0x00019080
void ScrSetPlayerHealth(RPCParameters *parameters);
// samp.dll RVA: 0x0001a690
void ScrSetPlayerArmor(RPCParameters *parameters);
// samp.dll RVA: 0x00019530
void ScrSetPlayerInterior(RPCParameters *parameters);
// samp.dll RVA: 0x0001a030
void ScrGivePlayerMoney(RPCParameters *parameters);
// samp.dll RVA: 0x00017ba0
void ScrResetPlayerMoney(RPCParameters *parameters);
// samp.dll RVA: 0x000181a0
void ScrSetPlayerSpecialAction(RPCParameters *parameters);
// samp.dll RVA: 0x00019dc0
void ScrTogglePlayerControllable(RPCParameters *parameters);
// samp.dll RVA: 0x00017c20
void UnRegisterScriptRPCs(RakClientInterface *client);
// samp.dll RVA: 0x00118448
extern const int g_rpcScrDisableVehicleCollisions;
// samp.dll RVA: 0x00118444
extern const int g_rpcScrForceClassSelection;
// samp.dll RVA: 0x00118440
extern const int g_rpcScrAttachObjectToPlayer;
// samp.dll RVA: 0x0011843c
extern const int g_rpcScrInitMenu;
// samp.dll RVA: 0x00118438
extern const int g_rpcScrShowMenu;
// samp.dll RVA: 0x00118434
extern const int g_rpcScrHideMenu;
// samp.dll RVA: 0x00118430
extern const int g_rpcScrSetPlayerWantedLevel;
// samp.dll RVA: 0x0011842c
extern const int g_rpcScrShowTextDraw;
// samp.dll RVA: 0x00118428
extern const int g_rpcScrHideTextDrawForPlayer;
// samp.dll RVA: 0x00118424
extern const int g_rpcScrTextDrawSetString;
// samp.dll RVA: 0x00118420
extern const int g_rpcScrGangZoneCreate;
// samp.dll RVA: 0x0011841c
extern const int g_rpcScrGangZoneDestroy;
// samp.dll RVA: 0x00118418
extern const int g_rpcScrGangZoneFlash;
// samp.dll RVA: 0x00118414
extern const int g_rpcScrGangZoneStopFlash;
// samp.dll RVA: 0x00118410
extern const int g_rpcScrApplyAnimation;
// samp.dll RVA: 0x0011840c
extern const int g_rpcScrClearAnimations;
// samp.dll RVA: 0x00118408
extern const int g_rpcScrSetPlayerSpecialAction;
// samp.dll RVA: 0x00118404
extern const int g_rpcScrEnableStuntBonusForPlayer;
// samp.dll RVA: 0x00118400
extern const int g_rpcScrSetPlayerFightingStyle;
// samp.dll RVA: 0x001183fc
extern const int g_rpcScrSetPlayerVelocity;
// samp.dll RVA: 0x001183f8
extern const int g_rpcScrSetVehicleVelocity;
// samp.dll RVA: 0x001183f4
extern const int g_rpcUnknown001183F4;
// samp.dll RVA: 0x001183f0
extern const int g_rpcUnknown001183F0;
// samp.dll RVA: 0x001183ec
extern const int g_rpcUnknown001183EC;
// samp.dll RVA: 0x001183e8
extern const int g_rpcUnknown001183E8;
// samp.dll RVA: 0x001183e4
extern const int g_rpcScrPlayCrimeReport;
// samp.dll RVA: 0x001183e0
extern const int g_rpcScrSetPlayerMapIcon;
// samp.dll RVA: 0x001183dc
extern const int g_rpcScrRemovePlayerMapIcon;
// samp.dll RVA: 0x001183d8
extern const int g_rpcScrSetPlayerAmmo;
// samp.dll RVA: 0x001183d4
extern const int g_rpcScrSetGravity;
// samp.dll RVA: 0x001183d0
extern const int g_rpcScrSetVehicleHealth;
// samp.dll RVA: 0x001183cc
extern const int g_rpcScrAttachTrailerToVehicle;
// samp.dll RVA: 0x001183c8
extern const int g_rpcScrDetachTrailerFromVehicle;
// samp.dll RVA: 0x001183c4
extern const int g_rpcScrCreateObject;
// samp.dll RVA: 0x001183c0
extern const int g_rpcScrSetObjectPosition;
// samp.dll RVA: 0x001183bc
extern const int g_rpcScrSetObjectRotation;
// samp.dll RVA: 0x00117694
extern const int g_rpcScrDestroyObject;
// samp.dll RVA: 0x001183b8
extern const int g_rpcScrCreateExplosion;
// samp.dll RVA: 0x001183b4
extern const int g_rpcScrShowPlayerNametagForPlayer;
// samp.dll RVA: 0x001183b0
extern const int g_rpcScrMoveObject;
// samp.dll RVA: 0x001183ac
extern const int g_rpcScrStopObject;
// samp.dll RVA: 0x001183a8
extern const int g_rpcScrSetNumberPlate;
// samp.dll RVA: 0x001183a4
extern const int g_rpcScrTogglePlayerSpectating;
// samp.dll RVA: 0x001183a0
extern const int g_rpcUnknown001183A0;
// samp.dll RVA: 0x0011839c
extern const int g_rpcScrPlayerSpectatePlayer;
// samp.dll RVA: 0x00118398
extern const int g_rpcScrPlayerSpectateVehicle;
// samp.dll RVA: 0x00118394
extern const int g_rpcScrMoveVehicleComponent;
// samp.dll RVA: 0x00118390
extern const int g_rpcScrSetPlayerShopName;
// samp.dll RVA: 0x0011838c
extern const int g_rpcScrSetPlayerDrunkLevel;
// samp.dll RVA: 0x00118388
extern const int g_rpcScrSetPlayerArmedWeapon;
// samp.dll RVA: 0x00118384
extern const int g_rpcScrSetPlayerAttachedObject;
// samp.dll RVA: 0x00118380
extern const int g_rpcScrPlayAudioStream;
// samp.dll RVA: 0x0011837c
extern const int g_rpcScrStopAudioStream;
// samp.dll RVA: 0x00118378
extern const int g_rpcScrRemoveBuildingForPlayer;
// samp.dll RVA: 0x00118374
extern const int g_rpcScrSetSpawnInfo;
// samp.dll RVA: 0x00118370
extern const int g_rpcScrSetPlayerTeam;
// samp.dll RVA: 0x0011836c
extern const int g_rpcScrSetPlayerName;
// samp.dll RVA: 0x00118368
extern const int g_rpcScrSetPlayerSkin;
// samp.dll RVA: 0x00118364
extern const int g_rpcScrSetPlayerPosition;
// samp.dll RVA: 0x00118360
extern const int g_rpcScrSetPlayerPositionFindZ;
// samp.dll RVA: 0x0011835c
extern const int g_rpcScrSetPlayerHealth;
// samp.dll RVA: 0x00118358
extern const int g_rpcScrPutPlayerInVehicle;
// samp.dll RVA: 0x00118354
extern const int g_rpcScrRemovePlayerFromVehicle;
// samp.dll RVA: 0x00118350
extern const int g_rpcScrSetPlayerColor;
// samp.dll RVA: 0x0011834c
extern const int g_rpcScrDisplayGametext;
// samp.dll RVA: 0x00118348
extern const int g_rpcScrSetPlayerInterior;
// samp.dll RVA: 0x00118344
extern const int g_rpcScrSetPlayerCameraPosition;
// samp.dll RVA: 0x00118340
extern const int g_rpcScrSetPlayerCameraLookAt;
// samp.dll RVA: 0x0011833c
extern const int g_rpcScrSetVehiclePosition;
// samp.dll RVA: 0x00118338
extern const int g_rpcScrSetVehicleZAngle;
// samp.dll RVA: 0x00118334
extern const int g_rpcScrSetVehicleParamsForPlayer;
// samp.dll RVA: 0x00118330
extern const int g_rpcScrSetCameraBehindPlayer;
// samp.dll RVA: 0x0011832c
extern const int g_rpcScrTogglePlayerControllable;
// samp.dll RVA: 0x00118328
extern const int g_rpcScrPlaySound;
// samp.dll RVA: 0x00118324
extern const int g_rpcScrSetPlayerWorldBounds;
// samp.dll RVA: 0x00118320
extern const int g_rpcScrGivePlayerMoney;
// samp.dll RVA: 0x0011831c
extern const int g_rpcScrSetPlayerFacingAngle;
// samp.dll RVA: 0x00118318
extern const int g_rpcScrResetPlayerMoney;
// samp.dll RVA: 0x00118314
extern const int g_rpcScrResetPlayerWeapons;
// samp.dll RVA: 0x00118310
extern const int g_rpcScrGivePlayerWeapon;
// samp.dll RVA: 0x0011830c
extern const int g_rpcUnknown0011830C;
// samp.dll RVA: 0x00118308
extern const int g_rpcScrLinkVehicleToInterior;
// samp.dll RVA: 0x00118304
extern const int g_rpcScrDeathMessage;
// samp.dll RVA: 0x00118300
extern const int g_rpcScrAttachCameraToObject;
// samp.dll RVA: 0x001182fc
extern const int g_rpcScrInterpolateCamera;
// samp.dll RVA: 0x001182f4
extern const int g_rpcScrSetObjectMaterial;
// samp.dll RVA: 0x001182f0
extern const int g_rpcScrStopObjectCameraCollision;
// samp.dll RVA: 0x001182ec
extern const int g_rpcScrSetActorAnimation;
// samp.dll RVA: 0x001182e8
extern const int g_rpcUnknown001182E8;
// samp.dll RVA: 0x001182e4
extern const int g_rpcScrSetActorRotation;
// samp.dll RVA: 0x001182e0
extern const int g_rpcScrSetActorPosition;
// samp.dll RVA: 0x001182dc
extern const int g_rpcScrSetActorHealth;
// samp.dll RVA: 0x001182d8
extern const int g_rpcUnknown001182D8;
#endif
