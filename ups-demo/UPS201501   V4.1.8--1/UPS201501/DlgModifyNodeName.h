#pragma once


// CDlgModifyNodeName �Ի���

class CDlgModifyNodeName : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModifyNodeName)

public:
	CDlgModifyNodeName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModifyNodeName();

// �Ի�������
	enum { IDD = IDD_DLG_MODIFY_NODE_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_str_nodeIP;
	CString m_str_nodename;
	CString m_upsPort;
};
