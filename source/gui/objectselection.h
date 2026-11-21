#ifndef OBJECTSELECTION_H
#define OBJECTSELECTION_H

#include <windows.h>

#include <static_assert.h>

#pragma pack(push, 1)
class CObjectSelection
{
public:
    DWORD enabled;
    WORD objectId;
    // 0x0006da10
    WORD DefineObject();
    // 0x0006da60
    void DrawLabels();
    // 0x0006dcd0
    void SendNotification();
    // 0x0006de40
    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x0006db80
    void Enable(BOOL enabled);
    // 0x0006da00
    CObjectSelection();
    // 0x0006dc00
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CObjectSelection) == 6);
// R5 address to verify
extern const float g_objectSelectionMaxDistance;
// 0x000ea664
extern const char g_objectSelectionLabelFormat[36];
// 0x000e5e9c
extern int g_objectSelectionRpc;

#endif // OBJECTSELECTION_H
