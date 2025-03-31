#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TcpClient.h"
#include "resource.h"


// CSoeDlg 对话框

class CSoeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSoeDlg)

public:
	CSoeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoeDlg();

// 对话框数据
	enum { IDD = IDD_SOE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFindRecord();
	afx_msg void OnBnClickedBtnRealtimeWarn();
	CListCtrl m_SoeList;

	virtual BOOL OnInitDialog();

public:
	void UpdateDisplay();
	void AddSoeRecord(UPS_SOE *pRecord);
	int m_iHistoryFlag;
	CTime m_StartDate, m_EndDate, m_StartTm, m_EndTm;
	CString m_csName;
	int m_FindType, m_iUps;

	CComboBox mListUPS;
	afx_msg void OnCustomdrawSoeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnTestSoe();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
