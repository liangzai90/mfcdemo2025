#pragma once
#include "afxcmn.h"


// CProgressDelDlg �Ի���

class CProgressDelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDelDlg)

public:
	CProgressDelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgressDelDlg();

// �Ի�������
	enum { IDD = IDD_PROGRESS_DEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	bool  STOP;

public:
	void OnBtnStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
