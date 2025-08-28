#pragma once
/********************************************************************
//    Created:     2024/09/06  13:48:48
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Socks\ModuleSession_Socks.h
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\ModuleSession_Socks
//    File Base:   ModuleSession_Socks
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     代理服务器SOCK5
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                SOCKS代理服务协议类结构体定义
//用户信息
typedef struct 
{
	XCHAR tszClientAddr[128];           //代理客户端地址
	XNETHANDLE xhClient;                //客户端句柄
	ENUM_PROXY_SESSION_CLIENT_STATUS enProxyStatus;             //客户端状态
}RFCPROTOCOL_SOCKS5CLIENT, * LPRFCPROTOCOL_SOCKS5CLIENT;


class CModuleSession_Socks
{
public:
	CModuleSession_Socks();
	~CModuleSession_Socks();
public:
	bool ModuleSession_Socks_Create(LPCXSTR lpszClientID);
	bool ModuleSession_Socks_Delete(LPCXSTR lpszClientID);
	bool ModuleSession_Socks_SetInfo(LPCXSTR lpszClientID, XNETHANDLE xhClient, LPCXSTR lpszClientAddr);
	bool ModuleSession_Socks_GetInfo(LPCXSTR lpszClientID, XNETHANDLE *pxhClient);
	bool ModuleSession_Socks_GetAddrForHandle(XNETHANDLE xhClient, XCHAR *ptszClientAddr);
	bool ModuleSession_Socks_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS* penSocks);
	bool ModuleSession_Socks_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS enStatus);
	bool ModuleSession_Socks_List(XCHAR*** ppptszClientList, int* pInt_ListCount);
private:
	shared_mutex st_Locker;             
private:
	unordered_map<xstring, RFCPROTOCOL_SOCKS5CLIENT*> stl_MapClients;
};
