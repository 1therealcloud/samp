#include "cmdwindow.h"
#include "main.h"
#include "runutil.h"
#include <DXUTgui.h>
// 0x00002e80
DWORD CInput::GetEnabledState()
{
    return m_bEnabled;
}
#include "chatwindow.h"
#include "cmdwindow.h"
#include "config.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <DXUTmisc.h>
#include <assert.h>
#include <new>
#include <string.h>
// 0x00068e10
CInput::CInput(IDirect3DDevice9* device)
{
    m_bEnabled = FALSE;
    m_pDevice = device;
    m_nCommandCount = 0;
    m_nTotalRecall = 0;
    m_pEditbox = 0;
    m_nCurrentRecall = -1;
    memset(m_szRecallBuffer, 0, sizeof(m_szRecallBuffer));
    memset(m_szInput, 0, sizeof(m_szInput));
    memset(m_szCurrentBuffer, 0, sizeof(m_szCurrentBuffer));
}
// 0x00068e80
InputRect* CInput::GetRect(InputRect* rect)
{
    memset(rect, 0, sizeof(*rect));
    if (m_pGameUi)
    {
        rect->right = m_pGameUi->GetWidth();
        rect->bottom = m_pGameUi->GetHeight();
    }
    return rect;
}
// 0x00068ec0
void CInput::Open()
{
    if (!m_bEnabled)
    {
        if (m_pEditbox)
        {
            RECT rect;
            GetClientRect(g_inputGameWindow, &rect);
            int width = (int)(rect.right * 0.6f);
            if (width > 800)
                width = 800;
            m_pEditbox->SetEnabled(true);
            m_pEditbox->SetVisible(true);
            m_pEditbox->SetLocation(40, g_pChat->m_nWindowBottom);
            m_pEditbox->SetSize(width, 14 - (int)(m_pGameUi->GetFont(0)->nHeight * -1.5f));
            m_pGameUi->RequestFocus(m_pEditbox);
            m_pEditbox->OnFocusIn();
            int bottom = g_pChat->m_nWindowBottom;
            m_pGameUi->SetSize(width + 100, bottom + 50);
        }
        m_bEnabled = TRUE;
    }
}
// 0x00069010
void CInput::AddRecall(const char* text)
{
    for (int i = 8; i >= 0; --i)
        strcpy(m_szRecallBuffer[i + 1], m_szRecallBuffer[i]);
    strcpy(m_szRecallBuffer[0], text);
    if (m_nTotalRecall < 10)
        ++m_nTotalRecall;
}
// 0x00069070
void CInput::RecallUp()
{
    if (m_nCurrentRecall < m_nTotalRecall - 1)
    {
        if (m_nCurrentRecall == -1)
        {
            strncpy(m_szCurrentBuffer, m_pEditbox->GetText(), 128);
            m_szCurrentBuffer[128] = 0;
        }
        ++m_nCurrentRecall;
        m_pEditbox->SetText(m_szRecallBuffer[m_nCurrentRecall], false);
    }
}
// 0x000690e0
void CInput::RecallDown()
{
    --m_nCurrentRecall;
    if (m_nCurrentRecall >= 0)
    {
        m_pEditbox->SetText(m_szRecallBuffer[m_nCurrentRecall], false);
        return;
    }
    if (m_nCurrentRecall == -1)
        m_pEditbox->SetText(m_szCurrentBuffer, false);
    m_nCurrentRecall = -1;
}
// 0x00069130
void CInput::EnableCursor()
{
    if (m_bEnabled)
        g_pGame->SetCursorMode(2, FALSE);
}
// 0x00069150
InputCommand CInput::GetCommandHandler(const char* name)
{
    for (int i = 0; i != m_nCommandCount; ++i)
        if (!_stricmp(name, m_szCommandName[i]))
            return m_pCommandProc[i];
    return 0;
}
// 0x000691a0
void CInput::SetDefaultCommand(InputCommand command)
{
    m_pDefaultCommand = command;
}
// 0x000691b0
void CInput::AddCommand(const char* name, InputCommand command)
{
    if (m_nCommandCount < 144 && strlen(name) < 32)
    {
        m_pCommandProc[m_nCommandCount] = command;
        strcpy(m_szCommandName[m_nCommandCount], name);
        ++m_nCommandCount;
    }
}
// 0x00069210
BOOL CInput::MsgProc(int message, int wparam, int lparam)
{
    if (m_bEnabled && m_pEditbox && CDXUTIMEEditBox::StaticMsgProc(message, wparam, lparam))
        return TRUE;
    return FALSE;
}
// 0x00069280
void CInput::ResetDialogControls(CDXUTDialog* dialog)
{
    m_pGameUi = dialog;
    if (dialog)
    {
        dialog->AddIMEEditBox(1, "", 10, 175, 570, 40, true, (CDXUTIMEEditBox**)&m_pEditbox);
        if (g_pConfig->GetIntValue("ime"))
        {
            InputEnableIme(true);
            InputResetIme();
        }
        m_pEditbox->m_Elements[0]->TextureColor.Init(0xf0050505, 0xc8808080, 0);
        m_pEditbox->SetTextColor(0xffffffff);
        m_pEditbox->SetCaretColor(0xff969696);
        m_pEditbox->SetSelectedBackColor(0xffb92228);
        m_pEditbox->SetSelectedTextColor(0xff0a0a0f);
        m_pEditbox->SetEnabled(false);
        m_pEditbox->SetVisible(false);
    }
}

