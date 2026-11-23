#include "config.h"
#include "gui/cmdwindow.h"
#include "main.h"
#include "runutil.h"

#include <DXUT.h>
#include <DXUTgui.h>
#include <DXUTmisc.h>

#include <assert.h>
#include <new>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Reconstructed from 0.3.DL-R1 machine code; 0.2x is structural context only.
// Build with the pinned VC++ 2003 flags. See analysis/chat-build-v51-O2-Ob2.json.
#include "gui/chatwindow.h"
// 0x002aca3c
extern CGame* g_pGame;
// 0x002aca14
extern CInput* g_pInput;

// 0x00068e00
LONG CChat::GetChatWindowBottom()
{
    return m_nWindowBottom;
}

// 0x00067f20
void CChat::SetTimestamps(BYTE enabled)
{
    *((BYTE*)this + offsetof(CChat, m_bTimestamps)) = enabled;
    m_bRedraw = TRUE;
}

// 0x00067f40
BYTE CChat::GetTimestamps()
{
    return *((BYTE*)this + offsetof(CChat, m_bTimestamps));
}

// These declarations do not call back into the original DLL.

// 0x00060d30
int CChat::GetMode()
{
    return m_nMode;
}

// 0x00060d40
void CChat::SwitchMode()
{
    int mode = m_nMode;
    m_bRedraw = TRUE;
    if (mode == DISPLAY_MODE_OFF)
    {
        m_nMode = DISPLAY_MODE_NORMAL;
        return;
    }
    if (mode == DISPLAY_MODE_NOSHADOW)
    {
        m_nMode = DISPLAY_MODE_OFF;
        return;
    }
    if (mode == DISPLAY_MODE_NORMAL)
        m_nMode = DISPLAY_MODE_NOSHADOW;
}

// 0x00066b90
void CChat::RecalcFontSize()
{
    RECT rect;
    m_pFontRenderer->m_pFont->DrawTextA(0, "Y", -1, &rect, DT_CALCRECT | DT_SINGLELINE, 0xff000000);
    m_nCharHeight = rect.bottom - rect.top;
    m_pFontRenderer->m_pFont->DrawTextA(0, "[19:58:34]", -1, &rect, DT_CALCRECT | DT_SINGLELINE, 0xff000000);
    m_nTimestampWidth = rect.right - rect.left;
}

// 0x00066c10
void CChat::OnLostDevice()
{
    if (m_pSurface)
    {
        m_pSurface->Release();
        m_pSurface = 0;
    }
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = 0;
    }
    if (m_pRenderToSurface)
    {
        m_pRenderToSurface->Release();
        m_pRenderToSurface = 0;
    }
}

// 0x00066c70
void CChat::UpdateScrollbar()
{
    if (m_pScrollbar)
    {
        m_pScrollbar->SetLocation(10, 40);
        m_pScrollbar->SetSize(20, (m_nCharHeight + 1) * m_nPageSize - 60);
        m_pScrollbar->SetTrackRange(1, 100);
        m_pScrollbar->SetPageSize(m_nPageSize);
        m_pScrollbar->SetTrackPos(100 - m_nPageSize);
    }
}

// 0x00066d10
void CChat::SetPageSize(int size)
{
    if (size >= 10 && size <= 100)
    {
        m_nPageSize = size;
        UpdateScrollbar();
        m_bRedraw = TRUE;
    }
}

// 0x00066d40
void CChat::PageUp()
{
    if (m_nMode && m_pScrollbar && !g_pGame->IsMenuVisible() && !g_pInput->IsActive())
    {
        int pos = m_pScrollbar->GetTrackPos() - m_nPageSize;
        if (pos < 1)
            pos = 1;
        m_pScrollbar->SetTrackPos(pos);
    }
}

// 0x00066da0
void CChat::PageDown()
{
    if (m_nMode && m_pScrollbar && !g_pGame->IsMenuVisible() && !g_pInput->IsActive())
    {
        int pos = m_pScrollbar->GetTrackPos();
        if (pos == 1)
            pos = m_nPageSize;
        else
            pos += m_nPageSize;
        if (pos > 100)
            pos = 100;
        m_pScrollbar->SetTrackPos(pos);
    }
}

// 0x00066e00
void CChat::ScrollToBottom()
{
    if (m_pScrollbar)
        m_pScrollbar->SetTrackPos(90);
}

