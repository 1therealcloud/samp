#include "game/object.h"
#include <math.h>
#include <stddef.h>
#include <string.h>
// Only the observed RenderWare prefixes used by these callbacks are described.
// The complete backend layouts and ownership remain outside these views.
enum ObjectRwObservedFlags
{
    ObjectRwAtomicType = 1,
    ObjectRwGeometryPrelit = 0x08,
    ObjectRwGeometryModulateMaterialColor = 0x40
};
struct ObjectRwObjectPrefix
{
    BYTE type, subType, flags, privateFlags;
    void* parent;
};
typedef char ObjectRwObjectExtent[(sizeof(ObjectRwObjectPrefix) == 8) ? 1 : -1];
typedef char ObjectRwObjectParentOffset[(offsetof(ObjectRwObjectPrefix, parent) == 4) ? 1 : -1];
struct ObjectRwTexturePrefix
{
    void* raster;
    BYTE unknown04[0x4c];
    // Whole raw filtering/addressing word; native increments all32 bits.
    DWORD filterAddressing;
};
typedef char ObjectRwTextureRasterOffset[(offsetof(ObjectRwTexturePrefix, raster) == 0) ? 1 : -1];
typedef char ObjectRwTextureFilterOffset[(offsetof(ObjectRwTexturePrefix, filterAddressing) == 0x50) ? 1 : -1];
struct ObjectRwColor
{
    BYTE red, green, blue, alpha;
};
struct ObjectRwSurface
{
    float ambient, specular, diffuse;
};
struct ObjectRwMaterialPrefix
{
    void* texture;
    ObjectRwColor color;
    void* pipeline;
    ObjectRwSurface surface;
};
struct ObjectRwMaterialListPrefix
{
    ObjectRwMaterialPrefix** materials;
    int count;
};
struct ObjectRwGeometryPrefix
{
    BYTE unknown00[8];
    DWORD flags;
    BYTE unknown0c[20];
    ObjectRwMaterialListPrefix materialList;
};
struct ObjectRwAtomicPrefix
{
    ObjectRwObjectPrefix object;
    BYTE unknown08[0x10];
    ObjectRwGeometryPrefix* geometry;
};
typedef char ObjectRwColorSize[(sizeof(ObjectRwColor) == 4) ? 1 : -1];
typedef char ObjectRwSurfaceSize[(sizeof(ObjectRwSurface) == 12) ? 1 : -1];
typedef char ObjectRwMaterialTextureOffset[(offsetof(ObjectRwMaterialPrefix, texture) == 0) ? 1 : -1];
typedef char ObjectRwMaterialColorOffset[(offsetof(ObjectRwMaterialPrefix, color) == 4) ? 1 : -1];
typedef char ObjectRwMaterialSurfaceOffsets[(offsetof(ObjectRwMaterialPrefix, surface.ambient) == 0xc && offsetof(ObjectRwMaterialPrefix, surface.specular) == 0x10 &&
                                             offsetof(ObjectRwMaterialPrefix, surface.diffuse) == 0x14)
                                                ? 1
                                                : -1];
typedef char ObjectRwGeometryOffsets[(offsetof(ObjectRwGeometryPrefix, flags) == 8 && offsetof(ObjectRwGeometryPrefix, materialList.materials) == 0x20 &&
                                      offsetof(ObjectRwGeometryPrefix, materialList.count) == 0x24)
                                         ? 1
                                         : -1];
typedef char ObjectRwAtomicGeometryOffset[(offsetof(ObjectRwAtomicPrefix, geometry) == 0x18) ? 1 : -1];

// 0x000715c0
CVector* CObject::GetRotation()
{
    return &eulerRotation;
}
// 0x00017750
void CObject::DisableCameraCollision()
{
    cameraCollisionDisabled = 1;
}
#include "game/scripting.h"
// 0x0011edfc
const Scripting::OpcodeInfo g_objectSetRotationCommand = {0x453, "ifff"};
// 0x000a7c90
void CObject::SetRotation(const CVector* rotation)
{
    if (m_pGameEntity && GamePoolObjectGetAt(m_nHandle))
    {
        Scripting::ProcessCommand(&g_objectSetRotationCommand, m_nHandle, (double)rotation->x, (double)rotation->y, (double)rotation->z);
        memcpy(&eulerRotation.x, &rotation->x, 4);
        memcpy(&eulerRotation.y, &rotation->y, 4);
        memcpy(&eulerRotation.z, &rotation->z, 4);
    }
}

#include "game/quaternion.h"
#include "game/util.h"
#include "main.h"
#include "net/localplayer.h"
#include "net/netgame.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"
#include <GetTime.h>
#include <string.h>
// 0x0011ee7c
extern const float g_objectMotionRotationSentinel = -999.0f;
// 0x000a7c20
void CObject::Stop()
{
    CVector zero = {0, 0, 0};
    SetSpeed(zero);
    SetTurnSpeed(zero);
    motionFlags &= 0xfe;
}
// 0x000a7bf0
float CObject::GetAngleDifference(float from, float to)
{
    to = GameNormalizeAngle(to);
    to -= from;
    return GameNormalizeAngle(to);
}
// 0x0011e648
extern const float g_objectDegreesToRadians = 0.01745329238474369f;
// Keep the native x87 trigonometry and float spill boundaries. Only the nine
// rotation coefficients are written; matrix padding and position are untouched.
// 0x000a7fb0
void CObject::Rotate(float x, float y, float z)
{
    eulerRotation.x = x;
    eulerRotation.y = y;
    eulerRotation.z = z;
    x *= g_objectDegreesToRadians;
    y *= g_objectDegreesToRadians;
    z *= g_objectDegreesToRadians;
    float cx = (float)cos(x), sx = (float)sin(x);
    float cy = (float)cos(y), sy = (float)sin(y);
    float cz = (float)cos(z), sz = (float)sin(z);
    float sxsz = sx * sz, sxcz = sx * cz;
    targetMatrix.right.x = cz * cy - sxsz * sy;
    targetMatrix.right.y = sxcz * sy + sz * cy;
    targetMatrix.right.z = -sy * cx;
    targetMatrix.up.x = -sz * cx;
    targetMatrix.up.y = cz * cx;
    targetMatrix.up.z = sx;
    targetMatrix.at.x = sxsz * cy + cz * sy;
    targetMatrix.at.y = sz * sy - sxcz * cy;
    targetMatrix.at.z = cy * cx;
}

