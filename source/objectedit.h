#ifndef SAMP_OBJECT_EDIT_H
#define SAMP_OBJECT_EDIT_H
#include <d3dx9.h>
class CMatrix;
struct CVector;
#pragma pack(push, 1)
class CObjectEdit
{
public:
    // 0x00017850
    WORD GetObjectId();
    BYTE unknown00[0x78];
    DWORD unknown78, unknown7c, unknown80, unknown84;
    WORD objectId;
    BYTE unknown8a[0x14];
    DWORD lastTick;
    BYTE flags[5], unknowna7[0x5c];
    IDirect3DDevice9* device;
    ID3DXLine* line;
    ID3DXFont *smallFont, *largeFont;
    DWORD unknown113;
    // 0x00071a30
    static const char* GetMaxSizeChar();
    // Selector is an axis (0..2), not a boolean font selector.
    // 0x00071a40
    const char* GetRenderChar(int axis);
    // Descriptive names for native rectangle helpers.
    // 0x00071ae0
    void CenterButtonRect(RECT* out, const POINT* center);
    // 0x00071b20
    void ClearButtonRect(RECT* out);
    // 0x00071ec0
    void EnterEditObject(WORD id, BOOL playerObject);
    // 0x00071f80
    void EnterEditAttachedObject(int index);
    // Descriptive name; rectangle is passed by value (native RET20).
    // 0x00071fe0
    void DrawButton(int processType, RECT rect);
    // 0x000726d0
    BOOL RenderControlsForObject(const CMatrix* matrix, float axisSize);
    // Native forwarding entry to RenderControlsForObject.
    // 0x00072ca0
    BOOL RenderControls(const CMatrix* matrix, float axisSize);
    // 0x000716c0
    float WorldToScreen(const CVector* position, float* screen);
    // 0x000717c0
    int RenderAxes(const CMatrix* matrix, float axisSize);
    // 0x00071b40
    void TryChangeProcessType();
    // 0x00071c90
    void SetEditMode(int mode);
    // 0x00071e60
    void ResetMousePos();
    // 0x00072f00
    void ApplyChanges(int axis, float difference);
    // 0x00072cb0
    void ApplyObjectChanges(int axis, float difference);
    // 0x00071cb0
    void ApplyAttachedChanges(int axis, float difference);
    // 0x00072f20
    void ProcessMouseMove();
    // 0x00072560
    void SendAttachedEditEndNotification(int status);
    // 0x00072350
    void SendEditEndNotification(int status);
    // 0x00072ff0
    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x00072660
    void Disable(BOOL send);
    // 0x00071600
    CObjectEdit(IDirect3DDevice9* device);
    // 0x00073220
    void Render();
};
#pragma pack(pop)
typedef char ObjectEditExtent[(sizeof(CObjectEdit) == 0x117) ? 1 : -1];
// Descriptive helper name. Output stores may alias, and API failures are ignored.
// 0x000739e0
void __cdecl ObjectEditBuildCursorRay(float rayLength, CVector* origin, CVector* end);
// 0x0011a7ac
extern const float g_cursorRayUnsignedBias;
// 0x00118388
extern const char g_objectEditGlyphMax[4];
// 0x0011c340
extern const char g_objectEditGlyphXPosition[4];
// 0x0011c33c
extern const char g_objectEditGlyphXRotation[4];
// 0x00118394
extern const char g_objectEditGlyphXScale[4];
// 0x00118304
extern const char g_objectEditGlyphYPosition[4];
// 0x00117880
extern const char g_objectEditGlyphYRotation[4];
// 0x00118308
extern const char g_objectEditGlyphYScale[4];
// 0x001183e0
extern const char g_objectEditGlyphZPosition[4];
// 0x00117950
extern const char g_objectEditGlyphZRotation[4];
// 0x001184c0
extern const char g_objectEditGlyphZScale[4];
// 0x001182d8
extern const char g_objectEditGlyphDefault[4];
// 0x0011c344
extern const char g_objectEditGlyphScale[4];
// 0x00118350
extern const char g_objectEditGlyphSave[4];
// 0x0011c768
extern const float g_objectEditDistanceScale;
// 0x00117f2c
extern int g_objectEditRpc;
// 0x00117f30
extern int g_objectEditAttachedRpc;
#endif
