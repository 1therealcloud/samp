#ifndef MODEL_PREVIEW_H
#define MODEL_PREVIEW_H

#include "game/entity.h"

class CModelPreview
{
public:
    void *camera, *frame, *light, *texture, *colorRaster, *depthRaster;
    DWORD unknown18;

    CModelPreview();

    bool Reset();
    // Descriptive native test-scene entry; uses the preview's existing render target.

    void RenderTestScene(int model);

    void* RenderPed(int model, DWORD background, const CVector* rotation, float zoom);

    void* RenderVehicle(int model, DWORD background, const CVector* rotation, float zoom, int primary, int secondary);

    void* RenderObjectEntity(int model, DWORD background, const CVector* rotation, float zoom);

    void* RenderObject(int model, DWORD background, const CVector* rotation, float zoom);

    void Draw();
};
static_assert(sizeof(CModelPreview) == 28);

extern CModelPreview* g_modelPreview;

extern const float g_previewPedDistanceScale;

extern const float g_previewBoatDistance;

extern const float g_previewBoatRadiusScale;

extern const float g_previewVehicleDistance;

extern const float g_previewHeight;

extern const float g_previewObjectDistance;

extern const float g_previewObjectRadiusScale;

#endif // MODEL_PREVIEW_H
