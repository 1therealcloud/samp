#include "plugins.h"
#include "console.h"
#include "filterscripts.h"
#include "gamemodes.h"

#if defined(LINUX) && !defined(__LINUX__)
#define __LINUX__
#endif
#include "../tools/plugin-sdk/plugincommon.h"

#include <sstream>
#include <string.h>

#ifdef _WIN32
typedef HMODULE PluginHandle;
#define OpenPlugin(path) LoadLibraryA(path)
#define ClosePlugin(handle) FreeLibrary(handle)
#define PluginSymbol(handle, name) GetProcAddress(handle, name)
#else
#include <dlfcn.h>
typedef void* PluginHandle;
#define OpenPlugin(path) dlopen(path, RTLD_NOW)
#define ClosePlugin(handle) dlclose(handle)
#define PluginSymbol(handle, name) dlsym(handle, name)
#endif

struct CPlugins::Plugin
{
    PluginHandle handle;
    void (PLUGIN_CALL *unload)();
    int (PLUGIN_CALL *amxLoad)(AMX*);
    int (PLUGIN_CALL *amxUnload)(AMX*);
    void (PLUGIN_CALL *tick)();
};

CPlugins* pPlugins;

static const unsigned int MaxPluginVersion = 0x0220;

// 0x00469950 (Windows)
static int CallPublicFS(char* name)
{
    return pFilterScripts && name ? pFilterScripts->CallPublic(name) : 0;
}

// 0x00469970 (Windows)
static int CallPublicGM(char* name)
{
    return pGameMode && name ? pGameMode->CallPublic(name) : 0;
}

// 0x0046a3a0 (Windows)
CPlugins::CPlugins()
{
    memset(m_pluginData, 0, sizeof(m_pluginData));
    m_pluginData[PLUGIN_DATA_LOGPRINTF] = (void*)&logprintf;
    m_pluginData[PLUGIN_DATA_AMX_EXPORTS] = m_amxExports;
    m_pluginData[PLUGIN_DATA_CALLPUBLIC_FS] = (void*)&CallPublicFS;
    m_pluginData[PLUGIN_DATA_CALLPUBLIC_GM] = (void*)&CallPublicGM;

#define EXPORT_AMX(name) m_amxExports[PLUGIN_AMX_EXPORT_##name] = (void*)&amx_##name
    EXPORT_AMX(Align16);
    EXPORT_AMX(Align32);
    EXPORT_AMX(Align64);
    EXPORT_AMX(Allot);
    EXPORT_AMX(Callback);
    EXPORT_AMX(Cleanup);
    EXPORT_AMX(Clone);
    EXPORT_AMX(Exec);
    EXPORT_AMX(FindNative);
    EXPORT_AMX(FindPublic);
    EXPORT_AMX(FindPubVar);
    EXPORT_AMX(FindTagId);
    EXPORT_AMX(Flags);
    EXPORT_AMX(GetAddr);
    EXPORT_AMX(GetNative);
    EXPORT_AMX(GetPublic);
    EXPORT_AMX(GetPubVar);
    EXPORT_AMX(GetString);
    EXPORT_AMX(GetTag);
    EXPORT_AMX(GetUserData);
    EXPORT_AMX(Init);
    EXPORT_AMX(InitJIT);
    EXPORT_AMX(MemInfo);
    EXPORT_AMX(NameLength);
    EXPORT_AMX(NativeInfo);
    EXPORT_AMX(NumNatives);
    EXPORT_AMX(NumPublics);
    EXPORT_AMX(NumPubVars);
    EXPORT_AMX(NumTags);
    EXPORT_AMX(Push);
    EXPORT_AMX(PushArray);
    EXPORT_AMX(PushString);
    EXPORT_AMX(RaiseError);
    EXPORT_AMX(Register);
    EXPORT_AMX(Release);
    EXPORT_AMX(SetCallback);
    EXPORT_AMX(SetDebugHook);
    EXPORT_AMX(SetString);
    EXPORT_AMX(SetUserData);
    EXPORT_AMX(StrLen);
    EXPORT_AMX(UTF8Check);
    EXPORT_AMX(UTF8Get);
    EXPORT_AMX(UTF8Len);
    EXPORT_AMX(UTF8Put);
#undef EXPORT_AMX
}

CPlugins::~CPlugins()
{
    for (std::vector<Plugin*>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
    {
        Plugin* plugin = *it;
        if (plugin->unload)
            plugin->unload();
        ClosePlugin(plugin->handle);
        delete plugin;
    }
}

// 0x0046a5d0 (Windows)
bool CPlugins::LoadSinglePlugin(const char* path)
{
    typedef unsigned int (PLUGIN_CALL *SupportsFn)();
    typedef bool (PLUGIN_CALL *LoadFn)(void**);

    PluginHandle handle = OpenPlugin(path);
    if (!handle)
        return false;

    SupportsFn supports = (SupportsFn)PluginSymbol(handle, "Supports");
    LoadFn load = (LoadFn)PluginSymbol(handle, "Load");
    const unsigned int flags = supports ? supports() : 0;
    if (!supports || !load || (flags & SUPPORTS_VERSION_MASK) > MaxPluginVersion || !load(m_pluginData))
    {
        ClosePlugin(handle);
        return false;
    }

    Plugin* plugin = new Plugin();
    plugin->handle = handle;
    plugin->unload = (void (PLUGIN_CALL *)())PluginSymbol(handle, "Unload");
    if (flags & SUPPORTS_AMX_NATIVES)
    {
        plugin->amxLoad = (int (PLUGIN_CALL *)(AMX*))PluginSymbol(handle, "AmxLoad");
        plugin->amxUnload = (int (PLUGIN_CALL *)(AMX*))PluginSymbol(handle, "AmxUnload");
    }
    if (flags & SUPPORTS_PROCESS_TICK)
        plugin->tick = (void (PLUGIN_CALL *)())PluginSymbol(handle, "ProcessTick");
    m_plugins.push_back(plugin);
    return true;
}

// 0x0046a740 (Windows)
unsigned int CPlugins::LoadPlugins(const char* directory)
{
    const char* list = pConsole->GetStringVariable("plugins");
    std::istringstream plugins(list ? list : "");
    std::string name;
    while (plugins >> name)
    {
        const std::string path = std::string(directory) + "/" + name;
        logprintf(" Loading plugin: %s", name.c_str());
        logprintf(LoadSinglePlugin(path.c_str()) ? "  Loaded." : "  Failed.");
    }
    return GetPluginCount();
}

// 0x004699b0 (Windows)
unsigned int CPlugins::GetPluginCount() const
{
    return static_cast<unsigned int>(m_plugins.size());
}

// 0x00469bf0 (Windows)
void CPlugins::DoProcessTick()
{
    for (std::vector<Plugin*>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
        if ((*it)->tick)
            (*it)->tick();
}

// 0x00469c30 (Windows)
void CPlugins::DoAmxLoad(AMX* amx)
{
    for (std::vector<Plugin*>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
        if ((*it)->amxLoad)
            (*it)->amxLoad(amx);
}

// 0x00469c80 (Windows)
void CPlugins::DoAmxUnload(AMX* amx)
{
    for (std::vector<Plugin*>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
        if ((*it)->amxUnload)
            (*it)->amxUnload(amx);
}
