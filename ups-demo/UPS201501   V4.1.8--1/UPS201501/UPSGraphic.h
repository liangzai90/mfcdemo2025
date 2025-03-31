#include <vector>

#pragma once


struct UpsPoint
{
	CPoint ptName;
	CPoint ptIP;
	CPoint ptWorkMode;
	CPoint ptOnline;
	CPoint ptLinkstate;
	CPoint ptbattery_C;
	CPoint ptbattery_V;
	CPoint ptbattery_T;
};



class CUPSGraphic
{
public:
	CUPSGraphic();
	~CUPSGraphic();

	// 相关操作
public:
	bool DrawUPSLine(CDC* pDC, CBitmap* pbmp,  int upsState);
	bool DrawUPSSwitch(CDC* pDC, CBitmap* pbmp, int upsState);
	bool DrawUPSInverter(CDC* pDC, CBitmap* pbmp, int upsState);
	bool DrawUPSBMS(CDC* pDC, CBitmap* pbmp,  int upsState);
	bool DrawUPSBattery(CDC* pDC, CBitmap* pbmp, int upsState);
	bool DrawUPSText(CDC* pDC, CBitmap* pbmp, int upsState);
	bool DrawUPSCharging(CDC* pDC, CBitmap* pbmp, int upsState);
	bool DrawUPSPowering(CDC* pDC, CBitmap* pbmp, int upsPowLevel);
	bool DrawString(CDC* pDC, CBitmap* pbmp,  HWND m_hWnd);
	bool DrawUPSList(CDC* pDC, CBitmap* pbmp, int upsState);
	bool DrawTimeInfo(CDC* pDC, CBitmap* pbmp, CString strArr[3]);
	bool ClearBmp(CDC* pDC, CBitmap* pbmp, CRect rt, CBrush* pbrush);

public:
	void InitLineData();
	void InitSwitchData();
	void InitInverterData();
	void InitBMSData();
	void InitBatteryData();
	void InitTextData();
	void InitUPSListPoint();
	void InitUPSDC();

	int lineWidth;
	int SwitchWidth;
	int InverterWidth;
	int Battery_EQ_Width;

	// 定义若干 坐标点 
	CPoint pt_line[60];
	CPoint pt_Switch[50];
	CPoint pt_Inverter[50];
	CPoint pt_BMS[100];
	CPoint pt_Battery[50];
	CPoint pt_Text[50];

	CPen pen_Line;
	CBrush brush_NO, brush_OK, brush_ERR;
	CBrush brush_bk;
	int i_move_X, i_move_Y;
	std::vector <UpsPoint> v_pt_upsList;
	UpsPoint  upsList_Title;
};