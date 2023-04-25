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
XHTHREAD CALLBACK XEngine_Forward_Thread(XPVOID lParam);
bool XEngine_Forward_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr);