/*
 CUPSGraphic �࣬��������UPS��  ���������ء����������ء�BMS��ذ塢���������Ϣ
 ���ߣ�����
 ʱ�䣺2015��3��10��10:08:42

  */


// ��������stdafx.hͷ�ļ�...
#include <stdafx.h>
#include "UPSGraphic.h"
#include "math.h"
#include "UPSView.h"

extern UPS_DRAW_SATTE ups_State;
extern int i_whichUPS;
extern std::vector<CString>  v_str_Para;
extern std::vector<list_ups> v_list_ups;
extern CString str_Root_Name;



CUPSGraphic::CUPSGraphic()
{
	InitUPSDC();
	InitLineData();
	InitSwitchData();
	InitInverterData();
	InitBMSData();
	InitBatteryData();
	InitTextData();
	InitUPSListPoint();
}


CUPSGraphic::~CUPSGraphic()
{
 
}


bool CUPSGraphic::DrawUPSLine(CDC* pDC, CBitmap* pbmp, int upsState)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldpen = pDC->SelectObject(&pen_Line );
	poldbrush = pDC->SelectObject(&brush_ERR);
	switch(upsState)
	{
	case 1:
		pDC->SelectObject(&brush_OK);
		pDC->Polygon(pt_line, 32);
		pDC->SelectObject(&brush_NO);
		pDC->Polygon(&pt_line[46], 6);
		break;
	
	case 2:
		pDC->SelectObject(&brush_OK);
		pDC->Polygon(pt_line, 32);
		pDC->SelectObject(&brush_NO);
		pDC->Polygon(&pt_line[32], 14);
		break;

	case 5:
		pDC->SelectObject(brush_ERR);
		pDC->Polygon(pt_line, 32);
		break;

	default:
		TRACE("\r\n==UPSGraphics.cpp DrawUPSLine:  �����������������Ϣ����ô����  ����  ����  ����2015��4��3��14:40:30====\r\n");
		break;
	}


	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);

	return TRUE;
}

// ������
bool CUPSGraphic::DrawUPSSwitch(CDC* pDC, CBitmap* pbmp, int upsState)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);
	CPen pen2, pen3;
	pen2.CreatePen(PS_SOLID, 4, RGB(0,0,0));
	pen3.CreatePen(PS_SOLID, 10, RGB(0,0,0));

	switch(upsState)
	{
	case 1:
		pDC->SelectObject(&brush_OK);
		pDC->Polygon(pt_Switch, 4);
		pDC->Polygon(&pt_Switch[12], 4);
		pDC->SelectObject(&brush_NO);
		pDC->Polygon(&pt_Switch[24], 4);

		pDC->SelectObject(&pen2);
		pDC->MoveTo(pt_Switch[5]);
		pDC->LineTo(pt_Switch[9]);
		pDC->MoveTo(pt_Switch[17]);
		pDC->LineTo(pt_Switch[21]);
		pDC->MoveTo(pt_Switch[29]);
		pDC->LineTo(pt_Switch[32]);
		break;


		// ���״̬  
		// 1��2�ſ��ش򿪣�3�ſ��رպ�
	case 2:
		pDC->SelectObject(&brush_NO);
		pDC->Polygon(pt_Switch, 4);
		pDC->Polygon(&pt_Switch[12], 4);
		pDC->SelectObject(&brush_OK);
		pDC->Polygon(&pt_Switch[24], 4);
		// 1��2�ſ��ش򿪣�3�ſ��رպ�
		pDC->SelectObject(&pen2);
		pDC->MoveTo(pt_Switch[5]);
		pDC->LineTo(pt_Switch[8]);
		pDC->MoveTo(pt_Switch[17]);
		pDC->LineTo(pt_Switch[20]);
		pDC->MoveTo(pt_Switch[29]);
		pDC->LineTo(pt_Switch[33]);
		break;

		// ��ɫ״̬
	case 5:
		pDC->SelectObject(&brush_ERR);
		// ͨ���жϣ����� ����ɫ ȫ���ǻ�ɫ
		pDC->Polygon(pt_Switch, 4);
		pDC->Polygon(&pt_Switch[12], 4);
		pDC->Polygon(&pt_Switch[24], 4);
		// 3�����ض���
		pDC->SelectObject(&pen2);
		pDC->MoveTo(pt_Switch[5]);
		pDC->LineTo(pt_Switch[8]);
		pDC->MoveTo(pt_Switch[17]);
		pDC->LineTo(pt_Switch[20]);
		pDC->MoveTo(pt_Switch[29]);
		pDC->LineTo(pt_Switch[32]);
		break;


	default:
		TRACE("\r\n==UPSGraphics.cpp DrawUPSSwitch:  �����������������Ϣ����ô����  ����  ����  ����2015��4��3��14:40:30====\r\n");
		break;
	}



	// ������  �����ṹ.  �����ǵĻ���ѡ����
	pDC->SelectObject(&pen2);
	// 1�ſ���
	pDC->MoveTo(pt_Switch[4]);
	pDC->LineTo(pt_Switch[5]);
	pDC->MoveTo(pt_Switch[6]);
	pDC->LineTo(pt_Switch[7]);
	// 2
	pDC->MoveTo(pt_Switch[16]);
	pDC->LineTo(pt_Switch[17]);
	pDC->MoveTo(pt_Switch[18]);
	pDC->LineTo(pt_Switch[19]);
	// 3
	pDC->MoveTo(pt_Switch[28]);
	pDC->LineTo(pt_Switch[29]);
	pDC->MoveTo(pt_Switch[30]);
	pDC->LineTo(pt_Switch[31]);
	// 1  �������ص�2�������
	pDC->SelectObject(&pen3);
	pDC->MoveTo(pt_Switch[10]);
	pDC->LineTo(pt_Switch[10]);
	pDC->MoveTo(pt_Switch[11]);
	pDC->LineTo(pt_Switch[11]);
	// 2
	pDC->MoveTo(pt_Switch[22]);
	pDC->LineTo(pt_Switch[22]);
	pDC->MoveTo(pt_Switch[23]);
	pDC->LineTo(pt_Switch[23]);
	// 3
	pDC->MoveTo(pt_Switch[34]);
	pDC->LineTo(pt_Switch[34]);
	pDC->MoveTo(pt_Switch[35]);
	pDC->LineTo(pt_Switch[35]);

	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);

	return TRUE;
}


// �������
bool CUPSGraphic::DrawUPSInverter(CDC* pDC, CBitmap* pbmp, int upsState)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);

	switch(upsState)
	{
		// ��·״̬
	case 1:
		pDC->SelectObject(&brush_OK);
		pDC->Polygon(pt_Inverter, 4);
		pDC->SelectObject(&brush_NO);
		pDC->Polygon(&pt_Inverter[8], 4);
	break;

	// ���״̬
	case 2:
		pDC->SelectObject(&brush_NO);
		pDC->Polygon(pt_Inverter, 4);
		pDC->SelectObject(&brush_OK);
		pDC->Polygon(&pt_Inverter[8], 4);
	break;

		// ��ɫ״̬ 
	case 5:
		pDC->SelectObject(&brush_ERR);
		pDC->Polygon(pt_Inverter, 4);
		pDC->Polygon(&pt_Inverter[8], 4);
		break;

	default:
		TRACE("\r\n==UPSGraphics.cpp DrawUPSInverter:  �����������������Ϣ����ô����  ����  ����  ����2015��4��3��14:40:30====\r\n");
		break;
	}

	

	// ��������ڲ���б�ߺ�2������
	pDC->MoveTo(pt_Inverter[1]);
	pDC->LineTo(pt_Inverter[3]);
	pDC->MoveTo(pt_Inverter[4]);
	pDC->LineTo(pt_Inverter[5]);
	pDC->MoveTo(pt_Inverter[6]);
	pDC->LineTo(pt_Inverter[7]);
	// ��������ڲ���б�ߺ�2������
	pDC->MoveTo(pt_Inverter[9]);
	pDC->LineTo(pt_Inverter[11]);
	pDC->MoveTo(pt_Inverter[12]);
	pDC->LineTo(pt_Inverter[13]);
	pDC->MoveTo(pt_Inverter[14]);
	pDC->LineTo(pt_Inverter[15]);




	// ����������1
	CPoint ptSin1[100];
	for(int t1=0; t1<100; t1++)
	{
		ptSin1[t1].x = t1 + 198 + i_move_X - 10 ;
		ptSin1[t1].y = 207 + 15 * sin((t1 / (1.6*3.1415926)) + 3.141592654) + i_move_Y;
	}

	pDC->MoveTo(ptSin1[0]);
	pDC->Polyline(ptSin1, 35);

	// ����������2	
	CPoint ptSin2[100];
	 	for(int t3=0; t3<100; t3++)
	 	{
	 		// +110����Ϊ�����������ƶ���110����
	 		ptSin2[t3].x = t3 + 400 + i_move_X + 90 + 110;
	 		ptSin2[t3].y = 243 + 15 * sin((t3 / (1.6*3.1415926)) + 3.141592654) + i_move_Y;
	 	}
	 	pDC->MoveTo(ptSin2[0]);
	 	pDC->Polyline(ptSin2, 35);
	
	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);

	return TRUE;
}


// ��BMS��
bool CUPSGraphic::DrawUPSBMS(CDC* pDC, CBitmap* pbmp, int upsState)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);

	
	CBrush FillBrush;
	FillBrush.CreateHatchBrush(2, RGB(0,255,0));


	switch(upsState)
	{
		// ��·�����״̬�£�BMS�����ǹ�����
	case 1:
	case 2:
		pDC->SetBkColor(RGB(0,125,0));
		pDC->SelectObject(&FillBrush);
		pDC->Polygon(pt_BMS, 4);
		break;

		// ��ɫ״̬
	case 5:
		pDC->SetBkColor(RGB(185,185,185));
		pDC->SelectObject(&FillBrush);
		pDC->Polygon(pt_BMS, 4);
		break;

	default:
		TRACE("\r\n==UPSGraphics.cpp DrawUPSBMS :  �����������������Ϣ����ô����  ����  ����  ����2015��4��3��14:40:30====\r\n");
		break;
	}
	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);
	
	return TRUE;
}


// �����
bool CUPSGraphic::DrawUPSBattery(CDC* pDC, CBitmap* pbmp, int upsState)
{
 	CBrush* poldbrush;
 	CPen* poldpen;
 	CBitmap* poldbmp;
 	poldbmp = pDC->SelectObject(pbmp);
 	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);
 
    pDC->MoveTo(pt_Battery[0]);
    pDC->Polyline(pt_Battery, 9);

  	pDC->SelectObject(poldbmp);
  	pDC->SelectObject(poldbrush);
  	pDC->SelectObject(poldpen);

	return TRUE;
}

