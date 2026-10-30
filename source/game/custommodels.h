#ifndef SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#define SAMP_CUSTOM_MODEL_ADDRESS_INTERFACE_H
#include <stddef.h>
#include <windows.h>
#pragma pack(push, 1)
struct CustomModelDownload
{
    void* file;
    BYTE type;
    DWORD downloaded, total, crc, hash, index;
    char temporaryPath[261];
};
struct CustomModelWorker
{
    DWORD id;
    BYTE active, paused, enabled, stop;
    DWORD thread;
    char url[257];
    BYTE failed;
    DWORD result;
    CustomModelDownload download;
};
#pragma pack(pop)
typedef char NativeCustomDownloadExtent[(sizeof(CustomModelDownload) == 0x11e) ? 1 : -1];
typedef char NativeCustomWorkerExtent[(sizeof(CustomModelWorker) == 0x230) ? 1 : -1];
typedef char NativeCustomWorkerDownloadOffset[(offsetof(CustomModelWorker, download) == 0x112) ? 1 : -1];
typedef char NativeCustomWorkerFailureOffset[(offsetof(CustomModelWorker, failed) == 0x10d) ? 1 : -1];
// Shared observed memory prefix for nine otherwise unidentified native entries.
// This view does not assign their original classes/template types to CCustomModels.
// Element values stay raw DWORDs; their pointer/value semantics are unconfirmed.
struct NativeModelArrayPrefix
{
    DWORD* slots;
    DWORD count;
    // 0x0000c440
    DWORD Read0000c440(DWORD index);
    // 0x0000c460
    DWORD Count0000c460();
    // 0x0000c470
    DWORD Read0000c470(DWORD index);
    // 0x0000c490
    DWORD Count0000c490();
    // 0x0000c4a0
    NativeModelArrayPrefix* Initialize0000c4a0();
    // 0x0000c4b0
    DWORD Read0000c4b0(DWORD index);
    // 0x0000c4d0
    DWORD Count0000c4d0();
    // 0x000bbc60
    NativeModelArrayPrefix* Initialize000bbc60();
    // 0x000bbc70
    NativeModelArrayPrefix* Initialize000bbc70();
};
typedef char NativeModelArrayPrefixExtent[(sizeof(NativeModelArrayPrefix) == 8) ? 1 : -1];
typedef char NativeModelArrayCountOffset[(offsetof(NativeModelArrayPrefix, count) == 4) ? 1 : -1];
// Native allocation extent0x206D; fields remain opaque except observed accesses.
// Download/validation paths are recovered; full native class coverage is not established.
class CCustomModels
{
public:
    // Record-array prefix initializer; does not free storage or reset download state.
    // 0x0000c400
    CCustomModels* InitializeRecordStorage();
    // Reloads the live record count after each entry; null entries are unchecked.
    // 0x0000cd50
    void ProcessDelayedReleases();
    // First matching type-2 record, skipping null records; raw DFF hash at +46.
    // 0x0000cdb0
    DWORD GetObjectDffHash(int model);
    // 0x0000cf90
    void ClearRecordStorage();
    // 0x0000beb0
    DWORD CountEnabledActiveWorkers();
    // 0x0000bf40
    DWORD CountEnabledPausedWorkers();

    // 0x0000c410
    void* GetRecord(DWORD index);
    // 0x00008200
    DWORD GetActiveVirtualWorld();
    // Unchanged worlds retain both completion/stop flags.
    // 0x00017820
    void SetActiveVirtualWorld(DWORD world);
    // 0x00008210
    BYTE GetStopFlag();
    // 0x0000e400
    void RequestStop();

    // 0x0000c430
    DWORD GetCount();

