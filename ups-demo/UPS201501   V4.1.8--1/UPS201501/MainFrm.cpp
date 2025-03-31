
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "resource.h"
#include "UPS201501.h"

#include "MainFrm.h"
#include "PasswordManage.h"
#include "LogInDlg.h"


#include "windows.h"
#include "windef.h"
#include "winbase.h"
#include "shlwapi.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL b_Update;
extern BOOL b_SoeShow;
extern bool b_warnListUpdate;
wchar_t g_cs_module_path[_MAX_PATH];
UPS_DRAW_SATTE ups_State;
int i_global_ups_Cnt = 0;






#define PACKVERSION(major,minor) MAKELONG(minor,major)
DWORD GetVersion(LPCTSTR lpszDllName)
{
    HINSTANCE hinstDll;
    DWORD dwVersion = 0;

    /* For security purposes, LoadLibrary should be provided with a fully qualified 
       path to the DLL. The lpszDllName variable should be tested to ensure that it 
       is a fully qualified path before it is used. */
    hinstDll = LoadLibrary(lpszDllName);
	
    if(hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion;
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

        /* Because some DLLs might not implement this function, you must test for 
           it explicitly. Depending on the particular DLL, the lack of a DllGetVersion 
           function can be a useful indicator of the version. */

        if(pDllGetVersion)
        {
            DLLVERSIONINFO dvi;
            HRESULT hr;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);

            hr = (*pDllGetVersion)(&dvi);

            if(SUCCEEDED(hr))
            {
               dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }
        FreeLibrary(hinstDll);
    }
    return dwVersion;
}







IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_NOTICE_MSG, OnUserNoticeMsg)
	ON_MESSAGE(WM_USER_SHOWTASK, OnUserShowTask)
	ON_COMMAND(IDM_SEARCH_WARN, &CMainFrame::OnSearchWarn)
	ON_COMMAND(IDM_SHOW_DLG, &CMainFrame::OnShowDlg)
	ON_COMMAND(WM_DESTROY1, &CMainFrame::OnDestroy)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_SDI_TOPMOST, &CMainFrame::OnSdiTopmost)
	ON_COMMAND(IDM_STATUSBAR, &CMainFrame::OnStatusbar)
	ON_COMMAND(IDS_DATE, &CMainFrame::OnStatusbarData)
	ON_COMMAND(IDS_TIME, &CMainFrame::OnStatusbarTime)
	ON_COMMAND(IDS_SHSANY, &CMainFrame::OnStatusbarShsany)
	ON_COMMAND(IDS_OURNETADD, &CMainFrame::OnStatusbarNetAddr)
	ON_COMMAND(IDS_SLOGAN, &CMainFrame::OnStatusbarSLOGAN)
	ON_COMMAND(IDM_PASSWORD, &CMainFrame::OnPassword)


END_MESSAGE_MAP()

static UINT indicators[] =
{
	IDS_DATE,
	IDS_TIME,
	IDS_SHSANY,
	IDS_OURNETADD,
	IDS_SLOGAN
};


