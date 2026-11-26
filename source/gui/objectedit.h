#ifndef OBJECTEDIT_H
#define OBJECTEDIT_H

#include <d3dx9.h>

class CMatrix;
struct CVector;
#pragma pack(push, 1)
class CObjectEdit
{
public:
    WORD GetObjectId();
    BYTE controlLayout[0x78];
    DWORD editTarget, editMode, editing, controlsVisible;
    WORD objectId;
    int attachedObjectSlot;
    BOOL playerObject;
    BYTE objectRotation[12];
    DWORD lastTick;
    BYTE flags[5], unknowna7[0x5c];
    IDirect3DDevice9* device;
    ID3DXLine* line;
    ID3DXFont *smallFont, *largeFont;
    DWORD hoveredControl;

    static const char* GetMaxSizeChar();
    // Selector is an axis (0..2), not a boolean font selector.

    const char* GetRenderChar(int axis);

    void CenterButtonRect(RECT* out, const POINT* center);

    void ClearButtonRect(RECT* out);

    void EnterEditObject(WORD id, BOOL playerObject);

    void EnterEditAttachedObject(int index);

    void DrawButton(int processType, RECT rect);

    BOOL RenderControlsForObject(const CMatrix* matrix, float axisSize);
    // Native forwarding entry to RenderControlsForObject.

    BOOL RenderControls(const CMatrix* matrix, float axisSize);

    float WorldToScreen(const CVector* position, float* screen);

    int RenderAxes(const CMatrix* matrix, float axisSize);

    void TryChangeProcessType();

    void SetEditMode(int mode);

    void ResetMousePos();

    void ApplyChanges(int axis, float difference);

    void ApplyObjectChanges(int axis, float difference);

    void ApplyAttachedChanges(int axis, float difference);

    void ProcessMouseMove();

    void SendAttachedEditEndNotification(int status);

    void SendEditEndNotification(int status);

    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);

    void Disable(BOOL send);

    CObjectEdit(IDirect3DDevice9* device);

    void Render();
};
#pragma pack(pop)
static_assert(sizeof(CObjectEdit) == 0x117);
static_assert(offsetof(CObjectEdit, attachedObjectSlot) == 0x8a);
static_assert(offsetof(CObjectEdit, playerObject) == 0x8e);
static_assert(offsetof(CObjectEdit, objectRotation) == 0x92);
// Output stores may alias. API failures are ignored.

void __cdecl ObjectEditBuildCursorRay(float rayLength, CVector* origin, CVector* end);

extern const float g_cursorRayUnsignedBias;

extern const char g_objectEditGlyphMax[4];

extern const char g_objectEditGlyphXPosition[4];

extern const char g_objectEditGlyphXRotation[4];

extern const char g_objectEditGlyphXScale[4];

extern const char g_objectEditGlyphYPosition[4];

extern const char g_objectEditGlyphYRotation[4];

extern const char g_objectEditGlyphYScale[4];

extern const char g_objectEditGlyphZPosition[4];

extern const char g_objectEditGlyphZRotation[4];

extern const char g_objectEditGlyphZScale[4];

extern const char g_objectEditGlyphDefault[4];

extern const char g_objectEditGlyphScale[4];

extern const char g_objectEditGlyphSave[4];

extern const float g_objectEditDistanceScale;

extern int g_objectEditRpc;

extern int g_objectEditAttachedRpc;

#endif // OBJECTEDIT_H
