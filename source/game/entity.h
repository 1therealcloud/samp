#ifndef SAMP_RECOVERED_ENTITY_H
#define SAMP_RECOVERED_ENTITY_H
#include <windows.h>
#include <stddef.h>
struct CVector { float x,y,z; };
class CMatrix { public: CVector right;DWORD flags;CVector up;DWORD pad1;CVector at;DWORD pad2;CVector position;DWORD pad3; };
#pragma pack(push,1)
// samp.dll vtable RVA: 0x0011e30c
class CEntity {
public:
    BYTE unknown04[60];
    void *m_pGameEntity;
    DWORD m_nHandle;
    // samp.dll RVA: 0x0009bce0
    CEntity();
    // samp.dll RVA: unknown (inline empty base destructor/vptr reset; scalar deleting entry RVA 0x0009bd10, standalone destructor entry not established)
    virtual ~CEntity() {}
    // samp.dll RVA: 0x0009f140
    virtual void Add();
    // samp.dll RVA: 0x0009f240
    virtual void Remove();
    // samp.dll RVA: 0x0009e950
    void GetMatrix(CMatrix *matrix);
    // samp.dll RVA: 0x0009ee80
    void Teleport(CVector position);
};
#pragma pack(pop)
typedef char EntityExtent[(sizeof(CEntity)==72)?1:-1];
typedef char EntityBackendOffset[(offsetof(CEntity,m_pGameEntity)==0x40)?1:-1];
// samp.dll RVA: 0x000b3720
void __stdcall GameWorldAdd(void *entity);
// samp.dll RVA: 0x000b3740
void __stdcall GameWorldRemove(void *entity);
#endif
