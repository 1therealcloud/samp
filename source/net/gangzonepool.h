#ifndef GANGZONEPOOL_H
#define GANGZONEPOOL_H

#include <windows.h>

#include <static_assert.h>

#pragma pack(push, 1)
struct GangZone
{
    float rectangle[4];
    DWORD color, flashColor;
};
class CGangZonePool
{
    GangZone* zones[1024];
    DWORD active[1024];

public:
    // 0x00002120
    CGangZonePool();
    // 0x00002150
    ~CGangZonePool();
    // 0x00002180
    void Create(WORD id, float minX, float minY, float maxX, float maxY, DWORD color);
    // 0x00002200
    void StartFlashing(WORD id, DWORD color);
    // 0x00002220
    void StopFlashing(WORD id);
    // 0x00002240
    void Delete(WORD id);
    // 0x00002270
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(GangZone) == 24);
STATIC_ASSERT(sizeof(CGangZonePool) == 0x2000);
// R5 address to verify
extern BYTE g_gangZoneFlashPhase;
// R5 address to verify
extern DWORD g_gangZoneFlashTick;

#endif // GANGZONEPOOL_H
