#ifndef CUSTOMMODELS_H
#define CUSTOMMODELS_H

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
static_assert(sizeof(CustomModelDownload) == 0x11e);
static_assert(sizeof(CustomModelWorker) == 0x230);
static_assert(offsetof(CustomModelWorker, download) == 0x112);
static_assert(offsetof(CustomModelWorker, failed) == 0x10d);
// Native allocation extent0x206D; fields remain opaque except observed accesses.
class CCustomModels
{
public:
    // Record-array prefix initializer; does not free storage or reset download state.

    CCustomModels* InitializeRecordStorage();
    // Reloads the live record count after each entry; null entries are unchecked.

    void ProcessDelayedReleases();
    // First matching type-2 record, skipping null records; raw DFF hash at +46.

    DWORD GetObjectDffHash(int model);

    void ClearRecordStorage();

    DWORD CountEnabledActiveWorkers();

    DWORD CountEnabledPausedWorkers();

    void* GetRecord(DWORD index);

    DWORD GetActiveVirtualWorld();
    // Unchanged worlds retain both completion/stop flags.

    void SetActiveVirtualWorld(DWORD world);

    BYTE GetStopFlag();

    void RequestStop();

    DWORD GetCount();

    BYTE nativeState[0x206d];

    void ProcessCompressedChunk(BYTE stage, DWORD unpackedLength, const void* data, DWORD length);

    bool VerifyDff(const char* filename, DWORD hash);

    bool VerifyTxd(const char* filename, DWORD hash);

    void FinishDownload(CustomModelDownload* download);

    bool StartWorkers();

    void QueueUrl(CustomModelDownload* download, const char* url);

    void BeginUrlDownload(BYTE type, DWORD hash, const char* url);

    int FindResourceIndex(BYTE type, DWORD hash);

    bool FillDownloadInfo(BYTE type, DWORD hash, CustomModelDownload* download);
    // Null download selects the embedded descriptor at222.

    void WriteDownload(CustomModelDownload* download, const void* data, DWORD length);

    void AbortDownload(CustomModelDownload* download, int error, const char* detail);

    BOOL BeginDirectDownload(BYTE type, DWORD hash);
    // Includes null-file matches; does not check slot flags.

    int FindWorkerByFile(void* file);

    int CountMissingResources();

    DWORD SanitizeCacheHost(char* output, DWORD capacity, const char* host);

    void Initialize();
    // Eight copied digits are deliberately not explicitly terminated.

    DWORD ParseCacheHash(const char* filename);

    BOOL DeleteCachedDff(DWORD hash);

    BOOL DeleteCachedTxd(DWORD hash);

    BOOL DeleteTemporaryFile(const char* filename);
    // Disabled manifests deliberately report every hash as present.

    bool HasResourceHash(DWORD hash);

    void PurgeUnusedCache();

    void RefreshCachedResources();

    void SetExpectedRecordCount(DWORD count);

    CCustomModels();
    // Clears transfer/worker storage only; neither frees nor clears the record array.

    void ClearTransferStorage();

    bool ResizeRecords(DWORD count);

    void TrimRecords();
    // Clears a slot and trims; does not free the record itself.

    bool EraseRecord(DWORD index);

    bool SetRecord(DWORD index, void* record);
    // Allocates before validation; invalid base IDs deliberately leak that allocation.

    DWORD AddRecord(DWORD index, const void* record);

    DWORD DeleteRecord(DWORD index);

    void ClearRecords();
    // Signals six workers; does not join threads or free their storage.

    void StopWorkers();

    void ResumeReadyWorker();

    bool IsDffCached(DWORD hash);

    bool IsTxdCached(DWORD hash);

    bool PauseReadyWorker();

    int CountReadyWorkers();

    bool IsDffRequested(DWORD hash);

    bool IsTxdRequested(DWORD hash);
    // Index and size are deliberately unused by the native request body.

    BOOL RequestResource(DWORD index, BYTE type, DWORD hash, DWORD size);

    void Process();
    // Descriptive request bookkeeping; neither method starts a transfer itself.

    bool HasEarlierResourceRequest(int endIndex, DWORD hash);
    // Returns-1 while disabled; marks flags and counts resources not requested by earlier entries.

    int MarkResourceRequests();

    bool IsPedEntry(int model);

