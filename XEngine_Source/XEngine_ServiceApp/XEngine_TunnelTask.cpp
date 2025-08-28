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
	ENUM_PROXY_SESSION_CLIENT_STATUS enClientStatus;
	ModuleSession_Tunnel_GetStatus(lpszClientAddr, &enClientStatus);
	//创建成功就只需要转发
	if (ENUM_PROXY_SESSION_CLIENT_CREATE == enClientStatus)
	{
		int nLen = 0;
		int nIPPort = 0;
		bool bProxy = true;
		XCHAR tszIPAddr[1024];
		XCHAR tszAuthInfo[XPATH_MAX];
		XCHAR tszMsgBuffer[XPATH_MAX];

		memset(tszIPAddr, '\0', sizeof(tszIPAddr));
		memset(tszAuthInfo, '\0', XPATH_MAX);
		memset(tszMsgBuffer, '\0', XPATH_MAX);
		if (!ModuleSession_Tunnel_Packet(lpszClientAddr, lpszMsgBuffer, nMsgLen, tszMsgBuffer, &nLen))
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, ERROR_XENGINE_PROXY_PROTOCOL_FORMAT);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("Tunnel客户端:%s,解析协议不成功,可能因为数据不完整,需要等待完整包,错误码:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
			return false;
		}
		if (!ProxyProtocol_TunnelCore_Parse(tszMsgBuffer, nLen, tszIPAddr, &nIPPort, tszAuthInfo, &bProxy))
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, ERROR_XENGINE_PROXY_PROTOCOL_SERVER);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,解析协议失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
			return false;
		}

		XCHAR tszConnectAddr[128];
		memset(tszConnectAddr, '\0', sizeof(tszConnectAddr));
		//是否为IP地址
		if (APIAddr_IPAddr_IsIPV4Addr(tszIPAddr))
		{
			_tcsxcpy(tszConnectAddr, tszIPAddr);
		}
		else
		{
			int nListCount = 0;
			XCHAR** ppszListAddr;
			ENUM_NETHELP_APIADDR_DOMAIN_TYPE enDomainType;
			APIADDR_DOMAIN st_APIUrl;

			memset(&st_APIUrl, '\0', sizeof(APIADDR_DOMAIN));

			APIAddr_Domain_GetInfo(tszIPAddr, &st_APIUrl, &enDomainType);
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
				ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, ERROR_XENGINE_PROXY_PROTOCOL_DOMAIN);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,解析域名:%s,失败,错误:%lX"), lpszClientAddr, tszIPAddr, ProxyProtocol_GetLastError());
				return false;
			}
			_tcsxcpy(tszConnectAddr, ppszListAddr[0]);   //随便选择一个IP地址
			BaseLib_Memory_Free((XPPPMEM)&ppszListAddr, nListCount);
		}
		XNETHANDLE xhClient = 0;
		if (!XClient_TCPSelect_InsertEx(xhTunnelClient, &xhClient, tszConnectAddr, nIPPort))
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, ERROR_XENGINE_PROXY_PROTOCOL_SERVER);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,创建客户端连接失败,连接到服务器:%s:%d,错误:%lX"), lpszClientAddr, tszConnectAddr, nIPPort, ProxyProtocol_GetLastError());
			return false;
		}
		//设置属于
		ModuleSession_Tunnel_SetStatus(lpszClientAddr, ENUM_PROXY_SESSION_CLIENT_FORWARD);
		ModuleSession_Tunnel_SetInfo(lpszClientAddr, xhClient, tszConnectAddr);
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
			XClient_TCPSelect_SendEx(xhTunnelClient, xhClient, lpszMsgBuffer, nMsgLen);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Tunnel客户端:%s,创建隧道代理服务成功,连接到服务器:%s:%d,代理模式:%s"), lpszClientAddr, tszConnectAddr, nIPPort, bProxy ? "代理" : "直连");
	}
	else
	{
		XNETHANDLE xhClient = 0;
		if (ModuleSession_Tunnel_GetInfo(lpszClientAddr, &xhClient))
		{
			//转发数据
			XClient_TCPSelect_SendEx(xhTunnelClient, xhClient, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			SocketOpt_HeartBeat_ForceOutAddrEx(xhSocksHeart, lpszClientAddr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,转发数据失败,可能与服务器的连接已经关闭,错误:%lX"), lpszClientAddr, ModuleSession_GetLastError());
		}
	}
	return true;
}

void XCALLBACK XEngine_Tunnel_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam)
{
	XCHAR tszClientAddr[XPATH_MIN] = {};
	if (ModuleSession_Tunnel_GetAddrForHandle(xhClient, tszClientAddr))
	{
		if (ENUM_XCLIENT_SOCKET_EVENT_RECV == enTCPClientEvents)
		{
			if (!XEngine_Network_Send(tszClientAddr, lpszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL))
			{
				SocketOpt_HeartBeat_ForceOutAddrEx(xhTunnelHeart, tszClientAddr);
			}
		}
		else if (ENUM_XCLIENT_SOCKET_EVENT_CLOSE == enTCPClientEvents)
		{
			//退出处理
			SocketOpt_HeartBeat_ForceOutAddrEx(xhTunnelHeart, tszClientAddr);
			//XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Tunnel客户端:%s,离开服务器,客户端主动断开"), tszClientAddr);
		}
	}
	else
	{
#if XENGINE_VERSION_KERNEL >= 9 && XENGINE_VERSION_MAIN >= 27
		XClient_TCPSelect_DeletePostEx(xhToken, xhClient);
#endif
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:未知,句柄:%lld 没有找到映射的客户端"), xhClient);
	}
}