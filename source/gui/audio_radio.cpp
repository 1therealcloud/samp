#include "gui/audio_stream.h"
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
