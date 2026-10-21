#include "audiostream.h"
#include "game/game.h"
#include "main.h"
#include "config.h"
#include "bass.h"
// samp.dll RVA: 0x0016c7cc
DWORD g_audioStreamHandle;
// samp.dll RVA: 0x001407b0
char g_audioStreamAgent[10]="SA-MP/0.3";
// samp.dll RVA: 0x00065f00
BOOL CAudioStream::Reset(){
    g_audioStreamHandle=0;BASS_Free();
    if(!BASS_Init(-1,44100,0,0,0))return FALSE;
    BASS_SetConfigPtr(16,g_audioStreamAgent);
    DWORD volume=(DWORD)(__int64)(g_pGame->m_pAudio->GetRadioVolume()*7000.0f);
    BASS_SetConfig(5,volume);BASS_SetConfig(21,1);BASS_SetConfig(11,10000);
    if(!g_pConfig->DoesExist("audioproxyoff"))g_pConfig->WriteIntValue("audioproxyoff",0,FALSE);
    if(g_pConfig->GetIntValue("audioproxyoff"))BASS_SetConfigPtr(17,0);
    BASS_SetEAXParameters(-1,0.0f,-1.0f,-1.0f);initialized=1;return TRUE;
}

#include "audiostream.h"
#include "game/game.h"
#include "main.h"

// samp.dll RVA: 0x0016c7d0
BYTE g_audioStreamRadioActive;

// samp.dll RVA: 0x00066500
void CAudioStream::ControlGameRadio() {
    if(initialized && g_audioStreamRadioActive) {
        g_pGame->m_pAudio->StartRadio(-1);
        g_pGame->m_pAudio->StopRadio();
    }
}
