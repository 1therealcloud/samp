#include "client/commands.h"
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
