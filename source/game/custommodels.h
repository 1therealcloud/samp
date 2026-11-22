#ifndef CUSTOMMODELS_H
#define CUSTOMMODELS_H

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

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
STATIC_ASSERT(sizeof(CustomModelDownload) == 0x11e);
STATIC_ASSERT(sizeof(CustomModelWorker) == 0x230);
STATIC_ASSERT(offsetof(CustomModelWorker, download) == 0x112);
STATIC_ASSERT(offsetof(CustomModelWorker, failed) == 0x10d);
// Native allocation extent0x206D; fields remain opaque except observed accesses.
class CCustomModels
{
public:
    // Record-array prefix initializer; does not free storage or reset download state.
    // 0x0000c890
    CCustomModels* InitializeRecordStorage();
    // Reloads the live record count after each entry; null entries are unchecked.
    // R5 address to verify
    void ProcessDelayedReleases();
    // First matching type-2 record, skipping null records; raw DFF hash at +46.
    // R5 address to verify
    DWORD GetObjectDffHash(int model);
    // R5 address to verify
    void ClearRecordStorage();
    // R5 address to verify
    DWORD CountEnabledActiveWorkers();
    // R5 address to verify
    DWORD CountEnabledPausedWorkers();

    // R5 address to verify
    void* GetRecord(DWORD index);
    // R5 address to verify
    DWORD GetActiveVirtualWorld();
    // Unchanged worlds retain both completion/stop flags.
    // R5 address to verify
    void SetActiveVirtualWorld(DWORD world);
    // R5 address to verify
    BYTE GetStopFlag();
    // R5 address to verify
    void RequestStop();

    // R5 address to verify
    DWORD GetCount();

