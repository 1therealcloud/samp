#ifndef SAMP_GAME_DEBUG_H
#define SAMP_GAME_DEBUG_H
#include <windows.h>
#include <stdio.h>
struct CVector;
struct KeyPad;
class CCamera;
class CVehicle;
// Descriptive recovery names for the native debug-state setters.
// samp.dll RVA: 0x0009dee0
void GameSetDebugState(DWORD first,DWORD second,DWORD stage);
// samp.dll RVA: 0x0009df00
void GameClearDebugState();
// samp.dll RVA: 0x001518d4
extern DWORD g_debugSelectedVehicleModel;
// samp.dll RVA: 0x0018dd48
extern KeyPad *g_debugVehicleKeys;
// samp.dll RVA: 0x0018dd4c
extern CCamera *g_debugVehicleCamera;
// samp.dll RVA: 0x0018de30
extern DWORD g_debugVehicleSelectionActive;
// samp.dll RVA: 0x0018de34
extern CVehicle *g_debugSelectedVehicle;
// samp.dll RVA: 0x0018ddec
extern DWORD g_debugSelectionFlagA;
// samp.dll RVA: 0x0018ddf0
extern DWORD g_debugSelectionFlagB;
// samp.dll RVA: 0x0011e530
extern const char g_debugVehicleSelectText[15];
// samp.dll RVA: 0x0011c198
extern const char g_debugVehicleTypeFormat[8];
// samp.dll RVA: 0x0009df10
void GameRebuildDebugVehicle();
// Historical public name; the body is the native vehicle selector.
// samp.dll RVA: 0x0009df50
void GameDrawDebugEntities();
// samp.dll RVA: 0x000b3a00
DWORD __stdcall GameCountUnusedModelSlots();
#endif

// Raw original debug export state; broader flag semantics remain unresolved.
// samp.dll RVA: 0x0018dc20
extern BYTE g_nativeDebugFlag18DC20;
// samp.dll RVA: 0x0018dd50
extern float g_debugHeightmapStartX;
// samp.dll RVA: 0x0018dde4
extern float g_debugHeightmapStartY;
// samp.dll RVA: 0x0018de18
extern DWORD g_debugHeightmapStage;
// samp.dll RVA: 0x0018de28
extern DWORD g_debugHeightmapFlagCopy;
// samp.dll RVA: 0x0018de2c
extern FILE *g_debugHeightmapFile;
// samp.dll RVA: 0x0011e50c
extern const char g_debugHeightmapFilename[];
// Existing shared mode storage in custommodels.cpp.
// samp.dll RVA: 0x00117d30
extern const char g_customDownloadWriteMode[];
// samp.dll RVA: 0x0009dc80
float GameDebugGroundHeight(const CVector *position);
// samp.dll RVA: 0x0009dcd0
void GameDebugWriteHeightmap();
// samp.dll RVA: 0x0009dda0
void GameDebugOpenHeightmap();
// samp.dll RVA: 0x0009d3a0
void __stdcall GameSetNativeDebugFlag18DC20(BYTE value);

#ifndef SAMP_GAME_DEBUG_PLACEMENT_H
#define SAMP_GAME_DEBUG_PLACEMENT_H
#pragma pack(push,1)
struct GameDebugPlacementRecord {
    float x,y,z,qx,qy,qz,qw;
    int model;DWORD unknown20;int raw24;
};
#pragma pack(pop)
// samp.dll RVA: 0x0018dc1c
extern FILE *g_debugPlacementFile;
// samp.dll RVA: 0x0011e3d8
extern const char g_debugPlacementFilename[];
// samp.dll RVA: 0x0011e3a8
extern const char g_debugPlacementFormat[];
// samp.dll RVA: 0x0011e340
extern const char g_debugEntityPlacementFormat[];
// samp.dll RVA: 0x0011d7c0
extern const double g_debugEulerNegativeOne;
// samp.dll RVA: 0x0011e36c
extern const float g_debugPlacementMaxDrawDistance;
// samp.dll RVA: 0x0011e368
extern const float g_debugPlacementMaxZ;
// Raw GTA backend entity; not a SA-MP CEntity wrapper pointer.
// samp.dll RVA: 0x0009ce60
void GameDebugWriteEntityPlacement(void *entity,FILE *file);
// samp.dll RVA: 0x0009d100
int GameDebugWritePlacementRecord(const GameDebugPlacementRecord *record);

#endif

// samp.dll RVA: 0x001518d0
extern DWORD g_debugHeightmapTileEnd;
// samp.dll RVA: 0x0018de1c
extern DWORD g_debugHeightmapFinished;
// samp.dll RVA: 0x0018de20
extern DWORD g_debugHeightmapTilePrepared;
// samp.dll RVA: 0x0018de24
extern DWORD g_debugHeightmapWaitFrames;
// samp.dll RVA: 0x0011e4d8
extern const char g_debugHeightmapBlockFormat[];
// samp.dll RVA: 0x0011e524
extern const char g_debugHeightmapFinishedText[];
// samp.dll RVA: 0x0009dba0
void GameDebugSelectHeightmapBlock(int index);
// samp.dll RVA: 0x0009dc30
void GameDebugMoveToHeightmapBlock();
// samp.dll RVA: 0x0009ddd0
void GameDebugProcessHeightmap();
