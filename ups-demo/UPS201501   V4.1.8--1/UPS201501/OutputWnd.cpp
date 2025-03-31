
// 何亮，时间：2015年1月2日23:46:21
// 地点：宿舍
// 


#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#define WARNLIST_COUNT  30

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool b_warnListUpdate = FALSE;
extern CArray<UPS_SOE, UPS_SOE> m_soe_array;
extern wchar_t g_cs_module_path[_MAX_PATH];
extern const wchar_t SOE_FILE_NAME[] = L"\\shsyrecords.csv";
extern int s_ReadSoeRecordsFromFile(const wchar_t *strFile);
extern BOOL b_Update;

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWarnInfoWnd::COutputWarnInfoWnd()
{
	// 构造函数
}

COutputWarnInfoWnd::~COutputWarnInfoWnd()
{
	// 析构函数
}

BEGIN_MESSAGE_MAP(COutputWarnInfoWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int COutputWarnInfoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	wchar_t file_name[_MAX_PATH];
	wcscpy_s(file_name, g_cs_module_path);
	wcscat_s(file_name, SOE_FILE_NAME);
	s_ReadSoeRecordsFromFile(file_name);

	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

 	// 创建选项卡窗口:   .... 就是在此处，将Tab窗口和COutputwnd关联起来的
   	if (! (m_wndTabs.Create(CMFCTabCtrl::STYLE_3D_ROUNDED, rectDummy, this, 10)) )
   	{
   		TRACE0("未能创建输出选项卡窗口\n");
   		MessageBox(_T("未能创建输出选项卡窗口"), _T("OutputWnd.cpp"));
   		return -1;      // 未能创建
   	}

	// 创建输出窗格:
	const DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_BORDER| LVS_ALIGNLEFT |
		LVS_REPORT | LVS_SINGLESEL | WS_HSCROLL | WS_VSCROLL;
 	if(!m_wndTabs.m_wndLatestWarnInfo.Create(dwStyle, rectDummy, &m_wndTabs, IDS_LISTCTRL_LASTEST_WARN) ||
 		!m_wndRealtimeInfo.Create(dwStyle,rectDummy, &m_wndTabs, 12) ||
 		!m_wndHistoryInfo.Create(dwStyle,rectDummy, &m_wndTabs,13))
 	{
 		TRACE0("未能创建输出窗口\n");
 		MessageBox(_T("创建 CListCtrl 控件错误！！！！"), _T("Warn Info List Err"));
 		return -1;      // 未能创建
 	}

	m_wndTabs.m_wndLatestWarnInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndRealtimeInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndHistoryInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	UpdateFonts();
	CString strTabName;
	BOOL bNameValid;
  	bNameValid = strTabName.LoadString(IDS_LATESTWARNINFO_TAB);
  	ASSERT(bNameValid);
  	m_wndTabs.AddTab(&(m_wndTabs.m_wndLatestWarnInfo), strTabName, (UINT)0);
 	m_wndTabs.SetLocation(CMFCBaseTabCtrl::LOCATION_TOP);
 	m_wndTabs.EnableTabSwap(TRUE);
 	m_wndTabs.HideSingleTab(TRUE);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 0, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(2, _T("名称"), LVCFMT_LEFT, 190, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(3, _T("描述"), LVCFMT_LEFT, 160, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(4, _T("时间"), LVCFMT_LEFT, 300, -1);


	// 字体更改 
	CFont *cFont = new CFont;  
	cFont->CreateFont(16,0,0,0,FW_NORMAL,FALSE,FALSE,0,GB2312_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("宋体")); 
	m_wndTabs.m_wndLatestWarnInfo.SetFont(cFont,true);


	UpdateWarnList();
	SetTimer(1, 2000, NULL);
	return 0;
}

void COutputWarnInfoWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWarnInfoWnd::AdjustHorzScroll(CListCtrl& wndListCtrl)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);
	int cxExtentMax = 0;
	for (int i = 0; i < wndListCtrl.GetItemCount(); i ++)
	{
		for(int j=0; j<5; j++)
		{
			CString strItem;
			strItem = wndListCtrl.GetItemText(i, j);
			cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
			wndListCtrl.SetColumnWidth(j, cxExtentMax);
		}
	}
	dc.SelectObject(pOldFont);
}

