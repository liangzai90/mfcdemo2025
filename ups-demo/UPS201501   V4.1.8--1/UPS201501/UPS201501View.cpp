
// UPS201501View.cpp : CUPS201501View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "UPS201501.h"
#endif

#include "UPS201501Doc.h"
#include "UPS201501View.h"
#include "UPSView.h"
//#include <vector>

#include "TcpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 
extern UPS_DRAW_SATTE ups_State;
extern int i_whichUPS;
extern std::vector<CString>  v_str_Para;
extern std::vector<list_ups> v_list_ups;
extern int i_UPS_POW_LEVEL;
extern int i_global_ups_Cnt;
extern UINT user_tcp_thread_function(LPVOID lParam);
extern CArray<UPS_NODE_CB, UPS_NODE_CB> m_ups_array;

// CUPS201501View

IMPLEMENT_DYNCREATE(CUPS201501View, CScrollView)

BEGIN_MESSAGE_MAP(CUPS201501View, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_COMMAND(IDM_DRAWTEST, &CUPS201501View::OnDrawtest)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CUPS201501View 构造/析构

CUPS201501View::CUPS201501View()
{
	m_pmdc = new CDC;
	m_pbmp = new CBitmap;
}

CUPS201501View::~CUPS201501View()
{
	delete m_pmdc;
	delete m_pbmp;
}

BOOL CUPS201501View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CUPS201501View 绘制

void CUPS201501View::OnDraw(CDC* pDC)
{
	CUPS201501Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
   	CBitmap* poldbmp=m_pmdc->SelectObject(m_pbmp);
   	pDC->BitBlt(0, 0, m_nmx,m_nmy,m_pmdc,0,0,SRCCOPY);
   	pDC->SelectObject(poldbmp);

}

void CUPS201501View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	sizeTotal.cx = 800;
	sizeTotal.cy = 490;
	SetScrollSizes(MM_TEXT, sizeTotal);
	for(int j=0; j<m_ups_array.GetCount(); j++)
	{
		m_ups_array[j].run_flag = 1;
		AfxBeginThread(user_tcp_thread_function, (LPVOID)&m_ups_array[j]);
	}

}

void CUPS201501View::UpdateViewHeight(const int upsState , const int upsCnt)
{
	CSize sizeTotal;
	sizeTotal.cx = 800;
	sizeTotal.cy = 490;
	int tempH = 7;

	if(DRAW__ROOT_ROOT == upsState)
	{
		if(upsCnt <=16)
		{
			sizeTotal.cy = 490;
		}
		else if(upsCnt>=16 && upsCnt<20)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 4;
		}
		else if(upsCnt>=20 && upsCnt<24)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 4;
		}
		else if(upsCnt>=24 && upsCnt<28)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 4;
		}
		else if(upsCnt>=28 && upsCnt<32)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 4;
		}
		else if(upsCnt>=32 && upsCnt<36)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 4;
		}
		else if(upsCnt>=36 && upsCnt<40)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 4;
		}
		else if(upsCnt>=40 && upsCnt<50)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 10;
		}
		else if(upsCnt>=50 && upsCnt<60)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 10;
		}
		else if(upsCnt>=60 && upsCnt<70)
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 10;
		}
		else 
		{
			sizeTotal.cy = 500 + (upsCnt-17) * tempH * 10;
		}

	}

	SetScrollSizes(MM_TEXT, sizeTotal);
};


// CUPS201501View 打印

BOOL CUPS201501View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CUPS201501View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CUPS201501View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CUPS201501View 诊断

#ifdef _DEBUG
void CUPS201501View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CUPS201501View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CUPS201501Doc* CUPS201501View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUPS201501Doc)));
	return (CUPS201501Doc*)m_pDocument;
}
#endif //_DEBUG


// CUPS201501View 消息处理程序




int CUPS201501View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_hWnd = this->GetSafeHwnd();
	m_bmpBK.LoadBitmap(IDB_SHSANY_LOGO);
	BITMAP  BM;
	m_bmpBK.GetBitmap(&BM);
	m_bmpBKWidth = BM.bmWidth;
	m_bmpBKHeight = BM.bmHeight;
	m_tempDC.CreateCompatibleDC(NULL);
	m_tempDC.SelectObject(m_bmpBK);
	m_nmx=GetSystemMetrics(SM_CXSCREEN);
	m_nmy=GetSystemMetrics(SM_CXSCREEN);

	my_rt.left = -1;
	my_rt.right = m_nmx;
	my_rt.top = -1;
	my_rt.bottom = m_nmy;
	// 就在这里初始化背景画刷
	// GRAY_BRUSH   WHITE_BRUSH    LTGRAY_BRUSH
	My_brush.CreateStockObject(LTGRAY_BRUSH);

	CDC* pDC=GetDC();
	m_pmdc->CreateCompatibleDC(pDC);
	m_pbmp->CreateCompatibleBitmap(pDC,m_nmx,m_nmy);
	CBitmap* pOldbitmap=m_pmdc->SelectObject(m_pbmp);
	CBrush brush; // GRAY_BRUSH        WHITE_BRUSH    LTGRAY_BRUSH
	brush.CreateStockObject(LTGRAY_BRUSH);
	CRect rect(-1,-1,m_nmx,m_nmy);
	m_pmdc->FillRect(rect,&brush);
	m_pmdc->SelectObject(pOldbitmap);	
	ReleaseDC(pDC);
	SetTimer(1, 1000, NULL);
	SetTimer(2, 2000, NULL);

	return 0;
}


