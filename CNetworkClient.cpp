#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include "communicate.h"
#define QUEUE 20

using namespace std;


CNetworkClient::CNetworkClient()
{
	memset(m_szServiceIP,0,sizeof(m_szServiceIP));
}

CNetworkClient::~CNetworkClient(){}

void CNetworkClient::SetSvrParame(char *szSvrIP,int iSvrPort)
{
	if(szSvrIP == NULL || iSvrPort < 0)
		return;
	memset(m_szServiceIP,0,sizeof(m_szServiceIP));
	strcpy(m_szServiceIP,szSvrIP);
	m_iServicePort = iSvrPort;
}

int  CNetworkClient::DisConnectSvr()
{
	if(m_iSockethandle >=0)
		close(m_iSockethandle);
	return 0;
}

int CNetworkClient::ConnectSvr()
{
	//定义sockfd
	m_iSockethandle = socket(AF_INET, SOCK_STREAM, 0);

	//定义sockaddr_in
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(m_iServicePort);  ///服务器端口
	servaddr.sin_addr.s_addr = inet_addr(m_szServiceIP);  ///服务器ip
	printf("ip address %s ,port %d\r\n",m_szServiceIP,m_iServicePort);

	//连接服务器，成功返回0，错误返回-1
	if (connect(m_iSockethandle, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		printf("connect server err\n");
	    return -1;
	}
	printf("connect server success\n");
	return 0;
}

int  CNetworkClient::ReciveData(char *szBuffer,int iReciveDataLen)
{
	int iReciveLen = 0;
	
	while( iReciveLen < iReciveDataLen)
	{
		int iLen = recv(m_iSockethandle, szBuffer + iReciveLen, iReciveDataLen - iReciveLen, 0);		
		if(iLen > 0)
			iReciveLen += iLen; 
		else
			break;
	}
	
	return iReciveLen;
}

int  CNetworkClient::ReciveData(int iSocketID,char *szBuffer,int iReciveDataLen)
{
	int iReciveLen = 0;
	
	while( iReciveLen < iReciveDataLen)
	{
		int iLen = recv(iSocketID, szBuffer + iReciveLen, iReciveDataLen - iReciveLen, 0);		
		if(iLen > 0)
			iReciveLen += iLen; 
		else
			break;
	}
	
	return iReciveLen;
}

int  CNetworkClient::SendData(char *szDataBuffer,int iDataLen)
{	
	int iHaveSendLen = 0;

	while(iHaveSendLen < iDataLen)
	{
		int iLen = send(m_iSockethandle, szDataBuffer + iHaveSendLen, iDataLen - iHaveSendLen, 0);			
		if(iLen > 0)
			iHaveSendLen += iLen;
		else
			break;
	}
	return iHaveSendLen;
}

int  CNetworkClient::SendData(int iSocket,char *szDataBuffer,int iDataLen)
{
	int iHaveSendLen = 0;

	while(iHaveSendLen < iDataLen)
	{
		int iLen = send(iSocket, szDataBuffer + iHaveSendLen, iDataLen - iHaveSendLen, 0);			
		if(iLen > 0)
			iHaveSendLen += iLen;
		else
			break;
	}
	return iHaveSendLen;
}