// 0x00066e30
void CChat::Scroll(int delta)
{
    if (m_nMode && m_pScrollbar)
    {
        unsigned int lines;
        SystemParametersInfoA(0x68, 0, &lines, 0);
        m_pScrollbar->Scroll(-(lines * delta));
    }
}

// 0x00066e90
void CChat::FilterOutInvalidChars(char* text)
{
    while (*text)
    {
        if (*text > 0 && *text < ' ')
            *text = ' ';
        ++text;
    }
}

// 0x00066ec0
void CChat::PushBack()
{
    // Native quirk: forward REP MOVSD copies 100 entries, reading one entry
    // past the history, including 180 bytes beyond this object. Do not shorten
    // to 99. This preserves target behavior, not portable memcpy semantics.
    // Group the address expressions to preserve VC++ 2003 instruction ordering.
    struct CopyAddresses
    {
        void* destination;
        void* source;
    } copy = {m_entry, m_entry + 1};
    memcpy(copy.destination, copy.source, sizeof(m_entry));
}

// 0x00067240
void CChat::Log(int type, const char* text, const char* prefix)
{
    time_t now = time(0);
    char stamp[65];
    if (m_bDoesLogExist)
    {
        FILE* file = nullptr;
        fopen_s(&file, m_szLogPath, "a");
        if (file)
        {
            tm timeInfo;
            if (localtime_s(&timeInfo, &now) == 0)
                strftime(stamp, sizeof(stamp), "[%H:%M:%S]", &timeInfo);
            else
                stamp[0] = 0;
            switch (type)
            {
                case ENTRY_TYPE_CHAT:
                    fprintf(file, "%s <%s> %s\r\n", stamp, prefix, text);
                    break;
                case ENTRY_TYPE_INFO:
                case ENTRY_TYPE_DEBUG:
                    fprintf(file, "%s %s\r\n", stamp, text);
                    break;
            }
            fclose(file);
        }
    }
}

// 0x00067310
void CChat::ResetDialogControls(CDXUTDialog* dialog)
{
    m_pGameUi = dialog;
    if (dialog)
    {
        m_pScrollbar = new CDXUTScrollBar(dialog);
        dialog->AddControl(m_pScrollbar);
        m_pScrollbar->SetVisible(true);
        m_pScrollbar->SetEnabled(true);
        UpdateScrollbar();
    }
}

// 0x00067650
void CChat::AddEntry(int type, const char* text, const char* prefix, D3DCOLOR color, D3DCOLOR prefixColor)
{
    int split = 0;
    PushBack();
    if (prefix && strlen(prefix) > 24)
        return;
    ChatEntry& entry = m_entry[99];
    entry.type = type;
    entry.textColor = color;
    entry.prefixColor = prefixColor;
    entry.timestamp = time(0);
    if (prefix)
    {
        strncpy_s(entry.prefix, prefix, sizeof(entry.prefix) - 2);
        strncat_s(entry.prefix, ":", _TRUNCATE);
    }
    else
        entry.prefix[0] = 0;
    Log(type, text, prefix);
    if (entry.type == ENTRY_TYPE_CHAT && strlen(text) > MAX_LINE_LENGTH)
    {
        split = MAX_LINE_LENGTH;
        while (text[split] != ' ' && split)
            --split;
        if (MAX_LINE_LENGTH - split > 12)
        {
            strncpy_s(entry.text, text, MAX_LINE_LENGTH);
            entry.text[MAX_LINE_LENGTH] = 0;
            PushBack();
            entry.type = type;
            entry.textColor = color;
            entry.prefixColor = prefixColor;
            entry.prefix[0] = 0;
            strncpy_s(entry.text, text + MAX_LINE_LENGTH, _TRUNCATE);
        }
        else
        {
            strncpy_s(entry.text, text, split);
            entry.text[split] = 0;
            PushBack();
            entry.type = type;
            entry.textColor = color;
            entry.prefixColor = prefixColor;
            entry.prefix[0] = 0;
            strncpy_s(entry.text, text + split + 1, _TRUNCATE);
        }
    }
    else
    {
        strncpy_s(entry.text, text, _TRUNCATE);
        entry.text[MAX_MESSAGE_LENGTH] = 0;
    }
    m_bRedraw = TRUE;
}