void CUPS201501View::OnDrawtest()
{
	// TODO: 在此添加命令处理程序代码
}


void CUPS201501View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认
	int ups_Cnt = 0;
	CTime t=CTime::GetCurrentTime();
	int i_Y = t.GetYear();   // =t.Format("%Y-%m-%d");
	int i_M = t.GetMonth();
	int i_D = t.GetDay();
	// 日期
	strTimeInfo[0] = _T("");
	strTimeInfo[0].Format(_T("日期：%d年%d月%d日"), i_Y, i_M, i_D);
	// 星期
	strTimeInfo[1] = _T("");
	int i = t.GetDayOfWeek();
	switch(i)
	{
	case 2:
		strTimeInfo[1] = _T("星期一");
		break;
	case 3:
		strTimeInfo[1] = _T("星期二");
		break;
	case 4:
		strTimeInfo[1] = _T("星期三");
		break;
	case 5:
		strTimeInfo[1] = _T("星期四");
		break;
	case 6:
		strTimeInfo[1] = _T("星期五");
		break;
	case 7:
		strTimeInfo[1] = _T("星期六");
		break;
	case 1:
		strTimeInfo[1] = _T("星期日");
		break;
	default:
		MessageBox(_T("获取日期异常！"), _T("caption"), MB_OK);
	}
	strTimeInfo[1] = _T("今天是：") + strTimeInfo[1];
	strTimeInfo[2] = t.Format(_T("%H:%M:%S"));
	strTimeInfo[2] = _T("当前时间:") + strTimeInfo[2];
	switch(nIDEvent)
	{
	case 1:
		{
			Ups_Graphic.ClearBmp(m_pmdc, m_pbmp, my_rt, &My_brush);
			switch(ups_State)
			{
			case DRAW__NODE_OK_OK_127:
				Ups_Graphic.DrawUPSLine(m_pmdc, m_pbmp, 1);
				Ups_Graphic.DrawUPSSwitch(m_pmdc, m_pbmp, 1);
				Ups_Graphic.DrawUPSInverter(m_pmdc, m_pbmp, 1);
				Ups_Graphic.DrawUPSBMS(m_pmdc, m_pbmp, 1);
				Ups_Graphic.DrawUPSBattery(m_pmdc, m_pbmp, 1);
				Ups_Graphic.DrawUPSText(m_pmdc, m_pbmp, 1);
				Ups_Graphic.DrawUPSCharging(m_pmdc, m_pbmp, 1);
				break;

			case DRAW__NODE_OK_OK_0:
				Ups_Graphic.DrawUPSLine(m_pmdc, m_pbmp, 2);
				Ups_Graphic.DrawUPSSwitch(m_pmdc, m_pbmp, 2);
				Ups_Graphic.DrawUPSInverter(m_pmdc, m_pbmp, 2);
				Ups_Graphic.DrawUPSBMS(m_pmdc, m_pbmp, 2);
				Ups_Graphic.DrawUPSBattery(m_pmdc, m_pbmp, 2);
				Ups_Graphic.DrawUPSText(m_pmdc, m_pbmp, 2);
				Ups_Graphic.DrawUPSPowering(m_pmdc, m_pbmp, i_UPS_POW_LEVEL);
				break;

			case DRAW__NODE_NO_NO:
				Ups_Graphic.DrawUPSLine(m_pmdc, m_pbmp, 5);
				Ups_Graphic.DrawUPSSwitch(m_pmdc, m_pbmp, 5);
				Ups_Graphic.DrawUPSInverter(m_pmdc, m_pbmp, 5);
				Ups_Graphic.DrawUPSBMS(m_pmdc, m_pbmp, 5);
				Ups_Graphic.DrawUPSBattery(m_pmdc, m_pbmp, 5);
				Ups_Graphic.DrawUPSText(m_pmdc, m_pbmp, 5);
				break;

			case DRAW__ROOT_NULL:
				Ups_Graphic.DrawString(m_pmdc, m_pbmp, m_hWnd);
				Ups_Graphic.DrawTimeInfo(m_pmdc, m_pbmp, strTimeInfo);
				m_pmdc->BitBlt(10, 60, m_bmpBKWidth, m_bmpBKHeight, &m_tempDC, 0, 0, SRCAND);
				break;

			case DRAW__ROOT_ROOT:
				Ups_Graphic.DrawUPSList(m_pmdc, m_pbmp, 2);
				break;

			case DRAW_NULL_NULL:
				break;
			case DRAW__NODE_OK_NO:
				break;
				
			default:
				//TRACE("\r\n-------------UPS201501View.cpp-----OnTimer里面有很多错误");
				break;
			} 

			Invalidate();
		}
		break;


	case 2:
		break;
	}


	CScrollView::OnTimer(nIDEvent);
}


void CUPS201501View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect RtBattery(256,300,314,395);
	CScrollView::OnLButtonDown(nFlags, point);
}


BOOL CUPS201501View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}


BOOL CUPS201501View::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: 在此添加专用代码和/或调用基类


	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}