// 0x0011ee24
extern const Scripting::OpcodeInfo g_objectIsAttachedCommand = {0x685, "i"};
// 0x0011ee10
extern const Scripting::OpcodeInfo g_objectAttachVehicleCommand = {0x681, "iiffffff"};
// 0x0011ee38
extern const Scripting::OpcodeInfo g_objectAttachObjectCommand = {0x69a, "iiffffff"};
// Keep differences/products in x87 registers through sqrt, returning ST0.
// 0x000a7bb0
float CObject::GetDistanceToTarget(const CMatrix* matrix)
{
    float z = matrix->position.z - targetMatrix.position.z;
    float y = matrix->position.y - targetMatrix.position.y;
    float x = matrix->position.x - targetMatrix.position.x;
    return (float)sqrt(x * x + y * y + z * z);
}
// 0x000a7e30
void CObject::AttachToVehicle(CVehicle* vehicle)
{
    if (!Scripting::ProcessCommand(&g_objectIsAttachedCommand, m_nHandle))
    {
        Scripting::ProcessCommand(&g_objectAttachVehicleCommand, m_nHandle, ((CEntity*)vehicle)->m_nHandle, (double)attachmentOffset.x, (double)attachmentOffset.y,
                                  (double)attachmentOffset.z, (double)attachmentRotation.x, (double)attachmentRotation.y, (double)attachmentRotation.z);
    }
}
// 0x000a7eb0
void CObject::AttachToObject(CObject* object)
{
    if (!Scripting::ProcessCommand(&g_objectIsAttachedCommand, m_nHandle))
    {
        Scripting::ProcessCommand(&g_objectAttachObjectCommand, m_nHandle, object->m_nHandle, (double)attachmentOffset.x, (double)attachmentOffset.y, (double)attachmentOffset.z,
                                  (double)attachmentRotation.x, (double)attachmentRotation.y, (double)attachmentRotation.z);
    }
}

// 0x000a97c0
void CObject::Move(CVector position, float speed, CVector rotation)
{
    CMatrix matrix;
    GetMatrix(&matrix);
    if (motionFlags & 1)
    {
        Stop();
        matrix.position.x = targetMatrix.position.x;
        matrix.position.y = targetMatrix.position.y;
        matrix.position.z = targetMatrix.position.z;
        if (rotationEnabled)
            GameQuaternionToMatrix(&targetQuaternion, &matrix);
        ApplyMatrix(matrix);
    }
    DWORD tick = RakNet::GetTime();
    moveStartTick = tick;
    targetMatrix.position.x = position.x;
    targetMatrix.position.y = position.y;
    targetMatrix.position.z = position.z;
    moveSpeed = speed;
    motionFlags |= 1;
    if (rotation.x > g_objectMotionRotationSentinel && rotation.y > g_objectMotionRotationSentinel && rotation.z > g_objectMotionRotationSentinel)
    {
        rotationEnabled = 1;
        float x, y, z;
        GetEulerInverted(&x, &y, &z);
        targetEuler.x = GameNormalizeAngle(rotation.y);
        targetEuler.y = GameNormalizeAngle(rotation.x);
        targetEuler.z = GameNormalizeAngle(rotation.z);
        angleDelta.x = GetAngleDifference(x, targetEuler.x);
        angleDelta.y = GetAngleDifference(y, targetEuler.y);
        angleDelta.z = GetAngleDifference(z, targetEuler.z);
        Rotate(rotation.x, rotation.y, rotation.z);
        CMatrix rotated;
        GetMatrix(&rotated);
        GameQuaternionFromMatrix(&rotated, &startQuaternion);
        GameQuaternionFromMatrix(&targetMatrix, &targetQuaternion);
        GameQuaternionNormalize(&startQuaternion);
        GameQuaternionNormalize(&targetQuaternion);
    }
    else
        rotationEnabled = 0;
    moveDistance = GetDistanceToPoint(targetMatrix.position.x, targetMatrix.position.y, targetMatrix.position.z);
    if (g_pNetGame)
    {
        CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
        CPlayerPool* players = pools->players;
        if (players)
            players->local->ProcessSurfing();
    }
    ClearBackendFlag8();
}

#include "gui.h"

