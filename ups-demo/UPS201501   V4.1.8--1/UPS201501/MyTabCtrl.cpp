// MyTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "UPS201501.h"
#include "MyTabCtrl.h"


IMPLEMENT_DYNAMIC(CMyTabCtrl, CMFCTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CMFCTabCtrl)
	ON_NOTIFY(NM_CUSTOMDRAW, IDS_LISTCTRL_LASTEST_WARN, OnNMCustomdraw)
END_MESSAGE_MAP()

void CMyTabCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
 	*pResult = 0;
	static int i_times = 0;
	i_times++;
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
			if(TRUE)
			{				
				if(col == 3)
				{
					DWORD dw = m_wndLatestWarnInfo.GetItemData(row);
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


/////////////////////////////////////////////////////////////////////////////
// COutputListCtrl

COutputListCtrl::COutputListCtrl()
{
	// 构造函数
}

COutputListCtrl::~COutputListCtrl()
{
	// 析构函数
}

BEGIN_MESSAGE_MAP(COutputListCtrl, CListCtrl)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputListCtrl 消息处理程序
