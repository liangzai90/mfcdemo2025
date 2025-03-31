// SoeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UPS201501.h"
#include "SoeDlg.h"
#include "afxdialogex.h"
#include <locale>
#include "TcpClient.h"

CArray<UPS_SOE, UPS_SOE> m_soe_array;
CArray<UPS_SOE, UPS_SOE> m_soe_history_array;

BOOL b_SoeShow = FALSE;
BOOL b_Update = FALSE;


extern CArray<UPS_NODE_CB, UPS_NODE_CB> m_ups_array;
extern wchar_t g_cs_module_path[_MAX_PATH];////Dlg.cpp
const wchar_t SOE_FILE_NAME[] = L"\\shsyrecords.csv";





int s_SaveSoeRecordtoFile(const wchar_t *strFile)
{
	ASSERT(strFile != NULL);

	setlocale(LC_CTYPE, "chs" );//设定 

	CString cs;
	CStdioFile my_file(strFile, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	int ps = m_soe_array.GetCount();
	if(ps > 0)
	{
		my_file.SeekToEnd();
		ps -= 1;
		UPS_SOE obj;
		obj = m_soe_array.GetAt(ps);

		cs.Format(_T("%d,%d,%ld,"), ps+1, obj.dpi, obj.tmStamp);

		cs += obj.wsIP;
		cs += L",";
		cs += obj.wsName;
		cs += L",";
		cs += obj.wsNote;
		cs += L"\n";
		my_file.WriteString(cs);
	}
	my_file.Close();
	return 0;
}


int s_ReadSoeRecordsFromFile(const wchar_t *strFile)
{
	ASSERT(strFile != NULL);

	setlocale(LC_CTYPE, "chs" );

	CString cs;
	cs = strFile;
	CStdioFile my_file;
	if(my_file.Open(cs, CFile::modeRead))
	{
		UPS_SOE obj;
		CString cstr;
		int ps, length;
		while(my_file.ReadString(cs))
		{
			length = cs.GetLength();
			ps = cs.ReverseFind(',');
			if(ps > 0)
			{
				wcscpy_s(obj.wsNote, 120, cs.Right(length - ps - 1));
				cstr = cs.Left(ps);

				cs = cstr;
				length = ps;
				ps = cs.ReverseFind(',');
				if(ps > 0)
				{
					wcscpy_s(obj.wsName, 120, cs.Right(length - ps - 1));
					cstr = cs.Left(ps);
					cs = cstr;
					length = ps;

					ps = cs.ReverseFind(',');
					if(ps > 0)
					{						
						wcscpy_s(obj.wsIP, 30, cs.Right(length - ps - 1));
						cstr = cs.Left(ps);
						cs = cstr;
						length = ps;
						ps = cs.ReverseFind(',');
						obj.tmStamp = _wtoi(cs.Right(length - ps-1));
						cstr = cs.Left(ps);
						cs = cstr;
						length = ps;
						ps = cs.ReverseFind(',');
						if(ps > 0)
						{
							obj.dpi = _wtoi(cs.Right(length - ps-1));
							m_soe_array.Add(obj);
						}
					}
				}
			}
		}
		my_file.Close();		
	}	
	return 0;
}




// CSoeDlg 对话框

IMPLEMENT_DYNAMIC(CSoeDlg, CDialogEx)

CSoeDlg::CSoeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoeDlg::IDD, pParent)
{
}

CSoeDlg::~CSoeDlg()
{

}

void CSoeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SOE_LIST, m_SoeList);
	DDX_Control(pDX, IDC_COMBO_UPSNAME, mListUPS);
}


BEGIN_MESSAGE_MAP(CSoeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_FIND_RECORD, &CSoeDlg::OnBnClickedFindRecord)
	ON_BN_CLICKED(IDC_BTN_REALTIME_WARN, &CSoeDlg::OnBnClickedBtnRealtimeWarn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SOE_LIST, &CSoeDlg::OnCustomdrawSoeList)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_TEST_SOE, &CSoeDlg::OnBnClickedBtnTestSoe)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSoeDlg 消息处理程序



