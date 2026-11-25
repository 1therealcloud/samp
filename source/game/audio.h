#ifndef AUDIO_H
#define AUDIO_H

#include "game/entity.h"

#pragma pack(push, 1)
class CAudio
{
public:
    BOOL m_bSoundLoaded;
    bool m_bOutdoorAmbienceTrackDisabled;

    CAudio() : m_bSoundLoaded(FALSE), m_bOutdoorAmbienceTrackDisabled(false) {}

    int GetRadioStation();

    void StartRadio(int station);
    // Native wrapper at A2080; external A2260 anchor points into HUD code.

    void StopRadio();

    float GetRadioVolume();

    void StopOutdoorAmbienceTrack();

    void SetOutdoorAmbienceTrack(int track);

    bool IsOutdoorAmbienceTrackDisabled();

    void Play(int sound, CVector position);

    void Process();
};
#pragma pack(pop)
static_assert(sizeof(CAudio) == 5);

#endif // AUDIO_H