// 0x00068fc0
void CInput::Close()
{
    if (m_bEnabled)
    {
        if (m_pEditbox)
        {
            m_pEditbox->OnFocusOut();
            m_pEditbox->SetEnabled(false);
            m_pEditbox->SetVisible(false);
        }
        g_pGame->SetCursorMode(0, TRUE);
        m_bEnabled = FALSE;
    }
}

// Recovered from 0x10069410. Byte identity is not required for register restores.
// 0x00069410
void CInput::ProcessInput()
{
    if (!m_pEditbox)
        return;

    strncpy(m_szInput, m_pEditbox->GetText(), 128);
    m_szInput[128] = 0;
    if (strlen(m_szInput))
    {
        AddRecall(m_szInput);
        m_nCurrentRecall = -1;

        if (m_szInput[0] != '/')
        {
            if (m_pDefaultCommand)
                m_pDefaultCommand(m_szInput);
        }
        else
        {
            char* name = m_szInput + 1;
            char* space = name;
            while (*space && *space != ' ')
                ++space;

            if (!*space)
            {
                InputCommand command = GetCommandHandler(name);
                if (command)
                    command("");
                else if (g_pNetGame)
                    Send(m_szInput);
                else
                    g_pChat->AddDebugMessage("I don't know that command.");
            }
            else
            {
                // Preserve the complete command before splitting its name.
                char full[129];
                strcpy(full, m_szInput);
                *space = 0;
                InputCommand command = GetCommandHandler(name);
                if (command)
                    command(space + 1);
                else if (g_pNetGame)
                    Send(full);
                else
                    g_pChat->AddDebugMessage("I don't know that command.");
            }
        }

        m_szInput[0] = 0;
        m_pEditbox->SetText("", false);
    }

    if (m_bEnabled)
    {
        if (g_pChat)
            g_pChat->ScrollToBottom();
        Close();
    }
}

#include "BitStream.h"
#include "RakClientInterface.h"
#include "cmdwindow.h"
#include <string.h>

// 0x002aca24
extern void* g_pNetGame;

// Partial view: only the independently observed client field is described.
#pragma pack(push, 1)
struct InputNetGameView
{
    unsigned char unknown[0x2c];
    RakClientInterface* client;
};
#pragma pack(pop)
typedef char InputClientOffset[(offsetof(InputNetGameView, client) == 0x2c) ? 1 : -1];

// Native 0x1011c310 contains the 32-bit numeric RPC identifier 50.
// 0x0011c310
int g_inputCommandRpcId = 50;

// Recovered from 0x10069340. The input length excludes the trailing NUL.
// 0x00069340
void CInput::Send(const char* text)
{
    if (g_pNetGame)
    {
        RakNet::BitStream stream;
        int length = (int)strlen(text);
        stream.Write(length);
        stream.Write(text, length);
        InputNetGameView* netGame = (InputNetGameView*)g_pNetGame;
        netGame->client->RPC(&g_inputCommandRpcId, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
    }
}

// 0x00069250
bool CInput::IsActive()
{
    if (m_pEditbox)
        return ChatImeActive();
    return false;
}
