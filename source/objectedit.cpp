#include "objectedit.h"
#include "gui/device_objects.h"
#include "game/entity.h"
#include "filehooks.h"
#include "main.h"
// samp.dll RVA: 0x00072ff0
BOOL CObjectEdit::MsgProc(UINT message,WPARAM,LPARAM) {
    BYTE *state=(BYTE*)this;
    if(!unknown80)return FALSE;
    if(message==WM_LBUTTONDOWN) {
        DWORD type=unknown113;
        if(!state[0xa3]&&(type==0||type==1||type==2)) {
            state[0xa3]=1;GetCursorPos((POINT*)(state+0xa7));GetCursorPos((POINT*)(state+0xaf));
            ScreenToClient(g_inputGameWindow,(POINT*)(state+0xaf));SetCapture(g_inputGameWindow);
            ((int(WINAPI*)(BOOL))g_archiveRealShowCursor)(FALSE);ClipCursor(0);
            CObjectPool *pool=*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14);
            CEntity *object=pool->Get(objectId);if(object)object->GetMatrix((CMatrix*)(state+0xc3));
            return TRUE;
        }
    } else if(message==WM_LBUTTONUP) {
        if(state[0xa3]) {
            state[0xa3]=0;ReleaseCapture();((int(WINAPI*)(BOOL))g_archiveRealShowCursor)(TRUE);
            POINT point;
            if(unknown113==0)point=*(POINT*)(state+8);
            else if(unknown113==1)point=*(POINT*)(state+0x18);
            else if(unknown113==2)point=*(POINT*)(state+0x28);
            else point=*(POINT*)(state+0xaf);
            ClientToScreen(g_inputGameWindow,&point);SetCursorPos(point.x,point.y);TryChangeProcessType();return TRUE;
        }
        if(unknown113==3){SetEditMode(0);return TRUE;}
        if(unknown113==4){SetEditMode(1);return TRUE;}
        if(unknown113==5){SetEditMode(2);return TRUE;}
        if(unknown113==10){Disable(TRUE);return TRUE;}
    } else if(message==WM_MOUSEMOVE&&state[0xa3]){ProcessMouseMove();return TRUE;}
    return FALSE;
}
// samp.dll RVA: 0x00071b40
void CObjectEdit::TryChangeProcessType() {
    POINT point;GetCursorPos(&point);ScreenToClient(g_inputGameWindow,&point);
    if(!unknown84||!g_pGame->m_nCursorMode){unknown113=0xffffffff;return;}
    BYTE *state=(BYTE*)this;
    for(int i=0;i<7;i++)if(PtInRect((RECT*)(state+8+i*16),point)){unknown113=i==6?10:i;return;}
    unknown113=0xffffffff;
}
// samp.dll RVA: 0x00071c90
void CObjectEdit::SetEditMode(int mode) {if(unknown78==1&&mode==2)mode=0;unknown7c=mode;}
// samp.dll RVA: 0x00071e60
void CObjectEdit::ResetMousePos() {
    RECT rect;GetClientRect(g_inputGameWindow,&rect);
    int x=(LONG)((DWORD)rect.right-(DWORD)rect.left)/2,y=(LONG)((DWORD)rect.bottom-(DWORD)rect.top)/2;
    BYTE *state=(BYTE*)this;*(int*)(state+0xa7)=x;*(int*)(state+0xab)=y;SetCursorPos(x,y);
}
// samp.dll RVA: 0x00072f00
void CObjectEdit::ApplyChanges(int axis,float difference) {if(unknown78==1)ApplyObjectChanges(axis,difference);else if(unknown78==2)ApplyAttachedChanges(axis,difference);}
// samp.dll RVA: 0x00072f20
void CObjectEdit::ProcessMouseMove() {
    POINT point;GetCursorPos(&point);BYTE *state=(BYTE*)this;
    int axis=(int)unknown113;BYTE reverse=0;DWORD vertical=0;
    if(axis>=0&&axis<3){reverse=state[0xa4+axis];vertical=*(DWORD*)(state+0xb7+axis*4);}
    bool horizontal=unknown7c==1?vertical!=0:vertical==0;
    int current=horizontal?point.x:point.y,previous=*(int*)(state+(horizontal?0xa7:0xab));
    int delta=(LONG)(reverse?(DWORD)previous-(DWORD)current:(DWORD)current-(DWORD)previous);
    if(delta){ApplyChanges(axis,(float)delta*0.01f);ResetMousePos();}
}

