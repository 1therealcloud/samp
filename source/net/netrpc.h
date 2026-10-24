#ifndef SAMP_NETRPC_H
#define SAMP_NETRPC_H
class RakClientInterface;
struct RPCParameters;
// samp.dll RVA: 0x0000e940
void ScrCreateActor(RPCParameters *parameters);
// samp.dll RVA: 0x00011cd0
void ScrDestroyActor(RPCParameters *parameters);
// samp.dll RVA: 0x00011090
void DamageVehicle(RPCParameters *parameters);
// samp.dll RVA: 0x000111c0
void ScrSetVehicleParamsEx(RPCParameters *parameters);
// samp.dll RVA: 0x0000f4b0
void ScrSetPlayerSkillLevel(RPCParameters *parameters);
// samp.dll RVA: 0x0000f5e0
void ScrCreate3DTextLabel(RPCParameters *parameters);
// samp.dll RVA: 0x0000f790
void ScrDestroy3DTextLabel(RPCParameters *parameters);
// samp.dll RVA: 0x0000f850
void ScrChatBubble(RPCParameters *parameters);
// samp.dll RVA: 0x0000f9c0
void ScrShowDialog(RPCParameters *parameters);
// samp.dll RVA: 0x0000ee60
void ScrClientMessage(RPCParameters *parameters);
// samp.dll RVA: 0x0000fbb0
void ScrServerJoin(RPCParameters *parameters);
// samp.dll RVA: 0x0000fd40
void ScrServerQuit(RPCParameters *parameters);
// samp.dll RVA: 0x0000e450
void SrvNetStats(RPCParameters *parameters);
// samp.dll RVA: 0x00010040
void ScrSetCheckpoint(RPCParameters *parameters);
// samp.dll RVA: 0x00010150
void ScrSetRaceCheckpoint(RPCParameters *parameters);
// samp.dll RVA: 0x0000ef50
void ScrCreatePickup(RPCParameters *parameters);
// samp.dll RVA: 0x0000f010
void ScrDestroyPickup(RPCParameters *parameters);
// samp.dll RVA: 0x0000f0b0
void ScrDestroyWeaponPickup(RPCParameters *parameters);
// samp.dll RVA: 0x0000edc0
void ScrSetWorldTime(RPCParameters *parameters);
// samp.dll RVA: 0x0000f240
void ScrSetWeather(RPCParameters *parameters);
// samp.dll RVA: 0x0000f2f0
void ScrSetPlayerTime(RPCParameters *parameters);
// samp.dll RVA: 0x0000f3d0
void ScrToggleClock(RPCParameters *parameters);
// samp.dll RVA: 0x0000fe00
void ScrSetIngameTimer(RPCParameters *parameters);
// samp.dll RVA: 0x0000e430
void ScrDisableCheckpoint(RPCParameters *parameters);
// samp.dll RVA: 0x0000e440
void ScrDisableRaceCheckpoint(RPCParameters *parameters);
// samp.dll RVA: 0x0000eb60
void UnRegisterRPCs(RakClientInterface *client);
// samp.dll RVA: 0x00117fa8
extern const int g_rpcScrSetIngameTimer;
// samp.dll RVA: 0x00117fa4
extern const int g_rpcScrWorldPlayerAdd;
// samp.dll RVA: 0x00117fa0
extern const int g_rpcScrWorldPlayerDeath;
// samp.dll RVA: 0x00117f9c
extern const int g_rpcScrWorldPlayerRemove;
// samp.dll RVA: 0x00117f98
extern const int g_rpcScrWorldVehicleAdd;
// samp.dll RVA: 0x00117f94
extern const int g_rpcScrWorldVehicleRemove;
// samp.dll RVA: 0x00117f90
extern const int g_rpcScrSetVehicleParamsEx;
// samp.dll RVA: 0x00117968
extern const int g_rpcDamageVehicle;
// samp.dll RVA: 0x00117f8c
extern const int g_rpcScrSetPlayerSkillLevel;
// samp.dll RVA: 0x00117f88
extern const int g_rpcScrCreate3DTextLabel;
// samp.dll RVA: 0x00117f84
extern const int g_rpcScrDestroy3DTextLabel;
// samp.dll RVA: 0x00117f80
extern const int g_rpcScrChatBubble;
// samp.dll RVA: 0x00117f7c
extern const int g_rpcScrShowDialog;
// samp.dll RVA: 0x00117f78
extern const int g_rpcScrServerJoin;
// samp.dll RVA: 0x00117f74
extern const int g_rpcScrServerQuit;
// samp.dll RVA: 0x00117f70
extern const int g_rpcScrInitGame;
// samp.dll RVA: 0x00117f6c
extern const int g_rpcScrSetCheckpoint;
// samp.dll RVA: 0x00117f68
extern const int g_rpcScrDisableCheckpoint;
// samp.dll RVA: 0x00117f64
extern const int g_rpcScrSetRaceCheckpoint;
// samp.dll RVA: 0x00117f60
extern const int g_rpcScrDisableRaceCheckpoint;
// samp.dll RVA: 0x00117f5c
extern const int g_rpcSrvNetStats;
// samp.dll RVA: 0x00117f58
extern const int g_rpcScrGamemodeRestart;
// samp.dll RVA: 0x00117f54
extern const int g_rpcConnectionRejected;
// samp.dll RVA: 0x00117f50
extern const int g_rpcScrClientMessage;
// samp.dll RVA: 0x00117f4c
extern const int g_rpcScrSetWorldTime;
// samp.dll RVA: 0x00117f48
extern const int g_rpcScrCreatePickup;
// samp.dll RVA: 0x00117f44
extern const int g_rpcScrDestroyPickup;
// samp.dll RVA: 0x00117f40
extern const int g_rpcScrDestroyWeaponPickup;
// samp.dll RVA: 0x00117f3c
extern const int g_rpcScrSetWeather;
// samp.dll RVA: 0x00117f38
extern const int g_rpcScrSetPlayerTime;
// samp.dll RVA: 0x00117f34
extern const int g_rpcScrToggleClock;
// samp.dll RVA: 0x00117f24
extern const int g_rpcScrCancelEdit;
// samp.dll RVA: 0x00117f20
extern const int g_rpcScrUpdateCameraTarget;
// samp.dll RVA: 0x00117f1c
extern const int g_rpcClientCheck;
// samp.dll RVA: 0x00117f18
extern const int g_rpcScrCreateActor;
// samp.dll RVA: 0x00117f14
extern const int g_rpcScrDestroyActor;
// samp.dll RVA: 0x00117f10
extern const int g_rpcUnknown00117F10;
// samp.dll RVA: 0x00117f0c
extern const int g_rpcUnknown00117F0C;
// samp.dll RVA: 0x00117f08
extern const int g_rpcUnknown00117F08;
// samp.dll RVA: 0x00117f04
extern const int g_rpcUnknown00117F04;
#endif
