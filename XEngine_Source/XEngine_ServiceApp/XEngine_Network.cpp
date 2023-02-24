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
BOOL CALLBACK Network_Callback_SocksLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//客户端连接后要把客户端插入心跳管理器中才有效
	ProxyProtocol_SocksCore_Create(lpszClientAddr);
	SocketOpt_HeartBeat_InsertAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("SOCKS客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK Network_Callback_SocksRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	//需要激活一次
	XEngine_SocksTask_Handle(lpszClientAddr, lpszRecvMsg, nMsgLen);
	SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("SOCKS客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_SocksLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//交给指定函数来处理客户端离开消息
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, XENGINE_CLIENT_CLOSE_NETWORK);
}
void CALLBACK Network_Callback_SocksHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	//同上
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////下面是Tunnel网络IO相关代码处理函数
BOOL CALLBACK Network_Callback_TunnelLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	ProxyProtocol_TunnelCore_Create(lpszClientAddr);
	SocketOpt_HeartBeat_InsertAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Tunnel客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK Network_Callback_TunnelRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	XEngine_TunnelTask_Handle(lpszClientAddr, lpszRecvMsg, nMsgLen);
	SocketOpt_HeartBeat_ActiveAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("Tunnel客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_TunnelLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, XENGINE_CLIENT_CLOSE_NETWORK);
}
void CALLBACK Network_Callback_TunnelHeart(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////下面是Tunnel网络IO相关代码处理函数
BOOL CALLBACK Network_Callback_ForwardLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	SocketOpt_HeartBeat_InsertAddrEx(xhForwardHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Forward客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK Network_Callback_ForwardRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	TCHAR tszDstAddr[128];
	memset(tszDstAddr, '\0', sizeof(tszDstAddr));

	if (ModuleSession_Forward_Get(lpszClientAddr, tszDstAddr))
	{
		//如果有转发,直接转发
		XEngine_Network_Send(tszDstAddr, lpszRecvMsg, nMsgLen, XENGINE_CLIENT_NETTYPE_FORWARD);
	}
	else
	{
		//没有绑定转发,投递到包中处理
		if (!HelpComponents_Datas_PostEx(xhForwardPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("Forward客户端:%s,投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhForwardHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("Forward客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_ForwardLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_FORWARD, XENGINE_CLIENT_CLOSE_NETWORK);
}
void CALLBACK Network_Callback_ForwardHeart(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_FORWARD, XENGINE_CLIENT_CLOSE_HEARTBEAT);
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
		//释放客户端
		int nListCount = 0;
		PROXYPROTOCOL_CLIENTINFO** ppSt_ClientList;
		ProxyProtocol_SocksCore_GetList((XPPPMEM)&ppSt_ClientList, &nListCount, sizeof(PROXYPROTOCOL_CLIENTINFO));
		for (int i = 0; i < nListCount; i++)
		{
			if (0 == _tcsnicmp(lpszClientAddr, ppSt_ClientList[i]->tszIPAddr, _tcslen(lpszClientAddr)))
			{
				XClient_TCPSelect_DeleteEx(xhSocksClient, ppSt_ClientList[i]->xhClient);
				break;
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
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
			NetCore_TCPXCore_CloseForClientEx(xhTunnelSocket, lpszClientAddr);
			SocketOpt_HeartBeat_DeleteAddrEx(xhTunnelHeart, lpszClientAddr);
		}
		//释放客户端
		int nListCount = 0;
		PROXYPROTOCOL_CLIENTINFO** ppSt_ClientList;
		ProxyProtocol_TunnelCore_GetList((XPPPMEM)&ppSt_ClientList, &nListCount, sizeof(PROXYPROTOCOL_CLIENTINFO));
		for (int i = 0; i < nListCount; i++)
		{
			if (0 == _tcsnicmp(lpszClientAddr, ppSt_ClientList[i]->tszIPAddr, _tcslen(lpszClientAddr)))
			{
				XClient_TCPSelect_DeleteEx(xhTunnelClient, ppSt_ClientList[i]->xhClient);
				break;
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
		ProxyProtocol_TunnelCore_Delete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Tunnel客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
	}
	else if (XENGINE_CLIENT_NETTYPE_FORWARD == nIPProto)
	{
		if (XENGINE_CLIENT_CLOSE_NETWORK == nCloseType)
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhForwardHeart, lpszClientAddr);
		}
		else if (XENGINE_CLIENT_CLOSE_HEARTBEAT == nCloseType)
		{
			NetCore_TCPXCore_CloseForClientEx(xhForwardSocket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhForwardHeart, lpszClientAddr);
			NetCore_TCPXCore_CloseForClientEx(xhForwardSocket, lpszClientAddr);
		}
		TCHAR tszClientAddr[128];
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		ModuleSession_Forward_Delete(lpszClientAddr, tszClientAddr);

		if (_tcslen(tszClientAddr) > 0)
		{
			XEngine_Network_Close(tszClientAddr, XENGINE_CLIENT_NETTYPE_FORWARD, XENGINE_CLIENT_CLOSE_SERVICE);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("Forward客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
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
		if (!NetCore_TCPXCore_SendEx(xhSocksSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, 1))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("SOCKS客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_TUNNEL == nIPProto)
	{
		if (!NetCore_TCPXCore_SendEx(xhTunnelSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, 1))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("Tunnel客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		SocketOpt_HeartBeat_ActiveAddrEx(xhTunnelHeart, lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_FORWARD == nIPProto)
	{
		if (!NetCore_TCPXCore_SendEx(xhForwardSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, 1))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("Forward客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		SocketOpt_HeartBeat_ActiveAddrEx(xhForwardHeart, lpszClientAddr);
	}
	else 
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("未知客户端:%s,发送数据给失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
	}
	return TRUE;
}