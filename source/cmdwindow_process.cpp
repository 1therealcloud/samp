#include "gui/input_native.h"

// Recovered from 0x10069410. Byte identity is not required for register restores.
// samp.dll RVA: 0x00069410
void CInput::ProcessInput() {
    if (!m_pEditbox)
        return;

    strncpy(m_szInput, m_pEditbox->GetText(), 128);
    m_szInput[128] = 0;
    if (strlen(m_szInput)) {
        AddRecall(m_szInput);
        m_nCurrentRecall = -1;

        if (m_szInput[0] != '/') {
            if (m_pDefaultCommand)
                m_pDefaultCommand(m_szInput);
        } else {
            char *name = m_szInput + 1;
            char *space = name;
            while (*space && *space != ' ')
                ++space;

            if (!*space) {
                InputCommand command = GetCommandHandler(name);
                if (command)
                    command("");
                else if (g_pNetGame)
                    Send(m_szInput);
                else
                    g_pChat->AddDebugMessage("I don't know that command.");
            } else {
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

    if (m_bEnabled) {
        if (g_pChat)
            g_pChat->ScrollToBottom();
        Close();
    }
}
