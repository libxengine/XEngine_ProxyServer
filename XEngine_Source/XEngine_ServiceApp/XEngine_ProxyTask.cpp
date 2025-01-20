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
	XSOCKET hSocket = 0;
	if (!XClient_TCPSelect_Create(&hSocket, st_ServiceConfig.st_XProxy.tszDstIPAddr, st_ServiceConfig.st_XProxy.nDstPort, 2))
	{
		return false;
	}
	SocketOpt_HeartBeat_InsertAddrEx(xhProxyHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Proxy客户端:%s,连接到服务器"), lpszClientAddr);
	return true;
}
bool XEngine_Proxy_Recvmsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	return true;
}