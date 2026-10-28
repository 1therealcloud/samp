#include "audiostream.h"
#include "bass.h"
#include "config.h"
#include "game/game.h"
#include "main.h"
// 0x0016c7cc
DWORD g_audioStreamHandle;
// 0x001407b0
char g_audioStreamAgent[10] = "SA-MP/0.3";
// 0x00065f00
BOOL CAudioStream::Reset()
{
    g_audioStreamHandle = 0;
    BASS_Free();
    if (!BASS_Init(-1, 44100, 0, 0, 0))
        return FALSE;
    BASS_SetConfigPtr(16, g_audioStreamAgent);
    DWORD volume = (DWORD)(__int64)(g_pGame->m_pAudio->GetRadioVolume() * 7000.0f);
    BASS_SetConfig(5, volume);
    BASS_SetConfig(21, 1);
    BASS_SetConfig(11, 10000);
    if (!g_pConfig->DoesExist("audioproxyoff"))
        g_pConfig->WriteIntValue("audioproxyoff", 0, FALSE);
    if (g_pConfig->GetIntValue("audioproxyoff"))
        BASS_SetConfigPtr(17, 0);
    BASS_SetEAXParameters(-1, 0.0f, -1.0f, -1.0f);
    initialized = 1;
    return TRUE;
}

// 0x0016c7d0
BYTE g_audioStreamRadioActive;

// 0x00066500
void CAudioStream::ControlGameRadio()
{
    if (initialized && g_audioStreamRadioActive)
    {
        g_pGame->m_pAudio->StartRadio(-1);
        g_pGame->m_pAudio->StopRadio();
    }
}

#include "fontrender.h"
#include "game/entity.h"
#include "game/gta_globals.h"
#include "gui/device_objects.h"
#include <stdio.h>
#include <string.h>
// 0x0016c5c0
char g_audioStreamTitle[257];
// 0x0016c3b0
char g_audioStreamStation[257];
// 0x0011bbf0
const char g_audioStreamInfoFormat[8] = "%s - %s";
// 0x00066530
void CAudioStream::DrawInfo()
{
    if (!initialized || !g_audioStreamRadioActive)
        return;
    char text[257] = {0};
    if (!strlen(g_audioStreamTitle) || !strlen(g_audioStreamStation))
        return;
    _snprintf(text, 256, g_audioStreamInfoFormat, g_audioStreamTitle, g_audioStreamStation);
    // GTA SA virtual addresses: screen height0x00c17048, width0x00c17044.
    DWORD height = *(DWORD*)0xc17048, width = (DWORD)g_chatScreenWidth;
    RECT rect = {15, (LONG)(height - 20), (LONG)width, (LONG)(height - 20 + 30)};
    g_deviceFonts->DrawLittleText(0, text, rect, 0x100, 0x99ffffff, FALSE);
}
// 0x00066610
void __cdecl AudioStreamSwapYZNegateX(const CVector* input, CVector* output)
{
    // clang-format off
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
    // clang-format on
}
// 0x00066630
void __cdecl AudioStreamSwapNegateYZ(const CVector* input, CVector* output)
{
    // clang-format off
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
    // clang-format on
}

#include <stdlib.h>
// 0x001407ba
BYTE g_audioStreamStopRequested;
// 0x0011bc08
const char g_audioStreamIcyTitleMarker[14] = "StreamTitle='";
// 0x0011bc00
const char g_audioStreamArtistMarker[8] = "artist=";
// 0x0011bbf8
const char g_audioStreamTitleMarker[7] = "title=";
// 0x001185d8
const char g_audioStreamStringFormat[3] = "%s";
// 0x00065fe0
BOOL CAudioStream::Stop(bool wait)
{
    if (!initialized || !g_audioStreamRadioActive)
        return FALSE;
    g_audioStreamStopRequested = 1;
    if (wait)
        while (*(volatile BYTE*)&g_audioStreamRadioActive)
            Sleep(2);
    BASS_StreamFree(g_audioStreamHandle);
    g_audioStreamHandle = 0;
    return TRUE;
}
// 0x00066040
void CAudioStream::ReadMetadata()
{
    memset(g_audioStreamTitle, 0, 257);
    const char* tags = BASS_ChannelGetTags(g_audioStreamHandle, 5);
    if (tags)
    {
        const char* title = strstr(tags, g_audioStreamIcyTitleMarker);
        if (title)
        {
            char* copy = _strdup(title + 13);
            // Original assumes allocation and a trailing quote/semicolon are present.
            char* separator = strchr(copy, ';');
            separator[-1] = 0;
            _snprintf(g_audioStreamTitle, 256, g_audioStreamStringFormat, copy);
            free(copy);
        }
        return;
    }
    tags = BASS_ChannelGetTags(g_audioStreamHandle, 2);
    if (!tags)
        return;
    const char *artist = 0, *title = 0;
    while (*tags)
    {
        if (!_strnicmp(tags, g_audioStreamArtistMarker, 7))
            artist = tags + 7;
        if (!_strnicmp(tags, g_audioStreamTitleMarker, 6))
            title = tags + 6;
        tags += strlen(tags) + 1;
    }
    if (artist)
        _snprintf(g_audioStreamTitle, 256, g_audioStreamInfoFormat, artist, title);
    else if (title)
        _snprintf(g_audioStreamTitle, 256, g_audioStreamStringFormat, title);
}
// 0x00066170
void CALLBACK CAudioStream::MetadataSync(DWORD, DWORD, DWORD, void*)
{
    ReadMetadata();
}

