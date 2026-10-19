#include "game/playerped.h"
#include "game/keystuff.h"
#include "game/ped_commands.h"
#include <string.h>
// samp.dll RVA: 0x0011f15c
const Scripting::OpcodeInfo g_pedDetachParachute={0x682,"ifffi"};
// samp.dll RVA: 0x0011f148
const Scripting::OpcodeInfo g_pedDestroyParachute={0x9a2,"i"};
// samp.dll RVA: 0x0011f238
const Scripting::OpcodeInfo g_pedDropStuff={0x70a,"iifffiissi"};
// samp.dll RVA: 0x0011efcc
const Scripting::OpcodeInfo g_pedWarpFromVehicle={0x362,"ifff"};
// samp.dll RVA: 0x0011f320
const char g_pedNullAnimation[5]="NULL";
// samp.dll RVA: 0x000b3770
void *__stdcall GamePoolPedGetAt(int handle) {
    // GTA SA virtual pool-pointer and thiscall backend addresses.
    return ((void*(__fastcall*)(void*,void*,int))0x404910)(*(void**)0xb74490,0,handle);
}
// samp.dll RVA: 0x000aea30
BOOL CPed::HasAccessory() {
    for(int i=0;i<10;++i)if(m_accessoryOccupied[i]==1)return TRUE;
    return FALSE;
}
// samp.dll RVA: 0x000b06b0
void CPed::DeleteAllAccessories() {
    for(int i=0;i<10;++i)if(m_accessoryOccupied[i]==1) {
        void *object=m_accessoryObjects[i];
        if(object) {
            // Observed scalar-deleting virtual entry; no complete CObject ABI assumed.
            ((void(__fastcall*)(void*,void*,int))(*(DWORD**)object)[0])(object,0,1);
            m_accessoryObjects[i]=0;
        }
        memset(&m_accessoryInfo[i],0,52);
        m_accessoryOccupied[i]=FALSE;
    }
}
// samp.dll RVA: 0x000ad800
void CPed::DropStuff() {
    if(m_stuffObject) {
        Scripting::ProcessCommand(&g_pedDropStuff,m_nHandle,m_stuffObject,
            0.0,0.0,0.0,6,16,g_pedNullAnimation,g_pedNullAnimation,0);
        m_stuffObject=0;
    }
    CMatrix matrix;GetMatrix(&matrix);
    Teleport(matrix.position);
    m_stuffType=0;
}
// samp.dll RVA: 0x000ac190
void CPed::WarpFromVehicle(float x,float y,float z) {
    if(GamePoolPedGetAt(m_nHandle) && m_pGamePed && (*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100))
        Scripting::ProcessCommand(&g_pedWarpFromVehicle,m_nHandle,(double)x,(double)y,(double)z);
}
// samp.dll RVA: 0x000b0ba0
void CPed::Destroy() {
    BYTE *original=(BYTE*)m_pGamePed;
    KeyStuff::ResetKeys(m_nPlayerNumber);
    GameAssignPlayerPed(m_nPlayerNumber,0);
    if(m_pGamePed && GamePoolPedGetAt(m_nHandle) && *(DWORD*)m_pGamePed!=0x863c40) {
        if(m_parachuteObject) {
            Scripting::ProcessCommand(&g_pedDetachParachute,m_parachuteObject,0.0,0.0,0.0,0);
            Scripting::ProcessCommand(&g_pedDestroyParachute,m_parachuteObject);
            m_parachuteObject=0;
        }
        if(HasAccessory())DeleteAllAccessories();
        if(m_stuffType)DropStuff();
        if(*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100)WarpFromVehicle(100.0f,100.0f,10.0f);
        *(DWORD*)(*(BYTE**)(original+0x480)+0x4c)=0;
        ((void(__fastcall*)(void*,void*,int))(*(DWORD**)original)[0])(original,0,1);
        m_pGamePed=0;m_pGameEntity=0;
        // Native successful deletion deliberately retains m_nHandle.
        return;
    }
    m_pGamePed=0;m_pGameEntity=0;m_nHandle=0;
}
