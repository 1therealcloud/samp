#ifndef SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#define SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#include <windows.h>
// Native allocation extent0x206D; fields remain opaque except observed accesses.
// Constructor is recovered; directory/worker startup and complete ownership remain open.
class CCustomModels {public:
    BYTE nativeState[0x206d];
    // samp.dll RVA: 0x0000d890
    CCustomModels();
    // Clears transfer/worker storage only; neither frees nor clears the record array.
    // samp.dll RVA: 0x0000b990
    void ClearTransferStorage();
    // samp.dll RVA: 0x0000c4e0
    bool ResizeRecords(DWORD count);
    // samp.dll RVA: 0x0000c6a0
    void TrimRecords();
    // Clears a slot and trims; does not free the record itself.
    // samp.dll RVA: 0x0000cfc0
    bool EraseRecord(DWORD index);
    // samp.dll RVA: 0x0000cff0
    bool SetRecord(DWORD index,void *record);
    // Allocates before validation; invalid base IDs deliberately leak that allocation.
    // samp.dll RVA: 0x0000d550
    DWORD AddRecord(DWORD index,const void *record);
    // samp.dll RVA: 0x0000d5a0
    DWORD DeleteRecord(DWORD index);
    // samp.dll RVA: 0x0000d5e0
    void ClearRecords();
    // Signals six workers; does not join threads or free their storage.
    // samp.dll RVA: 0x0000bd10
    void StopWorkers();
    // samp.dll RVA: 0x0000bd90
    void ResumeReadyWorker();
    // samp.dll RVA: 0x0000bb10
    bool IsDffCached(DWORD hash);
    // samp.dll RVA: 0x0000bba0
    bool IsTxdCached(DWORD hash);
    // samp.dll RVA: 0x0000bd40
    bool PauseReadyWorker();
    // samp.dll RVA: 0x0000bde0
    int CountReadyWorkers();
    // samp.dll RVA: 0x0000c6e0
    bool IsDffRequested(DWORD hash);
    // samp.dll RVA: 0x0000c720
    bool IsTxdRequested(DWORD hash);
    // Index and size are deliberately unused by the native request body.
    // samp.dll RVA: 0x0000d270
    BOOL RequestResource(DWORD index,BYTE type,DWORD hash,DWORD size);
    // samp.dll RVA: 0x0000d390
    void Process();
    // Descriptive request bookkeeping; neither method starts a transfer itself.
    // samp.dll RVA: 0x0000c950
    bool HasEarlierResourceRequest(int endIndex,DWORD hash);
    // Returns-1 while disabled; marks flags and counts resources not requested by earlier entries.
    // samp.dll RVA: 0x0000c9b0
    int MarkResourceRequests();
    // Native is deliberately a bare RET4; no object unload is performed here.
    // samp.dll RVA: 0x0000cd40
    void ReleaseObjectEntry(int model);
    // Descriptive query names follow observed record type bytes 1 and 2.
    // samp.dll RVA: 0x0000cc80
    bool IsPedEntry(int model);
    // samp.dll RVA: 0x0000ccc0
    bool IsObjectEntry(int model);
    // Returns the raw record byte, not a normalized boolean.
    // samp.dll RVA: 0x0000cd00
    BYTE GetLoadedFlag(int model);
    // Native DoesExist has loading side effects for type-1 entries.
    // samp.dll RVA: 0x0000ce30
    BOOL DoesExist(int model);
    // Raw time bytes of first matching type-2 record; null records skipped.
    // samp.dll RVA: 0x0000cf00
    BYTE GetObjectFirstHour(int model);
    // samp.dll RVA: 0x0000cf40
    BYTE GetObjectLastHour(int model);
    // First matching type-2 record, skipping null records; raw TXD hash at +4A.
    // samp.dll RVA: 0x0000cdf0
    DWORD GetObjectTextureHash(int model);
    // samp.dll RVA: 0x0000ce90
    BOOL LoadObjectEntry(int model);
    // ECX is this; four DWORD stack arguments, callee pops 16 bytes.
    // samp.dll RVA: 0x0000c1c0
    BOOL LoadPedModel(DWORD base,DWORD model,DWORD dffHash,DWORD txdHash);
    // samp.dll RVA: 0x0000c2e0
    BOOL LoadObjectModel(DWORD base,DWORD model,DWORD dffHash,DWORD txdHash);
};
typedef char NativeCustomModelsExtent[(sizeof(CCustomModels)==0x206d)?1:-1];
// samp.dll RVA: 0x002aca28
extern CCustomModels *g_customModels;
// samp.dll RVA: 0x00117c48
extern const char g_customDffCachePattern[];
// samp.dll RVA: 0x00117c54
extern const char g_customTxdCachePattern[];
// samp.dll RVA: 0x00117d60
extern int g_customRequestDffRpc;
// samp.dll RVA: 0x00117d5c
extern int g_customRequestTxdRpc;
// samp.dll RVA: 0x00117d64
extern int g_customRequestsCompleteRpc;
#endif
