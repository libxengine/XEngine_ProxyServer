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

bool HTTPTask_TastPost_Verification(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, XCHAR** pptszHDRList, int nHDRCount)
{
	//http验证
	int nVType = 0;
	int nSDLen = 0;
	XCHAR tszSDBuffer[XPATH_MAX] = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	st_HDRParam.nHttpCode = 401;
	st_HDRParam.bIsClose = true;
	st_HDRParam.bAuth = true;
	//打包验证信息
	int nHDRLen = 0;
	XCHAR tszHDRBuffer[XPATH_MAX] = {};
	if (1 == st_ServiceConfig.st_XVerifcation.nVType)
	{
		Verification_HTTP_BasicServerPacket(tszHDRBuffer, &nHDRLen);
	}
	else
	{
		XCHAR tszNonceStr[64] = {};
		XCHAR tszOpaqueStr[64] = {};
		Verification_HTTP_DigestServerPacket(tszHDRBuffer, &nHDRLen, tszNonceStr, tszOpaqueStr);
	}
	//后去验证方法
	if (!Verification_HTTP_GetType(pptszHDRList, nHDRCount, &nVType))
	{
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式:%d,错误:%lX"), lpszClientAddr, st_ServiceConfig.st_XVerifcation.nVType, Verification_GetLastError());
		return false;
	}
	//验证方式是否一致
	if (st_ServiceConfig.st_XVerifcation.nVType != nVType)
	{
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式错误,请求:%d,需求:%d"), lpszClientAddr, nVType, st_ServiceConfig.st_XVerifcation.nVType);
		return false;
	}
	bool bRet = false;
	if (1 == nVType)
	{
		bRet = Verification_HTTP_Basic(st_ServiceConfig.st_XVerifcation.tszUserName, st_ServiceConfig.st_XVerifcation.tszUserPass, pptszHDRList, nHDRCount);
	}
	else if (2 == nVType)
	{
		bRet = Verification_HTTP_Digest(st_ServiceConfig.st_XVerifcation.tszUserName, st_ServiceConfig.st_XVerifcation.tszUserPass, pSt_HTTPParam->tszHttpMethod, pptszHDRList, nHDRCount);
	}

	if (!bRet)
	{
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证处理错误,可能用户密码登信息不匹配,类型:%d"), lpszClientAddr, nVType);
		return false;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,HTTP验证类型:%d 通过"), lpszClientAddr, nVType);
	return true;
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
	//http验证
	if (st_ServiceConfig.st_XVerifcation.bEnable)
	{
		//全面验证
		if (!HTTPTask_TastPost_Verification(pSt_HTTPParam, lpszClientAddr, pptszListHdr, nHdrCount))
		{
			return false;
		}
	}
	XCHAR tszKey[128] = {};
	XCHAR tszValue[128] = {};
	LPCXSTR lpszAPIProxyRule = _X("proxyrule");
	LPCXSTR lpszAPIReload = _X("reload");
	//获得函数名
	BaseLib_String_GetKeyValue(pptszList[0], "=", tszKey, tszValue);
	//得到客户端请求的方法
	if (0 == _tcsxncmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		if (0 == _tcsxncmp(lpszAPIProxyRule, tszValue, _tcsxlen(lpszAPIProxyRule)))
		{
			//http://127.0.0.1:5400/api?function=get&value=proxyrule
		}
		else
		{
			ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,发送POST请求:%s,处理失败,不支持"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		}
	}
	else if (0 == _tcsxncmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		if (0 == _tcsxncmp(lpszAPIReload, tszValue, _tcsxlen(lpszAPIReload)))
		{
			delete st_ProxyConfig.pStl_ListDestAddr;
			delete st_ProxyConfig.pStl_ListRuleAddr;
			//http://127.0.0.1:5400/api?function=reload&type=0
			LPCXSTR lpszConfigProxy = _X("./XEngine_Config/XEngine_ProxyConfig.json");
			ModuleConfigure_Json_ProxyFile(lpszConfigProxy, &st_ProxyConfig);
			ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,发送重载配置请求成功"), lpszClientAddr);
		}
		else if (0 == _tcsxncmp(lpszAPIProxyRule, tszValue, _tcsxlen(lpszAPIProxyRule)))
		{
			//http://127.0.0.1:5400/api?function=proxyrule
			int nListCount = 0;
			SESSION_IPCONUT** ppSt_IPCountList;
			ModuleSession_ProxyRule_GetList(&ppSt_IPCountList, &nListCount);
			ModuleProtocol_Packet_GetProxyRuleList(tszSDBuffer, &nSDLen, &ppSt_IPCountList, nListCount);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_IPCountList, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送获取转发规则列表请求处理成功"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		}
		else
		{
			ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,发送GET请求:%s,处理失败,不支持"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		}
	}
	else
	{
		ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,协议错误"), lpszClientAddr);
	}
	return true;
}