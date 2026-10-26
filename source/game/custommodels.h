#ifndef SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#define SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#include <windows.h>
#include <stddef.h>
#pragma pack(push,1)
struct CustomModelDownload {
    void *file;BYTE type;DWORD downloaded,total,crc,hash,index;char temporaryPath[261];
};
struct CustomModelWorker {
    DWORD id;BYTE active,paused,enabled,stop;DWORD thread;char url[257];BYTE failed;DWORD result;CustomModelDownload download;
};
#pragma pack(pop)
typedef char NativeCustomDownloadExtent[(sizeof(CustomModelDownload)==0x11e)?1:-1];
typedef char NativeCustomWorkerExtent[(sizeof(CustomModelWorker)==0x230)?1:-1];
typedef char NativeCustomWorkerDownloadOffset[(offsetof(CustomModelWorker,download)==0x112)?1:-1];
typedef char NativeCustomWorkerFailureOffset[(offsetof(CustomModelWorker,failed)==0x10d)?1:-1];
// Native allocation extent0x206D; fields remain opaque except observed accesses.
// Worker startup is recovered; file validation/parser backends remain open.
class CCustomModels {public:
    BYTE nativeState[0x206d];
    // samp.dll RVA: 0x0000d070
    void ProcessCompressedChunk(BYTE stage,DWORD unpackedLength,const void *data,DWORD length);
    // DFF validator remains declaration-only until its parser backend is recovered.
    // samp.dll RVA: 0x0000da80
    bool VerifyDff(const char *filename,DWORD hash);
    // samp.dll RVA: 0x0000bfd0
    bool VerifyTxd(const char *filename,DWORD hash);
    // samp.dll RVA: 0x0000dbd0
    void FinishDownload(CustomModelDownload *download);
    // samp.dll RVA: 0x0000dff0
    bool StartWorkers();
    // samp.dll RVA: 0x0000e0a0
    void QueueUrl(CustomModelDownload *download,const char *url);
    // samp.dll RVA: 0x0000e180
    void BeginUrlDownload(BYTE type,DWORD hash,const char *url);
    // samp.dll RVA: 0x0000c7e0
    int FindResourceIndex(BYTE type,DWORD hash);
    // samp.dll RVA: 0x0000c830
    bool FillDownloadInfo(BYTE type,DWORD hash,CustomModelDownload *download);
    // Null download selects the embedded descriptor at222.
    // samp.dll RVA: 0x0000bc30
    void WriteDownload(CustomModelDownload *download,const void *data,DWORD length);
    // samp.dll RVA: 0x0000c760
    void AbortDownload(CustomModelDownload *download,int error,const char *detail);
    // samp.dll RVA: 0x0000d1c0
    BOOL BeginDirectDownload(BYTE type,DWORD hash);
    // Includes null-file matches; does not check slot flags.
    // samp.dll RVA: 0x0000bca0
    int FindWorkerByFile(void *file);
    // samp.dll RVA: 0x0000c8e0
    int CountMissingResources();
    // samp.dll RVA: 0x0000b9c0
    DWORD SanitizeCacheHost(char *output,DWORD capacity,const char *host);
    // samp.dll RVA: 0x0000ba40
    void Initialize();
    // Eight copied digits are deliberately not explicitly terminated.
    // samp.dll RVA: 0x0000c0b0
    DWORD ParseCacheHash(const char *filename);
    // samp.dll RVA: 0x0000c100
    BOOL DeleteCachedDff(DWORD hash);
    // samp.dll RVA: 0x0000c140
    BOOL DeleteCachedTxd(DWORD hash);
    // samp.dll RVA: 0x0000c180
    BOOL DeleteTemporaryFile(const char *filename);
    // Disabled manifests deliberately report every hash as present.
    // samp.dll RVA: 0x0000ca40
    bool HasResourceHash(DWORD hash);
    // samp.dll RVA: 0x0000ca90
    void PurgeUnusedCache();
    // samp.dll RVA: 0x0000cbf0
    void RefreshCachedResources();
    // samp.dll RVA: 0x0000d030
    void SetExpectedRecordCount(DWORD count);
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
// samp.dll RVA: 0x00117cd8
extern const char g_customDeleteDffPattern[];
// samp.dll RVA: 0x00117ce4
extern const char g_customDeleteTxdPattern[];
// samp.dll RVA: 0x00117cf0
extern const char g_customDeleteTempPattern[];
// samp.dll RVA: 0x00117d24
extern const char g_customFindTempPattern[];
// samp.dll RVA: 0x00117d18
extern const char g_customFindDffPattern[];
// samp.dll RVA: 0x00117d0c
extern const char g_customFindTxdPattern[];
// samp.dll RVA: 0x00117c30
extern const char g_customServerCachePattern[];
// samp.dll RVA: 0x00117c3c
extern const char g_customLocalCachePattern[];
// samp.dll RVA: 0x00140378
extern const DWORD g_customCrcTable[256];
// samp.dll RVA: 0x000628a0
DWORD __cdecl CustomModelCrc(DWORD seed,const void *data,DWORD length);
// Native loops on FILE EOF flag, without an ferror escape.
// samp.dll RVA: 0x000628e0
DWORD __cdecl CustomModelFileCrc(const char *filename);
// samp.dll RVA: 0x00156b44
extern CCustomModels *g_customDownloadManager;
// samp.dll RVA: 0x0000bcd0
DWORD __cdecl CustomModelDownloadWrite(void *data,DWORD size,DWORD count,void *download);
// samp.dll RVA: 0x00117cfc
extern const char g_customTemporaryPathPattern[];
// samp.dll RVA: 0x00117d30
extern const char g_customDownloadWriteMode[];
// samp.dll RVA: 0x00117d34
extern const char g_customFillDownloadError[];
// samp.dll RVA: 0x00118e1c
extern const char g_customDownloadReadMode[];
// Selected LZMA allocator g_Alloc supplies native allocator-table role155478.
// samp.dll RVA: 0x000c50d0
int __stdcall CustomModelLzmaDecode(BYTE *destination,DWORD *destinationLength,const BYTE *source,DWORD *sourceLength,const BYTE *properties,DWORD propertySize);
// samp.dll RVA: 0x00117e1c
extern const char g_customDownloadSizeError[];
// samp.dll RVA: 0x00117de8
extern const char g_customDownloadCrcError[];
// samp.dll RVA: 0x0000de60
BOOL __cdecl CustomModelFetchUrl(CustomModelWorker *worker,const char *proxy);
// samp.dll RVA: 0x0000df90
void __cdecl CustomModelDownloadThread(void *worker);
// samp.dll RVA: 0x000c52d0
const char *CustomModelGetProxy();
// samp.dll RVA: 0x002ac390
extern char g_customProxy[261];
// samp.dll RVA: 0x0011f9c0
extern const char g_customRegistryKey[];
// samp.dll RVA: 0x0011f9dc
extern const char g_customProxyValue[];
// samp.dll RVA: 0x00117e58
extern const char g_customUserAgent[];
// samp.dll RVA: 0x00117e9c
extern const char g_customLogUrlsOption[];
// samp.dll RVA: 0x00117e8c
extern const char g_customUrlLogFormat[];
// samp.dll RVA: 0x00117e64
extern const char g_customNoFreeWorkerError[];
// samp.dll RVA: 0x00117ed8
extern const char g_customUrlInfoError[];
// samp.dll RVA: 0x00117ea4
extern const char g_customUrlOpenError[];
#endif
