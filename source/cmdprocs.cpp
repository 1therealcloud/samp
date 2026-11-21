#include "cmdprocs.h"

#include "config.h"
#include "game/debug.h"
#include "game/hooks.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "game/vehicle.h"
#include "gui/chatwindow.h"
#include "gui/cmdwindow.h"
#include "gui/gui.h"
#include "gui/scoreboard.h"
#include "main.h"
#include "net/localplayer.h"

#include <BitStream.h>
#include <DXUT.h>
#include <DXUTgui.h>
#include <RakClientInterface.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// R5 address to verify
const Scripting::OpcodeInfo g_commandGetVehicleAngle = {0x174, "iv"};
// R5 address to verify
const Scripting::OpcodeInfo g_commandGetActorAngle = {0x172, "iv"};
// R5 address to verify
BYTE g_commandCameraTargetLabels;
// R5 address to verify
DWORD g_commandObjectLighting = 1;
// R5 address to verify
const Scripting::OpcodeInfo g_commandGetInterior = {0x77e, "v"};

// 0x00069110
void Commands::Setup()
{
    g_pInput->SetDefaultCommand(Default);
    g_pInput->AddCommand("quit", Quit);
    g_pInput->AddCommand("q", Quit);
    g_pInput->AddCommand("save", SavePosition);
    g_pInput->AddCommand("rs", SavePositionOnly);
    g_pInput->AddCommand("rcon", SendRconCommand);
    g_pInput->AddCommand("mem", PrintMemory);
    g_pInput->AddCommand("fpslimit", SetFrameLimiter);
    g_pInput->AddCommand("pagesize", SetChatPageSize);
    g_pInput->AddCommand("fontsize", SetChatFontSize);
    g_pInput->AddCommand("nametagstatus", DrawNameTagStatus);
    g_pInput->AddCommand("timestamp", DrawChatTimestamps);
    g_pInput->AddCommand("headmove", ToggleHeadMoves);
    g_pInput->AddCommand("hudscalefix", ToggleHudScaleFix);
    g_pInput->AddCommand("testdw", TestDeathWindow);
    if (g_clientSettings.debug)
    {
        g_pInput->AddCommand("vsel", DebugEnableVehicleSelection);
        g_pInput->AddCommand("v", DebugCreateVehicle);
        g_pInput->AddCommand("vehicle", DebugCreateVehicle);
        g_pInput->AddCommand("player_skin", DebugSetPlayerSkin);
        g_pInput->AddCommand("set_weather", DebugSetWorldWeather);
        g_pInput->AddCommand("set_time", DebugSetWorldTime);
    }
    g_pInput->AddCommand("interior", PrintCurrentInterior);
    g_pInput->AddCommand("togobjlight", ToggleObjectsLight);
    g_pInput->AddCommand("cmpstat", CmpStat);
    g_pInput->AddCommand("dl", ToggleDebugLabels);
    g_pInput->AddCommand("ctd", ToggleCameraTargetLabels);
    g_pInput->AddCommand("audiomsg", ToggleAudioStreamMessages);
    g_pInput->AddCommand("logurls", ToggleURLMessages);
}

// 0x00068500
void Commands::TestDeathWindow(char*)
{
    if (!g_deviceDeathWindow)
        return;
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage("Pooper", "Pooper333", 0xffffffff, 0xffffffff, 1);
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage("Pooper", "Pooper", 0xffffffff, 0xffffffff, 5);
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage("Pooper", "Pooper", 0xffffffff, 0xffffffff, 15);
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage("Pooper", "Pooper", 0xffffffff, 0xffffffff, 14);
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage("Pooper", "Pooper", 0xffffffff, 0xffffffff, 2);
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage(0, "PooperPooperPooper0001", 0xffffffff, 0xffffffff, 5);
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage(0, "Pooper", 0xffffffff, 0xffffffff, 255);
    ((CDeathWindow*)g_deviceDeathWindow)->AddMessage("Pooper", "PooperPooperPooper0001", 0xffffffff, 0xffffffff, 0);
}

// 0x00068d00
void Commands::DebugSetPlayerSkin(char* params)
{
    if (!g_clientSettings.debug)
        return;
    if (!strlen(params))
    {
        g_pChat->AddDebugMessage("Usage: player_skin (skin number).");
        return;
    }
    int model = atoi(params);
    if (!g_pGame->IsStarted())
        return;
    CPed* ped = g_pGame->GetPlayerPed();
    if (ped)
        ped->SetModelIndex(model);
}

