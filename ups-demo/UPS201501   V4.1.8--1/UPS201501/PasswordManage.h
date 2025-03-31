#pragma once


// CPasswordManage 对话框

class CPasswordManage : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordManage)

public:
	CPasswordManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPasswordManage();

// 对话框数据
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
