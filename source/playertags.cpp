#include "playertags.h"
#include "deathwindow.h"
#include "fontrender.h"
#include "gui/device_objects.h"
#include "gui/device_lifecycle.h"
#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/quaternion.h"
#include "chatbubble.h"
#include "game/aimstuff.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/vehicle.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "main.h"
#include <new>
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x0017e988
CMatrix g_nametagVehicleMatrix;
// samp.dll RVA: 0x0017eb08
char g_nametagName[128];
// samp.dll RVA: 0x0017eb88
CMatrix g_nametagLocalMatrix;
// samp.dll RVA: 0x0017ebcc
CVector g_nametagBone;
// samp.dll RVA: 0x0017ebf4
CVector g_nametagDrawPosition;
// samp.dll RVA: 0x0011c7a4
const Scripting::OpcodeInfo g_nametagLineOfSightCommand={0x6bd,"ffffffiiiii"};
// samp.dll RVA: 0x0011c7b8
const char g_nametagNameFormat[8]="%s (%d)";
// samp.dll RVA: 0x0011c670
const float g_playerTagDistanceHeight=0.0475f;
// samp.dll RVA: 0x00118388
const char g_playerTagStatusBack[2]="C";
// samp.dll RVA: 0x00118370
const char g_playerTagStatusFront[2]="E";
// samp.dll RVA: 0x0006d090
void CPlayerTags::OnLostDevice() {
    if(state){state->Release();state=0;}
    if(sprite)sprite->OnLostDevice();
}
// samp.dll RVA: 0x0006d0c0
void CPlayerTags::OnResetDevice() {
    if(sprite)sprite->OnResetDevice();
    if(!state)device->CreateStateBlock(D3DSBT_ALL,&state);
}



// samp.dll RVA: 0x0006c730
CPlayerTags::CPlayerTags(IDirect3DDevice9 *value) { device=value;state=0;sprite=0;D3DXCreateSprite(device,&sprite); }

// samp.dll RVA: 0x0006c760
CPlayerTags::~CPlayerTags() {
    if(state)state->Release();
    // Original uses scalar delete storage release, not COM Release.
    if(sprite){::operator delete(sprite);sprite=0;}
}
// samp.dll RVA: 0x0006c790
void CPlayerTags::EndHealthBar() {
    if(sprite)sprite->End();
    if(state)state->Apply();
}
// samp.dll RVA: 0x0006c7c0
void CPlayerTags::BeginLabel() {if(sprite)sprite->Begin(0x10);}
// samp.dll RVA: 0x0006c7d0
void CPlayerTags::EndLabel() {if(sprite)sprite->End();}
// samp.dll RVA: 0x0006c7e0
void CPlayerTags::DrawLabel(const CVector *position,const char *text,DWORD color,float distance,BYTE drawStatus,int status) {
    CVector point;memcpy(&point,position,12);
    float *height=&point.z;
    __asm {
        fld distance
        fmul g_playerTagDistanceHeight
        mov eax,height
        fadd dword ptr [eax]
        fadd g_chatBubbleHeightLift
        fstp dword ptr [eax]
    }
    D3DVIEWPORT9 viewport;device->GetViewport(&viewport);
    D3DXMATRIX identity;D3DXMatrixIdentity(&identity);
    D3DXVECTOR3 projected;
    D3DXVec3Project(&projected,(D3DXVECTOR3*)&point,&viewport,
        (D3DXMATRIX*)&g_deviceProjectionMatrix,(D3DXMATRIX*)&g_deviceViewMatrix,&identity);
    BYTE visible;float *depth=&projected.z;
    __asm {
        mov eax,depth
        fld dword ptr [eax]
        fcomp g_quaternionMatrixOne
        fnstsw ax
        test ah,41h
        setnz visible
    }
    // Native accepts unordered projected depth, ignores viewport/projection failures.
    if(!visible)return;
    int x=(int)projected.x,y=(int)projected.y;
    FontSize size;g_deviceFonts->GetLittleTextScreenSize(&size,text,0);
    RECT rect={x-size.cx/2,y+17-size.cy,x+size.cx/2,y};
    g_deviceFonts->DrawLittleText(sprite,text,rect,0x100,color,TRUE);
    if(!drawStatus)return;
    CDeathWindow *window=(CDeathWindow*)g_deviceDeathWindow;
    if(!window)return;
    if(!*(DWORD*)(window->nativeState+0x14b)) {
        window->CreateAuxiliaryFonts();window=(CDeathWindow*)g_deviceDeathWindow;
    }
    ID3DXFont *front=*(ID3DXFont**)(window->nativeState+0x14f);
    ID3DXFont *back=*(ID3DXFont**)(window->nativeState+0x153);
    if(!front||!back)return;
    RECT bounds={0,0,0,0};back->DrawTextA(0,g_playerTagStatusBack,1,&bounds,0x505,0);
    int left=x-22-(bounds.right-bounds.left)/2;
    int top=y+17+(bounds.bottom-bounds.top)/2;
    RECT badge={left,top,left+1,top+2};
    if(status==2) {
        back->DrawTextA(0,g_playerTagStatusBack,1,&badge,0x105,0xff000000);
        badge.top-=2;badge.left-=1;
        front->DrawTextA(0,g_playerTagStatusFront,1,&badge,0x105,0xffe3e1e3);
    }
}

