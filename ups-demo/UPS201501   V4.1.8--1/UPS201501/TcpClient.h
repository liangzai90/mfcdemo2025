#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#pragma once

#include "afxmt.h"
#include <vector>
#include <WinDef.h>

#define UPS_MAX_DATA_NUM 255
#define UPS_MAX_PARA_NUM 50

#define CONNECT_INTERVAL_TIME 30
#define CONNECT_ACTIVE_INTERVAL 20



typedef struct _TCP_LINK_CONTROL_BLOCK_
{
	char strIp[30];
	unsigned int port;
	CString csName;
	unsigned int link_state;
}TCP_LINK_CB;



typedef struct _UPS_NODE_CONTROL_BLOCK_
{
	TCP_LINK_CB tlcb;
	unsigned int run_flag, state, online, tmt_cnt, Slave_addr;
	unsigned int power_index, voltage_index, volume_index;
	int measure_0x67[UPS_MAX_DATA_NUM];
	BYTE dio_0x6F[UPS_MAX_DATA_NUM], old_dio_0x6F[UPS_MAX_DATA_NUM];
	int measure_0x6F[UPS_MAX_DATA_NUM];
	int Measure_ToHL[UPS_MAX_PARA_NUM];
	float rfa;
}UPS_NODE_CB;

typedef struct _UPS_NODE_SOE_RECORD_
{
	unsigned int dpi;
	time_t tmStamp;
	wchar_t wsIP[30], wsName[128], wsNote[128];	
}UPS_SOE;

#define UPS_SOE_CODE_TCP_LINK 1
#define UPS_SOE_CODE_ONLINE 2
#define UPS_SOE_CODE_POWER 3

#define WM_USER_NOTICE_MSG (WM_USER + 601)

class CTcpClient
{
public:
	CTcpClient(UPS_NODE_CB *pcb);
	~CTcpClient(void);

protected:
	char m_strServer[100];
	WORD m_wPort;
	SOCKET m_sock;
	WSAEVENT m_evt;
	CString m_strRecv, m_strSend;
	CString m_csLogFile;
	time_t m_tConnect, m_tRx, m_tTx;
	UPS_NODE_CB *m_pcb;
	
public:
	unsigned int RecCnt_OK;
	unsigned int RecCnt_Err;
	unsigned int RepeatCRCErrCnt;
	unsigned int RebootCnt;
	wchar_t logFile_path[MAX_PATH];
	wchar_t logFile_name[MAX_PATH];
	BOOL Initialize();
	void Run();
	void CloseConnect();
	BYTE m_TxDat[10], m_RxDat[255];
	WORD m_TxByteCnt, m_RxByteCnt;
	WORD m_dwTimeoutMs;
	int set_command(BYTE ti, BYTE *user_dat, unsigned int len);	
	int SaveLogFile(const CString str_Data);
protected:
	int pack_frame(BYTE *buff, BYTE ti);
	void unpack_frame(BYTE *pdat, unsigned int bytes);
	int poll_function();
	virtual int ReceiveData();
	virtual void SendData(BYTE *pdat, unsigned int bytes);
	void OnSocketMessage();
	void SetupConnect();
	void active_link();
};

BYTE get_crc_byte(BYTE *pDat, int nBytes);

WORD get_crc16_word(BYTE *pDat, int nBytes);

unsigned int user_GetCrcWord(const unsigned char *pData, const int len);

#endif