CMainFrame*  pMyMainFrm;

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = 208;   
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	// ״̬��
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;   
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	OnStatusbar();
	OnApplicationLook(theApp.m_nAppLook);
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("\r\n\r\n δ�ܴ���ͣ������ ");
		MessageBox(_T("δ�ܴ���ͣ������ "), _T("CreateDockingWindows  err"));
		return -1;
	}
	m_wndUPSView.SetMinSize(CSize(150,200));
	m_wndWarnWnd.SetMinSize(CSize(200,120));
 	m_wndUPSView.EnableDocking(CBRS_ALIGN_LEFT);
 	EnableDocking(CBRS_ALIGN_LEFT);
 	DockPane(&m_wndUPSView);
  	m_wndWarnWnd.EnableDocking(CBRS_ALIGN_BOTTOM);
  	EnableDocking(CBRS_ALIGN_BOTTOM);
  	DockPane(&m_wndWarnWnd);
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA); 
	m_nid.hWnd = this-> m_hWnd; 
	m_nid.uID = IDR_MAINFRAME; 
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; 
	m_nid.uCallbackMessage = WM_USER_SHOWTASK;//�Զ������Ϣ���� 
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)); 
	wcscpy_s(m_nid.szTip, L"UPS����ϵͳ-ɽԴ�Ƽ�");//��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
	Shell_NotifyIcon(NIM_ADD, &m_nid);//�����������ͼ�� 
	SetTimer(1, 800, NULL);
	b_DlgHaveShow = TRUE;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
	{
		return FALSE;
	}
	cs.style=WS_OVERLAPPEDWINDOW;
	dwNewLong1 = cs.style;
	dwNewLong2 = cs.dwExStyle;
	cs.lpszName = _T("�Ϻ�ɽԴUPS���߼��ϵͳ  ���ڲ����԰桿  V4.1.7");
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������


// ����ͣ������
BOOL CMainFrame::CreateDockingWindows()
{
	ups_State = DRAW__ROOT_NULL;
	GetModuleFileName(NULL, g_cs_module_path, _MAX_PATH);
	PathRemoveFileSpec(g_cs_module_path);
	BOOL bNameValid;
	CString strUPSView;
	bNameValid = strUPSView.LoadString(IDS_UPS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndUPSView.Create(strUPSView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_UPSVIEW,WS_CHILD | WS_VISIBLE  | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT  | CBRS_HIDE_INPLACE | WS_CAPTION , AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE ))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		MessageBox(_T("δ�ܴ��� USP_VIEW �������"), _T(""));
		return FALSE; // δ�ܴ���
	}
	CString strOutputWarnInfoWnd;
	bNameValid = strOutputWarnInfoWnd.LoadString(IDS_OUTPUT_WND);
	if (!m_wndWarnWnd.Create(strOutputWarnInfoWnd, this, CRect(0, 0, 240, 400), TRUE, ID_VIEW_OUTPUTWND,  WS_CHILD|WS_VISIBLE|WS_BORDER| LVS_ALIGNLEFT | LVS_REPORT | LVS_SINGLESEL | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE ))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	return TRUE;
}

// Ӧ�ó���������ú���
void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;
	theApp.m_nAppLook = 208;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}



void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(lpMMI->ptMinTrackSize.x <= 600)
		lpMMI->ptMinTrackSize.x = 600;
	if(lpMMI->ptMinTrackSize.y <= 400)
		lpMMI->ptMinTrackSize.y = 400;
	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	OnStatusbar();
	static int index = 1;
	++ index;
	index = index % 2 ;

	switch(nIDEvent)
	{
	case 1:
		if(1 == index)
		{
			SetUPSItemImage();
		}
		else
		{
			SetUPSItemImage();
		}
		break;

	case 2:
		break;

	case 3:
		break;
	default:
		TRACE("\r\n\r\n  �д����ˣ��ֵܣ�  2015��1��4��20:01:11 \r\n");
	}

	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::SetUPSItemImage(void)
{
	HTREEITEM hRoot_temp = m_wndUPSView.m_wndUPSView.GetRootItem();
	int i_count = m_wndUPSView.m_wndUPSView.GetCount();
	if(i_count <= 1)
		return ;
	HTREEITEM hChild_temp = m_wndUPSView.m_wndUPSView.GetChildItem(hRoot_temp);
	for (int i=2; i<=i_count; i++)
	{
		if(hChild_temp == NULL)
			break;
		int i_temp = m_wndUPSView.m_wndUPSView.GetItemData(hChild_temp);
		switch(i_temp)
		{
		case 3:
			m_wndUPSView.m_wndUPSView.SetItemImage(hChild_temp, 6, 6);
			m_wndUPSView.m_wndUPSView.SetItemData(hChild_temp, 6);
			break;
		case 6:
			m_wndUPSView.m_wndUPSView.SetItemImage(hChild_temp, 3, 3);
			m_wndUPSView.m_wndUPSView.SetItemData(hChild_temp, 3);
			break;
		case 4:
			m_wndUPSView.m_wndUPSView.SetItemImage(hChild_temp, 7, 7);
			m_wndUPSView.m_wndUPSView.SetItemData(hChild_temp, 7);
			break;
		case 7:
			m_wndUPSView.m_wndUPSView.SetItemImage(hChild_temp, 4, 4);
			m_wndUPSView.m_wndUPSView.SetItemData(hChild_temp, 4);
			break;
		case 1:
			m_wndUPSView.m_wndUPSView.SetItemImage(hChild_temp, 1, 1);
			break;
		case 2:
			m_wndUPSView.m_wndUPSView.SetItemImage(hChild_temp, 2, 2);
			break;
		case 5:
			m_wndUPSView.m_wndUPSView.SetItemImage(hChild_temp, 5, 5);
			break;
		case 0:
			break;

		default:
			TRACE("\r\n\r\n ����  case5 ��Ҫ���ǽ�����    �������ô��� *_* *_* *_* ");

		}

		hChild_temp = m_wndUPSView.m_wndUPSView.GetNextItem(hChild_temp, TVGN_NEXT);
	}


}


