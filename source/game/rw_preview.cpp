#include "game/rw_preview.h"
#include "game/input.h"
// samp.dll RVA: 0x0015535c
const CVector g_rwPreviewAxes[3]={{1,0,0},{0,1,0},{0,0,1}};
// samp.dll RVA: 0x000b1700
void *GameRwCreateLight(int type){return ((void*(__cdecl*)(int))(g_gameVersion==1?0x752110:0x752160))(type);}
// samp.dll RVA: 0x000b1770
void GameRwSetLightColor(void *light,const float *rgba){((void(__cdecl*)(void*,const float*))(g_gameVersion==1?0x751a90:0x751ae0))(light,rgba);}
// samp.dll RVA: 0x000b12d0
void *GameRwCreateRaster(int width,int height,int depth,int flags){return ((void*(__cdecl*)(int,int,int,int))(g_gameVersion==1?0x7fb230:0x7fb270))(width,height,depth,flags);}
// samp.dll RVA: 0x000b1430
void *GameRwCreateCamera(){return ((void*(__cdecl*)())(g_gameVersion==1?0x7ee4f0:0x7ee530))();}
// samp.dll RVA: 0x000b1390
void *GameRwCreateFrame(){return ((void*(__cdecl*)())(g_gameVersion==1?0x7f0410:0x7f0450))();}
// samp.dll RVA: 0x000b18f0
void GameRwTranslateFrame(void *frame,const CVector *position,int combine){((void(__cdecl*)(void*,const CVector*,int))(g_gameVersion==1?0x7f0e30:0x7f0e70))(frame,position,combine);}
// samp.dll RVA: 0x000b1920
void GameRwRotateFrame(void *frame,int axis,float angle){((void(__cdecl*)(void*,const CVector*,float,int))(g_gameVersion==1?0x7f1010:0x7f1050))(frame,g_rwPreviewAxes+axis,angle,1);}
// samp.dll RVA: 0x000b15b0
void GameRwSetCameraRasters(void *camera,void *color,void *depth){*(void**)((BYTE*)camera+0x60)=color;*(void**)((BYTE*)camera+0x64)=depth;}
// samp.dll RVA: 0x000b1580
void GameRwSetObjectFrame(void *object,void *frame){((void(__cdecl*)(void*,void*))(g_gameVersion==1?0x804ef0:0x804f30))(object,frame);}
// samp.dll RVA: 0x000b1680
void GameRwSetClipDistances(void *camera,float farClip,float nearClip){
    DWORD farAddress,nearAddress;if(g_gameVersion==1){farAddress=0x7ee2a0;nearAddress=0x7ee1d0;}else{farAddress=0x7ee2e0;nearAddress=0x7ee210;}
    ((void(__cdecl*)(void*,float))farAddress)(camera,farClip);((void(__cdecl*)(void*,float))nearAddress)(camera,nearClip);
}
// samp.dll RVA: 0x000b16d0
void GameRwSetViewWindow(void *camera,const float *window){((void(__cdecl*)(void*,const float*))(g_gameVersion==1?0x7ee410:0x7ee450))(camera,window);}
// samp.dll RVA: 0x000b18c0
void GameRwSetProjection(void *camera,int projection){((void(__cdecl*)(void*,int))(g_gameVersion==1?0x7ee3a0:0x7ee3e0))(camera,projection);}
// samp.dll RVA: 0x000b14b0
void GameRwAddCameraToWorld(void *camera){void *world=*(void**)0xc17038;if(world)((void(__cdecl*)(void*,void*))(g_gameVersion==1?0x750f20:0x750f70))(world,camera);}
// samp.dll RVA: 0x000b1320
void *GameRwCreateTexture(void *raster){return ((void*(__cdecl*)(void*))(g_gameVersion==1?0x7f37c0:0x7f3800))(raster);}
