#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <windows.h>

struct CVector;

class CAudioStream
{
public:
    BYTE initialized;

    BOOL Reset();

    static void Update3D();

    BOOL Play(const char* url, float x, float y, float z, float radius, bool spatial);

    static void __cdecl Worker(void* argument);

    BOOL Stop(bool wait);

    static void ReadMetadata();

    static void CALLBACK MetadataSync(DWORD handle, DWORD channel, DWORD data, void* user);

    void DrawInfo();

    void ControlGameRadio();
};

#endif // AUDIOSTREAM_H
