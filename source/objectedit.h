#ifndef SAMP_OBJECT_EDIT_H
#define SAMP_OBJECT_EDIT_H
#include <d3dx9.h>
class CMatrix;
struct CVector;
#pragma pack(push,1)
class CObjectEdit {public:
    BYTE unknown00[0x78];DWORD unknown78,unknown7c,unknown80,unknown84;WORD objectId;
    BYTE unknown8a[0x14];DWORD lastTick;BYTE flags[5],unknowna7[0x5c];
    IDirect3DDevice9 *device;ID3DXLine *line;ID3DXFont *smallFont,*largeFont;DWORD unknown113;
    // samp.dll RVA: 0x000716c0
    float WorldToScreen(const CVector *position,float *screen);
    // samp.dll RVA: 0x000717c0
    int RenderAxes(const CMatrix *matrix,float axisSize);
    // samp.dll RVA: 0x00071b40
    void TryChangeProcessType();
    // samp.dll RVA: 0x00071c90
    void SetEditMode(int mode);
    // samp.dll RVA: 0x00071e60
    void ResetMousePos();
    // samp.dll RVA: 0x00072f00
    void ApplyChanges(int axis,float difference);
    // Address-only mutation backends; object/ped integration remains open.
    // samp.dll RVA: 0x00072cb0
    void ApplyObjectChanges(int axis,float difference);
    // samp.dll RVA: 0x00071cb0
    void ApplyAttachedChanges(int axis,float difference);
    // samp.dll RVA: 0x00072f20
    void ProcessMouseMove();
    // samp.dll RVA: 0x00072560
    void SendAttachedEditEndNotification(BOOL send);
    // samp.dll RVA: 0x00072350
    void SendEditEndNotification(BOOL send);
    // samp.dll RVA: 0x00072ff0
    BOOL MsgProc(UINT message,WPARAM wparam,LPARAM lparam);
    // samp.dll RVA: 0x00072660
    void Disable(BOOL send);
    // samp.dll RVA: 0x00071600
    CObjectEdit(IDirect3DDevice9 *device);
    // samp.dll RVA: 0x00073220
    void Render();
};
#pragma pack(pop)
typedef char ObjectEditExtent[(sizeof(CObjectEdit)==0x117)?1:-1];
// Descriptive helper name. Output stores may alias, and API failures are ignored.
// samp.dll RVA: 0x000739e0
void __cdecl ObjectEditBuildCursorRay(float rayLength,CVector *origin,CVector *end);
// samp.dll RVA: 0x0011a7ac
extern const float g_cursorRayUnsignedBias;
#endif
