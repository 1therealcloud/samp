#include "game/debug.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "game/game.h"
#include "game/vehicle.h"
#include "game/playerped.h"
#include "game/keystuff.h"
#include "gui/render_dependencies.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x0011f56c
const char g_taskNoneName[5]="None";
// Native returns this constant even for a null task; no task dereference.
// samp.dll RVA: 0x000b3cf0
const char *GameGetTaskName(void *task) {(void)task;return g_taskNoneName;}
// samp.dll RVA: 0x000b3ca0
DWORD GameGetTaskType(void *task) {
    DWORD result=0;
    if(task) {
        DWORD table=*(DWORD*)task;
        if(table>=0x800000 && table<=0x900000) {
            __asm {
                pushad
                mov ecx,task
                mov edx,[ecx]
                call dword ptr [edx+0x10]
                mov result,eax
                popad
            }
        }
    }
    return result;
}
// samp.dll RVA: 0x000b3c50
void *GameGetTaskChild(void *task) {
    void *result=0;
    if(task) {
        DWORD table=*(DWORD*)task;
        if(table>=0x800000&&table<=0x900000) {
            // GTA SA virtual interface: preserve caller registers across the call.
            __asm {
                pushad
                mov ecx,task
                mov edx,[ecx]
                call dword ptr [edx+8]
                mov result,eax
                popad
            }
        }
    }
    return result;
}
// samp.dll RVA: 0x001518d4
DWORD g_debugSelectedVehicleModel=400;
// samp.dll RVA: 0x0018dd48
KeyPad *g_debugVehicleKeys;
// samp.dll RVA: 0x0018dd4c
CCamera *g_debugVehicleCamera;
// samp.dll RVA: 0x0018de30
DWORD g_debugVehicleSelectionActive;
// samp.dll RVA: 0x0018de34
CVehicle *g_debugSelectedVehicle;
// samp.dll RVA: 0x0018ddec
DWORD g_debugSelectionFlagA;
// samp.dll RVA: 0x0018ddf0
DWORD g_debugSelectionFlagB;
// samp.dll RVA: 0x0011e530
const char g_debugVehicleSelectText[15]="Vehicle Select";
// samp.dll RVA: 0x0011c198
const char g_debugVehicleTypeFormat[8]="TYPE_%d";
// samp.dll RVA: 0x0009dee0
void GameSetDebugState(DWORD first,DWORD second,DWORD stage) {
    g_gameDebugStage=stage;g_debugSelectionFlagA=first;g_debugSelectionFlagB=second;
}
// samp.dll RVA: 0x0009df00
void GameClearDebugState() {g_gameDebugStage=0;}

