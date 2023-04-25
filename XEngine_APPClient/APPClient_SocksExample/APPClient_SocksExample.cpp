#ifdef _MSC_BUILD
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
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"

//需要优先配置XEngine
//WINDOWS支持VS2022 x86 debug 编译调试
//linux使用下面的命令编译
//g++ -std=c++17 -Wall -g APPClient_SocksExample.cpp -o APPClient_SocksExample.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Client -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_ProxyProtocol
int main(int argc, char** argv)
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XSOCKET m_Socket;
	LPCXSTR lpszServiceAddr = _X("127.0.0.1");
	if (!XClient_TCPSelect_Create(&m_Socket, lpszServiceAddr, 5400))
	{
		printf("连接失败！错误:%lX\n", XClient_GetLastError());
		return 0;
	}
	printf("连接成功！\n");

	int nMsgLen = 2048;
	XCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	int nCount = 2;
	ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH enAuths[2] = { ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_ANONYMOUS ,ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_USERPASS };
	//////////////////////////////////////////////////////////////////////////验证
	ProxyProtocol_SocksClient_AuthPacket(tszMsgBuffer, &nMsgLen, enAuths, nCount);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
	{
		printf("接受数据失败！\n");
		return 0;
	}
	ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH enServerAuth;
	ProxyProtocol_SocksClient_AuthParse(tszMsgBuffer, &enServerAuth);
	printf("接受服务器返回数据,内容:%d\n", enServerAuth);
	//////////////////////////////////////////////////////////////////////////登录 匿名登录注释下面的代码
	ProxyProtocol_SocksClient_LoginPacket(tszMsgBuffer, &nMsgLen, "123123aa", "123123");
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
	{
		printf("接受数据失败！\n");
		return 0;
	}
	if (ProxyProtocol_SocksClient_LoginParse(tszMsgBuffer))
	{
		printf("login ok\n");
	}
	else
	{
		printf("login failed\n");
	}
	//////////////////////////////////////////////////////////////////////////连接
	ProxyProtocol_SocksClient_ConnectPacket(tszMsgBuffer, &nMsgLen, "www.xyry.org", 80, ENUM_RFCCOMPONENTS_PROXYSOCKS_COMMAND_CONNECT, ENUM_RFCCOMPONENTS_PROXYSOCKS_IPADDR_DOMAIN);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
	{
		printf("接受数据失败！\n");
		return 0;
	}
	int nCode = 0;
	ProxyProtocol_SocksClient_ConnectParse(tszMsgBuffer, &nCode);
	//请求
	LPCXSTR lpszGetHttp = _X("GET / HTTP/1.1\r\nHost: www.xyry.org\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36\r\nAccept: text/html\r\n\r\n");
	if (!XClient_TCPSelect_SendMsg(m_Socket, lpszGetHttp, _tcsxlen(lpszGetHttp)))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	while (1)
	{
		nMsgLen = 2048;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
		{
			break;
		}
		printf("接受服务器返回数据,内容:%s\n", tszMsgBuffer);
	}

	XClient_TCPSelect_Close(m_Socket);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}