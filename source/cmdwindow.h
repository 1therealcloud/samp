#ifndef CMDWINDOW_H
#define CMDWINDOW_H

#include <stddef.h>
#include <windows.h>

#include <d3d9.h>

#include <static_assert.h>
class CDXUTDialog;
class CDXUTEditBox;
typedef void(__cdecl* InputCommand)(char*);
// samp.dll RVA: unknown (inline layout helper; no standalone entry established)
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
    // 0x00002e80
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
    // 0x00068e10
    CInput(IDirect3DDevice9*);
    // 0x00068e80
    InputRect* GetRect(InputRect*);
    // 0x00068ec0
    void Open();
    // 0x00068fc0
    void Close();
    // 0x00069010
    void AddRecall(const char*);
    // 0x00069070
    void RecallUp();
    // 0x000690e0
    void RecallDown();
    // 0x00069130
    void EnableCursor();
    // 0x00069150
    InputCommand GetCommandHandler(const char*);
    // 0x000691a0
    void SetDefaultCommand(InputCommand);
    // 0x000691b0
    void AddCommand(const char*, InputCommand);
    // 0x00069210
    BOOL MsgProc(int, int, int);
    // 0x00069280
    void ResetDialogControls(CDXUTDialog*);
    // 0x00069340
    void Send(const char*);
    // 0x00069410
    void ProcessInput();
    // 0x00069250
    bool IsActive();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CInput) == 0x1afc);
STATIC_ASSERT(offsetof(CInput, m_bEnabled) == 0x14e0);
STATIC_ASSERT(offsetof(CInput, m_szRecallBuffer) == 0x1565);

#endif // CMDWINDOW_H