// �� �ı���Ϣ
bool CUPSGraphic::DrawUPSText(CDC* pDC, CBitmap* pbmp, int upsState)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);

	CString str[9];
	str[0] = _T("��Դ����");
	str[1] = _T("  ��·");
	str[2] = _T("  ���");
	str[3] = _T("�����");
	str[4] = _T("�����");
	str[5] = _T("�����");
	str[6] = _T("������");
	str[7] = _T("����127V");
	str[8] = _T("����");
	pDC->SetBkMode(TRANSPARENT);
	switch(ups_State)
	{
	case DRAW_NULL_NULL:
		pDC->SetTextColor(RGB(255,0,0));
		break;
	case DRAW__ROOT_NULL:
		pDC->SetTextColor(RGB(0,0,250));
		break;
	case DRAW__ROOT_ROOT:
		pDC->SetTextColor(RGB(0,0,250));
		break;
	case DRAW__NODE_OK_OK_127:
		pDC->SetTextColor(RGB(0,0,250));
		break;
	case DRAW__NODE_OK_OK_0:
		pDC->SetTextColor(RGB(0,0,250));
		break;
	case DRAW__NODE_OK_NO:
		pDC->SetTextColor(RGB(0,0,250));
		break;
	case DRAW__NODE_NO_NO:
		pDC->SetTextColor(RGB(0,0,250));
		break;
	default:
		pDC->SetTextColor(RGB(255,0,0));
	}
		
	// ������ı�һ��
	for(int i=0; i<9; i++)
	{
		pDC->TextOut(pt_Text[i].x, pt_Text[i].y, str[i]);
	}
	// ������ı�����
	CString strPara[14];
	strPara[0] = v_str_Para.at(43);
	strPara[1] = _T("�ŵ������");       strPara[2] = v_str_Para.at(14);
	strPara[3] = _T("��������");       strPara[4] = v_str_Para.at(13);
    strPara[5] = _T("���������");       strPara[6] = v_str_Para.at(27);
	strPara[7] = _T("�����ص�ѹ��");   strPara[8] = v_str_Para.at(12);
	strPara[9] = _T("����¶ȣ�");       strPara[10] = v_str_Para.at(15);
	strPara[11] = _T("���������ʣ�");  strPara[12] = v_str_Para.at(41);
	strPara[13] = _T(""); //	strPara[13] = _T("�����ز���"); 

	for(int j=10; j<22; j++)
	{
		pDC->TextOut(pt_Text[j].x,  pt_Text[j].y, strPara[j-9]);
	}
	CString str_comment[3];
	str_comment[0] = _T("����״̬");
	str_comment[1] = _T("����״̬");
	str_comment[2] = _T("ͨ���ж�");   //l,t,r,b,
	CRect rt_comment1(pt_Inverter[11].x + 20, pt_BMS[0].y,       pt_Inverter[11].x + 60 + 20, pt_BMS[0].y + 12);
	CRect rt_comment2(pt_Inverter[11].x + 20, pt_BMS[0].y + 22,  pt_Inverter[11].x + 60 + 20, pt_BMS[0].y + 22 + 12);
	CRect rt_comment3(pt_Inverter[11].x + 20, pt_BMS[0].y + 44,  pt_Inverter[11].x + 60 + 20, pt_BMS[0].y + 44 + 12);
	pDC->FillRect(rt_comment1, &brush_OK);
	pDC->FillRect(rt_comment2, &brush_NO);
	pDC->FillRect(rt_comment3, &brush_ERR);
	CPoint pt_comment[3];
	pt_comment[0].x = pt_Inverter[11].x + 64 + 20;    pt_comment[0].y = pt_BMS[0].y - 3;
	pt_comment[1].x = pt_Inverter[11].x + 64 + 20;    pt_comment[1].y = pt_BMS[0].y + 22 - 3;
	pt_comment[2].x = pt_Inverter[11].x + 64 + 20;    pt_comment[2].y = pt_BMS[0].y + 44 - 3;
	pDC->SetBkMode(TRANSPARENT);
	for(int k=0; k<3; k++)
	{
		pDC->TextOut(pt_comment[k].x, pt_comment[k].y, str_comment[k]);
	}

	CFont m_newFont;
	m_newFont.CreatePointFont(280, _T("����"));
	CFont* poldfont = pDC->SelectObject(&m_newFont);
	pDC->SetBkMode(OPAQUE);
	pDC->SetTextColor(RGB(0x7c, 0xfc, 0x00));
	CString str_Name = strPara[0];
	CRect rtText(40,20,800,200);
	int textLength = str_Name.GetLength();
	if(textLength>18)
	{
		str_Name = str_Name.Left(18);
		str_Name += _T("...");
	}
	pDC->DrawText(str_Name, str_Name.GetLength(), &rtText, DT_TOP | DT_LEFT | DT_CENTER);
	pDC->SelectObject(poldfont);
	pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);
	return TRUE;
}


// �� ----------------������س�硿��״̬
bool CUPSGraphic::DrawUPSCharging(CDC* pDC, CBitmap* pbmp, int upsState)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);

	CPen pen1, pen2;
	pen1.CreatePen(PS_SOLID, 2, RGB(255,0,0));
	pen2.CreatePen(PS_SOLID, 4, RGB(0,200,0));
	pDC->SelectObject(&pen2);
	CBrush brush1;
	brush1.CreateSolidBrush(RGB(0,200,0));
	pDC->SelectObject(brush1);
	CRect rt[11];
	int static i_index = 0;

	i_index++;
	i_index = i_index % 12;
	if(0 == i_index)
		i_index++;

	for(int i=0; i<i_index; i++)
	{
		rt[i].left = pt_Battery[i+9].x;
		rt[i].top = pt_Battery[i+9].y - 7;
		rt[i].right = pt_Battery[i+9].x + 56;
		rt[i].bottom = pt_Battery[i+9].y + Battery_EQ_Width - 7;

		pDC->FillRect(rt[i], &brush1);
	}

	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);
	return TRUE;
}



