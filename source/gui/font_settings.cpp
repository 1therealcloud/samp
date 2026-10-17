#include "gui/font_settings.h"
#include "config.h"
// GTA SA VA: 0x00c17044 (outside samp.dll)
extern int g_chatScreenWidth;

// samp.dll RVA: 0x000c6950
int FontGetHeight() {
    int height;
    if (g_chatScreenWidth < 1024) height = 14;
    else if (g_chatScreenWidth < 1400) height = 16;
    else if (g_chatScreenWidth < 1600) height = 18;
    else height = 20;
    return height + g_pConfig->GetIntValue("fontsize") * 2;
}

// samp.dll RVA: 0x000c6a00
int FontGetWeight() {
    int weight = g_pConfig->GetIntValue("fontweight");
    if (weight == 0) return 700;
    return weight == 1 ? 400 : 700;
}

// samp.dll RVA: 0x000c6a30
const char *FontGetFace() {
    if (g_pConfig && g_pConfig->GetStringValue("fontface"))
        return g_pConfig->GetStringValue("fontface");
    return "Arial";
}
