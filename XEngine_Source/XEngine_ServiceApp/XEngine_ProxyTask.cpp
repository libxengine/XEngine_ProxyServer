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
void CALLBACK XEngine_Proxy_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	SESSION_FORWARD st_ProxyInfo = {};
	if (!ModuleSession_Proxy_GetForToken(xhClient, &st_ProxyInfo))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Proxy客户端:%lld,获取转发地址信息失败,地址:%s,端口:%d,错误码:%lX"), xhToken, st_ProxyInfo.tszSrcAddr, st_ProxyInfo.tszDstAddr, ModuleSession_GetLastError());
		return;
	}
}

bool XEngine_Proxy_Connect(LPCXSTR lpszClientAddr)
{
	int nPort = 0;
	XNETHANDLE xhClient = 0;
	XCHAR tszIPAddr[128] = {};

	_tcsxcpy(tszIPAddr, st_ServiceConfig.st_XProxy.tszIPAddr);
	APIAddr_IPAddr_SegAddr(tszIPAddr, &nPort);
	if (!XClient_TCPSelect_InsertEx(xhProxyClient, &xhClient, tszIPAddr, nPort, false))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Proxy客户端:%s,连接到转发代理服务器失败,服务地址:%s,错误码:%lX"), lpszClientAddr, st_ServiceConfig.st_XProxy.tszIPAddr, XClient_GetLastError());
		return false;
	}
	SocketOpt_HeartBeat_InsertAddrEx(xhProxyHeart, lpszClientAddr);
	ModuleSession_Proxy_Insert(lpszClientAddr, st_ServiceConfig.st_XProxy.tszIPAddr, xhClient);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Proxy客户端:%s,连接到服务器"), lpszClientAddr);
	return true;
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
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("Proxy客户端:%s,转发数据成功,源地址:%s,目地址:%s,大小:%d"), lpszClientAddr, st_ProxyInfo.tszSrcAddr, st_ProxyInfo.tszDstAddr, nMsgLen);
	return true;
}
