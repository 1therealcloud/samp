#include "net/objectpool.h"
#include "net/pool_frame.h"
#include "main.h"
#include <GetTime.h>
// samp.dll RVA: 0x00012700
CObjectPool::CObjectPool() {
    for(int i=0;i<2100;++i){active[i]=0;objects[i]=0;}
    largestId=0;
}
// samp.dll RVA: 0x000126a0
void CObjectPool::UpdateLargestId() {
    int largest=0;for(int i=0;i<2100;++i)if(active[i])largest=i;
    largestId=largest;
}
// samp.dll RVA: 0x00012730
int CObjectPool::GetCount() {int count=0;for(int i=0;i<2100;++i)if(active[i])++count;return count;}
// samp.dll RVA: 0x00012750
BOOL CObjectPool::Delete(WORD id) {
    BYTE *state=(BYTE*)this;
    if(id>2100 || !*(DWORD*)(state+4+id*4) || !*(CObject**)(state+0x20d4+id*4))return FALSE;
    CObject *object=*(CObject**)(state+0x20d4+id*4);
    CCamera *camera=g_pGame->m_pCamera;
    if(camera->m_pAttachedTo==object)camera->Attach(0);
    object=*(CObject**)(state+0x20d4+id*4);
    *(DWORD*)(state+4+id*4)=0;
    delete object;
    *(CObject**)(state+0x20d4+id*4)=0;
    UpdateLargestId();return TRUE;
}
// samp.dll RVA: 0x00013060
CObjectPool::~CObjectPool() {for(WORD i=0;i<2100;++i)Delete(i);}
// samp.dll RVA: 0x000128d0
CObject *CObjectPool::Find(void *backend) {
    int largest=largestId;
    for(int i=1;i<=largest;++i)if(objects[i] && objects[i]->m_pGameEntity==backend)return objects[i];
    return 0;
}
// samp.dll RVA: 0x00012910
WORD CObjectPool::GetId(void *backend) {
    int largest=largestId;
    for(int i=1;i<=largest;++i)if(objects[i] && objects[i]->m_pGameEntity==backend)return (WORD)i;
    return 0xffff;
}
// samp.dll RVA: 0x00002e10
CEntity *CObjectPool::Get(WORD id) {
    BYTE *state=(BYTE*)this;
    if(id<2101&&*(DWORD*)(state+4+id*4))return *(CEntity**)(state+0x20d4+id*4);
    return 0;
}
// samp.dll RVA: 0x00156b74
DWORD g_objectPoolLastTick;
// samp.dll RVA: 0x00118130
const float g_objectPoolSecondsPerTick=0.001f;

// samp.dll RVA: 0x00012950
void CObjectPool::Process() {
    if(!g_objectPoolLastTick)g_objectPoolLastTick=RakNet::GetTime();
    DWORD now=RakNet::GetTime();LONG delta=(LONG)(now-g_objectPoolLastTick);float seconds;
    __asm {
        fild delta
        fmul g_objectPoolSecondsPerTick
        fstp seconds
    }
    BYTE *state=(BYTE*)this;
    if(*(LONG*)state>=0) {
        DWORD i=0;
        do {
            if(*(DWORD*)(state+4+i*4))(*(CObject**)(state+0x20d4+i*4))->Process(seconds);
        } while((LONG)++i<=*(LONG*)state);
    }
    g_objectPoolLastTick=now;
}
// samp.dll RVA: 0x00012a10
void CObjectPool::ConstructMaterials() {
    BYTE *state=(BYTE*)this;
    if(*(LONG*)state>=0) {
        DWORD i=0;
        do {
            if(*(DWORD*)(state+4+i*4)) {
                CObject *object=*(CObject**)(state+0x20d4+i*4);
                if(*(DWORD*)((BYTE*)object+0x118b))object->ConstructMaterialText();
            }
        } while((LONG)++i<=*(LONG*)state);
    }
}
// samp.dll RVA: 0x00012a50
void CObjectPool::ShutdownMaterials() {
    BYTE *state=(BYTE*)this;
    if(*(LONG*)state>=0) {
        DWORD i=0;
        do {
            if(*(DWORD*)(state+4+i*4)) {
                CObject *object=*(CObject**)(state+0x20d4+i*4);
                if(*(DWORD*)((BYTE*)object+0x118b))object->ShutdownMaterialText();
            }
        } while((LONG)++i<=*(LONG*)state);
    }
}

