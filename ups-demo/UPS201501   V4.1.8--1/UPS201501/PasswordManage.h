#pragma once


// CPasswordManage �Ի���

class CPasswordManage : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordManage)

public:
	CPasswordManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPasswordManage();

// �Ի�������
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
