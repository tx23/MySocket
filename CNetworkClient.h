#ifndef _ST_COMMUNITE__H_
#define _ST_COMMUNITE__H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "task_def.h"


class	CNetworkClient{
public:
	CNetworkClient();
	~CNetworkClient();	

public:
	void SetSvrParame(char *szSvrIP,int iSvrPort);
	int  ConnectSvr();
	int  DisConnectSvr();
	
	int  ReciveData(int iSocketID,char *szBuffer,int iReciveDataLen);
	int  SendData(int iSocket,char *szDataBuffer,int iDataLen);

	int  ReciveData(char *szBuffer,int iReciveDataLen);
	int  SendData(char *szDataBuffer,int iDataLen);
private:
	int  m_iSockethandle;
	char m_szServiceIP[128];
	int  m_iServicePort;	
};
#endif //_ST_COMMUNITE__H_
