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
#include "game/util.h"
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
// samp.dll RVA: 0x00140878
PlayerTagVertex g_playerTagVertices[12]={
    {0,0,0,1,0xff000000},{0,25,0,1,0xff000000},
    {100,25,0,1,0xff000000},{100,0,0,1,0xff000000},
    {2,2,0,1,0xff4b0b14},{2,18,0,1,0xff4b0b14},
    {96,18,0,1,0xff4b0b14},{96,2,0,1,0xff4b0b14},
    {2,2,0,1,0xffb92228},{2,18,0,1,0xffb92228},
    {96,18,0,1,0xffb92228},{96,2,0,1,0xffb92228}
};
// samp.dll RVA: 0x0011c674
const float g_playerTagFillScale=0.38461539149284363f;
// samp.dll RVA: 0x0011c678
const float g_playerTagInnerBottom=23.0f;
// samp.dll RVA: 0x0011c67c
const float g_playerTagHalfWidth=19.0f;
// samp.dll RVA: 0x0011c680
const float g_playerTagOuterRight=21.0f;
// samp.dll RVA: 0x0011c684
const float g_playerTagOuterBottom=24.0f;
// samp.dll RVA: 0x0011c688
const float g_playerTagOuterTop=18.0f;
// samp.dll RVA: 0x00117874
const float g_playerTagOuterLeft=20.0f;
// samp.dll RVA: 0x0011797c
const float g_playerTagArmourOffset=8.0f;
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
// samp.dll RVA: 0x0006d0f0
void CPlayerTags::BeginHealthBar() {
    if(!state)OnResetDevice();
    if(state)state->Capture();
    if(sprite)sprite->Begin(1);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
    device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
    device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG2);
    device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
    device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
    device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
    device->SetVertexShader(0);
    device->SetPixelShader(0);
    device->SetRenderState(D3DRS_ZENABLE,FALSE);
}

// samp.dll RVA: 0x0006cae0
void CPlayerTags::DrawHealthBar(const CVector *position,float health,float armour,float distance) {
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
    BYTE visible,acceptedHealth;float *depth=&projected.z;
    __asm {
        mov eax,depth
        fld dword ptr [eax]
        fcomp g_quaternionMatrixOne
        fnstsw ax
        test ah,41h
        setnz visible
    }
    if(!visible)return;
    __asm {
        fld health
        fcomp g_objectScaleLowerBound
        fnstsw ax
        test ah,5
        setp acceptedHealth
    }
    // Both gates accept unordered operands; only ordered negative health is rejected.
    if(!acceptedHealth)return;
    float x=(float)(int)projected.x,y=(float)(int)projected.y;
    PlayerTagVertex *border=g_playerTagVertices,*background=border+4,*fill=border+8;
    for(int i=0;i<4;i++){fill[i].color=0xffb92228;background[i].color=0xff4b0b14;}
    border[0].x=border[1].x=x-g_playerTagOuterLeft;
    border[0].y=border[3].y=y+g_playerTagOuterTop;
    border[1].y=border[2].y=y+g_playerTagOuterBottom;
    border[2].x=border[3].x=x+g_playerTagOuterRight;
    background[0].x=background[1].x=fill[0].x=fill[1].x=x-g_playerTagHalfWidth;
    background[0].y=background[3].y=fill[0].y=fill[3].y=y+g_playerTagHalfWidth;
    background[1].y=background[2].y=fill[1].y=fill[2].y=y+g_playerTagInnerBottom;
    background[2].x=background[3].x=x+g_playerTagHalfWidth;
    if(health>g_vehicleTrainSpeedMaximum)health=g_vehicleTrainSpeedMaximum;
    float right;
    __asm {
        fld health
        fmul g_playerTagFillScale
        fsub g_playerTagHalfWidth
        fadd x
        fstp right
    }
    fill[2].x=fill[3].x=right;
    if(armour>g_objectScaleLowerBound) {
        for(i=0;i<4;i++) {
            border[i].y+=g_playerTagArmourOffset;
            background[i].y=fill[i].y+g_playerTagArmourOffset;
            fill[i].y+=g_playerTagArmourOffset;
        }
    }
    device->SetFVF(0x44);
    device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,border,sizeof(PlayerTagVertex));
    device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,background,sizeof(PlayerTagVertex));
    device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,fill,sizeof(PlayerTagVertex));
    if(armour>g_objectScaleLowerBound) {
        for(i=0;i<4;i++){fill[i].color=0xffc8c8c8;background[i].color=0xff282828;}
        for(i=0;i<4;i++) {
            border[i].y-=g_playerTagArmourOffset;
            background[i].y-=g_playerTagArmourOffset;
            fill[i].y-=g_playerTagArmourOffset;
        }
        if(armour>g_vehicleTrainSpeedMaximum)armour=g_vehicleTrainSpeedMaximum;
        __asm {
            fld armour
            fmul g_playerTagFillScale
            fsub g_playerTagHalfWidth
            fadd x
            fstp right
        }
        fill[2].x=fill[3].x=right;
        device->SetFVF(0x44);
        device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,border,sizeof(PlayerTagVertex));
        device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,background,sizeof(PlayerTagVertex));
        device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,fill,sizeof(PlayerTagVertex));
    }
}
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

// samp.dll RVA: 0x00073cb0
void ClientDrawPlayerTags() {
    if(!g_pNetGame||!(*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x38])return;
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CPlayerPool *players=*(CPlayerPool**)(pools+8);
    g_pGame->GetPlayerPed()->GetMatrix(&g_nametagLocalMatrix);
    g_devicePlayerTags->BeginHealthBar();
    int highest=*(int*)((BYTE*)players+0x22);
    for(DWORD counter=0;(int)counter<=highest;counter++) {
        WORD id=(WORD)counter;
        if(id>=1004||*(DWORD*)((BYTE*)players+0xfd6+id*4)!=1)continue;
        BYTE *entry=*(BYTE**)((BYTE*)players+0x26+id*4);
        if(!entry)continue;
        BYTE *remote=*(BYTE**)(entry+8);
        if(!remote)continue;
        CPed *ped=*(CPed**)(remote+4);
        if(!ped||!remote[0x1a]||!*(DWORD*)(remote+0x10))continue;
        BYTE nearby;
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
        CVehicle *vehicle=*(CVehicle**)(remote+8);
        if(remote[0x1a]==0x13&&vehicle&&vehicle->IsRC()) {
            vehicle=*(CVehicle**)(remote+8);
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
        if(*(DWORD*)(remote+0x1c))continue;
        // Reported network values are captured before the distance call.
        float health,armour;
        memcpy(&health,remote+0x1b0,4);memcpy(&armour,remote+0x1ac,4);
        float distance=ped->GetDistanceToCamera();
        g_devicePlayerTags->DrawHealthBar(&g_nametagDrawPosition,health,armour,distance);
    }
    g_devicePlayerTags->EndHealthBar();
}
