#ifndef SAMP_RECOVERED_GAME_EFFECTS_H
#define SAMP_RECOVERED_GAME_EFFECTS_H
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#pragma pack(push,1)
class GameBufferReader {
public:
    const BYTE *data;DWORD position,length;bool eof,owns;
    DWORD unknown0e,unknown12;bool unknown16;
    // samp.dll RVA: unknown (inline reader initialization in native parser RVA 0x000b6610; standalone entry not established)
    GameBufferReader(const void *p,DWORD n) : data((const BYTE*)p),position(0),length(n),eof(false),owns(false),unknown0e(0),unknown12(0),unknown16(false) {}
    // samp.dll RVA: unknown (inline reader cleanup in native parser RVA 0x000b6610; standalone entry not established)
    ~GameBufferReader() { if(owns && data) { free((void*)data);data=0; } }
    // samp.dll RVA: 0x000bc1d0
    DWORD Read(void *out,DWORD count);
    // samp.dll RVA: unknown (inline position/EOF update in native parser; standalone entry not established)
    void Skip(DWORD count) { if(position+count<length)position+=count;else { position=length;eof=true; } }
};
#pragma pack(pop)
typedef char GameBufferReaderExtent[(sizeof(GameBufferReader)==23)?1:-1];
// samp.dll RVA: 0x000b2860
void *GameLoadDictionaryTexture(const char *dictionary,const char *name);
// samp.dll RVA: 0x000b3480
int GameTxdFind(const char *name);
// samp.dll RVA: 0x000b3510
void GameTxdPush();
// samp.dll RVA: 0x000b3520
void GameTxdPop();
// samp.dll RVA: 0x000b3530
void GameTxdSetCurrent(int index);
// samp.dll RVA: 0x000b3540
void *GameTxdReadTexture(const char *name);
// samp.dll RVA: 0x000b34f0
void *GameTxdAddRef(int index);
// samp.dll RVA: 0x000b3500
void GameTxdRemoveRef(int index);
// samp.dll RVA: 0x000b35b0
void GameTxdRemove(int index);
// samp.dll RVA: 0x000b35d0
void GameTxdRemoveSlot(int index);
// samp.dll RVA: 0x000b35f0
DWORD GameTxdGetReferenceCount(int index);
// samp.dll RVA: 0x000b3580
void GameTxdRemoveSlotByName(const char *name);
#endif
