#include "console.h"
#include "filterscripts.h"
#include "gamemodes.h"
#include "netgame.h"
#include "plugins.h"
#include "query.h"

#include <signal.h>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _WIN32
#include <sys/time.h>
#include <unistd.h>
#endif

// 0x004fc43c (Windows), 0x081d793c (Linux)
CConsole* pConsole;

// 0x004ebcec
static int iOnFootRate = 30;
// 0x004ebcf0
static int iInCarRate = 30;
// 0x004ebcf4
static int iWeaponRate = 30;
// 0x004ebcfc
static float fStreamDistance = 200.0f;
// 0x004ebd00
static int iStreamRate = 1000;
// 0x004ebd04
static int iSleep = 5;
// 0x004ebd08
static int iChatLogging = 1;
// 0x004ebd0c
static int iMessageHoleLimit = 3000;
// 0x004ebd10
static int iMessagesLimit = 500;
// 0x004ebd14
static int iAcksLimit = 3000;
// 0x004ebd18
static int iPlayerTimeout = 10000;
// 0x004ebd1c
static int iLagCompMode = 1;
// 0x004ebd20
static int iMtu = 576;
// 0x004ebd24
static int iConnSeedTime = 300000;
// 0x004ebd28
static int iConnCookies = 1;

// 0x004fc430 (Windows), 0x081b7d50 (Linux)
static FILE* pLogFile;
// 0x004fc458
static bool bLogQueries;
// 0x004fc45c
static int iMaxNpc;
// 0x004fc460
static int iMinConnectionTime;
// 0x004fc464
static int iDbLogging;
// 0x004fc468
static int iDbLogQueries;
// 0x004fc46c
static bool bUseArtwork;
// 0x004fc47c
static int iCookieLogging;

static volatile sig_atomic_t bQuitApp;

void RequestServerExit()
{
    bQuitApp = 1;
}

#ifdef _WIN32
static BOOL WINAPI HandleConsoleEvent(DWORD event)
{
    if (event != CTRL_C_EVENT && event != CTRL_BREAK_EVENT)
        return FALSE;
    RequestServerExit();
    return TRUE;
}
#else
static void HandleSignal(int)
{
    RequestServerExit();
}
#endif

static unsigned int GetMilliseconds()
{
#ifdef _WIN32
    return GetTickCount();
#else
    struct timeval now;
    gettimeofday(&now, NULL);
    return static_cast<unsigned int>(now.tv_sec) * 1000U + now.tv_usec / 1000U;
#endif
}

static void RunScripts()
{
    const char* modeSetting = pConsole->GetStringVariable("gamemode0");
    std::istringstream modeEntry(modeSetting ? modeSetting : "");
    std::string modeName;
    if (!(modeEntry >> modeName))
    {
        logprintf("Error: It needs a gamemode0 line at the very least.");
        return;
    }

#ifdef _WIN32
    SetConsoleCtrlHandler(HandleConsoleEvent, TRUE);
#else
    signal(SIGINT, HandleSignal);
    signal(SIGTERM, HandleSignal);
#endif

    {
        CFilterScripts filters;
        CGameMode game;
        CPlugins plugins;
        CNetGame netGame;
        if (!netGame.Start())
            return;

        pFilterScripts = &filters;
        pGameMode = &game;
        pPlugins = &plugins;

        logprintf("Server Plugins");
        logprintf("--------------");
        logprintf(" Loaded %u plugins.", plugins.LoadPlugins("plugins"));

        filters.LoadFilterScripts(pConsole->GetStringVariable("filterscripts"));
        const std::string path = "gamemodes/" + modeName + ".amx";
        if (game.Load(path.c_str()))
        {
            unsigned int previous = GetMilliseconds();
            while (!bQuitApp)
            {
                netGame.Process();
                const unsigned int now = GetMilliseconds();
                game.Frame(static_cast<float>(now - previous) / 1000.0f);
                previous = now;
                plugins.DoProcessTick();
                const unsigned int delay = iSleep > 0 ? static_cast<unsigned int>(iSleep) : 0;
#ifdef _WIN32
                Sleep(delay);
#else
                usleep(delay * 1000U);
#endif
            }
        }

        game.Unload();
        filters.UnloadFilterScripts();
    }
    pPlugins = NULL;
    pGameMode = NULL;
    pFilterScripts = NULL;
}

