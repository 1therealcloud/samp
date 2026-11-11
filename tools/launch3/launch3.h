//----------------------------------------------------------
//
// launch3.h
//
//----------------------------------------------------------

#pragma once

#include "resource.h"

class CLaunch3App : public CWinApp
{
public:
    CLaunch3App();
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

extern CLaunch3App theApp;

//----------------------------------------------------
// EOF
