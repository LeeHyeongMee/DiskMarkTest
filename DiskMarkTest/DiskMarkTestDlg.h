
// DiskMarkTestDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "TAB1.h"
#include "TAB2.h"
#include "Resource.h"

// CDiskMarkTestDlg dialog
class CDiskMarkTestDlg : public CDialogEx
{
// Construction
public:
	CDiskMarkTestDlg(CWnd* pParent = NULL);	// standard constructor

	CTAB1 m_tab1;
	CTAB2 m_tab2;

	CWnd* m_pwndShow;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISKMARKTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
