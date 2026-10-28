#include "game/model_preview.h"
#include "d3dhook/texture_binding.h"
#include "game/game.h"
#include "game/hooks_render.h"
#include "game/object.h"
#include "game/playerped.h"
#include "game/util.h"
#include "game/vehicle.h"
#include "gui/textdraw_texture.h"
#include "main.h"
#include <string.h>
// samp.dll RVA: 0x002aca38
CModelPreview* g_modelPreview;
// samp.dll RVA: 0x0006c700
CModelPreview::CModelPreview()
{
    camera = 0;
    frame = 0;
    texture = 0;
    unknown18 = 0;
    light = 0;
    Reset();
}
// samp.dll RVA: 0x0006b880
bool CModelPreview::Reset()
{
    light = GameRwCreateLight(2);
    if (light)
    {
        const float rgba[4] = {1, 1, 1, 1};
        GameRwSetLightColor(light, rgba);
        colorRaster = GameRwCreateRaster(256, 256, 0, 0x505);
        depthRaster = GameRwCreateRaster(256, 256, 0, 1);
        camera = GameRwCreateCamera();
        frame = GameRwCreateFrame();
        const CVector position = {0, 0, 50};
        GameRwTranslateFrame(frame, &position, 1);
        GameRwRotateFrame(frame, 0, 90.0f);
        if (camera && frame && colorRaster && depthRaster)
        {
            GameRwSetCameraRasters(camera, colorRaster, depthRaster);
            GameRwSetObjectFrame(camera, frame);
            GameRwSetClipDistances(camera, 300.0f, 0.01f);
            const float window[2] = {0.5f, 0.5f};
            GameRwSetViewWindow(camera, window);
            GameRwSetProjection(camera, 1);
            GameRwAddCameraToWorld(camera);
            texture = GameRwCreateTexture(colorRaster);
            return true;
        }
    }
    return false;
}

// samp.dll RVA: 0x0011c654
const float g_previewPedDistanceScale = -2.25f;
// samp.dll RVA: 0x0011c658
const float g_previewBoatDistance = -5.5f;
// samp.dll RVA: 0x0011c65c
const float g_previewBoatRadiusScale = 2.5f;
// samp.dll RVA: 0x0011c660
const float g_previewVehicleDistance = -1.0f;
// samp.dll RVA: 0x0011c664
const float g_previewHeight = 50.0f;
// samp.dll RVA: 0x0011c668
const float g_previewObjectDistance = -0.1f;
// samp.dll RVA: 0x0011c66c
const float g_previewObjectRadiusScale = 2.25f;

// samp.dll RVA: 0x0006b9d0
void CModelPreview::RenderTestScene(int model)
{
    if (!g_pGame->IsStarted() || !camera)
        return;
    DWORD background = 0xff330033;
    GameRwSetVisibilityCamera(camera);
    GameRwClearCamera(camera, &background, 3);
    GameRwBeginCameraUpdate(camera);
    GameRwWorldAddLight(light);
    ClientSetRenderState(6, 1);
    ClientSetRenderState(8, 1);
    ClientSetRenderState(7, 2);
    CVector position = {0, 0, 70.0f};
    CObject* object = new CObject(model, position, CVector(), 200.0f, 0, 0, 0);
    object->Add();
    position.z = 40.0f;
    object->Teleport(position);
    // GetMatrix deliberately leaves the four padding/flag words untouched.
    CMatrix matrix;
    object->GetMatrix(&matrix);
    GameRotateRwMatrixOnAxis(&matrix, 0, 50.0f);
    GameRotateRwMatrixOnAxis(&matrix, 2, -40.0f);
    object->ApplyMatrix(matrix);
    object->SetModelIndex(model);
    object->Native009f960();
    delete object;
    GameRwEndCameraUpdate(camera);
    GameRwWorldRemoveLight(light);
}