    BYTE nativeState[0x206d];
    // 0x0000d500
    void ProcessCompressedChunk(BYTE stage, DWORD unpackedLength, const void* data, DWORD length);
    // 0x0000e010
    bool VerifyDff(const char* filename, DWORD hash);
    // 0x0000c460
    bool VerifyTxd(const char* filename, DWORD hash);
    // 0x0000e160
    void FinishDownload(CustomModelDownload* download);
    // 0x0000c010
    bool StartWorkers();
    // 0x0000c380
    void QueueUrl(CustomModelDownload* download, const char* url);
    // 0x0000d650
    void BeginUrlDownload(BYTE type, DWORD hash, const char* url);
    // 0x0000cc70
    int FindResourceIndex(BYTE type, DWORD hash);
    // 0x0000ccc0
    bool FillDownloadInfo(BYTE type, DWORD hash, CustomModelDownload* download);
    // Null download selects the embedded descriptor at222.
    // 0x0000bf50
    void WriteDownload(CustomModelDownload* download, const void* data, DWORD length);
    // 0x0000cbf0
    void AbortDownload(CustomModelDownload* download, int error, const char* detail);
    // 0x0000d750
    BOOL BeginDirectDownload(BYTE type, DWORD hash);
    // Includes null-file matches; does not check slot flags.
    // R5 address to verify
    int FindWorkerByFile(void* file);
    // R5 address to verify
    int CountMissingResources();
    // 0x0000bce0
    DWORD SanitizeCacheHost(char* output, DWORD capacity, const char* host);
    // 0x0000bd60
    void Initialize();
    // Eight copied digits are deliberately not explicitly terminated.
    // 0x0000c540
    DWORD ParseCacheHash(const char* filename);
    // R5 address to verify
    BOOL DeleteCachedDff(DWORD hash);
    // R5 address to verify
    BOOL DeleteCachedTxd(DWORD hash);
    // R5 address to verify
    BOOL DeleteTemporaryFile(const char* filename);
    // Disabled manifests deliberately report every hash as present.
    // 0x0000ced0
    bool HasResourceHash(DWORD hash);
    // R5 address to verify
    void PurgeUnusedCache();
    // 0x0000d080
    void RefreshCachedResources();
    // 0x0000d4c0
    void SetExpectedRecordCount(DWORD count);
    // 0x0000de20
    CCustomModels();
    // Clears transfer/worker storage only; neither frees nor clears the record array.
    // 0x0000bcb0
    void ClearTransferStorage();
    // 0x0000c970
    bool ResizeRecords(DWORD count);
    // R5 address to verify
    void TrimRecords();
    // Clears a slot and trims; does not free the record itself.
    // 0x0000d450
    bool EraseRecord(DWORD index);
    // 0x0000d480
    bool SetRecord(DWORD index, void* record);
    // Allocates before validation; invalid base IDs deliberately leak that allocation.
    // 0x0000dae0
    DWORD AddRecord(DWORD index, const void* record);
    // 0x0000db30
    DWORD DeleteRecord(DWORD index);
    // R5 address to verify
    void ClearRecords();
    // Signals six workers; does not join threads or free their storage.
    // 0x0000c0c0
    void StopWorkers();
    // 0x0000c140
    void ResumeReadyWorker();
    // 0x0000be30
    bool IsDffCached(DWORD hash);
    // 0x0000bec0
    bool IsTxdCached(DWORD hash);
    // 0x0000c0f0
    bool PauseReadyWorker();
    // 0x0000c190
    int CountReadyWorkers();
    // 0x0000cb70
    bool IsDffRequested(DWORD hash);
    // 0x0000cbb0
    bool IsTxdRequested(DWORD hash);
    // Index and size are deliberately unused by the native request body.
    // 0x0000d800
    BOOL RequestResource(DWORD index, BYTE type, DWORD hash, DWORD size);
    // 0x0000d920
    void Process();
    // Descriptive request bookkeeping; neither method starts a transfer itself.
    // 0x0000cde0
    bool HasEarlierResourceRequest(int endIndex, DWORD hash);
    // Returns-1 while disabled; marks flags and counts resources not requested by earlier entries.
    // 0x0000ce40
    int MarkResourceRequests();
    // 0x0000d110
    bool IsPedEntry(int model);
    // 0x0000d150
    bool IsObjectEntry(int model);
    // Returns the raw record byte, not a normalized boolean.
    // 0x0000d190
    BYTE GetLoadedFlag(int model);
    // Native DoesExist has loading side effects for type-1 entries.
    // 0x0000d2c0
    BOOL DoesExist(int model);
    // Raw time bytes of first matching type-2 record; null records skipped.
    // 0x0000d390
    BYTE GetObjectFirstHour(int model);
    // 0x0000d3d0
    BYTE GetObjectLastHour(int model);
    // First matching type-2 record, skipping null records; raw TXD hash at +4A.
    // 0x0000d280
    DWORD GetObjectTextureHash(int model);
    // 0x0000d320
    BOOL LoadObjectEntry(int model);
    // ECX is this; four DWORD stack arguments, callee pops 16 bytes.
    // 0x0000c650
    BOOL LoadPedModel(DWORD base, DWORD model, DWORD dffHash, DWORD txdHash);
    // 0x0000c770
    BOOL LoadObjectModel(DWORD base, DWORD model, DWORD dffHash, DWORD txdHash);
};
STATIC_ASSERT(sizeof(CCustomModels) == 0x206d);
// 0x0026eb98
extern CCustomModels* g_customModels;
// 0x000e5bd8
extern const char g_customDffCachePattern[];
// 0x000e5be4
extern const char g_customTxdCachePattern[];
// R5 address to verify
extern int g_customRequestDffRpc;
// R5 address to verify
extern int g_customRequestTxdRpc;
// R5 address to verify
extern int g_customRequestsCompleteRpc;
// 0x000e5ca8
extern const char g_customDeleteDffPattern[];
// 0x000e5cb4
extern const char g_customDeleteTxdPattern[];
// 0x000e5cc0
extern const char g_customDeleteTempPattern[];
// 0x000e5cf4
extern const char g_customFindTempPattern[];
// 0x000e5ce8
extern const char g_customFindDffPattern[];
// 0x000e5cdc
extern const char g_customFindTxdPattern[];
// 0x000e5bc0
extern const char g_customServerCachePattern[];
// 0x000e5bcc
extern const char g_customLocalCachePattern[];
// 0x00102390
extern const DWORD g_customCrcTable[256];
// 0x00062e20
DWORD __cdecl CustomModelCrc(DWORD seed, const void* data, DWORD length);
// Native loops on FILE EOF flag, without an ferror escape.
// R5 address to verify
DWORD __cdecl CustomModelFileCrc(const char* filename);
// R5 address to verify
extern CCustomModels* g_customDownloadManager;
// R5 address to verify
DWORD __cdecl CustomModelDownloadWrite(void* data, DWORD size, DWORD count, void* download);
// 0x000e5ccc
extern const char g_customTemporaryPathPattern[];
// 0x000e5d34
extern const char g_customDownloadWriteMode[];
// 0x000e5d64
extern const char g_customFillDownloadError[];
// 0x000e6d8c
extern const char g_customDownloadReadMode[];
// Selected LZMA allocator g_Alloc supplies native allocator-table role155478.
// 0x000c3a00
int __stdcall CustomModelLzmaDecode(BYTE* destination, DWORD* destinationLength, const BYTE* source, DWORD* sourceLength, const BYTE* properties, DWORD propertySize);
// 0x000e5e4c
extern const char g_customDownloadSizeError[];
// 0x000e5e18
extern const char g_customDownloadCrcError[];
// 0x0000c000
void __cdecl CustomModelDownloadThread(void* worker);
// 0x000c3c00
const char* CustomModelGetProxy();
// R5 address to verify
extern char g_customProxy[261];
// 0x000ed8a4
extern const char g_customRegistryKey[];
// 0x000ed8c0
extern const char g_customProxyValue[];
// R5 address to verify
extern const char g_customUserAgent[];
// 0x000e5c28
extern const char g_customLogUrlsOption[];
// 0x000e5c18
extern const char g_customUrlLogFormat[];
// 0x000e5bf0
extern const char g_customNoFreeWorkerError[];
// 0x000e5d38
extern const char g_customUrlInfoError[];
// 0x000e5d00
extern const char g_customUrlOpenError[];
// 0x000b5dc0
BOOL GameFileExists(const char* path);
// 0x000a7bd0
BOOL GameRegisterCustomPed(int base, int model, const char* name, const char* dff, const char* txd);
// 0x000a7c30
BOOL GameRegisterCustomObject(int base, int model, const char* name, const char* dff, const char* txd);
// 0x000a7a80
void* GameClonePedModelInfo(int base, int model);
// 0x000a7ad0
void* GameCloneObjectModelInfo(int base, int model);
// 0x000a7b60
bool GameAttachModelTxd(int model, const char* name, const char* path);
// 0x000b1f10
void* GameLoadPedDff(const char* path);
// 0x000b1fc0
BOOL GameLoadObjectDff(const char* path, int model);
// 0x000b45d0
void __stdcall GamePublishPedClump(int model, void* clump);
// 0x000b38a0
int GameTxdAdd(const char* name);
// 0x000b38c0
DWORD GameTxdLoad(int index, const char* path);
// 0x000b4660
void __stdcall GameSetModelTxd(int model, int index);
// 0x000b1dc0
void* GameRwStreamOpen(int type, int access, const void* data);
// 0x000b1e00
BOOL GameRwStreamClose(void* stream, void* data);
// 0x000b1e80
BOOL GameRwFindChunk(void* stream, DWORD type, DWORD* length, DWORD* version);
// 0x000b1ed0
void* GameRwReadClump(void* stream);
// 0x000b1fa0
DWORD GameLoadAtomicStream(void* stream, int model);
// 0x000b4730
void __stdcall GameSetModelField8(int model, WORD value);
// 0x000b44e0
BOOL __stdcall GameIsObjectModelInfo(void* info);
// 0x001a25b0
extern DWORD g_gameLoadingCustomModel;

#endif // CUSTOMMODELS_H
