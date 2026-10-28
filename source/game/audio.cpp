#include "game/audio.h"
#include "game/scripting.h"
// samp.dll RVA: 0x000a2030
int CAudio::GetRadioStation()
{
    unsigned station = *(BYTE*)0x8cb7e1;
    return station <= 12 ? (int)station : -1;
}
// samp.dll RVA: 0x000a2060
void CAudio::StartRadio(int station)
{
    ((void(__fastcall*)(void*, void*, int, int))0x507dc0)((void*)0xb6bc90, 0, station, 0);
}
// samp.dll RVA: 0x000a2080
void CAudio::StopRadio()
{
    ((void(__fastcall*)(void*, void*, int, int))0x506f70)((void*)0xb6bc90, 0, 0, 0);
}
// samp.dll RVA: 0x000a20a0
float CAudio::GetRadioVolume()
{
    return *(float*)0xb5fcc8;
}
// samp.dll RVA: 0x000a20b0
void CAudio::StopOutdoorAmbienceTrack()
{
    ((void(__fastcall*)(void*, void*))0x4d6d60)((void*)0x8ac15c, 0);
}
// samp.dll RVA: 0x000a20c0
void CAudio::SetOutdoorAmbienceTrack(int track)
{
    ((void(__fastcall*)(void*, void*, int))0x4d6d50)((void*)0x8ac15c, 0, track);
}
// samp.dll RVA: 0x000a21c0
bool CAudio::IsOutdoorAmbienceTrackDisabled()
{
    return m_bOutdoorAmbienceTrackDisabled;
}
// samp.dll RVA: 0x000a20e0
void CAudio::Play(int sound, CVector position)
{
    if (sound == 0)
    {
        if (m_bSoundLoaded)
        {
            Scripting::ProcessCommand(&g_audioUnload, 1);
            m_bSoundLoaded = FALSE;
        }
        StopOutdoorAmbienceTrack();
        m_bOutdoorAmbienceTrackDisabled = false;
    }
    else if (sound == 1)
        m_bOutdoorAmbienceTrackDisabled = true;
    else if (sound < 1000)
        SetOutdoorAmbienceTrack(sound);
    else if (sound < 2000)
        Scripting::ProcessCommand(&g_audioPlay, (double)position.x, (double)position.y, (double)position.z, sound);
    else
    {
        Scripting::ProcessCommand(&g_audioUnload, 1);
        Scripting::ProcessCommand(&g_audioLoad, 1, sound);
        Scripting::ProcessCommand(&g_audioSetPosition, 1, (double)position.x, (double)position.y, (double)position.z);
        m_bSoundLoaded = TRUE;
    }
}

// samp.dll RVA: 0x000a1ff0
void CAudio::Process()
{
    if (m_bSoundLoaded == 1 && Scripting::ProcessCommand(&g_audioIsLoaded, 1))
    {
        Scripting::ProcessCommand(&g_audioStart, 1);
        m_bSoundLoaded = 2;
    }
}