// �� ----------------������� ���硿��״̬
bool CUPSGraphic::DrawUPSPowering(CDC* pDC, CBitmap* pbmp, int upsPowLevel)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);

	CBrush brushGreen, brushRed;
	brushGreen.CreateSolidBrush(RGB(0,200,0));
	brushRed.CreateSolidBrush(RGB(250,0,0));

	CRect rt_Pow;
	rt_Pow.left = pt_Battery[6].x + 1;
	rt_Pow.right = pt_Battery[6].x + 57;
	rt_Pow.bottom = pt_Battery[5].y - 1;
	rt_Pow.top = pt_Battery[5].y;
	int temp1 = (upsPowLevel)*0.9;

	if(upsPowLevel<0)
	{
		;
	}
	else if((upsPowLevel>=0) &&(upsPowLevel<=20))
	{
		rt_Pow.top =  pt_Battery[5].y - temp1 -1;
		pDC->FillRect(rt_Pow, &brushRed);
	}
	else if((upsPowLevel>=21) && (upsPowLevel<=100))
	{
		rt_Pow.top =  pt_Battery[5].y - temp1 + 1;
		pDC->FillRect(rt_Pow, &brushGreen);
	}
	else
	{
		;
	}
	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);
	return TRUE;
}


bool CUPSGraphic::DrawString(CDC* pDC, CBitmap* pbmp, HWND m_hWnd)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);


	LOGFONT logfont;
	memset(&logfont, 0, sizeof(LOGFONT));
	logfont.lfWidth = 20;
	logfont.lfHeight = 40;
	logfont.lfCharSet = GB2312_CHARSET;
	_tcscpy(logfont.lfFaceName, _T("����"));
	HFONT hFont = CreateFontIndirect(&logfont);
	HFONT hOldFont = (HFONT)pDC->SelectObject(hFont);

	COLORREF clrOldText = pDC->SetTextColor(RGB(255, 0, 128));
	pDC->SetBkMode(TRANSPARENT);
	CRect rcClient(0,10,900,600);

	CString str = _T("   ��ӭʹ���Ϻ�ɽԴUPS���߼��ϵͳ");  
	pDC->DrawText(str, str.GetLength(), &rcClient, DT_TOP | DT_LEFT );

///////////////
	pDC->SelectObject(hOldFont);
	pDC->SetBkMode(OPAQUE);

	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);

	return TRUE;
}



bool CUPSGraphic::DrawUPSList(CDC* pDC, CBitmap* pbmp, int upsState)
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,255));

	CFont m_newFont;
	CFont* oldFont;
	m_newFont.CreatePointFont(120, _T("����"));
	oldFont = pDC->SelectObject(&m_newFont);
	int i_size = v_list_ups.size();
	for(int i=0; i<i_size; i++)
	{
		CString str_name = v_list_ups.at(i).str_ups_name;
		int n_length = str_name.GetLength();
 		if(n_length > 12)
 		{
 			str_name = str_name.Left(12);
 			str_name += _T("...");
 		}    
		pDC->TextOut(v_pt_upsList.at(i).ptIP.x,        v_pt_upsList.at(i).ptIP.y, v_list_ups.at(i).str_ups_IP);
		// UPS����ģʽ
		pDC->TextOut(v_pt_upsList.at(i).ptWorkMode.x,  v_pt_upsList.at(i).ptWorkMode.y, v_list_ups.at(i).str_ups_workmode);

		// ͨ��״̬ (�������Ͽ�)
		if(_T("�ж�") == v_list_ups.at(i).str_ups_online)
		{
			pDC->SetTextColor(RGB(255,0,0));
			pDC->TextOut(v_pt_upsList.at(i).ptOnline.x,    v_pt_upsList.at(i).ptOnline.y, v_list_ups.at(i).str_ups_online);
			pDC->SetTextColor(RGB(0,0,255));
		}
		else
		{
			pDC->TextOut(v_pt_upsList.at(i).ptOnline.x,    v_pt_upsList.at(i).ptOnline.y, v_list_ups.at(i).str_ups_online);
		}


		// ������״̬��(���ӡ��Ͽ�)
		if(_T("�ж�") == v_list_ups.at(i).str_ups_linkstate)
		{
			pDC->SetTextColor(RGB(255,0,0));
			pDC->TextOut(v_pt_upsList.at(i).ptLinkstate.x, v_pt_upsList.at(i).ptLinkstate.y, v_list_ups.at(i).str_ups_linkstate);
			pDC->TextOut(v_pt_upsList.at(i).ptName.x,      v_pt_upsList.at(i).ptName.y, str_name);
			pDC->SetTextColor(RGB(0,0,255));
		}
		else
		{
			pDC->TextOut(v_pt_upsList.at(i).ptLinkstate.x, v_pt_upsList.at(i).ptLinkstate.y, v_list_ups.at(i).str_ups_linkstate);

			// UPS������Ҳ���ɫ
			pDC->TextOut(v_pt_upsList.at(i).ptName.x,      v_pt_upsList.at(i).ptName.y, str_name);
		}



		pDC->TextOut(v_pt_upsList.at(i).ptbattery_C.x, v_pt_upsList.at(i).ptbattery_C.y, v_list_ups.at(i).str_ups_battery_C);
		pDC->TextOut(v_pt_upsList.at(i).ptbattery_V.x, v_pt_upsList.at(i).ptbattery_V.y, v_list_ups.at(i).str_ups_battery_V);
		pDC->TextOut(v_pt_upsList.at(i).ptbattery_T.x, v_pt_upsList.at(i).ptbattery_T.y, v_list_ups.at(i).str_ups_battery_T);

	}

	// ���ڵ�����
	CFont CaptionFont;
	CaptionFont.CreatePointFont(340, _T("����"));
	pDC->SelectObject(CaptionFont);
	CString str_temp = str_Root_Name;
	// CRect(l,t,r,b)
	CRect  rt_root(10,20,800,300);
	int i_rootLength = str_temp.GetLength();
	if(i_rootLength>15)
	{
		str_temp = str_temp.Left(15);
		str_temp += _T("...");
	}
	pDC->DrawText(str_temp, str_temp.GetLength(), &rt_root, DT_TOP | DT_LEFT | DT_CENTER);

	CFont TitleFont;
	TitleFont.CreatePointFont(120, _T("����"));
	pDC->SelectObject(TitleFont);
	
	pDC->TextOut(upsList_Title.ptName.x,      upsList_Title.ptName.y, _T("UPS����"));
	pDC->TextOut(upsList_Title.ptIP.x,        upsList_Title.ptIP.y, _T("IP��ַ"));
	pDC->TextOut(upsList_Title.ptWorkMode.x,  upsList_Title.ptWorkMode.y, _T("����ģʽ"));
	pDC->TextOut(upsList_Title.ptOnline.x,    upsList_Title.ptOnline.y, _T("ͨ��״̬"));
	pDC->TextOut(upsList_Title.ptLinkstate.x, upsList_Title.ptLinkstate.y, _T("����״̬"));
	pDC->TextOut(upsList_Title.ptbattery_C.x, upsList_Title.ptbattery_C.y, _T("�������"));
	pDC->TextOut(upsList_Title.ptbattery_V.x, upsList_Title.ptbattery_V.y, _T("��ص�ѹ"));
	pDC->TextOut(upsList_Title.ptbattery_T.x, upsList_Title.ptbattery_T.y, _T("����¶�"));

	// ���������棬��һ���п�ȵ���
	CBrush pBrush_Line;
	pBrush_Line.CreateSolidBrush(RGB(0xab, 0x82, 0xff));
