#pragma once
#include "afxcmn.h"


// CProgressDelDlg 对话框

class CProgressDelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDelDlg)

public:
	CProgressDelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgressDelDlg();

// 对话框数据
	enum { IDD = IDD_PROGRESS_DEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	bool  STOP;

public:
	void OnBtnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
