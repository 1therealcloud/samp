#include "gui/object_edit.h"
#include "gui/device_objects.h"
#include "game/entity.h"
#include "archive/filehooks.h"
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
