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
bool CALLBACK Network_Callback_SocksLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	//客户端连接后要把客户端插入心跳管理器中才有效
	ModuleSession_Socks_Create(lpszClientAddr);
	SocketOpt_HeartBeat_InsertAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SOCKS客户端:%s,连接到服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_SocksRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	//需要激活一次
	XEngine_SocksTask_Handle(lpszClientAddr, lpszRecvMsg, nMsgLen);
	SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("SOCKS客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_SocksLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	//交给指定函数来处理客户端离开消息
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, XENGINE_CLIENT_CLOSE_NETWORK);
}
void CALLBACK Network_Callback_SocksHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	//同上
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_SOCKS, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////下面是Tunnel网络IO相关代码处理函数
bool CALLBACK Network_Callback_TunnelLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	ModuleSession_Tunnel_Create(lpszClientAddr);
	SocketOpt_HeartBeat_InsertAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Tunnel客户端:%s,连接到服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_TunnelRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	XEngine_TunnelTask_Handle(lpszClientAddr, lpszRecvMsg, nMsgLen);
	SocketOpt_HeartBeat_ActiveAddrEx(xhTunnelHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("Tunnel客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_TunnelLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, XENGINE_CLIENT_CLOSE_NETWORK);
}
void CALLBACK Network_Callback_TunnelHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_TUNNEL, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////下面是Tunnel网络IO相关代码处理函数
bool CALLBACK Network_Callback_ForwardLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	HelpComponents_Datas_CreateEx(xhForwardPacket, lpszClientAddr);
	SocketOpt_HeartBeat_InsertAddrEx(xhForwardHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端:%s,连接到服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_ForwardRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	SESSION_FORWARD st_ForwardClinet = {};
	if (ModuleSession_Forward_Get(lpszClientAddr, &st_ForwardClinet))
	{
		if (st_ForwardClinet.bForward)
		{
			if (st_ForwardClinet.bAnony)
			{
				//匿名转发
				if (XClient_TCPSelect_SendEx(xhForwardClient, st_ForwardClinet.xhClient, lpszRecvMsg, nMsgLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("Forward客户端:%s,匿名转发数据成功,大小:%d"), lpszClientAddr, nMsgLen);
				}
				else
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Forward客户端:%s,匿名转发数据失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, XClient_GetLastError());
				}
			}
			else
			{
				//非匿名转发
				XEngine_Network_Send(st_ForwardClinet.tszDstAddr, lpszRecvMsg, nMsgLen, XENGINE_CLIENT_NETTYPE_FORWARD);
			}
		}
		else
		{
			//没有绑定转发,投递到包中处理
			if (!HelpComponents_Datas_PostEx(xhForwardPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Forward客户端:%s,投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
			}
		}
	}
	else
	{
		//没有绑定转发,投递到包中处理
		if (!HelpComponents_Datas_PostEx(xhForwardPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Forward客户端:%s,投递数据包失败,大小:%d,错误:%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
		}
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhForwardHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("Forward客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_ForwardLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_FORWARD, XENGINE_CLIENT_CLOSE_NETWORK);
}
void CALLBACK Network_Callback_ForwardHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_FORWARD, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////Forward相关
bool CALLBACK Network_Callback_ProxyLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	return XEngine_Proxy_Connect(lpszClientAddr);
}
void CALLBACK Network_Callback_ProxyRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	XEngine_Proxy_Recvmsg(lpszClientAddr, lpszRecvMsg, nMsgLen);
}
void CALLBACK Network_Callback_ProxyLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_PROXY, XENGINE_CLIENT_CLOSE_NETWORK);
}
void CALLBACK Network_Callback_ProxyHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, XENGINE_CLIENT_NETTYPE_PROXY, XENGINE_CLIENT_CLOSE_HEARTBEAT);
}
//////////////////////////////////////////////////////////////////////////网络IO关闭操作
void XEngine_Network_Close(LPCXSTR lpszClientAddr, int nIPProto, int nCloseType)
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
		ModuleSession_Socks_GetList((XPPPMEM)&ppSt_ClientList, &nListCount, sizeof(PROXYPROTOCOL_CLIENTINFO));
		for (int i = 0; i < nListCount; i++)
		{
			if (0 == _tcsxnicmp(lpszClientAddr, ppSt_ClientList[i]->tszIPAddr, _tcsxlen(lpszClientAddr)))
			{
				XClient_TCPSelect_DeleteEx(xhSocksClient, ppSt_ClientList[i]->xhClient);
				break;
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
		ModuleSession_Socks_Delete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SOCKS客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
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
		ModuleSession_Tunnel_GetList((XPPPMEM)&ppSt_ClientList, &nListCount, sizeof(PROXYPROTOCOL_CLIENTINFO));
		for (int i = 0; i < nListCount; i++)
		{
			if (0 == _tcsxnicmp(lpszClientAddr, ppSt_ClientList[i]->tszIPAddr, _tcsxlen(lpszClientAddr)))
			{
				XClient_TCPSelect_DeleteEx(xhTunnelClient, ppSt_ClientList[i]->xhClient);
				break;
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
		ModuleSession_Tunnel_Delete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Tunnel客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
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
		SESSION_FORWARD st_ForwardInfo = {};
		if (ModuleSession_Forward_Get(lpszClientAddr, &st_ForwardInfo))
		{
			XClient_TCPSelect_DeleteEx(xhForwardClient, st_ForwardInfo.xhClient);
		}
		ModuleSession_Proxy_Delete(lpszClientAddr);
		HelpComponents_Datas_DeleteEx(xhForwardPacket, lpszClientAddr);
		ModuleSession_Forward_Delete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
	}
	else if (XENGINE_CLIENT_NETTYPE_PROXY == nIPProto)
	{
		if (XENGINE_CLIENT_CLOSE_NETWORK == nCloseType)
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhProxyHeart, lpszClientAddr);
		}
		else if (XENGINE_CLIENT_CLOSE_HEARTBEAT == nCloseType)
		{
			NetCore_TCPXCore_CloseForClientEx(xhProxySocket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhProxyHeart, lpszClientAddr);
			NetCore_TCPXCore_CloseForClientEx(xhProxySocket, lpszClientAddr);
		}
		SESSION_FORWARD st_ProxyInfo = {};
		if (ModuleSession_Proxy_GetForAddr(lpszClientAddr, &st_ProxyInfo))
		{
			XClient_TCPSelect_DeleteEx(xhProxyClient, st_ProxyInfo.xhClient);
		}
		ModuleSession_Proxy_Delete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Proxy客户端:%s,离开服务器,离开类型;%d"), lpszClientAddr, nCloseType);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("未知客户端:%s,离开服务器"), lpszClientAddr);
	}
}
//////////////////////////////////////////////////////////////////////////
bool XEngine_Network_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nIPProto)
{
	//根据客户端类型来处理发送业务逻辑
	if (XENGINE_CLIENT_NETTYPE_SOCKS == nIPProto)
	{
		//发送数据给指定客户端
		if (!NetCore_TCPXCore_SendEx(xhSocksSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, 1))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SOCKS客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhSocksHeart, lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_TUNNEL == nIPProto)
	{
		if (!NetCore_TCPXCore_SendEx(xhTunnelSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, 1))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Tunnel客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		SocketOpt_HeartBeat_ActiveAddrEx(xhTunnelHeart, lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_FORWARD == nIPProto)
	{
		if (!NetCore_TCPXCore_SendEx(xhForwardSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, 1))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Forward客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		SocketOpt_HeartBeat_ActiveAddrEx(xhForwardHeart, lpszClientAddr);
	}
	else if (XENGINE_CLIENT_NETTYPE_PROXY == nIPProto)
	{
		if (!NetCore_TCPXCore_SendEx(xhProxySocket, lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, 1))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("Proxy客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		SocketOpt_HeartBeat_ActiveAddrEx(xhProxyHeart, lpszClientAddr);
	}
	else 
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("未知客户端:%s,发送数据给失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
	}
	return true;
}