//	pBrush_Line.CreateSolidBrush(RGB(0x93, 0x70, 0xdb));
	CRect rt(10,118,770,124);
	pDC->FillRect(rt, &pBrush_Line);

	pDC->SelectObject(oldFont);
	///////////////
	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);
	return TRUE;
}



bool CUPSGraphic::DrawTimeInfo(CDC* pDC, CBitmap* pbmp, CString strArr[3])
{
	CBrush* poldbrush;
	CPen* poldpen;
	CBitmap* poldbmp;
	poldbmp = pDC->SelectObject(pbmp);
	poldbrush = pDC->SelectObject(&brush_OK);
	poldpen = pDC->SelectObject(&pen_Line);

	CFont TimeFont;
	TimeFont.CreatePointFont(320, _T("����"));
	CFont* poldfont = pDC->SelectObject(&TimeFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,255));


	CPoint  timePt[3];
	timePt[0].x = 340;    timePt[0].y = 160;
	timePt[1].x = 340;    timePt[1].y = 260;
	timePt[2].x = 340;    timePt[2].y = 360;

	pDC->TextOut(timePt[0].x, timePt[0].y, strArr[0]);
	pDC->TextOut(timePt[1].x, timePt[1].y, strArr[1]);
	pDC->TextOut(timePt[2].x, timePt[2].y, strArr[2]);

	pDC->SelectObject(poldfont);
	pDC->SelectObject(poldbmp);
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);
	return TRUE;
}




/////////////////////////��ʼ����������//////////////////////////


// ��ʼ��DC����
void CUPSGraphic::InitUPSDC()
{
	lineWidth = 10;
	SwitchWidth = 60;
	InverterWidth = 80;
	Battery_EQ_Width = 6;
	i_move_X = 10;
	i_move_Y = 60;
	pen_Line.CreatePen(PS_SOLID, 2, RGB(0,0,0));
	brush_NO.CreateSolidBrush(RGB(255, 255, 0));
	brush_OK.CreateSolidBrush(RGB(0,255,0));
	brush_ERR.CreateSolidBrush(RGB(125,125,125));
	brush_bk.CreateStockObject(LTGRAY_BRUSH);
}

