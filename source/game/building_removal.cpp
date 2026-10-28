#include "game/building_removal.h"
#include "game/util.h"
#include <string.h>
// 0x0011e338
const float g_buildingRemovalDrop = 2000.0f;
// 0x0011e33c
const float g_buildingLodCoordinateScale = 0.25f;
// 0x0009ca80
void GameHideObjectEntity(void* entity)
{
    // clang-format off
    __asm {
        mov eax,entity
        fld dword ptr [eax+0ch]
        mov byte ptr [eax+2fh],1
        fsub g_buildingRemovalDrop
        fstp dword ptr [eax+0ch]
        mov eax,dword ptr [eax+14h]
        test eax,eax
        jz hidden
        fld dword ptr [eax+38h]
        fsub g_buildingRemovalDrop
        fstp dword ptr [eax+38h]
    hidden:
    }
    // clang-format on
}
// 0x0009cab0
void GameHideBuildingEntity(void* entity)
{
    if (entity)
        GameHideObjectEntity(entity);
}
// 0x0009cb50
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
        BYTE inside;
        // clang-format off
        __asm {
            push z
            push y
            push x
            push pz
            push py
            push px
            call GameDistanceBetweenPoints
            fcomp radius
            fnstsw ax
            test ah,5
            setnp inside
        }
        // clang-format on
        if (inside) for (int word = 0; word < 6; word++)((short*)record)[word] = 0;
    }
}
// 0x0009cc10
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
            BYTE inside;
            // clang-format off
            __asm {
                push z
                push y
                push x
                mov edx,point
                push dword ptr [edx+8]
                push dword ptr [edx+4]
                push dword ptr [edx]
                call GameDistanceBetweenPoints
                fcomp radius
                fnstsw ax
                test ah,41h
                setnp inside
            }
            // clang-format on
            if (inside)
            {
                if (kind == 2)
                    GameHideObjectEntity(pool);
                else
                    GameHideBuildingEntity(pool);
            }
        }
    }
}
// 0x0009d210
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
// 0x00188df8
BuildingRemovalRecord g_buildingRemovalRecords[1000];
// 0x0018dc18
int g_buildingRemovalCount;
// Native ST0 returns an unspilled x87 result. Order is dz^2 + dy^2 + dx^2;
// reassociation or a float temporary changes radius-boundary comparisons.
// 0x000b4570
__declspec(naked) float __stdcall GameDistanceBetweenPoints(float x, float y, float z, float otherX, float otherY, float otherZ)
{
    // clang-format off
    __asm {
        fld dword ptr [esp+4]
        fsub dword ptr [esp+16]
        fld dword ptr [esp+8]
        fsub dword ptr [esp+20]
        fld dword ptr [esp+12]
        fsub dword ptr [esp+24]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        ret 24
    }
    // clang-format on
}
// 0x0009cae0
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

#include "game/hooks.h"
// 0x0009cdd0
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
