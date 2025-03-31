
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "UPSView.h"
#include "OutputWnd.h"
#include "TcpClient.h"
#include  "SoeDlg.h"

#define WM_USER_SHOWTASK (WM_USER + 1002)




class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	NOTIFYICONDATA m_nid;
	WINDOWPLACEMENT m_wp;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CMFCStatusBar     m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	DECLARE_MESSAGE_MAP()

public:
	BOOL CreateDockingWindows();


public:
	COutputWarnInfoWnd m_wndWarnWnd;
	CUPSView     m_wndUPSView;
	CSoeDlg m_SoePage;
	void SetUPSItemImage(void);
	void MinimizeTip();

	afx_msg LRESULT OnUserNoticeMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnUserShowTask(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSearchWarn();
	afx_msg void OnShowDlg();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSdiTopmost();
	afx_msg void OnStatusbar();
	afx_msg void OnStatusbarData();
	afx_msg void OnStatusbarTime();
	afx_msg void OnStatusbarShsany();
	afx_msg void OnStatusbarNetAddr();
	afx_msg void OnStatusbarSLOGAN();
	afx_msg void OnPassword();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool b_DlgHaveShow;
	LONG dwNewLong1;
	LONG dwNewLong2;

};