LRESULT CMainFrame::OnUserNoticeMsg(WPARAM wParam, LPARAM lParam)
{
	UPS_NODE_CB *pcb = (UPS_NODE_CB*)wParam;
	UPS_SOE obj;
	DWORD soe_code = lParam >> 8;
	CString cs = _T("");
	obj.tmStamp  = time(NULL);
	wcscpy_s(obj.wsName, pcb->tlcb.csName.GetString());
	size_t nCnt = 0;
	mbstowcs_s(&nCnt, obj.wsIP, pcb->tlcb.strIp, 30);
	obj.dpi = lParam & 0x01;
	switch(soe_code)
	{
	case UPS_SOE_CODE_TCP_LINK:
		if(obj.dpi > 0)
		{
			wcscpy_s(obj.wsNote, L"�����жϻָ�");
		}
		else
		{
			wcscpy_s(obj.wsNote, L"�����ж�");
		}
		break;


	case UPS_SOE_CODE_ONLINE:
		if(obj.dpi > 0)
		{
			wcscpy_s(obj.wsNote, L"ͨ���жϻָ�");
			obj.tmStamp  = time(NULL);
			m_SoePage.AddSoeRecord(&obj);
			b_Update = TRUE;
			b_warnListUpdate = TRUE;
		}
		else
		{
			wcscpy_s(obj.wsNote, L"ͨ���ж�");
			obj.tmStamp  = time(NULL);
			m_SoePage.AddSoeRecord(&obj);
			b_Update = TRUE;
			b_warnListUpdate = TRUE;
		}
		break;
	case UPS_SOE_CODE_POWER:
		if(obj.dpi > 0)
		{
			wcscpy_s(obj.wsNote, L"�����ָ�");
		}
		else
		{
			wcscpy_s(obj.wsNote, L"����ʧ��");
		}
		obj.tmStamp  = time(NULL);
		m_SoePage.AddSoeRecord(&obj);
		b_Update = TRUE;
		b_warnListUpdate = TRUE;
		break;
	default:
		break;
	}	
	m_nid.cbSize=sizeof(NOTIFYICONDATA);
	m_nid.uFlags = NIF_INFO;
	m_nid.uVersion = NOTIFYICON_VERSION;
	m_nid.uTimeout = 30000;
	m_nid.dwInfoFlags = NIIF_INFO;
	wcscpy_s(m_nid.szInfoTitle, L"�澯��ʾ");
	cs = obj.wsName;
	cs = L"\r\n" + cs;
	cs += L"                                 ";
	cs += L"  \r\n\r\n�澯���";
	cs += obj.wsNote;
	cs += L"    \r\n\r\n\r\n                  �Ϻ�ɽԴ���ӿƼ��ɷ����޹�˾";
	wcscpy_s(m_nid.szInfo, cs);//��������
	Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	return 0;
}



