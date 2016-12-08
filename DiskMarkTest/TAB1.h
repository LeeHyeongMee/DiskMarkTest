#pragma once
#include "afxwin.h"
#include "BenchmarkLogic.h"

// CTAB1 dialog

class CTAB1 : public CDialog
{
	DECLARE_DYNAMIC(CTAB1)

public:
	CTAB1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTAB1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB1_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboTrialNumber;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
