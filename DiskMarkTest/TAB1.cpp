// TAB1.cpp : implementation file
//

#include "stdafx.h"
#include "DiskMarkTest.h"
#include "TAB1.h"
#include "afxdialogex.h"
#include "resource.h"
//#include "BenchMarkData.h"
UINT WorkerThread(LPVOID lParam); //global function
// CTAB1 dialog

IMPLEMENT_DYNAMIC(CTAB1, CDialog)


CTAB1::CTAB1(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TAB1_DIALOG, pParent)
{

}

CTAB1::~CTAB1()
{
}

void CTAB1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comboTrialNumber);
	DDX_Control(pDX, IDC_COMBO2, comboDirectory);
	DDX_Control(pDX, IDC_COMBO3, comboOperation);
	DDX_Control(pDX, IDC_COMBO4, comboChunk);
	DDX_Control(pDX, IDC_EDIT1, seqThruResult);
	DDX_Control(pDX, IDC_EDIT2, SeqThruResult2);
	DDX_Control(pDX, IDC_EDIT3, RandTotalResult);
	DDX_Control(pDX, IDC_EDIT4, RandThruResult);
	DDX_Control(pDX, IDC_PROGRESS1, progressBar);
}


BEGIN_MESSAGE_MAP(CTAB1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAB1::OnBnClickedButton1)
/*	ON_BN_CLICKED(IDC_BUTTON2, &CTAB1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTAB1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTAB1::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTAB1::OnBnClickedButton5)*/
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTAB1::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTAB1::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CTAB1::OnCbnSelchangeCombo3)
	ON_EN_CHANGE(IDC_EDIT1, &CTAB1::OnEnChangeEdit1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CTAB1::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()


// CTAB1 message handlers

UINT WorkerThread(LPVOID lParam)   //Thread를호출한클래스의주소값을받는다
{
	CTAB1* dlg = (CTAB1*)lParam; //호출한클래스의포인터객체생성
	int per = 0;
	for (int i = 0; i<10000; i++)  //0부터999까지1000번의루프를실행하며
	{
		Sleep(1);
		dlg->progressBar.SetPos(i);       //프로그레스바를수정
		if (i % 10 == 0)
		{
			per++;
			//dlg->m_strWT.Format(_T("%d%%"), per);       //퍼센트출력
			//dlg->SetDlgItemTextW(IDC_STATIC_WT, dlg->m_strWT);
		}
	}
	dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);     //작업완료시버튼활성화
	return 0;
}


void CTAB1::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//Start Button

	UpdateData();
	
	//AfxBeginThread(ThreadFunc, progressBar);
/*	progressBar.SetRange(0, 100);
	progressBar.SetPos(0);
	progressBar.SetStep(1);

	for (int count = 0; count < 100; count++)
	{
		progressBar.StepIt();

		Sleep(10);
	}*/

	AfxBeginThread(WorkerThread, this);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(0);


	int numOfTrial = comboTrialNumber.GetCurSel() + 1; //0, 1, 2, 3, 4 to 1, 2, 3, 4, 5
	int dirSel = comboDirectory.GetCurSel();
	int chunk = comboChunk.GetCurSel();
	int operation = comboOperation.GetCurSel();
	//identify the directory
	/*CString strl = _T("seqThruText");
	seqThruResult.SetWindowText(strl);*/
	char dir;
	CString temp;
	this->comboDirectory.GetLBText(this->comboDirectory.GetCurSel(), temp);
	
	if (temp.CompareNoCase( _T("C")))
	{
		dir = 'C';
	}
	else if (temp.CompareNoCase(_T("D")))
	{
		dir = 'D';
	}
	else if (temp.CompareNoCase(_T("E")))
	{
		dir = 'E';
	}

	//identify the chunk size
	if (chunk == 0)
	{
		chunk = 512;
	}
	else if (chunk == 1)
	{
		chunk = 1024;
	}
	else if (chunk == 2)
	{
		chunk = 2048;
	}
	else if (chunk == 3)
	{
		chunk = 4096;
	}
	else if (chunk == 4)
	{
		chunk = 1024 * 8;
	}
	else if (chunk == 5)
	{
		chunk = 1024 * 16;
	}
	
	BenchMarkData* randData = NULL;
	BenchMarkData* seqData = NULL;
	
	
	CString Sum;
	CString band;

	if (operation == 0)
	{
		//read operation
		randData = main_thr(2, dir, numOfTrial, chunk);
		seqData = main_thr(1, dir, numOfTrial, chunk);
		//random
		Sum = _T("%d", seqData->seqRead);
		seqThruResult.SetWindowText(Sum);
		band = _T("%d", seqData->bandwidth);
		SeqThruResult2.SetWindowTextW(band);
		//sequential
		Sum = _T("%d", randData->randRead);
		RandTotalResult.SetWindowText(Sum);
		band = _T("%d", randData->bandwidth);
		RandThruResult.SetWindowText(band);
	}
	else if (operation == 1)
	{
		//write operation
		randData = main_thr(4, dir, numOfTrial, chunk);
		seqData = main_thr(3, dir, numOfTrial, chunk);
		Sum = _T("%d", seqData->seqWrite);
		seqThruResult.SetWindowText(Sum);
		band = _T("%d", seqData->bandwidth);
		SeqThruResult2.SetWindowTextW(band);
		//sequential
		Sum = _T("%d", randData->randWrite);
		RandTotalResult.SetWindowText(Sum);
		band = _T("%d", randData->bandwidth);
		RandThruResult.SetWindowText(band);
	}
	
	
//	VirtualFree(seqData, sizeof(BenchMarkData*), MEM_DECOMMIT);
//	VirtualFree(randData, sizeof(BenchMarkData*), MEM_DECOMMIT);
//	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(1);
	
}




void CTAB1::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}


void CTAB1::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
}


void CTAB1::OnCbnSelchangeCombo3()
{
	// TODO: Add your control notification handler code here
}


void CTAB1::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}




void CTAB1::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
