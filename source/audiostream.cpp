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

// samp.dll RVA: 0x0016c7d0
BYTE g_audioStreamRadioActive;

// samp.dll RVA: 0x00066500
void CAudioStream::ControlGameRadio() {
    if(initialized && g_audioStreamRadioActive) {
        g_pGame->m_pAudio->StartRadio(-1);
        g_pGame->m_pAudio->StopRadio();
    }
}

#include "fontrender.h"
#include "gui/device_objects.h"
#include "game/entity.h"
#include "game/gta_globals.h"
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x0016c5c0
char g_audioStreamTitle[257];
// samp.dll RVA: 0x0016c3b0
char g_audioStreamStation[257];
// samp.dll RVA: 0x0011bbf0
const char g_audioStreamInfoFormat[8]="%s - %s";
// samp.dll RVA: 0x00066530
void CAudioStream::DrawInfo() {
    if(!initialized||!g_audioStreamRadioActive)return;
    char text[257]={0};
    if(!strlen(g_audioStreamTitle)||!strlen(g_audioStreamStation))return;
    _snprintf(text,256,g_audioStreamInfoFormat,g_audioStreamTitle,g_audioStreamStation);
    // GTA SA virtual addresses: screen height0x00c17048, width0x00c17044.
    DWORD height=*(DWORD*)0xc17048,width=(DWORD)g_chatScreenWidth;
    RECT rect={15,(LONG)(height-20),(LONG)width,(LONG)(height-20+30)};
    g_deviceFonts->DrawLittleText(0,text,rect,0x100,0x99ffffff,FALSE);
}
// samp.dll RVA: 0x00066610
void __cdecl AudioStreamSwapYZNegateX(const CVector *input,CVector *output) {
    __asm {
        mov eax,input
        mov ecx,output
        fld dword ptr [eax]
        fchs
        fstp dword ptr [ecx]
        mov edx,dword ptr [eax+8]
        mov dword ptr [ecx+4],edx
        mov eax,dword ptr [eax+4]
        mov dword ptr [ecx+8],eax
    }
}
// samp.dll RVA: 0x00066630
void __cdecl AudioStreamSwapNegateYZ(const CVector *input,CVector *output) {
    __asm {
        mov eax,input
        mov edx,dword ptr [eax]
        mov ecx,output
        mov dword ptr [ecx],edx
        fld dword ptr [eax+8]
        fchs
        fstp dword ptr [ecx+4]
        fld dword ptr [eax+4]
        fchs
        fstp dword ptr [ecx+8]
    }
}
