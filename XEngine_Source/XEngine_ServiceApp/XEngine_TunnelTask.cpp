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
BOOL XEngine_TunnelTask_Handle(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	PROXYPROTOCOL_CLIENTINFO st_ProxyClient;
	memset(&st_ProxyClient, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));

	ProxyProtocol_TunnelCore_GetInfo(lpszClientAddr, &st_ProxyClient);
	//创建成功就只需要转发
	if (ENUM_RFCCOMPONENTS_PROXY_STATUS_CREATE == st_ProxyClient.enStatus)
	{
		int nLen = 0;
		int nIPPort = 0;
		BOOL bProxy = TRUE;
		TCHAR tszIPAddr[1024];
		TCHAR tszAuthInfo[MAX_PATH];
		TCHAR tszMsgBuffer[MAX_PATH];

		memset(tszIPAddr, '\0', sizeof(tszIPAddr));
		memset(tszAuthInfo, '\0', MAX_PATH);
		memset(tszMsgBuffer, '\0', MAX_PATH);
		if (!ProxyProtocol_TunnelCore_Parse(lpszClientAddr, lpszMsgBuffer, nMsgLen, tszIPAddr, &nIPPort, tszAuthInfo, &bProxy))
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, 400);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("Tunnel客户端:%s,解析协议失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
			return FALSE;
		}
		TCHAR tszConnectAddr[128];
		memset(tszConnectAddr, '\0', sizeof(tszConnectAddr));
		//是否为IP地址
		if (BaseLib_OperatorIPAddr_IsIPV4Addr(tszIPAddr))
		{
			_tcscpy(tszConnectAddr, tszIPAddr);
		}
		else
		{
			int nListCount = 0;
			CHAR** ppszListAddr;
			ENUM_APIHELP_DOMAINTYPE enDomainType;
			APIHELP_DOMAIN st_APIUrl;

			memset(&st_APIUrl, '\0', sizeof(APIHELP_DOMAIN));

			APIHelp_Domain_GetInfo(tszIPAddr, &st_APIUrl, &enDomainType);
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));
			_stprintf(tszIPAddr, _T("%s.%s"), st_APIUrl.tszSubDomain, st_APIUrl.tszMainDomain);

			if (!NetXApi_Socket_DomainToAddr(tszIPAddr, &ppszListAddr, &nListCount))
			{
				ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, 500);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("Tunnel客户端:%s,解析域名失败,错误:%lX"), lpszClientAddr, ProxyProtocol_GetLastError());
				return FALSE;
			}
			_tcscpy(tszConnectAddr, ppszListAddr[0]);   //随便选择一个IP地址
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListAddr, nListCount);
		}
		if (!XClient_TCPSelect_Create(&st_ProxyClient.hSocket, tszConnectAddr, nIPPort, AF_INET, 1))
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen, 500);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("Tunnel客户端:%s,创建客户端连接失败,连接到服务器:%s:%d,错误:%lX"), lpszClientAddr, tszConnectAddr, nIPPort, ProxyProtocol_GetLastError());
			return FALSE;
		}
		std::thread pSTDThread(XEngine_TunnelTask_Thread, lpszClientAddr);
		pSTDThread.detach();
		st_ProxyClient.enStatus = ENUM_RFCCOMPONENTS_PROXY_STATUS_FORWARD;
		_tcscpy(st_ProxyClient.tszIPAddr, lpszClientAddr);
		ProxyProtocol_TunnelCore_SetInfo(lpszClientAddr, &st_ProxyClient, sizeof(PROXYPROTOCOL_CLIENTINFO));
		//判断是代理还是非代理协议
		if (bProxy)
		{
			ProxyProtocol_TunnelCore_Packet(tszMsgBuffer, &nLen);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
		}
		else
		{
			//非代理协议直接发送数据
			XClient_TCPSelect_SendMsg(st_ProxyClient.hSocket, lpszMsgBuffer, nMsgLen);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Tunnel客户端:%s,创建隧道代理服务成功,连接到服务器:%s:%d"), lpszClientAddr, tszConnectAddr, nIPPort);
	}
	else
	{
		//转发数据
		XClient_TCPSelect_SendMsg(st_ProxyClient.hSocket, lpszMsgBuffer, nMsgLen);
	}
	return TRUE;
}
XHTHREAD CALLBACK XEngine_TunnelTask_Thread(LPCTSTR lpszClientAddr)
{
	TCHAR tszClientAddr[128];
	PROXYPROTOCOL_CLIENTINFO st_ProxyClient;

	memset(tszClientAddr, '\0', sizeof(tszClientAddr));
	_tcscpy(tszClientAddr, lpszClientAddr);
	while (1)
	{
		int nMsgLen = 4096;
		TCHAR tszMsgBuffer[4096];

		memset(&st_ProxyClient, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));
		if (!ProxyProtocol_TunnelCore_GetInfo(tszClientAddr, &st_ProxyClient))
		{
			break;
		}
		if (!XClient_TCPSelect_RecvMsg(st_ProxyClient.hSocket, tszMsgBuffer, &nMsgLen, FALSE))
		{
			break;
		}
		XEngine_Network_Send(st_ProxyClient.tszIPAddr, tszMsgBuffer, nMsgLen, XENGINE_CLIENT_NETTYPE_TUNNEL);
	}
	XEngine_Network_Close(tszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, XENGINE_CLIENT_CLOSE_SERVICE);
	return 0;
}