// samp.dll RVA: 0x0009df10
void GameRebuildDebugVehicle() {
    if(g_debugSelectedVehicle)delete g_debugSelectedVehicle;
    g_debugSelectedVehicle=g_pGame->CreateVehicle(g_debugSelectedVehicleModel,5,5,500,0,FALSE);
}
// samp.dll RVA: 0x0009df50
void GameDrawDebugEntities() {
    if(!g_debugVehicleSelectionActive) {
        g_debugVehicleKeys=KeyStuff::GetInternalKeys();
        g_debugVehicleCamera=g_pGame->m_pCamera;
        CVector position={-4,-4,502},rotation={0,0,0};
        g_debugVehicleCamera->Set(position,rotation);
        CVector point={5,5,500};g_debugVehicleCamera->PointAt(point,1);
        g_pGame->GetPlayerPed()->SetControllable(FALSE);
        g_pGame->DisplayGameText(g_debugVehicleSelectText,4000,6);
        GameRebuildDebugVehicle();
        g_debugVehicleSelectionActive=1;
    }
    g_pGame->EnableHUD(FALSE);
    CVehicle *preview=g_debugSelectedVehicle;
    if(preview&&preview->m_pGameEntity) {
        CVector turn={0,0,0.03f},speed={0,0,0};
        preview->SetTurnSpeed(turn);
        g_debugSelectedVehicle->SetSpeed(speed);
        CMatrix matrix;g_debugSelectedVehicle->GetMatrix(&matrix);
        CVector position={5,5,500};memcpy(&matrix.position,&position,12);
        g_debugSelectedVehicle->SetMatrix(matrix);
        preview=g_debugSelectedVehicle;
    }
    WORD *keys=(WORD*)g_debugVehicleKeys;
    if(keys[0x1c/2]&&!keys[0x4c/2]) {
        --g_debugSelectedVehicleModel;
        if(g_debugSelectedVehicleModel==538)g_debugSelectedVehicleModel=536;
        else if(g_debugSelectedVehicleModel==399)g_debugSelectedVehicleModel=611;
        GameRebuildDebugVehicle();return;
    }
    if(keys[0x20/2]&&!keys[0x50/2]) {
        ++g_debugSelectedVehicleModel;
        if(g_debugSelectedVehicleModel==537)g_debugSelectedVehicleModel=539;
        else if(g_debugSelectedVehicleModel==612)g_debugSelectedVehicleModel=400;
        GameRebuildDebugVehicle();return;
    }
    if(keys[0x1e/2]&&!keys[0x4e/2]) {
        if(preview)delete preview;
        g_debugSelectedVehicle=0;
        g_debugVehicleCamera->Restore();
        g_gameDebugStage=0;g_debugSelectionFlagA=0;g_debugSelectionFlagB=0;
        CPed *ped=g_pGame->GetPlayerPed();
        if(ped) {
            CMatrix matrix;ped->GetMatrix(&matrix);
            // Native still formats this unused temporary model label.
            char type[9]={0};sprintf(type,g_debugVehicleTypeFormat,g_debugSelectedVehicleModel);
            CVehicle *created=g_pGame->CreateVehicle(g_debugSelectedVehicleModel,
                matrix.position.x-5.0f,matrix.position.y-5.0f,matrix.position.z+1.0f,0,FALSE);
            if(created)ped->PutIntoVehicle(created->m_nHandle,0);
        }
        g_debugVehicleCamera->Detach();g_debugVehicleCamera->Restore();
        g_pGame->GetPlayerPed()->SetControllable(TRUE);
        g_pGame->EnableHUD(TRUE);g_debugVehicleSelectionActive=0;
    }
}
// samp.dll RVA: 0x000b3a00
DWORD __stdcall GameCountUnusedModelSlots() {
    DWORD count=0;
    // GTA SA virtual addresses: native model slots1000..19999.
    for(DWORD address=0xa9c068;address<0xaae948;address+=4)
        if(!*(DWORD*)address)++count;
    return count;
}

// samp.dll RVA: 0x0018dc20
BYTE g_nativeDebugFlag18DC20=0;
// samp.dll RVA: 0x0018dd50
float g_debugHeightmapStartX=0;
// samp.dll RVA: 0x0018dde4
float g_debugHeightmapStartY=0;
// samp.dll RVA: 0x0018de18
DWORD g_debugHeightmapStage=0;
// samp.dll RVA: 0x0018de28
DWORD g_debugHeightmapFlagCopy=0;
// samp.dll RVA: 0x0018de2c
FILE *g_debugHeightmapFile=0;
// samp.dll RVA: 0x0011e50c
const char g_debugHeightmapFilename[]="c:\\blocks\\SA2_bos.hmap";
// samp.dll RVA: 0x0009d3a0
void __stdcall GameSetNativeDebugFlag18DC20(BYTE value) {g_nativeDebugFlag18DC20=value;}
// Ignores GTA result and returns raw collision-position Z, even on failure.
// samp.dll RVA: 0x0009dc80
float GameDebugGroundHeight(const CVector *position) {
    CVector end;memcpy(&end,position,8);end.z=-100.0f;
    BYTE collision[52];void *entity=(void*)position;
    GameProcessLineOfSightRaw(position,&end,collision,&entity,1,0,0,1,1,1,0,0);
    return *(float*)(collision+8);
}
// samp.dll RVA: 0x0009dda0
void GameDebugOpenHeightmap() {
    g_debugHeightmapFile=fopen(g_debugHeightmapFilename,g_customDownloadWriteMode);
    g_debugHeightmapFlagCopy=g_debugSelectionFlagA;
    g_debugHeightmapStage=1;
}
// samp.dll RVA: 0x0009dcd0
void GameDebugWriteHeightmap() {
    DWORD firstX;memcpy(&firstX,&g_debugHeightmapStartX,4);
    if(!g_debugHeightmapFile)return;
    CVector start;memcpy(&start.y,&g_debugHeightmapStartY,4);start.z=530.0f;
    for(int row=0;row<200;++row) {
        __asm {
            fld dword ptr firstX
            fstp dword ptr start
        }
        for(int column=0;column<200;++column) {
            CVector end;end.x=start.x;memcpy(&end.y,&start.y,4);end.z=-100.0f;
            BYTE collision[52];void *entity;
            GameProcessLineOfSightRaw(&start,&end,collision,&entity,1,0,0,1,1,1,0,0);
            DWORD height;memcpy(&height,collision+8,4);
            fwrite(&height,1,4,g_debugHeightmapFile);
            start.x+=1.0f;
        }
        start.y-=1.0f;
    }
}

// samp.dll RVA: 0x0018dc1c
FILE *g_debugPlacementFile=0;
// samp.dll RVA: 0x0011e3d8
const char g_debugPlacementFilename[]="c:\\0\\lc2sa_LoadObjectInstance_1_noipllods_Zonly_Q2E.txt";
// samp.dll RVA: 0x0011e3a8
const char g_debugPlacementFormat[]="%d,%.7f,%.7f,%.7f,%.5f,%.5f,%.5f,%.2f,%u,%X\n";
// samp.dll RVA: 0x0011e340
const char g_debugEntityPlacementFormat[]="%u,%.5f,%.5f,%.5f,0.0,0.0,%.5f,%.5f\n";
// samp.dll RVA: 0x0011d7c0
const double g_debugEulerNegativeOne=-1.0;
// samp.dll RVA: 0x0011e36c
const float g_debugPlacementMaxDrawDistance=299.0f;
// samp.dll RVA: 0x0011e368
const float g_debugPlacementMaxZ=500.0f;
// samp.dll RVA: 0x0009ce60
void GameDebugWriteEntityPlacement(void *entity,FILE *file) {
    if(!entity)return;
    BYTE *backend=(BYTE*)entity;float heading;CVector position;
    void *matrix=*(void**)(backend+0x14);
    if(matrix) {
        __asm {
            mov ecx,matrix
            fld dword ptr [ecx+0x10]
            fchs
            fld dword ptr [ecx+0x14]
            fpatan
            fmul g_entityRadiansToDegrees
            fst heading
            fcomp g_objectScaleLowerBound
            fld heading
            fnstsw ax
            test ah,5
            jp nonnegative_heading
            fadd g_anglePeriod
            fstp heading
            jmp heading_done
        nonnegative_heading:
            fcomp g_anglePeriod
            fnstsw ax
            test ah,1
            jne heading_done
            fld heading
            fsub g_anglePeriod
            fstp heading
        heading_done:
        }
        matrix=*(void**)(backend+0x14);
        memcpy(&position,(BYTE*)matrix+0x30,12);
    } else {
        __asm {
            mov eax,backend
            fld dword ptr [eax+0x10]
            fmul g_entityRadiansToDegrees
            fstp heading
        }
        memcpy(&position,backend+4,12);
    }
    void *info=GameGetModelInfo(*(short*)(backend+0x22));
    float distance;BYTE withinDistance;
    // Native compares ST0 before the stored float distance is reloaded for output.
    __asm {
        push info
        call GameGetModelDrawDistance
        fst distance
        fcomp g_debugPlacementMaxDrawDistance
        fnstsw ax
        test ah,0x41
        setnp withinDistance
    }
    if(withinDistance&&position.z<g_debugPlacementMaxZ)
        fprintf(file,g_debugEntityPlacementFormat,(DWORD)(int)*(short*)(backend+0x22),
            (double)position.x,(double)position.y,(double)position.z,(double)heading,(double)distance);
}
// samp.dll RVA: 0x0009d100
int GameDebugWritePlacementRecord(const GameDebugPlacementRecord *record) {
    if(!g_debugPlacementFile) {
        // Native mode address11BBD4 shares its bytes with RPC integer119 ("w").
        g_debugPlacementFile=fopen(g_debugPlacementFilename,(const char*)&g_renderMapClickRpc);
        if(!g_debugPlacementFile)return 0;
    }
    double roll,pitch,yaw;
    GameQuaternionDoubleToEuler(record->qx,record->qy,record->qz,record->qw,&roll,&pitch,&yaw);
    float heading;
    __asm {
        fld yaw
        fmul g_debugEulerNegativeOne
        fstp heading
    }
    float distance=GameGetModelDrawDistance(GameGetModelInfo(record->model));
    WORD flags=GameGetModelFlagsWord(record->model);
    DWORD nonnegative=record->raw24>=0?1:0;
    double angle;
    __asm {
        fld heading
        fmul g_entityRadiansToDegrees
        fstp angle
    }
    fprintf(g_debugPlacementFile,g_debugPlacementFormat,record->model,
        (double)record->x,(double)record->y,(double)record->z,g_pedParachuteZero,g_pedParachuteZero,
        angle,(double)distance,nonnegative,(DWORD)flags);
    return 0;
}

#include "chatwindow.h"
#include "game/hooks_prerender.h"
// samp.dll RVA: 0x001518d0
DWORD g_debugHeightmapTileEnd=900;
// samp.dll RVA: 0x0018de1c
DWORD g_debugHeightmapFinished=0;
// samp.dll RVA: 0x0018de20
DWORD g_debugHeightmapTilePrepared=0;
// samp.dll RVA: 0x0018de24
DWORD g_debugHeightmapWaitFrames=0;
// samp.dll RVA: 0x0011e4d8
const char g_debugHeightmapBlockFormat[]="HM Setup Block: %d Row/Col: %d,%d XY: %.2f,%.2f";
// samp.dll RVA: 0x0011e524
const char g_debugHeightmapFinishedText[]="Finished HM";
// samp.dll RVA: 0x0009dba0
void GameDebugSelectHeightmapBlock(int index) {
    int row=index>29?index/30:0;
    int column=(int)((DWORD)index-(DWORD)row*30);
    int rawX=(int)((DWORD)column*200),rawY=(int)((DWORD)row*200);
    double displayY;
    // Native prints Y before a float reload, but X after its float spill.
    __asm {
        fild rawX
        fsub g_hookCollisionBoundHigh
        fstp g_debugHeightmapStartX
        fild rawY
        fsubr g_hookCollisionBoundHigh
        fst g_debugHeightmapStartY
        fstp displayY
    }
    g_pChat->AddDebugMessage(g_debugHeightmapBlockFormat,index,row,column,
        (double)g_debugHeightmapStartX,displayY);
}
// samp.dll RVA: 0x0009dc30
void GameDebugMoveToHeightmapBlock() {
    GameDebugSelectHeightmapBlock((int)g_debugHeightmapFlagCopy);
    CPed *ped=g_pGame->GetPlayerPed();
    CVector position;position.z=540.0f;
    __asm {
        fld g_debugHeightmapStartY
        fsub g_vehicleTrainSpeedMaximum
        fstp position.y
        fld g_debugHeightmapStartX
        fadd g_vehicleTrainSpeedMaximum
        fstp position.x
    }
    ped->Teleport(position);
}
// samp.dll RVA: 0x0009ddd0
void GameDebugProcessHeightmap() {
    if(!g_debugHeightmapStage) {
        GameDebugOpenHeightmap();
        return;
    }
    if(g_debugHeightmapFinished) {
        // Native leaves stage, completion flag and the closed FILE pointer intact.
        g_pChat->AddDebugMessage(g_debugHeightmapFinishedText);
        fclose(g_debugHeightmapFile);
        CPed *ped=g_pGame->GetPlayerPed();
        ped->SetCollisionProcessed(TRUE);ped->SetCollisionFlag(TRUE);
        return;
    }
    CPed *ped=g_pGame->GetPlayerPed();
    ped->SetCollisionProcessed(FALSE);ped->SetCollisionFlag(FALSE);
    if(!g_debugHeightmapTilePrepared) {
        GameDebugMoveToHeightmapBlock();
        g_debugHeightmapTilePrepared=1;
        g_debugHeightmapWaitFrames=110;
        return;
    }
    if(!g_debugHeightmapWaitFrames) {
        GameDebugWriteHeightmap();
        DWORD next=g_debugHeightmapFlagCopy+1;
        g_debugHeightmapTilePrepared=0;
        g_debugHeightmapFlagCopy=next;
        if(next==g_debugHeightmapTileEnd)g_debugHeightmapFinished=1;
        return;
    }
    --g_debugHeightmapWaitFrames;
}

