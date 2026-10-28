#ifndef SAMP_OBJECT_SELECTION_H
#define SAMP_OBJECT_SELECTION_H
#include <windows.h>
#pragma pack(push, 1)
class CObjectSelection
{
public:
    DWORD enabled;
    WORD objectId;
    // samp.dll RVA: 0x0006d450
    WORD DefineObject();
    // samp.dll RVA: 0x0006d4a0
    void DrawLabels();
    // samp.dll RVA: 0x0006d710
    void SendNotification();
    // samp.dll RVA: 0x0006d880
    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // samp.dll RVA: 0x0006d5c0
    void Enable(BOOL enabled);
    // samp.dll RVA: 0x0006d440
    CObjectSelection();
    // samp.dll RVA: 0x0006d640
    void Draw();
};
#pragma pack(pop)
typedef char ObjectSelectionExtent[(sizeof(CObjectSelection) == 6) ? 1 : -1];
// samp.dll RVA: 0x0011c6b0
extern const float g_objectSelectionMaxDistance;
// samp.dll RVA: 0x0011c68c
extern const char g_objectSelectionLabelFormat[36];
// samp.dll RVA: 0x00117f28
extern int g_objectSelectionRpc;
#endif
