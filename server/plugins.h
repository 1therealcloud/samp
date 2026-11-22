#ifndef SAMPSRV_PLUGINS_H
#define SAMPSRV_PLUGINS_H

#include "amx.h"

#include <vector>

class CPlugins
{
public:
    CPlugins();
    ~CPlugins();
    unsigned int LoadPlugins(const char* directory);
    bool LoadSinglePlugin(const char* path);
    unsigned int GetPluginCount() const;
    void DoProcessTick();
    void DoAmxLoad(AMX* amx);
    void DoAmxUnload(AMX* amx);

private:
    CPlugins(const CPlugins&);
    CPlugins& operator=(const CPlugins&);

    struct Plugin;
    std::vector<Plugin*> m_plugins;
    void* m_pluginData[0x100];
    void* m_amxExports[44];
};

extern CPlugins* pPlugins;

#endif
