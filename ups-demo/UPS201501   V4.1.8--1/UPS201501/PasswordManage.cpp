// PasswordManage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UPS201501.h"
#include "PasswordManage.h"
#include "afxdialogex.h"

CString ReadPwdFromFile();

void WritePwdToFile(CString str_pwd);

CString ReadPwdFromFile()
{
	CString str_pwd = _T("");
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString strtemp = szFilePath;
	CString fileName = _T("UserData.dat");
	strtemp += fileName; 
	CStdioFile stf;
	if (! stf.Open(strtemp, CFile::modeRead|CFile::typeBinary, NULL) )
	{
		// ���ļ�ʧ��
		AfxMessageBox(_T("���ļ�ʧ�ܣ�������򿪷�����ϵ�������°�װ�������"), MB_OK);
		return _T("SHSANY_LOGIN_ERROR");
	}
	stf.ReadString(str_pwd);
	stf.Close();

	return str_pwd;
}

void WritePwdToFile(CString str_pwd)
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString strtemp = szFilePath;
	CString fileName = _T("UserData.dat");
	strtemp += fileName; 
	CStdioFile stf;
	if (! stf.Open(strtemp, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, NULL) )
	{
		// ���ļ�ʧ��
		AfxMessageBox(_T("�ؼ��ļ���ʧ��������򿪷�����ϵ�������°�װ�������"), MB_OK);
		return ;
	}
	stf.WriteString(str_pwd);
	stf.Close();

}



// CPasswordManage �Ի���

IMPLEMENT_DYNAMIC(CPasswordManage, CDialogEx)

CPasswordManage::CPasswordManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPasswordManage::IDD, pParent)
{

}

CPasswordManage::~CPasswordManage()
{
}

void CPasswordManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPasswordManage, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPasswordManage::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CPasswordManage::OnBnClickedOk()
{
	CString str1 = _T("");
	CString str2 = _T("");
	CString str3 = _T("");
	CString str_temp = _T("1234");

	GetDlgItem(IDC_EDIT1)->GetWindowTextW(str1);
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(str2);
	GetDlgItem(IDC_EDIT3)->GetWindowTextW(str3);
	str_temp = ReadPwdFromFile();

	if(_T("SHSANY_LOGIN_ERROR") == str_temp)
		return ;
	if(str_temp == str1)
	{
		if(_T("") == str2)
		{
			GetDlgItem(IDC_STATIC_TIP)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("���벻��Ϊ�գ�"));
			GetDlgItem(IDC_EDIT2)->SetFocus();
			return ;
		}

		if(str2 == str3)
		{
			WritePwdToFile(str3);
			MessageBox(_T("�������޸ĳɹ������μ����������룬��Ҫ���������ˡ�"), _T("�޸ĳɹ���"), MB_OK );
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("��2����������벻ͬ������������"));
			GetDlgItem(IDC_EDIT2)->SetFocus();
			GetDlgItem(IDC_EDIT2)->SetWindowTextW(_T(""));
			GetDlgItem(IDC_EDIT3)->SetWindowTextW(_T(""));
			return ;
		}
	}
	else 
		if (_T("shsany54881286") == str1)
		{
			// ����Ϊ��
			if(_T("") == str2)
			{
				MessageBox(_T("���벻��Ϊ��!"), _T("��ȷ������������"), MB_OK);
				return ;
			}

			if(str2 == str3)
			{
				WritePwdToFile(str3);
				MessageBox(_T("�������޸ĳɹ������μ����������룬��Ҫ���������ˡ�"), _T("�޸ĳɹ���"), MB_OK );
			}
			else
			{
				MessageBox(_T("��2����������벻ͬ�����������롣"), _T("2���������������"), MB_OK );
				GetDlgItem(IDC_EDIT2)->SetWindowTextW(_T(""));
				GetDlgItem(IDC_EDIT3)->SetWindowTextW(_T(""));
				return ;
			}
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_TIP)->SetWindowTextW(_T("���������������������롣"));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			GetDlgItem(IDC_EDIT1)->SetWindowTextW(_T(""));
			GetDlgItem(IDC_EDIT2)->SetWindowTextW(_T(""));
			GetDlgItem(IDC_EDIT3)->SetWindowTextW(_T(""));
			return ;
		}
	CDialogEx::OnOK();
}


HBRUSH CPasswordManage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIP)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}



BOOL CPasswordManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_STATIC_TIP)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT1)->SetFocus();
	
	SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CPasswordManage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN) )
	{
		OnBnClickedOk();
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