// samp.dll RVA: 0x0006bb80
void* CModelPreview::RenderPed(int model, DWORD background, const CVector* rotation, float zoom)
{
    void* raster = GameRwCreateRaster(256, 256, 0, 0x505);
    void* result = GameRwCreateTexture(raster);
    CPed* ped = new CPed(208, 0, CVector(), 0.0f);
    if (!raster || !result || !ped)
        return 0;
    // Native special height is DWORD 0x42489999 (the lower neighbour of 50.15f).
    CVector position = {0, zoom * g_previewPedDistanceScale, model == 162 ? 50.149997711181640625f : 50.05f};
    ped->Teleport(position);
    ped->SetModelIndex(model);
    ped->SetCollisionProcessed(FALSE);
    ped->SetCollisionFlag(FALSE);
    CMatrix matrix;
    memset(&matrix, 0, sizeof(matrix));
    ped->GetMatrix(&matrix);
    if (rotation->x != 0.0f)
        GameRotateRwMatrixOnAxis(&matrix, 0, rotation->x);
    if (rotation->y != 0.0f)
        GameRotateRwMatrixOnAxis(&matrix, 1, rotation->y);
    if (rotation->z != 0.0f)
        GameRotateRwMatrixOnAxis(&matrix, 2, rotation->z);
    ped->ApplyMatrix(matrix);
    GameRwSetCameraRasters(camera, raster, depthRaster);
    GameRwSetVisibilityCamera(camera);
    GameRwClearCamera(camera, &background, 3);
    GameRwBeginCameraUpdate(camera);
    GameRwWorldAddLight(light);
    ClientSetRenderState(6, 1);
    ClientSetRenderState(8, 1);
    ClientSetRenderState(7, 2);
    ClientSetRenderState(14, 0);
    ped->Add();
    ped->UpdateClumpAnimations(100.0f, 1);
    ped->Native009f960();
    GameRwEndCameraUpdate(camera);
    GameRwWorldRemoveLight(light);
    ped->Remove();
    delete ped;
    if (!GameGetModelReferenceCount(model))
        g_pGame->ReleaseModel(model, 0);
    return result;
}

// samp.dll RVA: 0x0006be00
void* CModelPreview::RenderVehicle(int model, DWORD background, const CVector* rotation, float zoom, int primary, int secondary)
{
    void* raster = GameRwCreateRaster(256, 256, 0, 0x505);
    void* result = GameRwCreateTexture(raster);
    if (model == 570)
        model = 538;
    else if (model == 569)
        model = 537;
    CVehicle* vehicle = new CVehicle(model, 0, 0, 50.0f, 0, 0, FALSE);
    if (!raster || !result || !vehicle)
        return 0;
    vehicle->SetCollisionProcessed(FALSE);
    vehicle->SetCollisionFlag(FALSE);
    float distance;
    // clang-format off
    // Preserve native extended ST0 through the two operations, then spill before GetSubtype.
    __asm {
        push model
        call GameGetModelCollisionRadius
        add esp,4
        fadd st(0),st(0)
        fsubr dword ptr [g_previewVehicleDistance]
        fmul dword ptr [zoom]
        fstp dword ptr [distance]
    }
    // clang-format on
    if (vehicle->GetSubtype() == 4)
    {
        // clang-format off
        __asm {
            push model
            call GameGetModelCollisionRadius
            add esp,4
            fmul dword ptr [g_previewBoatRadiusScale]
            fsubr dword ptr [g_previewBoatDistance]
            fstp dword ptr [distance]
        }
        // clang-format on
    }
    CVector position = {0, distance, 50.0f};
    vehicle->Teleport(position);
    if (primary != -1 && secondary != -1)
        vehicle->SetColor((BYTE)primary, (BYTE)secondary);
    CMatrix matrix;
    memset(&matrix, 0, sizeof(matrix));
    vehicle->GetMatrix(&matrix);
    if (rotation->x != 0.0f)
        GameRotateRwMatrixOnAxis(&matrix, 0, rotation->x);
    if (rotation->y != 0.0f)
        GameRotateRwMatrixOnAxis(&matrix, 1, rotation->y);
    if (rotation->z != 0.0f)
        GameRotateRwMatrixOnAxis(&matrix, 2, rotation->z);
    vehicle->ApplyMatrix(matrix);
    GameRwSetCameraRasters(camera, raster, depthRaster);
    GameRwSetVisibilityCamera(camera);
    GameRwClearCamera(camera, &background, 3);
    GameRwBeginCameraUpdate(camera);
    GameRwWorldAddLight(light);
    ClientSetRenderState(6, 1);
    ClientSetRenderState(8, 1);
    ClientSetRenderState(7, 2);
    ClientSetRenderState(14, 0);
    vehicle->Add();
    g_hookDisableFog = 1;
    vehicle->Native009f960();
    g_hookDisableFog = 0;
    GameRwEndCameraUpdate(camera);
    GameRwWorldRemoveLight(light);
    delete vehicle;
    return result;
}

