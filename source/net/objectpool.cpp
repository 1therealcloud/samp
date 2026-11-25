#include "net/objectpool.h"

#include "game/custommodels.h"
#include "gui/gui.h"
#include "gui/label.h"
#include "main.h"

#include <GetTime.h>

#include <string.h>

DWORD g_objectPoolLastTick;

const float g_objectPoolSecondsPerTick = 0.001f;

GameEntityCollisionShape g_objectPoolRayOrigin;

GameEntityCollisionShape g_objectPoolRayEnd;

CObject* g_objectPoolRayDebugObject;

const char g_objectPoolRayOriginText[8] = "Sphere1";

const char g_objectPoolRayEndText[8] = "Sphere2";

CObjectPool::CObjectPool()
{
    for (int i = 0; i < 2100; ++i)
    {
        active[i] = 0;
        objects[i] = 0;
    }
    largestId = 0;
}

void CObjectPool::UpdateLargestId()
{
    int largest = 0;
    for (int i = 0; i < 2100; ++i)
        if (active[i])
            largest = i;
    largestId = largest;
}

int CObjectPool::GetCount()
{
    int count = 0;
    const DWORD* slot = active;
    int remaining = 2100;
    do
    {
        if (*slot)
            ++count;
        ++slot;
    }
    while (--remaining);
    return count;
}

BOOL CObjectPool::Delete(WORD id)
{
    BYTE* state = (BYTE*)this;
    if (id >= 2100 || !*(DWORD*)(state + 4 + id * 4) || !*(CObject**)(state + 0x20d4 + id * 4))
        return FALSE;
    CObject* object = *(CObject**)(state + 0x20d4 + id * 4);
    CCamera* camera = g_pGame->m_pCamera;
    if (camera->m_pAttachedTo == object)
        camera->Attach(0);
    object = *(CObject**)(state + 0x20d4 + id * 4);
    *(DWORD*)(state + 4 + id * 4) = 0;
    delete object;
    *(CObject**)(state + 0x20d4 + id * 4) = 0;
    UpdateLargestId();
    return TRUE;
}

CObjectPool::~CObjectPool()
{
    for (int i = 0; i < 2100; ++i)
        Delete((WORD)i);
}

CObject* CObjectPool::Find(void* backend)
{
    int largest = largestId;
    for (int i = 1; i <= largest; ++i)
        if (objects[i] && backend == objects[i]->m_pGameEntity)
            return objects[i];
    return 0;
}

WORD CObjectPool::GetId(void* backend)
{
    int largest = largestId;
    for (int i = 1; i <= largest; ++i)
        if (objects[i] && backend == objects[i]->m_pGameEntity)
            return (WORD)i;
    return 0xffff;
}

CEntity* CObjectPool::Get(WORD id)
{
    BYTE* state = (BYTE*)this;
    if (id < 2100 && *(DWORD*)(state + 4 + id * 4))
        return *(CEntity**)(state + 0x20d4 + id * 4);
    return 0;
}

void CObjectPool::Process()
{
    if (!g_objectPoolLastTick)
        g_objectPoolLastTick = RakNet::GetTime();
    DWORD now = RakNet::GetTime();
    LONG delta = (LONG)(now - g_objectPoolLastTick);
    float seconds = (float)delta * g_objectPoolSecondsPerTick;
    DWORD i = 0;
    if ((LONG)i <= largestId)
    {
        do
        {
            if (active[i])
                (objects[i])->Process(seconds);
        }
        while ((LONG)++i <= largestId);
    }
    g_objectPoolLastTick = now;
}

void CObjectPool::ConstructMaterials()
{
    DWORD i = 0;
    if ((LONG)i <= largestId)
    {
        do
        {
            if (active[i])
            {
                CObject* object = objects[i];
                if (object->hasMaterials)
                    object->ConstructMaterialText();
            }
        }
        while ((LONG)++i <= largestId);
    }
}

void CObjectPool::ShutdownMaterials()
{
    DWORD i = 0;
    if ((LONG)i <= largestId)
    {
        do
        {
            if (active[i])
            {
                CObject* object = objects[i];
                if (object->hasMaterials)
                    object->ShutdownMaterialText();
            }
        }
        while ((LONG)++i <= largestId);
    }
}

BOOL CObjectPool::Create(WORD id, int model, CVector position, CVector rotation, float drawDistance)
{
    BYTE* state = (BYTE*)this;
    if (*(CObject**)(state + 0x20d4 + id * 4))
        Delete(id);
    BYTE firstHour = 0, lastHour = 0;
    if (model < 0 && g_customModels && g_customModels->IsObjectEntry(model))
    {
        firstHour = g_customModels->GetObjectFirstHour(model);
        lastHour = g_customModels->GetObjectLastHour(model);
        if (!g_customModels->GetLoadedFlag(model) && !g_customModels->LoadObjectEntry(model))
            model = 18631;
    }
    CObject* object = g_pGame->NewObject(model, position.x, position.y, position.z, rotation, drawDistance, firstHour, lastHour);
    *(CObject**)(state + 0x20d4 + id * 4) = object;
    if (object)
    {
        *(DWORD*)(state + 4 + id * 4) = 1;
        UpdateLargestId();
        return TRUE;
    }
    return FALSE;
}