#include "game/object.h"
#include "game/util.h"
#include "game/playerped.h"
#include <string.h>
// samp.dll RVA: 0x00072cb0
void CObjectEdit::ApplyObjectChanges(int axis,float difference) {
    CObjectPool *pool=*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14);
    CObject *object=(CObject*)pool->Get(objectId);if(!object)return;
    float x,y,z;object->GetEulerInverted(&x,&y,&z);object->ClearBackendFlag8();
    CMatrix matrix;memcpy(&matrix,(BYTE*)this+0xc3,64);
    CVector position=matrix.position,delta={0,0,0};
    if(axis>=0&&axis<3) {
        if(unknown7c==0) {
            ((float*)&delta)[axis]=difference;GameTransformPoint(&position,&matrix,&delta);
            matrix.position=position;CVector noRotation={-1000.0f,-1000.0f,-1000.0f};object->Move(position,1.0f,noRotation);
        } else if(unknown7c==1) {
            float *angle=(float*)((BYTE*)this+0x92+axis*4);*angle=GameNormalizeAngle(difference*g_objectRotationFactor+*angle);
            object->SetRotation((CVector*)((BYTE*)this+0x92));
        }
    }
    SendEditEndNotification(2);memcpy((BYTE*)this+0xc3,&matrix,64);
}
// samp.dll RVA: 0x00071cb0
void CObjectEdit::ApplyAttachedChanges(int axis,float difference) {
    int slot=*(int*)((BYTE*)this+0x8a);CPed *ped=g_pGame->GetPlayerPed();
    if(slot<0||slot>=10||!ped->m_accessoryOccupied[slot])return;
    BYTE *record=ped->m_accessoryInfo[slot].opaque;if(!record)return;
    if(axis<0||axis>2)return;
    DWORD mode=unknown7c;
    if(mode==0){float *value=(float*)(record+8+axis*4);*value=difference*g_objectMutationStep+*value;}
    else if(mode==1){float *value=(float*)(record+0x14+axis*4);*value=GameNormalizeAngle(difference*g_objectRotationFactor+*value);}
    else if(mode==2) {
        float *value=(float*)(record+0x20+axis*4);
        // FST then compare the unrounded x87 result, including its unordered behavior.
        __asm {
            mov edx,value
            fld difference
            fmul g_objectMutationStep
            fadd dword ptr [edx]
            fst dword ptr [edx]
            fcomp g_objectScaleLowerBound
            fnstsw ax
            test ah,5
            jp done
            mov dword ptr [edx],0
        done:
        }
    }
}

#include "objectedit.h"
// samp.dll RVA: 0x00071600
CObjectEdit::CObjectEdit(IDirect3DDevice9 *value) {
    device=value;unknown7c=0;unknown113=0xffffffff;unknown80=0;unknown84=0;unknown78=0;
    flags[1]=0;flags[2]=0;flags[3]=0;flags[4]=0;flags[0]=0;objectId=0xffff;
    lastTick=GetTickCount();D3DXCreateLine(value,&line);
    D3DXCreateFontA(value,22,0,400,1,FALSE,SYMBOL_CHARSET,0,0,0,"SAMPAUX3",&smallFont);
    D3DXCreateFontA(value,28,0,400,1,FALSE,SYMBOL_CHARSET,0,0,0,"SAMPAUX3",&largeFont);
}

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/quaternion.h"
// samp.dll RVA: 0x000716c0
float CObjectEdit::WorldToScreen(const CVector *position,float *screen) {
    CVector point;memcpy(&point,position,12);
    D3DVIEWPORT9 viewport;device->GetViewport(&viewport);
    D3DXMATRIX identity;D3DXMatrixIdentity(&identity);
    D3DXVECTOR3 projected;
    D3DXVec3Project(&projected,(const D3DXVECTOR3*)&point,&viewport,
        (const D3DXMATRIX*)&g_deviceProjectionMatrix,
        (const D3DXMATRIX*)&g_deviceViewMatrix,&identity);
    const float *projectedData=(const float*)&projected;
    __asm {
        mov eax,screen
        mov ecx,projectedData
        fld dword ptr [ecx]
        fstp dword ptr [eax]
        mov edx,[ecx+4]
        mov [eax+4],edx
    }
    return projected.z;
}