// 0x000a8870
bool CObject::GetMaterialSize(int code, int* width, int* height)
{
    switch (code)
    {
        case 10:
            *width = 32;
            *height = 32;
            return true;
        case 20:
            *width = 64;
            *height = 32;
            return true;
        case 30:
            *width = 64;
            *height = 64;
            return true;
        case 40:
            *width = 128;
            *height = 32;
            return true;
        case 50:
            *width = 128;
            *height = 64;
            return true;
        case 60:
            *width = 128;
            *height = 128;
            return true;
        case 70:
            *width = 256;
            *height = 32;
            return true;
        case 80:
            *width = 256;
            *height = 64;
            return true;
        case 90:
            *width = 256;
            *height = 128;
            return true;
        case 100:
            *width = 256;
            *height = 256;
            return true;
        case 110:
            *width = 512;
            *height = 64;
            return true;
        case 120:
            *width = 512;
            *height = 128;
            return true;
        case 130:
            *width = 512;
            *height = 256;
            return true;
        case 140:
            *width = 512;
            *height = 512;
            return true;
        default:
            return false;
    }
}
// 0x000a9ad0
void CObject::ConstructMaterialText()
{
    for (int i = 0; i < 16; ++i)
    {
        ObjectMaterialTextInfo& info = materials.textInfo[i];
        if (materials.textures[i] && materials.types[i] == 2 && !materials.textProcessed[i])
        {
            int width, height;
            if (GetMaterialSize(info.size, &width, &height))
            {
                materials.renderedText[i] =
                    g_deviceMaterialText->Create(materials.text[i], info.font, info.fontSize, width, height, info.fontColor, info.backgroundColor, info.bold, info.alignment);
                // Native records the attempt even if Create returned NULL.
                materials.textProcessed[i] = 1;
            }
        }
    }
}
// 0x000a8ac0
void CObject::ShutdownMaterialText()
{
    for (int i = 0; i < 16; ++i)
    {
        IDirect3DTexture9*& texture = materials.renderedText[i];
        if (texture)
        {
            texture->Release();
            texture = 0;
            materials.textProcessed[i] = 0;
        }
    }
}

#include "game/game.h"
// 0x000a7b40
bool CObject::IsTimeVisible()
{
    BYTE hour = 0, minute = 0;
    if (timeVisibilityEnabled)
    {
        if (g_pGame)
            g_pGame->GetWorldTime(&hour, &minute);
        if (visibilityEndHour > visibilityStartHour)
        {
            if (hour >= visibilityStartHour && hour < visibilityEndHour)
                return true;
        }
        else if (visibilityEndHour < visibilityStartHour)
        {
            if (hour >= visibilityStartHour || hour < visibilityEndHour)
                return true;
        }
    }
    return false;
}
// 0x000a8b90
void CObject::ClearModelFlag0()
{
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
    {
        BYTE* info = (BYTE*)GameGetModelInfo(modelIndex);
        if (info)
            *(DWORD*)(info + 0x12) &= 0xfffffffe;
    }
}
// 0x000a9150
void CObject::ProcessStreaming()
{
    renderReady = 0;
    bool timeHidden = false;
    if (timeVisibilityEnabled && !IsTimeVisible())
        timeHidden = true;
    cameraDistance = GetDistanceToCamera();
    if (!(cameraDistance > streamingDistance) && !timeHidden)
    {
        if (!GetRwObject())
        {
            if (!g_pGame->IsModelAvailable(modelIndex))
            {
                g_pGame->RequestModel(modelIndex, 2);
                return;
            }
            SetModelIndex(modelIndex);
        }
        if (GetRwObject())
        {
            renderReady = 1;
            streamingHideCount = 0;
            ClearModelFlag0();
        }
    }
    else if (GetRwObject())
    {
        DWORD count = ++streamingHideCount;
        if (count == 1000)
        {
            DeleteRwObject();
            streamingHideCount = 0;
            int model = modelIndex;
            if (model > 0 && !GameGetModelReferenceCount(model))
                g_pGame->ReleaseModel(modelIndex, 0);
        }
    }
}