#include "chatwindow.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include <process.h>
// MSVC CRT conversion helper, supplied by selected static runtime.
// 0x000f9324
extern "C" __int64 __cdecl _ftol2();
// 0x0016c4b8
char g_audioStreamUrl[257];
// 0x0016c6c8
char g_audioStreamStationUrl[257];
// 0x0016c7d4
CVector g_audioStreamPosition;
// 0x0016c7e0
BYTE g_audioStreamSpatial;
// 0x001407bc
float g_audioStreamRadius = 50.0f;
// 0x0011bbec
const float g_audioStreamVolumeScale = 7000.0f;
// 0x0011bc18
const float g_audioStreamSpatialVolumeScale = 8000.0f;
// 0x0011bc28
const char g_audioStreamStationMarker[10] = "icy-name:";
// 0x0011bc1c
const char g_audioStreamStationUrlMarker[9] = "icy-url:";
// 0x0011bc48
const char g_audioStreamMessageOffKey[12] = "audiomsgoff";
// 0x0011bc34
const char g_audioStreamMessageFormat[17] = "Audio stream: %s";
// 0x000663e0
BOOL CAudioStream::Play(const char* url, float x, float y, float z, float radius, bool spatial)
{
    if (!initialized)
        return FALSE;
    Stop(true);
    if (g_audioStreamHandle)
    {
        BASS_ChannelStop(g_audioStreamHandle);
        g_audioStreamHandle = 0;
    }
    BYTE allowed;
    // clang-format off
    __asm {
        mov eax,g_pGame
        mov ecx,[eax]
        call CAudio::GetRadioVolume
        fcomp g_objectScaleLowerBound
        fnstsw ax
        test ah,41h
        setp allowed
    }
    // clang-format on
    // The original parity test accepts unordered radio volume as well as >0.
    if (!allowed) return FALSE;
    memset(g_audioStreamUrl, 0, 257);
    strncpy(g_audioStreamUrl, url, 256);
    memset(g_audioStreamTitle, 0, 257);
    memset(g_audioStreamStation, 0, 257);
    memset(g_audioStreamStationUrl, 0, 257);
    memcpy(&g_audioStreamPosition.x, &x, 4);
    memcpy(&g_audioStreamPosition.y, &y, 4);
    memcpy(&g_audioStreamPosition.z, &z, 4);
    memcpy(&g_audioStreamRadius, &radius, 4);
    g_audioStreamSpatial = spatial;
    if (!g_pConfig->GetIntValue(g_audioStreamMessageOffKey))
        g_pChat->AddInfoMessage(g_audioStreamMessageFormat, g_audioStreamUrl);
    g_audioStreamStopRequested = 0;
    _beginthread(Worker, 0, 0);
    return TRUE;
}
// 0x00066180
void __cdecl CAudioStream::Worker(void*)
{
    g_audioStreamRadioActive = 1;
    if (g_audioStreamHandle)
    {
        BASS_ChannelStop(g_audioStreamHandle);
        g_audioStreamHandle = 0;
    }
    g_audioStreamHandle = BASS_StreamCreateURL(g_audioStreamUrl, 0, 0x940000, 0, 0);
    BASS_ChannelPlay(g_audioStreamHandle, FALSE);
    memset(g_audioStreamStation, 0, 257);
    memset(g_audioStreamStationUrl, 0, 257);
    const char* tags = BASS_ChannelGetTags(g_audioStreamHandle, 4);
    if (!tags)
        tags = BASS_ChannelGetTags(g_audioStreamHandle, 3);
    if (tags)
        while (*tags)
        {
            if (!_strnicmp(tags, g_audioStreamStationMarker, 9))
                _snprintf(g_audioStreamStation, 256, g_audioStreamStringFormat, tags + 9);
            if (!_strnicmp(tags, g_audioStreamStationUrlMarker, 8))
                _snprintf(g_audioStreamStationUrl, 256, g_audioStreamStringFormat, tags + 8);
            tags += strlen(tags) + 1;
        }
    ReadMetadata();
    BASS_ChannelSetSync(g_audioStreamHandle, 4, 0, MetadataSync, 0);
    BASS_ChannelSetSync(g_audioStreamHandle, 12, 0, MetadataSync, 0);
    while (!*(volatile BYTE*)&g_audioStreamStopRequested)
    {
        BASS_StreamGetFilePosition(g_audioStreamHandle, 4);
        int volume = 0;
        if (!g_pGame->IsMenuVisible())
        {
            HWND window = g_inputGameWindow;
            if (GetForegroundWindow() == window)
            {
                if (g_audioStreamSpatial)
                {
                    int baseVolume;
                    // clang-format off
                    __asm {
                        mov eax,g_pGame
                        mov ecx,[eax]
                        call CAudio::GetRadioVolume
                        fmul g_audioStreamSpatialVolumeScale
                        call _ftol2
                        mov baseVolume,eax
                    }
                    // clang-format on
                    CPed* ped = g_pGame->GetPlayerPed();
                    if (ped)
                    {
                        DWORD px, py, pz;
                        memcpy(&px, &g_audioStreamPosition.x, 4);
                        memcpy(&py, &g_audioStreamPosition.y, 4);
                        memcpy(&pz, &g_audioStreamPosition.z, 4);
                        // clang-format off
                        __asm {
                            push pz
                            push py
                            push px
                            mov ecx,ped
                            call CEntity::GetDistanceToPoint
                            fcom g_audioStreamRadius
                            fnstsw ax
                            test ah,5
                            jp outside_radius
                            fdiv g_audioStreamRadius
                            fsubr g_quaternionMatrixOne
                            fimul baseVolume
                            call _ftol2
                            mov volume,eax
                            jmp distance_done
outside_radius:
                            fstp st(0)
distance_done:
                        }
                        // clang-format on
                    }
                }
                else
                {
                    // clang-format off
                    __asm {
                        mov eax,g_pGame
                        mov ecx,[eax]
                        call CAudio::GetRadioVolume
                        fmul g_audioStreamVolumeScale
                        call _ftol2
                        mov volume,eax
                    }
                    // clang-format on
                }
            }
        }
        BASS_SetConfig(5, (DWORD)volume);
        Sleep(20);
    }
    BASS_ChannelStop(g_audioStreamHandle);
    g_audioStreamHandle = 0;
    g_audioStreamRadioActive = 0;
}

