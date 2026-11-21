#ifndef CMDWINDOW_H
#define CMDWINDOW_H

#include <d3d9.h>

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

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
#pragma pack(push, 1)
class CInput
{
public:
    // Returns the raw DWORD; distinct from the AL bool getter IsActive.
    // R5 address to verify
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
    // 0x000693d0
    CInput(IDirect3DDevice9*);
    // 0x00069440
    InputRect* GetRect(InputRect*);
    // 0x00069480
    void Open();
    // 0x00069580
    void Close();
    // 0x000695d0
    void AddRecall(const char*);
    // 0x00069630
    void RecallUp();
    // 0x000696a0
    void RecallDown();
    // 0x000696f0
    void EnableCursor();
    // 0x00069710
    InputCommand GetCommandHandler(const char*);
    // 0x00069760
    void SetDefaultCommand(InputCommand);
    // 0x00069770
    void AddCommand(const char*, InputCommand);
    // 0x000697d0
    BOOL MsgProc(int, int, int);
    // 0x00069840
    void ResetDialogControls(CDXUTDialog*);
    // 0x00069900
    void Send(const char*);
    // 0x000699d0
    void ProcessInput();
    // 0x00069810
    bool IsActive();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CInput) == 0x1afc);
STATIC_ASSERT(offsetof(CInput, m_bEnabled) == 0x14e0);
STATIC_ASSERT(offsetof(CInput, m_szRecallBuffer) == 0x1565);

#endif // CMDWINDOW_H
