#ifndef SAMP_MODEL_PREVIEW_H
#define SAMP_MODEL_PREVIEW_H
#include "game/rw_preview.h"
#pragma pack(push,1)
// Descriptive recovered class name; original identity is not established.
class CModelPreview {public:
    void *camera,*frame,*light,*texture,*colorRaster,*depthRaster;DWORD unknown18;
    // samp.dll RVA: 0x0006c700
    CModelPreview();
    // samp.dll RVA: 0x0006b880
    bool Reset();
    // samp.dll RVA: 0x0006bb80
    void *RenderPed(int model,DWORD background,const CVector *rotation,float zoom);
    // samp.dll RVA: 0x0006be00
    void *RenderVehicle(int model,DWORD background,const CVector *rotation,float zoom,int primary,int secondary);
    // Descriptive name: constructs a CObject; the later renderer uses a raw RW object.
    // samp.dll RVA: 0x0006c0b0
    void *RenderObjectEntity(int model,DWORD background,const CVector *rotation,float zoom);
    // samp.dll RVA: 0x0006c3f0
    void *RenderObject(int model,DWORD background,const CVector *rotation,float zoom);
    // samp.dll RVA: 0x0006c6b0
    void Draw();
};
#pragma pack(pop)
typedef char ModelPreviewExtent[(sizeof(CModelPreview)==28)?1:-1];
// samp.dll RVA: 0x002aca38
extern CModelPreview *g_modelPreview;
// samp.dll RVA: 0x0011c654
extern const float g_previewPedDistanceScale;
// samp.dll RVA: 0x0011c658
extern const float g_previewBoatDistance;
// samp.dll RVA: 0x0011c65c
extern const float g_previewBoatRadiusScale;
// samp.dll RVA: 0x0011c660
extern const float g_previewVehicleDistance;
// samp.dll RVA: 0x0011c664
extern const float g_previewHeight;
// samp.dll RVA: 0x0011c668
extern const float g_previewObjectDistance;
// samp.dll RVA: 0x0011c66c
extern const float g_previewObjectRadiusScale;
#endif
