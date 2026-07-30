#include "../XEngine_Hdr.h"

bool XEngine_ProxyHTTP_Post(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszAPIType, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	LPCXSTR lpszAPIList = _X("list");
	LPCXSTR lpszAPIForward = _X("forward");

	XCHAR tszSDBuffer[XPATH_8MAX] = {};

	if (0 == _tcsxncmp(lpszAPIName, lpszAPIForward, _tcsxlen(lpszAPIForward)))
	{
		//http://127.0.0.1:5400/api?function=list&value=forward
		int nListCount = 0;
		SESSION_FORWARD** ppSt_ListUser;
		ModuleSession_Forward_List(&ppSt_ListUser, &nListCount);

		ModuleProtocol_Packet_ForwardList(tszSDBuffer, &nSDLen, &ppSt_ListUser, nListCount);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListUser, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,转发列表请求成功"), lpszClientAddr);
	}
	else
	{
		ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,发送POST请求名称:%s,类型:%s,处理失败,不支持"), lpszClientAddr, lpszAPIName, lpszAPIType);
	}
	return true;
}