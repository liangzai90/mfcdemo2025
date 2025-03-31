#include "stdafx.h"
#include "TcpClient.h"

// 这个头文件干啥的？
#include <locale.h>

#include "user_ulti.h"   
#include "mbsdef.h"

#pragma once
#include "UPS201501.h"


USHORT SW_CRC16(UCHAR CRCBuf[], UCHAR len)
{
	USHORT crc = 0xFFFF;
	UCHAR i,j;
	USHORT Current;
	for(i=0; i<len; i++)
	{
		Current = CRCBuf[i];

		for(j=0; j<8; j++)
		{
			if((crc^Current)&0x0001)
			{
				crc >>= 1;
				crc ^= 0x8408;
			}
			else
			{
				crc >>= 1;
			}

			Current >>= 1;
		}
	}

	return (crc);
}



CTcpClient::CTcpClient(UPS_NODE_CB *pcb)
{
	m_dwTimeoutMs = CONNECT_INTERVAL_TIME;
	m_evt = NULL;
	m_sock = INVALID_SOCKET;
	m_pcb = pcb;
	m_pcb->tlcb.link_state =0;
	m_pcb->online = 0;
}

CTcpClient::~CTcpClient(void)
{
	CloseConnect();
	if(m_evt != NULL)
	{
		WSACloseEvent(m_evt);
		m_evt = NULL;
	}
}

BOOL CTcpClient::Initialize()
{
	RecCnt_OK = 0;
	RecCnt_Err = 0;
	RepeatCRCErrCnt = 0;
	RebootCnt = 0;
	m_sock = INVALID_SOCKET;
	strcpy_s(m_strServer, 30, m_pcb->tlcb.strIp);
	m_wPort = m_pcb->tlcb.port;
	m_evt = WSACreateEvent();
	m_tRx = m_tTx = time(NULL);
	m_tConnect = m_tRx - CONNECT_ACTIVE_INTERVAL;
	m_TxByteCnt = m_RxByteCnt = 0;
	GetModuleFileName(NULL, logFile_path, _MAX_PATH);
	PathRemoveFileSpec(logFile_path);
	mbstowcs(logFile_name, m_pcb->tlcb.strIp, _MAX_PATH);
	wcscat_s(logFile_path, L"\\");
	wcscat_s(logFile_path, logFile_name);
	wcscat_s(logFile_path, L".txt");
	return TRUE;
}