// 0x00068d70
void Commands::DebugCreateVehicle(char* params)
{
    if (!g_clientSettings.debug)
        return;
    if (!strlen(params))
    {
        g_pChat->AddDebugMessage("Usage: /v (vehicle id).");
        return;
    }
    int model = atoi(params);
    if (!g_pGame->IsStarted())
    {
        g_pChat->AddDebugMessage("game not loaded.");
        return;
    }
    g_pGame->RequestModel(model, 2);
    g_pGame->LoadRequestedModels();
    CPed* ped = g_pGame->GetPlayerPed();
    if (!ped)
    {
        g_pChat->AddDebugMessage("I couldn't find the player actor.");
        return;
    }
    CMatrix matrix;
    ped->GetMatrix(&matrix);
    char type[9] = {0};
    sprintf(type, g_debugVehicleTypeFormat, model);
    CVector position = matrix.position;
    position.z += g_quaternionMatrixOne;
    position.y -= g_scoreboardHeaderInset;
    position.x -= g_scoreboardHeaderInset;
    g_pGame->CreateVehicle(model, position.x, position.y, position.z, 0, FALSE)->Add();
}

// 0x00068eb0
void Commands::DebugEnableVehicleSelection(char*)
{
    if (g_clientSettings.debug)
        GameSetDebugState(0, 0, 10);
}

