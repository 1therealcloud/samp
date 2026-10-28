#ifndef SAMP_NETRPC_H
#define SAMP_NETRPC_H
class RakClientInterface;
struct RPCParameters;
// 0x00012310
void RegisterRPCs(RakClientInterface* client);
// 0x00011d80
void ScrCustomModelRecord(RPCParameters* parameters);
// 0x0000ea40
void ScrCustomModelsReset(RPCParameters* parameters);
// 0x0000ead0
void ScrCustomModelsComplete(RPCParameters* parameters);
// 0x00010530
void ScrInitGame(RPCParameters* parameters);
// Broader role of the fourth initialization DWORD is not established.
// 0x0013c0b4
extern unsigned long g_rpcInitValue13C0B4;
// 0x001180ec
extern const char g_rpcConnectedFormat[];
// 0x0000e4e0
void ScrGamemodeRestart(RPCParameters* parameters);
// 0x000109d0
void ScrWorldPlayerAdd(RPCParameters* parameters);
// 0x00118108
extern const char g_rpcCouldNotSpawnPlayer[];
// 0x0000e500
void ScrWorldVehicleAdd(RPCParameters* parameters);
// 0x00010dc0
void ScrWorldVehicleRemove(RPCParameters* parameters);
// 0x0000f150
void ScmEvent(RPCParameters* parameters);
// 0x0000e910
unsigned char ClientCheckChecksum(const void* data, unsigned short length);
// 0x00011860
void SendClientCheckResponse(unsigned char type, unsigned long argument, unsigned char result);
// 0x00011930
void ClientCheck(RPCParameters* parameters);
// 0x0000fea0
void RequestClass(RPCParameters* parameters);
// 0x0000ff70
void RequestSpawn(RPCParameters* parameters);
// 0x00010410
void ConnectionRejected(RPCParameters* parameters);
// 0x000114a0
void EnterVehicle(RPCParameters* parameters);
// 0x000115c0
void ExitVehicle(RPCParameters* parameters);
// 0x000121c0
void Chat(RPCParameters* parameters);
// 0x00010c20
void ScrWorldPlayerDeath(RPCParameters* parameters);
// 0x00010ce0
void ScrWorldPlayerRemove(RPCParameters* parameters);
// 0x001180c4
extern const char g_rpcRejectedVersion[];
// 0x00118098
extern const char g_rpcRejectedNickname[];
// 0x00118060
extern const char g_rpcNicknameLength[];
// 0x00118040
extern const char g_rpcNicknameCharacters[];
// 0x0011800c
extern const char g_rpcNicknameQuit[];
// 0x00117fe4
extern const char g_rpcRejectedMod[];
// 0x00117fac
extern const char g_rpcRejectedSlot[];
// 0x000102b0
void UpdateScoresPingsIps(RPCParameters* parameters);
// 0x0000e740
void EnterEditObject(RPCParameters* parameters);
// 0x0000e800
void ScrCancelEdit(RPCParameters* parameters);
// 0x0000e8b0
void ScrUpdateCameraTarget(RPCParameters* parameters);
// 0x000116b0
void EditAttachedObject(RPCParameters* parameters);
// 0x00011770
void EditObject(RPCParameters* parameters);
// 0x0000e940
void ScrCreateActor(RPCParameters* parameters);
// 0x00011cd0
void ScrDestroyActor(RPCParameters* parameters);
// 0x00011090
void DamageVehicle(RPCParameters* parameters);
// 0x000111c0
void ScrSetVehicleParamsEx(RPCParameters* parameters);
// 0x0000f4b0
void ScrSetPlayerSkillLevel(RPCParameters* parameters);
// 0x0000f5e0
void ScrCreate3DTextLabel(RPCParameters* parameters);
// 0x0000f790
void ScrDestroy3DTextLabel(RPCParameters* parameters);
// 0x0000f850
void ScrChatBubble(RPCParameters* parameters);
// 0x0000f9c0
void ScrShowDialog(RPCParameters* parameters);
// 0x0000ee60
void ScrClientMessage(RPCParameters* parameters);
// 0x0000fbb0
void ScrServerJoin(RPCParameters* parameters);
// 0x0000fd40
void ScrServerQuit(RPCParameters* parameters);
// 0x0000e450
void SrvNetStats(RPCParameters* parameters);
// 0x00010040
void ScrSetCheckpoint(RPCParameters* parameters);
// 0x00010150
void ScrSetRaceCheckpoint(RPCParameters* parameters);
// 0x0000ef50
void ScrCreatePickup(RPCParameters* parameters);
// 0x0000f010
void ScrDestroyPickup(RPCParameters* parameters);
// 0x0000f0b0
void ScrDestroyWeaponPickup(RPCParameters* parameters);
// 0x0000edc0
void ScrSetWorldTime(RPCParameters* parameters);
// 0x0000f240
void ScrSetWeather(RPCParameters* parameters);
// 0x0000f2f0
void ScrSetPlayerTime(RPCParameters* parameters);
// 0x0000f3d0
void ScrToggleClock(RPCParameters* parameters);
// 0x0000fe00
void ScrSetIngameTimer(RPCParameters* parameters);
// 0x0000e430
void ScrDisableCheckpoint(RPCParameters* parameters);
// 0x0000e440
void ScrDisableRaceCheckpoint(RPCParameters* parameters);
// 0x0000eb60
void UnRegisterRPCs(RakClientInterface* client);
// 0x00011f80
void ScrCustomModelDownload(RPCParameters* parameters);
// 0x00117fa8
extern const int g_rpcScrSetIngameTimer;
// 0x00117fa4
extern const int g_rpcScrWorldPlayerAdd;
// 0x00117fa0
extern const int g_rpcScrWorldPlayerDeath;
// 0x00117f9c
extern const int g_rpcScrWorldPlayerRemove;
// 0x00117f98
extern const int g_rpcScrWorldVehicleAdd;
// 0x00117f94
extern const int g_rpcScrWorldVehicleRemove;
// 0x00117f90
extern const int g_rpcScrSetVehicleParamsEx;
// 0x00117968
extern const int g_rpcDamageVehicle;
// 0x00117f8c
extern const int g_rpcScrSetPlayerSkillLevel;
// 0x00117f88
extern const int g_rpcScrCreate3DTextLabel;
// 0x00117f84
extern const int g_rpcScrDestroy3DTextLabel;
// 0x00117f80
extern const int g_rpcScrChatBubble;
// 0x00117f7c
extern const int g_rpcScrShowDialog;
// 0x00117f78
extern const int g_rpcScrServerJoin;
// 0x00117f74
extern const int g_rpcScrServerQuit;
// 0x00117f70
extern const int g_rpcScrInitGame;
// 0x00117f6c
extern const int g_rpcScrSetCheckpoint;
// 0x00117f68
extern const int g_rpcScrDisableCheckpoint;
// 0x00117f64
extern const int g_rpcScrSetRaceCheckpoint;
// 0x00117f60
extern const int g_rpcScrDisableRaceCheckpoint;
// 0x00117f5c
extern const int g_rpcSrvNetStats;
// 0x00117f58
extern const int g_rpcScrGamemodeRestart;
// 0x00117f54
extern const int g_rpcConnectionRejected;
// 0x00117f50
extern const int g_rpcScrClientMessage;
// 0x00117f4c
extern const int g_rpcScrSetWorldTime;
// 0x00117f48
extern const int g_rpcScrCreatePickup;
// 0x00117f44
extern const int g_rpcScrDestroyPickup;
// 0x00117f40
extern const int g_rpcScrDestroyWeaponPickup;
// 0x00117f3c
extern const int g_rpcScrSetWeather;
// 0x00117f38
extern const int g_rpcScrSetPlayerTime;
// 0x00117f34
extern const int g_rpcScrToggleClock;
// 0x00117f24
extern const int g_rpcScrCancelEdit;
// 0x00117f20
extern const int g_rpcScrUpdateCameraTarget;
// 0x00117f1c
extern const int g_rpcClientCheck;
// 0x00117f18
extern const int g_rpcScrCreateActor;
// 0x00117f14
extern const int g_rpcScrDestroyActor;
// 0x00117f10
extern const int g_rpcUnknown00117F10;
// 0x00117f0c
extern const int g_rpcUnknown00117F0C;
// 0x00117f08
extern const int g_rpcUnknown00117F08;
// 0x00117f04
extern const int g_rpcUnknown00117F04;
#endif