    bool IsObjectEntry(int model);
    // Returns the raw record byte, not a normalized boolean.

    BYTE GetLoadedFlag(int model);
    // Native DoesExist has loading side effects for type-1 entries.

    BOOL DoesExist(int model);
    // Raw time bytes of first matching type-2 record; null records skipped.

    BYTE GetObjectFirstHour(int model);

    BYTE GetObjectLastHour(int model);
    // First matching type-2 record, skipping null records; raw TXD hash at +4A.

    DWORD GetObjectTextureHash(int model);

    BOOL LoadObjectEntry(int model);
    // ECX is this; four DWORD stack arguments, callee pops 16 bytes.

    BOOL LoadPedModel(DWORD base, DWORD model, DWORD dffHash, DWORD txdHash);

    BOOL LoadObjectModel(DWORD base, DWORD model, DWORD dffHash, DWORD txdHash);
};
static_assert(sizeof(CCustomModels) == 0x206d);

extern CCustomModels* g_customModels;

extern const char g_customDffCachePattern[];

extern const char g_customTxdCachePattern[];

extern int g_customRequestDffRpc;

extern int g_customRequestTxdRpc;

extern int g_customRequestsCompleteRpc;

extern const char g_customDeleteDffPattern[];

extern const char g_customDeleteTxdPattern[];

extern const char g_customDeleteTempPattern[];

extern const char g_customFindTempPattern[];

extern const char g_customFindDffPattern[];

extern const char g_customFindTxdPattern[];

extern const char g_customServerCachePattern[];

extern const char g_customLocalCachePattern[];

extern const DWORD g_customCrcTable[256];

DWORD __cdecl CustomModelCrc(DWORD seed, const void* data, DWORD length);
// Native loops on FILE EOF flag, without an ferror escape.

DWORD __cdecl CustomModelFileCrc(const char* filename);

extern CCustomModels* g_customDownloadManager;

DWORD __cdecl CustomModelDownloadWrite(void* data, DWORD size, DWORD count, void* download);

extern const char g_customTemporaryPathPattern[];

extern const char g_customDownloadWriteMode[];

extern const char g_customFillDownloadError[];

extern const char g_customDownloadReadMode[];
// Selected LZMA allocator g_Alloc supplies native allocator-table role155478.

int __stdcall CustomModelLzmaDecode(BYTE* destination, DWORD* destinationLength, const BYTE* source, DWORD* sourceLength, const BYTE* properties, DWORD propertySize);

extern const char g_customDownloadSizeError[];

extern const char g_customDownloadCrcError[];

void __cdecl CustomModelDownloadThread(void* worker);

const char* CustomModelGetProxy();

extern char g_customProxy[261];

extern const char g_customRegistryKey[];

extern const char g_customProxyValue[];

extern const char g_customUserAgent[];

extern const char g_customLogUrlsOption[];

extern const char g_customUrlLogFormat[];

extern const char g_customNoFreeWorkerError[];

extern const char g_customUrlInfoError[];

extern const char g_customUrlOpenError[];

BOOL GameFileExists(const char* path);

BOOL GameRegisterCustomPed(int base, int model, const char* name, const char* dff, const char* txd);

BOOL GameRegisterCustomObject(int base, int model, const char* name, const char* dff, const char* txd);

void* GameClonePedModelInfo(int base, int model);

void* GameCloneObjectModelInfo(int base, int model);

bool GameAttachModelTxd(int model, const char* name, const char* path);

void* GameLoadPedDff(const char* path);

BOOL GameLoadObjectDff(const char* path, int model);

void __stdcall GamePublishPedClump(int model, void* clump);

int GameTxdAdd(const char* name);

DWORD GameTxdLoad(int index, const char* path);

void __stdcall GameSetModelTxd(int model, int index);

void* GameRwStreamOpen(int type, int access, const void* data);

BOOL GameRwStreamClose(void* stream, void* data);

BOOL GameRwFindChunk(void* stream, DWORD type, DWORD* length, DWORD* version);

void* GameRwReadClump(void* stream);

DWORD GameLoadAtomicStream(void* stream, int model);

void __stdcall GameSetModelField8(int model, WORD value);

BOOL __stdcall GameIsObjectModelInfo(void* info);

extern DWORD g_gameLoadingCustomModel;

#endif // CUSTOMMODELS_H
