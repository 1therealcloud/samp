#include "cmdprocs.h"
#include "main.h"
#include "cmdwindow.h"
// samp.dll RVA: 0x00068b50
void Commands::Setup(){
    g_pInput->SetDefaultCommand(Default);
    g_pInput->AddCommand("quit",Quit);
    g_pInput->AddCommand("q",Quit);
    g_pInput->AddCommand("save",SavePosition);
    g_pInput->AddCommand("rs",SavePositionOnly);
    g_pInput->AddCommand("rcon",SendRconCommand);
    g_pInput->AddCommand("mem",PrintMemory);
    g_pInput->AddCommand("fpslimit",SetFrameLimiter);
    g_pInput->AddCommand("pagesize",SetChatPageSize);
    g_pInput->AddCommand("fontsize",SetChatFontSize);
    g_pInput->AddCommand("nametagstatus",DrawNameTagStatus);
    g_pInput->AddCommand("timestamp",DrawChatTimestamps);
    g_pInput->AddCommand("headmove",ToggleHeadMoves);
    g_pInput->AddCommand("hudscalefix",ToggleHudScaleFix);
    g_pInput->AddCommand("testdw",TestDeathWindow);
    if(g_clientSettings.debug){
        g_pInput->AddCommand("vsel",DebugEnableVehicleSelection);
        g_pInput->AddCommand("v",DebugCreateVehicle);
        g_pInput->AddCommand("vehicle",DebugCreateVehicle);
        g_pInput->AddCommand("player_skin",DebugSetPlayerSkin);
        g_pInput->AddCommand("set_weather",DebugSetWorldWeather);
        g_pInput->AddCommand("set_time",DebugSetWorldTime);
    }
    g_pInput->AddCommand("interior",PrintCurrentInterior);
    g_pInput->AddCommand("togobjlight",ToggleObjectsLight);
    g_pInput->AddCommand("cmpstat",CmpStat);
    g_pInput->AddCommand("dl",ToggleDebugLabels);
    g_pInput->AddCommand("ctd",ToggleCameraTargetLabels);
    g_pInput->AddCommand("audiomsg",ToggleAudioStreamMessages);
    g_pInput->AddCommand("logurls",ToggleURLMessages);
}