// 0x00069030
void Commands::SendRconCommand(char* params)
{
    if (!params || !g_pNetGame)
        return;
    RakNet::BitStream stream;
    BYTE packet = 201;
    stream.WriteBits(&packet, 8, true);
    DWORD length = (DWORD)strlen(params);
    stream.WriteBits((const unsigned char*)&length, 32, true);
    stream.Write(params, (int)length);
    (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->Send(&stream, HIGH_PRIORITY, RELIABLE, 0);
}

// 0x00068a00
void Commands::SavePosition(char* params)
{
    CPed* ped = g_pGame->GetPlayerPed();
    char path[260];
    sprintf(path, "%s\\savedpositions.txt", ClientGetDataDirectory());
    FILE* file = fopen(path, "a");
    if (!file)
    {
        g_pChat->AddDebugMessage("I can't open the savepositions.txt file for append.");
        return;
    }
    float angle;
    if (ped->IsInVehicle())
    {
        BYTE* vehicle = (BYTE*)ped->GetVehicle();
        Scripting::ProcessCommand(&g_commandGetVehicleAngle, GamePoolVehicleGetIndex(vehicle), &angle);
        BYTE* matrix = *(BYTE**)(vehicle + 0x14);
        fprintf(file, "AddStaticVehicle(%u,%.4f,%.4f,%.4f,%.4f,%u,%u); // %s\n", (int)*(short*)(vehicle + 0x22), *(float*)(matrix + 0x30), *(float*)(matrix + 0x34),
                *(float*)(matrix + 0x38), angle, (unsigned)vehicle[0x434], (unsigned)vehicle[0x435], params);
        fclose(file);
        g_pChat->AddInfoMessage("-> InCar position saved");
    }
    else
    {
        BYTE* backend = (BYTE*)ped->m_pGamePed;
        Scripting::ProcessCommand(&g_commandGetActorAngle, ped->m_nHandle, &angle);
        BYTE* matrix = *(BYTE**)(backend + 0x14);
        double heading = angle, z = *(float*)(matrix + 0x38), y = *(float*)(matrix + 0x34), x = *(float*)(matrix + 0x30);
        int model = ped->GetModelIndex();
        fprintf(file, "AddPlayerClass(%u,%.4f,%.4f,%.4f,%.4f,0,0,0,0,0,0); // %s\n", model, x, y, z, heading, params);
        fclose(file);
        g_pChat->AddInfoMessage("-> OnFoot position saved");
    }
}

// 0x00068b80
void Commands::SavePositionOnly(char* params)
{
    CPed* ped = g_pGame->GetPlayerPed();
    float angle;
    if (ped->IsInVehicle())
    {
        FILE* file = fopen("rawvehicles.txt", "a");
        if (!file)
        {
            g_pChat->AddDebugMessage("I can't open the rawvehicles.txt file for append.");
            return;
        }
        BYTE* vehicle = (BYTE*)ped->GetVehicle();
        DWORD vehicleIndex = GamePoolVehicleGetIndex(vehicle);
        Scripting::ProcessCommand(&g_commandGetVehicleAngle, vehicleIndex, &angle);
        BYTE* matrix = *(BYTE**)(vehicle + 0x14);
        fprintf(file, "%u,%.4f,%.4f,%.4f,%.4f,%u,%u ; %s\n", (int)*(short*)(vehicle + 0x22), *(float*)(matrix + 0x30), *(float*)(matrix + 0x34), *(float*)(matrix + 0x38), angle,
                (unsigned)vehicle[0x434], (unsigned)vehicle[0x435], params);
        fclose(file);
        g_pChat->AddDebugMessage("-> InCar pos saved");
    }
    else
    {
        BYTE* backend = (BYTE*)ped->m_pGamePed;
        Scripting::ProcessCommand(&g_commandGetActorAngle, ped->m_nHandle, &angle);
        char path[260];
        sprintf(path, "%s\\rawpositions.txt", ClientGetDataDirectory());
        FILE* file = fopen(path, "a");
        if (!file)
        {
            g_pChat->AddDebugMessage("I can't open the rawvehicles.txt file for append.");
            return;
        }
        BYTE* matrix = *(BYTE**)(backend + 0x14);
        fprintf(file, "%.4f,%.4f,%.4f,%.4f ; %s\n", *(float*)(matrix + 0x30), *(float*)(matrix + 0x34), *(float*)(matrix + 0x38), angle, params);
        fclose(file);
        g_pChat->AddDebugMessage("-> OnFoot pos saved");
    }
}

// 0x000684e0
void Commands::Default(char* params)
{
    if (g_pNetGame)
    {
        BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
        BYTE* players = *(BYTE**)(pools + 8);
        (*(CLocalPlayer**)(players + 0x1e))->Chat(params);
    }
}

// 0x000685e0
void Commands::ToggleCameraTargetLabels(char*)
{
    g_commandCameraTargetLabels = g_commandCameraTargetLabels == 0;
}

// 0x000685f0
void Commands::SetChatPageSize(char* params)
{
    int size;
    if (!strlen(params) || (size = atoi(params)) < 10 || size > 20)
    {
        if (g_pChat)
            g_pChat->AddDebugMessage("pagesize [10-20] (lines)");
        return;
    }
    if (g_pChat)
        g_pChat->SetPageSize(size);
    if (g_pConfig)
        g_pConfig->WriteIntValue("pagesize", size, FALSE);
}

// 0x00068670
void Commands::SetChatFontSize(char* params)
{
    if (strlen(params))
    {
        int size = atoi(params);
        if (size >= -3 && size <= 5)
        {
            if (g_pConfig)
                g_pConfig->WriteIntValue("fontsize", size, FALSE);
            if (g_deviceFonts)
                g_deviceFonts->Reset();
            if (g_pChat)
            {
                g_pChat->OnResetDevice();
                g_pChat->UpdateScrollbar();
            }
            if (g_deviceDeathWindow)
                ((CDeathWindow*)g_deviceDeathWindow)->ResetFonts();
            ClientRefreshGuiFonts();
            return;
        }
    }
    if (g_pChat)
        g_pChat->AddDebugMessage("Valid fontsize: -3 to 5");
}

// 0x00068720
void Commands::DrawNameTagStatus(char*)
{
    if (g_pNetGame)
        ((CNetGame*)g_pNetGame)->ToggleNameTagStatus();
}

// 0x00068730
void Commands::DrawChatTimestamps(char*)
{
    CChat* chat = g_pChat;
    if (chat)
    {
        if (chat->m_bTimestamps)
        {
            chat->m_bTimestamps = false;
            chat->m_bRedraw = TRUE;
            if (g_pConfig)
                g_pConfig->WriteIntValue("timestamp", 0, FALSE);
        }
        else
        {
            chat->m_bTimestamps = true;
            chat->m_bRedraw = TRUE;
            if (g_pConfig)
                g_pConfig->WriteIntValue("timestamp", 1, FALSE);
        }
    }
}

// 0x00068790
void Commands::ToggleAudioStreamMessages(char*)
{
    if (g_pConfig->GetIntValue("audiomsgoff") == 1)
    {
        g_pConfig->WriteIntValue("audiomsgoff", 0, FALSE);
        if (g_pChat)
            g_pChat->AddInfoMessage("Audio messages: On");
    }
    else
    {
        g_pConfig->WriteIntValue("audiomsgoff", 1, FALSE);
        if (g_pChat)
            g_pChat->AddInfoMessage("Audio messages: Off");
    }
}

// 0x00068800
void Commands::ToggleURLMessages(char*)
{
    if (g_pConfig->GetIntValue("logurls") == 1)
    {
        g_pConfig->WriteIntValue("logurls", 0, FALSE);
        if (g_pChat)
            g_pChat->AddInfoMessage("URL messages: Off");
    }
    else
    {
        g_pConfig->WriteIntValue("logurls", 1, FALSE);
        if (g_pChat)
            g_pChat->AddInfoMessage("URL messages: On");
    }
}

// R5 address to verify
void Commands::ToggleHudScaleFix(char*)
{
    g_hookHudAspectFix = g_hookHudAspectFix == 0;
}

// 0x000688b0
void Commands::PrintMemory(char*)
{
    // GTA SA virtual address: 0x008a5a80.
    g_pChat->AddDebugMessage("Memory: %u", *(DWORD*)0x8a5a80);
}

// 0x000688d0
void Commands::SetFrameLimiter(char* params)
{
    if (strlen(params))
    {
        DWORD value = atoi(params);
        if (value >= 20 && value <= 90)
        {
            g_pGame->SetFrameLimiter(value);
            if (g_pConfig)
                g_pConfig->WriteIntValue("fpslimit", value, FALSE);
            g_pChat->AddInfoMessage("-> Frame Limiter: %u", value);
        }
        else
            g_pChat->AddInfoMessage("-> Frame Limiter: valid amounts are 20-90", value);
    }
}

// 0x00068960
void Commands::ToggleHeadMoves(char*)
{
    if (g_pGame)
    {
        if (g_pGame->m_bHeadMove)
        {
            g_pGame->m_bHeadMove = FALSE;
            if (g_pConfig)
                g_pConfig->WriteIntValue("disableheadmove", 1, FALSE);
            g_pChat->AddInfoMessage("-> Head movements disabled");
        }
        else
        {
            g_pGame->m_bHeadMove = TRUE;
            if (g_pConfig)
                g_pConfig->WriteIntValue("disableheadmove", 0, FALSE);
            g_pChat->AddInfoMessage("-> Head movements enabled");
        }
    }
}

// 0x000689e0
void Commands::Quit(char*)
{
    ClientRequestQuit();
}

// Native empty callback, not a substitute for missing behavior.
// 0x000689f0
void Commands::CmpStat(char*) {}

// 0x00068ed0
void Commands::DebugSetWorldWeather(char* params)
{
    if (!strlen(params))
    {
        g_pChat->AddDebugMessage("Usage: set_weather (weather number)");
        return;
    }
    g_pGame->SetWorldWeather(atoi(params));
}

// 0x00068f20
void Commands::DebugSetWorldTime(char* params)
{
    if (!strlen(params))
    {
        g_pChat->AddDebugMessage("Usage: set_time (hour) (minute)");
        return;
    }
    int hour = 0, minute = 0;
    sscanf(params, "%d%d", &hour, &minute);
    if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59)
    {
        g_pGame->SetWorldTime(hour, minute);
        return;
    }
    g_pChat->AddDebugMessage("Invalid Time. Use /set_time (hour 0-23) (minute 0-59)");
}

// 0x00068fd0
void Commands::PrintCurrentInterior(char*)
{
    int interior;
    Scripting::ProcessCommand(&g_commandGetInterior, &interior);
    g_pChat->AddDebugMessage("Current Interior: %u", interior);
}

// 0x00069000
void Commands::ToggleObjectsLight(char*)
{
    g_commandObjectLighting = g_commandObjectLighting == 0;
}

// 0x00069020
void Commands::ToggleDebugLabels(char*)
{
    g_renderShowVehicleInfo = g_renderShowVehicleInfo == 0;
}
