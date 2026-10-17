#include "events.h"
#include "game/vehicle.h"
#include "net/vehiclepool.h"
#include "main.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#pragma pack(push,1)
// Partial pointer views; allocation/lifecycle and other members remain unrecovered.
struct ScmPoolsView { BYTE unknown00[0xc];CVehiclePool *vehicles; };
struct ScmNetGameView {
    BYTE unknown00[0x2c];RakClientInterface *client;
    BYTE unknown30[0x3ae];ScmPoolsView *pools;
};
#pragma pack(pop)
typedef char ScmPoolsOffset[(offsetof(ScmNetGameView,pools)==0x3de)?1:-1];
// samp.dll RVA: 0x00117690
int g_scmEventRpcId=96;
// samp.dll RVA: 0x00156988
DWORD g_scmEventParameters[4];
// samp.dll RVA: 0x00001d10
void GameProcessOutgoingEvent(DWORD vehicleHandle,DWORD param2,DWORD param3,DWORD eventType) {
    if(g_pNetGame) {
        CVehiclePool *pool=((ScmNetGameView*)g_pNetGame)->pools->vehicles;
        RakNet::BitStream stream;
        if(eventType<1 || eventType>4)return;
        WORD id=pool->Find(GamePoolVehicleGetAt((int)vehicleHandle));
        CVehicle *vehicle=eventType==4?pool->Get(id):0;
        if(id==0xffff)return;
        stream.Write((DWORD)id);stream.Write(param2);stream.Write(param3);stream.Write(eventType);
        ((ScmNetGameView*)g_pNetGame)->client->RPC(&g_scmEventRpcId,&stream,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
        if(eventType==3) {
            vehicle=pool->Get(id);if(vehicle)vehicle->SetColor((BYTE)param2,(BYTE)param3);
        } else if(eventType==4 && vehicle && vehicle->m_pGameVehicle) {
            if(param2==0) {
                vehicle->SetColor(((BYTE*)vehicle->m_pGameVehicle)[0x434],((BYTE*)vehicle->m_pGameVehicle)[0x435]);
                vehicle->m_bInModShop=FALSE;
            } else vehicle->m_bInModShop=TRUE;
        }
    }
}
// samp.dll RVA: 0x000020c0
__declspec(naked) void GameSetEventOpcode() {
    __asm {
        pushad
        push 4
        mov ecx,esi
        mov ebx,0x464080
        call ebx
        mov eax,dword ptr ds:[0xa43c7c]
        mov g_scmEventParameters[4],eax
        mov ecx,dword ptr ds:[0xa43c80]
        mov g_scmEventParameters[8],ecx
        mov edx,dword ptr ds:[0xa43c84]
        mov g_scmEventParameters[12],edx
        mov eax,dword ptr ds:[0xa43c78]
        mov g_scmEventParameters[0],eax
        push g_scmEventParameters[0]
        push g_scmEventParameters[12]
        push g_scmEventParameters[8]
        push g_scmEventParameters[4]
        call GameProcessOutgoingEvent
        add esp,16
        popad
        mov ebx,0x47bf2b
        jmp ebx
    }
}
