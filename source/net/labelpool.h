#ifndef LABELPOOL_H
#define LABELPOOL_H

#include "game/entity.h"

#include <stddef.h>

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

    DWORD IsActive(WORD id);
    TextLabel labels[2048];
    DWORD active[2048];

    CLabelPool();

    ~CLabelPool();
    // Native color input is RGBA; attachment flags/IDs are stored raw.

    void Create(WORD id, const char* text, DWORD color, CVector position, float drawDistance, BYTE behindWalls, WORD player, WORD vehicle);

    BOOL Delete(WORD id);

    void Draw();
};
#pragma pack(pop)
static_assert(sizeof(TextLabel) == 29);
static_assert(offsetof(TextLabel, attachedPlayer) == 0x19);
static_assert(offsetof(CLabelPool, active) == 0xe800);
static_assert(sizeof(CLabelPool) == 0x10800);

#endif // LABELPOOL_H
