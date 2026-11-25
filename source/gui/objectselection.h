#ifndef OBJECTSELECTION_H
#define OBJECTSELECTION_H

#include <windows.h>

#pragma pack(push, 1)
class CObjectSelection
{
public:
    DWORD enabled;
    WORD objectId;

    WORD DefineObject();

    void DrawLabels();

    void SendNotification();

    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);

    void Enable(BOOL enabled);

    CObjectSelection();

    void Draw();
};
#pragma pack(pop)
static_assert(sizeof(CObjectSelection) == 6);

extern const float g_objectSelectionMaxDistance;

extern const char g_objectSelectionLabelFormat[36];

extern int g_objectSelectionRpc;

#endif // OBJECTSELECTION_H
