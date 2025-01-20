#pragma once
/********************************************************************
//    Created:     2025/01/20  10:45:13
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp\XEngine_ProxyTask.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_ProxyTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     代理转发协议
//    History:
*********************************************************************/
bool XEngine_Proxy_Connect(LPCXSTR lpszClientAddr);
bool XEngine_Proxy_Recvmsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);