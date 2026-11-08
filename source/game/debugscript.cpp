#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game/debugscript.h"

#include "chatwindow.h"
#include "game/building_removal.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/vehicle.h"
#include "main.h"
#include "net/objectpool.h"
// 0x0018de38
CVector g_debugScriptCameraPosition;
// 0x0018de44
CObjectPool* g_debugScriptObjects;
// 0x0018de48
WORD g_debugScriptObjectCount;

// 0x0009e2f0
char* DebugScript::GetCommandParams(char* line)
{
    char *first = strchr(line, '('), *last = strchr(line, ')');
    if (!first || !last)
        return 0;
    int distance = last - first;
    strncpy(line, first + 1, distance - 1);
    line[distance] = 0;
    return line;
}
// 0x0009e340
void DebugScript::CreateObject(const char* params)
{
    if (!params)
        return;
    int model = atoi(strtok((char*)params, ","));
    CVector position, rotation;
    position.x = (float)atof(strtok(0, ","));
    position.y = (float)atof(strtok(0, ","));
    position.z = (float)atof(strtok(0, ","));
    rotation.x = (float)atof(strtok(0, ","));
    rotation.y = (float)atof(strtok(0, ","));
    rotation.z = (float)atof(strtok(0, ","));
    if (g_debugScriptObjects)
    {
        g_debugScriptObjects->Create(g_debugScriptObjectCount, model, position, rotation, 599.0f);
        g_debugScriptObjectCount++;
    }
}
// 0x0009e450
void DebugScript::CreateVehicle(const char* params)
{
    if (!params)
        return;
    int model = atoi(strtok((char*)params, ","));
    float x = (float)atof(strtok(0, ",")), y = (float)atof(strtok(0, ",")), z = (float)atof(strtok(0, ","));
    float rotation = (float)atof(strtok(0, ","));
    // Both colors are consumed by the native parser but never applied.
    atoi(strtok(0, ","));
    atoi(strtok(0, ","));
    new CVehicle(model, x, y, z, rotation, 0, FALSE);
}
// 0x0009e560
void DebugScript::SetPlayerCameraPos(const char* params)
{
    if (!params)
        return;
    strtok((char*)params, ",");
    float x = (float)atof(strtok(0, ",")), y = (float)atof(strtok(0, ","));
    g_debugScriptCameraPosition.z = (float)atof(strtok(0, ","));
    g_debugScriptCameraPosition.x = x;
    g_debugScriptCameraPosition.y = y;
}
// 0x0009e5d0
void DebugScript::RemoveBuildingForPlayer(const char* params)
{
    if (!params)
        return;
    strtok((char*)params, ",");
    int model = atoi(strtok(0, ","));
    float x = (float)atof(strtok(0, ",")), y = (float)atof(strtok(0, ",")), z = (float)atof(strtok(0, ","));
    float radius = (float)atof(strtok(0, ","));
    GameRemoveBuilding(model, x, y, z, radius);
}
// 0x0009e680
void DebugScript::SetPlayerCameraLookAt(const char*) {}
// 0x0009e690
void DebugScript::SetPlayerInterior(const char* params)
{
    if (!params)
        return;
    strtok((char*)params, ",");
    int interior = atoi(strtok(0, ","));
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
        ped->LinkToInterior((BYTE)interior, TRUE);
}
// 0x0009e6e0
void DebugScript::ProcessLine(const char* line)
{
    if (!strncmp(line, "CreateObject", 12))
        CreateObject(GetCommandParams((char*)line));
    else if (!strncmp(line, "CreateVehicle", 13) || !strncmp(line, "AddStaticVehicle", 16))
        CreateVehicle(GetCommandParams((char*)line));
    else if (!strncmp(line, "SetPlayerCameraPos", 18))
        SetPlayerCameraPos(GetCommandParams((char*)line));
    else if (!strncmp(line, "SetPlayerCameraLookAt", 21))
        SetPlayerCameraLookAt(GetCommandParams((char*)line));
    else if (!strncmp(line, "RemoveBuildingForPlayer", 23))
        RemoveBuildingForPlayer(GetCommandParams((char*)line));
    else if (!strncmp(line, "SetPlayerInterior", 17))
        SetPlayerInterior(GetCommandParams((char*)line));
}
// 0x0009e7f0
void DebugScript::Initialize(const char* filename)
{
    g_pGame->SetWorldTime(12, 0);
    g_pChat->AddDebugMessage("DEBUGSCRIPT: Loading %s", filename);
    g_debugScriptObjects = new CObjectPool;
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        g_pChat->AddDebugMessage("DEBUGSCRIPT: I can't open %s", filename);
        return;
    }
    char line[256];
    // Native ignores fgets failure, including the first read of an empty file.
    while (!feof(file))
    {
        fgets(line, 256, file);
        ProcessLine(line);
    }
    fclose(file);
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
        ped->Teleport(g_debugScriptCameraPosition);
}
