#ifndef NETRPC_H
#define NETRPC_H

class RakClientInterface;
struct RPCParameters;
// 0x00012450
void RegisterRPCs(RakClientInterface* client);
// 0x00010660
void ScrInitGame(RPCParameters* parameters);
// 0x000fe0b4
extern unsigned long g_rpcInitValue13C0B4;
// 0x000e6060
extern const char g_rpcConnectedFormat[];
// 0x0000e650
void ScrGamemodeRestart(RPCParameters* parameters);
// 0x00010b00
void ScrWorldPlayerAdd(RPCParameters* parameters);
// 0x000e607c
extern const char g_rpcCouldNotSpawnPlayer[];
// 0x0000e670
void ScrWorldVehicleAdd(RPCParameters* parameters);
// 0x00010ee0
void ScrWorldVehicleRemove(RPCParameters* parameters);
// 0x0000f280
void ScmEvent(RPCParameters* parameters);
// 0x0000ea80
unsigned char ClientCheckChecksum(const void* data, unsigned short length);
// 0x00011990
void SendClientCheckResponse(unsigned char type, unsigned long argument, unsigned char result);
// 0x00011a60
void ClientCheck(RPCParameters* parameters);
// 0x0000ffd0
void RequestClass(RPCParameters* parameters);
// 0x000100a0
void RequestSpawn(RPCParameters* parameters);
// 0x00010540
void ConnectionRejected(RPCParameters* parameters);
// 0x000115d0
void EnterVehicle(RPCParameters* parameters);
// 0x000116f0
void ExitVehicle(RPCParameters* parameters);
// 0x000122f0
void Chat(RPCParameters* parameters);
// 0x00010d40
void ScrWorldPlayerDeath(RPCParameters* parameters);
// 0x00010e00
void ScrWorldPlayerRemove(RPCParameters* parameters);
// 0x000e6038
extern const char g_rpcRejectedVersion[];
// 0x000e600c
extern const char g_rpcRejectedNickname[];
// 0x000e5fd4
extern const char g_rpcNicknameLength[];
// 0x000e5fb4
extern const char g_rpcNicknameCharacters[];
// 0x000e5f80
extern const char g_rpcNicknameQuit[];
// 0x000e5f58
extern const char g_rpcRejectedMod[];
// 0x000e5f20
extern const char g_rpcRejectedSlot[];
// 0x000103e0
void UpdateScoresPingsIps(RPCParameters* parameters);
// 0x0000e8b0
void EnterEditObject(RPCParameters* parameters);
// 0x0000e970
void ScrCancelEdit(RPCParameters* parameters);
// 0x0000ea20
void ScrUpdateCameraTarget(RPCParameters* parameters);
// 0x000117e0
void EditAttachedObject(RPCParameters* parameters);
// 0x000118a0
void EditObject(RPCParameters* parameters);
// 0x0000eab0
void ScrCreateActor(RPCParameters* parameters);
// 0x00011e00
void ScrDestroyActor(RPCParameters* parameters);
// 0x000111c0
void DamageVehicle(RPCParameters* parameters);
// 0x000112f0
void ScrSetVehicleParamsEx(RPCParameters* parameters);
// 0x0000f5e0
void ScrSetPlayerSkillLevel(RPCParameters* parameters);
// 0x0000f720
void ScrCreate3DTextLabel(RPCParameters* parameters);
// 0x0000f8d0
void ScrDestroy3DTextLabel(RPCParameters* parameters);
// 0x0000f990
void ScrChatBubble(RPCParameters* parameters);
// 0x0000faf0
void ScrShowDialog(RPCParameters* parameters);
// 0x0000ef90
void ScrClientMessage(RPCParameters* parameters);
// 0x0000fce0
void ScrServerJoin(RPCParameters* parameters);
// 0x0000fe70
void ScrServerQuit(RPCParameters* parameters);
// 0x0000e5c0
void SrvNetStats(RPCParameters* parameters);
// 0x00010170
void ScrSetCheckpoint(RPCParameters* parameters);
// 0x00010280
void ScrSetRaceCheckpoint(RPCParameters* parameters);
// 0x0000f080
void ScrCreatePickup(RPCParameters* parameters);
// 0x0000f140
void ScrDestroyPickup(RPCParameters* parameters);
// 0x0000f1e0
void ScrDestroyWeaponPickup(RPCParameters* parameters);
// 0x0000eef0
void ScrSetWorldTime(RPCParameters* parameters);
// 0x0000f370
void ScrSetWeather(RPCParameters* parameters);
// 0x0000f420
void ScrSetPlayerTime(RPCParameters* parameters);
// 0x0000f500
void ScrToggleClock(RPCParameters* parameters);
// 0x0000ff30
void ScrSetIngameTimer(RPCParameters* parameters);
// 0x0000e5a0
void ScrDisableCheckpoint(RPCParameters* parameters);
// 0x0000e5b0
void ScrDisableRaceCheckpoint(RPCParameters* parameters);
// 0x0000ecc0
void UnRegisterRPCs(RakClientInterface* client);
// 0x000120b0
void ScrCustomModelDownload(RPCParameters* parameters);
// 0x000e5f1c
extern const int g_rpcScrSetIngameTimer;
// 0x000e5f18
extern const int g_rpcScrWorldPlayerAdd;
// 0x000e5f14
extern const int g_rpcScrWorldPlayerDeath;
// 0x000e5f10
extern const int g_rpcScrWorldPlayerRemove;
// 0x000e5f0c
extern const int g_rpcScrWorldVehicleAdd;
// 0x000e5f08
extern const int g_rpcScrWorldVehicleRemove;
// 0x000e5f04
extern const int g_rpcScrSetVehicleParamsEx;
// 0x000e58f8
extern const int g_rpcDamageVehicle;
// 0x000e5f00
extern const int g_rpcScrSetPlayerSkillLevel;
// 0x000e5efc
extern const int g_rpcScrCreate3DTextLabel;
// 0x000e5ef8
extern const int g_rpcScrDestroy3DTextLabel;
// 0x000e5ef4
extern const int g_rpcScrChatBubble;
// 0x000e5ef0
extern const int g_rpcScrShowDialog;
// 0x000e5eec
extern const int g_rpcScrServerJoin;
// 0x000e5ee8
extern const int g_rpcScrServerQuit;
// 0x000e5ee4
extern const int g_rpcScrInitGame;
// 0x000e5ee0
extern const int g_rpcScrSetCheckpoint;
// 0x000e5edc
extern const int g_rpcScrDisableCheckpoint;
// 0x000e5ed8
extern const int g_rpcScrSetRaceCheckpoint;
// 0x000e5ed4
extern const int g_rpcScrDisableRaceCheckpoint;
// 0x000e5ed0
extern const int g_rpcSrvNetStats;
// 0x000e5ecc
extern const int g_rpcScrGamemodeRestart;
// 0x000e5ec8
extern const int g_rpcConnectionRejected;
// 0x000e5ec4
extern const int g_rpcScrClientMessage;
// 0x000e5ec0
extern const int g_rpcScrSetWorldTime;
// 0x000e5ebc
extern const int g_rpcScrCreatePickup;
// 0x000e5eb8
extern const int g_rpcScrDestroyPickup;
// 0x000e5eb4
extern const int g_rpcScrDestroyWeaponPickup;
// 0x000e5eb0
extern const int g_rpcScrSetWeather;
// 0x000e5eac
extern const int g_rpcScrSetPlayerTime;
// 0x000e5ea8
extern const int g_rpcScrToggleClock;
// 0x000e5e98
extern const int g_rpcScrCancelEdit;
// 0x000e5e94
extern const int g_rpcScrUpdateCameraTarget;
// 0x000e5e90
extern const int g_rpcClientCheck;
// 0x000e5e8c
extern const int g_rpcScrCreateActor;
// 0x000e5e88
extern const int g_rpcScrDestroyActor;

#endif // NETRPC_H