// 0x00491fa0 (Windows), 0x080b1ca0 (Linux)
void logprintf(const char* format, ...)
{
    char text[2048];
    va_list args;
    va_start(args, format);
#ifdef _WIN32
    _vsnprintf(text, sizeof(text) - 1, format, args);
#else
    vsnprintf(text, sizeof(text), format, args);
#endif
    va_end(args);
    text[sizeof(text) - 1] = '\0';

#ifdef _WIN32
    char consoleText[2048];
    CharToOemA(text, consoleText);
    puts(consoleText);
    fflush(stdout);
#else
    if (pConsole && pConsole->GetBoolVariable("output"))
        puts(text);
#endif

    if (pLogFile)
    {
        if (pConsole && pConsole->GetBoolVariable("timestamp"))
        {
            char timeText[256] = {0};
            time_t now = time(NULL);
            const struct tm* local = localtime(&now);
            if (local)
                strftime(timeText, sizeof(timeText), pConsole->GetStringVariable("logtimeformat"), local);
            fprintf(pLogFile, "%s %s\n", timeText, text);
        }
        else
        {
            fprintf(pLogFile, "%s\n", text);
        }
        fflush(pLogFile);
    }
    if (bRconSocketReply)
        RconSocketReply(text);
}

// 0x00492380 (Windows), 0x080b1e40 (Linux)
void LoadLogFile()
{
    const bool reload = pLogFile != NULL;
    if (reload)
        fclose(pLogFile);

    pLogFile = fopen("server_log.txt", "a");
    if (pLogFile)
    {
        logprintf("");
        logprintf("----------");
        if (reload)
            logprintf("Reloaded log file: \"server_log.txt\".");
        else
            logprintf("Loaded log file: \"server_log.txt\".");
        logprintf("----------");
    }
    else
    {
        logprintf("Failed to load log file: \"server_log.txt\".");
    }
}

// 0x00491e50
void ServerMaxPlayersChanged()
{
    const int maxPlayers = pConsole->GetIntVariable("maxplayers");
    if (maxPlayers < 0)
        pConsole->SetIntVariable("maxplayers", 0);
    if (maxPlayers > 1000)
        pConsole->SetIntVariable("maxplayers", 1000);
}

// 0x00491ef0
void ServerStreamRateChanged()
{
    const int rate = pConsole->GetIntVariable("stream_rate");
    if (rate < 500)
        pConsole->SetIntVariable("stream_rate", 500);
    if (rate > 5000)
        pConsole->SetIntVariable("stream_rate", 5000);
}

// 0x00491f40
void ServerStreamDistanceChanged()
{
    const float distance = pConsole->GetFloatVariable("stream_distance");
    if (distance < 50.0f)
        pConsole->SetFloatVariable("stream_distance", 50.0f);
    if (distance > 400.0f)
        pConsole->SetFloatVariable("stream_distance", 400.0f);
}

// 0x00492420 (Windows), 0x080b1ef0 (Linux)
int main(int argc, char** argv)
{
    bool bEnableAnnounce = true;
    int iMaxPlayers = 50;
    int iListenPort = 8192;
    bool bLanModeEnable = false;
    bool bAllowQuery = true;
    bool bEnableRcon = true;
    bool bEnableTimestamp = true;
    bool bGameMod = false;
#ifndef _WIN32
    bool bOutputEnable = false;
#endif

    LoadLogFile();
    logprintf("");
    logprintf("SA-MP Dedicated Server");
    logprintf("----------------------");
    logprintf("v0.3.DL-R1, (C)2005-2018 SA-MP Team\n");

    CConsole console;
    pConsole = &console;
    pConsole->AddVariable("announce", CON_VARTYPE_BOOL, 0, &bEnableAnnounce);
    pConsole->AddVariable("maxplayers", CON_VARTYPE_INT, 0, &iMaxPlayers, ServerMaxPlayersChanged);
    pConsole->AddVariable("mtu", CON_VARTYPE_INT, 0, &iMtu);
    pConsole->AddVariable("port", CON_VARTYPE_INT, 0, &iListenPort);
    pConsole->AddVariable("lanmode", CON_VARTYPE_BOOL, 0, &bLanModeEnable);
    pConsole->AddVariable("query", CON_VARTYPE_BOOL, 0, &bAllowQuery);
    pConsole->AddVariable("rcon", CON_VARTYPE_BOOL, 0, &bEnableRcon);
    pConsole->AddVariable("logqueries", CON_VARTYPE_BOOL, 0, &bLogQueries);
    pConsole->AddVariable("stream_rate", CON_VARTYPE_INT, 0, &iStreamRate, ServerStreamRateChanged);
    pConsole->AddVariable("stream_distance", CON_VARTYPE_FLOAT, 0, &fStreamDistance, ServerStreamDistanceChanged);
    pConsole->AddVariable("sleep", CON_VARTYPE_INT, 0, &iSleep);
    pConsole->AddVariable("maxnpc", CON_VARTYPE_INT, 0, &iMaxNpc);
    pConsole->AddVariable("onfoot_rate", CON_VARTYPE_INT, 0, &iOnFootRate);
    pConsole->AddVariable("incar_rate", CON_VARTYPE_INT, 0, &iInCarRate);
    pConsole->AddVariable("weapon_rate", CON_VARTYPE_INT, 0, &iWeaponRate);
    pConsole->AddVariable("chatlogging", CON_VARTYPE_INT, 0, &iChatLogging);
#ifndef _WIN32
    pConsole->AddVariable("output", CON_VARTYPE_BOOL, 0, &bOutputEnable);
#endif
    pConsole->AddVariable("timestamp", CON_VARTYPE_BOOL, 0, &bEnableTimestamp);
    pConsole->AddStringVariable("bind", 0, NULL);
    pConsole->AddStringVariable("password", 0, NULL);
    pConsole->AddStringVariable("hostname", 0, "SA-MP Server");
    pConsole->AddStringVariable("language", 0, NULL);
    pConsole->AddStringVariable("mapname", CON_VARFLAG_RULE, "San Andreas");
    pConsole->AddStringVariable("weburl", CON_VARFLAG_RULE, "www.sa-mp.com");
    pConsole->AddStringVariable("rcon_password", 0, "changeme");
    pConsole->AddStringVariable("gravity", 0, "0.008");
    pConsole->AddStringVariable("weather", CON_VARFLAG_RULE, "10");
    pConsole->AddStringVariable("gamemodetext", 0, "Unknown");
    pConsole->AddStringVariable("filterscripts", 0, "");
    pConsole->AddStringVariable("plugins", 0, "");
    pConsole->AddStringVariable("nosign", 0, "");
    pConsole->AddStringVariable("logtimeformat", 0, "[%H:%M:%S]");
    pConsole->AddVariable("messageholelimit", CON_VARTYPE_INT, 0, &iMessageHoleLimit);
    pConsole->AddVariable("messageslimit", CON_VARTYPE_INT, 0, &iMessagesLimit);
    pConsole->AddVariable("ackslimit", CON_VARTYPE_INT, 0, &iAcksLimit);
    pConsole->AddVariable("playertimeout", CON_VARTYPE_INT, 0, &iPlayerTimeout);
    pConsole->AddVariable("minconnectiontime", CON_VARTYPE_INT, 0, &iMinConnectionTime);
    pConsole->AddVariable("myriad", CON_VARTYPE_BOOL, 0, &bGameMod);
    pConsole->AddVariable("lagcompmode", CON_VARTYPE_INT, 0, &iLagCompMode);
    pConsole->AddVariable("connseedtime", CON_VARTYPE_INT, 0, &iConnSeedTime);
    pConsole->AddVariable("db_logging", CON_VARTYPE_INT, 0, &iDbLogging);
    pConsole->AddVariable("db_log_queries", CON_VARTYPE_INT, 0, &iDbLogQueries);
    pConsole->AddVariable("conncookies", CON_VARTYPE_INT, 0, &iConnCookies);
    pConsole->AddVariable("cookielogging", CON_VARTYPE_INT, 0, &iCookieLogging);
    pConsole->AddVariable("useartwork", CON_VARTYPE_BOOL, 0, &bUseArtwork);
    pConsole->AddStringVariable("artpath", 0, "models");
    for (unsigned int i = 0; i < 16; ++i)
    {
        char name[16];
        sprintf(name, "gamemode%u", i);
        pConsole->AddStringVariable(name, 0, "");
    }

    pConsole->Execute("exec server");
    pConsole->AddStringVariable("artwork", CON_VARFLAG_RULE | CON_VARFLAG_READONLY, bUseArtwork ? "Yes" : "No");
    pConsole->AddStringVariable("lagcomp", CON_VARFLAG_RULE | CON_VARFLAG_READONLY, iLagCompMode ? "On" : "Off");
    pConsole->AddStringVariable("version", CON_VARFLAG_RULE | CON_VARFLAG_READONLY, "0.3.DL-R1");
    pConsole->AddStringVariable("worldtime", CON_VARFLAG_RULE | CON_VARFLAG_READONLY, "12:00");
    const char* password = pConsole->GetStringVariable("rcon_password");
    if (password && strcmp(password, "changeme") == 0)
        logprintf("Error: Your password must be changed from the default password, please change it.");
    else
        RunScripts();

    pConsole = NULL;
    return 0;
}
