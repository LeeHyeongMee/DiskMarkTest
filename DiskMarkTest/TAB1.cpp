// TAB1.cpp : implementation file
//

#include "stdafx.h"
#include "DiskMarkTest.h"
#include "TAB1.h"
#include "afxdialogex.h"
//#include "BenchMarkData.h"

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
}


BEGIN_MESSAGE_MAP(CTAB1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAB1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTAB1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTAB1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTAB1::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTAB1::OnBnClickedButton5)
END_MESSAGE_MAP()


// CTAB1 message handlers


void CTAB1::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//random read operation

	UpdateData();
	int numOfTrial = comboTrialNumber.GetCurSel();
	//UpdateData();
	BenchMarkData* value = main_thr(3, numOfTrial + 5);
	CString v1_sMessageToDisplay;
	v1_sMessageToDisplay.Format(_T("number of trial is %d, and the time is %d"), numOfTrial+5, value->randRead[5]);
	MessageBox(v1_sMessageToDisplay, 0, MB_ICONINFORMATION);
}


void CTAB1::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	//random write operation
	UpdateData();
	int numOfTrial = comboTrialNumber.GetCurSel();
	//UpdateData();

	BenchMarkData* value = main_thr(4, numOfTrial+5);
	CString v1_sMessageToDisplay;
	v1_sMessageToDisplay.Format(_T("number of trial is %d, and time is %d"), value->trials, value->randWrite[5]);
	MessageBox(v1_sMessageToDisplay, 0, MB_ICONINFORMATION);
}


void CTAB1::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//sequential read operation
	UpdateData();
	int numOfTrial = comboTrialNumber.GetCurSel();
	
	//long long value = main_thr(1);
	BenchMarkData* tmp = main_thr(1, numOfTrial+5);
	CString v1_sMessageToDisplay;
	v1_sMessageToDisplay.Format(_T("time is %d, %d, %d, %d, %d, %d"), 
		tmp->seqRead[0], tmp->seqRead[1], tmp->seqRead[2], tmp->seqRead[3], tmp->seqRead[4], tmp->seqRead[5]);
	MessageBox(v1_sMessageToDisplay, 0, MB_ICONINFORMATION);
}


void CTAB1::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int numOfTrial = comboTrialNumber.GetCurSel();

	//long long value = main_thr(1);
	BenchMarkData* tmp = main_thr(2, numOfTrial + 5);
	CString v1_sMessageToDisplay;
	v1_sMessageToDisplay.Format(_T("time is %d, %d, %d, %d, %d, %d"),
		tmp->seqWrite[0], tmp->seqWrite[1], tmp->seqWrite[2], tmp->seqWrite[3], tmp->seqWrite[4], tmp->seqWrite[5]);
	MessageBox(v1_sMessageToDisplay, 0, MB_ICONINFORMATION);
}


void CTAB1::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	/*UpdateData();
	int numOfTrial = comboTrialNumber.GetCurSel();

	//long long value = main_thr(1);
	BenchMarkData* tmp = main_thr(2, numOfTrial + 5);
	CString v1_sMessageToDisplay;
	v1_sMessageToDisplay.Format(_T("time is %d, %d, %d, %d, %d, %d"),
		tmp->seqRead[0], tmp->seqRead[1], tmp->seqRead[2], tmp->seqRead[3], tmp->seqRead[4], tmp->seqRead[5]);
	MessageBox(v1_sMessageToDisplay, 0, MB_ICONINFORMATION);*/
}