// samp.dll RVA: 0x0006c0b0
void* CModelPreview::RenderObjectEntity(int model, DWORD background, const CVector* rotation, float zoom)
{
    void* raster = GameRwCreateRaster(256, 256, 0, 0x505);
    void* result = GameRwCreateTexture(raster);
    if (!raster || !result)
        return 0;
    if (model == 1373 || model == 3118 || model == 3552 || model == 3553)
        model = 18631;
    CVector initialPosition = {0, 0, 50.0f};
    CObject* object = new CObject(model, initialPosition, CVector(), 0, 0, 0, 0);
    if (!object || !object->m_pGameEntity)
        return result;
    float distance;
    // clang-format off
    __asm {
        push model
        call GameGetModelCollisionRadius
        add esp,4
        fmul dword ptr [g_previewObjectRadiusScale]
        fsubr dword ptr [g_previewObjectDistance]
        fmul dword ptr [zoom]
        fstp dword ptr [distance]
    }
    // clang-format on
    CMatrix matrix;
    memset(&matrix, 0, sizeof(matrix));
    object->GetMatrix(&matrix);
    if (model == 18631)
        GameRotateRwMatrixOnAxis(&matrix, 2, 180.0f);
    else
    {
        if (rotation->x != 0.0f)
            GameRotateRwMatrixOnAxis(&matrix, 0, rotation->x);
        if (rotation->y != 0.0f)
            GameRotateRwMatrixOnAxis(&matrix, 1, rotation->y);
        if (rotation->z != 0.0f)
            GameRotateRwMatrixOnAxis(&matrix, 2, rotation->z);
    }
    object->ApplyMatrix(matrix);
    CVector centre;
    object->GetBoundCentre(&centre);
    float height;
    // clang-format off
    __asm {
        fld dword ptr [centre.z]
        fsub dword ptr [g_previewHeight]
        fsubr dword ptr [g_previewHeight]
        fstp dword ptr [height]
    }
    // clang-format on
    CVector position = {-centre.x, distance, height};
    object->Teleport(position);
    BOOL changed = FALSE;
    if (!object->GetRwObject())
        changed = object->SetModelIndex(model);
    GameRwSetCameraRasters(camera, raster, depthRaster);
    GameRwSetVisibilityCamera(camera);
    GameRwClearCamera(camera, &background, 3);
    GameRwBeginCameraUpdate(camera);
    GameRwWorldAddLight(light);
    ClientSetRenderState(6, 1);
    ClientSetRenderState(8, 1);
    ClientSetRenderState(7, 2);
    ClientSetRenderState(14, 0);
    if (object->GetRwObject())
        object->Native009f960();
    GameGetModelInfo(model);
    GameRwEndCameraUpdate(camera);
    GameRwWorldRemoveLight(light);
    delete object;
    if (changed && !GameGetModelReferenceCount(model))
        g_pGame->ReleaseModel(model, 0);
    return result;
}

// samp.dll RVA: 0x0006c3f0
void* CModelPreview::RenderObject(int model, DWORD background, const CVector* rotation, float zoom)
{
    void* raster = GameRwCreateRaster(256, 256, 0, 0x505);
    void* result = GameRwCreateTexture(raster);
    if (!raster || !result)
        return 0;
    if (model == 1373 || model == 3118 || model == 3552 || model == 3553)
        model = 18631;
    bool requested = false;
    if (!g_pGame->IsModelAvailable(model))
    {
        requested = true;
        g_pGame->RequestModel(model, 2);
        g_pGame->LoadRequestedModels();
        while (!g_pGame->IsModelAvailable(model))
            Sleep(1);
    }
    void* object = (void*)GameQueryModelVirtual2c(model);
    if (!object)
        return result;
    float distance;
    // clang-format off
    __asm {
        push model
        call GameGetModelCollisionRadius
        add esp,4
        fmul dword ptr [g_previewObjectRadiusScale]
        fsubr dword ptr [g_previewObjectDistance]
        fmul dword ptr [zoom]
        fstp dword ptr [distance]
    }
    // clang-format on
    CVector centre = {0, 0, 0};
    GameGetModelCollisionCentre(model, &centre);
    void* objectFrame = *(void**)((BYTE*)object + 4);
    if (objectFrame)
    {
        CVector position = {-centre.x, distance, g_previewHeight - centre.z};
        GameRwTranslateFrame(objectFrame, &position, 1);
        if (model == 18631)
            GameRwRotateFrame(objectFrame, 2, 180.0f);
        else
        {
            if (rotation->x != 0.0f)
                GameRwRotateFrame(objectFrame, 0, rotation->x);
            if (rotation->y != 0.0f)
                GameRwRotateFrame(objectFrame, 1, rotation->y);
            if (rotation->z != 0.0f)
                GameRwRotateFrame(objectFrame, 2, rotation->z);
        }
    }
    GameRwSetCameraRasters(camera, raster, depthRaster);
    GameRwSetVisibilityCamera(camera);
    GameRwClearCamera(camera, &background, 3);
    GameRwBeginCameraUpdate(camera);
    GameRwWorldAddLight(light);
    ClientSetRenderState(6, 1);
    ClientSetRenderState(8, 1);
    ClientSetRenderState(7, 2);
    ClientSetRenderState(30, 0);
    ClientSetRenderState(20, 0);
    ClientSetRenderState(14, 0);
    GameRwRenderObject(object);
    GameReadBackendDword(GameGetModelInfo(model));
    GameRwEndCameraUpdate(camera);
    GameRwWorldRemoveLight(light);
    GameRwDestroyObject(object);
    if (requested)
        g_pGame->ReleaseModel(model, 0);
    return result;
}

