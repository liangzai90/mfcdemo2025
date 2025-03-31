
// UPS201501View.h : CUPS201501View 类的接口
//


#pragma once


#include "UPSGraphic.h"


class CUPS201501View : public CScrollView
{

public:
void UpdateViewHeight(const int upsState , const int upsCnt);

public:
	CBitmap m_bmpBK;
	int m_bmpBKWidth;
	int m_bmpBKHeight;
	CDC m_tempDC;
	HWND m_hWnd;
public:
	CRect my_rt;
	int m_nmx;
	int m_nmy;
	CBitmap *m_pbmp;
	CDC* m_pmdc;
	CUPSGraphic Ups_Graphic;
	CBrush My_brush; 
	CString strTimeInfo[3];


protected: 
	CUPS201501View();
	DECLARE_DYNCREATE(CUPS201501View)

// 特性
public:
	CUPS201501Doc* GetDocument() const;

// 操作
public:

// 重写
public:

	virtual void OnDraw(CDC* pDC); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); 
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CUPS201501View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawtest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
};

#ifndef _DEBUG  // UPS201501View.cpp 中的调试版本
inline CUPS201501Doc* CUPS201501View::GetDocument() const
   { return reinterpret_cast<CUPS201501Doc*>(m_pDocument); }
#endif