#include "config.h"
#include "chatwindow.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "gui/gui.h"
#include "game/hooks_render.h"
#include "game/playerped.h"
#include "net/localplayer.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x00156ac8
BYTE g_commandCameraTargetLabels;
// samp.dll RVA: 0x00154698
DWORD g_commandObjectLighting=1;
// samp.dll RVA: 0x0011bde4
const Scripting::OpcodeInfo g_commandGetInterior={0x77e,"v"};
// samp.dll RVA: 0x00067f50
void Commands::Default(char *params){
    if(g_pNetGame){BYTE*pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);BYTE*players=*(BYTE**)(pools+8);(*(CLocalPlayer**)(players+0x1e))->Chat(params);}
}
// samp.dll RVA: 0x00068050
void Commands::ToggleCameraTargetLabels(char*){g_commandCameraTargetLabels=g_commandCameraTargetLabels==0;}
// samp.dll RVA: 0x00068060
void Commands::SetChatPageSize(char *params){
    int size;
    if(!strlen(params)||(size=atol(params))<10||size>20){if(g_pChat)g_pChat->AddDebugMessage("pagesize [10-20] (lines)");return;}
    if(g_pChat)g_pChat->SetPageSize(size);
    if(g_pConfig)g_pConfig->WriteIntValue("pagesize",size,FALSE);
}
// samp.dll RVA: 0x000680e0
void Commands::SetChatFontSize(char *params){
    if(strlen(params)){
        int size=atol(params);
        if(size>=-3&&size<=5){
            if(g_pConfig)g_pConfig->WriteIntValue("fontsize",size,FALSE);
            if(g_deviceFonts)g_deviceFonts->Reset();
            if(g_pChat){g_pChat->OnResetDevice();g_pChat->UpdateScrollbar();}
            if(g_deviceDeathWindow)((CDeathWindow*)g_deviceDeathWindow)->ResetFonts();
            // Native shared refresh tail begins at RVA 0x000c6af0.
            if(g_guiScoreboardDialog)g_guiScoreboardDialog->InitFont();
            if(g_renderFirstDialog)g_renderFirstDialog->InitFont();
            if(g_renderSecondDialog)g_renderSecondDialog->InitFont();
            if(g_guiServerDialog)g_guiServerDialog->InitFont();
            if(g_guiPanelDialog)g_guiPanelDialog->InitFont();
            if(g_guiDownloadDialog)g_guiDownloadDialog->InitFont();
            if(g_renderDownloadWindow)g_renderDownloadWindow->ResetFonts();
            return;
        }
    }
    if(g_pChat)g_pChat->AddDebugMessage("Valid fontsize: -3 to 5");
}
// samp.dll RVA: 0x00068190
void Commands::DrawNameTagStatus(char*){if(g_pNetGame)((CNetGame*)g_pNetGame)->ToggleNameTagStatus();}
// samp.dll RVA: 0x000681a0
void Commands::DrawChatTimestamps(char*){
    if(g_pChat){
        if(g_pChat->m_bTimestamps){g_pChat->m_bTimestamps=false;g_pChat->m_bRedraw=TRUE;if(g_pConfig)g_pConfig->WriteIntValue("timestamp",0,FALSE);}
        else{g_pChat->m_bTimestamps=true;g_pChat->m_bRedraw=TRUE;if(g_pConfig)g_pConfig->WriteIntValue("timestamp",1,FALSE);}
    }
}
// samp.dll RVA: 0x00068200
void Commands::ToggleAudioStreamMessages(char*){
    if(g_pConfig->GetIntValue("audiomsgoff")==1){g_pConfig->WriteIntValue("audiomsgoff",0,FALSE);if(g_pChat)g_pChat->AddInfoMessage("Audio messages: On");}
    else{g_pConfig->WriteIntValue("audiomsgoff",1,FALSE);if(g_pChat)g_pChat->AddInfoMessage("Audio messages: Off");}
}
// samp.dll RVA: 0x00068270
void Commands::ToggleURLMessages(char*){
    if(g_pConfig->GetIntValue("logurls")==1){g_pConfig->WriteIntValue("logurls",0,FALSE);if(g_pChat)g_pChat->AddInfoMessage("URL messages: Off");}
    else{g_pConfig->WriteIntValue("logurls",1,FALSE);if(g_pChat)g_pChat->AddInfoMessage("URL messages: On");}
}
// samp.dll RVA: 0x000682e0
void Commands::ToggleHudScaleFix(char*){g_hookHudAspectFix=g_hookHudAspectFix==0;}
// samp.dll RVA: 0x000682f0
void Commands::PrintMemory(char*){
    // GTA SA virtual address: 0x008a5a80.
    g_pChat->AddDebugMessage("Memory: %u",*(DWORD*)0x8a5a80);
}
// samp.dll RVA: 0x00068310
void Commands::SetFrameLimiter(char *params){
    if(strlen(params)){
        DWORD value=atol(params);
        if(value>=20&&value<=90){g_pGame->SetFrameLimiter(value);if(g_pConfig)g_pConfig->WriteIntValue("fpslimit",value,FALSE);g_pChat->AddInfoMessage("-> Frame Limiter: %u",value);}
        else g_pChat->AddInfoMessage("-> Frame Limiter: valid amounts are 20-90",value);
    }
}
// samp.dll RVA: 0x000683a0
void Commands::ToggleHeadMoves(char*){
    if(g_pGame){
        if(g_pGame->m_bHeadMove){g_pGame->m_bHeadMove=FALSE;if(g_pConfig)g_pConfig->WriteIntValue("disableheadmove",1,FALSE);g_pChat->AddInfoMessage("-> Head movements disabled");}
        else{g_pGame->m_bHeadMove=TRUE;if(g_pConfig)g_pConfig->WriteIntValue("disableheadmove",0,FALSE);g_pChat->AddInfoMessage("-> Head movements enabled");}
    }
}
// samp.dll RVA: 0x00068420
void Commands::Quit(char*){ClientRequestQuit();}
// Native empty callback, not a substitute for missing behavior.
// samp.dll RVA: 0x00068430
void Commands::CmpStat(char*){}
// samp.dll RVA: 0x00068910
void Commands::DebugSetWorldWeather(char *params){
    if(!strlen(params)){g_pChat->AddDebugMessage("Usage: set_weather (weather number)");return;}
    g_pGame->SetWorldWeather(atol(params));
}
// samp.dll RVA: 0x00068960
void Commands::DebugSetWorldTime(char *params){
    if(!strlen(params)){g_pChat->AddDebugMessage("Usage: set_time (hour) (minute)");return;}
    int hour=0,minute=0;sscanf(params,"%d%d",&hour,&minute);
    if(hour>=0&&hour<24&&minute>=0&&minute<60){g_pGame->SetWorldTime(hour,minute);return;}
    g_pChat->AddDebugMessage("Invalid Time. Use /set_time (hour 0-23) (minute 0-59)");
}
// samp.dll RVA: 0x00068a10
void Commands::PrintCurrentInterior(char*){int interior;Scripting::ProcessCommand(&g_commandGetInterior,&interior);g_pChat->AddDebugMessage("Current Interior: %u",interior);}
// samp.dll RVA: 0x00068a40
void Commands::ToggleObjectsLight(char*){g_commandObjectLighting=g_commandObjectLighting==0;}
// samp.dll RVA: 0x00068a60
void Commands::ToggleDebugLabels(char*){g_renderShowVehicleInfo=g_renderShowVehicleInfo==0;}
