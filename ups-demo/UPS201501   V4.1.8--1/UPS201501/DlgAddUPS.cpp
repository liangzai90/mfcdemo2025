// DlgAddUPS.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UPS201501.h"
#include "DlgAddUPS.h"
#include "afxdialogex.h"


// CDlgAddUPS �Ի���

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


// CDlgAddUPS ��Ϣ�������


BOOL CDlgAddUPS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CEdit* m_edit_upsname = (CEdit*)GetDlgItem(IDC_EDIT_UPSNAME);

	m_str_upsName = _T("�Ϻ�ɽԴUPS");
	// Ĭ�϶˿��� 3000 
	m_TCPport = 30000;
	GetDlgItem(IDC_IPADDRESS_UPS)->SetWindowText(_T("192.168.22.200"));

	// false:��ֵ���ݸ��ؼ�.
	UpdateData(FALSE);

	m_edit_upsname->SetFocus();
	m_edit_upsname->SetSel(-2, -1);

	
	// ���� FALSE�� ����FALSE�� ����FALSE�� ����FALSE��
	return FALSE;
	



	// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgAddUPS::OnBnClickedOk()
{
	// �Ϸ��Լ��

	// ���������ݴ��� �ؼ�����
	UpdateData(TRUE);
	if(m_str_upsName == _T(""))
	{
		MessageBox(_T("������UPS���ƣ�лл��"), _T("caption"), MB_OK);
		return ;
	}
	
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_UPS))->GetAddress(cip0, cip1, cip2, cip3);
	// ������벻�Ϸ�����ֱ�� return 
 	if( (cip0 == 0) || (cip3 == 0) )
 	{
 		MessageBox(_T("IP��ַ�Ƿ�������"), _T("caption"), MB_OK);
 		return ;
 	}
	if( (cip0 == 0) & (cip1 == 0) & (cip2 == 0) & (cip3 == 0))
	{
		MessageBox(_T("IP��ַ�Ƿ�������-----22222222"), _T("caption        22222222"), MB_OK);
		return ;
	}

	// ����Ϸ�
	m_nPowerIndex = 2;
	m_nVoltageIndex = 0;
	m_nVolumeIndex = 1;
	m_fRsa = 0.01f;

	// �������Ϸ����͵��� OnOK ����
	CDialogEx::OnOK();
}
