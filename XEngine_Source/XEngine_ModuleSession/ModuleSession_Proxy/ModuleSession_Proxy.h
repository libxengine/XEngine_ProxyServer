#pragma once
/********************************************************************
//    Created:     2025/01/20  11:13:18
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy\ModuleSession_Proxy.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy
//    File Base:   ModuleSession_Proxy
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     全代理转发
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszIPAddr[128];
	XSOCKET hSocket;
}SESSION_PROXYCLIENT;

class CModuleSession_Proxy
{
public:
	CModuleSession_Proxy();
	~CModuleSession_Proxy();
public:
	bool ModuleSession_Proxy_Insert(LPCXSTR lpszSrcIPAddr, LPCXSTR lpszDstIPAddr, XNETHANDLE xhClient);
	bool ModuleSession_Proxy_GetForAddr(LPCXSTR lpszIPAddr, SESSION_FORWARD* pSt_ProxyInfo);
	bool ModuleSession_Proxy_GetForToken(XNETHANDLE xhToken, SESSION_FORWARD* pSt_ProxyInfo);
	bool ModuleSession_Proxy_List(SESSION_FORWARD*** pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr = NULL);
	bool ModuleSession_Proxy_Delete(LPCXSTR lpszIPAddr);
private:
	shared_mutex st_Locker;
private:
	unordered_map<xstring, SESSION_FORWARD> stl_MapSession;
};