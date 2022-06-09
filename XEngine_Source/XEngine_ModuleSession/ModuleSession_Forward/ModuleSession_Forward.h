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
typedef struct
{
	TCHAR tszSrcAddr[128];
	TCHAR tszDstAddr[128];
	BOOL bForward;
}SESSION_FORWARD, * LPSESSION_FORWARD;

class CModuleSession_Forward
{
public:
	CModuleSession_Forward();
	~CModuleSession_Forward();
public:
	BOOL ModuleSession_Forward_Insert(LPCTSTR lpszAddr);
	BOOL ModuleSession_Forward_List(TCHAR*** ppptszListAddr, int* pInt_Count, LPCTSTR lpszAddr = NULL);
	BOOL ModuleSession_Forward_Bind(LPCTSTR lpszSrcAddr, LPCTSTR lpszDstAddr);
	BOOL ModuleSession_Forward_Delete(LPCTSTR lpszAddr, TCHAR* ptszDstAddr);
	BOOL ModuleSession_Forward_Get(LPCTSTR lpszAddr, TCHAR* ptszDstAddr);
private:
	shared_mutex st_Locker;
private:
	unordered_map<tstring, SESSION_FORWARD> stl_MapSession;
};