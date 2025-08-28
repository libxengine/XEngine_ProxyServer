#include "pch.h"
#include "ModuleSession_Forward/ModuleSession_Forward.h"
#include "ModuleSession_Socks/ModuleSession_Socks.h"
#include "ModuleSession_Tunnel/ModuleSession_Tunnel.h"
#include "ModuleSession_Proxy/ModuleSession_Proxy.h"
#include "ModuleSession_Proxy/ModuleSession_ProxyRule.h"
/********************************************************************
//    Created:     2022/06/08  10:10:52
//    File Name:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession\pch.cpp
//    File Path:   D:\XEngine_ProxyServer\XEngine_Source\XEngine_ModuleSession
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Session_IsErrorOccur = false;
XLONG Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleSession_Forward m_Forward;
CModuleSession_Socks m_Socks;
CModuleSession_Tunnel m_Tunnel;
CModuleSession_Proxy m_Proxy;
CModuleSession_ProxyRule m_ProxyRule;
//////////////////////////////////////////////////////////////////////////
//                        导出函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleSession_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Session_dwErrorCode;
}
/************************************************************************/
/*                        转发导出函数                                  */
/************************************************************************/
extern "C" bool ModuleSession_Forward_Insert(LPCXSTR lpszAddr, XENGINE_PROTOCOL_USERAUTH * pSt_UserAuth)
{
	return m_Forward.ModuleSession_Forward_Insert(lpszAddr, pSt_UserAuth);
}
extern "C" bool ModuleSession_Forward_List(SESSION_FORWARD * **pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr)
{
	return m_Forward.ModuleSession_Forward_List(pppSt_ListUser, pInt_Count, lpszAddr);
}
extern "C" bool ModuleSession_Forward_BindNamed(LPCXSTR lpszSrcAddr, LPCXSTR lpszDstAddr)
{
	return m_Forward.ModuleSession_Forward_BindNamed(lpszSrcAddr, lpszDstAddr);
}
extern "C" bool ModuleSession_Forward_BindAnony(LPCXSTR lpszSrcAddr, LPCXSTR lpszDstAddr, XNETHANDLE xhClient)
{
	return m_Forward.ModuleSession_Forward_BindAnony(lpszSrcAddr, lpszDstAddr, xhClient);
}
extern "C" bool ModuleSession_Forward_Delete(LPCXSTR lpszAddr, XCHAR * ptszDstAddr)
{
	return m_Forward.ModuleSession_Forward_Delete(lpszAddr, ptszDstAddr);
}
extern "C" bool ModuleSession_Forward_Get(LPCXSTR lpszAddr, SESSION_FORWARD* pSt_ForwardClinet)
{
	return m_Forward.ModuleSession_Forward_Get(lpszAddr, pSt_ForwardClinet);
}
/************************************************************************/
/*                     SOCK代理服务器导出函数                           */
/************************************************************************/
extern "C" bool ModuleSession_Socks_Create(LPCXSTR lpszClientID)
{
	return m_Socks.ModuleSession_Socks_Create(lpszClientID);
}
extern "C" bool ModuleSession_Socks_Delete(LPCXSTR lpszClientID)
{
	return m_Socks.ModuleSession_Socks_Delete(lpszClientID);
}
extern "C" bool ModuleSession_Socks_SetInfo(LPCXSTR lpszClientID, XNETHANDLE xhClient, LPCXSTR lpszClientAddr)
{
	return m_Socks.ModuleSession_Socks_SetInfo(lpszClientID, xhClient, lpszClientAddr);
}
extern "C" bool ModuleSession_Socks_GetInfo(LPCXSTR lpszClientID, XNETHANDLE* pxhClient)
{
	return m_Socks.ModuleSession_Socks_GetInfo(lpszClientID, pxhClient);
}
extern "C" bool ModuleSession_Socks_GetAddrForHandle(XNETHANDLE xhClient, XCHAR* ptszClientAddr)
{
	return m_Socks.ModuleSession_Socks_GetAddrForHandle(xhClient, ptszClientAddr);
}
extern "C" bool ModuleSession_Socks_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS* penSocks)
{
	return m_Socks.ModuleSession_Socks_GetStatus(lpszClientID, penSocks);
}
extern "C" bool ModuleSession_Socks_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS enStatus)
{
	return m_Socks.ModuleSession_Socks_SetStatus(lpszClientID, enStatus);
}
extern "C" bool ModuleSession_Socks_List(XCHAR*** ppptszClientList, int* pInt_ListCount)
{
	return m_Socks.ModuleSession_Socks_List(ppptszClientList, pInt_ListCount);
}
/************************************************************************/
/*                     隧道代理服务                                     */
/************************************************************************/
extern "C" bool ModuleSession_Tunnel_Create(LPCXSTR lpszClientID)
{
	return m_Tunnel.ModuleSession_Tunnel_Create(lpszClientID);
}
extern "C" bool ModuleSession_Tunnel_Delete(LPCXSTR lpszClientID)
{
	return m_Tunnel.ModuleSession_Tunnel_Delete(lpszClientID);
}
extern "C" bool ModuleSession_Tunnel_SetInfo(LPCXSTR lpszClientID, XNETHANDLE xhClient, LPCXSTR lpszClientAddr)
{
	return m_Tunnel.ModuleSession_Tunnel_SetInfo(lpszClientID, xhClient, lpszClientAddr);
}
extern "C" bool ModuleSession_Tunnel_GetInfo(LPCXSTR lpszClientID, XNETHANDLE* pxhClient)
{
	return m_Tunnel.ModuleSession_Tunnel_GetInfo(lpszClientID, pxhClient);
}
extern "C" bool ModuleSession_Tunnel_GetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS* penStatus)
{
	return m_Tunnel.ModuleSession_Tunnel_GetStatus(lpszClientID, penStatus);
}
extern "C" bool ModuleSession_Tunnel_SetStatus(LPCXSTR lpszClientID, ENUM_PROXY_SESSION_CLIENT_STATUS enStatus)
{
	return m_Tunnel.ModuleSession_Tunnel_SetStatus(lpszClientID, enStatus);
}
extern "C" bool ModuleSession_Tunnel_GetAddrForHandle(XNETHANDLE xhClient, XCHAR* ptszClientAddr)
{
	return m_Tunnel.ModuleSession_Tunnel_GetAddrForHandle(xhClient, ptszClientAddr);
}
extern "C" bool ModuleSession_Tunnel_Packet(LPCXSTR lpszClientID, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszMSGBuffer, int* pInt_MSGLen)
{
	return m_Tunnel.ModuleSession_Tunnel_Packet(lpszClientID, lpszMsgBuffer, nMsgLen, ptszMSGBuffer, pInt_MSGLen);
}
extern "C" bool ModuleSession_Tunnel_List(XCHAR*** ppptszClientList, int* pInt_ListCount)
{
	return m_Tunnel.ModuleSession_Tunnel_List(ppptszClientList, pInt_ListCount);
}
/************************************************************************/
/*                     全转发代理服务                                   */
/************************************************************************/
extern "C" bool ModuleSession_Proxy_Insert(LPCXSTR lpszSrcIPAddr, LPCXSTR lpszDstIPAddr, XNETHANDLE xhClient)
{
	return m_Proxy.ModuleSession_Proxy_Insert(lpszSrcIPAddr, lpszDstIPAddr, xhClient);
}
extern "C" bool ModuleSession_Proxy_GetForAddr(LPCXSTR lpszIPAddr, SESSION_FORWARD* pSt_ProxyInfo)
{
	return m_Proxy.ModuleSession_Proxy_GetForAddr(lpszIPAddr, pSt_ProxyInfo);
}
extern "C" bool ModuleSession_Proxy_GetForToken(XNETHANDLE xhToken, SESSION_FORWARD* pSt_ProxyInfo)
{
	return m_Proxy.ModuleSession_Proxy_GetForToken(xhToken, pSt_ProxyInfo);
}
extern "C" bool ModuleSession_Proxy_List(SESSION_FORWARD*** pppSt_ListUser, int* pInt_Count, LPCXSTR lpszAddr)
{
	return m_Proxy.ModuleSession_Proxy_List(pppSt_ListUser, pInt_Count, lpszAddr);
}
extern "C" bool ModuleSession_Proxy_Delete(LPCXSTR lpszIPAddr)
{
	return m_Proxy.ModuleSession_Proxy_Delete(lpszIPAddr);
}
extern "C" bool ModuleSession_ProxyRule_Insert(LPCXSTR lpszIPAddr)
{
	return m_ProxyRule.ModuleSession_ProxyRule_Insert(lpszIPAddr);
}
extern "C" bool ModuleSession_ProxyRule_Delete(LPCXSTR lpszIPAddr)
{
	return m_ProxyRule.ModuleSession_ProxyRule_Delete(lpszIPAddr);
}
extern "C" bool ModuleSession_ProxyRule_Set(LPCXSTR lpszIPAddr, LPCXSTR lpszUseAddr, bool bAdd)
{
	return m_ProxyRule.ModuleSession_ProxyRule_Set(lpszIPAddr, lpszUseAddr, bAdd);
}
extern "C" bool ModuleSession_ProxyRule_GetCount(LPCXSTR lpszIPAddr, int* pInt_Count)
{
	return m_ProxyRule.ModuleSession_ProxyRule_GetCount(lpszIPAddr, pInt_Count);
}
extern "C" bool ModuleSession_ProxyRule_GetList(SESSION_IPCONUT*** pppSt_IPCount, int* pInt_Count)
{
	return m_ProxyRule.ModuleSession_ProxyRule_GetList(pppSt_IPCount, pInt_Count);
}