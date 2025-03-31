// DlgAddUPS.cpp : 实现文件
//

#include "stdafx.h"
#include "UPS201501.h"
#include "DlgAddUPS.h"
#include "afxdialogex.h"


// CDlgAddUPS 对话框

IMPLEMENT_DYNAMIC(CDlgAddUPS, CDialogEx)

CDlgAddUPS::CDlgAddUPS(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAddUPS::IDD, pParent)
	, m_str_upsName(_T(""))
	, m_TCPport(0)
{
}

CDlgAddUPS::~CDlgAddUPS()
{
}

void CDlgAddUPS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UPSNAME, m_str_upsName);
	DDX_Text(pDX, IDC_EDIT_TCPPORT, m_TCPport);
	DDV_MinMaxInt(pDX, m_TCPport, 1, 65535);
}


BEGIN_MESSAGE_MAP(CDlgAddUPS, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAddUPS::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddUPS 消息处理程序


BOOL CDlgAddUPS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CEdit* m_edit_upsname = (CEdit*)GetDlgItem(IDC_EDIT_UPSNAME);

	m_str_upsName = _T("上海山源UPS");
	// 默认端口是 3000 
	m_TCPport = 30000;
	GetDlgItem(IDC_IPADDRESS_UPS)->SetWindowText(_T("192.168.22.200"));

	// false:把值传递给控件.
	UpdateData(FALSE);

	m_edit_upsname->SetFocus();
	m_edit_upsname->SetSel(-2, -1);

	
	// 返回 FALSE； 返回FALSE； 返回FALSE； 返回FALSE；
	return FALSE;
	



	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgAddUPS::OnBnClickedOk()
{
	// 合法性检测

	// 将窗口数据传入 控件变量
	UpdateData(TRUE);
	if(m_str_upsName == _T(""))
	{
		MessageBox(_T("请输入UPS名称，谢谢！"), _T("caption"), MB_OK);
		return ;
	}
	
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_UPS))->GetAddress(cip0, cip1, cip2, cip3);
	// 如果输入不合法，就直接 return 
 	if( (cip0 == 0) || (cip3 == 0) )
 	{
 		MessageBox(_T("IP地址非法！！！"), _T("caption"), MB_OK);
 		return ;
 	}
	if( (cip0 == 0) & (cip1 == 0) & (cip2 == 0) & (cip3 == 0))
	{
		MessageBox(_T("IP地址非法！！！-----22222222"), _T("caption        22222222"), MB_OK);
		return ;
	}

	// 输入合法
	m_nPowerIndex = 2;
	m_nVoltageIndex = 0;
	m_nVolumeIndex = 1;
	m_fRsa = 0.01f;

	// 如果输入合法，就调用 OnOK 函数
	CDialogEx::OnOK();
}