void CObjectPool::Draw()
{
    for (int i = 0; i <= largestId; ++i)
        if (active[i])
        {
            CObject* object = objects[i];
            if (object->renderReady && !object->drawLast)
                object->Draw();
        }
}

void CObjectPool::DrawLast()
{
    float distances[1000] = {0};
    CObject* ordered[1000] = {0};
    int count = 0;
    for (int i = 0; i <= largestId; ++i)
        if (active[i] && count < 1000)
        {
            CObject* object = objects[i];
            if (object->renderReady && object->drawLast)
            {
                ordered[count] = object;
                distances[count] = object->GetDistanceToCamera();
                ++count;
            }
        }
    // Stable descending bubble pass; unordered pairs do not exchange places.
    for (int end = count - 1; end > 0; --end)
        for (int i = 0; i < end; ++i)
            if (distances[i + 1] > distances[i])
            {
                float distance = distances[i];
                distances[i] = distances[i + 1];
                distances[i + 1] = distance;
                CObject* object = ordered[i];
                ordered[i] = ordered[i + 1];
                ordered[i + 1] = object;
            }
    for (int j = 0; j < count; ++j)
        if (ordered[j])
            ordered[j]->Draw();
}

bool CObjectPool::TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second)
{
    const float limit = 200.0f;
    for (int i = 0; i <= largestId; ++i)
        if (active[i])
        {
            CObject* object = objects[i];
            if (!object || !object->IsAdded() || object->cameraCollisionDisabled)
                continue;
            if (!(object->GetDistanceToCamera() < limit) || object->IsIgnored())
                continue;
            if (object->CEntity::TestCollisionShapes(first, second, 1))
                return true;
        }
    return false;
}

WORD CObjectPool::FindInRay(float maxDistance, const CVector* origin, const CVector* end)
{
    GameEntityCollisionShape from, to;
    memcpy(from.words, origin, 12);
    from.words[3] = 0x3e800000;
    memcpy(to.words, end, 12);
    to.words[3] = 0x3e800000;
    // Neither native fifth word is written. Copy the opaque representations,
    // including those stack bytes, rather than inventing a zero/default value.
    memcpy(&g_objectPoolRayOrigin, &from, sizeof(from));
    memcpy(&g_objectPoolRayEnd, &to, sizeof(to));
    WORD selected = 0xffff;
    float closest = maxDistance;
    for (int i = 0; i <= largestId; ++i)
        if (active[i])
        {
            CObject* object = objects[i];
            // The original calls the distance method before checking the slot pointer.
            const volatile float distance = object->GetDistanceToPoint(origin->x, origin->y, origin->z);
            if (!object || !object->IsAdded() || object->GetModelIndex() == 19300 || !(distance < closest))
                continue;
            BYTE* collision = (BYTE*)GameGetModelCollision(object->GetModelIndex());
            if (!collision)
                continue;
            DWORD collisionEnabled;
            // clang-format off
        __asm {
            mov ecx,object
            call CEntity::GetCollisionFlag
            mov collisionEnabled,eax
        }
            // clang-format on
            BYTE hit;
            if (collisionEnabled && *(DWORD*)(collision + 0x2c))
                hit = object->CEntity::TestCollisionShapes(&from, &to, 0);
            else
            {
                CVector centre;
                object->GetBoundCentre(&centre);
                hit = GameTestEntityLineSphere((const CVector*)from.words, (const CVector*)to.words, object->m_pGameEntity, &centre, 1.0f, 1);
            }
            if (hit)
            {
                closest = distance;
                selected = (WORD)i;
            }
        }
    return selected;
}

void DrawObjectPoolCollisionDebug()
{
    if (!g_deviceLabel)
        return;
    CVector origin, end;
    memcpy(&origin, g_objectPoolRayOrigin.words, 12);
    memcpy(&end, g_objectPoolRayEnd.words, 12);
    g_deviceLabel->Draw(&origin, g_objectPoolRayOriginText, 0xffffffff, TRUE, FALSE);
    g_deviceLabel->Draw(&end, g_objectPoolRayEndText, 0xffffffff, TRUE, FALSE);
    if (!g_objectPoolRayDebugObject)
    {
        CVector rotation = {0, 0, 0};
        g_objectPoolRayDebugObject = g_pGame->NewObject(3065, 0, 0, 0, rotation, 200.0f, 0, 0);
        g_objectPoolRayDebugObject->SetCollisionFlag(FALSE);
        g_objectPoolRayDebugObject->Add();
    }
    CVector position;
    memcpy(&position, g_objectPoolRayEnd.words, 12);
    g_objectPoolRayDebugObject->Teleport(position);
}

int CObjectPool::GetLargestId()
{
    return largestId;
}

DWORD CObjectPool::IsActive(WORD id)
{
    if (id >= 2100)
        return 0;
    return *(DWORD*)((BYTE*)this + 4 + id * 4);
}

void CObjectPool::UpdateAttachedObjectRotations()
{
    DWORD i = 0;
    if ((LONG)i > largestId)
        return;
    do
    {
        if (active[i])
            (objects[i])->UpdateAttachedObjectRotation();
    }
    while ((LONG)++i <= largestId);
}
