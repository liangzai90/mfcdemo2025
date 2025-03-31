
// UPS201501View.h : CUPS201501View ��Ľӿ�
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

// ����
public:
	CUPS201501Doc* GetDocument() const;

// ����
public:

// ��д
public:

	virtual void OnDraw(CDC* pDC); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); 
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CUPS201501View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // UPS201501View.cpp �еĵ��԰汾
inline CUPS201501Doc* CUPS201501View::GetDocument() const
   { return reinterpret_cast<CUPS201501Doc*>(m_pDocument); }
#endif

