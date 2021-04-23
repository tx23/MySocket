#ifndef __COMMUNITE__H_
#define __COMMUNITE__H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>

class CNetworkServer
{
public:
	CNetworkServer();
	~CNetworkServer();

public:
	int StartNetListenServer();//socket、bind、listen
	int StopNetListenServer();
	int Accept();//accept

	int ReciveData(int iSocketID, char *szBuffer, int iReciveDataLen);
	int SendData(int iSocket, char *szDataBuffer, int iDataLen);
	char *GetClientIP(int iClientConnectHandle);
	int Close(int iClient);
	int ReleasClientIP(char *szIPName);

	void SetSvrPort(int iSvrPort, char *szIPAddress);
	int CloseClient(int iClientHandle);

private:
	int m_iPort; //端口号
	char m_szServerIP[128]; //IP
	int m_iServerSocketHandle;
};

#endif //__COMMUNITE__H_
