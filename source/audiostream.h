#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <windows.h>
struct CVector;
class CAudioStream
{
public:
    BYTE initialized;
    // 0x00066480
    BOOL Reset();
    // Descriptive static coordinator; native entry consumes no this pointer.
    // R5 address to verify
    static void Update3D();
    // 0x00066960
    BOOL Play(const char* url, float x, float y, float z, float radius, bool spatial);
    // CRT _beginthread entry; native ignores its argument and returns normally.
    // 0x00066700
    static void __cdecl Worker(void* argument);
    // 0x00066560
    BOOL Stop(bool wait);
    // Descriptive static helpers; native metadata entry consumes no this pointer.
    // 0x000665c0
    static void ReadMetadata();
    // BASS SYNCPROC ABI; all callback arguments are ignored by the original.
    // 0x000666f0
    static void CALLBACK MetadataSync(DWORD handle, DWORD channel, DWORD data, void* user);
    // 0x00066ab0
    void DrawInfo();
    // 0x00066a80
    void ControlGameRadio();
};
// Descriptive coordinate adapters; preserve sequential stores when input/output alias.
// R5 address to verify
void __cdecl AudioStreamSwapYZNegateX(const CVector* input, CVector* output);
// R5 address to verify
void __cdecl AudioStreamSwapNegateYZ(const CVector* input, CVector* output);
// Proven 257-byte clear regions in native metadata writers.
// 0x0012e5a8
extern char g_audioStreamTitle[257];
// 0x0012e398
extern char g_audioStreamStation[257];
// 0x000e9bb8
extern const char g_audioStreamInfoFormat[8];
// 0x0012e7b4
extern DWORD g_audioStreamHandle;
// 0x001027c8
extern char g_audioStreamAgent[10];
// Observed radio-control gate; stream start/stop writers remain to be recovered.
// 0x0012e7b8
extern BYTE g_audioStreamRadioActive;
// 0x001027d2
extern BYTE g_audioStreamStopRequested;
// 0x000e9bd0
extern const char g_audioStreamIcyTitleMarker[14];
// 0x000e9bc8
extern const char g_audioStreamArtistMarker[8];
// 0x000e9bc0
extern const char g_audioStreamTitleMarker[7];
// 0x000e6548
extern const char g_audioStreamStringFormat[3];
// R5 address to verify
extern char g_audioStreamUrl[257];
// R5 address to verify
extern char g_audioStreamStationUrl[257];
// R5 address to verify
extern CVector g_audioStreamPosition;
// R5 address to verify
extern BYTE g_audioStreamSpatial;
// 0x001027d4
extern float g_audioStreamRadius;
// 0x000e9bb4
extern const float g_audioStreamVolumeScale;
// R5 address to verify
extern const float g_audioStreamSpatialVolumeScale;
// 0x000e9bf0
extern const char g_audioStreamStationMarker[10];
// 0x000e9be4
extern const char g_audioStreamStationUrlMarker[9];
// 0x000e9c10
extern const char g_audioStreamMessageOffKey[12];
// 0x000e9bfc
extern const char g_audioStreamMessageFormat[17];
// 0x000e9c60
extern const char g_audioStreamOffsetFormat[45];
// 0x000e9c3c
extern const char g_audioStreamAttributesError[34];
// 0x000e9c1c
extern const char g_audioStreamPositionError[32];

#endif // AUDIOSTREAM_H