void CSoeDlg::OnBnClickedFindRecord()
{
	CDateTimeCtrl *pCtrl;
	pCtrl = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_START1);
	pCtrl->GetTime(m_StartDate);
	pCtrl = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_START2);
	pCtrl->GetTime(m_StartTm);

	pCtrl = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_END1);
	pCtrl->GetTime(m_EndDate);
	pCtrl = (CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_END2);
	pCtrl->GetTime(m_EndTm);


	CButton *pBtn1;
	pBtn1 = (CButton *)GetDlgItem(IDC_RADIO_TIME);

	int count = m_soe_array.GetCount();
	UPS_SOE obj;

	if(pBtn1->GetCheck() > 0)
	{
		m_FindType = 0;
		CTime myStart, myEnd;
		myStart = CTime(m_StartDate.GetYear(), m_StartDate.GetMonth(),m_StartDate.GetDay(),m_StartTm.GetHour(), m_StartTm.GetMinute(),m_StartTm.GetSecond());
		myEnd = CTime(m_EndDate.GetYear(), m_EndDate.GetMonth(), m_EndDate.GetDay(),m_EndTm.GetHour(),m_EndTm.GetMinute(), m_EndTm.GetSecond());
		m_soe_history_array.RemoveAll();			
		for(int j=0; j<count; j++)
		{
			obj = m_soe_array.GetAt(j);
			CTime evt_tm = CTime(obj.tmStamp);
			TRACE(evt_tm.Format(L"event : %Y-%m-%d %H:%M:%S\n"));
			if((evt_tm >= myStart) && (evt_tm <= myEnd))
			{
				m_soe_history_array.Add(obj);
			}
		}

	}
	else
	{ 
		m_FindType = 1;
		m_iUps = mListUPS.GetCurSel();
		if(m_iUps < 0)
		{
			MessageBox(_T("请选择UPS名称"), _T("查询提示："), MB_OK);
			return ;
		}
		m_csName = m_ups_array[m_iUps].tlcb.csName;

		
		m_soe_history_array.RemoveAll();			
		for(int j=0; j<count; j++)
		{
			obj = m_soe_array.GetAt(j);
			if(wcscmp(obj.wsName, m_csName.GetString()) == 0)
			{
				m_soe_history_array.Add(obj);
			}
		}
	}
	m_iHistoryFlag = 1;
	UpdateDisplay();
}

void CSoeDlg::OnBnClickedBtnRealtimeWarn()
{
	m_iHistoryFlag = 0;
	UpdateDisplay();
}

void CSoeDlg::UpdateDisplay()
{
	CString cs;
	int i;
	m_SoeList.DeleteAllItems();
	UPS_SOE obj;
	if(m_iHistoryFlag > 0)
	{
		for(i=0; i<m_soe_history_array.GetCount(); i++)
		{
			cs.Format(L"%d", i+1);
			m_SoeList.InsertItem(0xFFFF, cs);
			obj = m_soe_history_array.GetAt(i);
			m_SoeList.SetItemText(i, 0, cs);
			m_SoeList.SetItemText(i, 1, obj.wsIP);
			m_SoeList.SetItemText(i, 2, obj.wsName);
			m_SoeList.SetItemText(i, 3, obj.wsNote);
			m_SoeList.SetItemData(i, obj.dpi);
			CTime ttt(obj.tmStamp);
			m_SoeList.SetItemText(i, 4, ttt.Format(L"%Y-%m-%d %H:%M:%S")); 
		}
	}
	else
	{
		int total = m_soe_array.GetCount();
		for(i=0; i<total; i++)
		{
			cs.Format(L"%d", i+1);
			m_SoeList.InsertItem(0xFFFF, cs);
			obj = m_soe_array.GetAt(total-i-1);
			m_SoeList.SetItemText(i, 0, cs);
			m_SoeList.SetItemText(i, 1, obj.wsIP);
			m_SoeList.SetItemText(i, 2, obj.wsName);
			m_SoeList.SetItemText(i, 3, obj.wsNote);
			m_SoeList.SetItemData(i, obj.dpi);
			CTime ttt(obj.tmStamp);
			m_SoeList.SetItemText(i, 4, ttt.Format(L"%Y-%m-%d %H:%M:%S")); 
		}
	}
	b_Update = FALSE;
}


