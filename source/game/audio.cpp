#include "game/audio.h"

#include "game/scripting.h"

int CAudio::GetRadioStation()
{
    // GTA SA radio manager VA: 0x008cb6f8; station byte is at +0xe9.
    int station = 0;
    // clang-format off
    __asm {
        mov ecx,0x8cb6f8
        xor eax,eax
        mov al,byte ptr [ecx+0xe9]
        mov station,eax
    }
    // clang-format on
    if (station < 0 || station > 12) return -1;
    return station;
}

void CAudio::StartRadio(int station)
{
    typedef void(__fastcall * BackendFn)(void*, int, DWORD, DWORD);
    reinterpret_cast<BackendFn>(0x507dc0)((void*)0xb6bc90, 0, (DWORD)station, 0);
}

void CAudio::StopRadio()
{
    typedef void(__fastcall * BackendFn)(void*, int, DWORD, DWORD);
    reinterpret_cast<BackendFn>(0x506f70)((void*)0xb6bc90, 0, 0, 0);
}

float CAudio::GetRadioVolume()
{
    return *(float*)0xb5fcc8;
}

void CAudio::StopOutdoorAmbienceTrack()
{
    typedef void(__fastcall * BackendFn)(void*, int);
    reinterpret_cast<BackendFn>(0x4d6d60)((void*)0x8ac15c, 0);
}

void CAudio::SetOutdoorAmbienceTrack(int track)
{
    typedef void(__fastcall * BackendFn)(void*, int, DWORD);
    reinterpret_cast<BackendFn>(0x4d6d50)((void*)0x8ac15c, 0, (DWORD)track);
}

bool CAudio::IsOutdoorAmbienceTrackDisabled()
{
    return m_bOutdoorAmbienceTrackDisabled;
}

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

void CAudio::Process()
{
    if (m_bSoundLoaded && m_bSoundLoaded == 1 && Scripting::ProcessCommand(&g_audioIsLoaded, 1))
    {
        Scripting::ProcessCommand(&g_audioStart, 1);
        m_bSoundLoaded = 2;
    }
}
