#include "net/gangzonepool.h"
#include "game/game.h"
#include <stdlib.h>
#include <string.h>
// Native record allocation is malloc; release calls operator delete.
// 0x001569a0
BYTE g_gangZoneFlashPhase = 0;
// 0x001569a4
DWORD g_gangZoneFlashTick = 0;
// 0x00002150
CGangZonePool::CGangZonePool()
{
    for (WORD i = 0; i < 1024; ++i)
    {
        zones[i] = 0;
        active[i] = 0;
    }
}
// 0x00002180
CGangZonePool::~CGangZonePool()
{
    for (WORD i = 0; i < 1024; ++i)
        if (zones[i])
        {
            delete zones[i];
            zones[i] = 0;
        }
}
// 0x000021b0
void CGangZonePool::Create(WORD id, float minX, float minY, float maxX, float maxY, DWORD color)
{
    if (zones[id])
    {
        delete zones[id];
        zones[id] = 0;
        active[id] = 0;
    }
    GangZone* zone = (GangZone*)malloc(24);
    if (!zone)
        return;
    memcpy(zone->rectangle, &minX, 4);
    memcpy(zone->rectangle + 1, &maxY, 4);
    memcpy(zone->rectangle + 2, &maxX, 4);
    memcpy(zone->rectangle + 3, &minY, 4);
    zone->color = color;
    zone->flashColor = color;
    zones[id] = zone;
    active[id] = 1;
}
// 0x00002230
void CGangZonePool::StartFlashing(WORD id, DWORD color)
{
    if (zones[id])
        zones[id]->flashColor = color;
}
// 0x00002250
void CGangZonePool::StopFlashing(WORD id)
{
    if (zones[id])
        zones[id]->flashColor = zones[id]->color;
}
// 0x00002270
void CGangZonePool::Delete(WORD id)
{
    if (zones[id])
    {
        delete zones[id];
        zones[id] = 0;
        active[id] = 0;
    }
}
// 0x000022a0
void CGangZonePool::Draw()
{
    DWORD now = GetTickCount();
    if ((LONG)(now - g_gangZoneFlashTick) >= 500)
    {
        g_gangZoneFlashPhase = (BYTE)~g_gangZoneFlashPhase;
        g_gangZoneFlashTick = now;
    }
    for (WORD i = 0; i < 1024; ++i)
        if (active[i])
        {
            GangZone* zone = zones[i];
            g_pGame->DrawGangZone(zone->rectangle, g_gangZoneFlashPhase ? zone->flashColor : zone->color);
        }
}