void CUPSGraphic::InitLineData()
{
	for(int i=0; i<60; i++)
	{
		pt_line[i].x = 0;
		pt_line[i].y = 0;
	}
	// ------------- UPS �� ������ -------------------------------------
	// ����  ��32���㡿
	pt_line[0].x = 10;                   pt_line[0].y = 150;
	pt_line[1].x = 140;                  pt_line[1].y = 150;
	pt_line[2].x = 140;                  pt_line[2].y = 70 - lineWidth;
	pt_line[3].x = 650 + 3*lineWidth;    pt_line[3].y = 70 - lineWidth;
	pt_line[4].x = 650 + 3*lineWidth;    pt_line[4].y = 150;
	pt_line[5].x = 690 + 3*lineWidth;    pt_line[5].y = 150;
	pt_line[6].x = 690 + 3*lineWidth;    pt_line[6].y = 150 + lineWidth;
	pt_line[7].x = 650 + 3*lineWidth;    pt_line[7].y = 150 + lineWidth;
	pt_line[8].x = 650 + 3*lineWidth;    pt_line[8].y = 210 + 2*lineWidth;
	pt_line[9].x = 600 + 3*lineWidth;    pt_line[9].y = 210 + 2*lineWidth;
	pt_line[10].x = 600 + 3*lineWidth;   pt_line[10].y = 210 + 1*lineWidth;
	pt_line[11].x = 650 + 2*lineWidth;   pt_line[11].y = 210 + lineWidth;
	pt_line[12].x = 650 + 2*lineWidth;   pt_line[12].y = 70;
	pt_line[13].x = 140 + lineWidth;     pt_line[13].y = 70;
	pt_line[14].x = 140 + lineWidth;     pt_line[14].y = 210 + lineWidth;

	pt_line[15].x = 600 + 3*lineWidth;   pt_line[15].y = 210 + 1*lineWidth;
	pt_line[16].x = 600 + 3*lineWidth;   pt_line[16].y = 210 + 2*lineWidth;
	// �ϵ�������
//	pt_line[15].x = 470 + 3*lineWidth;   pt_line[15].y = 210 + 1*lineWidth;
//	pt_line[16].x = 470 + 3*lineWidth;   pt_line[16].y = 210 + 2*lineWidth;
//

	pt_line[17].x = 340 + 2*lineWidth;   pt_line[17].y = 210 + 2*lineWidth;
	pt_line[18].x = 340 + 2*lineWidth;   pt_line[18].y = 240 + 2*lineWidth;
	pt_line[19].x = 370 + 2*lineWidth + 15;   pt_line[19].y = 240 + 2*lineWidth;
// BMS����Ϸ�
	pt_line[20].x = 370 + 2*lineWidth + 15;   pt_line[20].y = 260 + 3*lineWidth + 40 + 10;
	pt_line[21].x = 370 + lineWidth + 15;     pt_line[21].y = 260 + 3*lineWidth + 40 + 10;

	pt_line[22].x = 370 + lineWidth + 15;     pt_line[22].y = 240 + 3*lineWidth;
	pt_line[23].x = 310 + 2*lineWidth - 15;   pt_line[23].y = 240 + 3*lineWidth;
	pt_line[24].x = 310 + 2*lineWidth - 15;   pt_line[24].y = 260 + 3*lineWidth;
	pt_line[25].x = 310 + lineWidth - 15;     pt_line[25].y = 260 + 3*lineWidth;
	pt_line[26].x = 310 + lineWidth - 15;     pt_line[26].y = 240 + 2*lineWidth;
	pt_line[27].x = 340 + lineWidth;     pt_line[27].y = 240 + 2*lineWidth;
	pt_line[28].x = 340 + lineWidth;     pt_line[28].y = 210 + 2*lineWidth;


	pt_line[29].x = 140;                 pt_line[29].y = 210 + 2*lineWidth;
	pt_line[30].x = 140;                 pt_line[30].y = 150 + lineWidth;
	pt_line[31].x = 10;                  pt_line[31].y = 150 + lineWidth;

	// ���Ļ�ɫ����  ��14���㡿
	pt_line[32].x = 10;                  pt_line[32].y = 150;
	pt_line[33].x = 140;                 pt_line[33].y = 150;
	pt_line[34].x = 140;                 pt_line[34].y = 70 - lineWidth;
	pt_line[35].x = 650 + 3*lineWidth;   pt_line[35].y = 70 - lineWidth;
	pt_line[36].x = 650 + 3*lineWidth;   pt_line[36].y = 150;
	pt_line[37].x = 650 + 2*lineWidth;   pt_line[37].y = 150;
	pt_line[38].x = 650 + 2*lineWidth;   pt_line[38].y = 70;
	pt_line[39].x = 140 + lineWidth;     pt_line[39].y = 70;
	pt_line[40].x = 140 + lineWidth;     pt_line[40].y = 210 + lineWidth;
//
	pt_line[41].x = 340 + lineWidth;     pt_line[41].y = 210 + 1*lineWidth;
	pt_line[42].x = 340 + lineWidth;     pt_line[42].y = 210 + 2*lineWidth;

	pt_line[43].x = 140;                 pt_line[43].y = 210 + 2*lineWidth;
	pt_line[44].x = 140;                 pt_line[44].y = 150 + lineWidth;
	pt_line[45].x = 10;                  pt_line[45].y = 150 + lineWidth;

	// ��·�Ļ�ɫ����  ��6���㡿
	pt_line[46].x = 340 + 2*lineWidth;   pt_line[46].y = 210 + 1*lineWidth;
	pt_line[47].x = 650 + 2*lineWidth;   pt_line[47].y = 210 + lineWidth;
	pt_line[48].x = 650 + 2*lineWidth;   pt_line[48].y = 150 + lineWidth;
	pt_line[49].x = 650 + 3*lineWidth;   pt_line[49].y = 150 + lineWidth;
	pt_line[50].x = 650 + 3*lineWidth;   pt_line[50].y = 210 + 2*lineWidth;
	pt_line[51].x = 340 + 2*lineWidth;   pt_line[51].y = 210 + 2*lineWidth;


	// ���Ҫ���ƣ�����������Ū��forѭ��
	for(int j=0; j<60; j++)
	{
		pt_line[j].x += i_move_X;
		pt_line[j].y += i_move_Y;
	}


}

// ��ʼ�� ���� ������
void CUPSGraphic::InitSwitchData()
{
	for(int i=0; i<50; i++)
	{
		pt_Switch[i].x = 0;
		pt_Switch[i].y = 0;
	}

	// --------------- ����  ������ -----------------------------------
	pt_Switch[0].x = 50;    pt_Switch[0].y = 125;
	pt_Switch[1].x = 110;    pt_Switch[1].y = 125;
	pt_Switch[2].x = 110;    pt_Switch[2].y = 175;
	pt_Switch[3].x = 50;    pt_Switch[3].y = 175;

	pt_Switch[4].x = 50;    pt_Switch[4].y = 155;
	pt_Switch[5].x = 70;    pt_Switch[5].y = 155;
	pt_Switch[6].x = 90;    pt_Switch[6].y = 155;
	pt_Switch[7].x = 108;    pt_Switch[7].y = 155;

	// ���ؿ�
	pt_Switch[8].x = 85;    pt_Switch[8].y = 130;
	// ���ر�
	pt_Switch[9].x = 90;    pt_Switch[9].y = 152;

	// ͻ����2����
	pt_Switch[10].x = 70;    pt_Switch[10].y = 155;
	pt_Switch[11].x = 90;    pt_Switch[11].y = 152;


	// �����ط�ֻ��Ҫ�ƶ�
	for(int j=0; j<12; j++)
	{
//		pt_Switch[j + 12].x = pt_Switch[j].x + 190 + lineWidth;
		pt_Switch[j + 12].x = pt_Switch[j].x + 400 + 2*lineWidth  - 110 + 80;
		pt_Switch[j + 12].y = pt_Switch[j].y - 80 - lineWidth;

		// ��3�����غ����������λ�á����� ���� 110
		pt_Switch[j + 24].x = pt_Switch[j].x + 400 + 2*lineWidth  - 110 + 80;
		pt_Switch[j + 24].y = pt_Switch[j].y + 60 + lineWidth;
	}



	for(int i=0; i<50; i++)
	{
		pt_Switch[i].x += i_move_X;
		pt_Switch[i].y += i_move_Y;
	}

}



