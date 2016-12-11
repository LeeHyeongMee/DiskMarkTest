#pragma once
#include "afxwin.h"
#include "BenchmarkLogic.h"
#include "BenchMarkData.h"
#include "afxcmn.h"
//#include "resource.h"
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
	/*afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();*/
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox comboDirectory;
	CComboBox comboOperation;
	afx_msg void OnCbnSelchangeCombo2();
	CComboBox comboChunk;
	afx_msg void OnCbnSelchangeCombo3();
	CEdit seqThruResult;
	afx_msg void OnEnChangeEdit1();
	CEdit SeqThruResult2;
	CEdit RandTotalResult;
	CEdit RandThruResult;
	CProgressCtrl progressBar;
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
};
