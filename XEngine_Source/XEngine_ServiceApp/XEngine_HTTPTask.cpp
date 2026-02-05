#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2026/02/05  16:00:12
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp\XEngine_HTTPTask.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ServiceApp
//    File Base:   XEngine_HTTPTask
//    File Ext:    cpp
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
XHTHREAD XCALLBACK XEngine_HTTPTask_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HttpProtocol_Server_EventWaitEx(xhHTTPPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HttpProtocol_Server_GetPoolEx(xhHTTPPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		//先循环客户端
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                                    //客户端发送的数据大小,不包括头
				int nHdrCount = 0;                                  //客户端的HTTP头字段数
				XCHAR** ppszHdrList;                                 //客户端的HTTP头字段列表
				XCHAR* ptszMsgBuffer = NULL;                         //客户端发送的数据
				RFCCOMPONENTS_HTTP_REQPARAM st_HTTPReqparam;        //客户端的请求参数

				memset(&st_HTTPReqparam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
				//得到一个指定客户端的完整数据包
				if (HttpProtocol_Server_GetMemoryEx(xhHTTPPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPReqparam, &ppszHdrList, &nHdrCount))
				{
					//在另外一个函数里面处理数据
					XEngine_HTTPTask_Handle(&st_HTTPReqparam, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, ppszHdrList, nHdrCount);
					//释放内存
					BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
					BaseLib_Memory_Free((XPPPMEM)&ppszHdrList, nHdrCount);
				}
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR** pptszListHdr, int nHdrCount)
{
	int nSDLen = 0;
	int nListCount = 0;
	XCHAR** pptszList;

	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodGet = _X("GET");
	XCHAR tszSDBuffer[4096] = {};

	HttpProtocol_ServerHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszList, &nListCount);
	if (nListCount < 1)
	{
		ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, 400, "Bad Request,parament is incorrent");
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		BaseLib_Memory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	XCHAR tszKey[128] = {};
	XCHAR tszValue[128] = {};
	LPCXSTR lpszAPICreate = _X("create");
	LPCXSTR lpszAPIQuery = _X("query");
	//获得函数名
	BaseLib_String_GetKeyValue(pptszList[0], "=", tszKey, tszValue);
	//得到客户端请求的方法
	if (0 == _tcsxnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		if (0 == _tcsxnicmp(lpszAPICreate, tszValue, _tcsxlen(lpszAPICreate)))
		{
			//http://127.0.0.1:5501/api?function=create&token=123123
			bool bCloud = false;
			XNETHANDLE xhToken = 0;

			for (int i = 1; i < nListCount; i++)
			{
				memset(tszKey, 0, sizeof(tszKey));
				memset(tszValue, 0, sizeof(tszValue));

				BaseLib_String_GetKeyValue(pptszList[i], "=", tszKey, tszValue);

				if (0 == _tcsxncmp(tszKey, "token", 5))
				{
					xhToken = _ttxoll(tszValue);
				}
				else if (0 == _tcsxncmp(tszKey, "cloud", 5))
				{
					bCloud = _ttxoi(tszValue);
				}
			}
		}
		else
		{
			ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("客户端:%s,发送POST请求:%s,处理失败,不支持"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		if (0 == _tcsxnicmp(lpszAPIQuery, tszValue, _tcsxlen(lpszAPIQuery)))
		{
			//http://127.0.0.1:5501/api?function=stop&token=123123&value=0
			for (int i = 1; i < nListCount; i++)
			{
				memset(tszKey, 0, sizeof(tszKey));
				memset(tszValue, 0, sizeof(tszValue));

				BaseLib_String_GetKeyValue(pptszList[i], "=", tszKey, tszValue);

				if (0 == _tcsxncmp(tszKey, "exist", 5))
				{
					
				}
				else if (0 == _tcsxncmp(tszKey, "pf", 2))
				{
					
				}
			}
		}
		else
		{
			ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("客户端:%s,发送GET请求:%s,处理失败,不支持"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		}
	}
	else
	{
		ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("客户端:%s,协议错误"), lpszClientAddr);
	}
	return true;
}