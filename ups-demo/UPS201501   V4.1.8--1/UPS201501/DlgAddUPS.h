#pragma once


// CDlgAddUPS 对话框

class CDlgAddUPS : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddUPS)

public:
	CDlgAddUPS(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddUPS();

// 对话框数据
	enum { IDD = IDD_DLG_NEWUPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_str_upsName;


	unsigned char cip0, cip1, cip2, cip3;
	unsigned int m_nPort, m_nPowerIndex, m_nVoltageIndex, m_nVolumeIndex;
	float m_fRsa;
	int m_TCPport;


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
