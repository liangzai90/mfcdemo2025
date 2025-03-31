
#pragma once
 
#include "ViewTree.h"

class CUPSViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


struct list_ups
{
	CString str_ups_name;
	CString str_ups_IP;
	CString str_ups_workmode;  
	CString str_ups_online;    
	CString str_ups_linkstate; 
	CString str_ups_battery_C; 
	CString str_ups_battery_V; 
	CString str_ups_battery_T; 
	int i_UPS_Data;

	// 增加端口号
	CString str_ups_port;
};



class CUPSView : public CDockablePane
{
	// 重载  亮仔定义
public:
public:
	CUPSView();
	void AdjustLayout();
	// 特性
public:
	int n_sample_ps ;
	int m_color_tab[64][256];
	HICON hIcons[10];
	CViewTree m_wndUPSView;
	CImageList m_UPSViewImages;
	CUPSViewToolBar m_wndToolBar;
public:
	void UpdateUPSTreeNodeImage();
	void FillUPSView();
	void ReadPara(int index);
	// 实现
public:
	virtual ~CUPSView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAddNode();
	afx_msg void OnDeleNode();
	afx_msg void OnSetNodeImage();
	afx_msg void OnModifyNodeItemText();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	
	DECLARE_MESSAGE_MAP()

public:
	bool b_ups_link_state;
	void SetUPSState();
	void InitAverageArr(void);
	int GetUPSIndex(void);
	void ArrayToVector(void);
	void VectorToArray(void);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickViewTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawUPSTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedUPSTree(NMHDR *pNMHDR, LRESULT *pResult);

};