    BYTE nativeState[0x206d];
    // 0x0000d070
    void ProcessCompressedChunk(BYTE stage, DWORD unpackedLength, const void* data, DWORD length);
    // 0x0000da80
    bool VerifyDff(const char* filename, DWORD hash);
    // 0x0000bfd0
    bool VerifyTxd(const char* filename, DWORD hash);
    // 0x0000dbd0
    void FinishDownload(CustomModelDownload* download);
    // 0x0000dff0
    bool StartWorkers();
    // 0x0000e0a0
    void QueueUrl(CustomModelDownload* download, const char* url);
    // 0x0000e180
    void BeginUrlDownload(BYTE type, DWORD hash, const char* url);
    // 0x0000c7e0
    int FindResourceIndex(BYTE type, DWORD hash);
    // 0x0000c830
    bool FillDownloadInfo(BYTE type, DWORD hash, CustomModelDownload* download);
    // Null download selects the embedded descriptor at222.
    // 0x0000bc30
    void WriteDownload(CustomModelDownload* download, const void* data, DWORD length);
    // 0x0000c760
    void AbortDownload(CustomModelDownload* download, int error, const char* detail);
    // 0x0000d1c0
    BOOL BeginDirectDownload(BYTE type, DWORD hash);
    // Includes null-file matches; does not check slot flags.
    // 0x0000bca0
    int FindWorkerByFile(void* file);
    // 0x0000c8e0
    int CountMissingResources();
    // 0x0000b9c0
    DWORD SanitizeCacheHost(char* output, DWORD capacity, const char* host);
    // 0x0000ba40
    void Initialize();
    // Eight copied digits are deliberately not explicitly terminated.
    // 0x0000c0b0
    DWORD ParseCacheHash(const char* filename);
    // 0x0000c100
    BOOL DeleteCachedDff(DWORD hash);
    // 0x0000c140
    BOOL DeleteCachedTxd(DWORD hash);
    // 0x0000c180
    BOOL DeleteTemporaryFile(const char* filename);
    // Disabled manifests deliberately report every hash as present.
    // 0x0000ca40
    bool HasResourceHash(DWORD hash);
    // 0x0000ca90
    void PurgeUnusedCache();
    // 0x0000cbf0
    void RefreshCachedResources();
    // 0x0000d030
    void SetExpectedRecordCount(DWORD count);
    // 0x0000d890
    CCustomModels();
    // Clears transfer/worker storage only; neither frees nor clears the record array.
    // 0x0000b990
    void ClearTransferStorage();
    // 0x0000c4e0
    bool ResizeRecords(DWORD count);
    // 0x0000c6a0
    void TrimRecords();
    // Clears a slot and trims; does not free the record itself.
    // 0x0000cfc0
    bool EraseRecord(DWORD index);
    // 0x0000cff0
    bool SetRecord(DWORD index, void* record);
    // Allocates before validation; invalid base IDs deliberately leak that allocation.
    // 0x0000d550
    DWORD AddRecord(DWORD index, const void* record);
    // 0x0000d5a0
    DWORD DeleteRecord(DWORD index);
    // 0x0000d5e0
    void ClearRecords();
    // Signals six workers; does not join threads or free their storage.
    // 0x0000bd10
    void StopWorkers();
    // 0x0000bd90
    void ResumeReadyWorker();
    // 0x0000bb10
    bool IsDffCached(DWORD hash);
    // 0x0000bba0
    bool IsTxdCached(DWORD hash);
    // 0x0000bd40
    bool PauseReadyWorker();
    // 0x0000bde0
    int CountReadyWorkers();
    // 0x0000c6e0
    bool IsDffRequested(DWORD hash);
    // 0x0000c720
    bool IsTxdRequested(DWORD hash);
    // Index and size are deliberately unused by the native request body.
    // 0x0000d270
    BOOL RequestResource(DWORD index, BYTE type, DWORD hash, DWORD size);
    // 0x0000d390
    void Process();
    // Descriptive request bookkeeping; neither method starts a transfer itself.
    // 0x0000c950
    bool HasEarlierResourceRequest(int endIndex, DWORD hash);
    // Returns-1 while disabled; marks flags and counts resources not requested by earlier entries.
    // 0x0000c9b0
    int MarkResourceRequests();
    // Native is deliberately a bare RET4; no object unload is performed here.
    // 0x0000cd40
    void ReleaseObjectEntry(int model);
    // Descriptive query names follow observed record type bytes 1 and 2.
    // 0x0000cc80
    bool IsPedEntry(int model);
    // 0x0000ccc0
    bool IsObjectEntry(int model);
    // Returns the raw record byte, not a normalized boolean.
    // 0x0000cd00
    BYTE GetLoadedFlag(int model);
    // Native DoesExist has loading side effects for type-1 entries.
    // 0x0000ce30
    BOOL DoesExist(int model);
    // Raw time bytes of first matching type-2 record; null records skipped.
    // 0x0000cf00
    BYTE GetObjectFirstHour(int model);
    // 0x0000cf40
    BYTE GetObjectLastHour(int model);
    // First matching type-2 record, skipping null records; raw TXD hash at +4A.
    // 0x0000cdf0
    DWORD GetObjectTextureHash(int model);
    // 0x0000ce90
    BOOL LoadObjectEntry(int model);
    // ECX is this; four DWORD stack arguments, callee pops 16 bytes.
    // 0x0000c1c0
    BOOL LoadPedModel(DWORD base, DWORD model, DWORD dffHash, DWORD txdHash);
    // 0x0000c2e0
    BOOL LoadObjectModel(DWORD base, DWORD model, DWORD dffHash, DWORD txdHash);
};
typedef char NativeCustomModelsExtent[(sizeof(CCustomModels) == 0x206d) ? 1 : -1];
// 0x002aca28
extern CCustomModels* g_customModels;
// 0x00117c48
extern const char g_customDffCachePattern[];
// 0x00117c54
extern const char g_customTxdCachePattern[];
// 0x00117d60
extern int g_customRequestDffRpc;
// 0x00117d5c
extern int g_customRequestTxdRpc;
// 0x00117d64
extern int g_customRequestsCompleteRpc;
// 0x00117cd8
extern const char g_customDeleteDffPattern[];
// 0x00117ce4
extern const char g_customDeleteTxdPattern[];
// 0x00117cf0
extern const char g_customDeleteTempPattern[];
// 0x00117d24
extern const char g_customFindTempPattern[];
// 0x00117d18
extern const char g_customFindDffPattern[];
// 0x00117d0c
extern const char g_customFindTxdPattern[];
// 0x00117c30
extern const char g_customServerCachePattern[];
// 0x00117c3c
extern const char g_customLocalCachePattern[];
// 0x00140378
extern const DWORD g_customCrcTable[256];
// 0x000628a0
DWORD __cdecl CustomModelCrc(DWORD seed, const void* data, DWORD length);
// Native loops on FILE EOF flag, without an ferror escape.
// 0x000628e0
DWORD __cdecl CustomModelFileCrc(const char* filename);
// 0x00156b44
extern CCustomModels* g_customDownloadManager;
// 0x0000bcd0
DWORD __cdecl CustomModelDownloadWrite(void* data, DWORD size, DWORD count, void* download);
// 0x00117cfc
extern const char g_customTemporaryPathPattern[];
// 0x00117d30
extern const char g_customDownloadWriteMode[];
// 0x00117d34
extern const char g_customFillDownloadError[];
// 0x00118e1c
extern const char g_customDownloadReadMode[];
// Selected LZMA allocator g_Alloc supplies native allocator-table role155478.
// 0x000c50d0
int __stdcall CustomModelLzmaDecode(BYTE* destination, DWORD* destinationLength, const BYTE* source, DWORD* sourceLength, const BYTE* properties, DWORD propertySize);
// 0x00117e1c
extern const char g_customDownloadSizeError[];
// 0x00117de8
extern const char g_customDownloadCrcError[];
// 0x0000de60
BOOL __cdecl CustomModelFetchUrl(CustomModelWorker* worker, const char* proxy);
// 0x0000df90
void __cdecl CustomModelDownloadThread(void* worker);
// 0x000c52d0
const char* CustomModelGetProxy();
// 0x002ac390
extern char g_customProxy[261];
// 0x0011f9c0
extern const char g_customRegistryKey[];
// 0x0011f9dc
extern const char g_customProxyValue[];
// 0x00117e58
extern const char g_customUserAgent[];
// 0x00117e9c
extern const char g_customLogUrlsOption[];
// 0x00117e8c
extern const char g_customUrlLogFormat[];
// 0x00117e64
extern const char g_customNoFreeWorkerError[];
// 0x00117ed8
extern const char g_customUrlInfoError[];
// 0x00117ea4
extern const char g_customUrlOpenError[];
#endif
