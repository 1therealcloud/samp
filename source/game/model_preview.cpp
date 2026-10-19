#include "game/model_preview.h"
// samp.dll RVA: 0x002aca38
CModelPreview *g_modelPreview;
// samp.dll RVA: 0x0006c700
CModelPreview::CModelPreview(){camera=0;frame=0;texture=0;unknown18=0;light=0;Reset();}
// samp.dll RVA: 0x0006b880
bool CModelPreview::Reset(){
    light=GameRwCreateLight(2);
    if(light){
        const float rgba[4]={1,1,1,1};GameRwSetLightColor(light,rgba);
        colorRaster=GameRwCreateRaster(256,256,0,0x505);depthRaster=GameRwCreateRaster(256,256,0,1);
        camera=GameRwCreateCamera();frame=GameRwCreateFrame();
        const CVector position={0,0,50};GameRwTranslateFrame(frame,&position,1);GameRwRotateFrame(frame,0,90.0f);
        if(camera&&frame&&colorRaster&&depthRaster){
            GameRwSetCameraRasters(camera,colorRaster,depthRaster);GameRwSetObjectFrame(camera,frame);
            GameRwSetClipDistances(camera,300.0f,0.01f);const float window[2]={0.5f,0.5f};GameRwSetViewWindow(camera,window);
            GameRwSetProjection(camera,1);GameRwAddCameraToWorld(camera);texture=GameRwCreateTexture(colorRaster);return true;
        }
    }
    return false;
}