// samp.dll RVA: 0x00074dc0
void ClientDrawLabels() {
    if(!g_pNetGame||!(*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x38])return;
    g_devicePlayerTags->BeginLabel();
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CPlayerPool *players=*(CPlayerPool**)(pools+8);
    g_pGame->GetPlayerPed()->GetMatrix(&g_nametagLocalMatrix);
    int highest=*(int*)((BYTE*)players+0x22);
    for(DWORD counter=0;(int)counter<=highest;counter++) {
        WORD id=(WORD)counter;
        if(id>=1004||*(DWORD*)((BYTE*)players+0xfd6+id*4)!=1)continue;
        BYTE *entry=*(BYTE**)((BYTE*)players+0x26+id*4);
        if(!entry)continue;
        CRemotePlayer *remote=*(CRemotePlayer**)(entry+8);
        if(!remote)continue;
        BYTE *remoteState=(BYTE*)remote;
        CPed *ped=*(CPed**)(remoteState+4);
        if(!ped||!remoteState[0x1a]||!*(DWORD*)(remoteState+0x10))continue;
        BYTE nearby;
        // Keep the distance in ST0 and reload settings after the callee.
        __asm {
            mov ecx,ped
            call CEntity::GetDistanceToCamera
            mov ecx,g_pNetGame
            mov edx,dword ptr [ecx+3d5h]
            fcomp dword ptr [edx+27h]
            fnstsw ax
            test ah,41h
            setnp nearby
        }
        if(!nearby)continue;
        CVehicle *vehicle=*(CVehicle**)(remoteState+8);
        if(remoteState[0x1a]==0x13&&vehicle&&vehicle->IsRC()) {
            vehicle=*(CVehicle**)(remoteState+8);
            vehicle->GetMatrix(&g_nametagVehicleMatrix);
            memcpy(&g_nametagBone,&g_nametagVehicleMatrix.position,12);
        } else {
            if(!ped->DoesExist())continue;
            memset(&g_nametagBone,0,12);
            ped->GetBonePosition(8,&g_nametagBone);
        }
        memcpy(&g_nametagDrawPosition,&g_nametagBone,12);
        CameraAim *aim=AimStuff::GetInternalAim();
        DWORD visible=0;
        if((*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2f])
            visible=Scripting::ProcessCommand(&g_nametagLineOfSightCommand,
                g_nametagDrawPosition.x,g_nametagDrawPosition.y,g_nametagDrawPosition.z,
                aim->source.x,aim->source.y,aim->source.z,1,0,0,1,0);
        if((*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2f]&&!visible)continue;
        sprintf(g_nametagName,g_nametagNameFormat,players->GetName(id),(int)counter);
        if(*(DWORD*)(remoteState+0x1c))continue;
        int status=remote->GetStatus();
        BYTE drawStatus=((BYTE*)g_pNetGame)[0x234];
        float distance=ped->GetDistanceToCamera();
        DWORD color=remote->GetColorAsARGB();
        g_devicePlayerTags->DrawLabel(&g_nametagDrawPosition,g_nametagName,color,distance,drawStatus,status);
    }
    g_devicePlayerTags->EndLabel();
}