LRESULT CMainFrame::OnUserShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME) 
		return   1; 
	switch(lParam) 
	{ 
	case WM_RBUTTONUP:
		{ 
			POINT   pt; 
			::GetCursorPos(&pt);
			CMenu   menu; 
			menu.CreatePopupMenu();
			menu.AppendMenu(MF_STRING, IDM_SHOW_DLG, L"  ��ʾ");
			menu.AppendMenu(MF_STRING, WM_DESTROY1, L"  �ر�");   
			menu.TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this); 
			HMENU   hmenu=menu.Detach(); 
			menu.DestroyMenu();
		} 
		break;
	case WM_LBUTTONDBLCLK:
		OnShowDlg();
		break;

	default:
		break;
	} 
	return 0; 
}



void CMainFrame::OnSearchWarn()
{
	b_SoeShow = TRUE;
	CSoeDlg  soeDlg;
	soeDlg.DoModal();
}


void CMainFrame::OnShowDlg()
{
	CLogInDlg lgDlg;

	if(b_DlgHaveShow)
	{
		;
	}
	else if(lgDlg.DoModal() == IDOK)
	{
 		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW); 
		ShowWindow(TRUE);
		b_DlgHaveShow = TRUE;
	}
	else
	{
		return ;
	}


}


void CMainFrame::OnDestroy()
{
	OnClose();
}


void CMainFrame::OnClose()
{
	if(MessageBox(L"�⽫ֹͣUPS���߼��ϵͳ��  ��ȷ���˳���?", L"��ʾ: ѡ���ǡ��˳�, ��������", MB_YESNO) == IDNO)
	{
		ShowWindow(SW_HIDE);
		b_DlgHaveShow = FALSE;
		// MinimizeTip();

	}
	else
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nid);
		CFrameWndEx::OnClose();
	}

}




LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_SYSCOMMAND:
		if(wParam == SC_MINIMIZE)
		{
			ShowWindow(SW_HIDE);
			LONG test_Style = ~WS_VISIBLE;
			SetWindowLong(m_hWnd, GWL_STYLE, test_Style);
			ShowWindow(SW_HIDE);
			b_DlgHaveShow = FALSE;
			// MinimizeTip();

		}
	}

	return CFrameWndEx::DefWindowProc(message, wParam, lParam);
}


void CMainFrame::OnSdiTopmost()
{
	int static i=0;
	i++;
	if(i==1)
	{
		GetMenu()->GetSubMenu(1)->CheckMenuItem(0, MF_CHECKED | MF_BYPOSITION);
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|WS_EX_TOPMOST);
	}
	else
	{
		GetMenu()->GetSubMenu(1)->CheckMenuItem(0, MF_UNCHECKED | MF_BYPOSITION);
		SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|WS_EX_TOPMOST);
	}
	
	i=i%2;
}

