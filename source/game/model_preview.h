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
    // Render bodies/lifecycle dependencies remain unrecovered.
    // samp.dll RVA: 0x0006bb80
    void *RenderPed(int model,DWORD background,const CVector *rotation,float zoom);
    // samp.dll RVA: 0x0006be00
    void *RenderVehicle(int model,DWORD background,const CVector *rotation,float zoom,int primary,int secondary);
    // samp.dll RVA: 0x0006c3f0
    void *RenderObject(int model,DWORD background,const CVector *rotation,float zoom);
};
#pragma pack(pop)
typedef char ModelPreviewExtent[(sizeof(CModelPreview)==28)?1:-1];
// samp.dll RVA: 0x002aca38
extern CModelPreview *g_modelPreview;
#endif
