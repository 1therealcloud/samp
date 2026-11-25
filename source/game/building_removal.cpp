#include "game/building_removal.h"

#include "game/hooks.h"
#include "game/util.h"

#include <math.h>
#include <string.h>

const float g_buildingRemovalDrop = 2000.0f;

const float g_buildingLodCoordinateScale = 0.25f;

BuildingRemovalRecord g_buildingRemovalRecords[1000];

int g_buildingRemovalCount;

void GameHideObjectEntity(void* entity)
{
    BYTE* object = static_cast<BYTE*>(entity);
    const float z = *reinterpret_cast<float*>(object + 0x0c);
    object[0x2f] = 1;
    *reinterpret_cast<float*>(object + 0x0c) = z - g_buildingRemovalDrop;

    BYTE* matrix = *reinterpret_cast<BYTE**>(object + 0x14);
    if (matrix)
        *reinterpret_cast<float*>(matrix + 0x38) -= g_buildingRemovalDrop;
}

void GameHideBuildingEntity(void* entity)
{
    if (entity)
        GameHideObjectEntity(entity);
}

void GameRemoveLodBuildings(float x, float y, float z, float radius)
{
    // GTA SA virtual addresses: count0x00c73f98, inline18-byte records0x00c73fa0.
    int count = *(int*)0xc73f98;
    BYTE* record = (BYTE*)0xc73fa0;
    for (int i = 0; i < count; i++, record += 18)
    {
        float px = (float)*(short*)record * g_buildingLodCoordinateScale;
        float py = (float)*(short*)(record + 2) * g_buildingLodCoordinateScale;
        float pz = (float)*(short*)(record + 4) * g_buildingLodCoordinateScale;
        if (GameDistanceBetweenPoints(px, py, pz, x, y, z) < radius)
            for (int word = 0; word < 6; ++word)
                ((short*)record)[word] = 0;
    }
}

void GameRemoveBuildingsInRange(int model, float x, float y, float z, float radius)
{
    BYTE* pool = (BYTE*)GamePoolBuildingGetArray();
    GameRemoveLodBuildings(x, y, z, 500.0f);
    for (int kind = 0; kind < 3; kind++)
    {
        if (kind == 1)
            pool = (BYTE*)GamePoolDummyGetArray();
        else if (kind == 2)
            pool = (BYTE*)GamePoolObjectGetArray();
        int count = kind == 0 ? 20000 : kind == 1 ? 16000 : 3100;
        int stride = kind == 2 ? 0x19c : 0x38;
        for (int i = 0; i < count; i++, pool += stride)
        {
            DWORD vtable = *(DWORD*)pool;
            // GTA SA virtual address0x00863c40 identifies the skipped backend type.
            if (!vtable || vtable == 0x863c40)
                continue;
            if ((int)*(short*)(pool + 0x22) != model && model != -1)
                continue;
            BYTE* matrix = *(BYTE**)(pool + 0x14);
            float* point = (float*)(matrix ? matrix + 0x30 : pool + 4);
            if (GameDistanceBetweenPoints(point[0], point[1], point[2], x, y, z) <= radius)
            {
                if (kind == 2)
                    GameHideObjectEntity(pool);
                else
                    GameHideBuildingEntity(pool);
            }
        }
    }
}

void GameRemoveBuilding(int model, float x, float y, float z, float radius)
{
    GameRemoveBuildingsInRange(model, x, y, z, radius);
    int count = g_buildingRemovalCount;
    BuildingRemovalRecord* record = (BuildingRemovalRecord*)((BYTE*)g_buildingRemovalRecords + (DWORD)count * 20);
    record->model = model;
    memcpy(&record->x, &x, 4);
    memcpy(&record->y, &y, 4);
    memcpy(&record->z, &z, 4);
    memcpy(&record->radius, &radius, 4);
    g_buildingRemovalCount = (int)((DWORD)count + 1);
}
// Keep the accumulation order for radius-boundary comparisons.

float __stdcall GameDistanceBetweenPoints(float x, float y, float z, float otherX, float otherY, float otherZ)
{
    const float dx = x - otherX;
    const float dy = y - otherY;
    const float dz = z - otherZ;
    return (float)sqrt(dz * dz + dy * dy + dx * dx);
}

BOOL GameShouldSuppressMarker(int model, float x, float y, float z)
{
    if (g_buildingRemovalCount)
    {
        int index = 0;
        do
        {
            const BuildingRemovalRecord& entry = g_buildingRemovalRecords[index];
            if (entry.model == -1 || entry.model == model)
            {
                if (GameDistanceBetweenPoints(x, y, z, entry.x, entry.y, entry.z) <= entry.radius)
                    return TRUE;
            }
            ++index;
        }
        while (index != g_buildingRemovalCount);
    }
    return FALSE;
}

void GameHideAllWorldEntities()
{
    BYTE* cursor = (BYTE*)GamePoolBuildingGetArray();
    for (int count = 20000; count; --count, cursor += 56)
        if (*(DWORD*)cursor && *(DWORD*)cursor != 0x863c40)
            GameHideBuildingEntity(cursor);
    cursor = (BYTE*)GamePoolDummyGetArray();
    for (int count = 16000; count; --count, cursor += 56)
        if (*(DWORD*)cursor && *(DWORD*)cursor != 0x863c40)
            GameHideBuildingEntity(cursor);
    cursor = (BYTE*)GamePoolObjectGetArray();
    for (int count = 3100; count; --count, cursor += 412)
        if (*(DWORD*)cursor && *(DWORD*)cursor != 0x863c40)
            GameHideObjectEntity(cursor);
    g_hookSuppressAllMarkers = 1;
}
