#ifndef SAMP_LABELPOOL_H
#define SAMP_LABELPOOL_H
#include "game/entity.h"
#include <stddef.h>
#pragma pack(push,1)
struct TextLabel {
    char *text;
    DWORD color;
    CVector position;
    float drawDistance;
    BYTE behindWalls;
    WORD attachedPlayer,attachedVehicle;
};
class CLabelPool {
public:
    TextLabel labels[2048];
    DWORD active[2048];
    // samp.dll RVA: 0x00001190
    CLabelPool();
    // samp.dll RVA: 0x000015e0
    ~CLabelPool();
    // Native color input is RGBA; attachment flags/IDs are stored raw.
    // samp.dll RVA: 0x000011d0
    void Create(WORD id,const char *text,DWORD color,CVector position,float drawDistance,
        BYTE behindWalls,WORD player,WORD vehicle);
    // samp.dll RVA: 0x000012e0
    BOOL Delete(WORD id);
    // samp.dll RVA: 0x00001350
    void Draw();
};
#pragma pack(pop)
typedef char TextLabelExtent[(sizeof(TextLabel)==29)?1:-1];
typedef char TextLabelPlayerOffset[(offsetof(TextLabel,attachedPlayer)==0x19)?1:-1];
typedef char LabelPoolFlagsOffset[(offsetof(CLabelPool,active)==0xe800)?1:-1];
typedef char LabelPoolExtent[(sizeof(CLabelPool)==0x10800)?1:-1];
#endif
