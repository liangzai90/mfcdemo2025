#pragma once


// CLogInDlg 对话框

class CLogInDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogInDlg)

public:
	CLogInDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogInDlg();

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnLogin();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();


	HANDLE m_hMutex2;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
};