// 0x00067a90
void CChat::AddChatMessage(const char* prefix, D3DCOLOR prefixColor, char* text)
{
    FilterOutInvalidChars(text);
    AddEntry(ENTRY_TYPE_CHAT, text, prefix, m_textColor, prefixColor);
}

// 0x00067be0
void CChat::AddMessage(D3DCOLOR color, char* text)
{
    if (strlen(text) <= MAX_MESSAGE_LENGTH)
    {
        ChatPrepareMessage(text, 255);
        FilterOutInvalidChars(text);
        AddEntry(ENTRY_TYPE_INFO, text, 0, (color >> 8) | 0xff000000, 0);
    }
}

// 0x00067ae0
void CChat::AddInfoMessage(const char* format, ...)
{
    char text[512];
    memset(text, 0, sizeof(text));
    va_list args;
    va_start(args, format);
    vsnprintf(text, sizeof(text), format, args);
    va_end(args);
    FilterOutInvalidChars(text);
    AddEntry(ENTRY_TYPE_INFO, text, 0, m_infoColor, 0);
}

// 0x00067b60
void CChat::AddDebugMessage(const char* format, ...)
{
    char text[512];
    memset(text, 0, sizeof(text));
    va_list args;
    va_start(args, format);
    vsnprintf(text, sizeof(text), format, args);
    va_end(args);
    FilterOutInvalidChars(text);
    AddEntry(ENTRY_TYPE_DEBUG, text, 0, m_debugColor, 0);
}

// 0x00067df0
CChat::CChat(IDirect3DDevice9* device, CFonts* fonts, const char* path)
{
    m_pDevice = device;
    m_pFontRenderer = fonts;
    m_nMode = DISPLAY_MODE_NORMAL;
    D3DXCreateSprite(device, &m_pTextSprite);
    D3DXCreateSprite(device, &m_pSprite);
    struct ClearRange
    {
        void* destination;
        size_t length;
    } clear = {m_entry, sizeof(m_entry)};
    memset(clear.destination, 0, clear.length);
    m_textColor = 0xffffffff;
    m_infoColor = 0xff88aa62;
    m_debugColor = 0xffa9c4e4;
    m_nPageSize = 10;
    m_bTimestamps = false;
    if (path && strlen(path))
    {
        memset(m_szLogPath, 0, sizeof(m_szLogPath));
        strncpy_s(m_szLogPath, path, 260);
        FILE* file = nullptr;
        fopen_s(&file, m_szLogPath, "w");
        if (file)
        {
            m_bDoesLogExist = TRUE;
            fclose(file);
        }
    }
    m_pSurface = 0;
    m_pTexture = 0;
    m_pRenderToSurface = 0;
    m_lastSurfaceUpdate = GetTickCount();
    m_nScrollbarPos = 1;
    m_pScrollbar = 0;
    m_pGameUi = 0;
    m_pEditbox = 0;
    OnResetDevice();
}

// 0x00067870
void CChat::Draw()
{
    if (m_pScrollbar)
    {
        if (m_nMode == 0 || (m_pScrollbar->GetTrackPos() == 100 - m_nPageSize && !g_pInput->m_bEnabled))
            m_pScrollbar->SetVisible(false);
        else
            m_pScrollbar->SetVisible(true);
        if (m_nMode)
        {
            if (m_bRenderToSurface && m_bSurfaceValid)
            {
                m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
                m_pSprite->Draw(m_pTexture, 0, 0, 0, 0xffffffff);
                m_pSprite->End();
                return;
            }
            m_pTextSprite->Begin(D3DXSPRITE_ALPHABLEND);
            Render();
            m_pTextSprite->End();
        }
    }
}

// 0x00067940
void CChat::RenderToSurface()
{
    if (m_bRenderToSurface && m_pDevice && m_pRenderToSurface && m_pTexture && m_pSurface && m_pFontRenderer->m_pFont)
    {
        if (g_pGame->IsMenuVisible())
        {
            m_bSurfaceValid = FALSE;
            m_bRedraw = TRUE;
            return;
        }
        if (m_nMode && (m_bRedraw || m_nScrollbarPos != m_pScrollbar->GetTrackPos()))
        {
            m_pRenderToSurface->BeginScene(m_pSurface, 0);
            m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 0.0f, 0);
            m_pTextSprite->Begin(D3DXSPRITE_ALPHABLEND);
            m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BLENDFACTOR);
            Render();
            m_pTextSprite->End();
            m_pRenderToSurface->EndScene(0);
            m_bRedraw = FALSE;
            m_bSurfaceValid = TRUE;
            m_lastSurfaceUpdate = GetTickCount();
            m_nScrollbarPos = m_pScrollbar->GetTrackPos();
        }
    }
}

