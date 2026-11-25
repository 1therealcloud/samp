#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <windows.h>
struct CVector;
class CAudioStream
{
public:
    BYTE initialized;

    BOOL Reset();
    // Descriptive static coordinator; native entry consumes no this pointer.

    static void Update3D();

    BOOL Play(const char* url, float x, float y, float z, float radius, bool spatial);
    // CRT _beginthread entry; native ignores its argument and returns normally.

    static void __cdecl Worker(void* argument);

    BOOL Stop(bool wait);
    // Descriptive static helpers; native metadata entry consumes no this pointer.

    static void ReadMetadata();
    // BASS SYNCPROC ABI; all callback arguments are ignored by the original.

    static void CALLBACK MetadataSync(DWORD handle, DWORD channel, DWORD data, void* user);

    void DrawInfo();

    void ControlGameRadio();
};
// Descriptive coordinate adapters; preserve sequential stores when input/output alias.

void __cdecl AudioStreamSwapYZNegateX(const CVector* input, CVector* output);

void __cdecl AudioStreamSwapNegateYZ(const CVector* input, CVector* output);
// Proven 257-byte clear regions in native metadata writers.

extern char g_audioStreamTitle[257];

extern char g_audioStreamStation[257];

extern const char g_audioStreamInfoFormat[8];

extern DWORD g_audioStreamHandle;

extern char g_audioStreamAgent[10];
// Observed radio-control gate; stream start/stop writers remain to be recovered.

extern BYTE g_audioStreamRadioActive;

extern BYTE g_audioStreamStopRequested;

extern const char g_audioStreamIcyTitleMarker[14];

extern const char g_audioStreamArtistMarker[8];

extern const char g_audioStreamTitleMarker[7];

extern const char g_audioStreamStringFormat[3];

extern char g_audioStreamUrl[257];

extern char g_audioStreamStationUrl[257];

extern CVector g_audioStreamPosition;

extern BYTE g_audioStreamSpatial;

extern float g_audioStreamRadius;

extern const float g_audioStreamVolumeScale;

extern const float g_audioStreamSpatialVolumeScale;

extern const char g_audioStreamStationMarker[10];

extern const char g_audioStreamStationUrlMarker[9];

extern const char g_audioStreamMessageOffKey[12];

extern const char g_audioStreamMessageFormat[17];

extern const char g_audioStreamOffsetFormat[45];

extern const char g_audioStreamAttributesError[34];

extern const char g_audioStreamPositionError[32];

#endif // AUDIOSTREAM_H
