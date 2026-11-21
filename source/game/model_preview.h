#ifndef MODEL_PREVIEW_H
#define MODEL_PREVIEW_H

#include "game/entity.h"

#include <static_assert.h>

#pragma pack(push, 1)
class CModelPreview
{
public:
    void *camera, *frame, *light, *texture, *colorRaster, *depthRaster;
    DWORD unknown18;
    // 0x0006ccc0
    CModelPreview();
    // 0x0006be40
    bool Reset();
    // Descriptive native test-scene entry; uses the preview's existing render target.
    // 0x0006bf90
    void RenderTestScene(int model);
    // 0x0006c140
    void* RenderPed(int model, DWORD background, const CVector* rotation, float zoom);
    // 0x0006c3c0
    void* RenderVehicle(int model, DWORD background, const CVector* rotation, float zoom, int primary, int secondary);
    // 0x0006c670
    void* RenderObjectEntity(int model, DWORD background, const CVector* rotation, float zoom);
    // 0x0006c9b0
    void* RenderObject(int model, DWORD background, const CVector* rotation, float zoom);
    // R5 address to verify
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CModelPreview) == 28);
// R5 address to verify
extern CModelPreview* g_modelPreview;
// 0x000ea62c
extern const float g_previewPedDistanceScale;
// 0x000ea630
extern const float g_previewBoatDistance;
// 0x000ea634
extern const float g_previewBoatRadiusScale;
// 0x000ea638
extern const float g_previewVehicleDistance;
// 0x000ea63c
extern const float g_previewHeight;
// 0x000ea640
extern const float g_previewObjectDistance;
// 0x000ea644
extern const float g_previewObjectRadiusScale;

#endif // MODEL_PREVIEW_H