// 0x00067c40
void CChat::OnResetDevice()
{
    m_bRenderToSurface = TRUE;
    if (m_pSurface)
    {
        m_pSurface->Release();
        m_pSurface = 0;
    }
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = 0;
    }
    if (m_pRenderToSurface)
    {
        m_pRenderToSurface->Release();
        m_pRenderToSurface = 0;
    }
    m_pDevice->GetDisplayMode(0, &m_displayMode);
    if (g_pConfig->GetIntValue("directmode"))
    {
        AddDebugMessage("ChatWindow: Using direct drawing mode.");
        RecalcFontSize();
        m_bRenderToSurface = FALSE;
        return;
    }
    HRESULT result;
    if (m_displayMode.Width <= 1024)
        result = D3DXCreateTexture(m_pDevice, 1024, 512, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture);
    else
        result = D3DXCreateTexture(m_pDevice, 2048, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture);
    if (FAILED(result))
    {
        AddDebugMessage("ChatWindow: Can't create a render surface texture. Will use direct mode.");
        m_bRenderToSurface = FALSE;
        return;
    }
    m_pTexture->GetSurfaceLevel(0, &m_pSurface);
    D3DSURFACE_DESC desc;
    m_pSurface->GetDesc(&desc);
    result = D3DXCreateRenderToSurface(m_pDevice, desc.Width, desc.Height, desc.Format, TRUE, D3DFMT_D16, &m_pRenderToSurface);
    if (FAILED(result))
    {
        AddDebugMessage("ChatWindow: Can't create a render to surface. Will use direct mode.");
        RecalcFontSize();
        m_bRenderToSurface = FALSE;
        return;
    }
    RecalcFontSize();
    m_bSurfaceValid = FALSE;
    m_bRedraw = TRUE;
}

// 0x000673b0
void CChat::Render()
{
    RECT rect, measure;
    // Keep the two native cursor slots in order for the VC++ 2003 stack layout.
    struct Cursor
    {
        int line;
        int length;
    } cursor;
    cursor.line = 0;
    char stamp[65];
    memset(stamp, 0, 64);
    rect.top = 10;
    rect.left = 45;
    rect.bottom = 110;
    rect.right = 550;
    if (m_pScrollbar->GetTrackPos() < 1)
        m_pScrollbar->SetTrackPos(1);
    int pos = m_pScrollbar->GetTrackPos();
    if (pos < 1)
        pos = 1;
    while (cursor.line != m_nPageSize)
    {
        ChatEntry& entry = m_entry[pos];
        if (m_bTimestamps)
        {
            time_t entryTime = (time_t)entry.timestamp;
            tm timeInfo;
            memset(stamp, 0, 64);
            if (localtime_s(&timeInfo, &entryTime) == 0)
                strftime(stamp, sizeof(stamp), "[%H:%M:%S]", &timeInfo);
        }
        switch (entry.type)
        {
            case ENTRY_TYPE_CHAT:
                if (m_bTimestamps && (cursor.length = strlen(stamp)) && entry.prefix[0])
                {
                    RenderEntry(stamp, rect, entry.textColor);
                    rect.left = m_nTimestampWidth + 50;
                }
                cursor.length = strlen(entry.prefix);
                if (cursor.length)
                {
                    m_pFontRenderer->m_pFont->DrawTextA(0, entry.prefix, -1, &measure, DT_CALCRECT, 0xff000000);
                    RenderEntry(entry.prefix, rect, entry.prefixColor);
                    rect.left += measure.right - measure.left + 5;
                }
                RenderEntry(entry.text, rect, entry.textColor);
                break;
            case ENTRY_TYPE_INFO:
            case ENTRY_TYPE_DEBUG:
                if (m_bTimestamps && (cursor.length = strlen(stamp)))
                {
                    RenderEntry(stamp, rect, entry.textColor);
                    rect.left = m_nTimestampWidth + 50;
                }
                RenderEntry(entry.text, rect, entry.textColor);
                break;
        }
        rect.top += m_nCharHeight + 1;
        rect.bottom = rect.top + m_nCharHeight + 1;
        rect.left = 45;
        ++pos;
        ++cursor.line;
    }
    m_nWindowBottom = rect.bottom;
}