#include "game/playerped.h"
#include "net/pool_frame.h"
// 0x00118200
extern const float g_objectStreamingNearDistance = 300.0f;
// 0x00117878
extern const float g_objectStreamingVehicleMargin = 30.0f;
// 0x001181b0
extern const float g_objectTurnVelocityScale = 0.01f;
// 0x0011ee78
extern const float g_objectNegativeTurnVelocityLimit = -0.001f;
// 0x000a9240
void CObject::Process(float elapsedSeconds)
{
    if (!hasMaterials && !(streamingDistance > g_objectStreamingNearDistance) && !timeVisibilityEnabled)
    {
        if (!GetRwObject() && g_pGame->GetPlayerPed()->IsInVehicle())
        {
            if (GetDistanceToCamera() < streamingDistance - g_objectStreamingVehicleMargin && !g_pGame->IsModelAvailable(modelIndex))
                g_pGame->RequestModel(modelIndex, 2);
        }
    }
    else
        ProcessStreaming();
    WORD vehicle = attachedVehicle;
    if (vehicle != 0xffff)
    {
        if (!g_pNetGame)
            return;
        CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
        CVehiclePool* pool = pools->vehicles;
        if (!pool || vehicle >= 2000 || !pool->m_bNotEmpty[vehicle])
            return;
        CVehicle* attached = pool->m_pObjects[vehicle];
        if (attached && ((CEntity*)attached)->DoesExist())
            AttachToVehicle(attached);
        return;
    }
    WORD object = attachedObject;
    if (object != 0xffff)
    {
        if (!g_pNetGame)
            return;
        CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
        CObjectPool* pool = pools->objects;
        if (!pool)
            return;
        CObject* attached = (CObject*)pool->Get(object);
        if (attached)
            AttachToObject(attached);
        return;
    }
    if (!(motionFlags & 1))
        return;
    CMatrix matrix;
    CVector old;
    float step, remaining, clockDistance, travelled;
    {
        CVector zero = {0, 0, 0};
        GetMatrix(&matrix);
        step = elapsedSeconds * moveSpeed;
        remaining = GetDistanceToTarget(&matrix);
        DWORD now = RakNet::GetTime();
        int elapsedTicks = (int)(now - moveStartTick);
        clockDistance = ((float)elapsedTicks * 0.001f) * moveSpeed;
        lastMoveTick = now;
        old = matrix.position;
        travelled = moveDistance - remaining;
        if (step >= remaining)
        {
            SetSpeed(zero);
            SetTurnSpeed(zero);
            matrix.position.x = targetMatrix.position.x;
            matrix.position.y = targetMatrix.position.y;
            matrix.position.z = targetMatrix.position.z;
            if (rotationEnabled)
                GameQuaternionToMatrix(&targetQuaternion, &matrix);
            ApplyMatrix(matrix);
            Stop();
            return;
        }
    }
    if (!(elapsedSeconds > 0.0))
        return;
    float reciprocal = 1.0f / (remaining / step);
    {
        CVector delta;
        delta.x = (targetMatrix.position.x - matrix.position.x) * reciprocal;
        delta.y = (targetMatrix.position.y - matrix.position.y) * reciprocal;
        delta.z = (targetMatrix.position.z - matrix.position.z) * reciprocal;
        delta.x += matrix.position.x;
        matrix.position.x = delta.x;
        delta.y += matrix.position.y;
        matrix.position.y = delta.y;
        delta.z += matrix.position.z;
        matrix.position.z = delta.z;
    }
    float progress = 1.0f - remaining / moveDistance;
    float remainingFraction = remaining / moveDistance;
    CVector speed;
    float inverseElapsed = 1.0f / elapsedSeconds;
    speed.x = (matrix.position.x - old.x) * inverseElapsed;
    speed.y = (matrix.position.y - old.y) * inverseElapsed;
    speed.z = (matrix.position.z - old.z) * inverseElapsed;
    speed.x *= 0.02f;
    speed.y *= 0.02f;
    speed.z *= 0.02f;
    if (FloatOffset(clockDistance, travelled) > 0.1f)
    {
        if (clockDistance > travelled)
        {
            float correction = (clockDistance - travelled) * 0.1f + 1.0f;
            speed.x *= correction;
            speed.y *= correction;
            speed.z *= correction;
        }
        else if (travelled > clockDistance)
        {
            float correction = 1.0f - (travelled - clockDistance) * 0.1f;
            speed.x *= correction;
            speed.y *= correction;
            speed.z *= correction;
        }
    }
    SetSpeed(speed);
    ApplyLinearSpeed();
    CMatrix current;
    if (rotationEnabled)
    {
        float x, y, z;
        GetEulerInverted(&x, &y, &z);
        float desired = targetEuler.z - remainingFraction * angleDelta.z;
        CVector turn;
        turn.z = GetAngleDifference(z, desired) * g_objectTurnVelocityScale;
        turn.x = 0;
        turn.y = 0;
        if (turn.z > 0.001f)
            turn.z = 0.001f;
        else if (turn.z < g_objectNegativeTurnVelocityLimit)
            turn.z = g_objectNegativeTurnVelocityLimit;
        SetTurnSpeed(turn);
        GetMatrix(&current);
        GameQuaternionSlerp(&interpolatedQuaternion, &startQuaternion, &targetQuaternion, progress);
        GameQuaternionNormalize(&interpolatedQuaternion);
        GameQuaternionToMatrix(&interpolatedQuaternion, &current);
        ApplyMatrix(current);
    }
    else
    {
        GetMatrix(&current);
        ApplyMatrix(current);
    }
}

#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/textdraw.h"
#include <stdlib.h>
// 0x001e0498
void* g_objectSharedMaterialTexture = 0;
// 0x0011ee4c
extern const Scripting::OpcodeInfo g_objectDestroyCommand = {0x108, "i"};
// 0x000a79a0
void CObject::Add()
{
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
        GameWorldAdd(m_pGameEntity);
}
// 0x000a79f0
void CObject::Remove()
{
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
        GameWorldRemove(m_pGameEntity);
}
// 0x000a9020
CObject::~CObject()
{
    BYTE* state = (BYTE*)this;
    m_pGameEntity = GamePoolObjectGetAt(m_nHandle);
    // Native deliberately skips resource cleanup with no valid backend or on quit.
    if (!m_pGameEntity || *(DWORD*)m_pGameEntity == 0x863c40 || g_clientQuitRequested)
        return;
    if (GetRwObject())
        DeleteRwObject();
    Scripting::ProcessCommand(&g_objectDestroyCommand, m_nHandle);
    int model = modelIndex;
    if (model < 0 || !GameGetModelReferenceCount(model))
        g_pGame->ReleaseModel(modelIndex, 0);
    if (hasMaterials)
    {
        ShutdownMaterialText();
        for (int i = 0; i < 16; ++i)
        {
            void** texture = &materials.textures[i];
            if (*texture && *texture != g_objectSharedMaterialTexture)
            {
                GameDestroyTextDrawTexture(*texture);
                IDirect3DTexture9* raster = materials.rasterField34[i];
                *texture = 0;
                if (raster)
                    materials.rasterField34[i] = 0;
            }
            char** text = &materials.text[i];
            if (*text)
            {
                free(*text);
                *text = 0;
            }
        }
        if (g_deviceMaterialOverrides == state)
        {
            g_deviceMaterialOverrides = 0;
            g_deviceMaterialOverrideEnabled = 0;
        }
    }
}

