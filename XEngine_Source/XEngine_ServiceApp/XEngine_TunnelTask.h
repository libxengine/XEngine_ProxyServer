#pragma once
/********************************************************************
//    Created:     2021/12/02  17:14:41
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_TunnelTask.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_TunnelTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
//任务处理相关函数,处理包的内容
BOOL XEngine_TunnelTask_Handle(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);
void CALLBACK XEngine_Tunnel_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, SOCKET hSocket, ENUM_NETCLIENT_TCPEVENTS enTCPClientEvents, LPCSTR lpszMsgBuffer, int nLen, LPVOID lParam);