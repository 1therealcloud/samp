#ifndef SAMP_RECOVERED_CHAT_H
#define SAMP_RECOVERED_CHAT_H

// Native x86 layout. Names come from references; offsets are checked below.
#include <d3dx9.h>
#include <stddef.h>
#include <windows.h>

class CDXUTDialog;
class CDXUTEditBox;
class CDXUTScrollBar;
#include "fontrender.h"

#pragma pack(push, 1)
class CChat
{
public:
    // 0x00068e00
    LONG GetChatWindowBottom();
    // Stores the raw timestamp byte and always requests redraw.
    // 0x00067f20
    void SetTimestamps(BYTE enabled);
    // 0x00067f40
    BYTE GetTimestamps();
    enum EntryType
    {
        ENTRY_TYPE_NONE = 0,
        ENTRY_TYPE_CHAT = 2,
        ENTRY_TYPE_INFO = 4,
        ENTRY_TYPE_DEBUG = 8
    };
    enum DisplayMode
    {
        DISPLAY_MODE_OFF = 0,
        DISPLAY_MODE_NOSHADOW = 1,
        DISPLAY_MODE_NORMAL = 2
    };
    enum
    {
        MAX_MESSAGES = 100,
        MAX_LINE_LENGTH = 72,
        MAX_MESSAGE_LENGTH = 144
    };
    struct ChatEntry
    {
        long timestamp;
        char prefix[28];
        char text[144];
        char unused[64];
        int type;
        D3DCOLOR textColor, prefixColor;
    };
    unsigned int m_nPageSize;
    char* m_szLastMessage;
    int m_nMode;
    bool m_bTimestamps;
    BOOL m_bDoesLogExist;
    char m_szLogPath[261];
    CDXUTDialog* m_pGameUi;
    CDXUTEditBox* m_pEditbox;
    CDXUTScrollBar* m_pScrollbar;
    D3DCOLOR m_textColor, m_infoColor, m_debugColor;
    long m_nWindowBottom;
    ChatEntry m_entry[MAX_MESSAGES];
    CFonts* m_pFontRenderer;
    ID3DXSprite *m_pTextSprite, *m_pSprite;
    IDirect3DDevice9* m_pDevice;
    BOOL m_bRenderToSurface;
    ID3DXRenderToSurface* m_pRenderToSurface;
    IDirect3DTexture9* m_pTexture;
    IDirect3DSurface9* m_pSurface;
    D3DDISPLAYMODE m_displayMode;
    DWORD m_lastSurfaceUpdate;
    BOOL m_bSurfaceValid;
    BOOL m_bRedraw;
    long m_nScrollbarPos, m_nCharHeight, m_nTimestampWidth;

    // 0x00067df0
    CChat(IDirect3DDevice9*, CFonts*, const char*);
    // 0x00060d30
    int GetMode();
    // 0x00060d40
    void SwitchMode();
    // 0x00066b90
    void RecalcFontSize();
    // 0x00066c10
    void OnLostDevice();
    // 0x00066c70
    void UpdateScrollbar();
    // 0x00066d10
    void SetPageSize(int);
    // 0x00066d40
    void PageUp();
    // 0x00066da0
    void PageDown();
    // 0x00066e00
    void ScrollToBottom();
    // 0x00066e30
    void Scroll(int);
    // 0x00066e90
    void FilterOutInvalidChars(char*);
    // 0x00066ec0
    void PushBack();
    // 0x00067240
    void Log(int, const char*, const char*);
    // 0x00067310
    void ResetDialogControls(CDXUTDialog*);
    // 0x00067650
    void AddEntry(int, const char*, const char*, D3DCOLOR, D3DCOLOR);
    // 0x00067a90
    void AddChatMessage(const char*, D3DCOLOR, char*);
    // 0x00067be0
    void AddMessage(D3DCOLOR, char*);
    // 0x00067ae0
    void AddInfoMessage(const char*, ...);
    // 0x00067b60
    void AddDebugMessage(const char*, ...);

    // Rendering is reconstructed too; exact-match status is per method.
    // 0x00066ee0
    void RenderEntry(const char*, RECT, D3DCOLOR);
    // 0x000673b0
    void Render();
    // 0x00067870
    void Draw();
    // 0x00067940
    void RenderToSurface();
    // 0x00067c40
    void OnResetDevice();
};
#pragma pack(pop)

