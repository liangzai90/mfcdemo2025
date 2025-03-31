#pragma once


// CDlgModifyNodeName 对话框

class CDlgModifyNodeName : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModifyNodeName)

public:
	CDlgModifyNodeName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModifyNodeName();

// 对话框数据
	enum { IDD = IDD_DLG_MODIFY_NODE_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_str_nodeIP;
	CString m_str_nodename;
	CString m_upsPort;
};
