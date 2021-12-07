#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2021/12/02  16:40:28
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_Network.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_Network
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO相关代码
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////下面是TCP网络IO相关代码处理函数
BOOL __stdcall Network_Callback_TCPLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//客户端连接后要把客户端插入心跳管理器中才有效
	SocketOpt_HeartBeat_InsertAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall Network_Callback_TCPRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	//需要激活一次
	SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("TCP客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void __stdcall Network_Callback_TCPLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//交给指定函数来处理客户端离开消息
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, FALSE);
}
void __stdcall Network_Callback_TCPHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	//同上
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, TRUE);
}
//////////////////////////////////////////////////////////////////////////下面是HTTP网络IO相关代码处理函数
BOOL __stdcall Network_Callback_HTTPLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	SocketOpt_HeartBeat_InsertAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall Network_Callback_HTTPRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	SocketOpt_HeartBeat_ActiveAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("HTTP客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void __stdcall Network_Callback_HTTPLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, FALSE);
}
void __stdcall Network_Callback_HTTPHeart(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, TRUE);
}
//////////////////////////////////////////////////////////////////////////网络IO关闭操作
void XEngine_Network_Close(LPCTSTR lpszClientAddr, int nIPProto, BOOL bHeart)
{
	if (XENGINE_CLIENT_NETTYPE_SOCKS == nIPProto)
	{
		//先关闭网络和心跳,他们主动回调的数据我们可以不用主动调用关闭
		if (bHeart)
		{
			//心跳超时属于主动关闭,所以要主动调用网络关闭
			NetCore_TCPXCore_CloseForClientEx(xhSocksSocket, lpszClientAddr);
		}
		else
		{
			//同上
			SocketOpt_HeartBeat_DeleteAddrEx(xhSocksHeart, lpszClientAddr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端:%s,离开服务器"), lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_TUNNEL == nIPProto)
	{
		if (bHeart)
		{
			NetCore_TCPXCore_CloseForClientEx(xhTunnelSocket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhTunnelHeart, lpszClientAddr);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,离开服务器"), lpszClientAddr);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("未知客户端:%s,离开服务器"), lpszClientAddr);
	}
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Network_Send(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nIPProto)
{
	//根据客户端类型来处理发送业务逻辑
	if (XENGINE_CLIENT_NETTYPE_SOCKS == nIPProto)
	{
		//发送数据给指定客户端
		if (!NetCore_TCPXCore_SendEx(xhSocksSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("TCP客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_TUNNEL == nIPProto)
	{
		if (!NetCore_TCPXCore_SendEx(xhTunnelSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		SocketOpt_HeartBeat_ActiveAddrEx(xhTunnelHeart, lpszClientAddr);
	}
	else 
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("未知客户端:%s,发送数据给失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
	}
	return TRUE;
}