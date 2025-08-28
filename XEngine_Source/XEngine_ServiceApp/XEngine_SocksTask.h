#pragma once
/********************************************************************
//    Created:     2021/12/02  17:06:56
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_SocksTask.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_SocksTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     Socks任务处理代码
//    History:
*********************************************************************/
//任务处理相关函数,处理包的内容
bool XEngine_SocksTask_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);
void XCALLBACK XEngine_Socks_CBRecv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam);