#include "audiostream.h"

#include "config.h"
#include "game/entity.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "main.h"
#include "render/fontrender.h"

#include <bass.h>

#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 0x0016c7cc
DWORD g_audioStreamHandle;
// 0x001407b0
char g_audioStreamAgent[10] = "SA-MP/0.3";
// 0x0016c7d0
BYTE g_audioStreamRadioActive;
// 0x0016c5c0
char g_audioStreamTitle[257];
// 0x0016c3b0
char g_audioStreamStation[257];
// 0x0011bbf0
const char g_audioStreamInfoFormat[8] = "%s - %s";
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
// 0x0011bc98
const char g_audioStreamOffsetFormat[45] = "BASS_ChannelSet3DAttributes offset: %f %f %f";
// 0x0011bc74
const char g_audioStreamAttributesError[34] = "BASS_ChannelSet3DAttributes Error";
// 0x0011bc54
const char g_audioStreamPositionError[32] = "BASS_ChannelSet3DPosition Error";

// 0x0016c4b8
char g_audioStreamUrl[257];

// 0x00065f00
BOOL CAudioStream::Reset()
{
    g_audioStreamHandle = 0;
    BASS_Free();
    if (BASS_Init(-1, 44100, 0, 0, 0))
    {
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
    return FALSE;
}

// 0x00066500
void CAudioStream::ControlGameRadio()
{
    if (initialized && g_audioStreamRadioActive)
    {
        g_pGame->m_pAudio->StartRadio(-1);
        g_pGame->m_pAudio->StopRadio();
    }
}

// 0x00066530
void CAudioStream::DrawInfo()
{
    if (!initialized || !g_audioStreamRadioActive)
        return;
    char text[257];
    memset(text, 0, sizeof(text));
    if (!strlen(g_audioStreamTitle) || !strlen(g_audioStreamStation))
        return;
    snprintf(text, 256, g_audioStreamInfoFormat, g_audioStreamTitle, g_audioStreamStation);
    // GTA SA virtual addresses: screen height0x00c17048, width0x00c17044.
    DWORD height = *(DWORD*)0xc17048, width = (DWORD)(*(int*)0x00c17044);
    RECT rect;
    rect.left = 15;
    rect.top = (LONG)(height - 20);
    rect.bottom = (LONG)((DWORD)rect.top + 30);
    rect.right = (LONG)width;
    g_deviceFonts->DrawLittleText(0, text, rect, 0x100, 0x99ffffff, FALSE);
}
// 0x00066610
void __cdecl AudioStreamSwapYZNegateX(const CVector* input, CVector* output)
{
    float x = input->x;
    float y = input->y;
    float z = input->z;

    output->x = -x;
    output->y = z;
    output->z = y;
}

// 0x00066630
void __cdecl AudioStreamSwapNegateYZ(const CVector* input, CVector* output)
{
    float x = input->x;
    float y = input->y;
    float z = input->z;

    output->x = x;
    output->y = -z;
    output->z = -y;
}

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
            snprintf(g_audioStreamTitle, 256, g_audioStreamStringFormat, copy);
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
        snprintf(g_audioStreamTitle, 256, g_audioStreamInfoFormat, artist, title);
    else if (title)
        snprintf(g_audioStreamTitle, 256, g_audioStreamStringFormat, title);
}

// 0x00066170
void CALLBACK CAudioStream::MetadataSync(DWORD, DWORD, DWORD, void*)
{
    ReadMetadata();
}

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
    if (g_pGame->m_pAudio->GetRadioVolume() <= g_objectScaleLowerBound)
        return FALSE;
    memset(g_audioStreamUrl, 0, 257);
    strncpy_s(g_audioStreamUrl, url, 256);
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
                snprintf(g_audioStreamStation, 256, g_audioStreamStringFormat, tags + 9);
            if (!_strnicmp(tags, g_audioStreamStationUrlMarker, 8))
                snprintf(g_audioStreamStationUrl, 256, g_audioStreamStringFormat, tags + 8);
            tags += strlen(tags) + 1;
        }
    ReadMetadata();
    BASS_ChannelSetSync(g_audioStreamHandle, 4, 0, MetadataSync, 0);
    BASS_ChannelSetSync(g_audioStreamHandle, 12, 0, MetadataSync, 0);
    while (!*(volatile BYTE*)&g_audioStreamStopRequested)
    {
        BASS_StreamGetFilePosition(g_audioStreamHandle, 4);
        if (!g_pGame->IsMenuVisible())
        {
            HWND window = (*(HWND*)0x00c97c1c);
            if (GetForegroundWindow() == window)
            {
                if (g_audioStreamSpatial)
                {
                    int baseVolume = (int)(g_pGame->m_pAudio->GetRadioVolume() * g_audioStreamSpatialVolumeScale);
                    int volume = 0;
                    CPed* ped = g_pGame->GetPlayerPed();
                    if (ped)
                    {
                        const float distance = ped->GetDistanceToPoint(g_audioStreamPosition.x, g_audioStreamPosition.y, g_audioStreamPosition.z);
                        if (distance < g_audioStreamRadius)
                            volume = (int)(baseVolume * (g_quaternionMatrixOne - distance / g_audioStreamRadius));
                    }
                    BASS_SetConfig(5, (DWORD)volume);
                }
                else
                {
                    BASS_SetConfig(5, (DWORD)(int)(g_pGame->m_pAudio->GetRadioVolume() * g_audioStreamVolumeScale));
                }
            }
            else
                BASS_SetConfig(5, 0);
        }
        else
            BASS_SetConfig(5, 0);
        Sleep(20);
    }
    BASS_ChannelStop(g_audioStreamHandle);
    g_audioStreamHandle = 0;
    g_audioStreamRadioActive = 0;
}

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
        const float *listenerData = (const float*)&listener, *sourceData = (const float*)&source;
        float* offsetData = (float*)&offset;

        double debugZ = (double)listenerData[2] - (double)sourceData[2];
        offsetData[0] = (float)((double)listenerData[0] - (double)sourceData[0]);
        offsetData[1] = (float)((double)listenerData[1] - (double)sourceData[1]);
        offsetData[2] = (float)debugZ;

        double debugX = offsetData[0];
        double debugY = offsetData[1];

        chat->AddDebugMessage(g_audioStreamOffsetFormat, debugX, debugY, debugZ);
        if (!BASS_ChannelSet3DAttributes(g_audioStreamHandle, 1, 10.0f, 100.0f, 360, 360, 1.0f))
            g_pChat->AddDebugMessage(g_audioStreamAttributesError);
        if (!BASS_ChannelSet3DPosition(g_audioStreamHandle, (const BASS_3DVECTOR*)&offset, 0, 0))
            g_pChat->AddDebugMessage(g_audioStreamPositionError);
    }
    BASS_Apply3D();
}