// ��ʼ�� ����� ������
void CUPSGraphic::InitInverterData()
{
	for(int i=0; i<50; i++)
	{
		pt_Inverter[i].x = 0;
		pt_Inverter[i].y = 0;
	}

	// ----------------  ������������ ����  ----------------------------------
	// �����   ������ֱ��
	pt_Inverter[0].x = 180;	  pt_Inverter[0].y = 185;
	pt_Inverter[1].x = 260;   pt_Inverter[1].y = 185;
	pt_Inverter[2].x = 260;   pt_Inverter[2].y = 265;
	pt_Inverter[3].x = 180;   pt_Inverter[3].y = 265;

	pt_Inverter[4].x = 224;   pt_Inverter[4].y = 250;
	pt_Inverter[5].x = 250;   pt_Inverter[5].y = 250;
	pt_Inverter[6].x = 224;   pt_Inverter[6].y = 240;
	pt_Inverter[7].x = 250;   pt_Inverter[7].y = 240;


	// �����  ֱ���佻��
	pt_Inverter[8].x = 470;	  pt_Inverter[8].y = 185;
	pt_Inverter[9].x = 550;   pt_Inverter[9].y = 185;
	pt_Inverter[10].x = 550;   pt_Inverter[10].y = 265;
	pt_Inverter[11].x = 470;   pt_Inverter[11].y = 265;

	pt_Inverter[12].x = 480;   pt_Inverter[12].y = 200;
	pt_Inverter[13].x = 506;   pt_Inverter[13].y = 200;
	pt_Inverter[14].x = 480;   pt_Inverter[14].y = 210;
	pt_Inverter[15].x = 506;   pt_Inverter[15].y = 210;

	for(int j=8; j<16; j++)
	{
		// ������Ϳ���3���˸�λ��
		pt_Inverter[j].x += 90;
	}



	for(int k=0; k<50; k++)
	{
		pt_Inverter[k].x += i_move_X;
		pt_Inverter[k].y += i_move_Y;
	}



	//	pt_Inverter[8].x = ;   pt_Inverter[8].y = ;
	//	pt_Inverter[9].x = ;   pt_Inverter[9].y = ;


}

// ��ʼ���� BMS�� ������
void CUPSGraphic::InitBMSData()
{
	for(int m=0; m<50; m++)
	{
		pt_BMS[m].x = 0;
		pt_BMS[m].y = 0;
	}


	pt_BMS[0].x = 320 + 50; 	pt_BMS[0].y = 330 + 10; 
	pt_BMS[1].x = 380 + 50; 	pt_BMS[1].y = 330 + 10; 
	pt_BMS[2].x = 380 + 50; 	pt_BMS[2].y = 380 + 8; 
	pt_BMS[3].x = 320 + 50; 	pt_BMS[3].y = 380 + 8; 



	for(int i=0; i<50; i++)
	{
		pt_BMS[i].x += i_move_X;
		pt_BMS[i].y += i_move_Y;
	}

}

// ��ʼ�� ��� ������
void CUPSGraphic::InitBatteryData()
{
	for(int i=0; i<100; i++)
	{
		pt_Battery[i].x = 0;
		pt_Battery[i].y = 0;
	}
	//��ؿ��
	pt_Battery[0].x = 276;    pt_Battery[0].y = 290;
	pt_Battery[1].x = 294;    pt_Battery[1].y = 290;

	pt_Battery[2].x = 294;    pt_Battery[2].y = 305 - 7;
	pt_Battery[3].x = 314;    pt_Battery[3].y = 305 - 7;
	pt_Battery[4].x = 314;    pt_Battery[4].y = 395 - 7;
	pt_Battery[5].x = 256;    pt_Battery[5].y = 395 - 7;
	pt_Battery[6].x = 256;    pt_Battery[6].y = 305 - 7;
	pt_Battery[7].x = 276;    pt_Battery[7].y = 305 - 7;
	pt_Battery[8].x = 276;    pt_Battery[8].y = 290;

	// ���״̬��һЩ�� 
	// ����10�������Կ�
	int temp = 2; // ���ֱ�ӵļ��
	int tempL = 1;// �����ҵļ��
	for(int j=0; j<11; j++)
	{
		pt_Battery[j+9].x = 256 + tempL;    pt_Battery[j+9].y = 395 - (j+1)*temp - (j+1)*Battery_EQ_Width;
	}

	// ��ص����ٷֱ�
	pt_Battery[20].x = 256 + 14;  pt_Battery[20].y = 350;



	for(int k=0; k<100; k++)
	{
		pt_Battery[k].x += i_move_X + 25;
		pt_Battery[k].y += i_move_Y;
	}
}




