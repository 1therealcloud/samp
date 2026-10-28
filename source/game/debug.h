#ifndef SAMP_GAME_DEBUG_H
#define SAMP_GAME_DEBUG_H
#include <stdio.h>
#include <windows.h>
// Names follow the 0.2x task helper family; DL name lookup is disabled natively.
// The unused task parameter follows that reference; DL consumes no argument.
// 0x000b3cf0
const char* GameGetTaskName(void* task);
// 0x0011f56c
extern const char g_taskNoneName[5];
// 0x000b3ca0
DWORD GameGetTaskType(void* task);
// Validates the raw GTA vtable range before calling virtual slot +8.
// 0x000b3c50
void* GameGetTaskChild(void* task);
struct CVector;
struct KeyPad;
class CCamera;
class CVehicle;
// Descriptive recovery names for the native debug-state setters.
// 0x0009dee0
void GameSetDebugState(DWORD first, DWORD second, DWORD stage);
// 0x0009df00
void GameClearDebugState();
// 0x001518d4
extern DWORD g_debugSelectedVehicleModel;
// 0x0018dd48
extern KeyPad* g_debugVehicleKeys;
// 0x0018dd4c
extern CCamera* g_debugVehicleCamera;
// 0x0018de30
extern DWORD g_debugVehicleSelectionActive;
// 0x0018de34
extern CVehicle* g_debugSelectedVehicle;
// 0x0018ddec
extern DWORD g_debugSelectionFlagA;
// 0x0018ddf0
extern DWORD g_debugSelectionFlagB;
// 0x0011e530
extern const char g_debugVehicleSelectText[15];
// 0x0011c198
extern const char g_debugVehicleTypeFormat[8];
// 0x0009df10
void GameRebuildDebugVehicle();
// Historical public name; the body is the native vehicle selector.
// 0x0009df50
void GameDrawDebugEntities();
// 0x000b3a00
DWORD __stdcall GameCountUnusedModelSlots();
#endif

// Raw original debug export state; broader flag semantics remain unresolved.
// 0x0018dc20
extern BYTE g_nativeDebugFlag18DC20;
// 0x0018dd50
extern float g_debugHeightmapStartX;
// 0x0018dde4
extern float g_debugHeightmapStartY;
// 0x0018de18
extern DWORD g_debugHeightmapStage;
// 0x0018de28
extern DWORD g_debugHeightmapFlagCopy;
// 0x0018de2c
extern FILE* g_debugHeightmapFile;
// 0x0011e50c
extern const char g_debugHeightmapFilename[];
// Existing shared mode storage in custommodels.cpp.
// 0x00117d30
extern const char g_customDownloadWriteMode[];
// 0x0009dc80
float GameDebugGroundHeight(const CVector* position);
// 0x0009dcd0
void GameDebugWriteHeightmap();
// 0x0009dda0
void GameDebugOpenHeightmap();
// 0x0009d3a0
void __stdcall GameSetNativeDebugFlag18DC20(BYTE value);

#ifndef SAMP_GAME_DEBUG_PLACEMENT_H
#define SAMP_GAME_DEBUG_PLACEMENT_H
#pragma pack(push, 1)
struct GameDebugPlacementRecord
{
    float x, y, z, qx, qy, qz, qw;
    int model;
    DWORD unknown20;
    int raw24;
};
#pragma pack(pop)
// 0x0018dc1c
extern FILE* g_debugPlacementFile;
// 0x0011e3d8
extern const char g_debugPlacementFilename[];
// 0x0011e3a8
extern const char g_debugPlacementFormat[];
// 0x0011e340
extern const char g_debugEntityPlacementFormat[];
// 0x0011d7c0
extern const double g_debugEulerNegativeOne;
// 0x0011e36c
extern const float g_debugPlacementMaxDrawDistance;
// 0x0011e368
extern const float g_debugPlacementMaxZ;
// Raw GTA backend entity; not a SA-MP CEntity wrapper pointer.
// 0x0009ce60
void GameDebugWriteEntityPlacement(void* entity, FILE* file);
// 0x0009d100
int GameDebugWritePlacementRecord(const GameDebugPlacementRecord* record);

#endif

// 0x001518d0
extern DWORD g_debugHeightmapTileEnd;
// 0x0018de1c
extern DWORD g_debugHeightmapFinished;
// 0x0018de20
extern DWORD g_debugHeightmapTilePrepared;
// 0x0018de24
extern DWORD g_debugHeightmapWaitFrames;
// 0x0011e4d8
extern const char g_debugHeightmapBlockFormat[];
// 0x0011e524
extern const char g_debugHeightmapFinishedText[];
// 0x0009dba0
void GameDebugSelectHeightmapBlock(int index);
// 0x0009dc30
void GameDebugMoveToHeightmapBlock();
// 0x0009ddd0
void GameDebugProcessHeightmap();

// 0x002aae10
extern void* g_debugRasterImage;
// 0x002abe40
extern void* g_debugRasterSource;
// 0x002abe44
extern void* g_debugRasterResult;
// 0x002abe48
extern void* g_debugRasterAlternate;
// 0x002a9a40
extern DWORD g_debugRasterWord10;
// 0x0011e370
extern const char g_debugAllBuildingsFilename[];
// 0x0011f594
extern const char g_debugRasterMissingText[];
// 0x0011f5cc
extern const char g_debugRasterFirstFilename[];
// 0x0011f5a0
extern const char g_debugRasterSecondFilename[];
// 0x0011f5ac
extern const char g_debugRasterDimensionsFormat[];
// 0x0009cf80
void GameDebugExportWorldPlacements();
// 0x000b5610
void GameDebugBindRasterResult();
// 0x000b5690
void GameDebugWriteRasterImage(void* raster, const char* filename);
// 0x000b5700
void GameDebugDumpRasterImages();
