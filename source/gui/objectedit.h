#ifndef OBJECTEDIT_H
#define OBJECTEDIT_H

#include <d3dx9.h>

#include <static_assert.h>

class CMatrix;
struct CVector;
#pragma pack(push, 1)
class CObjectEdit
{
public:
    // R5 address to verify
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
    // 0x00071f90
    static const char* GetMaxSizeChar();
    // Selector is an axis (0..2), not a boolean font selector.
    // 0x00071fa0
    const char* GetRenderChar(int axis);
    // 0x00072040
    void CenterButtonRect(RECT* out, const POINT* center);
    // 0x00072080
    void ClearButtonRect(RECT* out);
    // 0x00072420
    void EnterEditObject(WORD id, BOOL playerObject);
    // 0x000724e0
    void EnterEditAttachedObject(int index);
    // 0x00072540
    void DrawButton(int processType, RECT rect);
    // 0x00072c30
    BOOL RenderControlsForObject(const CMatrix* matrix, float axisSize);
    // Native forwarding entry to RenderControlsForObject.
    // 0x00073200
    BOOL RenderControls(const CMatrix* matrix, float axisSize);
    // 0x00071c20
    float WorldToScreen(const CVector* position, float* screen);
    // 0x00071d20
    int RenderAxes(const CMatrix* matrix, float axisSize);
    // 0x000720a0
    void TryChangeProcessType();
    // 0x000721f0
    void SetEditMode(int mode);
    // 0x000723c0
    void ResetMousePos();
    // 0x00073460
    void ApplyChanges(int axis, float difference);
    // 0x00073210
    void ApplyObjectChanges(int axis, float difference);
    // 0x00072210
    void ApplyAttachedChanges(int axis, float difference);
    // 0x00073480
    void ProcessMouseMove();
    // 0x00072ac0
    void SendAttachedEditEndNotification(int status);
    // 0x000728b0
    void SendEditEndNotification(int status);
    // 0x00073550
    BOOL MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x00072bc0
    void Disable(BOOL send);
    // 0x00071b60
    CObjectEdit(IDirect3DDevice9* device);
    // 0x00073780
    void Render();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CObjectEdit) == 0x117);
// Output stores may alias. API failures are ignored.
// 0x00073f40
void __cdecl ObjectEditBuildCursorRay(float rayLength, CVector* origin, CVector* end);
// 0x000e871c
extern const float g_cursorRayUnsignedBias;
// 0x000e6304
extern const char g_objectEditGlyphMax[4];
// 0x000ea318
extern const char g_objectEditGlyphXPosition[4];
// 0x000ea314
extern const char g_objectEditGlyphXRotation[4];
// 0x000e6310
extern const char g_objectEditGlyphXScale[4];
// 0x000e6280
extern const char g_objectEditGlyphYPosition[4];
// 0x000e5810
extern const char g_objectEditGlyphYRotation[4];
// 0x000e6284
extern const char g_objectEditGlyphYScale[4];
// 0x000e635c
extern const char g_objectEditGlyphZPosition[4];
// 0x000e58e0
extern const char g_objectEditGlyphZRotation[4];
// R5 address to verify
extern const char g_objectEditGlyphZScale[4];
// R5 address to verify
extern const char g_objectEditGlyphDefault[4];
// 0x000ea31c
extern const char g_objectEditGlyphScale[4];
// R5 address to verify
extern const char g_objectEditGlyphSave[4];
// R5 address to verify
extern const float g_objectEditDistanceScale;
// 0x000e5ea0
extern int g_objectEditRpc;
// 0x000e5ea4
extern int g_objectEditAttachedRpc;

#endif // OBJECTEDIT_H
