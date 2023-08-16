#pragma once
/********************************************************************
//    Created:     2022/06/08  09:57:34
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward\ModuleSession_Forward.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward
//    File Base:   ModuleSession_Forward
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     会话转发协议
//    History:
*********************************************************************/

class CModuleSession_Forward
{
public:
	CModuleSession_Forward();
	~CModuleSession_Forward();
public:
	bool ModuleSession_Forward_Insert(LPCXSTR lpszAddr, XENGINE_PROTOCOL_USERAUTH* pSt_UserAuth);
	bool ModuleSession_Forward_List(SESSION_FORWARD*** pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr = NULL);
	bool ModuleSession_Forward_Bind(LPCXSTR lpszSrcAddr, LPCXSTR lpszDstAddr);
	bool ModuleSession_Forward_UNBind(LPCXSTR lpszSrcAddr, LPCXSTR lpszDstAddr);
	bool ModuleSession_Forward_Delete(LPCXSTR lpszAddr, XCHAR* ptszDstAddr);
	bool ModuleSession_Forward_Get(LPCXSTR lpszAddr, XCHAR* ptszDstAddr);
private:
	shared_mutex st_Locker;
private:
	unordered_map<tstring, SESSION_FORWARD> stl_MapSession;
};