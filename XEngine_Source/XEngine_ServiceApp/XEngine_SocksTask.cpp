#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2021/12/02  17:08:19
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_SocksTask.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_SocksTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     Socks任务处理代码
//    History:
*********************************************************************/
bool XEngine_SocksTask_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nLen = 4096;
	XCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	ENUM_RFCCOMPONENTS_PROXY_STATUS enSocksStatus;
	if (!ProxyProtocol_SocksCore_GetStatus(lpszClientAddr, &enSocksStatus))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,获取状态失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
		return false;
	}
	//获取到的状态来处理相对应的事件
	if (ENUM_RFCCOMPONENTS_PROXY_STATUS_CREATE == enSocksStatus)
	{
		int nListCount = 0;
		ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH enListAuths[6];
		if (!ProxyProtocol_SocksCore_ParseAuth(lpszClientAddr, lpszMsgBuffer, nMsgLen, enListAuths, &nListCount))
		{
			ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_NOAUTH);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,解析验证协议失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
			return false;
		}
		bool bFoundAuth = false;
		XCHAR tszAuthBuffer[MAX_PATH];
		memset(tszAuthBuffer, '\0', MAX_PATH);
		
		//客户端支持的验证类型
		for (int i = 0; i < nListCount; i++)
		{
			XCHAR tszTmpBuffer[64];
			memset(tszTmpBuffer, '\0', sizeof(tszTmpBuffer));

			_xstprintf(tszTmpBuffer, _X("%d"), enListAuths[i]);
			_tcsxcat(tszAuthBuffer, tszTmpBuffer);
			_tcsxcat(tszAuthBuffer, _X(" "));
			//服务器是否支持
			if ((ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_USERPASS == enListAuths[i]) || (ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_ANONYMOUS == enListAuths[i]))
			{
				bFoundAuth = true;
			}
		}
		if (!bFoundAuth)
		{
			ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_NOAUTH);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,解析验证协议成功,但是验证类型不支持,客户端的支持验证:%s,服务器支持的验证:%d"), lpszClientAddr, tszAuthBuffer, st_ServiceConfig.st_XAuth.bAuth);
			return false;
		}
		ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, st_ServiceConfig.st_XAuth.bAuth);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
		//如果是匿名,不会有ENUM_RFCCOMPONENTS_PROXY_STATUS_AUTH步骤
		if (ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_ANONYMOUS == st_ServiceConfig.st_XAuth.bAuth)
		{
			//跳过
			ProxyProtocol_SocksCore_SetStatus(lpszClientAddr, ENUM_RFCCOMPONENTS_PROXY_STATUS_USER);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SOCKS客户端:%s,解析验证协议成功,支持的验证类型:%s,使用的验证类型:%d"), lpszClientAddr, tszAuthBuffer, st_ServiceConfig.st_XAuth.bAuth);
	}
	else if (ENUM_RFCCOMPONENTS_PROXY_STATUS_AUTH == enSocksStatus)
	{
		XCHAR tszUserName[MAX_PATH];
		XCHAR tszUserPass[MAX_PATH];
		ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH enProxyAuth;

		memset(tszUserName, '\0', MAX_PATH);
		memset(tszUserPass, '\0', MAX_PATH);

		if (!ProxyProtocol_SocksCore_ParseUser(lpszClientAddr, lpszMsgBuffer, nMsgLen, &enProxyAuth, tszUserName, tszUserPass))
		{
			ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, XENGINE_RFCCOMPONENT_PROXY_SOCKS_RESPONSE_FAILED);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,用户登录验证失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
			return false;
		}
		if (enProxyAuth != ENUM_RFCCOMPONENTS_PROXYSOCKS_AUTH_USERPASS)
		{
			ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, XENGINE_RFCCOMPONENT_PROXY_SOCKS_RESPONSE_FAILED);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,用户登录验证失败,客户端发送的验证类型不支持"), lpszClientAddr);
			return false;
		}
		int nHTTPCode = 0;
		ModuleProtocol_Packet_Auth(tszMsgBuffer, &nLen, tszUserName, tszUserPass);
		if (!APIClient_Http_Request(_X("POST"), st_ServiceConfig.st_XAuth.tszAuthUrl, tszMsgBuffer, &nHTTPCode))
		{
			ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, XENGINE_RFCCOMPONENT_PROXY_SOCKS_RESPONSE_FAILED);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,用户登录验证失败,用户名:%s,或者密码:%s 不正确"), lpszClientAddr, tszUserPass, tszUserPass);
			return false;
		}
		ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, XENGINE_RFCCOMPONENT_PROXY_SOCKS_RESPONSE_SUCCESS);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SOCKS客户端:%s,用户登录验证成功,使用的验证类型:%d,用户名:%s"), lpszClientAddr, enProxyAuth, tszUserName);
	}
	else if (ENUM_RFCCOMPONENTS_PROXY_STATUS_USER == enSocksStatus)
	{
		int nPort = 0;
		XCHAR tszClientAddr[1024];
		PROXYPROTOCOL_CLIENTINFO st_ProxyClient;
		ENUM_RFCCOMPONENTS_PROXYSOCKS_COMMAND enCommand;
		ENUM_RFCCOMPONENTS_PROXYSOCKS_IPADDR enIPType;

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(&st_ProxyClient, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));
		if (!ProxyProtocol_SocksCore_ParseConnect(lpszClientAddr, lpszMsgBuffer, nMsgLen, &enCommand, &enIPType, tszClientAddr, &nPort))
		{
			ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, XENGINE_RFCCOMPONENT_PROXY_SOCKS_RESPONSE_NETWORK);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,用户连接失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
			return false;
		}
		//命令类型
		if (ENUM_RFCCOMPONENTS_PROXYSOCKS_COMMAND_CONNECT == enCommand)
		{
			//连接类型,IP还是域名,没有IPV6环境.不好测试
			if (ENUM_RFCCOMPONENTS_PROXYSOCKS_IPADDR_IPV4 == enIPType)
			{
				if (!XClient_TCPSelect_InsertEx(xhSocksClient, &st_ProxyClient.xhClient, tszClientAddr, nPort))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,创建客户端连接失败,连接到服务器:%s:%d,错误:%lX"), lpszClientAddr, tszClientAddr, nPort, ProxyProtocol_GetLastError());
					return false;
				}
			}
			else
			{
				int nListCount = 0;
				XCHAR** ppszListAddr;
				ENUM_APIHELP_DOMAINTYPE enDomainType;
				APIHELP_DOMAIN st_APIUrl;

				memset(&st_APIUrl, '\0', sizeof(APIHELP_DOMAIN));

				APIHelp_Domain_GetInfo(tszClientAddr, &st_APIUrl, &enDomainType);
				memset(tszClientAddr, '\0', sizeof(tszClientAddr));
				if (_tcsxlen(st_APIUrl.tszSubDomain) > 0)
				{
					_xstprintf(tszClientAddr, _X("%s.%s"), st_APIUrl.tszSubDomain, st_APIUrl.tszMainDomain);
				}
				else
				{
					_xstprintf(tszClientAddr, _X("%s"), st_APIUrl.tszMainDomain);
				}

				if (!NetXApi_Socket_DomainToAddr(tszClientAddr, &ppszListAddr, &nListCount))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,解析域名失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
					return false;
				}
				memset(tszClientAddr, '\0', sizeof(tszClientAddr));
				_tcsxcpy(tszClientAddr, ppszListAddr[0]);   //随便选择一个IP地址
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListAddr, nListCount);
				if (!XClient_TCPSelect_InsertEx(xhSocksClient, &st_ProxyClient.xhClient, tszClientAddr, nPort))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,创建客户端连接失败,连接到服务器:%s:%d,错误:%lX"), lpszClientAddr, tszClientAddr, nPort, ProxyProtocol_GetLastError());
					return false;
				}
			}
		}
		//配置客户端信息
		st_ProxyClient.enStatus = ENUM_RFCCOMPONENTS_PROXY_STATUS_FORWARD;
		_tcsxcpy(st_ProxyClient.tszIPAddr, lpszClientAddr);
		ProxyProtocol_SocksCore_SetInfo(lpszClientAddr, &st_ProxyClient, sizeof(PROXYPROTOCOL_CLIENTINFO));
		//回复结果
		ProxyProtocol_SocksCore_HdrPacket(lpszClientAddr, tszMsgBuffer, &nLen, XENGINE_RFCCOMPONENT_PROXY_SOCKS_RESPONSE_SUCCESS);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SOCKS客户端:%s,用户登录验证成功,连接到地址:%s:%d"), lpszClientAddr, tszClientAddr, nPort);
	}
	else
	{
		//ENUM_RFCCOMPONENTS_PROXY_STATUS_FORWARD 转发
		PROXYPROTOCOL_CLIENTINFO st_ProxyClient;
		memset(&st_ProxyClient, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));
		if (ProxyProtocol_SocksCore_GetInfo(lpszClientAddr, &st_ProxyClient))
		{
			XClient_TCPSelect_SendEx(xhSocksClient, st_ProxyClient.xhClient, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Socks客户端:%s,转发数据失败,可能与服务器的连接已经关闭,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
		}
	}
	return true;
}

void CALLBACK XEngine_Socks_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_NETCLIENT_TCPEVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam)
{
	int nListCount = 0;
	PROXYPROTOCOL_CLIENTINFO** ppSt_ClientList;
	ProxyProtocol_SocksCore_GetList((XPPPMEM)&ppSt_ClientList, &nListCount, sizeof(PROXYPROTOCOL_CLIENTINFO));
	for (int i = 0; i < nListCount; i++)
	{
		if (xhClient == ppSt_ClientList[i]->xhClient)
		{
			if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_RECV == enTCPClientEvents)
			{
				if (!XEngine_Network_Send(ppSt_ClientList[i]->tszIPAddr, lpszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_SOCKS))
				{
					SocketOpt_HeartBeat_ForceOutAddrEx(xhSocksHeart, ppSt_ClientList[i]->tszIPAddr);
				}
			}
			else if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_CLOSE == enTCPClientEvents)
			{
				//退出处理
				SocketOpt_HeartBeat_ForceOutAddrEx(xhSocksHeart, ppSt_ClientList[i]->tszIPAddr);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Socks客户端:%s,离开服务器,客户端主动断开"), ppSt_ClientList[i]->tszIPAddr);
			}
			break;
		}
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
}