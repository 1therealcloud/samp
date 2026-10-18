#include "main.h"
#include <string.h>
// Native BSS storage and command-line settings. DLL startup remains open.
// samp.dll RVA: 0x002aca3c
CGame *g_pGame;
// samp.dll RVA: 0x002aca14
CInput *g_pInput;
// samp.dll RVA: 0x002aca10
CChat *g_pChat;
// samp.dll RVA: 0x002aca24
void *g_pNetGame;
// samp.dll RVA: 0x002abe78
ClientSettings g_clientSettings;
// Native parsers have no destination bound; missing opening quotes also have
// no terminator guard. Retain that behavior instead of silently truncating.
// samp.dll RVA: 0x000c5590
void Settings::GetFromCommandLine(const char *line,char *output) {
    while(*line==' ')++line;
    while(*line && *line!=' ' && *line!='-' && *line!='/')*output++=*line++;
    *output=0;
}
// samp.dll RVA: 0x000c55e0
void Settings::GetFromQuotes(const char *line,char *output) {
    while(*line!='"')++line;
    ++line;while(*line && *line!='"')*output++=*line++;
    *output=0;
}
// The scanner keeps walking through argument values, including quoted paths.
// Later -c/-d occurrences overwrite earlier debug/online selection.
// samp.dll RVA: 0x000c5c90
void Settings::Initialize() {
    const char *line=GetCommandLineA();memset(&g_clientSettings,0,sizeof(g_clientSettings));
    while(*line) {
        if(*line=='-' || *line=='/') {
            ++line;
            switch(*line) {
            case 'd':g_clientSettings.debug=TRUE;g_clientSettings.online=FALSE;break;
            case 'c':g_clientSettings.online=TRUE;g_clientSettings.debug=FALSE;break;
            case 'h':GetFromCommandLine(++line,g_clientSettings.host);break;
            case 'p':GetFromCommandLine(++line,g_clientSettings.port);break;
            case 'n':GetFromCommandLine(++line,g_clientSettings.nickname);break;
            case 'z':GetFromCommandLine(++line,g_clientSettings.password);break;
            case 'l':GetFromQuotes(++line,g_clientSettings.debugScript);break;
            }
        }
        ++line;
    }
}
