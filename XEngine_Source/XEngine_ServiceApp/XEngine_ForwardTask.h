#pragma once
/********************************************************************
//    Created:     2022/06/08  13:44:36
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardTask.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_ForwardTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     代理转发协议
//    History:
*********************************************************************/
//任务处理相关函数,处理包的内容
XHTHREAD XCALLBACK XEngine_Forward_Thread(XPVOID lParam);
bool XEngine_Forward_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);
void XCALLBACK XEngine_Forward_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam);