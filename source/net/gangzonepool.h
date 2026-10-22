#ifndef SAMP_GANGZONEPOOL_H
#define SAMP_GANGZONEPOOL_H
#include <windows.h>
#pragma pack(push,1)
struct GangZone {float rectangle[4];DWORD color,flashColor;};
class CGangZonePool {
    GangZone *zones[1024];
    DWORD active[1024];
public:
    // samp.dll RVA: 0x00002150
    CGangZonePool();
    // samp.dll RVA: 0x00002180
    ~CGangZonePool();
    // samp.dll RVA: 0x000021b0
    void Create(WORD id,float minX,float minY,float maxX,float maxY,DWORD color);
    // samp.dll RVA: 0x00002230
    void StartFlashing(WORD id,DWORD color);
    // samp.dll RVA: 0x00002250
    void StopFlashing(WORD id);
    // samp.dll RVA: 0x00002270
    void Delete(WORD id);
    // samp.dll RVA: 0x000022a0
    void Draw();
};
#pragma pack(pop)
typedef char NativeGangZoneRecordExtent[(sizeof(GangZone)==24)?1:-1];
typedef char NativeGangZonePoolExtent[(sizeof(CGangZonePool)==0x2000)?1:-1];
// samp.dll RVA: 0x001569a0
extern BYTE g_gangZoneFlashPhase;
// samp.dll RVA: 0x001569a4
extern DWORD g_gangZoneFlashTick;
#endif
