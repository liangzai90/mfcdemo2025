// ProgressDelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UPS201501.h"
#include "ProgressDelDlg.h"
#include "afxdialogex.h"


UINT PrgressDlgWorkerProc(LPVOID pParam)
{
	CProgressDelDlg* pThis = (CProgressDelDlg*)pParam;

	for(int i=0; i<10; i++)
	{
		pThis->m_progress.StepIt();

		Sleep(310);
	}
	
	pThis->OnBtnStop();
	return 0;
}




// CProgressDelDlg �Ի���

IMPLEMENT_DYNAMIC(CProgressDelDlg, CDialogEx)

CProgressDelDlg::CProgressDelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDelDlg::IDD, pParent)
{

}

CProgressDelDlg::~CProgressDelDlg()
{
}

void CProgressDelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(CProgressDelDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDelDlg ��Ϣ�������



void CProgressDelDlg::OnBtnStop()
{
	STOP = TRUE;
}






void CProgressDelDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(STOP)
	{
		OnOK();
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CProgressDelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	STOP = FALSE;
	SetTimer(0, 300, NULL);
	m_progress.SetRange(0, 100);
	m_progress.SetStep(10);
	m_progress.SetPos(0);
	AfxBeginThread(PrgressDlgWorkerProc, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