#define CHAT_LAYOUT_ASSERT(name, condition) typedef char name[(condition) ? 1 : -1]
CHAT_LAYOUT_ASSERT(chat_x86, sizeof(void*) == 4);
CHAT_LAYOUT_ASSERT(chat_entry_size, sizeof(CChat::ChatEntry) == 0xfc);
CHAT_LAYOUT_ASSERT(chat_size, sizeof(CChat) == 0x63ea);
CHAT_LAYOUT_ASSERT(chat_mode_offset, offsetof(CChat, m_nMode) == 8);
CHAT_LAYOUT_ASSERT(chat_timestamp_flag_offset, offsetof(CChat, m_bTimestamps) == 0xc);
CHAT_LAYOUT_ASSERT(chat_log_flag_offset, offsetof(CChat, m_bDoesLogExist) == 0xd);
CHAT_LAYOUT_ASSERT(chat_log_offset, offsetof(CChat, m_szLogPath) == 0x11);
CHAT_LAYOUT_ASSERT(chat_dialog_offset, offsetof(CChat, m_pGameUi) == 0x116);
CHAT_LAYOUT_ASSERT(chat_scroll_offset, offsetof(CChat, m_pScrollbar) == 0x11e);
CHAT_LAYOUT_ASSERT(chat_text_color_offset, offsetof(CChat, m_textColor) == 0x122);
CHAT_LAYOUT_ASSERT(chat_info_color_offset, offsetof(CChat, m_infoColor) == 0x126);
CHAT_LAYOUT_ASSERT(chat_debug_color_offset, offsetof(CChat, m_debugColor) == 0x12a);
CHAT_LAYOUT_ASSERT(chat_bottom_offset, offsetof(CChat, m_nWindowBottom) == 0x12e);
CHAT_LAYOUT_ASSERT(chat_entry_offset, offsetof(CChat, m_entry) == 0x132);
CHAT_LAYOUT_ASSERT(chat_entry_text_offset, offsetof(CChat::ChatEntry, text) == 0x20);
CHAT_LAYOUT_ASSERT(chat_entry_type_offset, offsetof(CChat::ChatEntry, type) == 0xf0);
CHAT_LAYOUT_ASSERT(chat_fonts_offset, offsetof(CChat, m_pFontRenderer) == 0x63a2);
CHAT_LAYOUT_ASSERT(chat_text_sprite_offset, offsetof(CChat, m_pTextSprite) == 0x63a6);
CHAT_LAYOUT_ASSERT(chat_sprite_offset, offsetof(CChat, m_pSprite) == 0x63aa);
CHAT_LAYOUT_ASSERT(chat_device_offset, offsetof(CChat, m_pDevice) == 0x63ae);
CHAT_LAYOUT_ASSERT(chat_surface_mode_offset, offsetof(CChat, m_bRenderToSurface) == 0x63b2);
CHAT_LAYOUT_ASSERT(chat_render_to_surface_offset, offsetof(CChat, m_pRenderToSurface) == 0x63b6);
CHAT_LAYOUT_ASSERT(chat_texture_offset, offsetof(CChat, m_pTexture) == 0x63ba);
CHAT_LAYOUT_ASSERT(chat_surface_offset, offsetof(CChat, m_pSurface) == 0x63be);
CHAT_LAYOUT_ASSERT(chat_display_mode_offset, offsetof(CChat, m_displayMode) == 0x63c2);
CHAT_LAYOUT_ASSERT(chat_tick_offset, offsetof(CChat, m_lastSurfaceUpdate) == 0x63d2);
CHAT_LAYOUT_ASSERT(chat_surface_valid_offset, offsetof(CChat, m_bSurfaceValid) == 0x63d6);
CHAT_LAYOUT_ASSERT(chat_redraw_offset, offsetof(CChat, m_bRedraw) == 0x63da);
CHAT_LAYOUT_ASSERT(chat_position_offset, offsetof(CChat, m_nScrollbarPos) == 0x63de);
CHAT_LAYOUT_ASSERT(chat_height_offset, offsetof(CChat, m_nCharHeight) == 0x63e2);
CHAT_LAYOUT_ASSERT(chat_timestamp_width_offset, offsetof(CChat, m_nTimestampWidth) == 0x63e6);
#undef CHAT_LAYOUT_ASSERT
#endif