BOOL CSoeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_SoeList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_SoeList.InsertColumn(0, L"序号", LVCFMT_LEFT, 50, -1);
	m_SoeList.InsertColumn(1, L"IP", LVCFMT_LEFT, 140, -1);
	m_SoeList.InsertColumn(2, L"名称", LVCFMT_LEFT, 150, -1);
	m_SoeList.InsertColumn(3, L"描述", LVCFMT_LEFT, 110, -1);
	m_SoeList.InsertColumn(4, L"时间", LVCFMT_LEFT, 165, -1);
	m_iHistoryFlag = 0;
	UpdateDisplay();
	((CButton *)GetDlgItem(IDC_RADIO_TIME))->SetCheck(TRUE);
	CComboBox* pCmbBox = (CComboBox*)GetDlgItem(IDC_COMBO_UPSNAME);
	int m_upsCnt = m_ups_array.GetCount();
	for (int i=0; i<m_upsCnt; i++)
	{
		CString strTemp = m_ups_array[i].tlcb.csName;

		pCmbBox->InsertString(-1, strTemp);
	}
	UpdateData(false);
	GetDlgItem(IDC_BTN_TEST_SOE)->ShowWindow(SW_HIDE);
	SetTimer(1, 1500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSoeDlg::OnCustomdrawSoeList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMLVCUSTOMDRAW * pDraw=(NMLVCUSTOMDRAW*)pNMCD;
	switch(pDraw->nmcd.dwDrawStage)
	{  
	case CDDS_PREPAINT:  
		{  
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		break;
	case CDDS_ITEMPREPAINT:
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW; 
		}
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			int row, col;
			row = pDraw->nmcd.dwItemSpec;
			col = pDraw->iSubItem;
			pDraw->clrText = RGB(0, 0, 0);
			if(m_iHistoryFlag == 0)
			{				
				if(col == 3)
				{
					DWORD dw = m_SoeList.GetItemData(row);
					if(dw == 0)
					{
						pDraw->clrText = RGB(255, 0, 0);
					}
					else
					{
						pDraw->clrText = RGB(0, 160, 0);
					}
				}				
			}
			else
			{
				pDraw->clrText = RGB(0, 0, 200);
			}
			*pResult = CDRF_DODEFAULT; 
		}  
		break;
	default:
		break;
	}

}



void CSoeDlg::AddSoeRecord(UPS_SOE *pRecord)
{
	m_soe_array.Add(*pRecord);
	wchar_t file_name[_MAX_PATH];
	wcscpy_s(file_name, g_cs_module_path);
	wcscat_s(file_name, SOE_FILE_NAME);
	s_SaveSoeRecordtoFile(file_name);
	m_iHistoryFlag = 0;
}

void CSoeDlg::OnClose()
{
	b_SoeShow = FALSE;
	CDialogEx::OnClose();
}


void CSoeDlg::OnBnClickedBtnTestSoe()
{
	UPS_SOE obj;
	wcscpy_s(obj.wsIP, L"192.168.222.222");
	wcscpy_s(obj.wsName, L"我们在测试...");
	wcscpy_s(obj.wsNote, L"交流失电");
	obj.dpi = 0;
	obj.tmStamp  = time(NULL);
	AddSoeRecord(&obj);
}


void CSoeDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(b_SoeShow && b_Update)
	{
		UpdateDisplay();
	}
	CDialogEx::OnTimer(nIDEvent);
}
