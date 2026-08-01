#include "../XEngine_Hdr.h"

bool XEngine_ProxyHTTP_Post(LPCXSTR lpszClientAddr, LPCXSTR lpszAPIName, LPCXSTR lpszAPIType, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	LPCXSTR lpszAPIList = _X("list");

	XCHAR tszSDBuffer[XPATH_8MAX] = {};

	if (0 == _tcsxncmp(lpszAPIName, lpszAPIList, _tcsxlen(lpszAPIList)))
	{
	}
	else
	{
		ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, ERROR_XENGINE_PROXY_PROTOCOL_NOTSUPPORT, _X("unknow protocol"));
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, XENGINE_CLIENT_NETTYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,发送POST请求名称:%s,类型:%s,处理失败,不支持"), lpszClientAddr, lpszAPIName, lpszAPIType);
	}
	return true;
}