#include "game/custom_model_io.h"
// 0x0011ede8
extern const Scripting::OpcodeInfo g_objectCreateCommand = {0x107, "ifffv"};
// 0x000a8bc0
void CObject::UpdateCachedCentre()
{
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if (m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40)
    {
        CMatrix matrix;
        GetMatrix(&matrix);
        CVector centre;
        if (GameGetModelCollisionCentre(modelIndex, &centre))
            GameTransformPoint(&worldCentre, &matrix, &centre);
        else
        {
            worldCentre.x = matrix.position.x;
            worldCentre.y = matrix.position.y;
            worldCentre.z = matrix.position.z;
        }
    }
}
// 0x000a8d00
CObject::CObject(int model, CVector position, CVector rotation, float drawDistance, BYTE lightingRequest, BYTE firstHour, BYTE lastHour) : CEntity()
{
    DWORD handle = 0;
    attachedVehicle = 0xffff;
    attachedObject = 0xffff;
    lightingPassRequested = lightingRequest;
    m_pGameEntity = 0;
    m_nHandle = 0;
    attachmentOffset.x = 0;
    attachmentOffset.y = 0;
    attachmentOffset.z = 0;
    attachmentRotation.x = 0;
    attachmentRotation.y = 0;
    attachmentRotation.z = 0;
    attachmentRotationSync = 1;
    unknown1b4 = 0;
    lightingPassEntered = 0;
    unknownb9 = 0;
    unknownba = 0;
    cameraCollisionDisabled = 0;
    timeVisibilityEnabled = 0;
    visibilityStartHour = 0;
    visibilityEndHour = 0;
    if (firstHour || lastHour)
    {
        timeVisibilityEnabled = 1;
        visibilityStartHour = firstHour;
        visibilityEndHour = lastHour;
    }
    if (!GameModelQueryB4170(model))
        model = 18631;
    void* info = GameGetModelInfo(model);
    drawLast = (GameGetModelFlagsByte(model) >> 1) & 1;
    if (drawDistance != 0.0f && info && GameIsObjectModelInfo(info))
        GameSetModelDrawDistance(info, drawDistance);
    if (model < 0)
    {
        // GTA SA virtual address: native negative-model create patch.
        GameUnprotect((void*)0x46977f, 1);
        *(BYTE*)0x46977f = 0x75;
    }
    Scripting::ProcessCommand(&g_objectCreateCommand, model, (double)position.x, (double)position.y, (double)position.z, &handle);
    if (model < 0)
        *(BYTE*)0x46977f = 0x7d;
    if (handle && GamePoolObjectGetAt(handle))
    {
        m_pGameEntity = GamePoolObjectGetAt(handle);
        m_nHandle = handle;
        backendCreationMarker = 1;
        motionFlags = 0;
        moveSpeed = 0.0f;
        unknown1b6[0] = 0;
        lastMoveTick = 0;
        modelIndex = model;
        if (GameIsObjectModelInfo(info))
        {
            usesModelDrawDistance = 1;
            streamingDistance = GameGetModelDrawDistance(info);
        }
        else
        {
            usesModelDrawDistance = 0;
            memcpy(&streamingDistance, &drawDistance, 4);
        }
        GetMatrix(&baseMatrix);
        baseMatrix.position.x = position.x;
        baseMatrix.position.y = position.y;
        baseMatrix.position.z = position.z;
        SetMatrix(baseMatrix);
        SetRotation(&rotation);
        SetBackendHighFlag();
        renderReady = 0;
        streamingHideCount = 0;
        collisionRadius = GameGetModelCollisionRadius(model);
        UpdateCachedCentre();
    }
    materialPassState = 0;
    materials.frameObjectsProcessed = 0;
    hasMaterials = 0;
    materials.snapshotValid = 0;
    materials.rwObject = 0;
    materials.texturesChanged = 0;
    materials.colorsChanged = 0;
    materials.savedGeometryFlags = 0;
    unknown1195 = 0;
    for (int i = 0; i < 16; ++i)
    {
        materials.savedTextures[i] = 0;
        materials.textures[i] = 0;
        materials.colors[i] = 0;
        materials.savedColors[i] = 0;
        materials.types[i] = 0;
        materials.textProcessed[i] = 0;
        memset(&materials.textInfo[i], 0, sizeof(materials.textInfo[i]));
        materials.text[i] = 0;
        materials.rasterField34[i] = 0;
        materials.renderedText[i] = 0;
    }
}

// 0x000a79c0
BOOL CObject::IsAdded()
{
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    return m_pGameEntity && *(DWORD*)m_pGameEntity != 0x863c40 && *(DWORD*)((BYTE*)m_pGameEntity + 0x18) && backendCreationMarker ? TRUE : FALSE;
}
// Descriptive recovered lighting operation; original SA-MP spelling unconfirmed.
// 0x000a8c30
void CObject::SetupLighting()
{
    if (m_pEntity && m_pEntity->vtable != 0x863c40)
    {
        *(DWORD*)((BYTE*)m_pGameEntity + 0x1c) |= 0x10000000;
        DWORD table = m_pEntity->vtable;
        void* backend = m_pGameEntity;
        // GTA SetupLighting(slot19); reload backend after publishing bLightObject.
        // clang-format off
        __asm {
            mov eax,table
            mov ecx,backend
            call dword ptr [eax+0x4c]
        }
        // clang-format on
        // This marks entering the call, regardless of the GTA boolean result.
        lightingPassEntered = 1;
    }
}
// Descriptive recovered lighting operation; original SA-MP spelling unconfirmed.
// 0x000a8c70
void CObject::RemoveLighting()
{
    if (m_pEntity && m_pEntity->vtable != 0x863c40 && lightingPassEntered)
    {
        DWORD table = m_pEntity->vtable;
        void* backend = m_pGameEntity;
        // GTA RemoveLighting(slot20), with native callee-popped reset word1.
        // clang-format off
        __asm {
            push 1
            mov eax,table
            mov ecx,backend
            call dword ptr [eax+0x50]
        }
        // clang-format on
        lightingPassEntered = 0;
    }
}
// 0x000a8cc0
bool CObject::HasRwFrameFlag8()
{
    if (!m_pGameEntity || *(DWORD*)m_pGameEntity == 0x863c40)
        return false;
    BYTE* rw = (BYTE*)GetRwObject();
    if (!rw)
        return false;
    // Native follows parent, then requires an atomic (type1), not a frame(type0).
    // Preserve that unusual path instead of treating the parent as a normal RwFrame.
    ObjectRwAtomicPrefix* parent = (ObjectRwAtomicPrefix*)((ObjectRwObjectPrefix*)rw)->parent;
    if (!parent || parent->object.type != ObjectRwAtomicType)
        return false;
    ObjectRwGeometryPrefix* geometry = parent->geometry;
    if (geometry && (geometry->flags & ObjectRwGeometryPrelit))
        return true;
    return false;
}

