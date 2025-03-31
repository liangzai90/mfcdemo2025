#pragma once


// CSingleUPSInfo 对话框
// 显示单个UPS的详细信息


class CSingleUPSInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSingleUPSInfo)

public:
	CSingleUPSInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSingleUPSInfo();

// 对话框数据
	enum { IDD = IDD_SINGLE_UPS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void InitData();
	void ShowData();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
