#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <typeinfo>

#include "communicate.h"

#define QUEUE 20
#define PORT 7000
#define  SERVER_TIME_OUT 5

using namespace std;

CNetworkServer::CNetworkServer()
{
	m_iPort = PORT;
	memset(m_szServerIP, 0, 128);
}

CNetworkServer::~CNetworkServer() {}

void CNetworkServer::SetSvrPort(int iSvrPort, char *szIPAddress)
{
	if (iSvrPort > 0)
		m_iPort = iSvrPort;
	else
		m_iPort = 7000;
	if (szIPAddress != NULL)
		strcpy(m_szServerIP, szIPAddress);
}

int CNetworkServer::StartNetListenServer()
{
	m_iServerSocketHandle = socket(AF_INET, SOCK_STREAM, 0);
	if (m_iServerSocketHandle == -1)
	{
		printf("create socket error\n");
		return -1;
	}
	printf("create socket success\n");

	socklen_t optlen = sizeof(struct timeval);

	struct timeval tv;

	tv.tv_sec = SERVER_TIME_OUT;
	tv.tv_usec = 0;

	setsockopt(m_iServerSocketHandle, SOL_SOCKET, SO_RCVTIMEO, &tv, optlen);//设置接收超时

	const int on = 1;

	setsockopt(m_iServerSocketHandle, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));//运行重用本地地址和端口

	// 创建一个servic
	struct sockaddr_in server_sockaddr;

	//结构体加入参数：端口和地址
	server_sockaddr.sin_family = AF_INET;//IPV4
	server_sockaddr.sin_port = htons(m_iPort);
	server_sockaddr.sin_addr.s_addr = inet_addr(m_szServerIP);
	if (bind(m_iServerSocketHandle, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1)
	{
		printf("\n,PORT:%d, ip:%d\n", m_iPort, m_szServerIP);
		printf("bind socket error\n");
		return -2;
	}

	if (listen(m_iServerSocketHandle, QUEUE) == -1)
	{
		printf("listen socket error\n");
		return -3;
	}

	printf(" m_szServerIP  %s bind m_iPort %d success\r\n", m_szServerIP, m_iPort);

	return 0;
}

int CNetworkServer::StopNetListenServer()
{
	if (m_iServerSocketHandle >= 0)
		close(m_iServerSocketHandle);

	printf("CNetworkServer::StopNetListenServer\r\n");

	return 0;
}

//成功返回非负描述字，出错返回-1
int CNetworkServer::Accept()
{
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	if (m_iServerSocketHandle < 0)
		return -1;

	int iClientHandle = accept(m_iServerSocketHandle, (struct sockaddr *)&client_addr, &length);

	if (iClientHandle < 0)
		return -1;

	return iClientHandle;
}

char *CNetworkServer::GetClientIP(int iClientConnectHandle)
{
	int iIPNameLen = -1;

	struct sockaddr_in addr_conn;
	socklen_t len = sizeof(addr_conn);
	char szPeerAddress[20];
	memset((void *)szPeerAddress, 0, sizeof(szPeerAddress));

	memset((void *)&addr_conn, 0, sizeof(addr_conn));
	if (!getpeername(iClientConnectHandle, (struct sockaddr *)&addr_conn, &len))
	{
		strcpy(szPeerAddress, inet_ntoa(addr_conn.sin_addr));
	}

	iIPNameLen = strlen(szPeerAddress) + 1;
	char *pIPAddress = new char[iIPNameLen];
	memset(pIPAddress, 0, iIPNameLen);
	strcpy(pIPAddress, szPeerAddress);

	return pIPAddress;
}

int CNetworkServer::ReleasClientIP(char *szIPName)
{
	delete[] szIPName;
	return 0;
}
int CNetworkServer::Close(int iClient)
{
	if (iClient >= 0)
		close(iClient);

	return 0;
}

int CNetworkServer::ReciveData(int iSocketID, char *szBuffer, int iReciveDataLen)
{
	int iReciveLen = 0;

	while (iReciveLen < iReciveDataLen)
	{
		int iLen = recv(iSocketID, szBuffer + iReciveLen, iReciveDataLen - iReciveLen, 0);

		if (iLen > 0)
		{
			iReciveLen += iLen;
		}
		else
		{
			break;
		}
		//printf(" recive iDataTotalLen %d irecvLen  %d iLen %d\n",iReciveDataLen,iReciveLen,iLen);
	}

	return iReciveLen;
}

int CNetworkServer::SendData(int iSocket, char *szDataBuffer, int iDataLen)
{
	int iSendLen = 0;

	while (iSendLen < iDataLen)
	{
		int iLen = send(iSocket, szDataBuffer + iSendLen, iDataLen - iSendLen, 0);
		if (iLen > 0)
		{
			iSendLen += iLen;
		}
		else
		{
			return iSendLen;
		}
	}

	return iSendLen;
}
int CNetworkServer::CloseClient(int iClientHandle)
{
	if (iClientHandle < 0)
		return -1;
	close(iClientHandle);
	return 0;
}