#include "game/hooks_render.h"
#pragma bss_seg(".pstate$g")
// samp.dll RVA: 0x002aae10
void *g_debugRasterImage;
#pragma bss_seg()
// samp.dll RVA: 0x002abe40
void *g_debugRasterSource;
// samp.dll RVA: 0x002abe44
void *g_debugRasterResult;
// samp.dll RVA: 0x002abe48
void *g_debugRasterAlternate;
// samp.dll RVA: 0x002a9a40
DWORD g_debugRasterWord10;
// samp.dll RVA: 0x0011e370
const char g_debugAllBuildingsFilename[]="c:\\0\\AllBuildingsLC2SA_1_raw_nolod_noint.txt";
// samp.dll RVA: 0x0011f594
const char g_debugRasterMissingText[]="No texture!";
// samp.dll RVA: 0x0011f5cc
const char g_debugRasterFirstFilename[]="Raster1.bmp";
// samp.dll RVA: 0x0011f5a0
const char g_debugRasterSecondFilename[]="Raster2.bmp";
// samp.dll RVA: 0x0011f5ac
const char g_debugRasterDimensionsFormat[]="Width %u Height: %u Depth: %u";
// samp.dll RVA: 0x0009cf80
void GameDebugExportWorldPlacements() {
    FILE *file=fopen(g_debugAllBuildingsFilename,(const char*)&g_renderMapClickRpc);
    if(!file)return;
    BYTE *cursor=(BYTE*)GamePoolBuildingGetArray();
    for(int count=20000;count;--count,cursor+=56)
        if(*(DWORD*)cursor&&*(DWORD*)cursor!=0x863c40)GameDebugWriteEntityPlacement(cursor,file);
    cursor=(BYTE*)GamePoolDummyGetArray();
    for(int count=16000;count;--count,cursor+=56)
        if(*(DWORD*)cursor&&*(DWORD*)cursor!=0x863c40)GameDebugWriteEntityPlacement(cursor,file);
    cursor=(BYTE*)GamePoolObjectGetArray();
    for(int count=3100;count;--count,cursor+=412)
        if(*(DWORD*)cursor&&*(DWORD*)cursor!=0x863c40)GameDebugWriteEntityPlacement(cursor,file);
    fclose(file);
}
// samp.dll RVA: 0x000b5610
void GameDebugBindRasterResult() {
    g_debugRasterSource=*(void**)0xc402d8;
    g_debugRasterResult=((void*(__cdecl*)(void*))0x7f37c0)(g_debugRasterSource);
    if(!g_debugRasterResult)g_pChat->AddDebugMessage(g_debugRasterMissingText);
    // Native live result reloads: raw GTA globals, including publication of NULL.
    *(void**)0xb4e690=g_debugRasterResult;*(void**)0xb4e68c=g_debugRasterResult;
    *(void**)0xb4e47c=g_debugRasterResult;*(void**)0xb4e3ec=g_debugRasterResult;
    *(DWORD*)0x8a7780=0x40000000;
}
// samp.dll RVA: 0x000b5690
void GameDebugWriteRasterImage(void *raster,const char *filename) {
    // Observed argument order from native snapshot globals; APIs remain unchecked.
    g_debugRasterImage=((void*(__cdecl*)(DWORD,DWORD,DWORD))0x8026e0)(
        g_gamePlayerTableUnknown2A9D98[1],g_debugRasterWord10,g_gamePlayerTableUnknown2A9D98[0]);
    ((void*(__cdecl*)(void*))0x8027a0)(g_debugRasterImage);
    ((void*(__cdecl*)(void*,void*))0x804250)(g_debugRasterImage,raster);
    ((void*(__cdecl*)(void*,const char*))0x7ce990)(g_debugRasterImage,filename);
    ((DWORD(__cdecl*)(void*))0x802740)(g_debugRasterImage);
}
// samp.dll RVA: 0x000b5700
void GameDebugDumpRasterImages() {
    g_debugRasterSource=*(void**)0xc402d8;
    BYTE *raster=(BYTE*)g_debugRasterSource;
    // Native dereferences the first pointer BEFORE checking it for null.
    g_gamePlayerTableUnknown2A9D98[0]=*(DWORD*)(raster+0x14);
    g_debugRasterWord10=*(DWORD*)(raster+0x10);
    g_gamePlayerTableUnknown2A9D98[1]=*(DWORD*)(raster+0xc);
    if(g_debugRasterSource) {
        GameDebugWriteRasterImage(g_debugRasterSource,g_debugRasterFirstFilename);
        g_pChat->AddDebugMessage(g_debugRasterDimensionsFormat,g_gamePlayerTableUnknown2A9D98[1],
            g_debugRasterWord10,g_gamePlayerTableUnknown2A9D98[0]);
    }
    if(g_debugRasterAlternate) {
        raster=(BYTE*)g_debugRasterAlternate;
        g_gamePlayerTableUnknown2A9D98[0]=*(DWORD*)(raster+0x14);
        g_debugRasterWord10=*(DWORD*)(raster+0x10);
        g_gamePlayerTableUnknown2A9D98[1]=*(DWORD*)(raster+0xc);
        if(g_debugRasterAlternate) {
            GameDebugWriteRasterImage(g_debugRasterAlternate,g_debugRasterSecondFilename);
            g_pChat->AddDebugMessage(g_debugRasterDimensionsFormat,g_gamePlayerTableUnknown2A9D98[1],
                g_debugRasterWord10,g_gamePlayerTableUnknown2A9D98[0]);
        }
    }
}
