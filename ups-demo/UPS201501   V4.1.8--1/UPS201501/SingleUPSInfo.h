#pragma once


// CSingleUPSInfo �Ի���
// ��ʾ����UPS����ϸ��Ϣ


class CSingleUPSInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CSingleUPSInfo)

public:
	CSingleUPSInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSingleUPSInfo();

// �Ի�������
	enum { IDD = IDD_SINGLE_UPS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void InitData();
	void ShowData();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