// 0x000a7d00
void CObject::SetAttachedVehicle(WORD id, const CVector* offset, const CVector* rotation)
{
    if (id == 0xffff)
    {
        attachedVehicle = id;
        for (int i = 0; i < 6; ++i)
            ((DWORD*)&attachmentOffset)[i] = 0;
        return;
    }
    attachedVehicle = id;
    for (int i = 0; i < 3; ++i)
        ((DWORD*)&attachmentOffset)[i] = *(const DWORD*)((const BYTE*)offset + i * 4);
    for (int i = 0; i < 3; ++i)
        ((DWORD*)&attachmentRotation)[i] = *(const DWORD*)((const BYTE*)rotation + i * 4);
}
// 0x000a7d90
void CObject::SetAttachedObject(WORD id, const CVector* offset, const CVector* rotation, BYTE flag)
{
    if (id == 0xffff)
    {
        attachedObject = id;
        for (int i = 0; i < 5; ++i)
            ((DWORD*)&attachmentOffset)[i] = 0;
        attachmentRotationSync = 1;
        *(DWORD*)&attachmentRotation.z = 0;
        return;
    }
    attachedObject = id;
    for (int i = 0; i < 3; ++i)
        ((DWORD*)&attachmentOffset)[i] = *(const DWORD*)((const BYTE*)offset + i * 4);
    for (int i = 0; i < 2; ++i)
        ((DWORD*)&attachmentRotation)[i] = *(const DWORD*)((const BYTE*)rotation + i * 4);
    DWORD last = *(const DWORD*)((const BYTE*)rotation + 8);
    attachmentRotationSync = flag;
    *(DWORD*)&attachmentRotation.z = last;
}
// 0x000a80b0
BOOL CObject::IsMoving()
{
    WORD object = attachedObject;
    if (object != 0xffff)
    {
        if (g_pNetGame)
        {
            CNetGamePools* pools = ((CNetGame*)g_pNetGame)->pools;
            BYTE* pool = (BYTE*)pools->objects;
            // Native accepts id2100 as well; the adjacent-state layout is unproven.
            if (pool && object <= 2100 && *(DWORD*)(pool + 4 + object * 4))
            {
                CObject* attached = *(CObject**)(pool + 0x20d4 + object * 4);
                if (attached && (attached->motionFlags & 1))
                    return TRUE;
            }
        }
        return FALSE;
    }
    if (attachedVehicle != 0xffff)
        return TRUE;
    if (motionFlags & 1)
        return TRUE;
    return FALSE;
}

// 0x000a8430
void* __cdecl ObjectApplyMaterials(void* atomic, void* object)
{
    ObjectRwAtomicPrefix* a = (ObjectRwAtomicPrefix*)atomic;
    if (a->object.type != ObjectRwAtomicType)
        return atomic;
    if (a->geometry)
    {
        CObject* state = (CObject*)object;
        if (state)
        {
            int count = a->geometry->materialList.count;
            if (count > 16)
                count = 16;
            for (int i = 0; i < count; ++i)
            {
                ObjectRwMaterialPrefix* material = a->geometry->materialList.materials[i];
                if (!material)
                    continue;
                void* texture = state->materials.textures[i];
                if (texture)
                {
                    material->texture = texture;
                    state->materials.texturesChanged = 1;
                }
                if (state->materials.colors[i])
                {
                    a->geometry->flags |= ObjectRwGeometryModulateMaterialColor;
                    a->geometry->flags &= ~ObjectRwGeometryPrelit;
                    material->surface.ambient = 1.0f;
                    material->surface.specular = 0.0f;
                    material->surface.diffuse = 1.0f;
                    memcpy(&material->color, &state->materials.colors[i], sizeof(material->color));
                    state->materials.colorsChanged = 1;
                }
            }
        }
    }
    return atomic;
}
// 0x000a86c0
void* __cdecl ObjectSaveMaterials(void* atomic, void* object)
{
    ObjectRwAtomicPrefix* a = (ObjectRwAtomicPrefix*)atomic;
    if (a->object.type != ObjectRwAtomicType || !a->geometry)
        return atomic;
    CObject* state = (CObject*)object;
    if (!state)
        return atomic;
    int count = a->geometry->materialList.count;
    if (count > 16)
        count = 16;
    state->materials.savedGeometryFlags = a->geometry->flags;
    for (int i = 0; i < count; ++i)
    {
        ObjectRwMaterialPrefix* material = a->geometry->materialList.materials[i];
        if (!material)
            continue;
        state->materials.savedTextures[i] = material->texture;
        memcpy(&state->materials.savedColors[i], &material->color, sizeof(material->color));
    }
    state->materials.snapshotValid = 1;
    return atomic;
}
// 0x000a8740
void* __cdecl ObjectRestoreMaterials(void* atomic, void* object)
{
    ObjectRwAtomicPrefix* a = (ObjectRwAtomicPrefix*)atomic;
    if (a->object.type != ObjectRwAtomicType || !a->geometry)
        return atomic;
    CObject* state = (CObject*)object;
    if (!state)
        return atomic;
    int count = a->geometry->materialList.count;
    if (count > 16)
        count = 16;
    for (int i = 0; i < count; ++i)
    {
        ObjectRwMaterialPrefix* material = a->geometry->materialList.materials[i];
        if (!material)
            continue;
        if (state->materials.textures[i] && material->texture != state->materials.savedTextures[i])
            material->texture = state->materials.savedTextures[i];
        if (state->materials.colorsChanged && state->materials.colors[i])
            memcpy(&material->color, &state->materials.savedColors[i], sizeof(material->color));
    }
    a->geometry->flags = state->materials.savedGeometryFlags;
    state->materials.colorsChanged = 0;
    state->materials.texturesChanged = 0;
    return atomic;
}

