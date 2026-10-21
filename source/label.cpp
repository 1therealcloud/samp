#include "label.h"
#include "gui/device_objects.h"
#include "game/aimstuff.h"
#include "game/scripting.h"
#include "game/quaternion.h"
#include "d3dhook/IDirect3DDevice9Hook.h"

// samp.dll RVA: 0x0011c640
const Scripting::OpcodeInfo g_labelLineOfSightCommand={0x6bd,"ffffffiiiii"};

// samp.dll RVA: 0x0006b5f0
CLabel::CLabel(IDirect3DDevice9 *value) { device=value;D3DXCreateSprite(device,&sprite); }

// samp.dll RVA: 0x0006b630
void CLabel::OnLostDevice() { if(sprite)sprite->OnLostDevice(); }

// samp.dll RVA: 0x0006b640
void CLabel::OnResetDevice() { if(sprite)sprite->OnResetDevice(); }

// Descriptive recovery name; original method spelling is not established.
// samp.dll RVA: 0x0006b650
BOOL CLabel::HasLineOfSight(float x,float y,float z) {
    CameraAim *aim=AimStuff::GetInternalAim();
    if(aim && Scripting::ProcessCommand(&g_labelLineOfSightCommand,
        x,y,z,aim->source.x,aim->source.y,aim->source.z,
        1,0,0,1,0))return TRUE;
    return FALSE;
}

// samp.dll RVA: 0x0006b6b0
void CLabel::Begin() { if(sprite)sprite->Begin(D3DXSPRITE_ALPHABLEND); }

// samp.dll RVA: 0x0006b6c0
void CLabel::End() { if(sprite)sprite->End(); }

// samp.dll RVA: 0x0006b6d0
void CLabel::Draw(const CVector *position,const char *text,DWORD color,BOOL shadow,BOOL status) {
    if(!device)return;
    // Native tests only the low byte of the fifth argument.
    if((BYTE)status && !HasLineOfSight(position->x,position->y,position->z))return;
    D3DVIEWPORT9 viewport;device->GetViewport(&viewport);
    D3DXMATRIX identity;D3DXMatrixIdentity(&identity);
    D3DXVECTOR3 projected;
    D3DXVec3Project(&projected,(const D3DXVECTOR3*)position,&viewport,
        (const D3DXMATRIX*)&g_deviceProjectionMatrix,
        (const D3DXMATRIX*)&g_deviceViewMatrix,&identity);
    if(projected.z>g_quaternionMatrixOne)return;
    int x=(int)projected.x,y=(int)projected.y;
    RECT rect={x,y,x+1,y+1};
    g_deviceFonts->DrawLittleText(sprite,text,rect,0x101,color,shadow);
}