void COutputWarnInfoWnd::FillLatestWarnInfoWindow()
{
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 0, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(2, _T("名称"), LVCFMT_LEFT, 175, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(3, _T("描述"), LVCFMT_LEFT, 160, -1);
	m_wndTabs.m_wndLatestWarnInfo.InsertColumn(4, _T("时间"), LVCFMT_LEFT, 300, -1);
	CString str[5][4];
	str[0][0] = "192.168.190.101";
	str[1][0] = "192.168.190.101";
	str[2][0] = "192.168.190.102";
	str[3][0] = "192.168.190.103";
	str[4][0] = "192.168.190.104";
	str[0][1] = "三盘区配电点UPS";
	str[1][1] = "三盘区配电点UPS";
	str[2][1] = "石门机头配电点UPS";
	str[3][1] = "二盘区配电点UPS";
	str[4][1] = "南翼绕道配电点UPS";
	str[0][2] = "通信中断恢复";   
	str[1][2] = "通信中断";
	str[2][2] = "交流失电";
	str[3][2] = "交流恢复";  
	str[4][2] = "通信中断";

	str[0][3] = "2014年11月18日  19:50:57";
	str[1][3] = "2014年11月18日  19:48:03";
	str[2][3] = "2014年11月18日  18:50:24";
	str[3][3] = "2014年11月18日  18:44:50";
	str[4][3] = "2014年11月18日  18:50:34";

	for(int i=0; i<5; i++)
	{
		CString str_temp = _T("");
		str_temp.Format(_T("%d"), i+1);
		m_wndTabs.m_wndLatestWarnInfo.InsertItem(i, str_temp);
		for(int j=0; j<4; j++)
		{
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(i,j+1,str[i][j]);
		}//for
	} //for
}



void COutputWarnInfoWnd::FillRealtimeInfoWindow()
{
	m_wndRealtimeInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50, -1);
	m_wndRealtimeInfo.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 130, -1);
	m_wndRealtimeInfo.InsertColumn(2, _T("名称"), LVCFMT_LEFT, 175, -1);
	m_wndRealtimeInfo.InsertColumn(3, _T("描述"), LVCFMT_LEFT, 120, -1);
	m_wndRealtimeInfo.InsertColumn(4, _T("时间"), LVCFMT_LEFT, 250, -1);
	// 1. IP
	CString str[5][4];
	str[0][0] = "192.168.190.101";
	str[1][0] = "192.168.190.101";
	str[2][0] = "192.168.190.102";
	str[3][0] = "192.168.190.103";
	str[4][0] = "192.168.190.104";

	// 2. 名称
	str[0][1] = "三盘区配电点UPS22";
	str[1][1] = "三盘区配电点UPS22";
	str[2][1] = "石门机头配电点UPS22";
	str[3][1] = "二盘区配电点UPS22";
	str[4][1] = "南翼绕道配电点UPS22";

	// 3. 描述
	str[0][2] = "通信中断恢复";   // OK
	str[1][2] = "通信中断";
	str[2][2] = "交流失电";
	str[3][2] = "交流恢复";   // OK
	str[4][2] = "通信中断";

	// 4. 时间
	str[0][3] = "2014年11月18日  19:50:57";
	str[1][3] = "2014年11月18日  19:48:03";
	str[2][3] = "2014年11月18日  18:50:24";
	str[3][3] = "2014年11月18日  18:44:50";
	str[4][3] = "2014年11月18日  18:50:34";

	for(int i=0; i<5; i++)
	{
		CString str_temp = _T("");
		str_temp.Format(_T("%d"), i+1);

		m_wndRealtimeInfo.InsertItem(i, str_temp);

		for(int j=0; j<4; j++)
		{
			m_wndRealtimeInfo.SetItemText(i,j+1,str[i][j]);
		}//for
	} //for

}