// samp.dll RVA: 0x0006c6b0
void CModelPreview::Draw()
{
    if (texture)
    {
        DWORD color = 0xffffffff;
        TextDrawRect rect = {0, 512, 512, 0};
        GameDrawTextDrawTexture(texture, &rect, &color);
    }
}

#include "game/game.h"
#include "game/rw_preview.h"
// samp.dll RVA: 0x0015535c
const CVector g_rwPreviewAxes[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
// samp.dll RVA: 0x000b1700
void* GameRwCreateLight(int type)
{
    return ((void*(__cdecl*)(int))(g_gameVersion == 1 ? 0x752110 : 0x752160))(type);
}
// samp.dll RVA: 0x000b1770
void GameRwSetLightColor(void* light, const float* rgba)
{
    ((void(__cdecl*)(void*, const float*))(g_gameVersion == 1 ? 0x751a90 : 0x751ae0))(light, rgba);
}
// samp.dll RVA: 0x000b12d0
void* GameRwCreateRaster(int width, int height, int depth, int flags)
{
    return ((void*(__cdecl*)(int, int, int, int))(g_gameVersion == 1 ? 0x7fb230 : 0x7fb270))(width, height, depth, flags);
}
// samp.dll RVA: 0x000b1430
void* GameRwCreateCamera()
{
    return ((void*(__cdecl*)())(g_gameVersion == 1 ? 0x7ee4f0 : 0x7ee530))();
}
// samp.dll RVA: 0x000b1390
void* GameRwCreateFrame()
{
    return ((void*(__cdecl*)())(g_gameVersion == 1 ? 0x7f0410 : 0x7f0450))();
}
// samp.dll RVA: 0x000b18f0
void GameRwTranslateFrame(void* frame, const CVector* position, int combine)
{
    ((void(__cdecl*)(void*, const CVector*, int))(g_gameVersion == 1 ? 0x7f0e30 : 0x7f0e70))(frame, position, combine);
}
// samp.dll RVA: 0x000b1920
void GameRwRotateFrame(void* frame, int axis, float angle)
{
    ((void(__cdecl*)(void*, const CVector*, float, int))(g_gameVersion == 1 ? 0x7f1010 : 0x7f1050))(frame, g_rwPreviewAxes + axis, angle, 1);
}
// samp.dll RVA: 0x000b15b0
void GameRwSetCameraRasters(void* camera, void* color, void* depth)
{
    *(void**)((BYTE*)camera + 0x60) = color;
    *(void**)((BYTE*)camera + 0x64) = depth;
}
// samp.dll RVA: 0x000b1580
void GameRwSetObjectFrame(void* object, void* frame)
{
    ((void(__cdecl*)(void*, void*))(g_gameVersion == 1 ? 0x804ef0 : 0x804f30))(object, frame);
}
// samp.dll RVA: 0x000b1680
void GameRwSetClipDistances(void* camera, float farClip, float nearClip)
{
    DWORD farAddress, nearAddress;
    if (g_gameVersion == 1)
    {
        farAddress = 0x7ee2a0;
        nearAddress = 0x7ee1d0;
    }
    else
    {
        farAddress = 0x7ee2e0;
        nearAddress = 0x7ee210;
    }
    ((void(__cdecl*)(void*, float))farAddress)(camera, farClip);
    ((void(__cdecl*)(void*, float))nearAddress)(camera, nearClip);
}
// samp.dll RVA: 0x000b16d0
void GameRwSetViewWindow(void* camera, const float* window)
{
    ((void(__cdecl*)(void*, const float*))(g_gameVersion == 1 ? 0x7ee410 : 0x7ee450))(camera, window);
}
// samp.dll RVA: 0x000b18c0
void GameRwSetProjection(void* camera, int projection)
{
    ((void(__cdecl*)(void*, int))(g_gameVersion == 1 ? 0x7ee3a0 : 0x7ee3e0))(camera, projection);
}
// samp.dll RVA: 0x000b14b0
void GameRwAddCameraToWorld(void* camera)
{
    void* world = *(void**)0xc17038;
    if (world)
        ((void(__cdecl*)(void*, void*))(g_gameVersion == 1 ? 0x750f20 : 0x750f70))(world, camera);
}
// samp.dll RVA: 0x000b1320
void* GameRwCreateTexture(void* raster)
{
    return ((void*(__cdecl*)(void*))(g_gameVersion == 1 ? 0x7f37c0 : 0x7f3800))(raster);
}
