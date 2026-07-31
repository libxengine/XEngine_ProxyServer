#include "../XEngine_Hdr.h"

bool XEngine_ProxyHTTP_Get(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszAPIType, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;

	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIReload = _X("reload");
	XCHAR tszSDBuffer[XPATH_8MAX] = {};

	if (0 == _tcsxncmp(lpszAPIList, lpszAPIName, _tcsxlen(lpszAPIList)))
	{
		LPCXSTR lpszAPIProxyRule = _X("rule");
		LPCXSTR lpszAPIProxy = _X("proxy");
		LPCXSTR lpszAPIForward = _X("forward");

		if (0 == _tcsxncmp(lpszAPIProxyRule, lpszAPIName, _tcsxlen(lpszAPIProxyRule)))
		{
			//http://127.0.0.1:5400/api?function=proxyrule&type=0
			int nListCount = 0;
			SESSION_IPCONUT** ppSt_IPCountList;
			ModuleSession_ProxyRule_GetList(&ppSt_IPCountList, &nListCount);
			ModuleProtocol_Packet_GetProxyRuleList(tszSDBuffer, &nSDLen, &ppSt_IPCountList, nListCount);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_IPCountList, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送获取转发规则列表请求处理成功"), lpszClientAddr);
		}
		else if (0 == _tcsxncmp(lpszAPIName, lpszAPIForward, _tcsxlen(lpszAPIForward)))
		{
			//http://127.0.0.1:5400/api?function=list&value=forward
			int nListCount = 0;
			SESSION_FORWARD** ppSt_ListUser;
			ModuleSession_Forward_List(&ppSt_ListUser, &nListCount);

			ModuleProtocol_Packet_ForwardList(tszSDBuffer, &nSDLen, &ppSt_ListUser, nListCount);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ListUser, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,转发forward 列表请求成功"), lpszClientAddr);
		}
		else if (0 == _tcsxncmp(lpszAPIProxy, lpszAPIName, _tcsxlen(lpszAPIProxy)))
		{
			//http://127.0.0.1:5400/api?function=list&value=proxy
			int nListCount = 0;
			SESSION_FORWARD** ppSt_ListUser;
			ModuleSession_Proxy_List(&ppSt_ListUser, &nListCount);
			ModuleProtocol_Packet_ForwardList(tszSDBuffer, &nSDLen, &ppSt_ListUser, nListCount);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ListUser, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,转发proxy 列表请求成功"), lpszClientAddr);
		}
	}
	else if (0 == _tcsxncmp(lpszAPIReload, lpszAPIName, _tcsxlen(lpszAPIReload)))
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
	else
	{
		ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,发送GET请求名称:%s,类型:%s,处理失败,不支持"), lpszClientAddr, lpszAPIName, lpszAPIType);
	}
	return true;
}