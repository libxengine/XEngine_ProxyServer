#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_ProxyProtocol.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"

//需要优先配置XEngine
//WINDOWS支持VS2019 x86 debug 编译调试
//linux使用下面的命令编译
//g++ -std=c++17 -Wall -g APPClient_TunnelExample.cpp -o APPClient_TunnelExample.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_ProxyProtocol
int main(int argc, char** argv)
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	SOCKET m_Socket;
	LPCTSTR lpszServiceAddr = _T("127.0.0.1");
	if (!XClient_TCPSelect_Create(&m_Socket, lpszServiceAddr, 5401))
	{
		printf("连接失败！错误:%lX\n", XClient_GetLastError());
		return 0;
	}
	printf("连接成功！\n");
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	ProxyProtocol_TunnelClient_Packet(tszMsgBuffer, &nMsgLen, "www.xyry.org:80", "MTIzMTIzYWE6MTIzMTIz");
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen, FALSE))
	{
		printf("接受数据失败！\n");
		return 0;
	}
	printf("接受服务器返回数据,内容:%s\n", tszMsgBuffer);

	LPCTSTR lpszGetHttp = _T("GET / HTTP/1.1\r\nHost: www.xyry.org\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36\r\nAccept: text/html\r\n\r\n");
	if (!XClient_TCPSelect_SendMsg(m_Socket, lpszGetHttp, _tcslen(lpszGetHttp)))
	{
		printf("发送投递失败！\n");
		return 0;
	}

	while (1)
	{
		nMsgLen = 2048;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen, FALSE))
		{
			break;
		}
		printf("接受服务器返回数据,内容:%s\n", tszMsgBuffer);
	}

	XClient_TCPSelect_Close(m_Socket);
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}