#pragma once


// �������̳� ��
class COutputListCtrl : public CListCtrl
{
	// ����
public:
	COutputListCtrl();

	// ʵ��
public:
	virtual ~COutputListCtrl();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()


public:

};

/////////////////////////////////////////////////////////////////////////////////////


class CMyTabCtrl : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);


 public:
	 COutputListCtrl m_wndLatestWarnInfo;   
};


