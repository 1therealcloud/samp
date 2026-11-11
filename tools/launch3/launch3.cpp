//----------------------------------------------------------
//
// launch3.cpp
//
//----------------------------------------------------------

#include "stdafx.h"
#include "launch3.h"
#include "launch3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------

BEGIN_MESSAGE_MAP(CLaunch3App, CWinApp)
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

//----------------------------------------------------

// 0x1010
CLaunch3App::CLaunch3App() {}

//----------------------------------------------------

CLaunch3App theApp;

//----------------------------------------------------

// 0x10A0
BOOL CLaunch3App::InitInstance()
{
    AfxEnableControlContainer();

    CLaunch3Dlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}

//----------------------------------------------------
// EOF
