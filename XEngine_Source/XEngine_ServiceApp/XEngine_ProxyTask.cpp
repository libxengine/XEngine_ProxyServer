#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2025/01/20  10:59:05
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp\XEngine_ProxyTask.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_ProxyTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     代理服务协议
//    History:
*********************************************************************/
bool XEngine_Proxy_Connect(LPCXSTR lpszClientAddr)
{
	int nSrcPort = 0;
	int nDstPort = 0;
	XNETHANDLE xhClient = 0;
	XCHAR tszIPAddr[128] = {};
	XCHAR tszSrcIPAddr[128] = {};
	XCHAR tszDstIPAddr[128] = {};

	_tcsxcpy(tszSrcIPAddr, lpszClientAddr);
	APIAddr_IPAddr_SegAddr(tszSrcIPAddr, &nSrcPort);

	bool bFound = false;
	auto stl_ListIterator = st_ServiceConfig.st_XProxy.pStl_ListRuleAddr->begin();
	for (; stl_ListIterator != st_ServiceConfig.st_XProxy.pStl_ListRuleAddr->end(); stl_ListIterator++)
	{
		XCHAR tszTmpIPAddr[128] = {};
		_stxscanf(stl_ListIterator->c_str(), _X("%[^-]-%s"), tszTmpIPAddr, tszDstIPAddr);
		//ip
		if (0 == _tcsxnicmp(tszSrcIPAddr, tszTmpIPAddr, _tcsxlen(tszSrcIPAddr)))
		{
			bFound = true;
			_tcsxcpy(tszIPAddr, tszDstIPAddr);
			APIAddr_IPAddr_SegAddr(tszDstIPAddr, &nDstPort);
			break;
		}
	}

	if (!bFound)
	{
		_tcsxcpy(tszDstIPAddr, st_ServiceConfig.st_XProxy.tszDefaultAddr);
		APIAddr_IPAddr_SegAddr(tszDstIPAddr, &nDstPort);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Proxy客户端:%s,代理转未命中,使用默认地址:%s:%d"), lpszClientAddr, tszDstIPAddr, nDstPort);
	}
	if (!XClient_TCPSelect_InsertEx(xhProxyClient, &xhClient, tszDstIPAddr, nDstPort, false))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Proxy客户端:%s,连接到转发代理服务器失败,服务地址:%s:%d,错误码:%lX"), lpszClientAddr, tszDstIPAddr, nDstPort, XClient_GetLastError());
		return false;
	}
	SocketOpt_HeartBeat_InsertAddrEx(xhProxyHeart, lpszClientAddr);
	ModuleSession_Proxy_Insert(lpszClientAddr, tszIPAddr, xhClient);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Proxy客户端:%s,连接到服务器:%s:%d 成功"), lpszClientAddr, tszDstIPAddr, nDstPort);
	return true;
}
void CALLBACK XEngine_Proxy_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	SESSION_FORWARD st_ProxyInfo = {};
	if (!ModuleSession_Proxy_GetForToken(xhClient, &st_ProxyInfo))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Proxy客户端:%lld,获取转发地址信息失败,原始地址:%s,目标地址:%s,错误码:%lX"), xhClient, st_ProxyInfo.tszSrcAddr, st_ProxyInfo.tszDstAddr, ModuleSession_GetLastError());
		return;
	}
	XEngine_Network_Send(st_ProxyInfo.tszSrcAddr, lpszMsgBuffer, nMsgLen, XENGINE_CLIENT_NETTYPE_PROXY);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("Proxy客户端:%lld,转发数据成功,原始地址:%s,目标地址:%s,大小:%d"), xhClient, st_ProxyInfo.tszSrcAddr, st_ProxyInfo.tszDstAddr, nMsgLen);
}
bool XEngine_Proxy_Recvmsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	SESSION_FORWARD st_ProxyInfo = {};
	if (!ModuleSession_Proxy_GetForAddr(lpszClientAddr, &st_ProxyInfo))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Proxy客户端:%s,获取转发地址信息失败,源地址:%s,目地址:%s,错误码:%lX"), lpszClientAddr, st_ProxyInfo.tszSrcAddr, st_ProxyInfo.tszDstAddr, ModuleSession_GetLastError());
		return false;
	}
	if (!XClient_TCPSelect_SendEx(xhProxyClient, st_ProxyInfo.xhClient, lpszMsgBuffer, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Proxy客户端:%s,转发数据失败,源地址:%s,目地址:%s,错误码:%lX"), lpszClientAddr, st_ProxyInfo.tszSrcAddr, st_ProxyInfo.tszDstAddr, XClient_GetLastError());
		return false;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhProxyHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("Proxy客户端:%s,转发数据成功,源地址:%s,目地址:%s,大小:%d"), lpszClientAddr, st_ProxyInfo.tszSrcAddr, st_ProxyInfo.tszDstAddr, nMsgLen);
	return true;
}
