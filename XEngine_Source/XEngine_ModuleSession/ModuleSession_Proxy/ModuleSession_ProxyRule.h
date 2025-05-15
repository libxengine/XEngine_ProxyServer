#pragma once
/********************************************************************
//    Created:     2025/05/15  11:33:09
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy\ModuleSession_ProxyRule.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Proxy
//    File Base:   ModuleSession_ProxyRule
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     负载转发规则处理
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszIPAddr[128];
}SESSION_PROXYRULE;

class CModuleSession_ProxyRule
{
public:
	CModuleSession_ProxyRule();
	~CModuleSession_ProxyRule();
public:
	bool ModuleSession_ProxyRule_Insert(LPCXSTR lpszIPAddr);
	bool ModuleSession_ProxyRule_Delete(LPCXSTR lpszIPAddr);
	bool ModuleSession_ProxyRule_Set(LPCXSTR lpszIPAddr, LPCXSTR lpszUseAddr, bool bAdd = true);
	bool ModuleSession_ProxyRule_GetCount(LPCXSTR lpszIPAddr, int* pInt_Count);
	bool ModuleSession_ProxyRule_GetList(SESSION_IPCONUT*** pppSt_IPCount, int* pInt_Count);
private:
	std::shared_mutex st_Locker;
private:
	std::unordered_map<xstring, std::list<SESSION_PROXYRULE> > stl_MapProxyRule;
};