#include "game/custommodels.h"
// samp.dll RVA: 0x000127d0
BOOL CObjectPool::Create(WORD id,int model,CVector position,CVector rotation,float drawDistance) {
    BYTE *state=(BYTE*)this;
    if(*(CObject**)(state+0x20d4+id*4))Delete(id);
    BYTE firstHour=0,lastHour=0;
    if(model<0 && g_customModels && g_customModels->IsObjectEntry(model)) {
        firstHour=g_customModels->GetObjectFirstHour(model);
        lastHour=g_customModels->GetObjectLastHour(model);
        if(!g_customModels->GetLoadedFlag(model) && !g_customModels->LoadObjectEntry(model))model=18631;
    }
    CObject *object=g_pGame->NewObject(model,position.x,position.y,position.z,rotation,drawDistance,firstHour,lastHour);
    *(CObject**)(state+0x20d4+id*4)=object;
    if(!object)return FALSE;
    *(DWORD*)(state+4+id*4)=1;UpdateLargestId();return TRUE;
}

// samp.dll RVA: 0x00012a90
void CObjectPool::Draw() {
    BYTE *state=(BYTE*)this;
    for(int i=0;i<=*(int*)state;++i)if(*(DWORD*)(state+4+i*4)) {
        CObject *object=*(CObject**)(state+0x20d4+i*4);
        BYTE *data=(BYTE*)object;
        if(data[0x49] && !data[0x6c])object->Render();
    }
}
// samp.dll RVA: 0x00012ad0
void CObjectPool::DrawLast() {
    float distances[1000]={0};CObject *ordered[1000]={0};int count=0;
    BYTE *state=(BYTE*)this;
    for(int i=0;i<=*(int*)state;++i)if(*(DWORD*)(state+4+i*4) && count<1000) {
        CObject *object=*(CObject**)(state+0x20d4+i*4);
        BYTE *data=(BYTE*)object;
        if(data[0x49] && data[0x6c]) {
            ordered[count]=object;
            distances[count]=object->GetDistanceToCamera();++count;
        }
    }
    // Stable descending bubble pass; unordered pairs do not exchange places.
    for(int end=count-1;end>0;--end)for(int i=0;i<end;++i)if(distances[i+1]>distances[i]) {
        float distance=distances[i];distances[i]=distances[i+1];distances[i+1]=distance;
        CObject *object=ordered[i];ordered[i]=ordered[i+1];ordered[i+1]=object;
    }
    for(int j=0;j<count;++j)if(ordered[j])ordered[j]->Render();
}

// samp.dll RVA: 0x00012ce0
bool CObjectPool::TestCollisionShapes(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second) {
    BYTE *state=(BYTE*)this;const float limit=200.0f;
    for(int i=0;i<=*(int*)state;++i)if(*(DWORD*)(state+4+i*4)) {
        CObject *object=*(CObject**)(state+0x20d4+i*4);
        if(!object || !object->IsAdded() || ((BYTE*)object)[0x53])continue;
        // Compare extended ST0 directly; rounding to float can change this gate.
        BYTE nearby;
        __asm {
            mov ecx,object
            call CEntity::GetDistanceToCamera
            fcomp limit
            fnstsw ax
            test ah,5
            setnp nearby
        }
        if(!nearby || object->IsIgnored())continue;
        if(object->CEntity::TestCollisionShapes(first,second,1))return true;
    }
    return false;
}
