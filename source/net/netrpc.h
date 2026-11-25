#ifndef NETRPC_H
#define NETRPC_H

class RakClientInterface;
struct RPCParameters;

void RegisterRPCs(RakClientInterface* client);

void ScrCustomModelRecord(RPCParameters* parameters);

void ScrCustomModelsReset(RPCParameters* parameters);

void ScrCustomModelsComplete(RPCParameters* parameters);

void ScrInitGame(RPCParameters* parameters);

extern unsigned long g_rpcInitValue13C0B4;

extern const char g_rpcConnectedFormat[];

void ScrGamemodeRestart(RPCParameters* parameters);

void ScrWorldPlayerAdd(RPCParameters* parameters);

extern const char g_rpcCouldNotSpawnPlayer[];

void ScrWorldVehicleAdd(RPCParameters* parameters);

void ScrWorldVehicleRemove(RPCParameters* parameters);

void ScmEvent(RPCParameters* parameters);

unsigned char ClientCheckChecksum(const void* data, unsigned short length);

void SendClientCheckResponse(unsigned char type, unsigned long argument, unsigned char result);

void ClientCheck(RPCParameters* parameters);

void RequestClass(RPCParameters* parameters);

void RequestSpawn(RPCParameters* parameters);

void ConnectionRejected(RPCParameters* parameters);

void EnterVehicle(RPCParameters* parameters);

void ExitVehicle(RPCParameters* parameters);

void Chat(RPCParameters* parameters);

void ScrWorldPlayerDeath(RPCParameters* parameters);

void ScrWorldPlayerRemove(RPCParameters* parameters);

extern const char g_rpcRejectedVersion[];

extern const char g_rpcRejectedNickname[];

extern const char g_rpcNicknameLength[];

extern const char g_rpcNicknameCharacters[];

extern const char g_rpcNicknameQuit[];

extern const char g_rpcRejectedMod[];

extern const char g_rpcRejectedSlot[];

void UpdateScoresPingsIps(RPCParameters* parameters);

void EnterEditObject(RPCParameters* parameters);

void ScrCancelEdit(RPCParameters* parameters);

void ScrUpdateCameraTarget(RPCParameters* parameters);

void EditAttachedObject(RPCParameters* parameters);

void EditObject(RPCParameters* parameters);

void ScrCreateActor(RPCParameters* parameters);

void ScrDestroyActor(RPCParameters* parameters);

void DamageVehicle(RPCParameters* parameters);

void ScrSetVehicleParamsEx(RPCParameters* parameters);

void ScrSetPlayerSkillLevel(RPCParameters* parameters);

void ScrCreate3DTextLabel(RPCParameters* parameters);

void ScrDestroy3DTextLabel(RPCParameters* parameters);

void ScrChatBubble(RPCParameters* parameters);

void ScrShowDialog(RPCParameters* parameters);

void ScrClientMessage(RPCParameters* parameters);

void ScrServerJoin(RPCParameters* parameters);

void ScrServerQuit(RPCParameters* parameters);

void SrvNetStats(RPCParameters* parameters);

void ScrSetCheckpoint(RPCParameters* parameters);

void ScrSetRaceCheckpoint(RPCParameters* parameters);

void ScrCreatePickup(RPCParameters* parameters);

void ScrDestroyPickup(RPCParameters* parameters);

void ScrDestroyWeaponPickup(RPCParameters* parameters);

void ScrSetWorldTime(RPCParameters* parameters);

void ScrSetWeather(RPCParameters* parameters);

void ScrSetPlayerTime(RPCParameters* parameters);

void ScrToggleClock(RPCParameters* parameters);

void ScrSetIngameTimer(RPCParameters* parameters);

void ScrDisableCheckpoint(RPCParameters* parameters);

void ScrDisableRaceCheckpoint(RPCParameters* parameters);

void UnRegisterRPCs(RakClientInterface* client);

void ScrCustomModelDownload(RPCParameters* parameters);

extern const int g_rpcScrSetIngameTimer;

extern const int g_rpcScrWorldPlayerAdd;

extern const int g_rpcScrWorldPlayerDeath;

extern const int g_rpcScrWorldPlayerRemove;

extern const int g_rpcScrWorldVehicleAdd;

extern const int g_rpcScrWorldVehicleRemove;

extern const int g_rpcScrSetVehicleParamsEx;

extern const int g_rpcDamageVehicle;

extern const int g_rpcScrSetPlayerSkillLevel;

extern const int g_rpcScrCreate3DTextLabel;

extern const int g_rpcScrDestroy3DTextLabel;

extern const int g_rpcScrChatBubble;

extern const int g_rpcScrShowDialog;

extern const int g_rpcScrServerJoin;

extern const int g_rpcScrServerQuit;

extern const int g_rpcScrInitGame;

extern const int g_rpcScrSetCheckpoint;

extern const int g_rpcScrDisableCheckpoint;

extern const int g_rpcScrSetRaceCheckpoint;

extern const int g_rpcScrDisableRaceCheckpoint;

extern const int g_rpcSrvNetStats;

extern const int g_rpcScrGamemodeRestart;

extern const int g_rpcConnectionRejected;

extern const int g_rpcScrClientMessage;

extern const int g_rpcScrSetWorldTime;

extern const int g_rpcScrCreatePickup;

extern const int g_rpcScrDestroyPickup;

extern const int g_rpcScrDestroyWeaponPickup;

extern const int g_rpcScrSetWeather;

extern const int g_rpcScrSetPlayerTime;

extern const int g_rpcScrToggleClock;

extern const int g_rpcScrCancelEdit;

extern const int g_rpcScrUpdateCameraTarget;

extern const int g_rpcClientCheck;

extern const int g_rpcScrCreateActor;

extern const int g_rpcScrDestroyActor;

// custommodels

extern const int g_rpcScrModelInfo;

extern const int g_rpcScrCustomModelsReset;

extern const int g_rpcScrModelFile;

extern const int g_rpcScrFinishedDownloading;

#endif // NETRPC_H