void COutputWarnInfoWnd::FillHistoryInfoWindow()
{
	m_wndHistoryInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50, -1);
	m_wndHistoryInfo.InsertColumn(1, _T("IP"), LVCFMT_LEFT, 130, -1);
	m_wndHistoryInfo.InsertColumn(2, _T("名称"), LVCFMT_LEFT, 175, -1);
	m_wndHistoryInfo.InsertColumn(3, _T("描述"), LVCFMT_LEFT, 120, -1);
	m_wndHistoryInfo.InsertColumn(4, _T("时间"), LVCFMT_LEFT, 250, -1);
	// 1. IP
	CString str[5][4];
	str[0][0] = "192.168.190.101";
	str[1][0] = "192.168.190.101";
	str[2][0] = "192.168.190.102";
	str[3][0] = "192.168.190.103";
	str[4][0] = "192.168.190.104";

	// 2. 名称
	str[0][1] = "三盘区配电点UPS333";
	str[1][1] = "三盘区配电点UPS333";
	str[2][1] = "石门机头配电点UPS333";
	str[3][1] = "二盘区配电点UPS333";
	str[4][1] = "南翼绕道配电点UPS333";

	// 3. 描述
	str[0][2] = "通信中断恢复";   // OK
	str[1][2] = "通信中断";
	str[2][2] = "交流失电";
	str[3][2] = "交流恢复";   // OK
	str[4][2] = "通信中断";

	// 4. 时间
	str[0][3] = "2014年11月18日  19:50:57";
	str[1][3] = "2014年11月18日  19:48:03";
	str[2][3] = "2014年11月18日  18:50:24";
	str[3][3] = "2014年11月18日  18:44:50";
	str[4][3] = "2014年11月18日  18:50:34";

	for(int i=0; i<5; i++)
	{
		CString str_temp = _T("");
		str_temp.Format(_T("%d"), i+1);

		m_wndHistoryInfo.InsertItem(i, str_temp);

		for(int j=0; j<4; j++)
		{
			m_wndHistoryInfo.SetItemText(i,j+1,str[i][j]);
		}//for
	} //for

}




void COutputWarnInfoWnd::UpdateFonts()
{
	m_wndTabs.m_wndLatestWarnInfo.SetFont(&afxGlobalData.fontRegular);
	m_wndRealtimeInfo.SetFont(&afxGlobalData.fontRegular);
	m_wndHistoryInfo.SetFont(&afxGlobalData.fontRegular);
}


void COutputWarnInfoWnd::UpdateWarnList(void)
{
	m_wndTabs.m_wndLatestWarnInfo.DeleteAllItems();
	int i_WarnCnt = m_soe_array.GetCount();
	if(i_WarnCnt<1)
		return ;
	if( i_WarnCnt < WARNLIST_COUNT)
	{
		for (int j=i_WarnCnt-1, k=0; j>=0; j--, k++)
		{
			CString cs = _T("");
			cs.Format(_T("%d"), k+1);
			m_wndTabs.m_wndLatestWarnInfo.InsertItem(0xFFFF, cs);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 0, cs);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 1, m_soe_array[j].wsIP);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 2, m_soe_array[j].wsName);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 3, m_soe_array[j].wsNote);
			m_wndTabs.m_wndLatestWarnInfo.SetItemData(k, m_soe_array[j].dpi);
			CTime t_time(m_soe_array[j].tmStamp);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 4, t_time.Format(L"%Y-%m-%d %H:%M:%S"));
		}
	}
	else
	{
		int temp = i_WarnCnt-1;
		for(int k=0; k<WARNLIST_COUNT; k++)
		{
			CString cs = _T("");
			cs.Format(_T("%d"), k+1);
			m_wndTabs.m_wndLatestWarnInfo.InsertItem(0xFFFF, cs);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 0, cs);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 1, m_soe_array[temp].wsIP);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 2, m_soe_array[temp].wsName);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 3, m_soe_array[temp].wsNote);
			m_wndTabs.m_wndLatestWarnInfo.SetItemData(k, m_soe_array[temp].dpi);
			CTime t_time(m_soe_array[temp].tmStamp);
			m_wndTabs.m_wndLatestWarnInfo.SetItemText(k, 4, t_time.Format(L"%Y-%m-%d %H:%M:%S"));
			temp--;
		}
	}

	b_warnListUpdate = FALSE; 
}




// 定义OnTimer 

void COutputWarnInfoWnd::OnTimer(UINT_PTR nIDEvent)
{
	if(b_warnListUpdate)
		UpdateWarnList();
}