// samp.dll RVA: 0x000717c0
int CObjectEdit::RenderAxes(const CMatrix *matrix,float axisSize) {
    if(!line)return 0;
    line->SetAntialias(TRUE);
    CVector delta,point;D3DXVECTOR2 vertices[2];float negativeSize;
    for(int axis=0;axis<3;++axis) {
        delta.x=0;delta.y=0;delta.z=0;memcpy((float*)&delta+axis,&axisSize,4);
        GameTransformPoint(&point,matrix,&delta);
        const CVector *position=&point;float *screen=(float*)&vertices[0];BYTE visible;
        __asm {
            push screen
            push position
            mov ecx,this
            call CObjectEdit::WorldToScreen
            fcomp g_quaternionMatrixOne
            fnstsw ax
            test ah,41h
            setnz visible
        }
        // Native permits unordered depth here and rejects only depth>1.
        if(!visible)return 0;
        if(axis==0) {
            __asm {
                fld axisSize
                fchs
                fstp negativeSize
            }
        }
        memcpy((float*)&delta+axis,&negativeSize,4);
        GameTransformPoint(&point,matrix,&delta);screen=(float*)&vertices[1];
        __asm {
            push screen
            push position
            mov ecx,this
            call CObjectEdit::WorldToScreen
            fcomp g_quaternionMatrixOne
            fnstsw ax
            test ah,41h
            setnz visible
        }
        if(!visible)return 0;
        line->Begin();line->Draw(vertices,2,0xffcc9999);line->End();
    }
    return 1;
}

// samp.dll RVA: 0x0011a7ac
const float g_cursorRayUnsignedBias=4294967296.0f;
// samp.dll RVA: 0x000739e0
void __cdecl ObjectEditBuildCursorRay(float rayLength,CVector *origin,CVector *end) {
    IDirect3DSurface9 *surface=0;D3DSURFACE_DESC description;
    g_gameD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&surface);
    surface->GetDesc(&description);surface->Release();
    POINT point;GetCursorPos(&point);ScreenToClient(g_inputGameWindow,&point);
    LONG x=point.x,y=point.y;DWORD width=description.Width,height=description.Height;
    float normalizedX,normalizedY;
    __asm {
        fild x
        fadd st(0),st(0)
        mov eax,width
        test eax,eax
        fild width
        jge width_unsigned
        fadd g_cursorRayUnsignedBias
width_unsigned:
        fdivp st(1),st(0)
        fsub g_quaternionMatrixOne
        fdiv dword ptr g_deviceProjectionMatrix
        fstp normalizedX
        fild y
        fadd st(0),st(0)
        mov eax,height
        test eax,eax
        fild height
        jge height_unsigned
        fadd g_cursorRayUnsignedBias
height_unsigned:
        fdivp st(1),st(0)
        fsub g_quaternionMatrixOne
        fdiv dword ptr [g_deviceProjectionMatrix+20]
        fchs
        fstp normalizedY
    }
    D3DXMATRIX product,copy,inverse;
    D3DXMatrixMultiply(&product,(const D3DXMATRIX*)&g_deviceWorldMatrix,
        (const D3DXMATRIX*)&g_deviceViewMatrix);
    memcpy(&copy,&product,64);
    // Native ignores singular-matrix/API failures and leaves untouched output bytes.
    D3DXMatrixInverse(&inverse,0,&copy);
    const float *inverseData=(const float*)&inverse;
    // Preserve y*x-basis summation order, float stores and overlapping outputs.
    __asm {
        mov eax,end
        mov ecx,origin
        mov esi,inverseData
        fld dword ptr [esi+16]
        fmul normalizedY
        fld dword ptr [esi+0]
        fmul normalizedX
        faddp st(1),st(0)
        fadd dword ptr [esi+32]
        fstp dword ptr [eax]
        fld dword ptr [esi+20]
        fmul normalizedY
        fld dword ptr [esi+4]
        fmul normalizedX
        faddp st(1),st(0)
        fadd dword ptr [esi+36]
        fstp dword ptr [eax+4]
        fld dword ptr [esi+24]
        fmul normalizedY
        fld dword ptr [esi+8]
        fmul normalizedX
        faddp st(1),st(0)
        fadd dword ptr [esi+40]
        fstp dword ptr [eax+8]
        mov edx,dword ptr [esi+52]
        mov [ecx+4],edx
        fld dword ptr [esi+48]
        mov edx,dword ptr [esi+56]
        fstp dword ptr [ecx]
        mov [ecx+8],edx
        fld rayLength
        fmul dword ptr [eax]
        fadd dword ptr [esi+48]
        fstp dword ptr [eax]
        fld rayLength
        fmul dword ptr [eax+4]
        fadd dword ptr [ecx+4]
        fstp dword ptr [eax+4]
        fld rayLength
        fmul dword ptr [eax+8]
        fadd dword ptr [ecx+8]
        fstp dword ptr [eax+8]
    }
}