// 0x000a87f0
void CObject::ProcessMaterials()
{
    BYTE* state = (BYTE*)this;
    if (!hasMaterials)
        return;
    BYTE* rw = (BYTE*)GetRwObject();
    if (!rw)
        return;
    if (materials.rwObject != rw)
    {
        materials.snapshotValid = 0;
        void* frame = ((ObjectRwObjectPrefix*)rw)->parent;
        if (!frame)
            return;
        GameRwForAllFrameObjects(frame, ObjectSaveMaterials, this);
        materials.rwObject = rw;
        materialPassState = 0;
        return;
    }
    g_deviceMaterialOverrideEnabled = 1;
    g_deviceMaterialOverrides = state;
    void* frame = ((ObjectRwObjectPrefix*)rw)->parent;
    if (frame)
        GameRwForAllFrameObjects(frame, ObjectApplyMaterials, this);
}

#include "game/rw_preview.h"
// 0x000a82a0
void CObject::SetMaterialText(int index, const char* text, BYTE size, const char* font, BYTE fontSize, BYTE bold, DWORD fontColor, DWORD backgroundColor, BYTE alignment)
{
    if (index > 16)
        return;
    void** texture = &materials.textures[index];
    if (*texture)
    {
        GameDestroyTextDrawTexture(*texture);
        IDirect3DTexture9* raster = materials.rasterField34[index];
        *texture = 0;
        if (raster)
            materials.rasterField34[index] = 0;
    }
    IDirect3DTexture9** rendered = &materials.renderedText[index];
    if (*rendered)
    {
        (*rendered)->Release();
        *rendered = 0;
        materials.textProcessed[index] = 0;
    }
    char** stored = &materials.text[index];
    if (*stored)
    {
        free(*stored);
        *stored = 0;
    }
    *texture = GameRwCreateTexture(GameRwCreateRaster(2, 2, 0, 0));
    if (!*texture)
        return;
    ++((ObjectRwTexturePrefix*)*texture)->filterAddressing;
    *stored = (char*)calloc(1, strlen(text) + 1);
    strcpy(*stored, text);
    materials.textInfo[index].index = (BYTE)index;
    materials.textInfo[index].size = size;
    strcpy(materials.textInfo[index].font, font);
    materials.textInfo[index].fontSize = fontSize;
    materials.textInfo[index].bold = bold != 0;
    materials.textInfo[index].fontColor = fontColor;
    materials.textInfo[index].backgroundColor = backgroundColor;
    materials.textInfo[index].alignment = alignment;
    void* materialTexture = *texture;
    materials.colors[index] = 0;
    materials.types[index] = 2;
    void* raster = ((ObjectRwTexturePrefix*)materialTexture)->raster;
    if (raster)
        materials.rasterField34[index] = (IDirect3DTexture9*)GameRwReadField34(raster);
    hasMaterials = 1;
}

#include "game/effects.h"
// 0x000a8120
void CObject::SetMaterial(int model, int index, const char* dictionary, const char* texture, DWORD color)
{
    if (index > 16)
        return;
    if (model > 0 && !GameModelQueryB4170(model))
        return;
    if (materials.textures[index])
    {
        GameDestroyTextDrawTexture(materials.textures[index]);
        IDirect3DTexture9* raster = materials.rasterField34[index];
        materials.textures[index] = 0;
        if (raster)
            materials.rasterField34[index] = 0;
    }
    if (model > 0)
    {
        if (!g_pGame->IsModelAvailable(model))
        {
            g_pGame->RequestModel(model, 2);
            g_pGame->LoadRequestedModels();
            int waits = 0;
            while (!g_pGame->IsModelAvailable(model))
            {
                if (++waits > 250)
                {
                    materials.textures[index] = 0;
                    goto releaseModel;
                }
                Sleep(2);
            }
        }
        materials.textures[index] = GameLoadDictionaryTexture(dictionary, texture);
    releaseModel:
        if (!GameGetModelReferenceCount(model) && g_pGame->IsModelAvailable(model))
            g_pGame->ReleaseModel(model, 0);
    }
    else if (model == 0)
    {
        if (!g_objectSharedMaterialTexture)
        {
            GameLoadTextureDictionary("blanktex", "blanktex", 1);
            g_objectSharedMaterialTexture = GameLoadDictionaryTexture("blanktex", "blanktex");
        }
        materials.textures[index] = g_objectSharedMaterialTexture;
    }
    materials.colors[index] = color;
    materials.types[index] = 1;
    hasMaterials = 1;
}