// 0x00066ee0
void CChat::RenderEntry(const char* text, RECT rect, D3DCOLOR color)
{
    ID3DXFont* shadow = m_pFontRenderer->m_pShadow;
    ID3DXFont* font = m_pFontRenderer->m_pFont;
    int length = strlen(text);
    if (m_nMode == DISPLAY_MODE_NORMAL)
    {
        char plain[512] = {0};
        strncpy_s(plain, text, 511);
        ChatStripColorTags(plain);
        int plainLength = strlen(plain);
        if ((*(int*)0x00c17044) > 1280)
        {
            rect.top -= 2;
            rect.bottom -= 2;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            ++rect.top;
            ++rect.bottom;
            --rect.left;
            --rect.right;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            ++rect.left;
            ++rect.right;
            ++rect.left;
            ++rect.right;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            --rect.left;
            --rect.right;
            ++rect.top;
            ++rect.bottom;
            rect.top += 2;
            rect.bottom += 2;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            --rect.top;
            --rect.bottom;
            --rect.left;
            --rect.right;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            ++rect.left;
            ++rect.right;
            ++rect.left;
            ++rect.right;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            --rect.left;
            --rect.right;
            --rect.top;
            --rect.bottom;
            rect.left -= 2;
            rect.right -= 2;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            rect.left += 2;
            rect.right += 2;
            rect.left += 2;
            rect.right += 2;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            rect.left -= 2;
            rect.right -= 2;
        }
        else
        {
            --rect.top;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            ++rect.top;
            ++rect.top;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            --rect.top;
            --rect.left;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            rect.left += 2;
            shadow->DrawTextA(m_pTextSprite, plain, plainLength, (RECT*)&rect, 0x120, 0xff000000);
            --rect.left;
        }
    }
    font->DrawTextA(m_pTextSprite, text, length, (RECT*)&rect, 0x120, color | 0xff000000);
}

// 0x000b5950
int ChatIsHexDigit(char value)
{
    if ((value >= '0' && value <= '9') || (value >= 'A' && value <= 'F') || (value >= 'a' && value <= 'f'))
        return 1;
    return 0;
}

// 0x000b59c0
unsigned long ChatParseColorTag(char* text)
{
    if (text[0] && text[0] == '{' && text[1] && ChatIsHexDigit(text[1]) && text[2] && ChatIsHexDigit(text[2]) && text[3] && ChatIsHexDigit(text[3]) && text[4] &&
        ChatIsHexDigit(text[4]) && text[5] && ChatIsHexDigit(text[5]) && text[6] && ChatIsHexDigit(text[6]) && text[7] && text[7] == '}')
    {
        char digits[17];
        memset(digits, 0, sizeof(digits));
        strncpy_s(digits, text + 1, 6);
        return strtoul(digits, 0, 16);
    }
    return 0xffffffff;
}

// 0x000b5c00
void ChatStripColorTags(char* text)
{
    while (*text)
    {
        if (ChatParseColorTag(text) != 0xffffffff)
            memmove(text, text + 8, strlen(text + 8) + 1);
        else
            ++text;
    }
    *text = 0;
}

// 0x000b5c40
int ChatPrepareMessage(char* text, int maximumLength)
{
    char buffer[2049];
    memset(buffer, 0, sizeof(buffer));
    if (text && strlen(text) < 400 && strstr(text, "~k~"))
    {
        strncpy_s(buffer, text, _TRUNCATE);

        // GTA SA's key-token expansion routine, outside samp.dll.
        // Native GTA bridge preserves the target's register/stack convention.
        // clang-format off
        __asm {
            lea edx,buffer
            push edx
            mov edx,0x69e160
            call edx
            pop edx
        }
        // clang-format on

        int length = (int)strlen(buffer);
        if (length <= maximumLength)
        {
            memcpy(text, buffer, length + 1);
            return length;
        }
    }
    return 0;
}
