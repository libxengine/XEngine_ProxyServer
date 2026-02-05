#pragma once
/********************************************************************
//    Created:     2026/02/05  16:00:07
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp\XEngine_HTTPTask.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_HTTPTask
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
//任务处理池,用来获取一个完整包
XHTHREAD XCALLBACK XEngine_HTTPTask_Thread(XPVOID lParam);
//任务处理相关函数,处理包的内容
bool XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR** pptszListHdr, int nHdrCount);