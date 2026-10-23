#ifndef SAMP_AUDIO_STREAM_NATIVE_H
#define SAMP_AUDIO_STREAM_NATIVE_H
#include <windows.h>
struct CVector;
class CAudioStream {public:
    BYTE initialized;
    // samp.dll RVA: 0x00065f00
    BOOL Reset();
    // samp.dll RVA: 0x00065fe0
    BOOL Stop(bool wait);
    // Descriptive static helpers; native metadata entry consumes no this pointer.
    // samp.dll RVA: 0x00066040
    static void ReadMetadata();
    // BASS SYNCPROC ABI; all callback arguments are ignored by the original.
    // samp.dll RVA: 0x00066170
    static void CALLBACK MetadataSync(DWORD handle,DWORD channel,DWORD data,void *user);
    // samp.dll RVA: 0x00066530
    void DrawInfo();
    // samp.dll RVA: 0x00066500
    void ControlGameRadio();
};
// Descriptive coordinate adapters; preserve sequential stores when input/output alias.
// samp.dll RVA: 0x00066610
void __cdecl AudioStreamSwapYZNegateX(const CVector *input,CVector *output);
// samp.dll RVA: 0x00066630
void __cdecl AudioStreamSwapNegateYZ(const CVector *input,CVector *output);
// Proven 257-byte clear regions in native metadata writers.
// samp.dll RVA: 0x0016c5c0
extern char g_audioStreamTitle[257];
// samp.dll RVA: 0x0016c3b0
extern char g_audioStreamStation[257];
// samp.dll RVA: 0x0011bbf0
extern const char g_audioStreamInfoFormat[8];
// samp.dll RVA: 0x0016c7cc
extern DWORD g_audioStreamHandle;
// samp.dll RVA: 0x001407b0
extern char g_audioStreamAgent[10];
// Observed radio-control gate; stream start/stop writers remain to be recovered.
// samp.dll RVA: 0x0016c7d0
extern BYTE g_audioStreamRadioActive;
// samp.dll RVA: 0x001407ba
extern BYTE g_audioStreamStopRequested;
// samp.dll RVA: 0x0011bc08
extern const char g_audioStreamIcyTitleMarker[14];
// samp.dll RVA: 0x0011bc00
extern const char g_audioStreamArtistMarker[8];
// samp.dll RVA: 0x0011bbf8
extern const char g_audioStreamTitleMarker[7];
// samp.dll RVA: 0x001185d8
extern const char g_audioStreamStringFormat[3];
#endif
