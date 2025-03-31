// DlgModifyNodeName.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UPS201501.h"
#include "DlgModifyNodeName.h"
#include "afxdialogex.h"


// CDlgModifyNodeName �Ի���

IMPLEMENT_DYNAMIC(CDlgModifyNodeName, CDialogEx)

CDlgModifyNodeName::CDlgModifyNodeName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModifyNodeName::IDD, pParent)
	, m_str_nodeIP(_T(""))
	, m_str_nodename(_T(""))
	, m_upsPort(_T(""))
{

}

CDlgModifyNodeName::~CDlgModifyNodeName()
{
}

void CDlgModifyNodeName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UPSIP, m_str_nodeIP);
	DDX_Text(pDX, IDC_EDIT_UPS_NAME, m_str_nodename);
	DDX_Text(pDX, IDC_EDIT_UPSPORT, m_upsPort);
}


BEGIN_MESSAGE_MAP(CDlgModifyNodeName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgModifyNodeName::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModifyNodeName ��Ϣ�������


void CDlgModifyNodeName::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(_T("") == m_str_nodename)
		return ;
	else
		CDialogEx::OnOK();
}
