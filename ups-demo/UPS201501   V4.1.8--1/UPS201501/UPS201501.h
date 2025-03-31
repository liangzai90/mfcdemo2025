
// UPS201501.h : UPS201501 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


class CUPS201501App : public CWinAppEx
{
public:
	HANDLE m_hMutex;
	CUPS201501App();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	UINT  m_nAppLook;
};

extern CUPS201501App theApp;
