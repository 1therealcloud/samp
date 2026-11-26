#ifndef GANGZONEPOOL_H
#define GANGZONEPOOL_H

#include <windows.h>

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
    CGangZonePool();

    ~CGangZonePool();

    void Create(WORD id, float minX, float minY, float maxX, float maxY, DWORD color);

    void StartFlashing(WORD id, DWORD color);

    void StopFlashing(WORD id);

    void Delete(WORD id);

    void Draw();
};
static_assert(sizeof(GangZone) == 24);
static_assert(sizeof(CGangZonePool) == 0x2000);

extern BYTE g_gangZoneFlashPhase;

extern DWORD g_gangZoneFlashTick;

#endif // GANGZONEPOOL_H
