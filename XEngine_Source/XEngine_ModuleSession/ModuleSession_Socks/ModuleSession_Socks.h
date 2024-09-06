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
	XCHAR tszCMDBuffer[128];            //地址信息
	int nCMDLen;                        //CMD地址信息大小
	int nCTMLen;                        //自定义大小
	ENUM_PROXY_SESSION_SOCKS_STATUS enProxyStatus;             //客户端状态
	XPVOID lParam;
}RFCPROTOCOL_SOCKS5CLIENT, * LPRFCPROTOCOL_SOCKS5CLIENT;


class CModuleSession_Socks
{
public:
	CModuleSession_Socks();
	~CModuleSession_Socks();
public:
	bool ModuleSession_Socks_Create(LPCXSTR lpszClientID);
	bool ModuleSession_Socks_Delete(LPCXSTR lpszClientID);
	bool ModuleSession_Socks_SetInfo(LPCXSTR lpszClientID, XPVOID lParam, int nLen);
	bool ModuleSession_Socks_GetInfo(LPCXSTR lpszClientID, XPVOID lParam, int* pInt_Len = NULL);
	bool ModuleSession_Socks_GetList(XPPPMEM xpppMem, int* pInt_Count, int nSize);
	bool ModuleSession_Socks_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS* penSocks);
	bool ModuleSession_Socks_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_SOCKS_STATUS enStatus);
	bool ModuleSession_Socks_List(XCHAR*** ppptszClientList, int* pInt_ListCount);
private:
	shared_mutex st_Locker;             
private:
	unordered_map<tstring, RFCPROTOCOL_SOCKS5CLIENT*> stl_MapClients;
};
