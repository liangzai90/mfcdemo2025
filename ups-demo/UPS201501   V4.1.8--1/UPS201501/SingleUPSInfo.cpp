// SingleUPSInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "UPS201501.h"
#include "SingleUPSInfo.h"
#include "afxdialogex.h"

#include <vector>

extern std::vector<CString>  v_str_Para;
extern CString str_Selected_UPSName;
// CSingleUPSInfo 对话框

IMPLEMENT_DYNAMIC(CSingleUPSInfo, CDialogEx)

CSingleUPSInfo::CSingleUPSInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSingleUPSInfo::IDD, pParent)
{

}

CSingleUPSInfo::~CSingleUPSInfo()
{
}

void CSingleUPSInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSingleUPSInfo, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CSingleUPSInfo::InitData()
{
	GetDlgItem(IDC_EDIT_UPSNAME)->SetWindowText(_T(""));

	GetDlgItem(IDC_STATIC_T1)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V1)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T2)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V2)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T3)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V3)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T4)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V4)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T5)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V5)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T6)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V6)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T7)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V7)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T8)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V8)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T9)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V9)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T10)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V10)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T11)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V11)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_T12)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_V12)->SetWindowText(_T(""));

}

void CSingleUPSInfo::ShowData()
{
	GetDlgItem(IDC_EDIT_UPSNAME)->SetWindowText(str_Selected_UPSName);
	// 12节电池单体电压
	GetDlgItem(IDC_STATIC_V1)->SetWindowText(v_str_Para.at(0));
	GetDlgItem(IDC_STATIC_V2)->SetWindowText(v_str_Para.at(1));
	GetDlgItem(IDC_STATIC_V3)->SetWindowText(v_str_Para.at(2));
	GetDlgItem(IDC_STATIC_V4)->SetWindowText(v_str_Para.at(3));
	GetDlgItem(IDC_STATIC_V5)->SetWindowText(v_str_Para.at(4));
	GetDlgItem(IDC_STATIC_V6)->SetWindowText(v_str_Para.at(5));
	GetDlgItem(IDC_STATIC_V7)->SetWindowText(v_str_Para.at(6));
	GetDlgItem(IDC_STATIC_V8)->SetWindowText(v_str_Para.at(7));
	GetDlgItem(IDC_STATIC_V9)->SetWindowText(v_str_Para.at(8));
	GetDlgItem(IDC_STATIC_V10)->SetWindowText(v_str_Para.at(9));
	GetDlgItem(IDC_STATIC_V11)->SetWindowText(v_str_Para.at(10));
	GetDlgItem(IDC_STATIC_V12)->SetWindowText(v_str_Para.at(11));

	// 电池单体温度
	GetDlgItem(IDC_STATIC_T1)->SetWindowTextW(v_str_Para.at(15));
	GetDlgItem(IDC_STATIC_T2)->SetWindowTextW(v_str_Para.at(16));
	GetDlgItem(IDC_STATIC_T3)->SetWindowTextW(v_str_Para.at(17));
	GetDlgItem(IDC_STATIC_T4)->SetWindowTextW(v_str_Para.at(18));
	GetDlgItem(IDC_STATIC_T5)->SetWindowTextW(v_str_Para.at(19));
	GetDlgItem(IDC_STATIC_T6)->SetWindowTextW(v_str_Para.at(20));
	GetDlgItem(IDC_STATIC_T7)->SetWindowTextW(v_str_Para.at(21));
	GetDlgItem(IDC_STATIC_T8)->SetWindowTextW(v_str_Para.at(22));
	GetDlgItem(IDC_STATIC_T9)->SetWindowTextW(v_str_Para.at(23));
	GetDlgItem(IDC_STATIC_T10)->SetWindowTextW(v_str_Para.at(24));
	GetDlgItem(IDC_STATIC_T11)->SetWindowTextW(v_str_Para.at(25));
	GetDlgItem(IDC_STATIC_T12)->SetWindowTextW(v_str_Para.at(26));

}




void CSingleUPSInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowData();
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSingleUPSInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitData();
	ShowData();
	SetTimer(1, 2000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