// 0x0011bc98
const char g_audioStreamOffsetFormat[45] = "BASS_ChannelSet3DAttributes offset: %f %f %f";
// 0x0011bc74
const char g_audioStreamAttributesError[34] = "BASS_ChannelSet3DAttributes Error";
// 0x0011bc54
const char g_audioStreamPositionError[32] = "BASS_ChannelSet3DPosition Error";

// 0x00066650
void CAudioStream::Update3D()
{
    CPed* ped = g_pGame->GetPlayerPed();
    if (!ped)
        return;
    CVector aim[2];
    memcpy(aim, ped->GetCameraAim(), 24);
    CVector front, listener, source;
    AudioStreamSwapNegateYZ(&aim[0], &front);
    AudioStreamSwapYZNegateX(&aim[1], &listener);
    AudioStreamSwapYZNegateX(&g_audioStreamPosition, &source);
    CVector velocity = {0, 0, 0};
    BYTE* backend;
    if (ped->IsInVehicle())
    {
        backend = (BYTE*)ped->GetVehicle();
        if (!backend)
            return;
    }
    else
        backend = (BYTE*)ped->m_pGamePed;
    memcpy(&velocity, backend + 0x44, 12);
    BASS_Set3DFactors(1.0f, 1.0f, 2.0f);
    BASS_Set3DPosition(0, (const BASS_3DVECTOR*)&velocity, (const BASS_3DVECTOR*)&front, 0);
    if (g_audioStreamHandle && g_audioStreamSpatial)
    {
        CChat* chat = g_pChat;
        CVector offset;
        double debugX, debugY, debugZ;
        const float *listenerData = (const float*)&listener, *sourceData = (const float*)&source;
        float* offsetData = (float*)&offset;
        // clang-format off
        __asm {
            mov ecx,listenerData
            mov edx,sourceData
            mov eax,offsetData
            fld dword ptr [ecx]
            fsub dword ptr [edx]
            fstp dword ptr [eax]
            fld dword ptr [ecx+4]
            fsub dword ptr [edx+4]
            fstp dword ptr [eax+4]
            fld dword ptr [ecx+8]
            fsub dword ptr [edx+8]
            fst dword ptr [eax+8]
            fstp debugZ
            fld dword ptr [eax+4]
            fstp debugY
            fld dword ptr [eax]
            fstp debugX
        }
        // clang-format on
        // Native Z debug value retains the difference before its float store.
        chat->AddDebugMessage(g_audioStreamOffsetFormat, debugX, debugY, debugZ);
        if (!BASS_ChannelSet3DAttributes(g_audioStreamHandle, 1, 10.0f, 100.0f, 360, 360, 1.0f))
            g_pChat->AddDebugMessage(g_audioStreamAttributesError);
        if (!BASS_ChannelSet3DPosition(g_audioStreamHandle, (const BASS_3DVECTOR*)&offset, 0, 0))
            g_pChat->AddDebugMessage(g_audioStreamPositionError);
    }
    BASS_Apply3D();
}