int CTcpClient::SaveLogFile(const CString str_Data)
{
	CStdioFile my_log_file(logFile_path, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	my_log_file.SeekToEnd();
	my_log_file.WriteString(str_Data);
	my_log_file.Close();
	return 0;
}


void CTcpClient::active_link()
{
	m_tTx = time(NULL);
}

//
int CTcpClient::poll_function()
{	
	m_TxByteCnt = 0;
	if(m_pcb->tlcb.link_state)
	{
		if(m_pcb->state == MODBUS_READ_CELL_DYNAMIC)
		{
			m_TxByteCnt = pack_frame(m_TxDat, MODBUS_READ_CELL_DYNAMIC);
		}
		else  if(m_pcb->state == MODBUS_READ_SYSTEM_DYNAMIC)
		{
			m_TxByteCnt = pack_frame(m_TxDat, MODBUS_READ_SYSTEM_DYNAMIC);
		}
		else
		{
			TRACE("\r\n ---  poll_function:  其它功能码？ 此处有错！！！  ----- \r\n");
		}
	}
	return m_TxByteCnt;
}

void CTcpClient::Run()
{
	time_t tCur;

		tCur = time(NULL);
		if(m_pcb->tlcb.link_state)
		{
				/////接收超时, 断开
				if((m_tRx + CONNECT_INTERVAL_TIME) < tCur)
				{
					TRACE("Receive timeout, close link %s", ctime(&tCur));
					CloseConnect();
				}
				
				if((m_tTx + CONNECT_ACTIVE_INTERVAL - 5) < tCur)
				{
					active_link();
				}
		}
		else
		{
			SetupConnect();
		}
		OnSocketMessage();
}

void CTcpClient::OnSocketMessage()
{
	WSANETWORKEVENTS netevt;
	DWORD dwResult = 0;

	if(m_evt == NULL)
		return ;

	poll_function();

	
	if(m_TxByteCnt > 0)
	{
		SendData(m_TxDat, m_TxByteCnt);
	}

	dwResult = WSAWaitForMultipleEvents(1, &m_evt, FALSE, 2000, FALSE);
	if(dwResult == WSA_WAIT_FAILED)
	{
		TRACE("Sockets event error ... CloseConnect()    %d \n", WSAGetLastError());
		CloseConnect();
	}
	else if(dwResult == WSA_WAIT_TIMEOUT)
	{
		m_pcb->tmt_cnt++;
		if(m_pcb->tmt_cnt > 5)
		{
			if(m_pcb->online > 0)
			{
				AfxGetMainWnd()->PostMessage(WM_USER_NOTICE_MSG, (WPARAM)m_pcb,  UPS_SOE_CODE_ONLINE << 8);
			}
			m_pcb->online = 0;			
		}
	}
	else
	{
		WSAEnumNetworkEvents(m_sock, m_evt, &netevt);
		if(netevt.lNetworkEvents & FD_READ)
		{
			if(netevt.iErrorCode[FD_READ_BIT] != 0)
			{
				OutputDebugString(L"Read event error\n");
				CloseConnect();
			}
			else  
			{
				if(ReceiveData() > 0)
				{
					m_pcb->tmt_cnt = 0;
					m_tRx = time(NULL);
				}
			}
		}
		
		if(netevt.lNetworkEvents & FD_CLOSE)
		{
			CloseConnect();
		}
		
		if(netevt.lNetworkEvents & FD_CONNECT)
		{
			if(netevt.iErrorCode[FD_CONNECT_BIT] == 0)
			{

				m_tRx = time(NULL);
				m_tTx = m_tRx;
				m_pcb->online = 0;
				m_pcb->state = MODBUS_READ_CELL_DYNAMIC;
				m_pcb->tmt_cnt = 0;
				TRACE("Connect success\n");
				AfxGetMainWnd()->PostMessage(WM_USER_NOTICE_MSG, (WPARAM)m_pcb,UPS_SOE_CODE_TCP_LINK << 8 | 1);
				m_pcb->tlcb.link_state = 1;

			}
		}
		Sleep(1000);
	}
}

int CTcpClient::ReceiveData()
{
	char	sz[40];
	SYSTEMTIME	tt;
	BYTE buff[1024];
	m_RxByteCnt = recv(m_sock, (char *)buff, 256, 0);

	if(m_RxByteCnt > 0)
	{
		if( (buff[MODBUS_RECV_HOST_ADDRESS_POS] == m_TxDat[MODBUS_SEND_HOST_ADDRESS_POS]) )
		{
			GetLocalTime(&tt);
			m_strRecv = m_strServer;
			sprintf(sz, " RX(%02d)[%02d:%02d:%02d.%03d]: ", m_RxByteCnt,tt.wHour, tt.wMinute, tt.wSecond, tt.wMilliseconds);
			m_strRecv += sz;
			for(int i=0; i<m_RxByteCnt; i++)
			{
				sprintf(sz, "%02X ", buff[i]);
				m_strRecv += sz;
			}
			m_strRecv += ("\n");
			WORD wCrc;
			wCrc = SW_CRC16(buff, m_RxByteCnt-2);
			BYTE B_CRCL, B_CRCH;
			B_CRCL = wCrc;
			B_CRCH = wCrc>>8;
			if((buff[m_RxByteCnt-2] == B_CRCL) && (buff[m_RxByteCnt-1] == B_CRCH))
			{
				unpack_frame(buff, m_RxByteCnt);
				RecCnt_OK++;
				CString strtemp1 = _T("");
				strtemp1.Format(_T("【%d】正常的接收\r"), RecCnt_OK);
                //SaveLogFile(strtemp1);
                SaveLogFile(m_strRecv);
				RepeatCRCErrCnt = 0;
			}
			else
			{
				TRACE("\r\n      真的存在 校验码错误的情况？？？？？？？？？*********   \r\n");
				m_strRecv = _T("\r\n ***** 校验有错！！！ ") + m_strRecv;
				RecCnt_Err++;
				CString strtemp2 = _T("");
				strtemp2.Format(_T("\r【%d】校验码错误 \r"), RecCnt_Err);
				SaveLogFile(strtemp2);
				SaveLogFile(m_strRecv);

				RepeatCRCErrCnt++;
				//	if(RepeatCRCErrCnt > 10)
				if(0)
				{
					m_pcb->tlcb.link_state = 0;
					m_pcb->online = 0;
					RepeatCRCErrCnt = 0;
					m_tConnect = time(NULL) - 10;
					RebootCnt++;
					CString str_reboot =  _T("");
					str_reboot.Format(_T("\r UPS第【%d】次重启 \r"), RebootCnt);
					//SaveLogFile(str_reboot);
				}

			}
			OutputDebugString(m_strRecv);
		}
		else
		{
			// TRACE("\r\n  TODO001: 真的存在，主机地址从机地址不对应的情况！！！ \r\n");
		}
	}
	else
	{
		// TRACE("\r\n  返回数据长度小于等于0。 是接收数据失败的意思吗？ \r\n");
	}
	return m_RxByteCnt;
}


void CTcpClient::unpack_frame(BYTE *pdat, unsigned int bytes)
{
	ASSERT(pdat != NULL);
	unsigned int data_Btye_Length = 0;
	unsigned int data_2Btyes_Length = 0;
	if(pdat[MODBUS_RECV_FUNCTION_POS] == MODBUS_READ_CELL_DYNAMIC)
	{
		data_Btye_Length = pdat[MODBUS_RECV_COUNT_POS];
		if(data_Btye_Length == 1)
		{
			TRACE("\r\n   错误回传帧结构  \r\n");
		}
		data_2Btyes_Length = (bytes - 6) / 2;
		if(data_2Btyes_Length > UPS_MAX_DATA_NUM)
		{
			data_2Btyes_Length = UPS_MAX_DATA_NUM;
			TRACE("\r\n----怎么可能？ 收到的数据长度 超过最大长度？？？-----\r\n");
		}
		for(int i=0; i<data_2Btyes_Length; i++)
		{
			m_pcb->measure_0x67[i] = pdat[MODBUS_RECV_DATA_POS + i*2 + 1];  
			m_pcb->measure_0x67[i] <<= 8;
			m_pcb->measure_0x67[i] |= pdat[MODBUS_RECV_DATA_POS + i*2];     
		}

		// 0~11: 电池电压
		// 12~23: 电池温度   
		// 电压
		for(int i=0; i<12; i++)
		{
			m_pcb->Measure_ToHL[i] = m_pcb->measure_0x67[i*6];
		}
		// 温度
		for(int i=12; i<24; i++)
		{
			m_pcb->Measure_ToHL[i] = m_pcb->measure_0x67[(i-12)*6+1] - 2731.5;
		}
		m_pcb->state = MODBUS_READ_SYSTEM_DYNAMIC;			
	}
	else if(pdat[MODBUS_RECV_FUNCTION_POS] == MODBUS_READ_SYSTEM_DYNAMIC)
	{
		data_Btye_Length = pdat[MODBUS_RECV_COUNT_POS];
		if(data_Btye_Length == 1)
		{
			TRACE("\r\n   错误回传帧结构  \r\n");
		}
		if(data_Btye_Length > UPS_MAX_DATA_NUM)
		{
			data_Btye_Length = UPS_MAX_DATA_NUM;
			TRACE("\r\n----怎么可能？ 收到的数据长度 超过最大长度？？？-----\r\n");
		}

		for(int i=0; i<data_Btye_Length; i++)
		{
			m_pcb->dio_0x6F[i] = pdat[MODBUS_RECV_DATA_POS + i];
		}

		//TODO2:旁路/逆变 判断
		//检测交流告警  2015/04/15  何亮
		int cur_dio, old_dio;
		cur_dio = m_pcb->dio_0x6F[26] & 0x40;   // 0100 0000
		old_dio = m_pcb->old_dio_0x6F[26] & 0x40;
		if(cur_dio != old_dio)
		{
			DWORD dw;
			if(cur_dio > 0)
			{
				dw = UPS_SOE_CODE_POWER << 8;
			}
			else
			{
				dw = UPS_SOE_CODE_POWER << 8 | 1;
			}
			AfxGetMainWnd()->PostMessage(WM_USER_NOTICE_MSG, (WPARAM)m_pcb, dw);
		}
		for(int i=0; i<data_Btye_Length; i++)
		{
			m_pcb->old_dio_0x6F[i] = m_pcb->dio_0x6F[i];
		}

		data_2Btyes_Length = (bytes - 6) / 2;
		if(data_2Btyes_Length > UPS_MAX_DATA_NUM)
		{
			data_2Btyes_Length = UPS_MAX_DATA_NUM;
			TRACE("\r\n----怎么可能？ 收到的数据长度 超过最大长度？？？-----\r\n");
		}
		for(int i=0; i<data_2Btyes_Length; i++)
		{
			m_pcb->measure_0x6F[i] = pdat[MODBUS_RECV_DATA_POS + i*2 + 1];  
			m_pcb->measure_0x6F[i] <<= 8;
			m_pcb->measure_0x6F[i] |= pdat[MODBUS_RECV_DATA_POS + i*2];     
		}
		m_pcb->Measure_ToHL[24] = m_pcb->measure_0x6F[4];
		m_pcb->Measure_ToHL[25] = m_pcb->measure_0x6F[6];
		m_pcb->Measure_ToHL[26] = m_pcb->measure_0x6F[1];

		//////////////////////////////
		BYTE B_IPScale = 0x00;
		B_IPScale = m_pcb->measure_0x6F[0];  
		int i_ipScale = B_IPScale;
		short s_temp_i = 0x0000;
		s_temp_i = m_pcb->measure_0x6F[2];
		int discharge_current = 0;
		discharge_current = m_pcb->dio_0x6F[26] & 0x40;





		// 调试跟踪放电电流
		CString str_0630_debug1 = _T("");
		CString str_0630_debug2 = _T("");
		CString str_0630_debug3 = _T("");
		CString str_0630_debug4 = _T("-----------------------------------\r\n");

		if(discharge_current>0)
		{
			str_0630_debug1 = _T("DSG=1，在放电. \r\n");

			if(s_temp_i<0)
			{
				USHORT US_temp1 = 0x0000;
				USHORT US_temp2 = 0xFFFF;
				US_temp1 = m_pcb->measure_0x6F[2];
				US_temp1 = US_temp2 - US_temp1;
				int temp_I = US_temp1;
				m_pcb->Measure_ToHL[27] = temp_I * i_ipScale;
			}
			else
			{
				m_pcb->Measure_ToHL[27] = 0;
			}

			m_pcb->Measure_ToHL[28] = 0;

			
			
			
			float temp_O_I = m_pcb->Measure_ToHL[27] * 0.001f;
			str_0630_debug2.Format(_T("-------- 接收到的数据显示，放电电流是：%0.3f A "), temp_O_I);

			// 真实的放电值！！！
			str_0630_debug3.Format(_T("***【负数】*** 在放电，真实的放电值是：%d \r\n"), s_temp_i);

		}
		else
		{
			m_pcb->Measure_ToHL[27] = 0;
			if(s_temp_i < 0)
			{
				m_pcb->Measure_ToHL[28] = 0;
			}
			else
			{
				m_pcb->Measure_ToHL[28] = m_pcb->measure_0x6F[2] * i_ipScale;
			}
		}
		m_pcb->state = MODBUS_READ_CELL_DYNAMIC;
		
	


		// 输出放电电流数值  
		//SaveLogFile(str_0630_debug1);
		//SaveLogFile(str_0630_debug2);
		//SaveLogFile(str_0630_debug3);


	}
	else
	{
		TRACE("\r\n  TODO001: 接收数据里面，功能码错误？？？ \r\n");
	}
	if(m_pcb->online == 0)
	{
		AfxGetMainWnd()->PostMessage(WM_USER_NOTICE_MSG, (WPARAM)m_pcb,	UPS_SOE_CODE_ONLINE << 8 | 1);
	}
	m_pcb->online = 1;

}

void CTcpClient::SendData(BYTE *pdat, unsigned int bytes)
{
	SYSTEMTIME tt;
	char sz[40];
	int i;

	GetLocalTime(&tt);
	m_strSend = m_strServer;
	
	sprintf(sz, "TX(%02d)[%02d:%02d:%02d.%03d]: ", bytes, tt.wHour, tt.wMinute, tt.wSecond, tt.wMilliseconds);
	m_strSend += sz;
	for(i=0; i<bytes; i++)
	{
		sprintf(sz, "%02X ", pdat[i]);
		m_strSend += sz;
	}
	m_strSend += ("\r");
	OutputDebugString(m_strSend);


	// 发送的数据
	SaveLogFile(m_strSend);

	send(m_sock, (char *)pdat, bytes, 0);
}

void CTcpClient::SetupConnect()
{
	time_t tCur = time(NULL);
	if((m_tConnect + CONNECT_ACTIVE_INTERVAL) < tCur)
	{
		if(m_sock != INVALID_SOCKET)
		{
			closesocket(m_sock);
			m_sock = INVALID_SOCKET;
		}
		m_sock = socket(AF_INET, SOCK_STREAM, 0);
		WSAEventSelect(m_sock, m_evt, FD_CONNECT | FD_READ | FD_CLOSE);
		struct sockaddr_in desthost;
		desthost.sin_family = AF_INET;
		desthost.sin_port = htons(m_wPort);
		desthost.sin_addr.s_addr = inet_addr(m_strServer);
		connect(m_sock, (sockaddr*)&desthost, sizeof(sockaddr));
		m_tConnect = tCur;
		CString cs;
		cs = L"[";
		cs += m_pcb->tlcb.strIp;
		cs += L":";
		OutputDebugString(cs);
		cs.Format(L"%d] start connect %d\n", m_pcb->tlcb.port, tCur);
		OutputDebugString(cs);
	}
}

void CTcpClient::CloseConnect()
{
	CString cs;
	cs.Format(L"%d closed\n", m_sock);
	OutputDebugString(cs);
	if(m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	m_sock = INVALID_SOCKET;
	if(m_pcb->tlcb.link_state > 0)
	{
		AfxGetMainWnd()->PostMessage(WM_USER_NOTICE_MSG, (WPARAM)m_pcb,	UPS_SOE_CODE_TCP_LINK << 8);
	}
	m_pcb->tlcb.link_state = 0;
	if(m_pcb->online > 0)
	{
		AfxGetMainWnd()->PostMessage(WM_USER_NOTICE_MSG, (WPARAM)m_pcb,	UPS_SOE_CODE_ONLINE << 8);
	}
	m_pcb->online = 0;
	m_tConnect = time(NULL) - 10;
}



int CTcpClient::pack_frame(BYTE *buff, BYTE ti)
{
	if(buff != NULL)
	{
		int i = 0;
		buff[i++] = m_pcb->Slave_addr;	// buff[0] 从机地址	
		buff[i++] = ti;                 // buff[1] 功能码
		buff[i++] = 0x01;   // buff[2] 主机地址
		buff[i++] = 0x00;   // buff[3] 数据长度
		WORD cc;
		cc = SW_CRC16(buff, i);  
		buff[i++] = cc;        
		buff[i++] = cc>>8;    

		return i;
	}
	return 0;
}
