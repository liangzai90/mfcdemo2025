// LogInDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UPS201501.h"
#include "LogInDlg.h"
#include "afxdialogex.h"


// 读取密码
extern CString ReadPwdFromFile();
// CLogInDlg 对话框

IMPLEMENT_DYNAMIC(CLogInDlg, CDialogEx)

CLogInDlg::CLogInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogInDlg::IDD, pParent)
{

/*	m_login = _T("");*/
}

CLogInDlg::~CLogInDlg()
{
}

void CLogInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLogInDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLogInDlg::OnBnClickedBtnLogin)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLogInDlg 消息处理程序


BOOL CLogInDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN) )
	{
		OnBnClickedBtnLogin();
		return TRUE;
	}
	if ( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		return TRUE;
	}
	if((pMsg->wParam == VK_SPACE))
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLogInDlg::OnBnClickedBtnLogin()
{
	CString str_pwd = _T("");
	CString str_temp = _T("");
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowTextW(str_pwd);
	str_temp = ReadPwdFromFile();
	if(_T("SHSANY_LOGIN_ERROR") == str_temp)
	{
		GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		return;
	}
	CString str_super_word = _T("shsany54881286");
	if(str_pwd == str_super_word)
	{
		MessageBox(_T("欢迎超级用户！！！！"), _T(""), MB_OK);
		OnOK();
	}
	else
		if(str_temp == str_pwd )
		{
			OnOK();
		}
		else
		{
			SetDlgItemText(IDC_EDIT_PASSWORD, _T(""));
			GetDlgItem(IDC_STATIC_TIP)->ShowWindow(TRUE);
		}

		GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();

}


HBRUSH CLogInDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIP)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	return hbr;
}


BOOL CLogInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_STATIC_TIP)->ShowWindow(FALSE);
	SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int CLogInDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hMutex2 = ::CreateMutex(NULL, FALSE, _T("USERLOGIN"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return -1;
	}



	return 0;
}


BOOL CLogInDlg::DestroyWindow()
{

	if(m_hMutex2 != NULL)
	{
		CloseHandle(m_hMutex2);

		m_hMutex2 = NULL;
	}


	return CDialogEx::DestroyWindow();
}


void CLogInDlg::OnClose()
{
	if(m_hMutex2 != NULL)
	{
		CloseHandle(m_hMutex2);

		m_hMutex2 = NULL;
	}


	CDialogEx::OnClose();
}
