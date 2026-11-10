//----------------------------------------------------------
//
// launch3Dlg.h
//
//----------------------------------------------------------

#pragma once

class CLaunch3Dlg : public CDialog
{
public:
	CLaunch3Dlg(CWnd* pParent = NULL);
	enum { IDD = IDD_LAUNCH3_DIALOG };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeNick();
	afx_msg void OnLaunch();
	afx_msg void OnButton2();
	afx_msg void OnButton3();

	DECLARE_MESSAGE_MAP()
};

//----------------------------------------------------
// EOF
