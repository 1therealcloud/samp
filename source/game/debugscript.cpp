#include "game/debugscript.h"

#include "game/building_removal.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/vehicle.h"
#include "gui/chatwindow.h"
#include "main.h"
#include "net/objectpool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CVector g_debugScriptCameraPosition;

CObjectPool* g_debugScriptObjects;

WORD g_debugScriptObjectCount;

char* DebugScript::GetCommandParams(char* line)
{
    char* openingParenthesis = strchr(line, '(');
    char* closingParenthesis = strchr(line, ')');
    if (!openingParenthesis || !closingParenthesis)
        return NULL;

    size_t parameterLength = closingParenthesis - openingParenthesis - 1;
    memmove(line, openingParenthesis + 1, parameterLength);
    line[parameterLength] = 0;
    return line;
}

void DebugScript::CreateObject(const char* params)
{
    if (!params)
        return;
    char* context = nullptr;
    int model = atoi(strtok_s((char*)params, ",", &context));
    CVector position, rotation;
    position.x = (float)atof(strtok_s(nullptr, ",", &context));
    position.y = (float)atof(strtok_s(nullptr, ",", &context));
    position.z = (float)atof(strtok_s(nullptr, ",", &context));
    rotation.x = (float)atof(strtok_s(nullptr, ",", &context));
    rotation.y = (float)atof(strtok_s(nullptr, ",", &context));
    rotation.z = (float)atof(strtok_s(nullptr, ",", &context));
    if (g_debugScriptObjects)
    {
        g_debugScriptObjects->Create(g_debugScriptObjectCount, model, position, rotation, 599.0f);
        g_debugScriptObjectCount++;
    }
}

void DebugScript::CreateVehicle(const char* params)
{
    if (!params)
        return;
    char* context = nullptr;
    int model = atoi(strtok_s((char*)params, ",", &context));
    float x = (float)atof(strtok_s(nullptr, ",", &context)), y = (float)atof(strtok_s(nullptr, ",", &context)), z = (float)atof(strtok_s(nullptr, ",", &context));
    float rotation = (float)atof(strtok_s(nullptr, ",", &context));
    // Both colors are consumed by the native parser but never applied.
    atoi(strtok_s(nullptr, ",", &context));
    atoi(strtok_s(nullptr, ",", &context));
    new CVehicle(model, x, y, z, rotation, 0, FALSE);
}

void DebugScript::SetPlayerCameraPos(const char* params)
{
    if (!params)
        return;
    char* context = nullptr;
    strtok_s((char*)params, ",", &context);
    float x = (float)atof(strtok_s(nullptr, ",", &context)), y = (float)atof(strtok_s(nullptr, ",", &context));
    g_debugScriptCameraPosition.z = (float)atof(strtok_s(nullptr, ",", &context));
    g_debugScriptCameraPosition.x = x;
    g_debugScriptCameraPosition.y = y;
}

void DebugScript::RemoveBuildingForPlayer(const char* params)
{
    if (!params)
        return;
    char* context = nullptr;
    strtok_s((char*)params, ",", &context);
    int model = atoi(strtok_s(nullptr, ",", &context));
    float x = (float)atof(strtok_s(nullptr, ",", &context)), y = (float)atof(strtok_s(nullptr, ",", &context)), z = (float)atof(strtok_s(nullptr, ",", &context));
    float radius = (float)atof(strtok_s(nullptr, ",", &context));
    GameRemoveBuilding(model, x, y, z, radius);
}

void DebugScript::SetPlayerCameraLookAt(const char*) {}

void DebugScript::SetPlayerInterior(const char* params)
{
    if (!params)
        return;
    char* context = nullptr;
    strtok_s((char*)params, ",", &context);
    int interior = atoi(strtok_s(nullptr, ",", &context));
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
        ped->LinkToInterior((BYTE)interior, TRUE);
}

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

void DebugScript::Initialize(const char* filename)
{
    g_pGame->SetWorldTime(12, 0);
    g_pChat->AddDebugMessage("DEBUGSCRIPT: Loading %s", filename);
    g_debugScriptObjects = new CObjectPool;
    FILE* file = nullptr;
    fopen_s(&file, filename, "r");
    if (!file)
    {
        g_pChat->AddDebugMessage("DEBUGSCRIPT: I can't open %s", filename);
        return;
    }
    char line[MAX_PATH];
    // Native ignores fgets failure, including the first read of an empty file.
    while (!feof(file))
    {
        fgets(line, 256, file);
        ProcessLine(line);
    }
    fclose(file);
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
        ped->Teleport(g_debugScriptCameraPosition.x, g_debugScriptCameraPosition.y, g_debugScriptCameraPosition.z);
}
