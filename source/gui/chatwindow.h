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

#pragma pack(push, 1)
class CChat
{
public:
    LONG GetChatWindowBottom();
    // Stores the raw timestamp byte and always requests redraw.

    void SetTimestamps(BYTE enabled);

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

    CChat(IDirect3DDevice9*, CFonts*, const char*);

    int GetMode();

    void SwitchMode();

    void RecalcFontSize();

    void OnLostDevice();

    void UpdateScrollbar();

    void SetPageSize(int);

    void PageUp();

    void PageDown();

    void ScrollToBottom();

    void Scroll(int);

    void FilterOutInvalidChars(char*);

    void PushBack();

    void Log(int, const char*, const char*);

    void ResetDialogControls(CDXUTDialog*);

    void AddEntry(int, const char*, const char*, D3DCOLOR, D3DCOLOR);

    void AddChatMessage(const char*, D3DCOLOR, char*);

    void AddMessage(D3DCOLOR, char*);

    void AddInfoMessage(const char*, ...);

    void AddDebugMessage(const char*, ...);

    // Rendering is reconstructed too; exact-match status is per method.

    void RenderEntry(const char*, RECT, D3DCOLOR);

    void Render();

    void Draw();

    void RenderToSurface();

    void OnResetDevice();
};
#pragma pack(pop)

static_assert(sizeof(void*) == 4);
static_assert(sizeof(CChat::ChatEntry) == 0xfc);
static_assert(sizeof(CChat) == 0x63ea);
static_assert(offsetof(CChat, m_nMode) == 8);
static_assert(offsetof(CChat, m_bTimestamps) == 0xc);
static_assert(offsetof(CChat, m_bDoesLogExist) == 0xd);
static_assert(offsetof(CChat, m_szLogPath) == 0x11);
static_assert(offsetof(CChat, m_pGameUi) == 0x116);
static_assert(offsetof(CChat, m_pScrollbar) == 0x11e);
static_assert(offsetof(CChat, m_textColor) == 0x122);
static_assert(offsetof(CChat, m_infoColor) == 0x126);
static_assert(offsetof(CChat, m_debugColor) == 0x12a);
static_assert(offsetof(CChat, m_nWindowBottom) == 0x12e);
static_assert(offsetof(CChat, m_entry) == 0x132);
static_assert(offsetof(CChat::ChatEntry, text) == 0x20);
static_assert(offsetof(CChat::ChatEntry, type) == 0xf0);
static_assert(offsetof(CChat, m_pFontRenderer) == 0x63a2);
static_assert(offsetof(CChat, m_pTextSprite) == 0x63a6);
static_assert(offsetof(CChat, m_pSprite) == 0x63aa);
static_assert(offsetof(CChat, m_pDevice) == 0x63ae);
static_assert(offsetof(CChat, m_bRenderToSurface) == 0x63b2);
static_assert(offsetof(CChat, m_pRenderToSurface) == 0x63b6);
static_assert(offsetof(CChat, m_pTexture) == 0x63ba);
static_assert(offsetof(CChat, m_pSurface) == 0x63be);
static_assert(offsetof(CChat, m_displayMode) == 0x63c2);
static_assert(offsetof(CChat, m_lastSurfaceUpdate) == 0x63d2);
static_assert(offsetof(CChat, m_bSurfaceValid) == 0x63d6);
static_assert(offsetof(CChat, m_bRedraw) == 0x63da);
static_assert(offsetof(CChat, m_nScrollbarPos) == 0x63de);
static_assert(offsetof(CChat, m_nCharHeight) == 0x63e2);
static_assert(offsetof(CChat, m_nTimestampWidth) == 0x63e6);

int ChatIsHexDigit(char value);

unsigned long ChatParseColorTag(char* text);

void ChatStripColorTags(char* text);

int ChatPrepareMessage(char* text, int maximumLength);

#endif // CHATWINDOW_H
