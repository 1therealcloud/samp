#ifndef SAMP_RECOVERED_GAME_EFFECTS_H
#define SAMP_RECOVERED_GAME_EFFECTS_H
#include "game/modelfile.h"
#include <windows.h>
// The effects parser uses the same23-byte native stream as DFF/TXD parsing.
// Its single implementation,including borrowed-buffer lifecycle,is in modelfile.
// 0x000b2790
BOOL GameLoadTextureDictionary(const char* name, const char* filename, BYTE fromSamp);
// 0x000b2860
void* GameLoadDictionaryTexture(const char* dictionary, const char* name);
// 0x000b3480
int GameTxdFind(const char* name);
// 0x000b3510
void GameTxdPush();
// 0x000b3520
void GameTxdPop();
// 0x000b3530
void GameTxdSetCurrent(int index);
// 0x000b3540
void* GameTxdReadTexture(const char* name);
// 0x000b34f0
void* GameTxdAddRef(int index);
// 0x000b3500
void GameTxdRemoveRef(int index);
// 0x000b35b0
void GameTxdRemove(int index);
// 0x000b35d0
void GameTxdRemoveSlot(int index);
// 0x000b35f0
DWORD GameTxdGetReferenceCount(int index);
// 0x000b3580
void GameTxdRemoveSlotByName(const char* name);
// 0x000b1ce0
void* GameRwGetGeometrySkin(void* geometry);
// 0x000b1da0
int GameRwRegisterClumpPlugin(DWORD pluginId);
// 0x000b1e00
int GameRwRegisterClumpPluginStream(DWORD pluginId, DWORD readCallback);
// 0x000b1e60
void GameRwEnableAutoMipmapping();
// 0x000b1ca0
void* GameRwGetFirstAtomic(void* clump);
// 0x000b1d40
void* GameRwGetClumpAnimHierarchy(void* clump);
// 0x000b1d70
void* GameRwCreateHierarchyAnimation(void* hierarchy);
#endif

// 0x000b1d20
void* GameRwGetClumpSkin(void* clump);
