#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/06/08  13:44:41
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardTask.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_ForwardTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     代理转发协议
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_Forward_Thread(XPVOID lParam)
{
	int nPoolIndex = *(int*)lParam;
	int nThreadPos = nPoolIndex + 1;

	while (bIsRun)
	{
		if (!HelpComponents_Datas_WaitEventEx(xhForwardPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListClient;
		HelpComponents_Datas_GetPoolEx(xhForwardPacket, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			XENGINE_PROTOCOLHDR st_ProtocolHdr;
			memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

			if (HelpComponents_Datas_GetMemoryEx(xhForwardPacket, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
			{
				XEngine_Forward_Handle(ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &st_ProtocolHdr);
				BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}

bool XEngine_Forward_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr)
{
	int nSDLen = 10240;
	XCHAR tszSDBuffer[10240];
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	//处理验证协议
	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LOGREQ == pSt_ProtocolHdr->unOperatorCode)
		{
			XENGINE_PROTOCOL_USERAUTH st_UserAuth;
			memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

			memcpy(&st_UserAuth, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_USERAUTH));
			pSt_ProtocolHdr->wReserve = 0;
			pSt_ProtocolHdr->unPacketSize = 0;
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LOGREP;
			ModuleSession_Forward_Insert(lpszClientAddr, &st_UserAuth);
			XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，设置的用户：%s，登录到服务器"), lpszClientAddr, st_UserAuth.tszUserName);
		}
	}
	//处理转发协议
	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_USER_FORWARD == pSt_ProtocolHdr->unOperatorType)
	{
		if (!ModuleSession_Forward_Get(lpszClientAddr))
		{
			pSt_ProtocolHdr->wReserve = 401;
			pSt_ProtocolHdr->unPacketSize = 0;
			pSt_ProtocolHdr->unOperatorCode = pSt_ProtocolHdr->unOperatorCode + 1;
			XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，请求转发协议:%d 失败,因为没有登录"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
			return false;
		}

		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LISTREQ == pSt_ProtocolHdr->unOperatorCode)
		{
			int nListCount = 0;
			SESSION_FORWARD** ppSt_ListUser;

			pSt_ProtocolHdr->wReserve = 0;
			pSt_ProtocolHdr->unPacketSize = 0;
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_LISTREP;
			ModuleSession_Forward_List(&ppSt_ListUser, &nListCount, lpszClientAddr);
			ModuleProtocol_Packet_ForwardList(tszSDBuffer, &nSDLen, pSt_ProtocolHdr, &ppSt_ListUser, nListCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListUser, nListCount);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_FORWARD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，请求可用转发列表成功"), lpszClientAddr);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_NAMEDREQ == pSt_ProtocolHdr->unOperatorCode)
		{
			XCHAR tszSrcAddr[128];
			XCHAR tszDstAddr[128];

			memset(tszSrcAddr, '\0', sizeof(tszSrcAddr));
			memset(tszDstAddr, '\0', sizeof(tszDstAddr));

			ModuleProtocol_Parse_ForwardBind(lpszMsgBuffer, nMsgLen, tszSrcAddr, tszDstAddr);
			if (!ModuleSession_Forward_BindNamed(lpszClientAddr, tszDstAddr))
			{
				pSt_ProtocolHdr->wReserve = 404;
				pSt_ProtocolHdr->unPacketSize = 0;
				pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_NAMEDREP;
				XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，请求命名绑定转发地址:%s 失败,错误:%lX"), lpszClientAddr, tszDstAddr);
				return false;
			}
			//先告知对方要转发数据
			pSt_ProtocolHdr->wReserve = 0;
			pSt_ProtocolHdr->unPacketSize = 0;
			XEngine_Network_Send(tszDstAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
			//最后返回结果
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_NAMEDREP;
			XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，请求命名绑定转发地址:%s 成功"), lpszClientAddr, tszDstAddr);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_ANONYREQ == pSt_ProtocolHdr->unOperatorCode)
		{
			XCHAR tszSrcAddr[128];
			XCHAR tszDstAddr[128];

			memset(tszSrcAddr, '\0', sizeof(tszSrcAddr));
			memset(tszDstAddr, '\0', sizeof(tszDstAddr));

			ModuleProtocol_Parse_ForwardBind(lpszMsgBuffer, nMsgLen, tszSrcAddr, tszDstAddr);
			//匿名绑定,请求连接
			int nPort = 0;
			XNETHANDLE xhClient = 0;
			BaseLib_OperatorIPAddr_SegAddr(tszDstAddr, &nPort);
			if (!XClient_TCPSelect_InsertEx(xhForwardClient, &xhClient, tszDstAddr, nPort))
			{
				pSt_ProtocolHdr->wReserve = 500;
				pSt_ProtocolHdr->unPacketSize = 0;
				pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_ANONYREP;
				XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，请求匿名绑定转发地址:%s 失败,网络连接错误,错误码:%lX"), lpszClientAddr, tszDstAddr, XClient_GetLastError());
				return false;
			}
			if (!ModuleSession_Forward_BindAnony(lpszClientAddr, tszDstAddr, xhClient))
			{
				pSt_ProtocolHdr->wReserve = 404;
				pSt_ProtocolHdr->unPacketSize = 0;
				pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_ANONYREP;
				XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，请求匿名绑定转发地址:%s 失败,错误:%lX"), lpszClientAddr, tszDstAddr);
				return false;
			}
			//返回结果
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_FORWARD_ANONYREP;
			XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), XENGINE_CLIENT_NETTYPE_FORWARD);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端：%s，请求匿名绑定转发地址:%s 成功"), lpszClientAddr, tszDstAddr);
		}
	}
	
	return true;
}
void CALLBACK XEngine_Forward_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam)
{
	int nListCount = 0;
	SESSION_FORWARD** ppSt_ClientList;
	ModuleSession_Forward_List(&ppSt_ClientList, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		if (xhClient == ppSt_ClientList[i]->xhClient)
		{
			if (ENUM_XCLIENT_SOCKET_EVENT_RECV == enTCPClientEvents)
			{
				if (!XEngine_Network_Send(ppSt_ClientList[i]->tszSrcAddr, lpszMsgBuffer, nLen, XENGINE_CLIENT_NETTYPE_FORWARD))
				{
					SocketOpt_HeartBeat_ForceOutAddrEx(xhForwardHeart, ppSt_ClientList[i]->tszSrcAddr);
				}
			}
			else if (ENUM_XCLIENT_SOCKET_EVENT_CLOSE == enTCPClientEvents)
			{
				//退出处理
				SocketOpt_HeartBeat_ForceOutAddrEx(xhForwardHeart, ppSt_ClientList[i]->tszSrcAddr);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("Forward客户端:%s,离开服务器,客户端主动断开"), ppSt_ClientList[i]->tszSrcAddr);
			}
			break;
		}
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
}