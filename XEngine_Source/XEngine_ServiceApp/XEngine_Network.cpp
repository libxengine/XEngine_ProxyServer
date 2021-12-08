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
//////////////////////////////////////////////////////////////////////////下面是SOCKS网络IO相关代码处理函数
BOOL __stdcall Network_Callback_SocksLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//客户端连接后要把客户端插入心跳管理器中才有效
	ProxyProtocol_SocksCore_Create(lpszClientAddr);
	SocketOpt_HeartBeat_InsertAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("SOCKS客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall Network_Callback_SocksRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	//需要激活一次
	XEngine_SocksTask_Handle(lpszClientAddr, lpszRecvMsg, nMsgLen);
	SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("SOCKS客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void __stdcall Network_Callback_SocksLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//交给指定函数来处理客户端离开消息
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, XENGINE_CLIENT_CLOSE_NETWORK);
}
void __stdcall Network_Callback_SocksHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	//同上
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////下面是Tunnel网络IO相关代码处理函数
BOOL __stdcall Network_Callback_TunnelLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	ProxyProtocol_TunnelCore_Create(lpszClientAddr);
	SocketOpt_HeartBeat_InsertAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Tunnel客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall Network_Callback_TunnelRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	XEngine_TunnelTask_Handle(lpszClientAddr, lpszRecvMsg, nMsgLen);
	SocketOpt_HeartBeat_ActiveAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("Tunnel客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void __stdcall Network_Callback_TunnelLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, XENGINE_CLIENT_CLOSE_NETWORK);
}
void __stdcall Network_Callback_TunnelHeart(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////网络IO关闭操作
void XEngine_Network_Close(LPCTSTR lpszClientAddr, int nIPProto, int nCloseType)
{
	if (XENGINE_CLIENT_NETTYPE_SOCKS == nIPProto)
	{
		//先关闭网络和心跳,他们主动回调的数据我们可以不用主动调用关闭
		if (XENGINE_CLIENT_CLOSE_NETWORK == nCloseType)
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhSocksHeart, lpszClientAddr);
		}
		else if (XENGINE_CLIENT_CLOSE_HEARTBEAT == nCloseType)
		{
			//心跳超时属于主动关闭,所以要主动调用网络关闭
			NetCore_TCPXCore_CloseForClientEx(xhSocksSocket, lpszClientAddr);
		}
		else
		{
			//主动关闭
			SocketOpt_HeartBeat_DeleteAddrEx(xhSocksHeart, lpszClientAddr);
			NetCore_TCPXCore_CloseForClientEx(xhSocksSocket, lpszClientAddr);
		}
		PROXYPROTOCOL_CLIENTINFO st_ProxyClient;
		memset(&st_ProxyClient, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));
		if (ProxyProtocol_SocksCore_GetInfo(lpszClientAddr, &st_ProxyClient))
		{
			XClient_TCPSelect_Close(st_ProxyClient.hSocket);
		}
		ProxyProtocol_SocksCore_Delete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("SOCKS客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
	}
	else if (XENGINE_CLIENT_NETTYPE_TUNNEL == nIPProto)
	{
		if (XENGINE_CLIENT_CLOSE_NETWORK == nCloseType)
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhTunnelHeart, lpszClientAddr);
		}
		else if (XENGINE_CLIENT_CLOSE_HEARTBEAT == nCloseType)
		{
			NetCore_TCPXCore_CloseForClientEx(xhTunnelSocket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhTunnelHeart, lpszClientAddr);
			NetCore_TCPXCore_CloseForClientEx(xhTunnelSocket, lpszClientAddr);
		}
		PROXYPROTOCOL_CLIENTINFO st_ProxyClient;
		memset(&st_ProxyClient, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));
		if (ProxyProtocol_TunnelCore_GetInfo(lpszClientAddr, &st_ProxyClient))
		{
			XClient_TCPSelect_Close(st_ProxyClient.hSocket);
		}
		ProxyProtocol_TunnelCore_Delete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Tunnel客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
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
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("SOCKS客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_TUNNEL == nIPProto)
	{
		if (!NetCore_TCPXCore_SendEx(xhTunnelSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("Tunnel客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
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