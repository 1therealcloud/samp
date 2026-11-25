#ifndef CMDWINDOW_H
#define CMDWINDOW_H

#include <d3d9.h>

#include <stddef.h>
#include <windows.h>

class CDXUTDialog;
class CDXUTEditBox;
typedef void(__cdecl* InputCommand)(char*);
// inline
struct InputRect : RECT
{
    InputRect()
    {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }
};
class CInput
{
public:
    // Returns the raw DWORD; distinct from the AL bool getter IsActive.

    DWORD GetEnabledState();
    IDirect3DDevice9* m_pDevice;
    CDXUTDialog* m_pGameUi;
    CDXUTEditBox* m_pEditbox;
    InputCommand m_pCommandProc[144];
    char m_szCommandName[144][33];
    int m_nCommandCount;
    BOOL m_bEnabled;
    char m_szInput[129], m_szRecallBuffer[10][129], m_szCurrentBuffer[129];
    int m_nCurrentRecall, m_nTotalRecall;
    InputCommand m_pDefaultCommand;

    CInput(IDirect3DDevice9*);

    InputRect* GetRect(InputRect*);

    void Open();

    void Close();

    void AddRecall(const char*);

    void RecallUp();

    void RecallDown();

    void EnableCursor();

    InputCommand GetCommandHandler(const char*);

    void SetDefaultCommand(InputCommand);

    void AddCommand(const char*, InputCommand);

    BOOL MsgProc(int, int, int);

    void ResetDialogControls(CDXUTDialog*);

    void Send(const char*);

    void ProcessInput();

    bool IsActive();
};
static_assert(sizeof(CInput) == 0x1afc);
static_assert(offsetof(CInput, m_bEnabled) == 0x14e0);
static_assert(offsetof(CInput, m_szRecallBuffer) == 0x1565);

#endif // CMDWINDOW_H
