#ifndef SAMP_OBJECT_SELECTION_H
#define SAMP_OBJECT_SELECTION_H
#include <windows.h>
#pragma pack(push,1)
class CObjectSelection {public:
    DWORD enabled;WORD objectId;
    // samp.dll RVA: 0x0006d880
    BOOL MsgProc(UINT message,WPARAM wparam,LPARAM lparam);
    // samp.dll RVA: 0x0006d5c0
    void Enable(BOOL enabled);
    // samp.dll RVA: 0x0006d440
    CObjectSelection();
    // samp.dll RVA: 0x0006d640
    void Draw();
};
#pragma pack(pop)
typedef char ObjectSelectionExtent[(sizeof(CObjectSelection)==6)?1:-1];
#endif
