
// UPS201501.h : UPS201501 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


class CUPS201501App : public CWinAppEx
{
public:
	HANDLE m_hMutex;
	CUPS201501App();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	UINT  m_nAppLook;
};

extern CUPS201501App theApp;