// 0x000a9a20
void CObject::BeforeMaterialRender()
{
    if (hasMaterials && materialPassState)
    {
        ProcessMaterials();
        if (lightingPassRequested)
            SetupLighting();
    }
    materialPassState = 1;
}
// 0x000a9a60
void CObject::AfterMaterialRender()
{
    if (!hasMaterials)
        return;
    if (lightingPassRequested)
        RemoveLighting();
    g_deviceMaterialOverrideEnabled = 0;
    g_deviceMaterialOverrides = 0;
    if (!materials.texturesChanged && !materials.colorsChanged)
        return;
    BYTE* rw = (BYTE*)GetRwObject();
    if (!rw || materials.rwObject != rw)
        return;
    void* frame = ((ObjectRwObjectPrefix*)rw)->parent;
    if (frame)
        GameRwForAllFrameObjects(frame, ObjectRestoreMaterials, this);
}

// 0x001e0450
CMatrix g_objectAttachmentMatrix;
// 0x001e049c
CVector g_objectAttachmentZeroSpeed = {0, 0, 0};
// 0x000a7a10
void CObject::UpdateAttachedObjectRotation()
{
    if (attachedObject == 0xffff || attachmentRotationSync)
        return;
    GetMatrix(&g_objectAttachmentMatrix);
    g_objectAttachmentMatrix.right = baseMatrix.right;
    g_objectAttachmentMatrix.up = baseMatrix.up;
    g_objectAttachmentMatrix.at = baseMatrix.at;
    GameRotateRwMatrixOnAxis(&g_objectAttachmentMatrix, 0, attachmentRotation.x);
    GameRotateRwMatrixOnAxis(&g_objectAttachmentMatrix, 1, attachmentRotation.y);
    GameRotateRwMatrixOnAxis(&g_objectAttachmentMatrix, 2, attachmentRotation.z);
    SetMatrix(g_objectAttachmentMatrix);
    SetTurnSpeed(g_objectAttachmentZeroSpeed);
    SetSpeed(g_objectAttachmentZeroSpeed);
}

#include "game/camera.h"
#include "game/hooks.h"
// 0x0011ee80
extern const float g_objectRenderDistanceThreshold = 600.0f;
// 0x001e04a8
BYTE g_objectRenderFlag;
// Name follows the DL API map; original SA-MP spelling unconfirmed.
// 0x000a8b50
void CObject::Render()
{
    if (!m_pEntity)
        return;
    DWORD vtable = m_pEntity->vtable;
    if (vtable == 0x863c40)
        return;
    DWORD table = vtable;
    void* backend = m_pGameEntity;
    // GTA PreRender(slot17), then CRenderer::RenderOneNonRoad on the captured backend.
    // clang-format off
    __asm {
        mov eax,table
        mov ecx,backend
        call dword ptr [eax+0x44]
        push backend
        mov edx,0x553260
        call edx
        pop edx
    }
    // clang-format on
}
// Name follows the DL API map; original SA-MP spelling unconfirmed.
// 0x000a9b80
void CObject::Draw()
{
    // GTA SA virtual address: excluded backend vtable.
    if (!m_pGameEntity || *(DWORD*)m_pGameEntity == 0x863c40 || !GetRwObject())
        return;
    UpdateCachedCentre();
    CCamera* camera = *(CCamera**)((BYTE*)g_pGame + 4);
    CVector* centre = &worldCentre;
    float* radius = &collisionRadius;
    if (!(camera->GetDistanceToPoint(centre) < *radius) && !GameTestCameraSphere(centre, *radius))
        return;
    if (drawLast)
    {
        // GTA SA virtual address: native weather/render state gate.
        if (*(DWORD*)0xb72914 || (GameGetModelFlagsByte(modelIndex) & 8))
            ClientSetRenderState(30, 0);
        else
            ClientSetRenderState(30, 100);
    }
    if (GameGetModelFlagsByte(modelIndex) & 0x40)
        ClientSetRenderState(20, 2);
    else
        ClientSetRenderState(20, 1);
    if (hasMaterials && materialPassState)
        ProcessMaterials();
    GameSetupD3DFog(1);
    if (hasMaterials && materials.colorsChanged)
    {
        if (cameraDistance < g_objectRenderDistanceThreshold)
        {
            SetupLighting();
            GameRwRenderObject(GetRwObject());
            RemoveLighting();
        }
        else
            GameRwRenderObject(GetRwObject());
    }
    else if (cameraDistance < g_objectRenderDistanceThreshold)
        Render();
    else
        GameRwRenderObject(GetRwObject());
    if (hasMaterials && materialPassState)
        AfterMaterialRender();
    g_objectRenderFlag = 0;
    materialPassState = 1;
}

// 0x000a8b00
void* __cdecl ObjectFrameIdentity(void* object, void*)
{
    return object;
}
// 0x000a8b10
void CObject::ProcessFrameObjectsOnce()
{
    BYTE* rw = (BYTE*)GetRwObject();
    if (!rw || materials.frameObjectsProcessed)
        return;
    void* frame = ((ObjectRwObjectPrefix*)rw)->parent;
    if (!frame)
        return;
    GameRwForAllFrameObjects(frame, ObjectFrameIdentity, this);
    materials.frameObjectsProcessed = 1;
}
// 0x000a7f30
void CObject::AttachToBackend(void* target, const CVector* offset, const CVector* rotation)
{
    DWORD entity = (DWORD)m_pGameEntity;
    DWORD other = (DWORD)target;
    if (!entity || !other)
        return;
    float rx = rotation->x * g_objectDegreesToRadians;
    float ry = rotation->y * g_objectDegreesToRadians;
    float rz = rotation->z * g_objectDegreesToRadians;
    float x = offset->x, y = offset->y, z = offset->z;
    // GTA SA virtual address54D570: ECX receiver, seven callee-popped words.
    // clang-format off
    __asm {
        push rz
        push ry
        push rx
        push z
        push y
        push x
        push other
        mov ecx,entity
        mov edx,0x54d570
        call edx
    }
    // clang-format on
}
