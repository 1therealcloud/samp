#ifndef SAMP_RECOVERED_AUDIO_H
#define SAMP_RECOVERED_AUDIO_H
#include "game/entity.h"
#pragma pack(push, 1)
class CAudio
{
public:
    BOOL m_bSoundLoaded;
    bool m_bOutdoorAmbienceTrackDisabled;
    // samp.dll RVA: unknown (inline initialization in CGame RVA 0x0009fdc0; standalone entry not established)
    CAudio() : m_bSoundLoaded(FALSE), m_bOutdoorAmbienceTrackDisabled(false) {}
    // 0x000a2030
    int GetRadioStation();
    // 0x000a2060
    void StartRadio(int station);
    // Native wrapper at A2080; external A2260 anchor points into HUD code.
    // 0x000a2080
    void StopRadio();
    // 0x000a20a0
    float GetRadioVolume();
    // 0x000a20b0
    void StopOutdoorAmbienceTrack();
    // 0x000a20c0
    void SetOutdoorAmbienceTrack(int track);
    // 0x000a21c0
    bool IsOutdoorAmbienceTrackDisabled();
    // 0x000a20e0
    void Play(int sound, CVector position);
    // 0x000a1ff0
    void Process();
};
#pragma pack(pop)
typedef char AudioExtent[(sizeof(CAudio) == 5) ? 1 : -1];
#endif