// ��ʼ�� �ı� ������
void CUPSGraphic::InitTextData()
{
	for(int n=0; n<50; n++)
	{
		pt_Text[n].x = 0;
		pt_Text[n].y = 0;
	}


	// ����ע�⡣�˴��õ�������ط������������������õ������������������pt_Text֮ǰ��ʼ��
	// 
	// ����1���½�
	pt_Text[0].x = pt_Switch[3].x ;    pt_Text[0].y = pt_Switch[3].y + 8;
	// ����2���Ͻ�
	pt_Text[1].x = pt_Switch[12].x + 7;   pt_Text[1].y = pt_Switch[12].y - 22;
	// ����3���½�
//	pt_Text[2].x = pt_Switch[27].x + 7;   pt_Text[2].y = pt_Switch[27].y + 8;
	// ����3���Ͻ�
	pt_Text[2].x = pt_Switch[24].x + 6;   pt_Text[2].y = pt_Switch[24].y - 22;

	// �����
	// ���Ͻ�
	pt_Text[3].x = pt_Inverter[0].x + 18;   pt_Text[3].y = pt_Inverter[0].y -22;
	// �����
	// ���Ͻ�
	pt_Text[4].x = pt_Inverter[8].x + 16;   pt_Text[4].y = pt_Inverter[8].y - 22;

	// ������½�
	pt_Text[5].x = pt_Battery[5].x + 6;   pt_Text[5].y = pt_Battery[5].y + 5;
	// ��ء�����в�������
//	pt_Text[5].x = pt_Battery[5].x + 8;   pt_Text[5].y = (pt_Battery[3].y + pt_Battery[4].y)/2 + 5;

	// BMS�����½�
	pt_Text[6].x = pt_BMS[3].x + 10;       pt_Text[6].y = pt_BMS[3].y + 5;

	// ����127V����
	pt_Text[7].x = pt_line[0].x;           pt_Text[7].y = pt_line[0].y - 60;

	// ��������
	pt_Text[8].x = pt_line[5].x - 10;      pt_Text[8].y = pt_line[5].y - 25;






// 	// UPS���ƣ�����ʾ���������...   ##########################
	// *********** ����������Ϣ *********** 
	pt_Text[9].x = 220;    pt_Text[9].y = 20; 
	// �ŵ�������ַ�����
	pt_Text[10].x = pt_Inverter[11].x - 20;         pt_Text[10].y = pt_Inverter[11].y + 6; 
	// �ŵ�����ġ���ֵ��
	pt_Text[11].x = pt_Inverter[11].x + 52;    pt_Text[11].y = pt_Inverter[11].y + 6; 
	// ���������ַ�����
	pt_Text[12].x = pt_Inverter[3].x - 20;         pt_Text[12].y = pt_Inverter[3].y + 6; 
	// �������ġ���ֵ��
	pt_Text[13].x = pt_Inverter[3].x + 52;    pt_Text[13].y = pt_Inverter[3].y + 6; 


//-------------------------------------------------------
	for(int i=14; i<22; i++)
	{
		pt_Text[i].x = pt_line[0].x + 100;         pt_Text[i].y = pt_Inverter[3].y + 40 + (i-14)*12 + 12;
		pt_Text[i+1].x = pt_line[0].x + 200;         pt_Text[i+1].y = pt_Inverter[3].y + 40 + (i-14)*12 + 12;  
		i++;
	}
	pt_Text[22].x = pt_line[0].x + 20;          pt_Text[22].y = pt_Inverter[3].y + 20 + 10*12;  
}



////////////// UPSList�ĸ�����Ҳ�˴���ʼ��
void CUPSGraphic::InitUPSListPoint()
{

	UpsPoint  oneUPS;
	int temp_Width = 23;
	for(int i=0; i<64; i++)
	{
		oneUPS.ptName.x = 10;           oneUPS.ptName.y = 130 + i*temp_Width;
		oneUPS.ptIP.x = 220 + 5;	        oneUPS.ptIP.y = 130 + i*temp_Width;
		oneUPS.ptWorkMode.x = 350 + 5;      oneUPS.ptWorkMode.y = 130 + i*temp_Width;
		oneUPS.ptOnline.x = 435 + 5;        oneUPS.ptOnline.y = 130 + i*temp_Width;
		oneUPS.ptLinkstate.x = 506 + 5;     oneUPS.ptLinkstate.y = 130 + i*temp_Width;
		oneUPS.ptbattery_C.x = 570 + 5;     oneUPS.ptbattery_C.y = 130 + i*temp_Width;
		oneUPS.ptbattery_V.x = 640 + 5;     oneUPS.ptbattery_V.y = 130 + i*temp_Width;
		oneUPS.ptbattery_T.x = 710 + 5;     oneUPS.ptbattery_T.y = 130 + i*temp_Width;
		v_pt_upsList.push_back(oneUPS);
	}


	// UPS�б������������
	upsList_Title.ptName.x = 60;               upsList_Title.ptName.y = 90;
	upsList_Title.ptIP.x = 220 + 35;           upsList_Title.ptIP.y = 90;
	upsList_Title.ptWorkMode.x = 350 + 5;      upsList_Title.ptWorkMode.y = 90;
	upsList_Title.ptOnline.x = 420 + 5;        upsList_Title.ptOnline.y = 90;
	upsList_Title.ptLinkstate.x = 490 + 5;     upsList_Title.ptLinkstate.y = 90;
	upsList_Title.ptbattery_C.x = 560 + 5;     upsList_Title.ptbattery_C.y = 90;
	upsList_Title.ptbattery_V.x = 630 + 5;     upsList_Title.ptbattery_V.y = 90;
	upsList_Title.ptbattery_T.x = 700 + 5;     upsList_Title.ptbattery_T.y = 90;
}





bool CUPSGraphic::ClearBmp(CDC* pDC, CBitmap* pbmp, CRect rt, CBrush* pbrush)
{
	pDC->FillRect(rt, pbrush);

	return TRUE;
}






