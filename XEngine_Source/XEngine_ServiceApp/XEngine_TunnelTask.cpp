#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2021/12/02  17:15:57
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_TunnelTask.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_TunnelTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
bool XEngine_TunnelTask_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	PROXYPROTOCOL_CLIENTINFO st_ProxyClient;
	memset(&st_ProxyClient, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));

	ModuleSession_Tunnel_GetInfo(lpszClientAddr, &st_ProxyClient);
	//创建成功就只需要转发
	if (ENUM_PROXY_SESSION_SOCKS_STATUS_CREATE == st_ProxyClient.enStatus)
	{
		int nLen = 0;
		int nIPPort = 0;
		bool bProxy = true;
		XCHAR tszIPAddr[1024];
		XCHAR tszAuthInfo[MAX_PATH];
		XCHAR tszMsgBuffer[MAX_PATH];

		memset(tszIPAddr, '\0', sizeof(tszIPAddr));
		memset(tszAuthInfo, '\0', MAX_PATH);
		memset(tszMsgBuffer, '\0', MAX_PATH);
		if (!ModuleSession_Tunnel_Packet(lpszClientAddr, lpszMsgBuffer, nMsgLen, tszMsgBuffer, &nLen))
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, 400);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,解析协议失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
			return false;
		}
		ProxyProtocol_TunnelCore_Parse(tszMsgBuffer, nLen, tszIPAddr, &nIPPort, tszAuthInfo, &bProxy);

		XCHAR tszConnectAddr[128];
		memset(tszConnectAddr, '\0', sizeof(tszConnectAddr));
		//是否为IP地址
		if (BaseLib_OperatorIPAddr_IsIPV4Addr(tszIPAddr))
		{
			_tcsxcpy(tszConnectAddr, tszIPAddr);
		}
		else
		{
			int nListCount = 0;
			XCHAR** ppszListAddr;
			ENUM_APIHELP_DOMAINTYPE enDomainType;
			APIHELP_DOMAIN st_APIUrl;

			memset(&st_APIUrl, '\0', sizeof(APIHELP_DOMAIN));

			APIHelp_Domain_GetInfo(tszIPAddr, &st_APIUrl, &enDomainType);
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));
			if (_tcsxlen(st_APIUrl.tszSubDomain) > 0)
			{
				_xstprintf(tszIPAddr, _X("%s.%s"), st_APIUrl.tszSubDomain, st_APIUrl.tszMainDomain);
			}
			else
			{
				_xstprintf(tszIPAddr, _X("%s"), st_APIUrl.tszMainDomain);
			}

			if (!XSocket_Api_DomainToAddr(tszIPAddr, &ppszListAddr, &nListCount))
			{
				ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, 500);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,解析域名:%s,失败,错误:%lX"), lpszClientAddr, tszIPAddr, ProxyProtocol_GetLastError());
				return false;
			}
			_tcsxcpy(tszConnectAddr, ppszListAddr[0]);   //随便选择一个IP地址
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListAddr, nListCount);
		}
		if (!XClient_TCPSelect_InsertEx(xhTunnelClient, &st_ProxyClient.xhClient, tszConnectAddr, nIPPort))
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, 500);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,创建客户端连接失败,连接到服务器:%s:%d,错误:%lX"), lpszClientAddr, tszConnectAddr, nIPPort, ProxyProtocol_GetLastError());
			return false;
		}
		//设置属于
		st_ProxyClient.enStatus = ENUM_PROXY_SESSION_SOCKS_STATUS_FORWARD;
		_tcsxcpy(st_ProxyClient.tszIPAddr, lpszClientAddr);
		ModuleSession_Tunnel_SetInfo(lpszClientAddr, &st_ProxyClient, sizeof(PROXYPROTOCOL_CLIENTINFO));
		//判断是代理还是非代理协议
		if (bProxy)
		{
			//客户端->代理服务->客户端
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
		}
		else
		{
			//非代理协议直接发送数据,客户端->代理服务->服务器
			XClient_TCPSelect_SendEx(xhTunnelClient, st_ProxyClient.xhClient, lpszMsgBuffer, nMsgLen);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Tunnel客户端:%s,创建隧道代理服务成功,连接到服务器:%s:%d,代理模式:%s"), lpszClientAddr, tszConnectAddr, nIPPort, bProxy ? "代理" : "直连");
	}
	else
	{
		//转发数据
		XClient_TCPSelect_SendEx(xhTunnelClient, st_ProxyClient.xhClient, lpszMsgBuffer, nMsgLen);
	}
	return true;
}

void CALLBACK XEngine_Tunnel_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam)
{
	int nListCount = 0;
	PROXYPROTOCOL_CLIENTINFO** ppSt_ClientList;
	ModuleSession_Tunnel_GetList((XPPPMEM)&ppSt_ClientList, &nListCount, sizeof(PROXYPROTOCOL_CLIENTINFO));
	for (int i = 0; i < nListCount; i++)
	{
		if (xhClient == ppSt_ClientList[i]->xhClient)
		{
			if (ENUM_XCLIENT_SOCKET_EVENT_RECV == enTCPClientEvents)
			{
				if (!XEngine_Network_Send(ppSt_ClientList[i]->tszIPAddr, lpszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL))
				{
					SocketOpt_HeartBeat_ForceOutAddrEx(xhTunnelHeart, ppSt_ClientList[i]->tszIPAddr);
				}
			}
			else if (ENUM_XCLIENT_SOCKET_EVENT_CLOSE == enTCPClientEvents)
			{
				//退出处理
				SocketOpt_HeartBeat_ForceOutAddrEx(xhTunnelHeart, ppSt_ClientList[i]->tszIPAddr);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Tunnel客户端:%s,离开服务器,客户端主动断开"), ppSt_ClientList[i]->tszIPAddr);
			}
			break;
		}
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
}