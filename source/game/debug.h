#ifndef SAMP_GAME_DEBUG_H
#define SAMP_GAME_DEBUG_H
#include <stdio.h>
#include <windows.h>
// Names follow the 0.2x task helper family; DL name lookup is disabled natively.
// The unused task parameter follows that reference; DL consumes no argument.
// R5 address to verify
const char* GameGetTaskName(void* task);
// R5 address to verify
extern const char g_taskNoneName[5];
// 0x000b40a0
DWORD GameGetTaskType(void* task);
// Validates the raw GTA vtable range before calling virtual slot +8.
// 0x000b4050
void* GameGetTaskChild(void* task);
struct CVector;
struct KeyPad;
class CCamera;
class CVehicle;
// 0x0009e0a0
void GameSetDebugState(DWORD first, DWORD second, DWORD stage);
// R5 address to verify
void GameClearDebugState();
// 0x001138ec
extern DWORD g_debugSelectedVehicleModel;
// R5 address to verify
extern KeyPad* g_debugVehicleKeys;
// R5 address to verify
extern CCamera* g_debugVehicleCamera;
// R5 address to verify
extern DWORD g_debugVehicleSelectionActive;
// 0x0014ff9c
extern CVehicle* g_debugSelectedVehicle;
// 0x0014ff54
extern DWORD g_debugSelectionFlagA;
// 0x0014ff58
extern DWORD g_debugSelectionFlagB;
// 0x000ec438
extern const char g_debugVehicleSelectText[15];
// 0x000ea170
extern const char g_debugVehicleTypeFormat[8];
// 0x0009e0d0
void GameRebuildDebugVehicle();
// Historical public name; the body is the native vehicle selector.
// 0x0009e110
void GameDrawDebugEntities();
// 0x000b3e00
DWORD __stdcall GameCountUnusedModelSlots();
#endif

// Raw original debug export state; broader flag semantics remain unresolved.
// R5 address to verify
extern BYTE g_nativeDebugFlag18DC20;
// R5 address to verify
extern float g_debugHeightmapStartX;
// R5 address to verify
extern float g_debugHeightmapStartY;
// R5 address to verify
extern DWORD g_debugHeightmapStage;
// R5 address to verify
extern DWORD g_debugHeightmapFlagCopy;
// R5 address to verify
extern FILE* g_debugHeightmapFile;
// 0x000ec414
extern const char g_debugHeightmapFilename[];
// Existing shared mode storage in custommodels.cpp.
// 0x000e5d34
extern const char g_customDownloadWriteMode[];
// 0x0009de40
float GameDebugGroundHeight(const CVector* position);
// 0x0009de90
void GameDebugWriteHeightmap();
// R5 address to verify
void GameDebugOpenHeightmap();
// R5 address to verify
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
// R5 address to verify
extern FILE* g_debugPlacementFile;
// R5 address to verify
extern const char g_debugPlacementFilename[];
// R5 address to verify
extern const char g_debugPlacementFormat[];
// R5 address to verify
extern const char g_debugEntityPlacementFormat[];
// 0x000eb798
extern const double g_debugEulerNegativeOne;
// R5 address to verify
extern const float g_debugPlacementMaxDrawDistance;
// R5 address to verify
extern const float g_debugPlacementMaxZ;
// Raw GTA backend entity; not a SA-MP CEntity wrapper pointer.
// R5 address to verify
void GameDebugWriteEntityPlacement(void* entity, FILE* file);
// R5 address to verify
int GameDebugWritePlacementRecord(const GameDebugPlacementRecord* record);

#endif

// R5 address to verify
extern DWORD g_debugHeightmapTileEnd;
// R5 address to verify
extern DWORD g_debugHeightmapFinished;
// R5 address to verify
extern DWORD g_debugHeightmapTilePrepared;
// R5 address to verify
extern DWORD g_debugHeightmapWaitFrames;
// 0x000ec3e0
extern const char g_debugHeightmapBlockFormat[];
// 0x000ec42c
extern const char g_debugHeightmapFinishedText[];
// R5 address to verify
void GameDebugSelectHeightmapBlock(int index);
// R5 address to verify
void GameDebugMoveToHeightmapBlock();
// R5 address to verify
void GameDebugProcessHeightmap();

// R5 address to verify
extern const char g_debugAllBuildingsFilename[];
// 0x000ed484
extern const char g_debugRasterMissingText[];
// 0x000ed4bc
extern const char g_debugRasterFirstFilename[];
// 0x000ed490
extern const char g_debugRasterSecondFilename[];
// 0x000ed49c
extern const char g_debugRasterDimensionsFormat[];
// R5 address to verify
void GameDebugExportWorldPlacements();
// R5 address to verify
void GameDebugBindRasterResult();
// 0x000b5b50
void GameDebugWriteRasterImage(void* raster, const char* filename);
// R5 address to verify
void GameDebugDumpRasterImages();
