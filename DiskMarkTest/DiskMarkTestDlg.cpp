
// DiskMarkTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DiskMarkTest.h"
#include "DiskMarkTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDiskMarkTestDlg dialog



CDiskMarkTestDlg::CDiskMarkTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DISKMARKTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDiskMarkTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CDiskMarkTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDiskMarkTestDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CDiskMarkTestDlg message handlers

BOOL CDiskMarkTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CString strTab = _T("");
	strTab.Format(_T("TAB1"));
	this->m_Tab.InsertItem(0, strTab, 0);
	strTab.Format(_T("TAB2"));
	this->m_Tab.InsertItem(1, strTab, 0);

	CRect rect;
	this->m_Tab.GetClientRect(&rect);
	this->m_tab1.Create(IDD_TAB1_DIALOG, &this->m_Tab);
	this->m_tab1.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	this->m_tab2.Create(IDD_TAB2_DIALOG, &this->m_Tab);
	this->m_tab2.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW);

	this->m_pwndShow = &this->m_tab1;

	CTAB1* tab = &this->m_tab1;
	tab->comboTrialNumber.AddString(_T("1"));
	tab->comboTrialNumber.AddString(_T("2"));
	tab->comboTrialNumber.AddString(_T("3"));
	tab->comboTrialNumber.AddString(_T("4"));
	tab->comboTrialNumber.AddString(_T("5"));

	DWORD dwDriveList = ::GetLogicalDrives();
	int directoryIdx = 0; //C, D, E from 0 to 2
	int dirList[5];

	while (dwDriveList)
	{
		// DWORD형으로 넘어온 값 하나씩 '&' 연산으로 드라이브 존재 유무 판단
		if (dwDriveList & 1)
		{
			// "C:\"과 같이 드라이브를 표시하는 문자열로 만듦
			// ‘A' 값 기준으로 아스키코드 값 이용
			dirList[directoryIdx] = 1;
			//	m_strDrive.SetAt(0, 'A' + m_nDrivePos);
		}
		// 우로 1비트 이동(= 다음 드라이브 체크)
		else
		{
			dirList[directoryIdx] = -1;
		}
		directoryIdx++;
		dwDriveList >>= 1;
		//	m_nDrivePos++;
	} 
	
	if (dirList[2] == 1)
	{
		tab->comboDirectory.AddString(_T("C"));
	}
	
	if (dirList[3] == 1)
	{
		tab->comboDirectory.AddString(_T("D"));
	}
	
	if (dirList[4] == 1)
	{
		tab->comboDirectory.AddString(_T("E"));
	}
	

	tab->comboOperation.AddString(_T("READ"));
	tab->comboOperation.AddString(_T("WRITE"));
	 
	tab->comboChunk.AddString(_T("512Byte"));
	tab->comboChunk.AddString(_T("1KB"));
	tab->comboChunk.AddString(_T("2KB"));
	tab->comboChunk.AddString(_T("4KB"));
	tab->comboChunk.AddString(_T("8KB"));
	tab->comboChunk.AddString(_T("16KB"));
	
	//tab->comboTrialNumber.SetCurSel(0);
	tab->progressBar.SetRange(0, 10000);



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDiskMarkTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDiskMarkTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDiskMarkTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDiskMarkTestDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if (this->m_pwndShow != NULL)
	{
		this->m_pwndShow->ShowWindow(SW_HIDE);
		this->m_pwndShow = NULL;
	}

	int tabIndex = this->m_Tab.GetCurSel();

	switch (tabIndex)
	{
	case 0:
		this->m_tab1.ShowWindow(SW_SHOW);
		this->m_pwndShow = &this->m_tab1;
		break;

	case 1:
		this->m_tab2.ShowWindow(SW_SHOW);
		this->m_pwndShow = &this->m_tab2;
		break;
	}

	*pResult = 0;
}
