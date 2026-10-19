#include "game/frame_limit.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/patches.h"
#include <GetTime.h>

// samp.dll RVA: 0x0018e520
BYTE g_frameKeyCycle;
// samp.dll RVA: 0x0018e524
DWORD g_frameKeyCycleTick;
// samp.dll RVA: 0x0018e528
DWORD g_frameSampleCount;
// samp.dll RVA: 0x0018e52c
DWORD g_frameSampleTick;
// samp.dll RVA: 0x0018e530
DWORD g_frameSleepTick;
// samp.dll RVA: 0x0018e534
float g_frameScale;
// samp.dll RVA: 0x001181d8
const float g_frameScaleMultiplier=3.0f;
// samp.dll RVA: 0x0011c664
const float g_frameScaleNumerator=50.0f;

// samp.dll RVA: 0x0009fe50
void GameUpdateKeyCycle() {
    DWORD now=RakNet::GetTime();
    if(!g_frameKeyCycleTick){g_frameKeyCycleTick=now;return;}
    if((LONG)(now-g_frameKeyCycleTick)>30) {
        ++g_frameKeyCycle;
        g_frameKeyCycleTick=now;
        if(g_frameKeyCycle==5)g_frameKeyCycle=0;
    }
    *(BYTE*)0xb7356e=g_frameKeyCycle;
}
// samp.dll RVA: 0x0009fed0
void CGame::Sleep(int elapsed,int limit) {
    if(elapsed && limit) {
        int period=1000/limit;
        if(elapsed<period)::Sleep((DWORD)period-(DWORD)elapsed-1u);
    }
}
// samp.dll RVA: 0x000b3ca0
DWORD GameGetTaskType(void *task) {
    DWORD result=0;
    if(task) {
        DWORD table=*(DWORD*)task;
        if(table>=0x800000 && table<=0x900000) {
            __asm {
                pushad
                mov ecx,task
                mov edx,[ecx]
                call dword ptr [edx+0x10]
                mov result,eax
                popad
            }
        }
    }
    return result;
}
// samp.dll RVA: 0x000ac8f0
BOOL CPed::HasTaskType268() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(ped && !(*(DWORD*)(ped+0x46c)&0x100)) {
        void *task=*(void**)(*(BYTE**)(ped+0x47c)+0xc);
        if(task)return GameGetTaskType(task)==0x10c;
    }
    return FALSE;
}
// samp.dll RVA: 0x000a1a30
void CGame::ProcessFrameLimiter() {
    DWORD now=RakNet::GetTime();
    if(!g_frameSampleTick) {
        *(DWORD*)0xb7cb5c=0x3f800000;*(DWORD*)0xb7cb58=0x3f800000;
        g_gamePatchValue1546f0=0x40400000;
        g_frameSampleTick=RakNet::GetTime();g_frameSleepTick=g_frameSampleTick;return;
    }
    GameUpdateKeyCycle();++g_frameSampleCount;
    if((LONG)(now-g_frameSampleTick)>250) {
        LONG frames=(LONG)(g_frameSampleCount*4u-4u);
        // Preserve the unrounded quotient through publication and multiplication.
        __asm {
            cmp frames,16
            jle minimum_scale
            fild frames
            fdivr g_frameScaleNumerator
            jmp publish_scale
        minimum_scale:
            fld g_frameScaleMultiplier
        publish_scale:
            fst g_frameScale
            fld st(0)
            fxch st(1)
            fstp dword ptr ds:[0xb7cb5c]
            fst dword ptr ds:[0xb7cb58]
            fmul g_frameScaleMultiplier
            fstp g_gamePatchValue1546f0
        }
        g_frameSampleTick=RakNet::GetTime();g_frameSampleCount=0;
    }
    CPed *ped=GetPlayerPed();
    DWORD elapsed;
    if(ped && GetPlayerPed()->HasTaskType268()) {
        if(*(BYTE*)0xba6794 && (DWORD)m_nFrameLimiter<=45) {
            Sleep((int)(now-g_frameSleepTick),m_nFrameLimiter);
        } else {
            elapsed=now-g_frameSleepTick;
            if(elapsed && (LONG)elapsed<22)::Sleep(21u-elapsed);
        }
    } else {
        elapsed=now-g_frameSleepTick;
        if(!*(BYTE*)0xba6794) {
            if(elapsed && (LONG)elapsed<11)::Sleep(10u-elapsed);
        } else if(elapsed && m_nFrameLimiter) {
            int period=1000/m_nFrameLimiter;
            if((LONG)elapsed<period)::Sleep((DWORD)period-elapsed-1u);
        }
    }
    g_frameSleepTick=RakNet::GetTime();
}
