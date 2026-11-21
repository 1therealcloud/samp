#ifndef CHATWINDOW_H
#define CHATWINDOW_H

// Native x86 layout. Names come from references; offsets are checked below.
#include <d3dx9.h>

#include <stddef.h>
#include <windows.h>

class CDXUTDialog;
class CDXUTEditBox;
class CDXUTScrollBar;
#include "render/fontrender.h"

#include <static_assert.h>

#pragma pack(push, 1)
class CChat
{
public:
    // R5 address to verify
    LONG GetChatWindowBottom();
    // Stores the raw timestamp byte and always requests redraw.
    // R5 address to verify
    void SetTimestamps(BYTE enabled);
    // R5 address to verify
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

    // 0x00068380
    CChat(IDirect3DDevice9*, CFonts*, const char*);
    // 0x000612b0
    int GetMode();
    // 0x000612c0
    void SwitchMode();
    // 0x00067120
    void RecalcFontSize();
    // 0x000671a0
    void OnLostDevice();
    // 0x00067200
    void UpdateScrollbar();
    // 0x000672a0
    void SetPageSize(int);
    // 0x000672d0
    void PageUp();
    // 0x00067330
    void PageDown();
    // 0x00067390
    void ScrollToBottom();
    // 0x000673c0
    void Scroll(int);
    // 0x00067420
    void FilterOutInvalidChars(char*);
    // 0x00067450
    void PushBack();
    // 0x000677d0
    void Log(int, const char*, const char*);
    // 0x000678a0
    void ResetDialogControls(CDXUTDialog*);
    // 0x00067be0
    void AddEntry(int, const char*, const char*, D3DCOLOR, D3DCOLOR);
    // 0x00068020
    void AddChatMessage(const char*, D3DCOLOR, char*);
    // 0x00068170
    void AddMessage(D3DCOLOR, char*);
    // 0x00068070
    void AddInfoMessage(const char*, ...);
    // 0x000680f0
    void AddDebugMessage(const char*, ...);

    // Rendering is reconstructed too; exact-match status is per method.
    // 0x00067470
    void RenderEntry(const char*, RECT, D3DCOLOR);
    // 0x00067940
    void Render();
    // 0x00067e00
    void Draw();
    // 0x00067ed0
    void RenderToSurface();
    // 0x000681d0
    void OnResetDevice();
};
#pragma pack(pop)

STATIC_ASSERT(sizeof(void*) == 4);
STATIC_ASSERT(sizeof(CChat::ChatEntry) == 0xfc);
STATIC_ASSERT(sizeof(CChat) == 0x63ea);
STATIC_ASSERT(offsetof(CChat, m_nMode) == 8);
STATIC_ASSERT(offsetof(CChat, m_bTimestamps) == 0xc);
STATIC_ASSERT(offsetof(CChat, m_bDoesLogExist) == 0xd);
STATIC_ASSERT(offsetof(CChat, m_szLogPath) == 0x11);
STATIC_ASSERT(offsetof(CChat, m_pGameUi) == 0x116);
STATIC_ASSERT(offsetof(CChat, m_pScrollbar) == 0x11e);
STATIC_ASSERT(offsetof(CChat, m_textColor) == 0x122);
STATIC_ASSERT(offsetof(CChat, m_infoColor) == 0x126);
STATIC_ASSERT(offsetof(CChat, m_debugColor) == 0x12a);
STATIC_ASSERT(offsetof(CChat, m_nWindowBottom) == 0x12e);
STATIC_ASSERT(offsetof(CChat, m_entry) == 0x132);
STATIC_ASSERT(offsetof(CChat::ChatEntry, text) == 0x20);
STATIC_ASSERT(offsetof(CChat::ChatEntry, type) == 0xf0);
STATIC_ASSERT(offsetof(CChat, m_pFontRenderer) == 0x63a2);
STATIC_ASSERT(offsetof(CChat, m_pTextSprite) == 0x63a6);
STATIC_ASSERT(offsetof(CChat, m_pSprite) == 0x63aa);
STATIC_ASSERT(offsetof(CChat, m_pDevice) == 0x63ae);
STATIC_ASSERT(offsetof(CChat, m_bRenderToSurface) == 0x63b2);
STATIC_ASSERT(offsetof(CChat, m_pRenderToSurface) == 0x63b6);
STATIC_ASSERT(offsetof(CChat, m_pTexture) == 0x63ba);
STATIC_ASSERT(offsetof(CChat, m_pSurface) == 0x63be);
STATIC_ASSERT(offsetof(CChat, m_displayMode) == 0x63c2);
STATIC_ASSERT(offsetof(CChat, m_lastSurfaceUpdate) == 0x63d2);
STATIC_ASSERT(offsetof(CChat, m_bSurfaceValid) == 0x63d6);
STATIC_ASSERT(offsetof(CChat, m_bRedraw) == 0x63da);
STATIC_ASSERT(offsetof(CChat, m_nScrollbarPos) == 0x63de);
STATIC_ASSERT(offsetof(CChat, m_nCharHeight) == 0x63e2);
STATIC_ASSERT(offsetof(CChat, m_nTimestampWidth) == 0x63e6);

// 0x000b5e10
int ChatIsHexDigit(char value);
// 0x000b5e80
unsigned long ChatParseColorTag(char* text);
// 0x000b60c0
void ChatStripColorTags(char* text);
// 0x000b6100
int ChatPrepareMessage(char* text, int maximumLength);

#endif // CHATWINDOW_H
