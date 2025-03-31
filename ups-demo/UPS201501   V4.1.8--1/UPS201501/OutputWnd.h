
// 面向对象 。。

#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口
#include "MyTabCtrl.h"




class COutputWarnInfoWnd : public CDockablePane
{
	// 构造
public:
	COutputWarnInfoWnd();
	void UpdateFonts();

	// 特性
public:
	CMyTabCtrl m_wndTabs;
	COutputListCtrl m_wndRealtimeInfo;
	COutputListCtrl m_wndHistoryInfo;

protected:
	void FillLatestWarnInfoWindow();
	void FillRealtimeInfoWindow();
	void FillHistoryInfoWindow();
	void UpdateWarnList();
	void AdjustHorzScroll(CListCtrl& wndListCtrl);

	// 实现
public:
	virtual ~COutputWarnInfoWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	// 
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
