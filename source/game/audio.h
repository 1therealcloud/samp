#ifndef AUDIO_H
#define AUDIO_H

#include "game/entity.h"

#include <static_assert.h>

#pragma pack(push, 1)
class CAudio
{
public:
    BOOL m_bSoundLoaded;
    bool m_bOutdoorAmbienceTrackDisabled;
    // inline
    CAudio() : m_bSoundLoaded(FALSE), m_bOutdoorAmbienceTrackDisabled(false) {}
    // 0x000a2210
    int GetRadioStation();
    // 0x000a2240
    void StartRadio(int station);
    // Native wrapper at A2080; external A2260 anchor points into HUD code.
    // 0x000a2260
    void StopRadio();
    // 0x000a2280
    float GetRadioVolume();
    // 0x000a2290
    void StopOutdoorAmbienceTrack();
    // 0x000a22a0
    void SetOutdoorAmbienceTrack(int track);
    // 0x000a23a0
    bool IsOutdoorAmbienceTrackDisabled();
    // 0x000a22c0
    void Play(int sound, CVector position);
    // 0x000a21d0
    void Process();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CAudio) == 5);

#endif // AUDIO_H
