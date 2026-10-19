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
};
#pragma pack(pop)
typedef char ModelPreviewExtent[(sizeof(CModelPreview)==28)?1:-1];
// samp.dll RVA: 0x002aca38
extern CModelPreview *g_modelPreview;
#endif
