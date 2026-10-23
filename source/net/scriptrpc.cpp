#include "net/scriptrpc.h"
#include "RakClientInterface.h"
#include "textdrawselection.h"
// samp.dll RVA: 0x00118448
const int g_rpcScrDisableVehicleCollisions=167;
// samp.dll RVA: 0x00118444
const int g_rpcScrForceClassSelection=74;
// samp.dll RVA: 0x00118440
const int g_rpcScrAttachObjectToPlayer=75;
// samp.dll RVA: 0x0011843c
const int g_rpcScrInitMenu=76;
// samp.dll RVA: 0x00118438
const int g_rpcScrShowMenu=77;
// samp.dll RVA: 0x00118434
const int g_rpcScrHideMenu=78;
// samp.dll RVA: 0x00118430
const int g_rpcScrSetPlayerWantedLevel=133;
// samp.dll RVA: 0x0011842c
const int g_rpcScrShowTextDraw=134;
// samp.dll RVA: 0x00118428
const int g_rpcScrHideTextDrawForPlayer=135;
// Handler RVA0x0001ca30; semantic name not established.
// samp.dll RVA: 0x00118424
const int g_rpcUnknown00118424=105;
// samp.dll RVA: 0x00118420
const int g_rpcScrGangZoneCreate=108;
// samp.dll RVA: 0x0011841c
const int g_rpcScrGangZoneDestroy=120;
// samp.dll RVA: 0x00118418
const int g_rpcScrGangZoneFlash=121;
// samp.dll RVA: 0x00118414
const int g_rpcScrGangZoneStopFlash=85;
// samp.dll RVA: 0x00118410
const int g_rpcScrApplyAnimation=86;
// samp.dll RVA: 0x0011840c
const int g_rpcScrClearAnimations=87;
// samp.dll RVA: 0x00118408
const int g_rpcScrSetPlayerSpecialAction=88;
// samp.dll RVA: 0x00118404
const int g_rpcScrEnableStuntBonusForPlayer=104;
// samp.dll RVA: 0x00118400
const int g_rpcScrSetPlayerFightingStyle=89;
// samp.dll RVA: 0x001183fc
const int g_rpcScrSetPlayerVelocity=90;
// samp.dll RVA: 0x001183f8
const int g_rpcScrSetVehicleVelocity=91;
// Handler RVA0x000185c0; semantic name not established.
// samp.dll RVA: 0x001183f4
const int g_rpcUnknown001183F4=111;
// Handler RVA0x00018670; semantic name not established.
// samp.dll RVA: 0x001183f0
const int g_rpcUnknown001183F0=98;
// Handler RVA0x00018750; semantic name not established.
// samp.dll RVA: 0x001183ec
const int g_rpcUnknown001183EC=92;
// Handler RVA0x00018800; semantic name not established.
// samp.dll RVA: 0x001183e8
const int g_rpcUnknown001183E8=150;
// samp.dll RVA: 0x001183e4
const int g_rpcScrPlayCrimeReport=112;
// samp.dll RVA: 0x001183e0
const int g_rpcScrSetPlayerMapIcon=56;
// samp.dll RVA: 0x001183dc
const int g_rpcScrRemovePlayerMapIcon=144;
// samp.dll RVA: 0x001183d8
const int g_rpcScrSetPlayerAmmo=145;
// samp.dll RVA: 0x001183d4
const int g_rpcScrSetGravity=146;
// samp.dll RVA: 0x001183d0
const int g_rpcScrSetVehicleHealth=147;
// samp.dll RVA: 0x001183cc
const int g_rpcScrAttachTrailerToVehicle=148;
// samp.dll RVA: 0x001183c8
const int g_rpcScrDetachTrailerFromVehicle=149;
// samp.dll RVA: 0x001183c4
const int g_rpcScrCreateObject=44;
// samp.dll RVA: 0x001183c0
const int g_rpcScrSetObjectPosition=45;
// samp.dll RVA: 0x001183bc
const int g_rpcScrSetObjectRotation=46;
// samp.dll RVA: 0x00117694
const int g_rpcScrDestroyObject=47;
// samp.dll RVA: 0x001183b8
const int g_rpcScrCreateExplosion=79;
// samp.dll RVA: 0x001183b4
const int g_rpcScrShowPlayerNametagForPlayer=80;
// samp.dll RVA: 0x001183b0
const int g_rpcScrMoveObject=99;
// samp.dll RVA: 0x001183ac
const int g_rpcScrStopObject=122;
// samp.dll RVA: 0x001183a8
const int g_rpcScrSetNumberPlate=123;
// samp.dll RVA: 0x001183a4
const int g_rpcScrTogglePlayerSpectating=124;
// Handler RVA0x00017bd0; semantic name not established.
// samp.dll RVA: 0x001183a0
const int g_rpcUnknown001183A0=125;
// samp.dll RVA: 0x0011839c
const int g_rpcScrPlayerSpectatePlayer=126;
// samp.dll RVA: 0x00118398
const int g_rpcScrPlayerSpectateVehicle=127;
// samp.dll RVA: 0x00118394
const int g_rpcScrMoveVehicleComponent=57;
// samp.dll RVA: 0x00118390
const int g_rpcScrSetPlayerShopName=33;
// samp.dll RVA: 0x0011838c
const int g_rpcScrSetPlayerDrunkLevel=35;
// samp.dll RVA: 0x00118388
const int g_rpcScrSetPlayerArmedWeapon=67;
// samp.dll RVA: 0x00118384
const int g_rpcScrSetPlayerAttachedObject=113;
// samp.dll RVA: 0x00118380
const int g_rpcScrPlayAudioStream=41;
// samp.dll RVA: 0x0011837c
const int g_rpcScrStopAudioStream=42;
// samp.dll RVA: 0x00118378
const int g_rpcScrRemoveBuildingForPlayer=43;
// samp.dll RVA: 0x00118374
const int g_rpcScrSetSpawnInfo=68;
// samp.dll RVA: 0x00118370
const int g_rpcScrSetPlayerTeam=69;
// samp.dll RVA: 0x0011836c
const int g_rpcScrSetPlayerName=11;
// samp.dll RVA: 0x00118368
const int g_rpcScrSetPlayerSkin=153;
// samp.dll RVA: 0x00118364
const int g_rpcScrSetPlayerPosition=12;
// samp.dll RVA: 0x00118360
const int g_rpcScrSetPlayerPositionFindZ=13;
// samp.dll RVA: 0x0011835c
const int g_rpcScrSetPlayerHealth=14;
// samp.dll RVA: 0x00118358
const int g_rpcScrPutPlayerInVehicle=70;
// samp.dll RVA: 0x00118354
const int g_rpcScrRemovePlayerFromVehicle=71;
// samp.dll RVA: 0x00118350
const int g_rpcScrSetPlayerColor=72;
// samp.dll RVA: 0x0011834c
const int g_rpcScrDisplayGametext=73;
// samp.dll RVA: 0x00118348
const int g_rpcScrSetPlayerInterior=156;
// samp.dll RVA: 0x00118344
const int g_rpcScrSetPlayerCameraPosition=157;
// samp.dll RVA: 0x00118340
const int g_rpcScrSetPlayerCameraLookAt=158;
// samp.dll RVA: 0x0011833c
const int g_rpcScrSetVehiclePosition=159;
// samp.dll RVA: 0x00118338
const int g_rpcScrSetVehicleZAngle=160;
// samp.dll RVA: 0x00118334
const int g_rpcScrSetVehicleParamsForPlayer=161;
// samp.dll RVA: 0x00118330
const int g_rpcScrSetCameraBehindPlayer=162;
// samp.dll RVA: 0x0011832c
const int g_rpcScrTogglePlayerControllable=15;
// samp.dll RVA: 0x00118328
const int g_rpcScrPlaySound=16;
// samp.dll RVA: 0x00118324
const int g_rpcScrSetPlayerWorldBounds=17;
// samp.dll RVA: 0x00118320
const int g_rpcScrGivePlayerMoney=18;
// samp.dll RVA: 0x0011831c
const int g_rpcScrSetPlayerFacingAngle=19;
// samp.dll RVA: 0x00118318
const int g_rpcScrResetPlayerMoney=20;
// samp.dll RVA: 0x00118314
const int g_rpcScrResetPlayerWeapons=21;
// samp.dll RVA: 0x00118310
const int g_rpcScrGivePlayerWeapon=22;
// Handler RVA0x0001a230; semantic name not established.
// samp.dll RVA: 0x0011830c
const int g_rpcUnknown0011830C=64;
// samp.dll RVA: 0x00118308
const int g_rpcScrLinkVehicleToInterior=65;
// samp.dll RVA: 0x00118304
const int g_rpcScrDeathMessage=55;
// samp.dll RVA: 0x00118300
const int g_rpcScrAttachCameraToObject=81;
// samp.dll RVA: 0x001182fc
const int g_rpcScrInterpolateCamera=82;
// samp.dll RVA: 0x001182f4
const int g_rpcScrSetObjectMaterial=84;
// samp.dll RVA: 0x001182f0
const int g_rpcScrStopObjectCameraCollision=169;
// samp.dll RVA: 0x001182ec
const int g_rpcScrSetActorAnimation=173;
// Handler RVA0x0001d3f0; semantic name not established.
// samp.dll RVA: 0x001182e8
const int g_rpcUnknown001182E8=174;
// samp.dll RVA: 0x001182e4
const int g_rpcScrSetActorRotation=175;
// samp.dll RVA: 0x001182e0
const int g_rpcScrSetActorPosition=176;
// samp.dll RVA: 0x001182dc
const int g_rpcScrSetActorHealth=178;
// Handler RVA0x0001d780; semantic name not established.
// samp.dll RVA: 0x001182d8
const int g_rpcUnknown001182D8=48;
// samp.dll RVA: 0x00017c20
void UnRegisterScriptRPCs(RakClientInterface *client) {
    const int *ids[]={
        &g_rpcScrDisableVehicleCollisions,&g_rpcScrForceClassSelection,&g_rpcScrAttachObjectToPlayer,&g_rpcScrInitMenu,
        &g_rpcScrShowMenu,&g_rpcScrHideMenu,&g_rpcScrSetPlayerWantedLevel,&g_rpcScrShowTextDraw,
        &g_rpcScrHideTextDrawForPlayer,&g_rpcUnknown00118424,&g_rpcScrGangZoneCreate,&g_rpcScrGangZoneDestroy,
        &g_rpcScrGangZoneFlash,&g_rpcScrGangZoneStopFlash,&g_rpcScrApplyAnimation,&g_rpcScrClearAnimations,
        &g_rpcScrSetPlayerSpecialAction,&g_rpcScrEnableStuntBonusForPlayer,&g_rpcScrSetPlayerFightingStyle,&g_rpcScrSetPlayerVelocity,
        &g_rpcScrSetVehicleVelocity,&g_rpcUnknown001183F4,&g_rpcUnknown001183F0,&g_rpcUnknown001183EC,
        &g_rpcUnknown001183E8,&g_rpcScrPlayCrimeReport,&g_rpcScrSetPlayerMapIcon,&g_rpcScrRemovePlayerMapIcon,
        &g_rpcScrSetPlayerAmmo,&g_rpcScrSetGravity,&g_rpcScrSetVehicleHealth,&g_rpcScrAttachTrailerToVehicle,
        &g_rpcScrDetachTrailerFromVehicle,&g_rpcScrCreateObject,&g_rpcScrSetObjectPosition,&g_rpcScrSetObjectRotation,
        &g_rpcScrDestroyObject,&g_rpcScrCreateExplosion,&g_rpcScrShowPlayerNametagForPlayer,&g_rpcScrMoveObject,
        &g_rpcScrStopObject,&g_rpcScrSetNumberPlate,&g_rpcScrTogglePlayerSpectating,&g_rpcUnknown001183A0,
        &g_rpcScrPlayerSpectatePlayer,&g_rpcScrPlayerSpectateVehicle,&g_rpcScrMoveVehicleComponent,&g_rpcScrSetPlayerShopName,
        &g_rpcScrSetPlayerDrunkLevel,&g_rpcScrSetPlayerArmedWeapon,&g_rpcScrSetPlayerAttachedObject,&g_rpcScrPlayAudioStream,
        &g_rpcScrStopAudioStream,&g_rpcScrRemoveBuildingForPlayer,&g_rpcScrSetSpawnInfo,&g_rpcScrSetPlayerTeam,
        &g_rpcScrSetPlayerName,&g_rpcScrSetPlayerSkin,&g_rpcScrSetPlayerPosition,&g_rpcScrSetPlayerPositionFindZ,
        &g_rpcScrSetPlayerHealth,&g_rpcScrPutPlayerInVehicle,&g_rpcScrRemovePlayerFromVehicle,&g_rpcScrSetPlayerColor,
        &g_rpcScrDisplayGametext,&g_rpcScrSetPlayerInterior,&g_rpcScrSetPlayerCameraPosition,&g_rpcScrSetPlayerCameraLookAt,
        &g_rpcScrSetVehiclePosition,&g_rpcScrSetVehicleZAngle,&g_rpcScrSetVehicleParamsForPlayer,&g_rpcScrSetCameraBehindPlayer,
        &g_rpcScrTogglePlayerControllable,&g_rpcScrPlaySound,&g_rpcScrSetPlayerWorldBounds,&g_rpcScrGivePlayerMoney,
        &g_rpcScrSetPlayerFacingAngle,&g_rpcScrResetPlayerMoney,&g_rpcScrResetPlayerWeapons,&g_rpcScrGivePlayerWeapon,
        &g_rpcUnknown0011830C,&g_rpcScrLinkVehicleToInterior,&g_rpcScrDeathMessage,&g_rpcScrAttachCameraToObject,
        &g_rpcScrInterpolateCamera,&g_textDrawClickRpc,&g_rpcScrSetObjectMaterial,&g_rpcScrStopObjectCameraCollision,
        &g_rpcScrSetActorAnimation,&g_rpcUnknown001182E8,&g_rpcScrSetActorRotation,&g_rpcScrSetActorPosition,
        &g_rpcScrSetActorHealth,&g_rpcUnknown001182D8,
    };
    for(unsigned i=0;i<sizeof(ids)/sizeof(ids[0]);i++)client->UnregisterAsRemoteProcedureCall(const_cast<int*>(ids[i]));
}
