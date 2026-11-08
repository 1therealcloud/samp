#ifndef LABELPOOL_H
#define LABELPOOL_H

#include <stddef.h>

#include <static_assert.h>
#include "game/entity.h"
#pragma pack(push, 1)
struct TextLabel
{
    char* text;
    DWORD color;
    CVector position;
    float drawDistance;
    BYTE behindWalls;
    WORD attachedPlayer, attachedVehicle;
};
class CLabelPool
{
public:
    // Returns the raw activity DWORD; out-of-range IDs return zero.
    // 0x0000e3c0
    DWORD IsActive(WORD id);
    TextLabel labels[2048];
    DWORD active[2048];
    // 0x00001190
    CLabelPool();
    // 0x000015e0
    ~CLabelPool();
    // Native color input is RGBA; attachment flags/IDs are stored raw.
    // 0x000011d0
    void Create(WORD id, const char* text, DWORD color, CVector position, float drawDistance, BYTE behindWalls, WORD player, WORD vehicle);
    // 0x000012e0
    BOOL Delete(WORD id);
    // 0x00001350
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(TextLabel) == 29);
STATIC_ASSERT(offsetof(TextLabel, attachedPlayer) == 0x19);
STATIC_ASSERT(offsetof(CLabelPool, active) == 0xe800);
STATIC_ASSERT(sizeof(CLabelPool) == 0x10800);

#endif // LABELPOOL_H
