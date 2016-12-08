#pragma once


// CTAB2 dialog

class CTAB2 : public CDialog
{
	DECLARE_DYNAMIC(CTAB2)

public:
	CTAB2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTAB2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB2_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
