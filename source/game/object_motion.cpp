#include "game/object.h"
#include "game/object_math.h"
#include "game/quaternion.h"
#include "net/localplayer.h"
#include "main.h"
#include <GetTime.h>
#include <string.h>
// samp.dll RVA: 0x0011ee7c
extern const float g_objectMotionRotationSentinel=-999.0f;
// samp.dll RVA: 0x000a7c20
void CObject::Stop(){CVector zero={0,0,0};SetSpeed(zero);SetTurnSpeed(zero);((BYTE*)this)[0x1af]&=0xfe;}
// samp.dll RVA: 0x000a7bf0
float CObject::GetAngleDifference(float from,float to){return GameNormalizeAngle(GameNormalizeAngle(to)-from);}
// samp.dll RVA: 0x000a97c0
void CObject::Move(CVector position,float speed,CVector rotation) {
    BYTE *state=(BYTE*)this;CMatrix matrix;GetMatrix(&matrix);
    if(state[0x1af]&1) {
        Stop();memcpy(&matrix.position,state+0x10b,12);
        if(state[0x1b5])GameQuaternionToMatrix((GameQuaternion*)(state+0x167),&matrix);
        ApplyMatrix(matrix);
    }
    DWORD tick=RakNet::GetTime();*(DWORD*)(state+0x15f)=tick;
    memcpy(state+0x10b,&position.x,4);memcpy(state+0x1b0,&speed,4);state[0x1af]|=1;
    memcpy(state+0x10f,&position.y,4);memcpy(state+0x113,&position.z,4);
    if(rotation.x>g_objectMotionRotationSentinel&&rotation.y>g_objectMotionRotationSentinel&&rotation.z>g_objectMotionRotationSentinel) {
        state[0x1b5]=1;float x,y,z;GetEulerInverted(&x,&y,&z);
        *(float*)(state+0x197)=GameNormalizeAngle(rotation.y);
        *(float*)(state+0x19b)=GameNormalizeAngle(rotation.x);
        *(float*)(state+0x19f)=GameNormalizeAngle(rotation.z);
        *(float*)(state+0x1a3)=GetAngleDifference(x,*(float*)(state+0x197));
        *(float*)(state+0x1a7)=GetAngleDifference(y,*(float*)(state+0x19b));
        *(float*)(state+0x1ab)=GetAngleDifference(z,*(float*)(state+0x19f));
        Rotate(rotation);CMatrix rotated;GetMatrix(&rotated);
        GameQuaternionFromMatrix(&rotated,(GameQuaternion*)(state+0x187));
        GameQuaternionFromMatrix((CMatrix*)(state+0xdb),(GameQuaternion*)(state+0x167));
        GameQuaternionNormalize((GameQuaternion*)(state+0x187));GameQuaternionNormalize((GameQuaternion*)(state+0x167));
    }else state[0x1b5]=0;
    *(float*)(state+0x15b)=GetDistanceToPoint(*(float*)(state+0x10b),*(float*)(state+0x10f),*(float*)(state+0x113));
    if(g_pNetGame){BYTE*pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);BYTE*players=*(BYTE**)(pools+8);if(players)(*(CLocalPlayer**)(players+0x1e))->ProcessSurfing();}
    ClearBackendFlag8();
}