void CMainFrame::OnStatusbar()
{
	CTime t=CTime::GetCurrentTime();
	int i_Y = t.GetYear();   // =t.Format("%Y-%m-%d");
	int i_M = t.GetMonth();
	int i_D = t.GetDay();
	CString str_Date = _T("");
	str_Date.Format(_T("  %d��%d��%d��"), i_Y, i_M, i_D);
	CString str_week = _T("");

	int i = t.GetDayOfWeek();
	switch(i)
	{
	case 2:
		str_week = _T("����һ");
		break;
	case 3:
		str_week = _T("���ڶ�");
		break;
	case 4:
		str_week = _T("������");
		break;
	case 5:
		str_week = _T("������");
		break;
	case 6:
		str_week = _T("������");
		break;
	case 7:
		str_week = _T("������");
		break;
	case 1:
		str_week = _T("������");
		break;
	default:
		MessageBox(_T("��ȡ�����쳣��"), _T("caption"), MB_OK);
	}
	str_Date = str_Date + _T(" ") + str_week;
	CString str_time = t.Format(_T("%H:%M:%S"));
	CString str_time_temp = _T("  ʱ��:");
	str_time = str_time_temp + str_time;
	CString str_companyName = _T("  �Ϻ�ɽԴ���ӿƼ��ɷ����޹�˾");
	CString str_netAdd = _T("  www.shsany.com");
	CString str_slogan = _T("  ������ҵ������ƽ̨����߰������ܻ�ˮƽ��");
	CClientDC dc(this);
	CSize sz_Date = dc.GetTextExtent(str_Date);
	CSize sz_Time = dc.GetTextExtent(str_time);
	CSize sz_company_name = dc.GetTextExtent(str_companyName);
	CSize sz_netAdd = dc.GetTextExtent(str_netAdd);
	CSize sz_slogan = dc.GetTextExtent(str_slogan);
	m_wndStatusBar.SetPaneInfo(0, IDS_DATE, SBPS_NORMAL, sz_Date.cx -10);
	m_wndStatusBar.SetPaneInfo(1, IDS_TIME, SBPS_NORMAL, sz_Time.cx -10);
	m_wndStatusBar.SetPaneInfo(2, IDS_SHSANY, SBPS_NORMAL, sz_company_name.cx -10);
	m_wndStatusBar.SetPaneInfo(3, IDS_OURNETADD, SBPS_NORMAL, sz_netAdd.cx -10);
	m_wndStatusBar.SetPaneInfo(4, IDS_SLOGAN, SBPS_STRETCH, sz_slogan.cx -10);
	m_wndStatusBar.SetPaneText(0, str_Date);
	m_wndStatusBar.SetPaneText(1, str_time);
	m_wndStatusBar.SetPaneText(2, str_companyName);
	m_wndStatusBar.SetPaneText(3, str_netAdd);
	m_wndStatusBar.SetPaneText(4, str_slogan);
}

void CMainFrame::OnStatusbarData()
{

}
void CMainFrame::OnStatusbarTime()
{

}
void CMainFrame::OnStatusbarShsany()
{

}
void CMainFrame::OnStatusbarNetAddr()
{

}
void CMainFrame::OnStatusbarSLOGAN()
{

}

void CMainFrame::OnPassword()
{
	CPasswordManage  pwDlg;
	pwDlg.DoModal();
}


void CMainFrame::MinimizeTip()
{
// 	LPCTSTR lpszDllName = L"C://Windows//System32//Shell32.dll";
// 	DWORD dwVer = GetVersion(lpszDllName);
// 	DWORD dwTarget = PACKVERSION(6, 0);
// 	CString cs;
// 
// 	if(dwVer >= dwTarget)
// 	{
// 		m_nid.cbSize=sizeof(NOTIFYICONDATA);
// 		m_nid.uVersion = NOTIFYICON_VERSION;
// 	}
// 	else
// 	{
// 		m_nid.cbSize=sizeof(NOTIFYICONDATA_V3_SIZE);
// 		m_nid.uVersion = NOTIFYICON_VERSION;
// 	}
		CString cs;
		m_nid.cbSize=sizeof(NOTIFYICONDATA);
		m_nid.uFlags = NIF_INFO;
		m_nid.uVersion = NOTIFYICON_VERSION;
		m_nid.uTimeout = 15000;
		m_nid.dwInfoFlags = 0;

//	 	m_nid.dwInfoFlags = NIIF_INFO;
	 	wcscpy_s(m_nid.szInfoTitle, L"��С����ʾ");
	 	cs = _T("����С���� ���Ϻ�ɽԴUPS���߼��ϵͳ��   ");
	 	cs = L"\r\n" + cs;
	 	cs += L"    \r\n\r\n\r\n                �Ϻ�ɽԴ���ӿƼ��ɷ����޹�˾";
	 	wcscpy_s(m_nid.szInfo, cs);
	 	Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}
